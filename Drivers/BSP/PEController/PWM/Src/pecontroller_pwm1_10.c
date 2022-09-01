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
/** keeps the callback functions of all PWM sub-modules
 */
static PWMResetCallback callbacks[5] = {0};
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
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
			int dt = (mod->deadtime.nanoSec * BSP_HRTim_GetTimerFreq(TimerIdx)) / 1000;
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
			dt += (mod->deadtime.nanoSec * BSP_HRTim_GetTimerFreq(TimerIdx)) / 1000;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP1xR = 3;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP2xR = onTime + dt;
	}
	return duty;
}

/**
 * @brief Activates a specific PWM Inverted pair
 * @param pwmNo Channel no of reference channel is the PWM pair (Valid Values 1-10). <br>
 * 				<b>Pairs are classified as :</b>
 * 				-# CH1 = Reference channel available at pin pwmNo
 * 				-# CH2 = Inverted Channel from reference available at pin pwmNo + 1 if pwmNo is odd else pwmNo - 1
 * @param en <c>true</c> if needs activation, else false
 */
void PWM1_10_ActivateInvertedPair(uint32_t pwmNo, bool en)
{
	if (en)
	{
		HAL_HRTIM_WaveformOutputStart(&hhrtim, 3U << (pwmNo - 1));
	}
	else
	{
		HAL_HRTIM_WaveformOutputStop(&hhrtim, 3U << (pwmNo - 1));
	}
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
	HRTIM_TimerCfgTypeDef pTimerCfg = BSP_HRTim_GetDefaultTimerConfig(mod->periodInUsec, TimerIdx);
	pTimerCfg.DeadTimeInsertion = IsDeadtimeEnabled(&mod->deadtime) ? HRTIM_TIMDEADTIMEINSERTION_ENABLED : HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	pTimerCfg.StartOnSync = mod->synchOnStart ? HRTIM_SYNCSTART_ENABLED : HRTIM_SYNCSTART_DISABLED;
	pTimerCfg.ResetTrigger = TimerIdx == HRTIM_TIMERINDEX_TIMER_A ? HRTIM_TIMRESETTRIGGER_MASTER_PER :
			(TimerIdx == HRTIM_TIMERINDEX_TIMER_B ? HRTIM_TIMRESETTRIGGER_MASTER_CMP2 :
					(TimerIdx == HRTIM_TIMERINDEX_TIMER_C ? HRTIM_TIMRESETTRIGGER_MASTER_CMP3 : HRTIM_TIMRESETTRIGGER_MASTER_CMP4));
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, TimerIdx, &pTimerCfg) != HAL_OK)
		Error_Handler();

	/* compare configuration */
	HRTIM_CompareCfgTypeDef pCompareCfg = BSP_HRTim_GetDefaultCompareConfig();
	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_1, &pCompareCfg) != HAL_OK)
		Error_Handler();
	if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_2, &pCompareCfg) != HAL_OK)
		Error_Handler();

	/* dead time configuration */
	float deadTicks = 3;	/* dead ticks because compare value can't be lower than 3 */
	if (IsDeadtimeEnabled(&mod->deadtime))
	{
		HRTIM_DeadTimeCfgTypeDef pDeadTimeCfg = BSP_HRTim_GetDefaultDeadtimeConfig();
		if(isCh2)
		{
			pDeadTimeCfg.RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_NEGATIVE;
			pDeadTimeCfg.FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_NEGATIVE;
		}
		pDeadTimeCfg.FallingValue = pDeadTimeCfg.RisingValue = (mod->deadtime.nanoSec * BSP_HRTim_GetTimerFreq(TimerIdx)) / (16000);
		if (HAL_HRTIM_DeadTimeConfig(&hhrtim, TimerIdx, &pDeadTimeCfg) != HAL_OK)
			Error_Handler();

		if (config->dutyMode == OUTPUT_DUTY_AT_PWMH)
			deadTicks += (pDeadTimeCfg.RisingValue * 16);
	}

	/* output configuration */
	HRTIM_OutputCfgTypeDef pOutputCfg = BSP_HRTim_GetDefaultOutputConfig();
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
	config->lim.max = 1 - ((deadTicks / BSP_HRTim_GetTimerFreq(TimerIdx)) / mod->periodInUsec);
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
	if(!hrtimEnabled)
		BSP_HRTim_Init();
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
	HRTIM_TimerCfgTypeDef pTimerCfg = BSP_HRTim_GetDefaultTimerConfig(mod->periodInUsec, TimerIdx);
	pTimerCfg.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	pTimerCfg.StartOnSync = mod->synchOnStart ? HRTIM_SYNCSTART_ENABLED : HRTIM_SYNCSTART_DISABLED;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, TimerIdx, &pTimerCfg) != HAL_OK)
		Error_Handler();

	/* compare configuration */
	HRTIM_CompareCfgTypeDef pCompareCfg = BSP_HRTim_GetDefaultCompareConfig();
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
	HRTIM_OutputCfgTypeDef pOutputCfg = BSP_HRTim_GetDefaultOutputConfig();
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
	config->lim.max = 1 - ((deadTicks / BSP_HRTim_GetTimerFreq(TimerIdx)) / mod->periodInUsec);
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
	if(!hrtimEnabled)
		BSP_HRTim_Init();
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
		IRQn_Type irq = BSP_HRTim_GetIRQn(TimerIdx);
		HAL_NVIC_SetPriority(irq, priority, 0);
		HAL_NVIC_EnableIRQ(irq);
	}
	else
	{
		hhrtim.TimerParam[TimerIdx].InterruptRequests = HRTIM_TIM_IT_NONE;
		__HAL_HRTIM_TIMER_DISABLE_IT(&hhrtim, TimerIdx, hhrtim.TimerParam[TimerIdx].InterruptRequests);
		HAL_NVIC_DisableIRQ(BSP_HRTim_GetIRQn(TimerIdx));
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
