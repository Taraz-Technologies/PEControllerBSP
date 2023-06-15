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
  * @{
  */
/**
 * @brief PWM frequency in Hz. Minimum value is 5000
 */
#define PWM_FREQ_Hz						(25000)
/**
 * @brief Acceleration factor for increasing speed of the controller
 */
#define ACCELERATION					(1.00001f)
/**
 * @brief Initial frequency used by the system
 */
#define INITIAL_FREQ					(1.f)
/**
 * @brief Dead time value to be used
 */
#define INVERTER_DEADTIME_ns			(200)

#define HAS_DUPLICATE_SWITCH			(PECONTROLLER_CONFIG == PLB_TNPC || PECONTROLLER_CONFIG == PLB_MMC ? 1 : 0)
#define RELAY_COUNT						(PECONTROLLER_CONFIG == PLB_TNPC || PECONTROLLER_CONFIG == PLB_MMC ? 0 : (PELAB_VERSION >= 4 ? 4 : 2))
#define LEG_SWITCH_COUNT				(PECONTROLLER_CONFIG == PLB_TNPC ? 4 : 2)
#define VFD_COUNT						(PECONTROLLER_CONFIG == PLB_TNPC ? 1 : 2)
#define VFD1_PIN1						(1)
#if VFD_COUNT == 2
#define VFD2_PIN1						(VFD1_PIN1 + (HAS_DUPLICATE_SWITCH ? LEG_SWITCH_COUNT * 4 : LEG_SWITCH_COUNT * 3))
#endif

#define DEFAULT_OUTPUT_FREQ				(25)
#define DEFAULT_NOMINAL_FREQ			(50)
#define DEFAULT_NOMINAL_m				(.7f)
#define MIN_OUTPUT_FREQ					(1.f)
#define MIN_NOMINAL_FREQ				(1.f)
#define MIN_NOMINAL_m					(.05f)
#define MAX_OUTPUT_FREQ					(70.f)
#define MAX_NOMINAL_FREQ				(70.f)
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
