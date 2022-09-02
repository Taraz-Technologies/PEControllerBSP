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

#ifndef PECONTROLLER_AUX_TIM_H_
#define PECONTROLLER_AUX_TIM_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "pecontroller_pwm_base.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef enum
{
	TIM_SLAVE_RISING = TIM_TRIGGERPOLARITY_RISING,
	TIM_SLAVE_FALLING = TIM_TRIGGERPOLARITY_FALLING
} tim_slave_edge_t;
typedef enum
{
	HRTIM_COMP1 = 0,
	HRTIM_COMP2 = 2,
	HRTIM_COMP3 = 3,
	HRTIM_COMP4 = 4
} hrtim_comp_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	pwm_sync_src_t syncSrc;
	pwm_sync_type_t syncType;
	pwm_period_t periodInUsecs;
} hrtim_opts_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
void BSP_AuxTim_ConfigTim3(float periodInUsecs, float triggerDelayInUsecs);
void BSP_AuxTim_ConfigTim2(float periodInUsecs, pwm_sync_type_t slaveType, tim_slave_edge_t slaveEdge);
extern void BSP_AuxTim_ConfigHRTIM(hrtim_opts_t* opts);
extern void BSP_AuxTim_SetValueShift(hrtim_comp_t comp, uint32_t value);
extern void BSP_AuxTim_SetDutyShift(hrtim_opts_t* opts, hrtim_comp_t comp, float duty);
void BSP_AuxTim_StartTim3(bool startHrtimMaster);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
