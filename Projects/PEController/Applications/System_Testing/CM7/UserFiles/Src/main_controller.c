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

static void PWM_S12switch(pwm_alignment_t align, duty_mode_t dutyMode)
{
	static pwm_module_config_t moduleConfig1 = {0};
	moduleConfig1.alignment = align;
	moduleConfig1.deadtime.on = true;
	moduleConfig1.deadtime.nanoSec = 1000;
	moduleConfig1.periodInUsec = 40;
	static pwm_config_t pwmConfig1 = {0};
	pwmConfig1.module = &moduleConfig1;
	pwmConfig1.dutyMode = dutyMode;
	pwmConfig1.invertPol = false;
	pwmConfig1.lim.min = 0;
	pwmConfig1.lim.max = 1;
	pwmConfig1.lim.minMaxDutyCycleBalancing = false;

	DutyCycleUpdateFnc fnc = BSP_PWM_ConfigInvertedPair(1, &pwmConfig1);
	fnc(1, 0.2f, &pwmConfig1);
	fnc = BSP_PWM_ConfigInvertedPair(9, &pwmConfig1);
	fnc(9, 0.2f, &pwmConfig1);
	fnc = BSP_PWM_ConfigInvertedPair(13, &pwmConfig1);
	fnc(13, 0.2f, &pwmConfig1);

	pwmConfig1.invertPol = true;
	fnc = BSP_PWM_ConfigInvertedPair(3, &pwmConfig1);
	fnc(3, 0.2f, &pwmConfig1);
	fnc = BSP_PWM_ConfigInvertedPair(11, &pwmConfig1);
	fnc(11, 0.2f, &pwmConfig1);
	fnc = BSP_PWM_ConfigInvertedPair(15, &pwmConfig1);
	fnc(15, 0.2f, &pwmConfig1);
}

void MainControl_Init(void)
{
	PWM_S12switch(CENTER_ALIGNED, OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH);
	//PWM_S12switch(EDGE_ALIGNED, OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH);
	//PWM_S12switch(CENTER_ALIGNED, OUTPUT_DUTY_AT_PWMH);
	//PWM_S12switch(EDGE_ALIGNED, OUTPUT_DUTY_AT_PWMH);

	BSP_PWM_Start(0xffff);

	for (int i = 1; i < 17; i++)
		BSP_Dout_SetAsPWMPin(i);
}


/* EOF */
