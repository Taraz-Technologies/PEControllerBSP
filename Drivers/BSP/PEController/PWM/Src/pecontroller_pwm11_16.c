/**
 ********************************************************************************
 * @file    	pecontroller_pwm11_16.c
 * @author 		Waqas Ehsan Butt
 * @date    	December 2, 2021
 *
 * @brief		Controls the PWM 11-16
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
#include "pecontroller_pwm11_16.h"
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
/** <c>true</c> if module previously initialized
 */
static bool pwm11_16_enabled = false;
/** Timer configurations
 */
static TIM_OC_InitTypeDef sConfigOC =
{
		.OCMode = TIM_OCMODE_PWM1,
		.Pulse = 0,
		.OCPolarity = TIM_OCPOLARITY_HIGH,
		.OCNPolarity = TIM_OCNPOLARITY_HIGH,
		.OCFastMode = TIM_OCFAST_DISABLE,
		.OCIdleState = TIM_OCIDLESTATE_RESET,
		.OCNIdleState = TIM_OCNIDLESTATE_SET,
};
static float dutyDeadTime = 0;
static bool isEdgeAligned;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
/** Timer 1 handle
 */
TIM_HandleTypeDef htim1;
/** keeps the callback function of all PWM module
 */
static PWMResetCallback resetCallback = NULL;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/

/**
 * @brief Initialize the relevant PWM modules (Timer1). Frequency is constant for the PWMs 11-16
 * @param *config Pointer to a structure that contains the configuration
 * 				   parameters for the PWM pair
 */
static void PWM11_16_Drivers_Init(pwm_config_t* config)
{
	if(pwm11_16_enabled)
		return;
	__HAL_RCC_TIM1_CLK_ENABLE();
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	if(config->module->alignment == CENTER_ALIGNED)
	{
		htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED2;
		htim1.Init.Period = (int)(config->module->periodInUsec * (TIM1_FREQ_MHz / 2.f));
		isEdgeAligned = false;
	}
	else
	{
		htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim1.Init.Period = (config->module->periodInUsec * TIM1_FREQ_MHz) - 1;
		isEdgeAligned = true;
	}
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
		Error_Handler();
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
		Error_Handler();
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
		Error_Handler();
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	if (IsDeadtimeEnabled(&config->module->deadtime))
	{
		int valBase = (int)((config->module->deadtime.nanoSec * TIM1_FREQ_MHz) / 1000.f);
		if (valBase < 128)
			sBreakDeadTimeConfig.DeadTime = valBase;
		else
		{
			int val = (int)(valBase / 2 - 64);
			if (val < 64)
				sBreakDeadTimeConfig.DeadTime = val | 0x80;
			else
			{
				val = (int)(valBase / 8 - 32);
				if (val < 32)
					sBreakDeadTimeConfig.DeadTime = val | 0xC0;
				else
				{
					val = (int)(valBase / 16 - 32);
					if (val < 32)
						sBreakDeadTimeConfig.DeadTime = val | 0xE0;
				}
			}
		}
		sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
		dutyDeadTime = config->module->deadtime.nanoSec / (config->module->periodInUsec * 1000.f);
	}
	else
		sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = IsDeadtimeEnabled(&config->module->deadtime) ? TIM_AUTOMATICOUTPUT_ENABLE : TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
		Error_Handler();

	float oldMax = config->lim.max;
	if (IsDeadtimeEnabled(&config->module->deadtime))
	{
		config->lim.max = 1 - dutyDeadTime;
		if (oldMax < config->lim.max && oldMax != 0)
			config->lim.max = oldMax;
	}

	if (config->lim.minMaxDutyCycleBalancing  && config->lim.max > .5f)
		config->lim.min = 1 - config->lim.max;

	pwm11_16_enabled = true;
}
/**
 * @brief Update the Duty Cycle of an Inverted Pair
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param duty duty cycle to be applied to the pair (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return float Duty cycle applied in this cycle. May differ from the @ref duty variable if outside permitted limits
 */
float BSP_PWM11_16_UpdatePairDuty(uint32_t pwmNo, float duty, pwm_config_t* config)
{
	/* check for duty cycle limits */
	if (duty > config->lim.max)
		duty = config->lim.max;
	else if (duty < config->lim.min)
		duty = config->lim.min;

	float dutyUse = duty;

	if (IsDeadtimeEnabled(&config->module->deadtime) && config->dutyMode == OUTPUT_DUTY_AT_PWMH)
		dutyUse += dutyDeadTime;

	uint32_t ch = (pwmNo - 11) / 2;
	*(((uint32_t*)&(TIM1->CCR1)) + ch) = (isEdgeAligned ? dutyUse : (1 - dutyUse)) * TIM1->ARR;

	return duty;
}

