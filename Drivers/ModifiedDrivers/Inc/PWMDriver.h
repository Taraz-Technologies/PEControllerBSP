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
#include "GeneralHeader.h"
#include <DigitalPins.h>
#include <stdbool.h>
#include <stddef.h>
#include <stm32h7xx_hal_tim.h>
#include <sys/_stdint.h>
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define HRTIM_FREQ					(480)
/*******************************************************************************
 * Enums
 ******************************************************************************/
typedef enum
{
	CENTER_ALIGNED,
	EDGE_ALIGNED,
} pwm_alignment_t;
/*******************************************************************************
 * Structs
 ******************************************************************************/
typedef struct
{
	bool dtEnabled;
	bool interruptEnabled;
	pwm_alignment_t alignment;
	uint32_t periodInUsec;
	uint32_t dtInNanoSec;
	float maxDutyCycle;  /* value auto computed to be between 0-1 */
} pwm_pair_config_t;
typedef struct
{
	pwm_alignment_t alignment;
	pwm_fnc_t fnc;
} pwm_channel_t;

typedef struct
{
	bool interruptEnabled;
	uint32_t periodInUsec;
	pwm_channel_t ch1;
	pwm_channel_t ch2;
} pwm_individual_config_t;

/**
 * @brief structure containing the configuration of a certain PWM channel
 *
 */
typedef struct
{
	bool interruptEnabled;
	pwm_alignment_t alignment;
	uint32_t periodInUsec;
} pwm_ch_config_t;

typedef void (*PWMPairUpdateCallback)(uint32_t pwmNo, float duty, pwm_pair_config_t *config);
typedef void (*PWMUpdateCallback)(uint32_t pwmNo, float duty, pwm_ch_config_t *config);
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * @brief Initialize the relevant PWM modules (high Precision timers)
 *
 */
void PWM1_10_Drivers_Init(void);
/**
 * @brief Configure the inverted pair
 *
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return PWMPairUpdateCallback Returns the function pointer of the type PWMPairUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
PWMPairUpdateCallback PWM1_10_ConfigPair(uint32_t pwmNo, pwm_pair_config_t *config);
/**
 * @brief Update the PWM of the Channels
 *
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param duty duty cycle to be applied to the pair (Range 0-1)
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
void PWM1_10_UpdatePair(uint32_t pwmNo, float duty, pwm_pair_config_t *config);
/**
 * @brief Configure the PWM channel
 *
 * @param pwmNo channel no
 * @param *config Pointer to the channel configuration structure
 * @return PWMUpdateCallback Returns the function pointer of the type PWMUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the channel needs to be updated
 */
PWMUpdateCallback PWM1_10_ConfigChannel(uint32_t pwmNo, pwm_ch_config_t* config);
/**
 * @brief Update the PWM Channels duty cycle
 *
 * @param pwmNo channel no
 * @param duty duty cycle to be applied to the channel (Range 0-1)
 * @param *config Pointer to the channel configuration structure
 */
void PWM1_10_UpdateChannel(uint32_t pwmNo, float duty, pwm_ch_config_t* config);
/**
 * @brief Initialize the relevant PWM modules (Timer1). Frequency is constant for the PWMs 11-16
 *
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
void PWM11_16_Drivers_Init(pwm_pair_config_t *config);
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
PWMPairUpdateCallback PWM11_16_ConfigPair(uint32_t pwmNo, pwm_pair_config_t *config);
/**
 * @brief Update the PWM of the Channels
 *
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param duty duty cycle to be applied to the pair (Range 0-1)
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
void PWM11_16_UpdatePair(uint32_t pwmNo, float duty, pwm_pair_config_t *config);
/**
 * @brief Configure the PWM channel
 *
 * @param pwmNo channel no. Valid Values (11, 13, 15)
 * @param *config Pointer to the channel configuration structure
 * @return PWMUpdateCallback Returns the function pointer of the type PWMUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the channel needs to be updated
 */
PWMUpdateCallback PWM11_16_ConfigChannel(uint32_t pwmNo, pwm_ch_config_t* config);
/**
 * @brief Update the PWM Channels duty cycle
 *
 * @param pwmNo channel no. Valid Values (11, 13, 15)
 * @param duty duty cycle to be applied to the channel (Range 0-1)
 * @param *config Pointer to the channel configuration structure
 */
void PWM11_16_UpdateChannel(uint32_t pwmNo, float duty, pwm_ch_config_t* config);
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern TIM_HandleTypeDef htim1;
/*******************************************************************************
 * Code
 ******************************************************************************/
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
/* EOF */
