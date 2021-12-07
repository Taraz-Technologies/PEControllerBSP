/**
 ********************************************************************************
 * @file    	open_loop_vf_controller.c
 * @author 		Waqas Ehsan Butt
 * @date    	December 7, 2021
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
#pragma GCC push_options
#pragma GCC optimize ("-Ofast")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "open_loop_vf_controller.h"
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
static pwm_module_config_t inverterPWMModuleConfig =
{
		.interruptEnabled = true,
		.alignment = CENTER_ALIGNED,
		.periodInUsec = PWM_PERIOD_Us,
		.deadtime = {
				.on = true,
				.nanoSec = INVERTER_DEADTIME_ns,
		},
};
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Initialize the grid tie controller
 * @param gridTie Pointer to the grid tie structure
 * @param pwmResetCallback Function callback issued after each PWM completion
 */
void OpenLoopVfControl_Init(openloopvf_config_t* config, PWMResetCallback pwmResetCallback)
{
	/***************** Configure Inverter *********************/
	inverterPWMModuleConfig.callback = pwmResetCallback;
	inverterPWMModuleConfig.interruptEnabled = pwmResetCallback == NULL ? true : false;
	inverter3Ph_config_t* inverterConfig = &config->inverterConfig;
	inverterConfig->legType = LEG_DEFAULT;
	inverterConfig->pwmConfig.lim.min = 0;
	inverterConfig->pwmConfig.lim.max = 1;
	inverterConfig->pwmConfig.lim.minMaxDutyCycleBalancing = MIN_MAX_BALANCING_INVERTER;
	inverterConfig->pwmConfig.dutyMode = INVERTER_DUTY_MODE;
	inverterConfig->pwmConfig.module = &inverterPWMModuleConfig;
	Inverter3Ph_Init(inverterConfig);
	/***************** Configure Inverter *********************/

	/***************** Configure Control *********************/
	config->pwmFreq = PWM_FREQ_Hz;
	config->nominalFreq = NOMINAL_FREQ;
	config->nominalModulationIndex = NOMINAL_MODULATION_INDEX;
	config->outputFreq = OUTPUT_FREQ;
	config->acceleration = ACCELERATION;
	config->wt = 0;
	config->currentFreq = INITIAL_FREQ;
	/***************** Configure Control *********************/
}

/**
 * @brief This function computes new duty cycles for the inverter in each cycle
 * @param config Pointer to the inverter structure
 * @details Here the frequency starts from the @ref INITIAL_FREQ and keeps increasing till
 * 	it reaches the @ref OUTPUT_FREQ value with constant @ref ACCELERATION. The @ref  currentModulationIndex
 * 	is acquired by @ref nominalModulationIndex / @ref nominalFreq
 */
void OpenLoopVfControl_Loop(openloopvf_config_t* config)
{
	float duties[3];

	// adjust the frequency with given acceleration
	if(config->currentFreq < config->outputFreq)
	{
		config->currentFreq *= config->acceleration;
		if(config->currentFreq > config->outputFreq)
			config->currentFreq = config->outputFreq;
	}
	if(config->currentFreq > config->outputFreq)
	{
		config->currentFreq /= config->acceleration;
		if(config->currentFreq < config->outputFreq)
			config->currentFreq = config->outputFreq;
	}

	// compute the current modulation index
	config->currentModulationIndex = (config->nominalModulationIndex / config->nominalFreq) * config->currentFreq;
	float stepSize = (TWO_PI * config->currentFreq) / config->pwmFreq;
	config->wt += stepSize;
	if(config->wt > TWO_PI)
		config->wt -= TWO_PI;

	// generate SPWM according to the theta and modulation index
	ComputeDuty_SPWM(config->wt, config->currentModulationIndex, duties);
}
#pragma GCC pop_options
/* EOF */
