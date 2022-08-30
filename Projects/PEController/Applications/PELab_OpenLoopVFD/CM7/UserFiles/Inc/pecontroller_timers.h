/**
 ********************************************************************************
 * @file 		pecontroller_timers.h
 * @author 		Waqas Ehsan Butt
 * @date 		Aug 30, 2022
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

#ifndef PWM_PECONTROLLER_TIMERS_H_
#define PWM_PECONTROLLER_TIMERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "pecontroller_pwm_base.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define HRTIM_FREQ					(480)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** Pointer to the timer structure
 */
extern HRTIM_HandleTypeDef hhrtim;
/** Enabled HRTimer
 * <c>true</c> if module previously initialized
 */
extern bool hrtimEnabled;
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
uint32_t BSP_HRTim_GetTimerFreq(uint32_t TimerIdx);
HRTIM_TimerCfgTypeDef BSP_HRTim_GetDefaultTimerConfig(pwm_period_t periodInUsec, uint32_t TimerIdx);
HRTIM_CompareCfgTypeDef BSP_HRTim_GetDefaultCompareConfig(void);
HRTIM_DeadTimeCfgTypeDef BSP_HRTim_GetDefaultDeadtimeConfig(void);
HRTIM_OutputCfgTypeDef BSP_HRTim_GetDefaultOutputConfig(void);
IRQn_Type BSP_HRTim_GetIRQn(uint32_t TimerIdx);
/**
 * @brief  Initialize the HRTimer
 */
void BSP_HRTim_Init(void);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
