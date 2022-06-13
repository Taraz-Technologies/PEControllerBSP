/**
 ********************************************************************************
 * @file    	pecontroller_pwm1_10.c
 * @author 		Waqas Ehsan Butt
 * @date    	December 2, 2021
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

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "pecontroller_pwm1_10.h"
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
/** <c>true</c> if module previously initialized
 */
static bool pwm1_10_enabled = false;
/** keeps the callback functions of all PWM sub-modules
 */
static PWMResetCallback callbacks[5] = {0};
/********************************************************************************
 * Global Variables
 *******************************************************************************/
/** Pointer to the timer structure
 */
HRTIM_HandleTypeDef hhrtim;
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
static inline uint32_t GetTimerFreqMHz(uint32_t TimerIdx)
{
	static const uint32_t divs[8] = { 1, 1, 1, 1, 1, 1, 2, 4};
	return HRTIM_FREQ / divs[(hhrtim.Instance->sTimerxRegs[TimerIdx].TIMxCR & 0x7)];
}

static HRTIM_TimerCfgTypeDef GetDefaultTimerConfig(uint32_t periodInUsec, uint32_t TimerIdx)
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
	pTimeBaseCfg.Period = ticks;
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
static HRTIM_CompareCfgTypeDef GetDefaultCompareConfig(void)
{
	HRTIM_CompareCfgTypeDef pCompareCfg =
	{
			.CompareValue = 3,
			.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR,
			.AutoDelayedTimeout = 0x0000,
	};
	return pCompareCfg;
}
static HRTIM_DeadTimeCfgTypeDef GetDefaultDeadtimeConfig(void)
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
static HRTIM_OutputCfgTypeDef GetDefaultOutputConfig(void)
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
static IRQn_Type GetIRQn(uint32_t TimerIdx)
{
	return TimerIdx == HRTIM_TIMERINDEX_TIMER_A ? HRTIM1_TIMA_IRQn :
			(TimerIdx == HRTIM_TIMERINDEX_TIMER_B ? HRTIM1_TIMB_IRQn :
					(TimerIdx == HRTIM_TIMERINDEX_TIMER_C ? HRTIM1_TIMC_IRQn :
							(TimerIdx == HRTIM_TIMERINDEX_TIMER_D ? HRTIM1_TIMD_IRQn : HRTIM1_TIME_IRQn)));
}

/**
 * @brief  Initialize the relevant PWM modules (High Precision Timers)
 */
static void PWM1_10_Drivers_Init(void)
{
	if(pwm1_10_enabled)
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
	hhrtim.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;
	if (HAL_HRTIM_Init(&hhrtim) != HAL_OK)
		Error_Handler();
	pwm1_10_enabled = true;
}

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
float BSP_PWM1_10_UpdatePairDuty(uint32_t pwmNo, float duty, pwm_config_t* config)
{
	// get copies of parameters
	uint32_t TimerIdx = (pwmNo - 1) / 2;
	uint32_t period = hhrtim.Instance->sTimerxRegs[TimerIdx].PERxR;
	pwm_module_config_t* mod = config->module;

	/* check for duty cycle limits */
	if (duty > config->lim.max)
		duty = config->lim.max;
	else if (duty < config->lim.min)
		duty = config->lim.min;

	uint32_t onTime = duty * period;

	if(onTime == 0)
	{
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP1xR = period + 2;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP2xR = period + 2;
	}
	else if(mod->alignment == CENTER_ALIGNED)
	{
		if(config->dutyMode == OUTPUT_DUTY_AT_PWMH && IsDeadtimeEnabled(&mod->deadtime))
		{
			int dt = (mod->deadtime.nanoSec * GetTimerFreqMHz(TimerIdx)) / 1000;
			onTime += dt;
		}
		int t0 = (period - onTime) / 2; 		// half time
		int tEnd = t0 + onTime;					// last edge at this time
		if (t0 < 3)
			t0 = 3;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP1xR = t0;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP2xR = tEnd;
	}
	else
	{
		uint32_t dt = 3;
		if(config->dutyMode == OUTPUT_DUTY_AT_PWMH && IsDeadtimeEnabled(&mod->deadtime))
			dt += (mod->deadtime.nanoSec * GetTimerFreqMHz(TimerIdx)) / 1000;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP1xR = 3;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP2xR = onTime + dt;
	}
	return duty;
}

