/**
 ********************************************************************************
 * @file 		BasicGridTieControl.h
 * @author 		Waqas Ehsan Butt
 * @date 		Oct 11, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */

#ifndef INC_BASICGRIDTIECONTROL_H_
#define INC_BASICGRIDTIECONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "GeneralHeader.h"
#include "Control.h"
#include "Pll.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/**
 * @brief Data structure used by the basic grid tie controller
 */
typedef struct
{
	/*! @brief Voltage level of DC Link */
	float vdc;
	/** @brief Coordinates for the Phase to Phase Voltages */
	LIB_COOR_ALL_t vCoor;
	/*! @brief PLL structure used by the basic grid tie converter */
	pll_lock_t pll;
} basic_grid_tie_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief Evaluates the Duty Cycle Values for the Basic Grid Tie
 *
 * @param *gridTie Pointer to the parameter structure
 * @param *duties resultant duty cycles for the inverter (Range 0 - 1)
 */
void BasicGridTieControl_GetDuties(basic_grid_tie_t* gridTie, float* duties);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
