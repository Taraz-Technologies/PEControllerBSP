/**
 ********************************************************************************
 * @file 		pecontroller_pwm1_10.h
 * @author 		Waqas Ehsan Butt
 * @date 		November 24, 2021
 *
 * @brief	Controls the PWM 1-10
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
#ifndef PECONTROLLER_PWM1_10_H
#define PECONTROLLER_PWM1_10_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup PWM
 * @{
 */

/** @defgroup PWM1-10 PWM 1-10
 * @brief Contains the declaration and procedures for the PWMs (1-10)
 * @details This module has five separate time bases available for each paired PWM.
 * 	-# <b>PWM (1,2):</b> HRTIM_TIMERA
 * 	-# <b>PWM (3,4):</b> HRTIM_TIMERB
 * 	-# <b>PWM (5,6):</b> HRTIM_TIMERC
 * 	-# <b>PWM (7,8):</b> HRTIM_TIMERD
 * 	-# <b>PWM (9,10):</b> HRTIM_TIMERE<br>
 *
 * Each PWM can either be configured as individual channel or an inverted pair<br>
 * - <b>Individual Channels:</b><br>
 * For configuring the channels use @ref BSP_PWM1_10_ConfigChannels(),
 * whereas for updating the duty cycle of the output PWM use @ref BSP_PWM1_10_UpdateChannelDuty().<br>
 * - <b>Inverted Pair Channels:</b><br>
 * For configuring the channels use @ref BSP_PWM1_10_ConfigInvertedPairs(),
 * whereas for updating the duty cycle of the output PWM use @ref BSP_PWM1_10_UpdatePairDuty().<br>
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "pecontroller_pwm_base.h"
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
/** @defgroup PWM1_10_Exported_Functions Functions
  * @{
  */
/**
 * @brief Configures consecutive inverted pairs for PWM
 * @param pwmNo Channel no of reference channel is the PWM pair (Valid Values 1-10). <br>
 * 				<b>Pairs are classified as :</b>
 * 				-# CH1 = Reference channel available at pin pwmNo
 * 				-# CH2 = Inverted Channel from reference available at pin pwmNo + 1 if pwmNo is odd else pwmNo - 1
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @param pairCount No of PWM pairs to be configured
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
extern DutyCycleUpdateFnc BSP_PWM1_10_ConfigInvertedPairs(uint32_t pwmNo, pwm_config_t* config, int pairCount);
/**
 * @brief Update the Duty Cycle of an Inverted Pair
 * @param pwmNo Channel no of reference channel is the PWM pair (Valid Values 1-10). <br>
 * 				<b>Pairs are classified as :</b>
 * 				-# CH1 = Reference channel available at pin pwmNo
 * 				-# CH2 = Inverted Channel from reference available at pin pwmNo + 1 if pwmNo is odd else pwmNo - 1
 * @param duty duty cycle to be applied to the pair (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return float Duty cycle applied in this cycle. May differ from the duty variable if outside permitted limits
 */
extern float BSP_PWM1_10_UpdatePairDuty(uint32_t pwmNo, float duty, pwm_config_t* config);
/**
 * @brief Configures consecutive PWM channels
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1-10)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @param chCount No of channels to be configured with the setting
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
extern DutyCycleUpdateFnc BSP_PWM1_10_ConfigChannels(uint32_t pwmNo, pwm_config_t* config, int chCount);
/**
 * @brief Update the Duty Cycle of a channel
 * @param pwmNo PWM channel to be configured (Valid Values 1-10)
 * @param duty duty cycle to be applied to the channel (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channel
 * @return float Duty cycle applied in this cycle. May differ from the duty variable if outside permitted limits
 */
extern float BSP_PWM1_10_UpdateChannelDuty(uint32_t pwmNo, float duty, pwm_config_t* config);
/**
 * @brief Enable / Disable interrupt for a PWM channel as per requirement
 * @param pwmNo Channel no of the PWM Channel (Range 1-10)
 * @param enable If enable interrupt set this parameter to <c>true</c>
 * @param callback Specifies the function to be called when the PWM is reset
 * @param priority Interrupt priority. Range (0-15). Here 0 is the highest priority
 */
extern void BSP_PWM1_10_Config_Interrupt(uint32_t pwmNo, bool enable, PWMResetCallback callback, int priority);
/********************************************************************************
 * Code
 *******************************************************************************/


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
#endif 
/* EOF */
