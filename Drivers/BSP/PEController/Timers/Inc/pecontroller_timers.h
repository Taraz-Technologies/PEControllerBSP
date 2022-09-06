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
 * @details The relevant timers and their operation mechnics are described below:
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
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "pecontroller_pwm_base.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define HRTIM_FREQ					(480)
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
	pwm_sync_src_t syncSrc;/**<  @brief Defines the synchronous source. Only TIM1 can be selected as the source*/
	pwm_sync_type_t syncType;/**<  @brief Defines the synchronization event type */
	pwm_period_t periodInUsecs;/**<  @brief Defines the time period of the master HRTIM in micro-seconds */
} hrtim_opts_t;
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
 * @return Clock frequency in MHz.
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
 * @param periodInUsec Defines the time period in micro-seconds.
 * @param TimerIdx Index of the timer submodule.
 * @return Timer configuration.
 */
extern HRTIM_TimerCfgTypeDef BSP_HRTIM_GetDefaultTimerConfig(pwm_period_t periodInUsec, uint32_t TimerIdx);
/**
 * @brief Gets the default compare configuration for the HRTIM.
 * @details <b>Default Configuration</b>:
 * -# CompareValue = 3
 * -# AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR
 * @return The compare configuration.
 */
extern HRTIM_CompareCfgTypeDef BSP_HRTIM_GetDefaultCompareConfig(void);
/**
 * @brief Gets the default dead time configuration for the HRTIM.
 * @details <b>Default Configuration</b>:
 * -# Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_DIV16
 * -# RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE
 * -# FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE
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
 */
void BSP_HRTIM_Init(void);
/**
 * @brief Configures the Fiber/Sync Tx pin to generate a PWM of required frequency.
 * @details The output period is controlled by the periodInUsecs while the onTime is controlled via triggerDelayInUsecs.
 * The output trigger is generated after triggerDelayInUsecs in each PWM period.
 * @param periodInUsecs Time period of the wave in micro-seconds
 * @param triggerDelayInUsecs Output trigger delay from the start of period in micro-seconds
 */
extern void BSP_TIM3_ConfigFiberTx(float periodInUsecs, float triggerDelayInUsecs);
/**
 * @brief Configures the Fiber/Sync Rx pin to receive the synchronization event via TIM2 module.
 * @details The output trigger event for the TIM2 module will be obtained based on the settings of slaveType
 * and slaveEdge on Rx Pin of the Fiber/Sync connectors.
 * @param slaveType Configures the type of input trigger
 * @param slaveEdge Configures the edge for the input trigger
 */
extern void BSP_TIM2_ConfigFiberRx(pwm_sync_type_t slaveType, tim_slave_edge_t slaveEdge);
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
 * @brief Start the generation of PWM for the TIM3 modules (Fiber/Sync Tx)
 * @param startHrtimMaster Master HRTIM and TIM3 can also be enabled at the same time via software.
 * To enable this feature pass <c>true</c> in this parameter.
 */
extern void BSP_TIM3_FiberTxStart(bool startHrtimMaster);
/**
 * @brief Enable / Disable interrupt for Master HRTIM unit
 * @param enable If enable interrupt set this parameter to <c>true</c>
 * @param callback Specifies the function to be called when the PWM is reset
 * @param priority Interrupt priority. Range (0-15). Here 0 is the highest priority
 */
extern void BSP_HRTIM_Config_Interrupt(bool enable, PWMResetCallback callback, int priority);
/********************************************************************************
 * Code
 *******************************************************************************/

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
