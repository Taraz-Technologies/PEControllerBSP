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

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "BasicGridTieControl.h"
#include "DoutPort.h"
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
 * @brief Evaluates the Duty Cycle Values for the Basic Grid Tie
 *
 * @param *gridTie Pointer to the parameter structure
 * @param *duties resultant duty cycles for the inverter (Range 0 - 1)
 */
void BasicGridTieControl_GetDuties(basic_grid_tie_t* gridTie, float* duties)
{
	// copy the grid voltages
	memcpy(&gridTie->pll.coords.abc, &gridTie->v1, 12);
	pll_lock_t* pll = &gridTie->pll;

	if (Pll_LockGrid(pll) == PLL_LOCKED)
	{
		if(pll->prevStatus == PLL_INVALID)
			Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_SET);
		Transform_alphaBeta0_dq0(&pll->coords.alBe0, &pll->coords.dq0, &pll->coords.sinCosAngle, SRC_DQ0, PARK_SINE);
		GetSVPWM_FromVref(&pll->coords.alBe0, gridTie->vdc, duties);
	}
	else
	{
		if(pll->prevStatus == PLL_LOCKED)
			Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_RESET);
		duties[0] = 0; duties[1] = 0; duties[2] = 0;
	}
}


/* EOF */
