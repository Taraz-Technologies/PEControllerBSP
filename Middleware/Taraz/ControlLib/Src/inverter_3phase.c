/*!
@file Inverter3Ph.c
@brief file for control of three phase inverter
@details

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "inverter_3phase.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define PI    										(3.141593f)
#define TWO_PI 										(2 * PI)
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

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief
 *
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9,11,13)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * 				 Channel3 = pwmNo + 2
 * 				 Channel3 = pwmNo + 3
 * @param duty duty cycle to be applied to the pair (Range 0 - 1)
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
static void TnpcPWM_UpdatePair(uint32_t pwmNo, float duty, pwm_config_t *config)
{
	duty = (fabsf(duty - 0.5f)) * 2;
	if(duty < 0)
	{
		BSP_PWM_UpdatePairDuty(pwmNo, 0, config);
		BSP_PWM_UpdatePairDuty(pwmNo + 2, duty, config);
	}
	else
	{
		BSP_PWM_UpdatePairDuty(pwmNo, duty, config);
		BSP_PWM_UpdatePairDuty(pwmNo + 2, 0, config);
	}
}
/**
 * @brief Configure the PWMs for a single leg of the inverter
 *
 * @param pwmNo Channel no of the initial switch of the leg.
 * @param *config pointer to the configuration of the 3 phase inverter
 * @return PWMPairUpdateCallback Returns the function pointer of the type PWMPairUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the leg needs to be updated
 */
static DutyCycleUpdateFnc ConfigSingleLeg(uint16_t pwmNo, inverter3Ph_config_t* config)
{
	DutyCycleUpdateFnc callback = BSP_PWM_ConfigInvertedPair(pwmNo, &config->pwmConfig);
	BSP_Dout_SetAsPWMPin(pwmNo);
	BSP_Dout_SetAsPWMPin(pwmNo + 1);
	/* for Tnpc use four switches */
	if (config->legType == LEG_TNPC)
	{
		BSP_PWM_ConfigInvertedPair(pwmNo + 2, &config->pwmConfig);
		callback = TnpcPWM_UpdatePair; 				/* use this function to update all 4 switch duty cycles */
		BSP_Dout_SetAsPWMPin(pwmNo + 2);
		BSP_Dout_SetAsPWMPin(pwmNo + 3);
	}

	return callback;
}

/**
 * @brief Initialize an inverter module
 *
 * @param *config Pointer to the Inverter Configurations
 * @return *inverter3Ph_config_t handle representing the inverter
 */
void Inverter3Ph_Init(inverter3Ph_config_t* config)
{
	/* an even output pin should be used as inverted signals are required for the PWM */
	if((config->s1PinNos[0] % 2 == 0) || (config->s1PinNos[1] % 2 == 0) || (config->s1PinNos[2] % 2 == 0))
		return;

	// configure PWM
	for (int i = 0; i < 3; i++)
	{
		config->updateCallbacks[i] = ConfigSingleLeg(config->s1PinNos[i], config);
		config->updateCallbacks[i](config->s1PinNos[i], 0.5f, &config->pwmConfig);
	}

	// enable the pwm signals by disabling any disable feature. Disable is by default active high
	for (int i = 0; i < config->dsblPinCount; i++)
		BSP_Dout_SetAsIOPin(config->dsblPinNo + i, GPIO_PIN_RESET);
}

/**
 * @brief Update the duty cycles of the inverter
 *
 * @param *config handle representing the inverter
 * @param *duties pointer to the three duty cycles of the inverter (Range 0-1)
 */
void Inverter3Ph_UpdateDuty(inverter3Ph_config_t* config, float* duties)
{
	for (int i = 0; i < 3; i++)
		config->updateCallbacks[i](config->s1PinNos[i], duties[i], &config->pwmConfig);
}

/**
 * @brief Update the duty cycles of the inverter by using SPWM configuration
 *
 * @param *config handle representing the inverter
 * @param *theta angle of phase u in radians
 * @param modulationIndex modulation index to be used for the generation
 */
void Inverter3Ph_UpdateSPWM(inverter3Ph_config_t* config, float theta, float modulationIndex)
{
	float duties[3];
	ComputeDuty_SPWM(theta, modulationIndex, duties);
	Inverter3Ph_UpdateDuty(config, duties);
}


/* EOF */
