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

/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup OPENLOOPVF_Exported_Macros Configurations
  * @{
  */
/**
 * @brief PWM time period in micro-seconds
 */
#define PWM_PERIOD_Us					(40)
/**
 * @brief Nominal frequency used for computation of current modulation index
 */
#define NOMINAL_FREQ					(50)
/**
 * @brief Nominal modulation index required at @ref NOMINAL_FREQ
 */
#define NOMINAL_MODULATION_INDEX		(.7f)
/**
 * @brief Acceleration factor for increasing speed of the controller
 */
#define ACCELERATION					(1.00001f)
/**
 * @brief Initial frequency used by the system
 */
#define INITIAL_FREQ					(1.f)
/**
 * @brief Final output frequency of the system
 */
#define OUTPUT_FREQ						(25)
/**
 * @brief Dead time value to be used
 */
#define INVERTER_DEADTIME_ns			(500)
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
