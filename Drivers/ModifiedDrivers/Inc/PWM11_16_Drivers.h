/**
 ********************************************************************************
 * @file 		PWM11_16_Drivers.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 18, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */

#ifndef PWM11_16_DRIVERS_H_
#define PWM11_16_DRIVERS_H_

#ifdef __cplusplus
extern "C" {
#endif

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
#if 0
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
#endif
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
