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
 * @details Use the configurations to control the PWM for the VFDs.
 * Some control for the VFDs are provided on the external touch screen.
 * While the limits and default values may be controlled from here.
 * Make sure to set these values to proper limits to avoid generation of invalid output.
 * @note <b>This application is only supported for the 6-Phase configuration currently.</b>
  * @{
  */
#if PECONTROLLER_CONFIG != PLB_6PH && PECONTROLLER_CONFIG != PEC_CUSTOM
#error("This example is only supported for 6-Phase Inverter")
#endif
/**
 * @brief PWM frequency in Hz.
 * @note Minimum value is 5000
 */
#define PWM_FREQ_Hz						(40000)
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
 * @note Using multiple legs as boost converter reduces the load for each
 * leg making it possible to achieve higher output power.
 */
#define BOOST_COUNT						(3)
/**
 * @brief Set point voltage for boost converter
 * @note This should be kept as high as possible to avoid clamping at the top of the output current wave
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
 * @note This value should be low if working on high input/PV voltage
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
#define RELAY_TURN_ON_VDC				(620.f)
/**
 * @brief Defines the turn off condition for the relay
 */
#define RELAY_TURN_OFF_VDC				(500.f)
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
 * @brief The frequency of the grid 1st time the application is run. Current value can be updated from screen
 */
#define DEFAULT_GRID_FREQ				(50)
/**
 * @brief The output inductance of the inverter 1st time the application is run. Current value can be updated from screen
 */
#define DEFAULT_LOUT_mH					(1.f)
/**
 * @brief The phase voltage of the inverter 1st time the application is run. Current value can be updated from screen
 */
#define DEFAULT_GRID_VOLTAGE			(220.f)
/**
 * @brief The output current of the inverter 1st time the application is run. Current value can be updated from screen
 */
#define DEFAULT_CURRENT_INJ				(3.f)
/**
 * @brief The maximum allowed value of the grid frequency
 */
#define MAX_GRID_FREQ					(65)
/**
 * @brief The minimum allowed value of the grid frequency
 */
#define MIN_GRID_FREQ					(45)
/**
 * @brief The maximum allowed value of the output inductance
 */
#define MAX_LOUT_mH						(10.f)
/**
 * @brief The minimum allowed value of the output inductance
 */
#define MIN_LOUT_mH						(0.5f)
/**
 * @brief The maximum allowed value of grid phase voltage
 */
#define MAX_GRID_VOLTAGE				(250.f)
/**
 * @brief The minimum allowed value of grid phase voltage
 */
#define MIN_GRID_VOLTAGE				(40.f)
/**
 * @brief The maximum allowed value of output current to be injected
 */
#define MAX_CURRENT_INJ					(20.f)
/**
 * @brief The minimum allowed value of output current to be injected
 */
#define MIN_CURRENT_INJ					(.1f)
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
