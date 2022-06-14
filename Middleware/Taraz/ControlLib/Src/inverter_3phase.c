/**
 ********************************************************************************
 * @file    	inverter_3phase.c
 * @author 		Waqas Ehsan Butt
 * @date    	Nov 25, 2021
 *
 * @brief   File for control of three phase inverter
 ********************************************************************************
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
static float Tnpc_PWM_UpdatePair(uint32_t pwmNo, float duty, pwm_config_t *config)
{
	float d1 = duty;
	duty = (fabsf(duty - 0.5f)) * 2;
	if(d1 < .5f)
	{
		BSP_PWM_UpdatePairDuty(pwmNo, 0, config);
		BSP_PWM_UpdatePairDuty(pwmNo + 2, 1 - duty, config);
	}
	else
	{
		BSP_PWM_UpdatePairDuty(pwmNo, duty, config);
		BSP_PWM_UpdatePairDuty(pwmNo + 2, 1, config);
	}
	return d1;
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
		callback = Tnpc_PWM_UpdatePair; 				/* use this function to update all 4 switch duty cycles */
		BSP_Dout_SetAsPWMPin(pwmNo + 2);
		BSP_Dout_SetAsPWMPin(pwmNo + 3);
	}
	return callback;
}

/**
 * @brief Enable/Disable the PWMs for a single leg of the inverter
 *
 * @param *config Pointer to the Inverter Configurations
 * @param pwmNo Channel no of the initial switch of the leg.
 * @param en True if needs to be enabled else false
 */
static void EnableSingleLeg(inverter3Ph_config_t* config, uint16_t pwmNo, bool en)
{
	BSP_PWMOut_Enable(((config->legType == LEG_TNPC ? 15U : 3U) << (pwmNo - 1)) , en);
}

/**
 * @brief Initialize an inverter module
 *
 * @param *config Pointer to the Inverter Configurations
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

	// if the duplicate pin is defined also intialize it
	if (config->s1PinDuplicate)
	{
		config->updateCallbackDuplicate = ConfigSingleLeg(config->s1PinDuplicate, config);
		config->updateCallbackDuplicate(config->s1PinDuplicate, 0.5f, &config->pwmConfig);
	}

	// enable the pwm signals by disabling any disable feature. Disable is by default active high
	for (int i = 0; i < config->dsblPinCount; i++)
		BSP_Dout_SetAsIOPin(config->dsblPinNo + i, GPIO_PIN_RESET);

	// Deactivate the inverter at startup
	Inverter3Ph_Activate(config, false);
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

	// if the duplicate pin is defined also process it
	if (config->s1PinDuplicate)
		config->updateCallbackDuplicate(config->s1PinDuplicate, duties[2], &config->pwmConfig);
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

/**
 * @brief Activate/Deactive the 3-Phase inverter output
 * @param *config handle representing the inverter
 * @param en <c>true</c> if needs to be enabled, else <c>false</c>
 */
void Inverter3Ph_Activate(inverter3Ph_config_t* config, bool en)
{
	for (int i = 0; i < 3; i++)
		EnableSingleLeg(config, config->s1PinNos[i], en);
	if (config->s1PinDuplicate)
		EnableSingleLeg(config, config->s1PinDuplicate, en);
	config->state = en ? INVERTER_ACTIVE : INVERTER_INACTIVE;
}

/* EOF */
