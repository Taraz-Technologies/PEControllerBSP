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

/** @addtogroup PELab_OpenLoopVFD
 * @{
 */

/** @addtogroup PELab_OpenLoopVFD_CM7
 * @{
 */

/** @addtogroup PELab_OpenLoopVFD_CM7_Configs
 * @{
 */

/** @defgroup PELab_OpenLoopVFD_CM7_ControlConfigs Control Configurations
 * @brief Defines the configurations used by the control system
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/

/********************************************************************************
 * Defines
 *******************************************************************************/
#define PWM_PERIOD_Us					(40)
#define PWM_PERIOD_s					(PWM_PERIOD_Us/1000000.f)
#define PWM_FREQ_KHz					(1000.f/PWM_PERIOD_Us)
#define PWM_FREQ_Hz						(1.f/PWM_PERIOD_s)

#define NOMINAL_FREQ					(50)
#define NOMINAL_MODULATION_INDEX		(.7f)
#define ACCELERATION					(1.00001f)
#define INITIAL_FREQ					(1.f)
#define OUTPUT_FREQ						(25)
#define MIN_MAX_BALANCING_INVERTER		(false)
#define INVERTER_DUTY_MODE				OUTPUT_DUTY_AT_PWMH
#define INVERTER_DEADTIME_ns			(500)
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
/**
 * @}
 */
/**
 * @}
 */
#endif 
/* EOF */
