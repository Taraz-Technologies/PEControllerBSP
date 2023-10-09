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
#include "user_config.h"
#include "open_loop_vf_controller.h"
#include "shared_memory.h"
#include "pecontroller_timers.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
//#define PWM_PERIOD_s					(PWM_PERIOD_Us/1000000.f)
//#define PWM_FREQ_KHz					(1000.f/PWM_PERIOD_Us)
//#define PWM_FREQ_Hz						(1.f/PWM_PERIOD_s)
#define MIN_MAX_BALANCING_INVERTER		(false)
#define INVERTER_DUTY_MODE				OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH
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
		.alignment = CENTER_ALIGNED,
		.f = PWM_FREQ_Hz,
		.deadtime = {
				.on = true,
				.nanoSec = INVERTER_DEADTIME_ns,
		},
};
static tim_in_trigger_config_t timerTriggerIn =
{
		.src = TIM_TRG_SRC_TIM1,
		.type = TIM_TRGI_TYPE_RESET_AND_START
};
static tim_out_trigger_config_t timerTriggerOut =
{
		.type = TIM_TRGO_OUT_UPDATE
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
 * @param config Pointer to the configuration structure
 * @param pwmResetCallback Function callback issued after each PWM completion
 */
void OpenLoopVfControl_Init(openloopvf_config_t* config, PWMResetCallback pwmResetCallback)
{
	/***************** Configure Inverter *********************/
	inverter3Ph_config_t* inverterConfig = &config->inverterConfig;
#if PECONTROLLER_CONFIG == PLB_TNPC
	inverterConfig->legType = LEG_TNPC;
#else
	inverterConfig->legType = LEG_DEFAULT;
#endif
	inverterConfig->pwmConfig.lim.min = 0;
	inverterConfig->pwmConfig.lim.max = 1;
	inverterConfig->pwmConfig.lim.minMaxDutyCycleBalancing = MIN_MAX_BALANCING_INVERTER;
	inverterConfig->pwmConfig.dutyMode = INVERTER_DUTY_MODE;
	inverterConfig->pwmConfig.slaveOpts = &timerTriggerIn;
	inverterConfig->pwmConfig.masterOpts = &timerTriggerOut;
	inverterConfig->pwmConfig.module = &inverterPWMModuleConfig;
	Inverter3Ph_Init(inverterConfig);
	/***************** Configure Inverter *********************/

	/***************** Configure Control *********************/
	config->pwmFreq = PWM_FREQ_Hz;
	config->acceleration= ACCELERATION;
	config->wt = 0;
	config->currentFreq = INITIAL_FREQ;
	/***************** Configure Control *********************/

	if(pwmResetCallback != NULL)
		BSP_PWM_Config_Interrupt(inverterConfig->s1PinNos[0], true, pwmResetCallback, 1);

	Inverter3Ph_Activate(inverterConfig, false);
}
/**
 * @brief This function computes new duty cycles for the inverter in each cycle
 * @param config Pointer to the inverter structure
 * @details Here the frequency starts from the @ref INITIAL_FREQ and keeps increasing till
 * 	it reaches the required frequency value with constant @ref ACCELERATION. The currentModulationIndex
 * 	is acquired by nominalModulationIndex / nominalFreq
 */
void OpenLoopVfControl_Loop(openloopvf_config_t* config)
{
	if (config->inverterConfig.state == INVERTER_INACTIVE)
		return;
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

	// generate and apply SPWM according to the theta and modulation index
	Inverter3Ph_UpdateSPWM(&config->inverterConfig, config->wt, config->currentModulationIndex);
}
/**
 * @brief Activate/Deactivate the inverter
 * @param config Pointer to the inverter structure
 * @param activate en <c>true</c> if needs to be enabled else <c>false</c>
 */
void OpenLoopVfControl_Activate(openloopvf_config_t* config, bool activate)
{
	if (activate)
	{
		if (config->inverterConfig.state == INVERTER_INACTIVE)
			config->currentFreq = INITIAL_FREQ;
		Inverter3Ph_Activate(&config->inverterConfig, true);
	}
	else
	{
		if (config->inverterConfig.state == INVERTER_ACTIVE)
			config->currentFreq = INITIAL_FREQ;
		Inverter3Ph_Activate(&config->inverterConfig, false);
	}
}

#pragma GCC pop_options
/* EOF */
