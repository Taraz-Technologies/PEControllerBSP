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

typedef void (*UpdatePWMPair)(uint32_t pwmNo, float duty, pwm_pair_config_t *config);
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
 * @return UpdatePWMPair Returns the function pointer of the type UpdatePWMPair which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
UpdatePWMPair PWM1_10_ConfigPair(uint32_t pwmNo, pwm_pair_config_t *config);
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
 * @return UpdatePWMPair Returns the function pointer of the type UpdatePWMPair which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
UpdatePWMPair PWM11_16_ConfigPair(uint32_t pwmNo, pwm_pair_config_t *config);
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
 * @return UpdatePWMPair Returns the function pointer of the type UpdatePWMPair which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
static UpdatePWMPair PWMDriver_ConfigPair(uint16_t pwmNo,
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

#endif
/* EOF */
