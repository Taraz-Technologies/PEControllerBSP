/**
 ********************************************************************************
 * @file 		grid_tie_controller.h
 * @author 		Waqas Ehsan Butt
 * @date 		December 7, 2021
 *
 * @brief    
 ********************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 Taraz Technologies Pvt. Ltd.</center></h2>
 * <h3><center>All rights reserved.</center></h3>
 *
 * <center>This software component is licensed by Taraz Technologies under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the License. You may obtain 
 * a copy of the License at:
 *                        www.opensource.org/licenses/BSD-3-Clause</center>
 *
 ********************************************************************************
 */

#ifndef GRID_TIE_CONTROLLER_H_
#define GRID_TIE_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "control_library.h"
#include "grid_tie_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup GRIDTIE_Exported_Typedefs Type Definitions
  * @{
  */
/**
 * @brief Defines the states for the grid tie controller
 */
typedef enum
{
	GRID_TIE_INACTIVE,      /**< Grid tie non functional */
	GRID_TIE_ACTIVE,        /**< Grid tie functional */
} grid_tie_state_t;
/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup GRIDTIE_Exported_Structures Structures
  * @{
  */
/**
 * @brief Defines the parameters for grid tie controller
 */
typedef struct
{
	float vdc;								/**< @brief DC link voltage */
	float vpv;								/**< @brief Input voltage for the boost inductor */
	float idc;								/**< @brief Input current for the DC link */
	float iRef;								/**< @brief Constant reference current for output */
	float iLRef;								/**< @brief Constant reference current for output */
	float boostD0;							/**< @brief Duty cycle applied in the last cycle */
	LIB_COOR_ALL_t vCoor;					/**< @brief Phase voltage coordinates */
	LIB_COOR_ALL_t iCoor;					/**< @brief Phase current coordinates */
	pi_compensator_t iQComp;				/**< @brief Compensator for Q value of DQ transform for grid currents */
	pi_compensator_t iDComp;				/**< @brief Compensator for D value of DQ transform for grid currents */
	pll_lock_t pll;							/**< @brief PLL structure used by the grid tie controller */
	inverter3Ph_config_t inverterConfig;	/**< @brief Output inverter configuration */
	independent_pwm_config_t boostConfig;	/**< @brief Boost configuration for developing DC link */
	uint16_t boostDiodePin;					/**< @brief Pin no of the switch acting as diode */
	grid_tie_state_t state;					/**< @brief Current state of the grid tie controller */
	float tempIndex;						/**< @brief Temporary variable */
} grid_tie_t;
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup GRIDTIE_Exported_Functions Functions
  * @{
  */
/**
 * @brief Initialize the grid tie controller
 * @param gridTie Pointer to the grid tie structure
 * @param pwmResetCallback Function callback issued after each PWM completion
 */
extern void GridTieControl_Init(grid_tie_t* gridTie, PWMResetCallback pwmResetCallback);
/**
 * @brief This function computes new duty cycles for the inverter and boost in each cycle
 * @param gridTie Pointer to the grid tie structure
 */
extern void GridTieControl_Loop(grid_tie_t* gridTie);
/********************************************************************************
 * Code
 *******************************************************************************/


/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
