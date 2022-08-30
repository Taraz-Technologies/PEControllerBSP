/**
 ********************************************************************************
 * @file    	pecontroller_timers.c
 * @author 		Waqas Ehsan Butt
 * @date    	Aug 30, 2022
 *
 * @brief   
 ********************************************************************************
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

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "pecontroller_timers.h"
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
 * Static Variables
 *******************************************************************************/

/********************************************************************************
 * Global Variables
 *******************************************************************************/
/** Pointer to the timer structure
 */
HRTIM_HandleTypeDef hhrtim;
/** Enabled HRTimer
 * <c>true</c> if module previously initialized
 */
bool hrtimEnabled = false;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Gets the clock frequency of the specific timer in MHz.
 * @param TimerIdx Index of the timer submodule.
 * @return Clock frequency in MHz.
 */
uint32_t BSP_HRTim_GetTimerFreq(uint32_t TimerIdx)
{
	static const uint32_t divs[8] = { 1, 1, 1, 1, 1, 1, 2, 4};
	uint32_t regVal = (TimerIdx == HRTIM_TIMERINDEX_MASTER ? hhrtim.Instance->sMasterRegs.MCR : hhrtim.Instance->sTimerxRegs[TimerIdx].TIMxCR) & 0x7;
	return HRTIM_FREQ / divs[regVal];
}
HRTIM_TimerCfgTypeDef BSP_HRTim_GetDefaultTimerConfig(pwm_period_t periodInUsec, uint32_t TimerIdx)
{
	/* timer base configuration */
	HRTIM_TimeBaseCfgTypeDef pTimeBaseCfg =
	{
			.RepetitionCounter = 0x00,
			.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1,
			.Mode = HRTIM_MODE_CONTINUOUS,
	};
	uint32_t ticks = periodInUsec * HRTIM_FREQ;
	if(ticks > 65535)
	{
		ticks /= 2;
		pTimeBaseCfg.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV2;
		if(ticks > 65535)
		{
			ticks /= 2;
			pTimeBaseCfg.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV4;
		}
	}
	pTimeBaseCfg.Period = (uint32_t)(periodInUsec * HRTIM_FREQ);
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim, TimerIdx, &pTimeBaseCfg) != HAL_OK)
		Error_Handler();

	/* configuration for the timer */
	HRTIM_TimerCfgTypeDef pTimerCfg =
	{
			.DMARequests = HRTIM_TIM_DMA_NONE,
			.DMASrcAddress = 0x0000,
			.DMADstAddress = 0x0000,
			.DMASize = 0x1,
			.HalfModeEnable = HRTIM_HALFMODE_DISABLED,
			.StartOnSync = HRTIM_SYNCSTART_DISABLED,
			.ResetOnSync = HRTIM_SYNCRESET_DISABLED,
			.DACSynchro = HRTIM_DACSYNC_NONE,
			.PreloadEnable = HRTIM_PRELOAD_ENABLED,
			.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT,
			.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK,
			.RepetitionUpdate = HRTIM_UPDATEONREPETITION_DISABLED,
			.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED,
			.FaultEnable = HRTIM_TIMFAULTENABLE_NONE,
			.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE,
			.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED,	// HRTIM_TIMER_D_E_DELAYEDPROTECTION_DISABLED, both have same value no need to replace
			.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_NONE,
			.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE,
			.ResetUpdate = HRTIM_TIMUPDATEONRESET_ENABLED,
	};
	return pTimerCfg;
}
HRTIM_CompareCfgTypeDef BSP_HRTim_GetDefaultCompareConfig(void)
{
	HRTIM_CompareCfgTypeDef pCompareCfg =
	{
			.CompareValue = 3,
			.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR,
			.AutoDelayedTimeout = 0x0000,
	};
	return pCompareCfg;
}
HRTIM_DeadTimeCfgTypeDef BSP_HRTim_GetDefaultDeadtimeConfig(void)
{
	HRTIM_DeadTimeCfgTypeDef pDeadTimeCfg =
	{
			.Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_DIV16,
			.RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE,
			.RisingLock = HRTIM_TIMDEADTIME_RISINGLOCK_READONLY,
			.RisingSignLock = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE,
			.FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE,
			.FallingLock = HRTIM_TIMDEADTIME_FALLINGLOCK_READONLY,
			.FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE,
	};
	return pDeadTimeCfg;
}
HRTIM_OutputCfgTypeDef BSP_HRTim_GetDefaultOutputConfig(void)
{
	HRTIM_OutputCfgTypeDef pOutputCfg =
	{
			.Polarity = HRTIM_OUTPUTPOLARITY_HIGH,
			.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE,
			.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE,
			.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE,
			.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED,
			.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR,
			.SetSource = HRTIM_OUTPUTSET_TIMCMP1,
			.ResetSource = HRTIM_OUTPUTRESET_TIMCMP2,
	};
	return pOutputCfg;
}
IRQn_Type BSP_HRTim_GetIRQn(uint32_t TimerIdx)
{
	return TimerIdx == HRTIM_TIMERINDEX_TIMER_A ? HRTIM1_TIMA_IRQn :
			(TimerIdx == HRTIM_TIMERINDEX_TIMER_B ? HRTIM1_TIMB_IRQn :
					(TimerIdx == HRTIM_TIMERINDEX_TIMER_C ? HRTIM1_TIMC_IRQn :
							(TimerIdx == HRTIM_TIMERINDEX_TIMER_D ? HRTIM1_TIMD_IRQn :
									TimerIdx == HRTIM_TIMERINDEX_TIMER_E ? HRTIM1_TIME_IRQn : HRTIM1_Master_IRQn)));
}
/**
 * @brief  Initialize the HRTimer
 */
void BSP_HRTim_Init(void)
{
	if(hrtimEnabled)
		return;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_HRTIM1;
	PeriphClkInitStruct.Hrtim1ClockSelection = RCC_HRTIM1CLK_CPUCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		Error_Handler();
	__HAL_RCC_HRTIM1_CLK_ENABLE();
	/* enable the HRTimer (high Resolution Timer) responsible for PWM1-PWM10 */
	hhrtim.Instance = HRTIM1;
	hhrtim.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
	hhrtim.Init.SyncInputSource = HRTIM_SYNCINPUTSOURCE_INTERNALEVENT;
	if (HAL_HRTIM_Init(&hhrtim) != HAL_OK)
		Error_Handler();
	/// ----------- event
	HRTIM_EventCfgTypeDef pEventCfg = {0};
	pEventCfg.Source = HRTIM_EVENTSRC_3;			// Timer 1
	pEventCfg.Sensitivity = HRTIM_EVENTSENSITIVITY_RISINGEDGE;
	HAL_HRTIM_EventConfig(&hhrtim, HRTIM_EVENT_1, &pEventCfg);
	pEventCfg.Source = HRTIM_EVENTSRC_3;			// Timer 2
	pEventCfg.Sensitivity = HRTIM_EVENTSENSITIVITY_RISINGEDGE;
	HAL_HRTIM_EventConfig(&hhrtim, HRTIM_EVENT_2, &pEventCfg);
	pEventCfg.Source = HRTIM_EVENTSRC_3;			// Timer 3
	pEventCfg.Sensitivity = HRTIM_EVENTSENSITIVITY_RISINGEDGE;
	HAL_HRTIM_EventConfig(&hhrtim, HRTIM_EVENT_3, &pEventCfg);
	hrtimEnabled = true;
}

/* EOF */
