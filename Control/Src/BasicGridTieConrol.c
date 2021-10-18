/**
 ********************************************************************************
 * @file    	BasicGridTieConrol.c
 * @author 		Waqas Ehsan Butt
 * @date    	Oct 11, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */
//#pragma GCC push_options
//#pragma GCC optimize ("O3")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "BasicGridTieControl.h"
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

/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
LIB_3COOR_DQ0_t dq0 = {0};
/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Evaluates the Duty Cycle Values for the Basic Grid Tie
 *
 * @param *gridTie Pointer to the parameter structure
 * @param *duties resultant duty cycles for the inverter (Range 0 - 1)
 */
void BasicGridTieControl_GetDuties(basic_grid_tie_t* gridTie, float* duties)
{
	pll_lock_t* pll = &gridTie->pll;
	if (pll->coords == NULL)
		pll->coords = &gridTie->vCoor;

	if (Pll_LockGrid(pll) == PLL_LOCKED)
	{
		if(pll->prevStatus != PLL_LOCKED)
			Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_SET);
		//Transform_alphaBeta0_dq0(&gridTie->vCoor.alBe0, &gridTie->vCoor.dq0, &gridTie->vCoor.sinCosAngle, SRC_DQ0, PARK_SINE);

		Transform_abc_dq0_wt0(&gridTie->vCoor.abc, &dq0, SRC_ABC, PARK_SINE);
#if PWM_TECH == SVPWM
		GetSVPWM_FromVrefDQ(&dq0, gridTie->vdc, duties);
		//GetSVPWM_FromVref(&gridTie->vCoor.alBe0, gridTie->vdc, duties);
#else
		GenerateSPWM(gridTie->vCoor.sinCosAngle.wt, 0.5f, duties);
#endif
	}
	else
	{
		if(pll->prevStatus == PLL_LOCKED)
			Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_RESET);
		duties[0] = 0.5f; duties[1] = 0.5f; duties[2] = 0.5f;
	}
}

//#pragma GCC pop_options
/* EOF */
