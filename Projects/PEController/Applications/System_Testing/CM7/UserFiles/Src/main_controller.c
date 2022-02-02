/**
 ********************************************************************************
 * @file    	main_controller.c
 * @author 		Waqas Ehsan Butt
 * @date    	Feb 2, 2022
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

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "control_library.h"
#include "adc_config.h"
#include "pecontroller_pwm.h"
#include "pecontroller_digital_in.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/

/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/

static void PWM_S12switch(pwm_alignment_t align, duty_mode_t dutyMode, bool dt)
{
	static pwm_module_config_t moduleConfig = {0};
	static pwm_config_t pwmConfig = {0};
	BSP_PWM_GetDafaultModuleConfig(&moduleConfig);
	BSP_PWM_GetDefaultConfig(&pwmConfig, &moduleConfig);
	moduleConfig.alignment = align;
	moduleConfig.deadtime.on = dt;
	pwmConfig.dutyMode = dutyMode;

	DutyCycleUpdateFnc fnc = BSP_PWM_ConfigInvertedPair(1, &pwmConfig);
	fnc(1, 0.2f, &pwmConfig);
	fnc = BSP_PWM_ConfigInvertedPair(9, &pwmConfig);
	fnc(9, 0.2f, &pwmConfig);
	fnc = BSP_PWM_ConfigInvertedPair(13, &pwmConfig);
	fnc(13, 0.2f, &pwmConfig);
	//fnc = BSP_PWM_ConfigChannel(5, &pwmConfig);
	//fnc(5, 0.2f, &pwmConfig);
	//fnc = BSP_PWM_ConfigChannel(15, &pwmConfig);
	//fnc(5, 0.2f, &pwmConfig);

	fnc = BSP_PWM_ConfigInvertedPair(4, &pwmConfig);
	fnc(4, 0.2f, &pwmConfig);
	fnc = BSP_PWM_ConfigInvertedPair(12, &pwmConfig);
	fnc(12, 0.2f, &pwmConfig);
}

void MainControl_Init(void)
{
	//PWM_S12switch(CENTER_ALIGNED, OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH, false);
	//PWM_S12switch(EDGE_ALIGNED, OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH, false);
	//PWM_S12switch(CENTER_ALIGNED, OUTPUT_DUTY_AT_PWMH, false);
	//PWM_S12switch(EDGE_ALIGNED, OUTPUT_DUTY_AT_PWMH, false);
	//PWM_S12switch(CENTER_ALIGNED, OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH, true);
	//PWM_S12switch(EDGE_ALIGNED, OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH, true);
	//PWM_S12switch(CENTER_ALIGNED, OUTPUT_DUTY_AT_PWMH, true);
	PWM_S12switch(EDGE_ALIGNED, OUTPUT_DUTY_AT_PWMH, true);

	BSP_PWM_Start(0xffff);

	for (int i = 1; i < 17; i++)
		BSP_Dout_SetAsPWMPin(i);
}


/* EOF */
