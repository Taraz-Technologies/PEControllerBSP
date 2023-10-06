/**
 ********************************************************************************
 * @file 		pecontroller_timers.h
 * @author 		Waqas Ehsan Butt
 * @date 		Aug 30, 2022
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

#ifndef PECONTROLLER_TIMERS_H_
#define PECONTROLLER_TIMERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @defgroup Timers Timers
 * @brief Contains the declaration and procedures relating the supporting timer modules (HRTIM Master, TIM2, TIM3).
 * @details The following timers can be configured using this support package.
 * -# <b>HRTIM Master:</b> This timer can be used to control the timing of the PWM channels 1-10.
 * The timing units of this timer can be used to reset these PWMs at required times;
 * thus generating the possibility of creating phase shifted PWMs.
 * The operation mechanics of relevant timers are described below:
 * -# <b>TIM2 (Fiber Rx For Slave Operation):</b> This timer can be configured to trigger a reset on all
 * PWM channels on a rising or falling edge at the Fiber Rx connector.
 * -# <b>TIM3 (Fiber Tx For Master Operation):</b> This timer can be configured to trigger a reset on a slave PEController
 *  by connecting it to the Fiber Rx pin of the slave controller.
 * -# <b>Other timers:</b> These timers can act as master/slave timers to other timers/events. <br><br>
 *
 *
 * The following list describes the operating mechanics for the relevant timers.
 * -# <b>HRTIM Master:</b> @ref BSP_MasterHRTIM_Config() can be used to configure the
 * HRTIM Master timer for synchronization with timer 1 as well as HRTIMA-HRTIME.
 * @ref BSP_MasterHRTIM_SetShiftValue() and @ref BSP_MasterHRTIM_SetShiftPercent() can then be used to
 * change the instance of trigger occurance for each compare unit.
 * -# <b>TIM2:</b> @ref BSP_TIM2_ConfigFiberRx() can be used to initialize the TIM2 in slave mode using the
 * fiber Rx pin. It can be configured to produce a trigger output event at occurance of a rising or falling edge.
 * -# <b>TIM3:</b> @ref BSP_TIM3_ConfigFiberTx() can be used to initialize the TIM3 in master mode where it
 * generates a PWM at fiber Tx pin. The width of the PWM can also be selected via this function.
 * This can be used to synchronize with other PEControllers by connected to the Fiber-Rx (TIM2) pin and
 * configuring them as slaves. @ref BSP_TIM3_FiberTxStart() starts the TIM3 with optional synching of HRTIM.
 * -# <b>Other timers: </b> Use @ref BSP_Timer_SetInputTrigger() and @ref BSP_Timer_SetOutputTrigger() to set the trigger
 * characteristics of other timers.
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define HRTIM_FREQ_Hz					(480000000)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup Timers_Exported_Typedefs Type Definitions
  * @{
  */
/**
 * @brief Defines the available trigger edge types for external timer events.
 */
typedef enum
{
	TIM_SLAVE_RISING = TIM_TRIGGERPOLARITY_RISING, 	/**< Event at rising edge. */
	TIM_SLAVE_FALLING = TIM_TRIGGERPOLARITY_FALLING	/**< Event at falling edge. */
} tim_slave_edge_t;
/**
 * @brief Defines the available comparators used by the master timer of HRTIM.
 */
typedef enum
{
	HRTIM_COMP1 = 0,/**< Defines the 1st comparator for master HRTIM. */
	HRTIM_COMP2 = 2,/**< Defines the 2nd comparator for master HRTIM. */
	HRTIM_COMP3 = 3,/**< Defines the 3rd comparator for master HRTIM. */
	HRTIM_COMP4 = 4 /**< Defines the 4th comparator for master HRTIM. */
} hrtim_comp_t;
/**
 * @brief Defines the synchronization sources available for the timers
 * @note All sources are not available for all timers. See details for relevant information.
 *
 */
