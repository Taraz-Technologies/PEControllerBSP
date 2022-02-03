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
typedef struct
{
	pwm_alignment_t align;
	duty_mode_t dutyMode;
	bool dt;
} options_t;
/********************************************************************************
 * Static Variables
 *******************************************************************************/

/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
static options_t optsAll[8] =
{
		{ .align = EDGE_ALIGNED, .dutyMode = OUTPUT_DUTY_AT_PWMH, .dt = true },
		{ .align = CENTER_ALIGNED, .dutyMode = OUTPUT_DUTY_AT_PWMH, .dt = true },
		{ .align = EDGE_ALIGNED, .dutyMode = OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH, .dt = true },
		{ .align = CENTER_ALIGNED, .dutyMode = OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH, .dt = true },
		{ .align = EDGE_ALIGNED, .dutyMode = OUTPUT_DUTY_AT_PWMH, .dt = false },
		{ .align = CENTER_ALIGNED, .dutyMode = OUTPUT_DUTY_AT_PWMH, .dt = false },
		{ .align = EDGE_ALIGNED, .dutyMode = OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH, .dt = false },
		{ .align = CENTER_ALIGNED, .dutyMode = OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH, .dt = false },
};
/********************************************************************************
 * Code
 *******************************************************************************/
// minimum value doesn't matter in individual channels
static void PWM_LimitTests_Balancing(options_t* opts)
{
	static pwm_module_config_t moduleConfig = {0};
	static pwm_config_t pwmConfig = {0};
	BSP_PWM_GetDafaultModuleConfig(&moduleConfig);
	BSP_PWM_GetDefaultConfig(&pwmConfig, &moduleConfig);
	moduleConfig.alignment = opts->align;
	pwmConfig.dutyMode = opts->dutyMode;
	moduleConfig.deadtime.on = true;
	moduleConfig.deadtime.nanoSec = 1000;
	moduleConfig.periodInUsec = 50;

	// min max correction by balancing
	pwmConfig.lim.max = 1.f;
	pwmConfig.lim.min = 0.f;
	pwmConfig.lim.minMaxDutyCycleBalancing = true;
	BSP_PWM_ConfigInvertedPair(1, &pwmConfig);
	if ((pwmConfig.lim.max > .985f || pwmConfig.lim.max < .975f) || (pwmConfig.lim.min > .025f || pwmConfig.lim.min < .015f))
		Error_Handler();

	pwmConfig.lim.max = 1.f;
	pwmConfig.lim.min = 0.f;
	BSP_PWM_ConfigInvertedPair(11, &pwmConfig);
	if ((pwmConfig.lim.max > .985f || pwmConfig.lim.max < .975f) || (pwmConfig.lim.min > .025f || pwmConfig.lim.min < .015f))
		Error_Handler();

	// no correction if no balancing
	pwmConfig.lim.max = 1.f;
	pwmConfig.lim.min = 0.f;
	pwmConfig.lim.minMaxDutyCycleBalancing = false;
	BSP_PWM_ConfigInvertedPair(1, &pwmConfig);
	if ((pwmConfig.lim.max > .985f || pwmConfig.lim.max < .975f) || (pwmConfig.lim.min != 0))
		Error_Handler();

	pwmConfig.lim.max = 1.f;
	pwmConfig.lim.min = 0.f;
	BSP_PWM_ConfigInvertedPair(11, &pwmConfig);
	if ((pwmConfig.lim.max > .985f || pwmConfig.lim.max < .975f) || (pwmConfig.lim.min != 0))
		Error_Handler();

	// correction if not auto balancing
	pwmConfig.lim.max = .6f;
	pwmConfig.lim.min = 0.f;
	pwmConfig.lim.minMaxDutyCycleBalancing = true;
	BSP_PWM_ConfigInvertedPair(1, &pwmConfig);
	if ((pwmConfig.lim.max > .605f || pwmConfig.lim.max < .595f) || (pwmConfig.lim.min > .405f || pwmConfig.lim.min < .395f))
		Error_Handler();

	pwmConfig.lim.max = .6f;
	pwmConfig.lim.min = 0.f;
	BSP_PWM_ConfigInvertedPair(11, &pwmConfig);
	if ((pwmConfig.lim.max > .605f || pwmConfig.lim.max < .595f) || (pwmConfig.lim.min > .405f || pwmConfig.lim.min < .395f))
		Error_Handler();

	// no correction if lower than 0.5f
	pwmConfig.lim.max = .45f;
	pwmConfig.lim.min = 0.f;
	pwmConfig.lim.minMaxDutyCycleBalancing = true;
	DutyCycleUpdateFnc fnc = BSP_PWM_ConfigInvertedPair(1, &pwmConfig);
	if ((pwmConfig.lim.max > .455f || pwmConfig.lim.max < .445f) || (pwmConfig.lim.min != 0))
		Error_Handler();

	pwmConfig.lim.max = .45f;
	pwmConfig.lim.min = 0.f;
	BSP_PWM_ConfigInvertedPair(11, &pwmConfig);
	if ((pwmConfig.lim.max > .455f || pwmConfig.lim.max < .445f) || (pwmConfig.lim.min != 0))
		Error_Handler();

	fnc(1, .7f, &pwmConfig);
}
// minimum value doesn't matter in individual channels
static void PWM_LimitTests_Channel(options_t* opts)
{
	static pwm_module_config_t moduleConfig = {0};
	static pwm_config_t pwmConfig = {0};
	BSP_PWM_GetDafaultModuleConfig(&moduleConfig);
	BSP_PWM_GetDefaultConfig(&pwmConfig, &moduleConfig);
	moduleConfig.alignment = opts->align;
	moduleConfig.deadtime.on = opts->dt;
	pwmConfig.dutyMode = opts->dutyMode;
	pwmConfig.lim.max = .4f;
	pwmConfig.lim.min = .2f;
	pwmConfig.lim.minMaxDutyCycleBalancing = false;

	DutyCycleUpdateFnc fnc = BSP_PWM_ConfigChannel(1, &pwmConfig);
	float d = fnc(1, .1f, &pwmConfig);
	if (d > .1f)
		Error_Handler();
	fnc = BSP_PWM_ConfigChannel(11, &pwmConfig);
	d = fnc(11, .1f, &pwmConfig);
	if (d > .1f)
		Error_Handler();

	fnc = BSP_PWM_ConfigChannel(3, &pwmConfig);
	d = fnc(3, .5f, &pwmConfig);
	if (d > .4f)
		Error_Handler();
	fnc = BSP_PWM_ConfigChannel(13, &pwmConfig);
	d = fnc(13, .5f, &pwmConfig);
	if (d > .4f)
		Error_Handler();

	fnc = BSP_PWM_ConfigChannel(5, &pwmConfig);
	d = fnc(5, .25f, &pwmConfig);
	if (d < .24f || d > .26f)
		Error_Handler();
	fnc = BSP_PWM_ConfigChannel(15, &pwmConfig);
	d = fnc(15, .25f, &pwmConfig);
	if (d < .24f || d > .26f)
		Error_Handler();
}
// min and maximum limit violation tests
static void PWM_LimitTests_Pair(options_t* opts)
{
	static pwm_module_config_t moduleConfig = {0};
	static pwm_config_t pwmConfig = {0};
	BSP_PWM_GetDafaultModuleConfig(&moduleConfig);
	BSP_PWM_GetDefaultConfig(&pwmConfig, &moduleConfig);
	moduleConfig.alignment = opts->align;
	moduleConfig.deadtime.on = opts->dt;
	pwmConfig.dutyMode = opts->dutyMode;
	pwmConfig.lim.max = .4f;
	pwmConfig.lim.min = .2f;
	pwmConfig.lim.minMaxDutyCycleBalancing = false;

	DutyCycleUpdateFnc fnc = BSP_PWM_ConfigInvertedPair(1, &pwmConfig);
	float d = fnc(1, .1f, &pwmConfig);
	if (d < .2f)
		Error_Handler();
	fnc = BSP_PWM_ConfigInvertedPair(11, &pwmConfig);
	d = fnc(11, .1f, &pwmConfig);
	if (d < .2f)
		Error_Handler();

	fnc = BSP_PWM_ConfigInvertedPair(3, &pwmConfig);
	d = fnc(3, .5f, &pwmConfig);
	if (d > .4f)
		Error_Handler();
	fnc = BSP_PWM_ConfigInvertedPair(13, &pwmConfig);
	d = fnc(13, .5f, &pwmConfig);
	if (d > .4f)
		Error_Handler();

	fnc = BSP_PWM_ConfigInvertedPair(5, &pwmConfig);
	d = fnc(5, .25f, &pwmConfig);
	if (d < .24f || d > .26f)
		Error_Handler();
	fnc = BSP_PWM_ConfigInvertedPair(15, &pwmConfig);
	d = fnc(15, .25f, &pwmConfig);
	if (d < .24f || d > .26f)
		Error_Handler();
}

