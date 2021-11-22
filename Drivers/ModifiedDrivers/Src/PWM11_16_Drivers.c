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
#if 0
/**
 * @brief Initialize the relevant PWM modules (Timer1). Frequency is constant for the PWMs 11-16
 *
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
void PWM11_16_Drivers_Init(pwm_pair_config_t* config)
{
	if(moduleEnabled)
		return;
	__HAL_RCC_TIM1_CLK_ENABLE();
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	if(config->alignment == CENTER_ALIGNED)
	{
		htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED2;
		htim1.Init.Period = (config->periodInUsec * 120);
	}
	else
	{
		htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim1.Init.Period = (config->periodInUsec * 240) - 1;
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
	sBreakDeadTimeConfig.AutomaticOutput = config->dtEnabled ? TIM_AUTOMATICOUTPUT_ENABLE : TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
		Error_Handler();
	moduleEnabled = true;
}
/**
 * @brief Update the PWM Pairs duty cycle
 *
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param duty duty cycle to be applied to the pair (Range 0-1)
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
void PWM11_16_UpdatePair(uint32_t pwmNo, float duty, pwm_pair_config_t* config)
{
	uint32_t ch = (pwmNo - 11) / 2; 
	*(((uint32_t*)&(TIM1->CCR1)) + ch) = (config->alignment == CENTER_ALIGNED ? 1 - duty : duty) * TIM1->ARR;
}
/**
 * @brief Configure the inverted pair
 *
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return PWMPairUpdateCallback Returns the function pointer of the type PWMPairUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
PWMPairUpdateCallback PWM11_16_ConfigPair(uint32_t pwmNo, pwm_pair_config_t* config)
{
	uint32_t ch = (pwmNo - 11);
	// If pair not selected correctly
	if(ch % 2 != 0)
		Error_Handler();
	ch = ch == 0 ? TIM_CHANNEL_1 : (ch == 2 ? TIM_CHANNEL_2 : TIM_CHANNEL_3);
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, ch) != HAL_OK)
		Error_Handler();
	return PWM11_16_UpdatePair;
}

/**
 * @brief Update the PWM Channels duty cycle
 *
 * @param pwmNo channel no. Valid Values (11, 13, 15)
 * @param duty duty cycle to be applied to the channel (Range 0-1)
 * @param *config Pointer to the channel configuration structure
 */
void PWM11_16_UpdateChannel(uint32_t pwmNo, float duty, pwm_ch_config_t* config)
{
	uint32_t ch = (pwmNo - 11) / 2;
	*(((uint32_t*)&(TIM1->CCR1)) + ch) = (config->alignment == CENTER_ALIGNED ? 1 - duty : duty) * TIM1->ARR;
}

/**
 * @brief Configure the PWM channel
 *
 * @param pwmNo channel no. Valid Values (11, 13, 15)
 * @param *config Pointer to the channel configuration structure
 * @return PWMUpdateCallback Returns the function pointer of the type PWMUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the channel needs to be updated
 */
PWMUpdateCallback PWM11_16_ConfigChannel(uint32_t pwmNo, pwm_ch_config_t* config)
{
	uint32_t ch = (pwmNo - 11);
	// Even pairs may only be selected as inverted channels
	if(ch % 2 != 0)
		Error_Handler();
	ch = ch == 0 ? TIM_CHANNEL_1 : (ch == 2 ? TIM_CHANNEL_2 : TIM_CHANNEL_3);
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, ch) != HAL_OK)
		Error_Handler();
	return PWM11_16_UpdateChannel;
}
#endif
/* EOF */