typedef enum
{
	TIM_TRG_SRC_NONE,               /**< No synchronization */
	TIM_TRG_SRC_TIM1,               /**< Synchronizes with TIM1 trigger.
									This event can be configured using @ref BSP_Timer_SetOutputTrigger().
									This event is available for all PWM channels.  */
	TIM_TRG_SRC_TIM2,               /**< Synchronizes with TIM2 trigger.
									This event can be configured using @ref BSP_Timer_SetOutputTrigger().
									This event can be configured using @ref BSP_TIM2_ConfigFiberRx().
									This event can start and reset PWM11-16, while it can only reset PWM1-10   */
	TIM_TRG_SRC_TIM3,               /**< Synchronizes with TIM3 trigger.
									This event can be configured using @ref BSP_Timer_SetOutputTrigger().
									This event can be configured using @ref BSP_TIM3_ConfigFiberTx().
									This event can start and reset PWM11-16, while it can only reset PWM1-10   */
	TIM_TRG_SRC_TIM4,				/**< Synchronizes with TIM4 trigger.
									This event can be configured using @ref BSP_Timer_SetOutputTrigger(). */
	TIM_TRG_SRC_TIM5,				/**< Synchronizes with TIM5 trigger.
									This event can be configured using @ref BSP_Timer_SetOutputTrigger(). */
	TIM_TRG_SRC_TIM8,				/**< Synchronizes with TIM8 trigger.
									This event can be configured using @ref BSP_Timer_SetOutputTrigger(). */
	TIM_TRG_SRC_TIM15,				/**< Synchronizes with TIM15 trigger.
									This event can be configured using @ref BSP_Timer_SetOutputTrigger(). */
	TIM_TRG_SRC_HRTIM_MASTER_PERIOD,/**< Synchronizes with HRTim Master Period Completion.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
	TIM_TRG_SRC_HRTIM_MASTER_CMP1,  /**< Synchronizes with HRTim Master Compare 1 vaue matching.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
	TIM_TRG_SRC_HRTIM_MASTER_CMP2,  /**< Synchronizes with HRTim Master Compare 2 vaue matching.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
	TIM_TRG_SRC_HRTIM_MASTER_CMP3,  /**< Synchronizes with HRTim Master Compare 3 vaue matching.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
	TIM_TRG_SRC_HRTIM_MASTER_CMP4,  /**< Synchronizes with HRTim Master Compare 4 vaue matching.
									This event can be configured using @ref BSP_MasterHRTIM_Config().
									This event can only reset PWM1-10. Not avaiable for PWM11-16  */
	TIM_TRG_SRC_PIN,
} timer_trigger_src_t;
/**
 * @brief Defines the input synchronization of a timer
 *
 */
typedef enum
{
	TIM_TRGI_TYPE_NONE = TIM_SLAVEMODE_DISABLE,						   /**< The timer is not slave to any other timer */
	TIM_TRGI_TYPE_RST = TIM_SLAVEMODE_RESET,                           /**< Reset the timer at @ref timer_trigger_src_t */
	TIM_TRGI_TYPE_START = TIM_SLAVEMODE_TRIGGER,                       /**< Start the timer at @ref timer_trigger_src_t */
	TIM_TRGI_TYPE_RESET_AND_START = TIM_SLAVEMODE_COMBINED_RESETTRIGGER/**< Resets and starts the timer at @ref timer_trigger_src_t */
} tim_trg_in_type_t;
/**
 * @brief Defines types of event generated by the timers.
 */
typedef enum
{
	TIM_TRGO_OUT_RST = TIM_TRGO_RESET,		/**< Generate trigger on timer reset event. */
	TIM_TRGO_OUT_EN = TIM_TRGO_ENABLE,		/**< Generate trigger on timer enable event. */
	TIM_TRGO_OUT_UPDATE = TIM_TRGO_UPDATE,	/**< Generate trigger on timer update event. */
	TIM_TRGO_OUT_OC1 = TIM_TRGO_OC1,		/**< Generate trigger on timer channel 1 match event. */
} tim_trg_out_type_t;
/**
 * @brief Defines the PWM reset callback,
 * The drivers will call this function whenever the timer for the PWM is reset if enabled in configuration
 */
typedef void (*PWMResetCallback)(void);
/**
 * @brief Defines the type for PWM frequency Specification
 */
