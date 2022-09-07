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
#define TIM2_FREQ_MHz					(240)
#define TIM3_FREQ_MHz					(240)
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
/** keeps the callback functions of all PWM sub-modules
 */
PWMResetCallback hrtimCallbacks[6] = {0};
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
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
uint32_t BSP_HRTIM_GetTimerFreq(uint32_t TimerIdx)
{
	static const uint32_t divs[8] = { 1, 1, 1, 1, 1, 1, 2, 4};
	uint32_t regVal = (TimerIdx == HRTIM_TIMERINDEX_MASTER ? hhrtim.Instance->sMasterRegs.MCR : hhrtim.Instance->sTimerxRegs[TimerIdx].TIMxCR) & 0x7;
	return HRTIM_FREQ / divs[regVal];
}
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
HRTIM_TimerCfgTypeDef BSP_HRTIM_GetDefaultTimerConfig(pwm_period_t periodInUsec, uint32_t TimerIdx)
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
/**
 * @brief Gets the default compare configuration for the HRTIM.
 * @details <b>Default Configuration</b>:
 * -# CompareValue = 3
 * -# AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR
 * @return The compare configuration.
 */
HRTIM_CompareCfgTypeDef BSP_HRTIM_GetDefaultCompareConfig(void)
{
	HRTIM_CompareCfgTypeDef pCompareCfg =
	{
			.CompareValue = 3,
			.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR,
			.AutoDelayedTimeout = 0x0000,
	};
	return pCompareCfg;
}
/**
 * @brief Gets the default dead time configuration for the HRTIM.
 * @details <b>Default Configuration</b>:
 * -# Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_DIV16
 * -# RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE
 * -# FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE
 * @return The dead time configuration.
 */
HRTIM_DeadTimeCfgTypeDef BSP_HRTIM_GetDefaultDeadtimeConfig(void)
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
/**
 * @brief Gets the default output configuration for the HRTIM.
 * @details <b>Default Configuration</b>:
 * -# Polarity = HRTIM_OUTPUTPOLARITY_HIGH
 * -# IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE
 * -# SetSource = HRTIM_OUTPUTSET_TIMCMP1
 * -# ResetSource = HRTIM_OUTPUTRESET_TIMCMP2
 * @return The output configuration.
 */
HRTIM_OutputCfgTypeDef BSP_HRTIM_GetDefaultOutputConfig(void)
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
/**
 * @brief Gets the IRQ for a timer submodule for HRTIM.
 * @param TimerIdx Index of the timer submodule.
 * @return The IRQn number for the specified submodule.
 */
IRQn_Type BSP_HRTIM_GetIRQn(uint32_t TimerIdx)
{
	return TimerIdx == HRTIM_TIMERINDEX_TIMER_A ? HRTIM1_TIMA_IRQn :
			(TimerIdx == HRTIM_TIMERINDEX_TIMER_B ? HRTIM1_TIMB_IRQn :
					(TimerIdx == HRTIM_TIMERINDEX_TIMER_C ? HRTIM1_TIMC_IRQn :
							(TimerIdx == HRTIM_TIMERINDEX_TIMER_D ? HRTIM1_TIMD_IRQn :
									TimerIdx == HRTIM_TIMERINDEX_TIMER_E ? HRTIM1_TIME_IRQn : HRTIM1_Master_IRQn)));
}
/**
 * @brief  Initialize the HRTIM.
 * @details Configures the HRTIM with CPU Clock. The following events are configured.
 * -# TIM1_TRG0
 * -# TIM2_TRG0
 * -# TIM3_TRG0
 */
