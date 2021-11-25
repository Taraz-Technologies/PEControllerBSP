/**
 ********************************************************************************
 * @file 		PWM11_16_Drivers.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 18, 2021
 *
 * @brief
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
#ifndef PWM11_16_DRIVERS_H_
#define PWM11_16_DRIVERS_H_

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup PEController_Framework_Drivers
 * @{
 */

/** @addtogroup PWM
 * @{
 */

/** @defgroup PWM11-16 PWM 11-16
 * @brief This module contains the functionality and definitions for the PWMs (11-16)
 * @details  This module has a single time base, so all PWMs under this module will operate on the same frequency.
 * 	This module can only be configured with three channel configurations. <br>The channels 12, 14 and 16 can
 * 	only be used as inverted PWM pairs of 11, 13 and 15 respectively and can 't be used separately
 * 		-# <b>PWM (11,12):</b> TIM1_CH1
 * 		-# <b>PWM (13,14):</b> TIM1_CH2
 * 		-# <b>PWM (15,16):</b> TIM1_CH3<br>
 *
 * The PWMs can either configured for individual or paired operation <br>
 * - <b>Individual Channels:</b><br>
 * For configuring the channels use PWM11_16_Drivers_ConfigChannels(),
 * whereas for updating the duty cycle of the output PWM use PWM11_16_UpdateChannelDuty().
 * The PWMs 12, 14, 16 can't be configured for individual channel operation.<br>
 * - <b>Inverted Pair Channels:</b><br>
 * For configuring the channels use PWM11_16_Drivers_ConfigInvertedPairs(),
 * whereas for updating the duty cycle of the output PWM use PWM11_16_UpdatePairDuty().<br>
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "PWMDriverBase.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** Exports the timer 1 handle for external use
 */
extern TIM_HandleTypeDef htim1;
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
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
extern DutyCycleUpdateFnc PWM11_16_Drivers_ConfigInvertedPairs(uint32_t pwmNo, pwm_config_t* config, int pairCount);
/**
 * @brief Update the Duty Cycle of an Inverted Pair
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 11,13,15)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param duty duty cycle to be applied to the pair (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
extern void PWM11_16_UpdatePairDuty(uint32_t pwmNo, float duty, pwm_config_t* config);
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
extern DutyCycleUpdateFnc PWM11_16_Drivers_ConfigChannels(uint32_t pwmNo, pwm_config_t* config, int chCount);
/**
 * @brief Update the Duty Cycle of a channel
 * @param pwmNo PWM channel to be configured (Valid Values 11,13,15)
 * @param duty duty cycle to be applied to the channel (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channel
 */
extern void PWM11_16_UpdateChannelDuty(uint32_t pwmNo, float duty, pwm_config_t* config);
/********************************************************************************
 * Code
 *******************************************************************************/


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

#endif 
/* EOF */
