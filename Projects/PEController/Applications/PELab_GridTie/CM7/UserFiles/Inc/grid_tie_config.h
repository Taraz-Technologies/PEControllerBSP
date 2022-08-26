/**
 ********************************************************************************
 * @file 		grid_tie_config.h
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

#ifndef GRID_TIE_CONFIG_H_
#define GRID_TIE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup ApplicationExamples Application Examples
 * @{
 */

/** @addtogroup 3PhGridTie
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup GRIDTIE_Exported_Macros Configurations
  * @{
  */
/**
 * @brief PWM Time Period in micro-seconds
 */
#define PWM_PERIOD_Us					(25)
/**
 * @brief Frequency of the grid in Hz
 */
#define GRID_FREQ						(50)
/**
 * @brief Series Output Inductance in Henry
 */
#define L_OUT							(.0025f)
/**
 * @brief Number of Boost Switches to control
 */
#define BOOST_COUNT						(3)
/**
 * @brief Set point voltage for boost converter
 */
#define BOOST_VSET						(720)
/**
 * @brief Value of Kp for boost voltage regulation
 */
#define KP_BOOST						(0.002f)
/**
 * @brief Value of Ki for boost voltage regulation
 */
#define KI_BOOST						(0.1f)
/**
 * @brief Value of Kp for PLL
 */
#define KP_PLL							(10)
/**
 * @brief Value of Ki for PLL
 */
#define KI_PLL							(.5f)
/**
 * @brief Value of Kp for current loop
 */
#define KP_I							(60)
/**
 * @brief Value of Ki for current loop
 */
#define KI_I							(1000)
/**
 * @brief Maximum duty cycle limit for the boost controller
 */
#define BOOST_DUTYCYCLE_MAX				(.95f)

#if PECONTROLLER_CONFIG == PLB_6PH
#if PELAB_VERSION > 3
/**
 * @brief Number of connected relays to grid
 * @note The relay IO should start from @ref GRID_RELAY_IO and use consecutive IOs after that
 */
#define GRID_RELAY_COUNT				(4)
/**
 * @brief First grid relay IO
 */
#define GRID_RELAY_IO					(13)
#endif
#endif

#ifndef GRID_RELAY_COUNT
/**
 * @brief Number of connected relays to grid
 * @note The relay IO should start from @ref GRID_RELAY_IO and use consecutive IOs after that
 */
#define GRID_RELAY_COUNT				(2)
#endif
#ifndef GRID_RELAY_IO
/**
 * @brief First grid relay IO
 */
#define GRID_RELAY_IO					(15)
#endif

/**
 * @brief Defines the turn on condition for the relay
 */
#define RELAY_TURN_ON_VBST				(620.f)
/**
 * @brief Defines the turn off condition for the relay
 */
#define RELAY_TURN_OFF_VBST				(530.f)
/**
 * @brief Dead time value in nano-seconds for the inverter
 */
#define INVERTER_DEADTIME_ns			(200)
/**
 * @brief Duty cycle balancing disabled for the inverter
 */
#define MIN_MAX_BALANCING_INVERTER		(false)
/**
 * @brief Inverter dead time subtraction mode selection. Can be selected from @ref duty_mode_t
 */
#define INVERTER_DUTY_MODE				OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH
/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/

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

/**
 * @}
 */
/**
 * @}
 */

#endif 
/* EOF */
