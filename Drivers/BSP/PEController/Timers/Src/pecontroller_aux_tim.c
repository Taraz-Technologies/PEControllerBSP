/**
 ********************************************************************************
 * @file    	pecontroller_aux_tim.c
 * @author 		Waqas Ehsan Butt
 * @date    	Aug 26, 2022
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
#include "pecontroller_aux_tim.h"
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
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
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
 * @brief Configures a single PWM channel
 * @param pwmNo Channel no of the PWM Channel in the pair (Valid Values 1-10)
 * @param *config Pointer to a  pwm_config_t structure that contains the configuration
 * 				   parameters for the PWM channels
 */
void BSP_AuxTim_ConfigHRTIM(hrtim_opts_t* opts)
{
	BSP_HRTim_Init();
	/* timer configuration */
	HRTIM_TimerCfgTypeDef pTimerCfg = BSP_HRTim_GetDefaultTimerConfig(opts->periodInUsecs, HRTIM_TIMERINDEX_MASTER);
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

void BSP_AuxTim_SetValueShift(hrtim_comp_t comp, uint32_t value)
{
	if (value < 3)
		value = 3;
	*((uint32_t*)(&hhrtim.Instance->sMasterRegs.MCMP1R) + comp) = value;
}

void BSP_AuxTim_SetDutyShift(hrtim_opts_t* opts, hrtim_comp_t comp, float duty)
{
	uint32_t val = (opts->periodInUsecs * HRTIM_FREQ - 1) * duty;
	if (val < 3)
		val = 3;
	*((uint32_t*)(&hhrtim.Instance->sMasterRegs.MCMP1R) + comp) = val;
}

void BSP_AuxTim_ConfigTim2(float periodInUsecs, pwm_sync_type_t slaveType, tim_slave_edge_t slaveEdge)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_SlaveConfigTypeDef sSlaveConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = (int)((periodInUsecs * TIM2_FREQ_MHz) - 1);
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

void BSP_AuxTim_ConfigTim3(float periodInUsecs, float triggerDelayInUsecs)
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

void BSP_AuxTim_StartTim3(bool startHrtimMaster)
{
	/* Enable the Capture compare channel */
	TIM_CCxChannelCmd(htim3.Instance, 0, TIM_CCx_ENABLE);
	__HAL_TIM_MOE_ENABLE(&htim3);
	if (startHrtimMaster)
		hhrtim.Instance->sMasterRegs.MCR |= (HRTIM_TIMERID_MASTER);
	__HAL_TIM_ENABLE(&htim3);

}

/* EOF */
