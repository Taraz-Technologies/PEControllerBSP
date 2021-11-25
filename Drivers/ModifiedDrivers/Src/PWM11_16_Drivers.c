/*! 
@file PWM11_16_Drivers.c
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "GeneralHeader.h"
#include "PWMDriver.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static TIM_OC_InitTypeDef sConfigOC = 
{
		.OCMode = TIM_OCMODE_PWM2,
		.Pulse = 0,
		.OCPolarity = TIM_OCPOLARITY_HIGH,
		.OCNPolarity = TIM_OCNPOLARITY_HIGH,
		.OCFastMode = TIM_OCFAST_DISABLE,
		.OCIdleState = TIM_OCIDLESTATE_RESET,
		.OCNIdleState = TIM_OCNIDLESTATE_SET,
};
TIM_HandleTypeDef htim1;
static bool moduleEnabled = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Initialize the relevant PWM modules (Timer1). Frequency is constant for the PWMs 11-16
 * @param *config Pointer to a structure that contains the configuration
 * 				   parameters for the PWM pair
 */
static void Drivers_Init(pwm_config_t* config)
{
	if(moduleEnabled)
		return;
	__HAL_RCC_TIM1_CLK_ENABLE();
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	if(config->module->alignment == CENTER_ALIGNED)
	{
		htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED2;
		htim1.Init.Period = (config->module->periodInUsec * 120);		// --todo-- centralize
	}
	else
	{
		htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim1.Init.Period = (config->module->periodInUsec * 240) - 1;  // --todo-- centralize
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
	sBreakDeadTimeConfig.DeadTime = 185;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = IsDeadtimeEnabled(&config->module->deadtime) ? TIM_AUTOMATICOUTPUT_ENABLE : TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
		Error_Handler();
	moduleEnabled = true;
}
/**
 * @brief Update the Duty Cycle of an Inverted Pair
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param duty duty cycle to be applied to the pair (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
void PWM11_16_UpdatePairDuty(uint32_t pwmNo, float duty, pwm_config_t* config)
{

}

/**
 * @brief Configures a single inverted pair for PWM
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
static void ConfigInvertedPair(uint32_t pwmNo, pwm_config_t* config)
{
	uint32_t ch = (pwmNo - 11);
	// If pair not selected correctly report as error
	if(ch % 2 != 0)
		Error_Handler();
	ch = ch == 0 ? TIM_CHANNEL_1 : (ch == 2 ? TIM_CHANNEL_2 : TIM_CHANNEL_3);
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, ch) != HAL_OK)
		Error_Handler();
	return PWM11_16_UpdatePairDuty;
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
DutyCycleUpdateFnc PWM11_16_Drivers_ConfigInvertedPairs(uint32_t pwmNo, pwm_config_t* config, int pairCount)
{
	if(!moduleEnabled)
		Drivers_Init(config);
	while (pairCount--)
	{
		ConfigInvertedPair(pwmNo, config);
		pwmNo += 2;
	}
	return PWM11_16_UpdatePairDuty;
}

/**
 * @brief Update the Duty Cycle of a channel
 * @param pwmNo PWM channel to be configured (Valid Values 11,13,15)
 * @param duty duty cycle to be applied to the channel (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channel
 */
void PWM11_16_UpdateChannelDuty(uint32_t pwmNo, float duty, pwm_config_t* config)
{

}
/**
 * @brief Configures a single PWM channel
 * @param pwmNo Channel no of the PWM Channel in the pair (Valid Values 11,13,15)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channels
 */
static void ConfigChannel(uint32_t pwmNo, pwm_config_t* config)
{
	uint32_t ch = (pwmNo - 11);
	// Even pairs may only be selected as inverted channels
	if(ch % 2 != 0)
		Error_Handler();
	ch = ch == 0 ? TIM_CHANNEL_1 : (ch == 2 ? TIM_CHANNEL_2 : TIM_CHANNEL_3);
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, ch) != HAL_OK)
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
DutyCycleUpdateFnc PWM11_16_Drivers_ConfigChannels(uint32_t pwmNo, pwm_config_t* config, int chCount)
{
	if(!moduleEnabled)
		Drivers_Init(config);
	while (chCount--)
	{
		ConfigChannel(pwmNo, config);
		pwmNo += 2;						// No need to configure the other
	}
	return PWM11_16_UpdateChannelDuty;
}
/* EOF */
