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
/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief initialize drivers can't change the frequency and timer settings for individual channels */
void PWM11_16_Drivers_Init(pwm_pair_config_t* config)
{
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
}

/*! @brief update the duty cycle from 0-1 */
void PWM11_16_UpdatePair(uint32_t pwmNo, float duty, pwm_pair_config_t* config)
{
	uint32_t ch = (pwmNo - 11) / 2; 
	*(((uint32_t*)&(TIM1->CCR1)) + ch) = (config->alignment == CENTER_ALIGNED ? 1 - duty : duty) * TIM1->ARR;
}

/*! @brief Confiure the inverted pair */
UpdatePWMPair PWM11_16_ConfigPair(uint32_t pwmNo, pwm_pair_config_t* config)
{
	uint32_t ch = (pwmNo - 11);
	ch = ch == TIM_CHANNEL_1 ? 0 : (ch == 2 ? TIM_CHANNEL_2 : TIM_CHANNEL_3);
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, ch) != HAL_OK)
    Error_Handler();
	return PWM11_16_UpdatePair;
}
/* EOF */