void BSP_HRTIM_Init(void)
{
	if(hrtimEnabled)
		return;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_HRTIM1;
	PeriphClkInitStruct.Hrtim1ClockSelection = RCC_HRTIM1CLK_CPUCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		Error_Handler();
	__HAL_RCC_HRTIM1_CLK_ENABLE();
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

/**
 * @brief Configures the master timer for HRTIM1 module.
 * @details The master timer is used as a support timer, which may be used for synchronizing different HRTIM submodules.
 * This timer can only operate as a master and can only be enabled or reset based upon the TIM1 module.
 * Use the @ref BSP_MasterHRTIM_SetShiftValue() and BSP_MasterHRTIM_SetShiftPercent()
 * functions to configure the trigger delays for other timer modules.
 * @param opts Contains the configuration options for the Master HRTIM.
 */
void BSP_MasterHRTIM_Config(hrtim_opts_t* opts)
{
	BSP_HRTIM_Init();
	/* timer configuration */
	HRTIM_TimerCfgTypeDef pTimerCfg = BSP_HRTIM_GetDefaultTimerConfig(opts->periodInUsecs, HRTIM_TIMERINDEX_MASTER);
	pTimerCfg.RepetitionUpdate = HRTIM_MCR_MREPU;
	pTimerCfg.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	pTimerCfg.StartOnSync = (opts->syncSrc == PWM_SYNC_SRC_TIM1 &&
			(opts->syncType == PWM_SYNC_START || opts->syncType == PWM_SYNC_RESET_AND_START)) ? HRTIM_SYNCSTART_ENABLED : HRTIM_SYNCSTART_DISABLED;
	pTimerCfg.ResetOnSync = (opts->syncSrc == PWM_SYNC_SRC_TIM1 &&
			(opts->syncType == PWM_SYNC_RST || opts->syncType == PWM_SYNC_RESET_AND_START)) ? HRTIM_SYNCRESET_ENABLED : HRTIM_SYNCRESET_DISABLED;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_MASTER, &pTimerCfg) != HAL_OK)
		Error_Handler();

	/* compare configuration */
	hhrtim.Instance->sMasterRegs.MCMP1R = hhrtim.Instance->sMasterRegs.MCMP2R =
			hhrtim.Instance->sMasterRegs.MCMP3R = hhrtim.Instance->sMasterRegs.MCMP4R = 3;
}
/**
 * @brief Configures the compare value for the HRTIM compare unit.
 * This value can be used to reset the HRTIM submodules at required instances.
 * @param comp Type of the compare unit.
 * @param value Phase shift value to be fed in the compare unit.
 */
void BSP_MasterHRTIM_SetShiftValue(hrtim_comp_t comp, uint32_t value)
{
	if (value < 3)
		value = 3;
	*((uint32_t*)(&hhrtim.Instance->sMasterRegs.MCMP1R) + comp) = value;
}
/**
 * @brief Configures the compare value for the HRTIM compare.
 * Instead of a value user provides a percentage between 0-1; thus specifying the shift in percentage
 * instead of value for easier computation on user end.
 * This value can be used to reset the HRTIM submodules at required instances.
 * @param opts Contains the configuration options for the Master HRTIM.
 * @param comp Type of the compare unit.
 * @param percent Phase shift in percentage.
 */
void BSP_MasterHRTIM_SetShiftPercent(hrtim_opts_t* opts, hrtim_comp_t comp, float percent)
{
	uint32_t val = (opts->periodInUsecs * BSP_HRTIM_GetTimerFreq(HRTIM_TIMERINDEX_MASTER) - 1) * percent;
	if (val < 3)
		val = 3;
	*((uint32_t*)(&hhrtim.Instance->sMasterRegs.MCMP1R) + comp) = val;
}

/**
 * @brief Configures the Fiber/Sync Rx pin to receive the synchronization event via TIM2 module.
 * @details The output trigger event for the TIM2 module will be obtained based on the settings of slaveType
 * and slaveEdge on Rx Pin of the Fiber/Sync connectors.
 * @param slaveType Configures the type of input trigger
 * @param slaveEdge Configures the edge for the input trigger
 */
void BSP_TIM2_ConfigFiberRx(pwm_sync_type_t slaveType, tim_slave_edge_t slaveEdge)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_SlaveConfigTypeDef sSlaveConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 2;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65535; //(int)((periodInUsecs * TIM2_FREQ_MHz) - 1);
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	__HAL_RCC_TIM2_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**TIM2 GPIO Configuration
	    PB3 (JTDO/TRACESWO)     ------> TIM2_CH2
	 */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sSlaveConfig.SlaveMode = slaveType;
	sSlaveConfig.InputTrigger = TIM_TS_TI2FP2;
	sSlaveConfig.TriggerPolarity = slaveEdge;
	sSlaveConfig.TriggerFilter = 0;
	if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief Configures the Fiber/Sync Tx pin to generate a PWM of required frequency.
 * @details The output period is controlled by the periodInUsecs while the onTime is controlled via triggerDelayInUsecs.
 * The output trigger is generated after triggerDelayInUsecs in each PWM period.
 * @param periodInUsecs Time period of the wave in micro-seconds
 * @param triggerDelayInUsecs Output trigger delay from the start of period in micro-seconds
 */
