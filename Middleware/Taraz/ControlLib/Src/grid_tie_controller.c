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
#include "grid_tie_controller.h"
#include "pecontroller_digital_out.h"
#include <string.h>
/********************************************************************************
 * Defines
 *******************************************************************************/
#define GRID_RELAY_IO					(15)
#define GRID_FREQ						(50)
#define L_OUT							(.0025f)
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
	// get pointer to the coordinates
	pll_lock_t* pll = &gridTie->pll;
	LIB_COOR_ALL_t* vCoor = &gridTie->vCoor;
	LIB_COOR_ALL_t* iCoor = &gridTie->iCoor;
	// the PLL voltages should point to the main voltages
	if (pll->coords == NULL)
		pll->coords = vCoor;

	// lock the voltage phase, if locked turn on relay
	if (Pll_LockGrid(pll) == PLL_LOCKED)
	{
		// turn on relay if grid was not locked previously
		if(pll->prevStatus != PLL_LOCKED)
			BSP_Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_SET);

		// As only real power is needed so voltage and current should be completely in phase
		memcpy(&iCoor->trigno, &vCoor->trigno, sizeof(vCoor->trigno));

		// Transform the current measurements to DQ coordinates
		Transform_abc_dq0(&iCoor->abc, &iCoor->dq0, &iCoor->trigno, SRC_ABC, PARK_SINE);

		// Apply PI control to both DQ coordinates gridTie->dCompensator.dt
		iCoor->dq0.d = PI_Compensate(&gridTie->dCompensator, gridTie->iRef - iCoor->dq0.d) + vCoor->dq0.d
				- TWO_PI * GRID_FREQ * L_OUT * iCoor->dq0.q;

		iCoor->dq0.q = PI_Compensate(&gridTie->qCompensator, 0             - iCoor->dq0.q) + vCoor->dq0.q
				+ TWO_PI * GRID_FREQ * L_OUT * iCoor->dq0.d;

		//iCoor->dq0.zero = 0;

		// get ABCD coordinates from DQ coordinates
		LIB_3COOR_ABC_t abc;
		iCoor->dq0.d *= (1.f/(gridTie->vdc));
		iCoor->dq0.q *= (1.f/(gridTie->vdc));
		Transform_abc_dq0(&abc, &iCoor->dq0, &iCoor->trigno, SRC_DQ0, PARK_SINE);

		// get duty cycles based on ABC coordinates
/*
		duties[0] = (abc.a / gridTie->vdc) + .5f;
		duties[1] = (abc.b / gridTie->vdc) + .5f;
		duties[2] = (abc.c / gridTie->vdc) + .5f;
*/

		duties[0] = (abc.a + 1) * .5f;
		duties[1] = (abc.b + 1) * .5f;
		duties[2] = (abc.c + 1) * .5f;
	}
	else
	{
		if(pll->prevStatus == PLL_LOCKED)
			BSP_Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_RESET);
		duties[0] = 0.5f; duties[1] = 0.5f; duties[2] = 0.5f;
	}
}

#pragma GCC pop_options
/* EOF */
