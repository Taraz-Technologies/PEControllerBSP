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
#include "pecontroller_pwm_base.h"
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
/**
 * @brief Configures an PWM pair as inverted pair
 * @param pwmNo Channel no of reference channel is the PWM pair (Valid Values 1-16). <br>
 * 				<b>Pairs are classified as :</b>
 * 				-# CH1 = Reference channel available at pin pwmNo
 * 				-# CH2 = Inverted Channel from reference available at pin pwmNo + 1 if pwmNo is odd else pwmNo - 1
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
extern DutyCycleUpdateFnc BSP_PWM_ConfigInvertedPair(uint16_t pwmNo, pwm_config_t *config);
/**
 * @brief Configures a PWM channel
 * @param pwmNo PWM channel to be configured (Valid Values 1-16)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
extern DutyCycleUpdateFnc BSP_PWM_ConfigChannel(uint16_t pwmNo, pwm_config_t *config);

/**
 * @brief Update the Duty Cycle of an Inverted Pair
 * @param pwmNo Channel no of reference channel is the PWM pair (Valid Values 1-16). <br>
 * 				<b>Pairs are classified as :</b>
 * 				-# CH1 = Reference channel available at pin pwmNo
 * 				-# CH2 = Inverted Channel from reference available at pin pwmNo + 1 if pwmNo is odd else pwmNo - 1
 * @param duty duty cycle to be applied to the pair (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @return float Duty cycle applied in this cycle. May differ from the duty variable if outside permitted limits
 */
extern float BSP_PWM_UpdatePairDuty(uint32_t pwmNo, float duty, pwm_config_t* config);

/**
 * @brief Update the Duty Cycle of a channel
 * @param pwmNo PWM channel to be configured (Valid Values 1-16)
 * @param duty duty cycle to be applied to the channel (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channel
 * @return float Duty cycle applied in this cycle. May differ from the duty variable if outside permitted limits
 */
extern float BSP_PWM_UpdateChannelDuty(uint32_t pwmNo, float duty, pwm_config_t* config);

/**
 * @brief Enable / Disable interrupt for a PWM channel as per requirement
 * @param pwmNo Channel no of the PWM Channel (Range 1-16)
 * @param enable If enable interrupt set this parameter to <c>true</c>
 * @param callback Specifies the function to be called when the PWM is reset
 * @param priority Interrupt priority. Range (0-15). Here 0 is the highest priority
 */
extern void BSP_PWM_Config_Interrupt(uint32_t pwmNo, bool enable, PWMResetCallback callback, int priority);
/**
 * @brief Starts the PWM on required PWM pins
 * @code
 * // Start PWM for channel 1
 * BSP_PWM_Start(0x1);
 * // Start PWM for channel 1 and channel 2
 * BSP_PWM_Start(0x3);
 * // Start PWM for channel n and channel m, where n & m are between 1 & 16
 * BSP_PWM_Start((1U << (n - 1)) | (1U << (m - 1)));
 * @endcode
 * @param pwmMask Set the PWM channels needed to be run.<br>
 * 				<b>Valid Range</b> =  (0x0001 - 0xffff)
 */
extern void BSP_PWM_Start(uint32_t pwmMask);
/**
 * @brief Stops the PWM on required PWM pins
 * @code
 * // Stop PWM for channel 1
 * BSP_PWM_Stop(0x1);
 * // Stop PWM for channel 1 and channel 2
 * BSP_PWM_Stop(0x3);
 * // Stop PWM for channel n and channel m, where n & m are between 1 & 16
 * BSP_PWM_Stop((1U << (n - 1)) | (1U << (m - 1)));
 * @endcode
 * @param pwmMask Set the PWM channels needed to be stopped.<br>
 * 				<b>Valid Range</b> =  (0x0001 - 0xffff)
 */
extern void BSP_PWM_Stop(uint32_t pwmMask);
/**
 * @brief Populates the provided @ref pwm_module_config_t parameter with the default configuration
 * @details <b>Default Configuration</b>:
 * -# alignment = CENTER_ALIGNED
 * -# deadtime.on = false
 * -# deadtime.nanoSec = 1000
 * -# periodInUsec = 40
 * @param moduleConfig module configuration to be updated
 */
extern void BSP_PWM_GetDafaultModuleConfig(pwm_module_config_t* moduleConfig);
/**
 * @brief Populates the provided @ref pwm_config_t parameter with the default configuration
 * @details <b>Default Configuration</b>:
 * -# dutyMode = OUTPUT_DUTY_AT_PWMH
 * -# lim.min = 0
 * -# lim.max = 1
 * -# lim.minMaxDutyCycleBalancing = false
 * @param pwmConfig PWM configuration structure to be updated
 * @param moduleConfig module configuration used by this module. Make sure to call
 * @ref BSP_PWM_GetDafaultModuleConfig() before calling this function
 */
extern void BSP_PWM_GetDefaultConfig(pwm_config_t* pwmConfig, pwm_module_config_t* moduleConfig);
/**
 * @brief Enable disable the output for required PWM channels
 * @code
 * // Enable PWM output for channel 1
 * BSP_PWMOut_Enable(0x1, true);
 * // Enable PWM output for channel 1 and channel 2
 * BSP_PWMOut_Enable(0x3, true);
 * // Disable PWM output for channel n and channel m, where n & m are between 1 & 16
 * BSP_PWMOut_Enable((1U << (n - 1)) | (1U << (m - 1)), false);
 * @endcode
 * @param pwmMask Set the PWM channels needed to be run.<br>
 * 				<b>Valid Range</b> =  (0x0001 - 0xffff)
 * @param en <c>true</c> if needs to be enabled else <c>false</c>
 */
extern void BSP_PWMOut_Enable(uint32_t pwmMask, bool en);
/*******************************************************************************
 * Code
 ******************************************************************************/


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