/**
 * @brief Configures a single inverted pair for PWM
 * @param pwmNo Channel no of reference channel is the PWM pair (Valid Values 1-10). <br>
 * 				<b>Pairs are classified as :</b>
 * 				-# CH1 = Reference channel available at pin pwmNo
 * 				-# CH2 = Inverted Channel from reference available at pin pwmNo + 1 if pwmNo is odd else pwmNo - 1
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
static void PWM1_10_ConfigInvertedPair(uint32_t pwmNo, pwm_config_t* config)
{
	// get timer module references
	uint32_t TimerIdx = (pwmNo - 1) / 2;
	uint32_t out1 = 1U << (TimerIdx * 2);
	uint32_t out2 = 1U << (TimerIdx * 2 + 1);
	uint32_t isCh2 = (pwmNo - 1) % 2;
	pwm_module_config_t* mod = config->module;

	/* timer configuration */
	HRTIM_TimerCfgTypeDef pTimerCfg = GetDefaultTimerConfig(mod->periodInUsec, TimerIdx);
	pTimerCfg.DeadTimeInsertion = IsDeadtimeEnabled(&mod->deadtime) ? HRTIM_TIMDEADTIMEINSERTION_ENABLED : HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, TimerIdx, &pTimerCfg) != HAL_OK)
		Error_Handler();

	/* compare configuration */
	HRTIM_CompareCfgTypeDef pCompareCfg = GetDefaultCompareConfig();
	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_1, &pCompareCfg) != HAL_OK)
		Error_Handler();
	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_2, &pCompareCfg) != HAL_OK)
		Error_Handler();

	/* dead time configuration */
	float deadTicks = 3;	/* dead ticks because compare value can't be lower than 3 */
	if (IsDeadtimeEnabled(&mod->deadtime))
	{
		HRTIM_DeadTimeCfgTypeDef pDeadTimeCfg = GetDefaultDeadtimeConfig();
		if(isCh2)
		{
			pDeadTimeCfg.RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_NEGATIVE;
			pDeadTimeCfg.FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_NEGATIVE;
		}
		pDeadTimeCfg.FallingValue = pDeadTimeCfg.RisingValue = (mod->deadtime.nanoSec * GetTimerFreqMHz(TimerIdx)) / (16000);
		if (HAL_HRTIM_DeadTimeConfig(&hhrtim, TimerIdx, &pDeadTimeCfg) != HAL_OK)
			Error_Handler();

		if (config->dutyMode == OUTPUT_DUTY_AT_PWMH)
			deadTicks += (pDeadTimeCfg.RisingValue * 16);
	}

	/* output configuration */
	HRTIM_OutputCfgTypeDef pOutputCfg = GetDefaultOutputConfig();
	if(isCh2)
		pOutputCfg.Polarity = HRTIM_OUTPUTPOLARITY_LOW;
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim, TimerIdx, out1, &pOutputCfg) != HAL_OK)
		Error_Handler();
	if(IsDeadtimeEnabled(&mod->deadtime))
	{
		pOutputCfg.SetSource = HRTIM_OUTPUTSET_NONE;
		pOutputCfg.ResetSource = HRTIM_OUTPUTRESET_NONE;
	}
	else
		pOutputCfg.Polarity = HRTIM_OUTPUTPOLARITY_LOW;
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim, TimerIdx, out2, &pOutputCfg) != HAL_OK)
		Error_Handler();
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, TimerIdx, &pTimerCfg) != HAL_OK)
		Error_Handler();

	/* parameter adjustment */
	if (mod->alignment == CENTER_ALIGNED)
		deadTicks = deadTicks * 2 + 1;
	else
		deadTicks++;

	float oldMax = config->lim.max;
	config->lim.max = 1 - ((deadTicks / GetTimerFreqMHz(TimerIdx)) / mod->periodInUsec);
	if (oldMax < config->lim.max && oldMax != 0)
		config->lim.max = oldMax;
	if (config->lim.minMaxDutyCycleBalancing && config->lim.max > .5f)
		config->lim.min = 1 - config->lim.max;
}

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
DutyCycleUpdateFnc BSP_PWM1_10_ConfigInvertedPairs(uint32_t pwmNo, pwm_config_t* config, int pairCount)
{
	if(!pwm1_10_enabled)
		PWM1_10_Drivers_Init();
	while (pairCount--)
	{
		PWM1_10_ConfigInvertedPair(pwmNo, config);
		pwmNo += 2;
	}
	return BSP_PWM1_10_UpdatePairDuty;
}

