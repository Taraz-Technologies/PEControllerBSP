/*! 
 @file PWMDriver.h
 @brief
 @details

 @author Waqas Ehsan Butt
 @copyright Taraz Technologies Pvt. Ltd.
 */
#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "PWM1_10_Drivers.h"
#include "PWM11_16_Drivers.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define HRTIM_FREQ					(480)					// --todo-- centralize
/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/
//typedef void (*PWMPairUpdateCallback)(uint32_t pwmNo, float duty, pwm_pair_config_t *config);
//typedef void (*PWMUpdateCallback)(uint32_t pwmNo, float duty, pwm_ch_config_t *config);
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern TIM_HandleTypeDef htim1;
/*******************************************************************************
 * Code
 ******************************************************************************/
static PWMPairUpdateCallback PWMDriver_ConfigPair(uint16_t pwmNo, pwm_config_t *config, int pairCount)
{
	return PWM1_10_Drivers_ConfigInvertedPairs(pwmNo, config, pairCount);
}
static PWMPairUpdateCallback PWMDriver_ConfigChannels(uint16_t pwmNo, pwm_config_t *config, int chCount)
{
	return PWM1_10_Drivers_ConfigChannels(pwmNo, config, chCount);
}
#if 0
/**
 * @brief Configures a Pair for Inverted Operation
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9,11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return PWMPairUpdateCallback Returns the function pointer of the type PWMPairUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
static PWMPairUpdateCallback PWMDriver_ConfigPair(uint16_t pwmNo,
		pwm_pair_config_t *config)
{
	if (pwmNo <= 10)
	{
		PWM1_10_Drivers_Init();
		return PWM1_10_ConfigPair(pwmNo, config);
	}
	else if (pwmNo <= 16)
	{
		PWM11_16_Drivers_Init(config);
		return PWM11_16_ConfigPair(pwmNo, config);
	}
	return NULL;
}

/**
 * @brief Update the PWM of the Channels
 *
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9,11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param duty duty cycle to be applied to the pair (Range 0-1)
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @note For better efficiency use the function pointer provided by the PWMDriver_ConfigPair
 */
static void PWMDriver_UpdatePair(uint16_t pwmNo, float duty,
		pwm_pair_config_t *config)
{
	if (pwmNo <= 10)
		PWM1_10_UpdatePair(pwmNo, duty, config);
	else if (pwmNo <= 16)
		PWM11_16_UpdatePair(pwmNo, duty, config);
}

/**
 * @brief Configure the PWM channel
 *
 * @param pwmNo channel no
 * @param *config Pointer to the channel configuration structure
 * @return PWMUpdateCallback Returns the function pointer of the type PWMUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the channel needs to be updated
 */
static PWMUpdateCallback PWMDriver_ConfigChannel(uint16_t pwmNo,
		pwm_ch_config_t *config)
{
	if (pwmNo <= 10)
	{
		PWM1_10_Drivers_Init();
		return PWM1_10_ConfigChannel(pwmNo, config);
	}
	else if (pwmNo <= 16)
	{
		pwm_pair_config_t pairConfig = {
						.alignment = config->alignment,
						.dtEnabled = false,
						.interruptEnabled = config->interruptEnabled,
						.periodInUsec = config->periodInUsec
				};
		PWM11_16_Drivers_Init(&pairConfig);
		return PWM11_16_ConfigChannel(pwmNo, config);
	}
	return NULL;
}

/**
 * @brief Update the PWM Channels' duty cycle
 *
 * @param pwmNo channel no
 * @param duty duty cycle to be applied to the channel (Range 0-1)
 * @param *config Pointer to the channel configuration structure
 */
static void PWMDriver_UpdateChannel(uint32_t pwmNo, float duty, pwm_ch_config_t* config)
{
	if (pwmNo <= 10)
		PWM1_10_UpdateChannel(pwmNo, duty, config);
	else if (pwmNo <= 16)
		PWM11_16_UpdateChannel(pwmNo, duty, config);
}
#endif
#endif
/* EOF */
