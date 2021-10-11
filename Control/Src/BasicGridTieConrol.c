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
/********************************************************************************
 * Defines
 *******************************************************************************/

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
static void ConnectGrid()
{
	Error_Handler();  // --todo--
}

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
			ConnectGrid();
		Transform_abc_alphaBeta(&pll->coords.abc, &pll->coords.alphaBeta);
		GetSVPWM_FromVref(&pll->coords.alphaBeta, gridTie->vdc, duties);
	}
	else
	{
		duties[0] = 0; duties[1] = 0; duties[2] = 0;
	}
}


/* EOF */