void BSP_TIM3_ConfigFiberTx(float periodInUsecs, float triggerDelayInUsecs)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = (int)((periodInUsecs * TIM3_FREQ_MHz) - 1);
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	__HAL_RCC_TIM3_CLK_ENABLE();
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = (int)((triggerDelayInUsecs * TIM3_FREQ_MHz) - 1);
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**TIM3 GPIO Configuration
	    PB4 (NJTRST)     ------> TIM3_CH1
	 */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
/**
 * @brief Start the generation of PWM for the TIM3 modules (Fiber/Sync Tx)
 * @param startHrtimMaster Master HRTIM and TIM3 can also be enabled at the same time via software.
 * To enable this feature pass <c>true</c> in this parameter.
 */
void BSP_TIM3_FiberTxStart(bool startHrtimMaster)
{
	/* Enable the Capture compare channel */
	TIM_CCxChannelCmd(htim3.Instance, 0, TIM_CCx_ENABLE);
	__HAL_TIM_MOE_ENABLE(&htim3);
	if (startHrtimMaster)
		hhrtim.Instance->sMasterRegs.MCR |= (HRTIM_TIMERID_MASTER);
	__HAL_TIM_ENABLE(&htim3);
}
/**
 * @brief Use this function to start the Master HRTIM
 */
void BSP_MasterHRTIM_Start(void)
{
	hhrtim.Instance->sMasterRegs.MCR |= (HRTIM_TIMERID_MASTER);
}
/**
 * @brief Callback function invoked when the timer x counter reset/roll-over
 *         event occurs.
 * @param *hhrtim Pointer to HAL HRTIM handle
 * @param TimerIdx Timer index
 *                   This parameter can be one of the following values:
 *                   @arg HRTIM_TIMERINDEX_TIMER_A for timer A
 *                   @arg HRTIM_TIMERINDEX_TIMER_B for timer B
 *                   @arg HRTIM_TIMERINDEX_TIMER_C for timer C
 *                   @arg HRTIM_TIMERINDEX_TIMER_D for timer D
 *                   @arg HRTIM_TIMERINDEX_TIMER_E for timer E
 */
void HAL_HRTIM_CounterResetCallback(HRTIM_HandleTypeDef * hhrtim,
		uint32_t TimerIdx)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hhrtim);

	if(hrtimCallbacks[TimerIdx])
		hrtimCallbacks[TimerIdx]();
}

/**
 * @brief Callback function invoked when the timer x roll-over event occurs
 * @param *hhrtim Pointer to HAL HRTIM handle
 * @param TimerIdx Timer index
 */
void HAL_HRTIM_RepetitionEventCallback(HRTIM_HandleTypeDef * hhrtim,
		uint32_t TimerIdx)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hhrtim);

	if(hrtimCallbacks[TimerIdx])
		hrtimCallbacks[TimerIdx]();
}

/**
 * @brief Enable / Disable interrupt for Master HRTIM unit
 * @param enable If enable interrupt set this parameter to <c>true</c>
 * @param callback Specifies the function to be called when the PWM is reset
 * @param priority Interrupt priority. Range (0-15). Here 0 is the highest priority
 */
void BSP_HRTIM_Config_Interrupt(bool enable, PWMResetCallback callback, int priority)
{
	__HAL_HRTIM_ENABLE_IT(&hhrtim, hhrtim.Init.HRTIMInterruptResquests);
	if (enable)
	{
		hrtimCallbacks[HRTIM_TIMERINDEX_MASTER] = callback;
		hhrtim.TimerParam[HRTIM_TIMERINDEX_MASTER].InterruptRequests = HRTIM_TIM_IT_REP;
		__HAL_HRTIM_MASTER_ENABLE_IT(&hhrtim, hhrtim.TimerParam[HRTIM_TIMERINDEX_MASTER].InterruptRequests);
		IRQn_Type irq = BSP_HRTIM_GetIRQn(HRTIM_TIMERINDEX_MASTER);
		HAL_NVIC_SetPriority(irq, priority, 0);
		HAL_NVIC_EnableIRQ(irq);
	}
	else
	{
		hhrtim.TimerParam[HRTIM_TIMERINDEX_MASTER].InterruptRequests = HRTIM_TIM_IT_NONE;
		__HAL_HRTIM_MASTER_DISABLE_IT(&hhrtim, hhrtim.TimerParam[HRTIM_TIMERINDEX_MASTER].InterruptRequests);
		HAL_NVIC_DisableIRQ(BSP_HRTIM_GetIRQn(HRTIM_TIMERINDEX_MASTER));
		hrtimCallbacks[HRTIM_TIMERINDEX_MASTER] = NULL;
	}
}

/* EOF */
