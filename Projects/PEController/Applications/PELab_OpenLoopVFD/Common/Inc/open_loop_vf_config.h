/**
 ********************************************************************************
 * @file 		open_loop_vf_config.h
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

#ifndef OPEN_LOOP_VF_CONFIG_H_
#define OPEN_LOOP_VF_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup ApplicationExamples Application Examples
 * @{
 */

/** @addtogroup OpenLoopVf Open Loop V/f Example
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup OPENLOOPVF_Exported_Macros Configurations
 * @details Use the configurations to control the PWM for the VFDs.
 * Some control for the VFDs are provided on the external touch screen.
 * While the limits and default values may be controlled from here.
 * Make sure to set these values to proper limits to avoid generation of invalid output.
  * @{
  */
/**
 * @brief PWM frequency in Hz.
 * @note Minimum value is 5000
 */
#define PWM_FREQ_Hz						(25000)
/**
 * @brief Acceleration factor for increasing speed of the controller
 * @note The acceleration should be grater than 1
 */
#define ACCELERATION					(1.00001f)
/**
 * @brief Initial frequency used by the system
 * @note The initial frequency should be equal to or greater than 1
 */
#define INITIAL_FREQ					(1.f)
/**
 * @brief Dead time (ns) value to be used
 */
#define INVERTER_DEADTIME_ns			(200)
/**
 * @brief In inverters that have four legs per inverter that 4th leg will receive the same signal as the third leg, so they will both be interchangeable.
 */
#define HAS_DUPLICATE_SWITCH			(PECONTROLLER_CONFIG == PLB_TNPC || PECONTROLLER_CONFIG == PLB_MMC ? 1 : 0)
/**
 * @brief No of relays in the system
 */
#define RELAY_COUNT						(PECONTROLLER_CONFIG == PLB_TNPC || PECONTROLLER_CONFIG == PLB_MMC ? 0 : (PELAB_VERSION >= 4 ? 4 : 2))
/**
 * @brief No of switches per leg.
 * @note This figure depends on the topology. e.g. TNPC requires four switches per leg
 */
#define LEG_SWITCH_COUNT				(PECONTROLLER_CONFIG == PLB_TNPC ? 4 : 2)
/**
 * @brief No of switches per leg.
 * @note This figure depends on the topology. e.g. TNPC requires four switches per leg
 */
#define VFD_COUNT						(PECONTROLLER_CONFIG == PLB_6PH || PECONTROLLER_CONFIG == PEC_CUSTOM ? 2 : 1)
/**
 * @brief 1st pin of 1st VFD
 */
#define VFD1_PIN1						(1)
#if VFD_COUNT == 2
/**
 * @brief 1st pin of 2nd VFD
 */
#define VFD2_PIN1						(VFD1_PIN1 + (HAS_DUPLICATE_SWITCH ? LEG_SWITCH_COUNT * 4 : LEG_SWITCH_COUNT * 3))
#endif

/**
 * @brief The output frequency of the inverter 1st time the application is run. Current value can be updated from screen
 */
#define DEFAULT_OUTPUT_FREQ				(25)
/**
 * @brief The nominal frequency of the inverter 1st time the application is run. Current value can be updated from screen
 */
#define DEFAULT_NOMINAL_FREQ			(50)
/**
 * @brief The nominal modulation index of the inverter 1st time the application is run. Current value can be updated from screen
 */
#define DEFAULT_NOMINAL_m				(.7f)
/**
 * @brief The minimum allowed value of the output frequency
 */
#define MIN_OUTPUT_FREQ					(1.f)
/**
 * @brief The minimum allowed value of the nominal frequency
 */
#define MIN_NOMINAL_FREQ				(1.f)
/**
 * @brief The minimum allowed value of the nominal modulation index
 */
#define MIN_NOMINAL_m					(.05f)
/**
 * @brief The maximum allowed value of the output frequency
 */
#define MAX_OUTPUT_FREQ					(70.f)
/**
 * @brief The maximum allowed value of the nominal frequency
 */
#define MAX_NOMINAL_FREQ				(70.f)
/**
 * @brief The maximum allowed value of the nominal modulation index
 */
#define MAX_NOMINAL_m					(.95f)
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
