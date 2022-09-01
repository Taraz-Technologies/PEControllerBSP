/**
 ********************************************************************************
 * @file 		pecontroller_aux_tim.h
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

#ifndef PWM_PECONTROLLER_AUX_TIM_H_
#define PWM_PECONTROLLER_AUX_TIM_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef enum
{
	TIM_SLAVE_RESET = TIM_SLAVEMODE_RESET,
	TIM_SLAVE_START = TIM_SLAVEMODE_TRIGGER,
	TIM_SLAVE_RESET_AND_START = TIM_SLAVEMODE_COMBINED_RESETTRIGGER
} tim_slave_type_t;

typedef enum
{
	TIM_SLAVE_RISING = TIM_TRIGGERPOLARITY_RISING,
	TIM_SLAVE_FALLING = TIM_TRIGGERPOLARITY_FALLING
} tim_slave_edge_t;
/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variablesd
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
void BSP_AuxTim_ConfigTim3(float periodInUsecs, float triggerDelayInUsecs);
void BSP_AuxTim_ConfigTim2(float periodInUsecs, tim_slave_type_t slaveType, tim_slave_edge_t slaveEdge);
void BSP_AuxTim_ConfigHRTIM(pwm_period_t periodInUsec, bool startOnSync, int trigger);
void BSP_AuxTim_StartTim3(void);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
