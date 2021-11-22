/**
 ********************************************************************************
 * @file 		PWM1_10_Drivers.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 18, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */

#ifndef PWM1_10_DRIVERS_H_
#define PWM1_10_DRIVERS_H_

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
/**
 * @brief Configures consecutive inverted pairs for PWM
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @param pairCount No of PWM pairs to be configured
 * @return PWMPairUpdateCallback Returns the function pointer of the type PWMPairUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
PWMPairUpdateCallback PWM1_10_Drivers_ConfigInvertedPairs(uint32_t pwmNo, pwm_config_t* config, int pairCount);
/**
 * @brief Configures consecutive PWM channels
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1-10)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @param chCount No of channels to be configured with the setting
 * @return PWMPairUpdateCallback Returns the function pointer of the type PWMPairUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
PWMPairUpdateCallback PWM1_10_Drivers_ConfigChannels(uint32_t pwmNo, pwm_config_t* config, int chCount);
#if 0
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
 * @return PWMPairUpdateCallback Returns the function pointer of the type PWMPairUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
PWMPairUpdateCallback PWM1_10_ConfigPair(uint32_t pwmNo, pwm_pair_config_t *config);
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
 * @brief Configure the PWM channel
 *
 * @param pwmNo channel no
 * @param *config Pointer to the channel configuration structure
 * @return PWMUpdateCallback Returns the function pointer of the type PWMUpdateCallback which needs to be called
 * 						  whenever the duty cycles of the channel needs to be updated
 */
PWMUpdateCallback PWM1_10_ConfigChannel(uint32_t pwmNo, pwm_ch_config_t* config);
/**
 * @brief Update the PWM Channels duty cycle
 *
 * @param pwmNo channel no
 * @param duty duty cycle to be applied to the channel (Range 0-1)
 * @param *config Pointer to the channel configuration structure
 */
void PWM1_10_UpdateChannel(uint32_t pwmNo, float duty, pwm_ch_config_t* config);
#endif
/********************************************************************************
 * Code
 *******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
