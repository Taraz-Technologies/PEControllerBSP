/**
 ********************************************************************************
 * @file 		pecontroller_pwm_base.h
 * @author 		Waqas Ehsan Butt
 * @date 		November 18, 2021
 *
 * @brief    Controls the PWM base functionality and definitions
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

#ifndef PECONTROLLER_PWM_BASE_H
#define PECONTROLLER_PWM_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup PWM
 * @{
 */

/** @addtogroup PWMBase
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "pecontroller_digital_pins.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup PWM_Exported_Typedefs Type Definitions
  * @{
  */
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
 * @brief Defines the synchronization sources available for the PWM channels
 * @note All sources are not available for all PWM channels. See details for relevant information.
 *
 */
typedef enum
{
	PWM_SYNC_SRC_NONE,               /**< No synchronization */
	PWM_SYNC_SRC_TIM1,               /**< Synchronizes with TIM1 trigger.
									This event is triggered on TIM1 start and TIM1 reset events.
									This event is available for all PWM channels.  */
	PWM_SYNC_SRC_TIM2,               /**< Synchronizes with TIM2 trigger.
									This event can be configured using @ref BSP_TIM2_ConfigFiberRx().
									This event can start and reset PWM11-16, while it can only reset PWM1-10   */
	PWM_SYNC_SRC_TIM3,               /**< Synchronizes with TIM3 trigger.
									This event can be configured using @ref BSP_TIM3_ConfigFiberTx().
									This event can start and reset PWM11-16, while it can only reset PWM1-10   */
	PWM_SYNC_SRC_HRTIM_MASTER_PERIOD,/**< Synchronizes with HRTim Master Period Completion.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
	PWM_SYNC_SRC_HRTIM_MASTER_CMP1,  /**< Synchronizes with HRTim Master Compare 1 vaue matching.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
	PWM_SYNC_SRC_HRTIM_MASTER_CMP2,  /**< Synchronizes with HRTim Master Compare 2 vaue matching.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
	PWM_SYNC_SRC_HRTIM_MASTER_CMP3,  /**< Synchronizes with HRTim Master Compare 3 vaue matching.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
	PWM_SYNC_SRC_HRTIM_MASTER_CMP4,  /**< Synchronizes with HRTim Master Compare 4 vaue matching.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
} pwm_sync_src_t;
/**
 * @brief Defines the input synchronization event for PWM timers.
 *
 */
typedef enum
{
	PWM_SYNC_RST = TIM_SLAVEMODE_RESET,                           /**< Reset the PWM channel at @ref pwm_sync_src_t */
	PWM_SYNC_START = TIM_SLAVEMODE_TRIGGER,                       /**< Start the PWM channel at @ref pwm_sync_src_t */
	PWM_SYNC_RESET_AND_START = TIM_SLAVEMODE_COMBINED_RESETTRIGGER/**< Starts and resets the PWM channel at @ref pwm_sync_src_t */
} pwm_sync_type_t;
/**
 * @brief Defines the type for PWM Period Specification
 */
typedef float pwm_period_t;
/**
 * @brief Defines the PWM reset callback,
 * The drivers will call this function whenever the timer for the PWM is reset if enabled in configuration
 */
typedef void (*PWMResetCallback)(void);
/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup PWM_Exported_Structures Structures
  * @{
  */
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
 * @brief Defines the dead time relating settings of an inverted PWM pair
 */
typedef struct
{
	bool on;						/**< @brief Controls dead time insertion in a pair */
	uint32_t nanoSec;				/**< @brief Value of dead time in nano-seconds if enabled */
} deadtime_t;
/**
 * @brief Defines the parameters for a specific PWM module.
 */
typedef struct
{
	pwm_alignment_t alignment;		/**< @brief Specifies the alignment of PWM */
	pwm_period_t periodInUsec;			/**< @brief Specifies the period of the PWM in micro-seconds (Maximum possible value is 250Us) */
	deadtime_t deadtime;			/**< @brief The dead time parameter for the paired inverted PWM.
										For individual PWMs this value should be NULL */
} pwm_module_config_t;
/**
 * @brief Defines the PWM synchronization information
 *
 */
typedef struct
{
	pwm_sync_src_t syncSrc;			/**<  @brief Specifies the synchronization source */
	pwm_sync_type_t syncType;		/**<  @brief Specifies the synchronization type */
} pwm_slave_opts_t;
/**
 * @brief Defines the parameters for specific PWM configuration
 */
typedef struct
{
	duty_limits_t lim;				/**< @brief Defines the duty cycle limits for the specified channels */
	duty_mode_t dutyMode;			/**< @brief Controls the duty cycle computation mode.
										This parameter is used only when the dead time is enabled */
	pwm_slave_opts_t* slaveOpts;	/**< @brief Defines the options for slave operation for the PWM.
										@note Keep in mind that some options may only be available for specific PWM channels only.
										For details view the relevant source definitions */
	pwm_module_config_t* module;	/**< @brief Defines the module configuration */
} pwm_config_t;
/**
 * @}
 */
/** @addtogroup PWM_Exported_Typedefs
  * @{
  */
/**
 * @brief Function prototype for the functions to update the duty cycle of specified PWMs
 * @param pwmNo Channel no of the first PWM in sequence
 * @param duty Duty cycle to be applied (Range 0-1)
 * @param *config Pointer to the PWM configuration
 * @return float Duty cycle applied in this cycle. May differ from the duty variable if outside permitted limits
 */
typedef float (*DutyCycleUpdateFnc)(uint32_t pwmNo, float duty, pwm_config_t *config);
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup PWM_Exported_Functions Functions
  * @{
  */

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Checks if the dead time is enabled for the PWM
 * @param *dt Pointer to the dead time structure
 * @return bool <c>true</c> if enabled for the specified PWM else <c>false</c>
 */
static inline bool IsDeadtimeEnabled(deadtime_t* dt)
{
	if(dt == NULL)
		return false;
	return dt->on;
}

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

