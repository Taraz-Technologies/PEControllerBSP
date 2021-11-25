/**
 ********************************************************************************
 * @file 		GridTieControl.h
 * @author 		Waqas Ehsan Butt
 * @date 		Oct 13, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */

#ifndef GRIDTIECONTROL_H_
#define GRIDTIECONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "ControlLib.h"
#include "Pll.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
/**
 * @brief Data structure used by the Grid Tie Control
 */
typedef struct
{
	/*! @brief Voltage level of DC Link */
	float vdc;
	/*! @brief Reference current to be used by the Grid Tie Control */
	float iRef;
	/** @brief Coordinates for the Phase to Phase Voltages */
	LIB_COOR_ALL_t vCoor;
	/** @brief Coordinates for the Phase Currents */
	LIB_COOR_ALL_t iCoor;
	/** @brief DQ compensator for Q Value */
	pi_compensator_t qCompensator;
	/** @brief DQ compensator for D Value */
	pi_compensator_t dCompensator;
	/*! @brief PLL structure used by the Grid Tie Control */
	pll_lock_t pll;
} grid_tie_t;
/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief Evaluates the Duty Cycle Values for the Grid Tie Control
 *
 * @param *gridTie Pointer to the parameter structure
 * @param *duties resultant duty cycles for the inverter (Range 0 - 1)
 */
void GridTieControl_GetDuties(grid_tie_t* gridTie, float* duties);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