/**
 * @brief Update the Duty Cycle of a channel
 * @param pwmNo PWM channel to be configured (Valid Values 1-10)
 * @param duty duty cycle to be applied to the channel (Range 0-1 or given in the config parameter)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channel
 * @return float Duty cycle applied in this cycle. May differ from the duty variable if outside permitted limits
 */
float BSP_PWM1_10_UpdateChannelDuty(uint32_t pwmNo, float duty, pwm_config_t* config)
{
	// get copies of parameters
	uint32_t TimerIdx = (pwmNo - 1) / 2;
	uint32_t period = hhrtim.Instance->sTimerxRegs[TimerIdx].PERxR;
	pwm_module_config_t* mod = config->module;

	/* check for duty cycle limits */
	if (duty > config->lim.max)
		duty = config->lim.max;

	uint32_t onTime = duty * period;
	if(mod->alignment == CENTER_ALIGNED)
	{
		int t0 = (period - onTime) / 2; 		// half time
		if (t0 < 3)
			t0 = 3;
		if(pwmNo % 2)
		{
			hhrtim.Instance->sTimerxRegs[TimerIdx].CMP1xR = t0;
			hhrtim.Instance->sTimerxRegs[TimerIdx].CMP2xR = t0 + onTime;
			MODIFY_REG(hhrtim.Instance->sTimerxRegs[TimerIdx].TIMxCR, HRTIM_TIMCR_DELCMP2, 0U);
		}
		else
		{
			hhrtim.Instance->sTimerxRegs[TimerIdx].CMP3xR = t0;
			hhrtim.Instance->sTimerxRegs[TimerIdx].CMP4xR = t0 + onTime;
			MODIFY_REG(hhrtim.Instance->sTimerxRegs[TimerIdx].TIMxCR, HRTIM_TIMCR_DELCMP4, 0U);
		}
	}
	else if(pwmNo % 2)
	{
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP1xR = 3;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP2xR = onTime + 3;
		MODIFY_REG(hhrtim.Instance->sTimerxRegs[TimerIdx].TIMxCR, HRTIM_TIMCR_DELCMP2, 0U);
	}
	else
	{
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP3xR = 3;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP4xR = onTime + 3;
		MODIFY_REG(hhrtim.Instance->sTimerxRegs[TimerIdx].TIMxCR, HRTIM_TIMCR_DELCMP4, 0U);
	}
	return duty;
}
/**
 * @brief Configures a single PWM channel
 * @param pwmNo Channel no of the PWM Channel in the pair (Valid Values 1-10)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channels
 */
