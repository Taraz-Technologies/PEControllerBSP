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
#include "Control.h"
#include "pll.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
/**
 * @brief Data structure containing all grid tie related measurements
 */
typedef struct
{
	/*! @brief Voltage level of phase V */
	float v1;
	/*! @brief Voltage level of phase V */
	float v2;
	/*! @brief Voltage level of phase W */
	float v3;
	/*! @brief Voltage level of DC Link */
	float vdc;
	/*! @brief Current level of phase V */
	float i1;
	/*! @brief Current level of phase V */
	float i2;
	/*! @brief Current level of phase W */
	float i3;
} grid_tie_measurements;
/**
 * @brief Data structure used by the Grid Tie Control
 */
typedef struct
{
	/*! @brief Measurements required by the Grid Tie Control */
	grid_tie_measurements measures;
	/*! @brief Reference current to be used by the Grid Tie Control */
	float iRef;
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

/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
