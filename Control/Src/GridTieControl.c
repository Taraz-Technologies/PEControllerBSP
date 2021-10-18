/**
 ********************************************************************************
 * @file    	GridTieControl.c
 * @author 		Waqas Ehsan Butt
 * @date    	Oct 13, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */
#pragma GCC push_options
#pragma GCC optimize ("O3")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "GridTieControl.h"
#include "DoutPort.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define GRID_RELAY_IO					(15)

#define SPWM							(0)
#define SVPWM							(1)
#define PWM_TECH						(SVPWM)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static pi_data_t qPI = { .Integral = 0, .Kp = 0.1f, .Ki = 5.f, .dt = 0.00004f };
static pi_data_t dPI = { .Integral = 0, .Kp = 0.1f, .Ki = 5.f, .dt = 0.00004f };
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Evaluates the Duty Cycle Values using the Grid Tie Control
 *
 * @param *gridTie Pointer to the parameter structure
 * @param *duties resultant duty cycles for the inverter (Range 0 - 1)
 */
void GridTieControl_GetDuties(grid_tie_t* gridTie, float* duties)
{
	pll_lock_t* pll = &gridTie->pll;
	LIB_COOR_ALL_t* vCoor = &gridTie->vCoor;
	LIB_COOR_ALL_t* iCoor = &gridTie->iCoor;
	if (pll->coords == NULL)
		pll->coords = vCoor;

	if (Pll_LockGrid(pll) == PLL_LOCKED)
	{
		if(pll->prevStatus != PLL_LOCKED)
			Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_SET);

		iCoor->sinCosAngle.wt = vCoor->sinCosAngle.wt;
		iCoor->sinCosAngle.wt = AdjustTheta(iCoor->sinCosAngle.wt);
		Transform_theta_sincos(&iCoor->sinCosAngle);

		// Get DQ0
		Transform_abc_dq0(&iCoor->abc, &iCoor->dq0, &iCoor->sinCosAngle, SRC_ABC, PARK_SINE);

		// Apply PIs
		iCoor->dq0.q = EvaluatePI(&qPI, -iCoor->dq0.q);
		iCoor->dq0.d = EvaluatePI(&dPI, gridTie->iRef -iCoor->dq0.d);

		// Get ALBE0
		Transform_alphaBeta0_dq0(&iCoor->alBe0, &iCoor->dq0, &iCoor->sinCosAngle, SRC_DQ0, PARK_SINE);

#if PWM_TECH == SVPWM
		GetSVPWM_FromAlBe(&gridTie->iCoor.alBe0, duties);
#else
		GenerateSPWM(gridTie->iCoor.sinCosAngle.wt, 0.5f, duties);
#endif
	}
	else
	{
		if(pll->prevStatus == PLL_LOCKED)
			Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_RESET);
		duties[0] = 0.5f; duties[1] = 0.5f; duties[2] = 0.5f;
	}
}

#pragma GCC pop_options
/* EOF */