static void PWM_S12switch(options_t* opts)
{
	static pwm_module_config_t moduleConfig = {0};
	static pwm_config_t pwmConfig = {0};
	BSP_PWM_GetDafaultModuleConfig(&moduleConfig);
	BSP_PWM_GetDefaultConfig(&pwmConfig, &moduleConfig);
	moduleConfig.alignment = opts->align;
	moduleConfig.deadtime.on = opts->dt;
	pwmConfig.dutyMode = opts->dutyMode;

	DutyCycleUpdateFnc fnc = BSP_PWM_ConfigInvertedPair(1, &pwmConfig);
	fnc(1, 0.2f, &pwmConfig);
	fnc = BSP_PWM_ConfigInvertedPair(9, &pwmConfig);
	fnc(9, 0.2f, &pwmConfig);
	fnc = BSP_PWM_ConfigInvertedPair(13, &pwmConfig);
	fnc(13, 0.2f, &pwmConfig);
	fnc = BSP_PWM_ConfigChannel(5, &pwmConfig);
	fnc(5, 0.2f, &pwmConfig);
	fnc = BSP_PWM_ConfigChannel(8, &pwmConfig);
	fnc(8, 0.2f, &pwmConfig);
	fnc = BSP_PWM_ConfigChannel(16, &pwmConfig);
	fnc(16, 0.2f, &pwmConfig);


	fnc = BSP_PWM_ConfigInvertedPair(4, &pwmConfig);
	fnc(4, 0.2f, &pwmConfig);
	fnc = BSP_PWM_ConfigInvertedPair(12, &pwmConfig);
	fnc(12, 0.2f, &pwmConfig);
}

void MainControl_Init(void)
{
	//PWM_S12switch(optsAll + 0);
	PWM_LimitTests_Balancing(optsAll + 0);


	BSP_PWM_Start(0xffff);

	for (int i = 1; i < 17; i++)
		BSP_Dout_SetAsPWMPin(i);
}


/* EOF */
