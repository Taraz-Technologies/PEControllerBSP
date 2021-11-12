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
#pragma GCC optimize ("-Ofast")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "GridTieControl.h"
#include "DoutPort.h"
#include <string.h>
/********************************************************************************
 * Defines
 *******************************************************************************/
#define GRID_RELAY_IO					(15)
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
		{
			Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_SET);
		}
		memcpy(&iCoor->sinCosAngle, &vCoor->sinCosAngle, sizeof(vCoor->sinCosAngle));

		Transform_abc_dq0(&iCoor->abc, &iCoor->dq0, &iCoor->sinCosAngle, SRC_ABC, PARK_SINE);
		Transform_abc_dq0(&vCoor->abc, &vCoor->dq0, &vCoor->sinCosAngle, SRC_ABC, PARK_SINE);

		iCoor->dq0.d = EvaluatePI(&gridTie->dCompensator, gridTie->iRef - iCoor->dq0.d) - TWO_PI * 50 * gridTie->dCompensator.dt * iCoor->dq0.q + vCoor->dq0.d;
		iCoor->dq0.q = EvaluatePI(&gridTie->qCompensator, 0             - iCoor->dq0.q) + TWO_PI * 50 * gridTie->dCompensator.dt * iCoor->dq0.d + vCoor->dq0.q;

		LIB_3COOR_ABC_t abc;
		Transform_abc_dq0(&abc, &iCoor->dq0, &iCoor->sinCosAngle, SRC_DQ0, PARK_SINE);

		duties[0] = (abc.a / gridTie->vdc) + .5f;
		duties[1] = (abc.b / gridTie->vdc) + .5f;
		duties[2] = (abc.c / gridTie->vdc) + .5f;
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