static void PWM1_10_ConfigChannel(uint32_t pwmNo, pwm_config_t* config)
{
	// get timer module references
	uint32_t TimerIdx = (pwmNo - 1) / 2;
	bool isPWM1 = pwmNo % 2 == 0 ? false : true;
	uint32_t out = 1U << (TimerIdx * 2 + (isPWM1 ? 0 : 1));
	pwm_module_config_t* mod = config->module;

	/* timer configuration */
	HRTIM_TimerCfgTypeDef pTimerCfg = GetDefaultTimerConfig(mod->periodInUsec, TimerIdx);
	pTimerCfg.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, TimerIdx, &pTimerCfg) != HAL_OK)
		Error_Handler();

	/* compare configuration */
	HRTIM_CompareCfgTypeDef pCompareCfg = GetDefaultCompareConfig();
	if (isPWM1)
	{
		if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_1, &pCompareCfg) != HAL_OK)
			Error_Handler();
		if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_2, &pCompareCfg) != HAL_OK)
			Error_Handler();
	}
	else
	{
		if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_3, &pCompareCfg) != HAL_OK)
			Error_Handler();
		if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_4, &pCompareCfg) != HAL_OK)
			Error_Handler();
	}

	/* dead time configuration */
	float deadTicks = 3;

	/* output configuration */
	HRTIM_OutputCfgTypeDef pOutputCfg = GetDefaultOutputConfig();
	if (isPWM1 == false)
	{
		pOutputCfg.SetSource = HRTIM_OUTPUTRESET_TIMCMP3;
		pOutputCfg.ResetSource = HRTIM_OUTPUTRESET_TIMCMP4;
	}
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim, TimerIdx, out, &pOutputCfg) != HAL_OK)
		Error_Handler();
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, TimerIdx, &pTimerCfg) != HAL_OK)
		Error_Handler();

	/* parameter adjustment */
	if (mod->alignment == CENTER_ALIGNED)
		deadTicks = deadTicks * 2 + 1;
	else
		deadTicks++;

	float oldMax = config->lim.max;
	config->lim.max = 1 - ((deadTicks / GetTimerFreqMHz(TimerIdx)) / mod->periodInUsec);
	if (oldMax < config->lim.max && oldMax != 0)
		config->lim.max = oldMax;
}

/**
 * @brief Configures consecutive PWM channels
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1-10)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 * @param chCount No of channels to be configured with the setting
 * @return DutyCycleUpdateFnc Returns the function pointer of the type DutyCycleUpdateFnc which needs to be called
 * 						  whenever the duty cycles of the pair need to be updated
 */
DutyCycleUpdateFnc BSP_PWM1_10_ConfigChannels(uint32_t pwmNo, pwm_config_t* config, int chCount)
{
	if(!pwm1_10_enabled)
		PWM1_10_Drivers_Init();
	while (chCount--)
		PWM1_10_ConfigChannel(pwmNo++, config);
	return BSP_PWM1_10_UpdateChannelDuty;
}

/**
 * @brief Enable / Disable interrupt for a PWM channel as per requirement
 * @param pwmNo Channel no of the PWM Channel (Range 1-10)
 * @param enable If enable interrupt set this parameter to <c>true</c>
 * @param callback Specifies the function to be called when the PWM is reset
 * @param priority Interrupt priority. Range (0-15). Here 0 is the highest priority
 */
void BSP_PWM1_10_Config_Interrupt(uint32_t pwmNo, bool enable, PWMResetCallback callback, int priority)
{
	uint32_t TimerIdx = (pwmNo - 1) / 2;
	__HAL_HRTIM_ENABLE_IT(&hhrtim, hhrtim.Init.HRTIMInterruptResquests);
	if (enable)
	{
		callbacks[TimerIdx] = callback;
		hhrtim.TimerParam[TimerIdx].InterruptRequests = HRTIM_TIM_IT_RST;
		__HAL_HRTIM_TIMER_ENABLE_IT(&hhrtim, TimerIdx, hhrtim.TimerParam[TimerIdx].InterruptRequests);
		IRQn_Type irq = GetIRQn(TimerIdx);
		HAL_NVIC_SetPriority(irq, priority, 0);
		HAL_NVIC_EnableIRQ(irq);
	}
	else
	{
		hhrtim.TimerParam[TimerIdx].InterruptRequests = HRTIM_TIM_IT_NONE;
		__HAL_HRTIM_TIMER_DISABLE_IT(&hhrtim, TimerIdx, hhrtim.TimerParam[TimerIdx].InterruptRequests);
		HAL_NVIC_DisableIRQ(GetIRQn(TimerIdx));
		callbacks[TimerIdx] = NULL;
	}
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

	if(callbacks[TimerIdx])
		callbacks[TimerIdx]();
}

/* EOF */
