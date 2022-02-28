/*! 
@file PWM1_10_Drivers.c
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "PWMDriver.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/

 /*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
HRTIM_HandleTypeDef hhrtim;
static bool moduleEnabled = false;
static HRTIM_TimeBaseCfgTypeDef pTimeBaseCfg =
{
	.RepetitionCounter = 0x00,
	.PrescalerRatio = HRTIM_PRESCALERRATIO_DIV1,
	.Mode = HRTIM_MODE_CONTINUOUS,
};
static HRTIM_TimerCfgTypeDef pTimerCfg = 
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
static HRTIM_DeadTimeCfgTypeDef pDeadTimeCfg =
{
	.Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_DIV16,
  .RisingValue = 0x20,
  .RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE,
  .RisingLock = HRTIM_TIMDEADTIME_RISINGLOCK_READONLY,
  .RisingSignLock = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE,
  .FallingValue = 0x20,
  .FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE,
  .FallingLock = HRTIM_TIMDEADTIME_FALLINGLOCK_READONLY,
  .FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE,
};
static HRTIM_CompareCfgTypeDef pCompareCfg =
{
	.CompareValue = 3,
	.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR,
  .AutoDelayedTimeout = 0x0000,
};
static HRTIM_OutputCfgTypeDef pOutputCfg = 
{
	.Polarity = HRTIM_OUTPUTPOLARITY_HIGH,
  .IdleMode = HRTIM_OUTPUTIDLEMODE_NONE,
  .IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE,
  .FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE,
  .ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED,
  .BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR,
};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief initialize the pwm modules */
void PWM1_10_Drivers_Init(void)
{
	if(moduleEnabled)
		return;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_HRTIM1;
  PeriphClkInitStruct.Hrtim1ClockSelection = RCC_HRTIM1CLK_CPUCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
      Error_Handler();
  __HAL_RCC_HRTIM1_CLK_ENABLE();
  HAL_NVIC_SetPriority(HRTIM1_TIMA_IRQn, 0, 0);				// --todo-- enable on interrupt selection
  HAL_NVIC_EnableIRQ(HRTIM1_TIMA_IRQn);
	/* enable the HRTimer (high Resolution Timer) responsible for PWM1-PWM10 */
  hhrtim.Instance = HRTIM1;
	hhrtim.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
  hhrtim.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;
  if (HAL_HRTIM_Init(&hhrtim) != HAL_OK)
    Error_Handler();
	moduleEnabled = true;
}
/*! @brief update the duty cycle from 0-1 */
void PWM1_10_UpdatePair(uint32_t pwmNo, float duty, pwm_pair_config_t* config)
{
	uint32_t TimerIdx = (pwmNo - 1) / 2;
	uint32_t period = hhrtim.Instance->sTimerxRegs[TimerIdx].PERxR;
	uint32_t onTime = duty * period;
	if(config->alignment == CENTER_ALIGNED)
	{
		int t0 = (period - onTime) / 2;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP1xR = t0;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP2xR = t0 + onTime;
		MODIFY_REG(hhrtim.Instance->sTimerxRegs[TimerIdx].TIMxCR, HRTIM_TIMCR_DELCMP2, 0U);
	}
	else
	{
		uint32_t dt = 0;
		if(config->dtEnabled)
			dt = (config->dtInNanoSec * 16) / HRTIM_FREQ;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP1xR = dt;
		hhrtim.Instance->sTimerxRegs[TimerIdx].CMP2xR = dt + onTime;
		MODIFY_REG(hhrtim.Instance->sTimerxRegs[TimerIdx].TIMxCR, HRTIM_TIMCR_DELCMP2, 0U);
	}
}
/*! @brief Confiure the inverted pair */
UpdatePWMPair PWM1_10_ConfigPair(uint32_t pwmNo, pwm_pair_config_t* config)
{
	uint32_t TimerIdx = (pwmNo - 1) / 2;
	uint32_t out1 = 1U << (TimerIdx * 2);
	uint32_t out2 = 1U << (TimerIdx * 2 + 1);
	
  pTimeBaseCfg.Period = config->periodInUsec * HRTIM_FREQ;
//	if(config->alignment == CENTER_ALIGNED)
//		pTimeBaseCfg.Period += 1;
  if (HAL_HRTIM_TimeBaseConfig(&hhrtim, TimerIdx, &pTimeBaseCfg) != HAL_OK)
    Error_Handler();
  pTimerCfg.InterruptRequests = config->interruptEnabled ? HRTIM_TIM_IT_RST : HRTIM_TIM_IT_NONE;
  pTimerCfg.DeadTimeInsertion = config->dtEnabled ? HRTIM_TIMDEADTIMEINSERTION_ENABLED : HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, TimerIdx, &pTimerCfg) != HAL_OK)
    Error_Handler();
  pCompareCfg.CompareValue = 3;
  if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_1, &pCompareCfg) != HAL_OK)
    Error_Handler();
  if (HAL_HRTIM_WaveformCompareConfig(&hhrtim, TimerIdx, HRTIM_COMPAREUNIT_2, &pCompareCfg) != HAL_OK)
    Error_Handler();
	if (config->dtEnabled)
  {
		pDeadTimeCfg.Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_DIV16;
		pDeadTimeCfg.FallingValue = pDeadTimeCfg.RisingValue = (config->dtInNanoSec * 16) / HRTIM_FREQ;
		if (HAL_HRTIM_DeadTimeConfig(&hhrtim, TimerIdx, &pDeadTimeCfg) != HAL_OK)
			Error_Handler();
	}
  pOutputCfg.SetSource = HRTIM_OUTPUTRESET_TIMCMP1;
  pOutputCfg.ResetSource = HRTIM_OUTPUTRESET_TIMCMP2;
  if (HAL_HRTIM_WaveformOutputConfig(&hhrtim, TimerIdx, out1, &pOutputCfg) != HAL_OK)
    Error_Handler();
  pOutputCfg.SetSource = HRTIM_OUTPUTSET_NONE;
  pOutputCfg.ResetSource = HRTIM_OUTPUTRESET_NONE;
  if (HAL_HRTIM_WaveformOutputConfig(&hhrtim, TimerIdx, out2, &pOutputCfg) != HAL_OK)
    Error_Handler();
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim, TimerIdx, &pTimerCfg) != HAL_OK)
    Error_Handler();
	return PWM1_10_UpdatePair;
}


/* EOF */