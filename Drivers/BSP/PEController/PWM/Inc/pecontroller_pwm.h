/**
 ********************************************************************************
 * @file 		pecontroller_pwm.h
 * @author 		Waqas Ehsan Butt
 * @date 		November 18, 2021
 *
 * @brief	Controls the PWM functionality and definitions
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

#ifndef PECONTROLLER_PWM_H
#define PECONTROLLER_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @defgroup PWM PWM
 * @brief Contains the declaration and procedures relating the PWM drivers
 * @details The available drivers are currently only supported for PWMs (1-16).
 * The drivers are further subdivided in the following groups based upon the governing timer modules.
 * 	-# <b>PWM (1-10):</b>	HRTIM<br>
 * 	This module has five separate time bases available for each paired PWM
 * 		-# <b>PWM (1,2):</b> HRTIM_TIMERA
 * 		-# <b>PWM (3,4):</b> HRTIM_TIMERB
 * 		-# <b>PWM (5,6):</b> HRTIM_TIMERC
 * 		-# <b>PWM (7,8):</b> HRTIM_TIMERD
 * 		-# <b>PWM (9,10):</b> HRTIM_TIMERE
 * 	-# <b>PWM (11-16):</b>	TIM1<br>
 * 	This module has a single time base, so all PWMs under this module will operate on the same frequency.
 * 	This module can only be configured with three channel configurations. <br>The channels 12,14 and 16 can
 * 	only be used as inverted PWM pairs of 11,13 and 15 respectively and can 't be used separately
 * 		-# <b>PWM (11,12):</b> TIM1_CH1
 * 		-# <b>PWM (13,14):</b> TIM1_CH2
 * 		-# <b>PWM (15,16):</b> TIM1_CH3
 * @{
 */

/** @defgroup PWMBase PWM Info
 * @brief Contains the declaration and procedures relating the base PWM drivers
 * @details The PWM channels can be configured in individual / inverted pair form
 * 		-# <b>Individual PWM:</b><br>
 * 		To configure the channel for Individual PWM use @ref BSP_PWM_ConfigChannel().
 * 		Once configured the duty cycle of the relevant channel can be updated by the function @ref BSP_PWM_UpdateChannelDuty().
 * 		-# <b>Inverted Pair PWM:</b><br>
 * 		To configure the channel for Individual PWM use @ref BSP_PWM_ConfigInvertedPair().
 * 		Once configured the duty cycle of the relevant channels can be updated by the function @ref BSP_PWM_UpdatePairDuty().
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "pecontroller_pwm1_10.h"
#include "pecontroller_pwm11_16.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/

/*******************************************************************************
 * Structures
 ******************************************************************************/
/** @addtogroup PWM_Exported_Structures
  * @{
  */
/**
 * @brief Defines the parameters for an independent (individual/Pair) PWM
 */
typedef struct
{
	uint16_t pinNo;							/**< @brief Defines the starting pin no of the specified PWMs.
												Other associated PWMs if any should be consecutively after this pin */
	DutyCycleUpdateFnc dutyUpdateFnc;		/**< @brief Function to be used for updating the duty cycle */
	pwm_config_t pwmConfig;					/**< @brief The PWM configurations */
} independent_pwm_config_t;
/**
 * @}
 */
/*******************************************************************************
 * Exported Variables
 ******************************************************************************/

/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @addtogroup PWM_Exported_Functions
  * @{
  */

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Configures an PWM pair as inverted pair
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9,11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
static DutyCycleUpdateFnc BSP_PWM_ConfigInvertedPair(uint16_t pwmNo, pwm_config_t *config)
{
	if (pwmNo <= 10)
		return BSP_PWM1_10_ConfigInvertedPairs(pwmNo, config, 1);
	else if (pwmNo <= 16)
		return BSP_PWM11_16_ConfigInvertedPairs(pwmNo, config, 1);
	return NULL;
}
/**
 * @brief Configures a PWM channel
 * @param pwmNo PWM channel to be configured (Valid Values 1-10,11,13,15)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
static DutyCycleUpdateFnc BSP_PWM_ConfigChannel(uint16_t pwmNo, pwm_config_t *config)
{
	if (pwmNo <= 10)
		return BSP_PWM1_10_ConfigChannels(pwmNo, config, 1);
	else if (pwmNo <= 16)
		return BSP_PWM11_16_ConfigChannels(pwmNo, config, 1);
	return NULL;
}

/**
 * @brief Update the Duty Cycle of an Inverted Pair
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9,11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param duty duty cycle to be applied to the pair (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
static void BSP_PWM_UpdatePairDuty(uint32_t pwmNo, float duty, pwm_config_t* config)
{
	if (pwmNo <= 10)
		BSP_PWM1_10_UpdatePairDuty(pwmNo, duty, config);
	else if (pwmNo <= 16)
		BSP_PWM11_16_UpdatePairDuty(pwmNo, duty, config);
}

/**
 * @brief Update the Duty Cycle of a channel
 * @param pwmNo PWM channel to be configured (Valid Values 1-10, 11, 13, 15)
 * @param duty duty cycle to be applied to the channel (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channel
 */
static void BSP_PWM_UpdateChannelDuty(uint32_t pwmNo, float duty, pwm_config_t* config)
{
	if (pwmNo <= 10)
		BSP_PWM1_10_UpdateChannelDuty(pwmNo, duty, config);
	else if (pwmNo <= 16)
		BSP_PWM11_16_UpdateChannelDuty(pwmNo, duty, config);
}

#endif

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
/* EOF */