typedef float timer_frequency_t;
/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup Timers_Exported_Structures Structures
  * @{
  */
/**
 * @brief Defines the configurations for the master HRTIM, used while configuring it for synzhronous actions.
 */
typedef struct
{
	timer_trigger_src_t syncSrc;/**<  @brief Defines the synchronous source. Only TIM1 can be selected as the source*/
	tim_trg_in_type_t syncType;/**<  @brief Defines the synchronization event type */
	timer_frequency_t f;	/**<  @brief Defines the time frequency of the master HRTIM in Hz */
} hrtim_opts_t;
/**
 * @brief Contains the properties of the timer input trigger event.
 */
typedef struct
{
	tim_trg_in_type_t type;		/**< @brief Defines the trigger event type */
	timer_trigger_src_t src;	/**< @brief Defines the trigger event source */
	tim_slave_edge_t edge;		/**< @brief Defines the trigger event edge type for edge based events */
} tim_in_trigger_config_t;
/**
 * @brief
 */
typedef struct
{
	tim_trg_out_type_t type;
	bool isTriggerDelayInitRequired;
	float triggerDelayInUsec;
} tim_out_trigger_config_t;
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @addtogroup Timers_Exported_Variables Variables
  * @{
  */
/** Pointer to the timer structure
 */
extern HRTIM_HandleTypeDef hhrtim;
/** Enabled HRTimer
 * <c>true</c> if module previously initialized
 */
extern bool hrtimEnabled;
/** keeps the callback functions of all PWM sub-modules
 */
extern PWMResetCallback hrtimCallbacks[6];
/**
 * @}
 */
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @addtogroup Timers_Exported_Functions Functions
  * @{
  */
/**
 * @brief Gets the clock frequency of the specific timer in MHz.
 * @param TimerIdx Index of the timer submodule.
 * @return Clock frequency in Hz.
 */
extern uint32_t BSP_HRTIM_GetTimerFreq(uint32_t TimerIdx);
/**
 * @brief Gets the default timer configuration for the HRTIM.
 * @details <b>Default Configuration</b>:
 * -# StartOnSync = false
 * -# ResetOnSync = false
 * -# RepetitionUpdate = false
 * -# PreloadEnable = true
 * -# ResetTrigger = false
 * -# ResetUpdate = true
 *
 * @param f Defines the freuency in Hz.
 * @param TimerIdx Index of the timer submodule.
 * @return Timer configuration.
 */
extern HRTIM_TimerCfgTypeDef BSP_HRTIM_GetDefaultTimerConfig(timer_frequency_t f, uint32_t TimerIdx);
/**
 * @brief Gets the default compare configuration for the HRTIM.
 * @details <b>Default Configuration</b>:
 * -# CompareValue = 3
 * -# AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR
 *
 * @return The compare configuration.
 */
extern HRTIM_CompareCfgTypeDef BSP_HRTIM_GetDefaultCompareConfig(void);
/**
 * @brief Gets the default dead time configuration for the HRTIM.
 * @details <b>Default Configuration</b>:
 * -# Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_DIV16
 * -# RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE
 * -# FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE
 *
 * @return The dead time configuration.
 */
extern HRTIM_DeadTimeCfgTypeDef BSP_HRTIM_GetDefaultDeadtimeConfig(void);
/**
 * @brief Gets the default output configuration for the HRTIM.
 * @details <b>Default Configuration</b>:
 * -# Polarity = HRTIM_OUTPUTPOLARITY_HIGH
 * -# IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE
 * -# SetSource = HRTIM_OUTPUTSET_TIMCMP1
 * -# ResetSource = HRTIM_OUTPUTRESET_TIMCMP2
 *
 * @return The output configuration.
 */
extern HRTIM_OutputCfgTypeDef BSP_HRTIM_GetDefaultOutputConfig(void);
/**
 * @brief Gets the IRQ for a timer submodule for HRTIM.
 * @param TimerIdx Index of the timer submodule.
 * @return The IRQn number for the specified submodule.
 */
extern IRQn_Type BSP_HRTIM_GetIRQn(uint32_t TimerIdx);
/**
 * @brief  Initialize the HRTIM.
 * @details Configures the HRTIM with CPU Clock. The following events are configured.
 * -# TIM1_TRG0
 * -# TIM2_TRG0
 * -# TIM3_TRG0
 *
 */
void BSP_HRTIM_Init(void);
/**
 * @brief Configures the compare value for the HRTIM compare unit.
 * This value can be used to reset the HRTIM submodules at required instances.
 * @param comp Type of the compare unit.
 * @param value Phase shift value to be fed in the compare unit.
 */
extern void BSP_MasterHRTIM_SetShiftValue(hrtim_comp_t comp, uint32_t value);
/**
 * @brief Configures the compare value for the HRTIM compare.
 * Instead of a value user provides a percentage between 0-1; thus specifying the shift in percentage
 * instead of value for easier computation on user end.
 * This value can be used to reset the HRTIM submodules at required instances.
 * @param opts Contains the configuration options for the Master HRTIM.
 * @param comp Type of the compare unit.
 * @param percent Phase shift in percentage.
 */
extern void BSP_MasterHRTIM_SetShiftPercent(hrtim_opts_t* opts, hrtim_comp_t comp, float percent);
/**
 * @brief Configures the Fiber/Sync Rx pin to receive the synchronization event via TIM2 module.
 * @param slaveType Control what to do when signal received on the RX pin
 * @param slaveEdge Select the rising or falling edge for the RX pin as trigger
 * @param outTrigger Output trigger configuration for the timer. Used to synchronize another timer to the RX signal
 * @param _fs Required frequency at Fiber/Sync RX Pin
 */
extern void BSP_TIM2_ConfigFiberRx(tim_trg_in_type_t slaveType, tim_slave_edge_t slaveEdge, tim_out_trigger_config_t* outTrigger, float _fs);
/**
 * @brief Configures the Fiber/Sync Tx pin to generate a PWM of required frequency.
 * @param inTrigger Input trigger configuration for the timer. Used to synchronize the TX signal to another trigger/Source
 * @param outTrigger Output trigger configuration for the timer. Used to synchronize another timer to the TX signal
 * @param _fs Required frequency at Fiber/Sync TX Pin
 */
extern void BSP_TIM3_ConfigFiberTx(tim_in_trigger_config_t* inTrigger, tim_out_trigger_config_t* outTrigger, float _fs);
/**
 * @brief Configures the master timer for HRTIM1 module.
 * @details The master timer is used as a support timer, which may be used for synchronizing different HRTIM submodules.
 * This timer can only operate as a master and can only be enabled or reset based upon the TIM1 module.
 * Use the @ref BSP_MasterHRTIM_SetShiftValue() and BSP_MasterHRTIM_SetShiftPercent()
 * functions to configure the trigger delays for other timer modules.
 * @param opts Contains the configuration options for the Master HRTIM.
 */
extern void BSP_MasterHRTIM_Config(hrtim_opts_t* opts);
/**
 * @brief Start the generation of PWM for the TIM3 modules (Fiber/Sync Tx)
 * @param startHrtimMaster Master HRTIM and TIM3 can also be enabled at the same time via software.
 * To enable this feature pass <c>true</c> in this parameter.
 */
extern void BSP_TIM3_FiberTxStart(bool startHrtimMaster);
/**
 * @brief Use this function to start the Master HRTIM
 */
extern void BSP_MasterHRTIM_Start(void);
/**
 * @brief Enable / Disable interrupt for Master HRTIM unit
 * @param enable If enable interrupt set this parameter to <c>true</c>
 * @param callback Specifies the function to be called when the PWM is reset
 * @param priority Interrupt priority. Range (0-15). Here 0 is the highest priority
 */
extern void BSP_HRTIM_Config_Interrupt(bool enable, PWMResetCallback callback, int priority);
/**
 * @brief Set the input trigger of a timer to make it a slave of any of the @ref timer_trigger_src_t.
 * @note Each timer can only be trigger by only a subset of the available options. Invalid selection will throw an error.
 * The following are the trigger options for all available timers.
 * -# <b>TIM1: </b> TIM15, TIM2, TIM3, TIM4
 * -# <b>TIM2: </b> TIM1, TIM8, TIM3, TIM4, External Pin
 * -# <b>TIM3: </b> TIM1, TIM2, TIM15, TIM4
 * -# <b>TIM4: </b> TIM1, TIM2, TIM3, TIM8
 * -# <b>TIM5: </b> TIM1, TIM8, TIM3, TIM4
 * -# <b>TIM8: </b> TIM1, TIM2, TIM4, TIM5
 * -# <b>TIM12: </b> TIM4, TIM5
 *
 * @param _htim Handle of the timer to be configured. (Make sure to initialize this parameter before this function).
 * @param _config Required configuration. If this value is set to NULL the timer will be configured as independent timer.
 */
extern void BSP_Timer_SetInputTrigger(TIM_HandleTypeDef* _htim, tim_in_trigger_config_t* _config);
/**
 * @brief Set the output trigger of a timer to make it a slave of any other timer.
 * @param _htim Handle of the timer to be configured. (Make sure to initialize this parameter before this function).
 * @param _config Required configuration for the output trigger.
 */
extern void BSP_Timer_SetOutputTrigger(TIM_HandleTypeDef* _htim, tim_out_trigger_config_t* _config);
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
#endif 
/* EOF */
