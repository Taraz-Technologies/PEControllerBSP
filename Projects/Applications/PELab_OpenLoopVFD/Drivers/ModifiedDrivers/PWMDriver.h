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
#include "DoutController.h"
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

typedef void (*UpdatePWMPair)(uint32_t pwmNo, float duty, pwm_pair_config_t* config);
 /*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*! @brief initialize the pwm modules */
void PWM1_10_Drivers_Init(void);
/*! @brief Confiure the inverted pair */
UpdatePWMPair PWM1_10_ConfigPair(uint32_t pwmNo, pwm_pair_config_t* config);
/*! @brief update the duty cycle from 0-1 */
void PWM1_10_UpdatePair(uint32_t pwmNo, float duty, pwm_pair_config_t* config);
/*! @brief initialize drivers can't change the frequency and timer settings for individual channels */
void PWM11_16_Drivers_Init(pwm_pair_config_t* config);
/*! @brief Confiure the inverted pair */
UpdatePWMPair PWM11_16_ConfigPair(uint32_t pwmNo, pwm_pair_config_t* config);
/*! @brief update the duty cycle from 0-1 */
void PWM11_16_UpdatePair(uint32_t pwmNo, float duty, pwm_pair_config_t* config);
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern HRTIM_HandleTypeDef hhrtim;
extern TIM_HandleTypeDef htim1;
/*******************************************************************************
 * Code
 ******************************************************************************/
static UpdatePWMPair PWMDriver_ConfigPair(uint16_t pwmNo, pwm_pair_config_t* config)
{
	if(pwmNo <= 10)
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

static void PWMDriver_UpdatePair(uint16_t pwmNo, float duty, pwm_pair_config_t* config)
{
	if(pwmNo <= 10)
		PWM1_10_UpdatePair(pwmNo, duty, config);
	else if (pwmNo <= 16)
		PWM11_16_UpdatePair(pwmNo, duty, config);
}

#endif
/* EOF */