/**
 * @brief Configures a single inverted pair for PWM
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
static void PWM11_16_ConfigInvertedPair(uint32_t pwmNo, pwm_config_t* config)
{
	uint32_t ch = (pwmNo - 11) / 2;
	uint32_t isCh2 = (pwmNo - 11) % 2;

	ch = ch == 0 ? TIM_CHANNEL_1 : (ch == 1 ? TIM_CHANNEL_2 : TIM_CHANNEL_3);

	TIM_OC_InitTypeDef sConfigOCLocal;
	memcpy((void*)&sConfigOCLocal, (void*)&sConfigOC, sizeof(TIM_OC_InitTypeDef));

	sConfigOCLocal.OCMode = (isCh2 ^ isEdgeAligned) ? TIM_OCMODE_PWM1 : TIM_OCMODE_PWM2;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOCLocal, ch) != HAL_OK)
		Error_Handler();
}
/**
 * @brief Configures consecutive inverted pairs for PWM
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @param pairCount No of PWM pairs to be configured
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
DutyCycleUpdateFnc BSP_PWM11_16_ConfigInvertedPairs(uint32_t pwmNo, pwm_config_t* config, int pairCount)
{
	if(!pwm11_16_enabled)
		PWM11_16_Drivers_Init(config);
	while (pairCount--)
	{
		PWM11_16_ConfigInvertedPair(pwmNo, config);
		pwmNo += 2;
	}
	return BSP_PWM11_16_UpdatePairDuty;
}

/**
 * @brief Update the Duty Cycle of a channel
 * @param pwmNo PWM channel to be configured (Valid Values 11,13,15)
 * @param duty duty cycle to be applied to the channel (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channel
 * @return float Duty cycle applied in this cycle. May differ from the @ref duty variable if outside permitted limits
 */
float BSP_PWM11_16_UpdateChannelDuty(uint32_t pwmNo, float duty, pwm_config_t* config)
{
	/* check for duty cycle limits */
	// no need of minimum limit because it may cause confusion as timer is constant
	if (duty > config->lim.max)
		duty = config->lim.max;

	float dutyUse = duty;

	// always OUTPUT_DUTY_AT_PWMH MODE because dead time will be always added if due to common timer
	if (IsDeadtimeEnabled(&config->module->deadtime))
		dutyUse += dutyDeadTime;

	uint32_t ch = (pwmNo - 11) / 2;
	*(((uint32_t*)&(TIM1->CCR1)) + ch) = dutyUse * TIM1->ARR;

	return duty;
}
/**
 * @brief Configures a single PWM channel
 * @param pwmNo Channel no of the PWM Channel in the pair (Valid Values 11,13,15)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channels
 */
static void PWM11_16_ConfigChannel(uint32_t pwmNo, pwm_config_t* config)
{
	uint32_t ch = (pwmNo - 11);
	// Even pairs may only be selected as inverted channels
	TIM_OC_InitTypeDef sConfigOCLocal;
	memcpy((void*)&sConfigOCLocal, (void*)&sConfigOC, sizeof(TIM_OC_InitTypeDef));
	if(ch % 2 != 0)
	{
		sConfigOCLocal.OCMode = TIM_OCMODE_PWM2;
		//sConfigOCLocal.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	}
	ch = (ch & 0x6) ? ((ch & 2) ? TIM_CHANNEL_2 : TIM_CHANNEL_3) : TIM_CHANNEL_1;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOCLocal, ch) != HAL_OK)
		Error_Handler();
}

/**
 * @brief Configures consecutive PWM channels
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channels
 * @param chCount No of channels to be configured with the setting. Max supported value is 3. The value should be counted while skipping even channels.
 * 					e.g. if pwmNo = 11 and chCount = 2 the PWMs 11 and 13 will be configured with the specified setting while PWM12 will be the inverted version of PWM11
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
DutyCycleUpdateFnc BSP_PWM11_16_ConfigChannels(uint32_t pwmNo, pwm_config_t* config, int chCount)
{
	if(!pwm11_16_enabled)
		PWM11_16_Drivers_Init(config);
	while (chCount--)
	{
		PWM11_16_ConfigChannel(pwmNo, config);
		pwmNo += 2;						// No need to configure the other
	}
	return BSP_PWM11_16_UpdateChannelDuty;
}

/**
 * @brief Enable / Disable interrupt for a PWM channel as per requirement
 * @param enable If enable interrupt set this parameter to <c>true</>
 * @param callback Specifies the function to be called when the PWM is reset
 * @param priority Interrupt priority. Range (0-15). Here 0 is the highest priority
 */
void BSP_PWM11_16_Config_Interrupt(bool enable, PWMResetCallback callback, int priority)
{
	if (enable)
	{
		resetCallback = callback;
		__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
		HAL_NVIC_SetPriority(TIM1_UP_IRQn, priority, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}
	else
	{
		__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
		HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
		resetCallback = NULL;
	}
}

/**
 * @brief Timer1 update IRQ Handler to process Timer 1 interrupts
 */
void TIM1_UP_IRQHandler(void)
{
	/* TIM Update event */
	if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_UPDATE) != RESET)
	{
		if (__HAL_TIM_GET_IT_SOURCE(&htim1, TIM_IT_UPDATE) != RESET)
		{
			resetCallback();
			__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);
		}
	}
}
/* EOF */
