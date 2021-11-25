/**
 ********************************************************************************
 * @file 		PWM1_10_Drivers.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 24, 2021
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
#ifndef PWM1_10_DRIVERS_H_
#define PWM1_10_DRIVERS_H_

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup PEController_Framework_Drivers
 * @{
 */

/** @addtogroup PWM
 * @{
 */

/** @defgroup PWM1-10 PWM 1-10
 * @brief This module contains the functionality and definitions for the PWMs (1-10)
 * @details This module has five separate time bases available for each paired PWM.
 * 	-# <b>PWM (1,2):</b> HRTIM_TIMERA
 * 	-# <b>PWM (3,4):</b> HRTIM_TIMERB
 * 	-# <b>PWM (5,6):</b> HRTIM_TIMERC
 * 	-# <b>PWM (7,8):</b> HRTIM_TIMERD
 * 	-# <b>PWM (9,10):</b> HRTIM_TIMERE<br>
 *
 * Each PWM can either be configured as individual channel or an inverted pair<br>
 * - <b>Individual Channels:</b><br>
 * For configuring the channels use PWM1_10_Drivers_ConfigChannels(),
 * whereas for updating the duty cycle of the output PWM use PWM1_10_UpdateChannelDuty().<br>
 * - <b>Inverted Pair Channels:</b><br>
 * For configuring the channels use PWM1_10_Drivers_ConfigInvertedPairs(),
 * whereas for updating the duty cycle of the output PWM use PWM1_10_UpdatePairDuty().<br>
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

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief Configures consecutive inverted pairs for PWM
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @param pairCount No of PWM pairs to be configured
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
extern DutyCycleUpdateFnc PWM1_10_Drivers_ConfigInvertedPairs(uint32_t pwmNo, pwm_config_t* config, int pairCount);
/**
 * @brief Update the Duty Cycle of an Inverted Pair
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param duty duty cycle to be applied to the pair (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
extern void PWM1_10_UpdatePairDuty(uint32_t pwmNo, float duty, pwm_config_t* config);
/**
 * @brief Configures consecutive PWM channels
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1-10)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @param chCount No of channels to be configured with the setting
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
extern DutyCycleUpdateFnc PWM1_10_Drivers_ConfigChannels(uint32_t pwmNo, pwm_config_t* config, int chCount);
/**
 * @brief Update the Duty Cycle of a channel
 * @param pwmNo PWM channel to be configured (Valid Values 1-10)
 * @param duty duty cycle to be applied to the channel (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channel
 */
extern void PWM1_10_UpdateChannelDuty(uint32_t pwmNo, float duty, pwm_config_t* config);
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
