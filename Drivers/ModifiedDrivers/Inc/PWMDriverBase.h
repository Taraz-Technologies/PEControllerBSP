/**
 ********************************************************************************
 * @file 		PWMDriverBase.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 18, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */

#ifndef PWMDRIVERBASE_H_
#define PWMDRIVERBASE_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "GeneralHeader.h"
#include <DigitalPins.h>
#include <stdbool.h>
#include <stddef.h>
#include <stm32h7xx_hal_tim.h>
#include <sys/_stdint.h>
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
/**
 * @brief PWM alignment definitions
 */
typedef enum
{
	CENTER_ALIGNED,		/**< Aligns the PWM to center. Preferred in most control systems */
	EDGE_ALIGNED,		/**< Aligns the PWM to the left edge */
} pwm_alignment_t;
/**
 * @brief Output duty cycle computation definitions in case of PWM pairs.
 * 			This decides the signal from which the dead time should be removed.
 */
typedef enum
{
	OUTPUT_DUTY_AT_PWMH,              	/**< The output duty cycle at high side switch is the same as
											the supplied duty cycle. Dead time will be deducted on both edges from
											the low side switch  */
	OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH	/**< The output duty cycle at high side switch is D-dead time.
											On one edge duty cycle will be removed from high side whereas
											on the other edge duty cycle will be removed from the lower side switch */
} duty_mode_t;
/**
 * @brief Defines the PWM reset callback
 */
typedef void (*PWMResetCallback)(void);
/********************************************************************************
 * Structures
 *******************************************************************************/
/**
 * @brief Define the practical limit for the duty cycle of specified channels.
 * @note In-case no limit is applied limits are auto-computed and will be in the range 0 to 1
 */
typedef struct
{
	float max;						/**< @brief Specifies the maximum duty cycle limit for the PWM (Range 0-1) */
	float min;						/**< @brief Specifies the minimum duty cycle limit for the PWM (Range 0-1) */
	bool minMaxDutyCycleBalancing; 	/**< @brief If enabled balances the shortage in maximum duty cycle with the
	 	 	 	 	 	 	 	 	 	 minimum duty cycle. e.g. if the maximum duty cycle after deducting
	 	 	 	 	 	 	 	 	 	 dead time is evaluated to be 0.95 the minimum duty cycle will be limited
	 	 	 	 	 	 	 	 	 	 to 0.05 so that the deviation from center point (0.5 duty cycle)
	 	 	 	 	 	 	 	 	 	 in both directions is identical */
} duty_limits_t;
/**
 * @brief Defines the parameters for a specific PWM module.
 */
typedef struct
{
	bool dtEnabled;					/**< @brief Controls dead time insertion in a pair */
	uint32_t dtInNanoSec;			/**< @brief Value of dead time in nano-seconds if enabled */
	bool interruptEnabled;			/**< @brief Controls interrupt callback at PWM reset */
	pwm_alignment_t alignment;		/**< @brief Specifies the alignment of PWM */
	uint32_t periodInUsec;			/**< @brief Specifies the period of the PWM in micro-seconds */
	PWMResetCallback callback;		/**< @brief Specifies the function to be called when the PWM is reset */
} pwm_module_config_t;
/**
 * @brief Defines the parameters for specific PWM configuration
 */
typedef struct
{
	duty_limits_t lim;				/**< @brief Defines the duty cycle limits for the specified channels */
	duty_mode_t dutyMode;			/**< @brief Controls the duty cycle computation mode */
	pwm_module_config_t* module;	/**< @brief Define the module configuration */
} pwm_config_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
typedef void (*PWMPairUpdateCallback)(uint32_t pwmNo, float duty, pwm_config_t *config); /* --todo-- change name */
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
