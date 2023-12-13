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
/** @addtogroup ApplicationExamples Application Examples
 * @{
 */

/** @defgroup 3PhGridTie 3-Phase Grid Tie Inverter
 * @brief Describes the three-phase grid tie inverter with boost converter implementation for the PEController
 * @details
 * This example controls a three phase inverter module as well as a boost converter for PEController.
 * At the start of the application the boost converter starts regulation to provide a stabilized DC-Link voltage.
 * After stabilizing the DC-Link voltage, the relay turns on for connection with the grid.
 * The control algorithm implements a PLL algorithm to detect the phase of the grid voltages.
 * After estimating the exact phase angle the inverter is enabled and starts injecting power in the grid.
 * Make sure to provide the DC Link before connecting to the grid to avoid failure due to in-rush current.
 * Initializes the algorithm using @ref GridTieControl_Init().
 * Once initialized constantly polls using @ref GridTieControl_Loop().
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "control_library.h"
#include "grid_tie_config.h"
#include "error_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define PWM_PERIOD_s				(1.f / PWM_FREQ_Hz)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup GRIDTIE_Exported_Structures Structures
  * @{
  */
/**
 * @brief Defines the parameters for grid-tie controller
 */
typedef struct
{
	bool isBoostEnabled;					/**< @brief Current state of the boost controller */
	bool isRelayOn;							/**< @brief Current state of the control relays */
	bool isInverterEnabled;					/**< @brief Current state of the grid tie inverter */
	float vdc;								/**< @brief DC link voltage */
	float iRef;								/**< @brief Constant reference current for output */
	LIB_COOR_ALL_t vCoor;					/**< @brief Phase voltage coordinates */
	LIB_COOR_ALL_t iCoor;					/**< @brief Phase current coordinates */
	pi_compensator_t iQComp;				/**< @brief Compensator for Q value of DQ transform for grid currents */
	pi_compensator_t iDComp;				/**< @brief Compensator for D value of DQ transform for grid currents */
	pll_lock_t pll;							/**< @brief PLL structure used by the grid tie controller */
	inverter3Ph_config_t inverterConfig;	/**< @brief Output inverter configuration */
	independent_pwm_config_t boostConfig[BOOST_COUNT];	/**< @brief Boost configuration for developing DC link */
	uint16_t boostDiodePin[BOOST_COUNT];				/**< @brief Pin no of the switches acting as diode */
	float VbstSet;							/**< @brief Boost set point voltage */
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
/**
 * @brief Enable/Disable the boost converter for the grid tie controller
 * @param gridTie Pointer to the grid tie structure
 * @param en <c>true</c> if needs to be enabled else <c>false</c>
 * @return <c>ERR_OK</c> if command successfully run else some other error
 */
extern device_err_t GridTie_EnableBoost(grid_tie_t* gridTie, bool en);
/**
 * @brief Enable/Disable the inverter for the grid tie controller
 * @param gridTie Pointer to the grid tie structure
 * @param en <c>true</c> if needs to be enabled else <c>false</c>
 * @return <c>ERR_OK</c> if command successfully run else some other error
 */
extern device_err_t GridTie_EnableInverter(grid_tie_t* gridTie, bool en);
/********************************************************************************
 * Code
 *******************************************************************************/


/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
