/**
 ********************************************************************************
 * @file    	max11046_drivers.c
 * @author 		Waqas Ehsan Butt
 * @date    	November 30, 2021
 *
 * @brief   Controls the max11046 functionality
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
#include "general_header.h"
#if MAX11046_ENABLE && IS_ADC_CORE
#include "user_config.h"
#include "adc_config.h"
#include "max11046_drivers.h"
#include "shared_memory.h"
#include "monitoring_library.h"
#include "pecontroller_timers.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#ifdef CORE_CM7
#define READ_ADC_CH(tempData)			maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;  \
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;  \
		*tempData++ = (uint16_t)MAX11046_GPIO->IDR; \
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
#else
#define READ_ADC_CH(tempData)			maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;  \
		*tempData++ = (uint16_t)MAX11046_GPIO->IDR; \
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
#endif
#if ENABLE_PROFILING
/**
 * @brief If you need to profile ADC conversion, enable this macro.
 */
#define PROFILE_CONVERSION				(1)
#endif
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
#if EN_DMA_ADC_DATA_COLLECTION
typedef struct
{
	uint32_t rdHigh;
	uint32_t rdLow;
	uint16_t data[32];
} adc_dma_data_t;
#endif
/********************************************************************************
 * Static Variables
 *******************************************************************************/
/** module status
 */
static volatile bool moduleActive = false;
/** Current applied ADC configurations
 */
static adc_cont_config_t adcContConfig = { .fs = 25000, .callback = NULL };
/** Current ADC acquisition mode
 */
static adc_acq_mode_t acqType = ADC_MODE_CONT;
/** Defines the offsets for each member of the ADC measurement.
 * These values are used to convert ADC data to meaningful measurements according to the formula <b>value = (adcData - adcOffsets) * adcMultipiers</b>
 */

static volatile adc_raw_data_t* rawData;
static volatile adc_processed_data_t* processedData;
static adc_info_t* adcInfo;
static TIM_HandleTypeDef htim5;
static TIM_HandleTypeDef htim4;
/**
 * @brief Handle for the ADC conversion timer
 */
static TIM_HandleTypeDef htimCnv;

#if EN_DMA_ADC_DATA_COLLECTION
static TIM_HandleTypeDef htimRead;			// TIM8
static DMA_HandleTypeDef hdma_rd_low;
static DMA_HandleTypeDef hdma_acq_data;
static DMA_HandleTypeDef hdma_rd_high;
static adc_dma_data_t* dmaData = (adc_dma_data_t*)0x3800F000;
static volatile bool isLastTimerCollect = false;
#endif
/********************************************************************************
 * Global Variables
 *******************************************************************************/
/** Contains the latest values of the acquired ADC readings
 */
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
#if !EN_DMA_ADC_DATA_COLLECTION
// change optimization level so that timing may never be effected
#pragma GCC push_options
#pragma GCC optimize ("-O0")
/**
 * @brief Measure data from all channels - optimized version
 * @param tempData
 */
static inline void CollectData_BothADCs(uint16_t* tempData)
{
	// ADC1
	maxCS1_GPIO_Port->BSRR = (uint32_t)maxCS1_Pin << 16U | maxCS2_Pin;

	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);

	// as CS1 and CS2 port is same in this case
	maxCS1_GPIO_Port->BSRR = (uint32_t)maxCS2_Pin << 16U | maxCS1_Pin;

	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);
	READ_ADC_CH(tempData);

	SET_Pin(maxCS2_GPIO_Port, maxCS2_Pin | maxCS1_Pin);
}
#pragma GCC pop_options
#endif

#pragma GCC push_options
#pragma GCC optimize ("-Ofast")
/**
 * @brief Acquire measurements and convert to meaningful data for both ADCs
 * @param *fData Pointer to where the data needs to be stored
 * @param *uData Pointer to where the raw adc data
 * @param *mults Pointer to the multiplier information
 * @param *offsets Pointer to the offset information
 */
TCritical static void CollectConvertData_BothADCs(float* fData, uint16_t* uData, const float* mults, const float* offsets)
{
	// Also collect data if not already collected via DMA
#if !EN_DMA_ADC_DATA_COLLECTION
	CollectData_BothADCs(uData);
#endif

	int i = 15;
	do
	{
		*fData++ =  (*uData++ - *offsets++) * (*mults++);
	} while (i--);
}
#pragma GCC pop_options

static void GPIOs_Init(void)
{
	/*Configure GPIO pin Output Level */
	SET_Pin(GPIOE, maxCS1_Pin|maxCS2_Pin);
	SET_Pin(GPIOI, maxWrite_Pin);
	SET_Pin(maxRead_GPIO_Port, maxRead_Pin);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	BSP_GPIOStruct_IOConfig(&GPIO_InitStruct, maxCS1_Pin|maxCS2_Pin, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	BSP_GPIOStruct_IOConfig(&GPIO_InitStruct, maxWrite_Pin, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	BSP_GPIOStruct_IOConfig(&GPIO_InitStruct, maxRead_Pin, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_GPIO_Init(maxRead_GPIO_Port, &GPIO_InitStruct);

	BSP_GPIOStruct_IOConfig(&GPIO_InitStruct, maxBusy1_Pin, GPIO_MODE_IT_FALLING, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_GPIO_Init(maxBusy1_GPIO_Port, &GPIO_InitStruct);

	BSP_GPIOStruct_IOConfig(&GPIO_InitStruct, maxBusy2_Pin, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_GPIO_Init(maxBusy2_GPIO_Port, &GPIO_InitStruct);

	BSP_GPIOStruct_IOConfig(&GPIO_InitStruct, GPIO_PIN_All, GPIO_MODE_INPUT, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

#if PROFILE_CONVERSION
	BSP_GPIOStruct_IOConfig(&GPIO_InitStruct, PROFILE_GPIO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_GPIO_Init(PROFILE_GPIO_PORT, &GPIO_InitStruct);
#endif
}

static void ConversionTimer_Init(void)
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_TIM12_CLK_ENABLE();

	htimCnv.Instance = TIM12;
	htimCnv.Init.Prescaler = 0;
	htimCnv.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimCnv.Init.Period = (uint32_t)((240 * 1000000) / adcContConfig.fs) - 1;
	htimCnv.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimCnv.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&htimCnv) != HAL_OK)
		Error_Handler();
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = MAX11046_CLK_Us * 2;				// 2Usec pulse
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htimCnv, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_ConfigChannel(&htimCnv, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
		Error_Handler();

	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	BSP_GPIOStruct_DefaultConfig(&GPIO_InitStruct, maxCNV1_Pin, GPIO_MODE_AF_PP, GPIO_AF2_TIM12);
	HAL_GPIO_Init(maxCNV1_GPIO_Port, &GPIO_InitStruct);

	BSP_GPIOStruct_DefaultConfig(&GPIO_InitStruct, maxCNV2_Pin, GPIO_MODE_AF_PP, GPIO_AF2_TIM12);
	HAL_GPIO_Init(maxCNV2_GPIO_Port, &GPIO_InitStruct);

	ADC_TIMER->CCR1 = ADC_TIMER->CCR2 = MAX11046_CLK_Us * 2;				// 2Usec pulse
	if(acqType == ADC_MODE_SINGLE)
		HAL_TIM_OnePulse_Init(&htimCnv, TIM_OPMODE_SINGLE);
}

#if EN_DMA_ADC_DATA_COLLECTION

/**
 * @brief TIM8 Initialization Function
 * @param None
 * @retval None
 */
static void ReadTimer_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
	__HAL_RCC_TIM8_CLK_ENABLE();

	htimRead.Instance = TIM8;
	htimRead.Init.Prescaler = 0;
	htimRead.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimRead.Init.Period = 39;
	htimRead.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimRead.Init.RepetitionCounter = 7;
	htimRead.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htimRead) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_OnePulse_Init(&htimRead, TIM_OPMODE_SINGLE) != HAL_OK)
		Error_Handler();
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htimRead, &sMasterConfig) != HAL_OK)
		Error_Handler();
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 1;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htimRead, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		Error_Handler();
	sConfigOC.Pulse = 25;
	if (HAL_TIM_PWM_ConfigChannel(&htimRead, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
		Error_Handler();
	sConfigOC.Pulse = 35;
	if (HAL_TIM_PWM_ConfigChannel(&htimRead, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
		Error_Handler();
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htimRead, &sBreakDeadTimeConfig) != HAL_OK)
		Error_Handler();

	// Custom configuration
	htimRead.Instance->DIER = 0xe00;
	htimRead.Instance->CCER |= (uint32_t)(1U << ((TIM_CHANNEL_1 | TIM_CHANNEL_2| TIM_CHANNEL_3) & 0x1FU));
	__HAL_TIM_MOE_ENABLE(&htimRead);
	__HAL_TIM_ENABLE_IT(&htimRead, TIM_IT_UPDATE);
	/* TIM8 interrupt Init */
	HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
}

static void DataCollectDMAs_Init(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_TIM8_CLK_ENABLE();

	dmaData->rdHigh = maxRead_Pin;
	dmaData->rdLow = maxRead_Pin << 16;
	/* TIM8 DMA Init */
	/* TIM8_CH1 Init */
	hdma_rd_low.Instance = DMA1_Stream0;
	hdma_rd_low.Init.Request = DMA_REQUEST_TIM8_CH1;
	hdma_rd_low.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_rd_low.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_rd_low.Init.MemInc = DMA_MINC_DISABLE;
	hdma_rd_low.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_rd_low.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_rd_low.Init.Mode = DMA_CIRCULAR;
	hdma_rd_low.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdma_rd_low.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_rd_low) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(&htimRead,hdma[TIM_DMA_ID_CC1],hdma_rd_low);

	/* TIM8_CH2 Init */
	hdma_acq_data.Instance = DMA1_Stream1;
	hdma_acq_data.Init.Request = DMA_REQUEST_TIM8_CH2;
	hdma_acq_data.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_acq_data.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_acq_data.Init.MemInc = DMA_MINC_ENABLE;
	hdma_acq_data.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_acq_data.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_acq_data.Init.Mode = DMA_CIRCULAR;
	hdma_acq_data.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdma_acq_data.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_acq_data) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(&htimRead,hdma[TIM_DMA_ID_CC2],hdma_acq_data);

	/* TIM8_CH3 Init */
	hdma_rd_high.Instance = DMA1_Stream2;
	hdma_rd_high.Init.Request = DMA_REQUEST_TIM8_CH3;
	hdma_rd_high.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_rd_high.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_rd_high.Init.MemInc = DMA_MINC_DISABLE;
	hdma_rd_high.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_rd_high.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_rd_high.Init.Mode = DMA_CIRCULAR;
	hdma_rd_high.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdma_rd_high.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_rd_high) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(&htimRead,hdma[TIM_DMA_ID_CC3],hdma_rd_high);

	// Enable the DMAs
	HAL_DMA_Start(&hdma_rd_low, (uint32_t)&dmaData->rdLow, (uint32_t)&maxRead_GPIO_Port->BSRR, 16);
	HAL_DMA_Start(&hdma_acq_data, (uint32_t)&MAX11046_GPIO->IDR, (uint32_t)&rawData->dataRecord[rawData->recordIndex << 4], 16);
	HAL_DMA_Start(&hdma_rd_high, (uint32_t)&dmaData->rdHigh, (uint32_t)&maxRead_GPIO_Port->BSRR, 16);
}
/**
 * @brief TIM_PWM MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param htim_pwm: TIM_PWM handle pointer
 * @retval None
 */
void DataCollectDMAs_DeInit(TIM_HandleTypeDef* htim_pwm)
{
	__HAL_RCC_TIM8_CLK_DISABLE();

	/* TIM8 DMA DeInit */
	HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC1]);
	HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC2]);
	HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC3]);

	/* TIM8 interrupt DeInit */
	HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);

}

#endif

static volatile bool isTim4OnePulse = false;
static volatile bool isTim5OnePulse = false;

static void Tim4_Init(void)
{
	__HAL_RCC_TIM4_CLK_ENABLE();

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 0;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 65535;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
		Error_Handler();
}

static void Tim5_Init(void)
{
	__HAL_RCC_TIM5_CLK_ENABLE();

	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 0;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 65535;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
		Error_Handler();
}

// --FIXME-- Period, Pulse period etc.
// --FIXME-- Start Stop
/**
 * @brief Sets the input and output trigger functions for the ADC.
 * @note Currently only the @ref TIM_TRGI_TYPE_RST, @ref TIM_TRGI_TYPE_START and @ref TIM_TRGI_TYPE_RESET_AND_START
 * functionalities are available for this timer synchronization. Delays can't be added in master or slave mode triggering through OC1.
 * --TODO-- Add list of possible timers for triggering in different modes.
 * @param _slaveConfig Configuration for the slave. Send NULL if no slave functionality needed.
 * @param _masterConfig Configuration for the master. Send NULL if no master functionality needed.
 * @param _fs Required sampling frequency of the ADC converter
 * @return timer_trigger_src_t Trigger source if configuration required as master else returns NULL.
 */
timer_trigger_src_t BSP_MAX11046_SetInputOutputTrigger(tim_in_trigger_config_t* _slaveConfig, tim_out_trigger_config_t* _masterConfig, float _fs)
{
	timer_trigger_src_t  result = TIM_TRG_SRC_NONE;
	float inTriggerDelay = 0;
	tim_in_trigger_config_t _slave;
	tim_out_trigger_config_t _master;
	isTim4OnePulse = false;
	isTim5OnePulse = false;
	if(_slaveConfig != NULL && _slaveConfig->type == TIM_TRGI_TYPE_NONE)
		_slaveConfig = NULL;

	// possible triggers TIM1,3,8
	if (_slaveConfig && _masterConfig)
	{
		// configure slave timers
		if (_slaveConfig->type == TIM_TRGI_TYPE_START)
		{
			isTim5OnePulse = true;
			if (_masterConfig->type == TIM_TRGO_OUT_EN)
				isTim4OnePulse = true;
		}
		_slave.src = _slaveConfig->src;
		_slave.type = _slaveConfig->type;
		BSP_Timer_SetInputTrigger(&htim5, &_slave);
		BSP_Timer_SetInputTrigger(&htim4, &_slave);

		// configure master timers
		if (inTriggerDelay > 0)
		{
			_master.isTriggerDelayInitRequired = true;
			_master.triggerDelayInUsec = inTriggerDelay;
			_master.type = TIM_TRGO_OUT_OC1;
		}
		else
		{
			_master.isTriggerDelayInitRequired = false;
			_master.triggerDelayInUsec = 0;
			_master.type = _slaveConfig->type == TIM_TRGI_TYPE_START ? TIM_TRGO_OUT_EN : TIM_TRGO_OUT_RST;
		}
		BSP_Timer_SetOutputTrigger(&htim5, &_master);

		_master.isTriggerDelayInitRequired = true;
		_master.triggerDelayInUsec = inTriggerDelay;
		if(_masterConfig->isTriggerDelayInitRequired)
			_master.triggerDelayInUsec += _masterConfig->triggerDelayInUsec;
		_master.type = TIM_TRGO_OUT_OC1;
		BSP_Timer_SetOutputTrigger(&htim4, &_master);

		// configure adc timer slave configuration
		_slave.src = TIM_TRG_SRC_TIM5;
		_slave.type = _slaveConfig->type;
		BSP_Timer_SetInputTrigger(&htimCnv, &_slave);
		result = TIM_TRG_SRC_TIM4;
	}
	// possible triggers TIM1,2,3,8
	else if (_slaveConfig)
	{
		// No need for TIM5
		BSP_Timer_SetInputTrigger(&htim5, NULL);
		BSP_Timer_SetOutputTrigger(&htim5, NULL);

		// TIM4 triggers ADC
		_slave.src = _slaveConfig->src;
		_slave.type = _slaveConfig->type;
		if (_slaveConfig->type == TIM_TRGI_TYPE_START)
			isTim4OnePulse = true;
		BSP_Timer_SetInputTrigger(&htim4, &_slave);

		// configure master timers
		if (inTriggerDelay > 0)
		{
			_master.isTriggerDelayInitRequired = true;
			_master.triggerDelayInUsec = inTriggerDelay;
			_master.type = TIM_TRGO_OUT_OC1;
		}
		else
		{
			_master.isTriggerDelayInitRequired = false;
			_master.triggerDelayInUsec = 0;
			_master.type = _slaveConfig->type == TIM_TRGI_TYPE_START ? TIM_TRGO_OUT_EN : TIM_TRGO_OUT_RST;
		}
		BSP_Timer_SetOutputTrigger(&htim4, &_master);

		// Select ADC triggering
		_slave.src = TIM_TRG_SRC_TIM4;
		_slave.type = _slaveConfig->type;
		BSP_Timer_SetInputTrigger(&htimCnv, &_slave);

	}
	else if (_masterConfig)
	{
		// No need for TIM5
		BSP_Timer_SetInputTrigger(&htim5, NULL);
		BSP_Timer_SetOutputTrigger(&htim5, NULL);
		BSP_Timer_SetInputTrigger(&htim4, NULL);
		if (_masterConfig->type == TIM_TRGO_OUT_EN)
			isTim4OnePulse = true;
		// Can't trigger with delay in only master mode
		if (_masterConfig->type == TIM_TRGO_OUT_OC1)
			Error_Handler();

		BSP_Timer_SetOutputTrigger(&htim4, _masterConfig);

		// Select ADC triggering
		_slave.src = TIM_TRG_SRC_TIM4;
		_slave.type = _masterConfig->type == TIM_TRGO_OUT_EN ? TIM_TRGI_TYPE_START : TIM_TRGI_TYPE_RST;
		BSP_Timer_SetInputTrigger(&htimCnv, &_slave);
		result = TIM_TRG_SRC_TIM4;
	}
	// No input/output triggers
	else
	{
		BSP_Timer_SetInputTrigger(&htim5, NULL);
		BSP_Timer_SetInputTrigger(&htim4, NULL);
		BSP_Timer_SetOutputTrigger(&htim5, NULL);
		BSP_Timer_SetOutputTrigger(&htim4, NULL);
		BSP_Timer_SetInputTrigger(&htimCnv, NULL);
	}
	BSP_Timer_SetOutputTrigger(&htimCnv, NULL);
	TIM4->ARR = TIM5->ARR = TIM12->ARR = (uint32_t)((240 * 1000000) / _fs) - 1;			// Dynamic Frequency Computation

	// update the sampling frequency
	processedData->info.fs = _fs;

	return result;
}
/**
 * @brief Initializes the MAX11046 drivers
 * @param type ADC_MODE_SINGLE or ADC_MODE_CONT for single or continuous conversions respectively
 * @param contConfig adc_cont_config_t contains the continuous transfer configuration
 * @param rawAdcData Pointer to the raw ADC data container
 * @param processedAdcData Pointer to the processed ADC data container
 */
void BSP_MAX11046_Init(adc_acq_mode_t type, adc_cont_config_t* contConfig, volatile adc_raw_data_t* rawAdcData, volatile adc_processed_data_t* processedAdcData)
{
	// DeInitialize if already initialized
	if(moduleActive)
		BSP_MAX11046_DeInit();

	// set parameters
	rawData = rawAdcData;
	processedData = processedAdcData;
	adcInfo = (adc_info_t*)&processedData->info;

	acqType = type;
	adcContConfig.fs = contConfig->fs;
	adcContConfig.callback = contConfig->callback;

	GPIOs_Init();
#if EN_DMA_ADC_DATA_COLLECTION
	DataCollectDMAs_Init();
	ReadTimer_Init();
#endif
	ConversionTimer_Init();
	Tim4_Init();
	Tim5_Init();
	BSP_MAX11046_SetInputOutputTrigger(NULL, NULL, adcContConfig.fs);

	moduleActive = true;
}

/**
 * @brief Starts the ADC conversion
 * @details For single conversion mode performs and provides the results in a blocking way.
 * For continuous conversion mode starts the conversions, after each conversion adc_cont_config_t.callback will be called
 * @return adc_measures_t_t* For single conversion mode returns the pointer to the acquired values. Returns NULL for continuous conversion mode
 */
adc_measures_t* BSP_MAX11046_Run(void)
{
	adc_measures_t* result = NULL;
	/* EXTI interrupt init*/
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy1_Pin);
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy2_Pin);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	if(acqType == ADC_MODE_SINGLE)
	{
		// wait for the pending operations to end
		while(HAL_GPIO_ReadPin(maxBusy1_GPIO_Port,maxBusy1_Pin));
		HAL_TIM_OnePulse_Start(&htimCnv,TIM_OPMODE_SINGLE);
		result = NULL; // --FIXME-- // Invalid Result (adc_measures_t*)processedData->dataRecord[processedData->];
	}
	else
	{
		// enable PWM to produce continuous data
		HAL_TIM_PWM_Start(&htimCnv,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htimCnv,TIM_CHANNEL_2);
		result = NULL;
	}
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);

	return result;
}

/**
 * @brief Stops the ADC data collection module, only effective for ADC_MODE_CONT
 */
void BSP_MAX11046_Stop(void)
{
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim5,TIM_CHANNEL_2);
	if(acqType != ADC_MODE_SINGLE)
	{
		// stop timer
		HAL_TIM_PWM_Stop(&htimCnv,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htimCnv,TIM_CHANNEL_2);
	}
	else
	{
		HAL_TIM_OnePulse_Stop(&htimCnv,TIM_CHANNEL_1);
		HAL_TIM_OnePulse_Stop(&htimCnv,TIM_CHANNEL_2);
	}
	// clear the flag
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy1_Pin);
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy2_Pin);
}

/**
 * @brief De-initialize the MAX11046 drivers
 */
void BSP_MAX11046_DeInit(void)
{
	// clear the flag, then enable negative edge interrupt, then assign callback to relevant port pin
	HAL_GPIO_DeInit(maxBusy1_GPIO_Port,maxBusy1_Pin);
	HAL_GPIO_DeInit(maxBusy2_GPIO_Port,maxBusy2_Pin);
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy1_Pin);
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy2_Pin);

	HAL_TIM_PWM_DeInit(&htimCnv);
	moduleActive = false;
}

#pragma GCC push_options
#pragma GCC optimize ("-Ofast")

TCritical static inline void ManipulateData(void)
{
#if USE_LOCAL_ADC_STORAGE
	float* fData = adcLocalConvStorage[adcLocalIndexRingBuff.wrIndex];
#else
	float* fData = (float*)&processedData->dataRecord[processedData->recordIndex];
#endif
#if !EN_DMA_ADC_DATA_COLLECTION && USE_LOCAL_ADC_STORAGE
	uint16_t* uData = adcLocalRawStorage[adcLocalIndexRingBuff.wrIndex];
#else
	uint16_t* uData = (uint16_t*)&rawData->dataRecord[rawData->recordIndex << 4];
#endif
	CollectConvertData_BothADCs(fData, uData, adcSensitivity, adcOffsets);
	if(adcContConfig.callback)
		adcContConfig.callback((adc_measures_t*)fData);
#if USE_LOCAL_ADC_STORAGE
	RingBuffer_Write(&adcLocalIndexRingBuff);
#else
	processedData->recordIndex = (processedData->recordIndex + 1) & (MEASURE_SAVE_COUNT - 1);
#endif
#if EN_DMA_ADC_DATA_COLLECTION || !USE_LOCAL_ADC_STORAGE
	rawData->recordIndex = (rawData->recordIndex + 1) & (RAW_MEASURE_SAVE_COUNT - 1);
#endif
}

/**
 * @brief Call this function when the acquisition is completed
 */
TCritical static inline void AcquistionComplete(void)
{
#if EN_DMA_ADC_DATA_COLLECTION
	isLastTimerCollect = false;
	((DMA_Stream_TypeDef *)hdma_acq_data.Instance)->CR =  0x32D00;
	((DMA_Stream_TypeDef *)hdma_acq_data.Instance)->M0AR = (uint32_t)&rawData->dataRecord[rawData->recordIndex << 4];
	((DMA_Stream_TypeDef *)hdma_acq_data.Instance)->CR =  0x32D01;
	maxCS1_GPIO_Port->BSRR = ((uint32_t)maxCS2_Pin << 0) | ((uint32_t)maxCS1_Pin << 16U);
	htimRead.Instance->CR1 = 0x9;
#else
	ManipulateData();
#endif
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
TCritical void EXTI15_10_IRQHandler(void)
{
#if PROFILE_CONVERSION
	SET_Pin(PROFILE_GPIO_PORT, PROFILE_GPIO_Pin);
#endif
#if defined(DUAL_CORE) && defined(CORE_CM4)
	if (__HAL_GPIO_EXTID2_GET_IT(GPIO_PIN_14) != 0x00U)
	{
		__HAL_GPIO_EXTID2_CLEAR_IT(GPIO_PIN_14);
		AcquistionComplete();
	}
#else
	/* EXTI line interrupt detected */
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != 0x00U)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
		AcquistionComplete();
	}
#endif
	__HAL_GPIO_EXTID2_CLEAR_IT(GPIO_PIN_15);
#if PROFILE_CONVERSION
	CLR_Pin(PROFILE_GPIO_PORT, PROFILE_GPIO_Pin);
#endif
	__ISB();
	__DSB();
}

#if EN_DMA_ADC_DATA_COLLECTION
TCritical void TIM8_UP_TIM13_IRQHandler(void)
{
#if PROFILE_CONVERSION
	SET_Pin(PROFILE_GPIO_PORT, PROFILE_GPIO_Pin);
#endif
	__HAL_TIM_CLEAR_IT(&htimRead, TIM_IT_UPDATE);
	if (isLastTimerCollect == false)
	{
		isLastTimerCollect = true;
		maxCS1_GPIO_Port->BSRR = ((uint32_t)maxCS1_Pin << 0) | ((uint32_t)maxCS2_Pin << 16U);
		htimRead.Instance->CR1 = 0x9;
	}
	else
	{
		maxCS1_GPIO_Port->BSRR = ((uint32_t)maxCS2_Pin << 0) | ((uint32_t)maxCS1_Pin << 0);
		ManipulateData();
	}
#if PROFILE_CONVERSION
	CLR_Pin(PROFILE_GPIO_PORT, PROFILE_GPIO_Pin);
#endif
	__ISB();
	__DSB();
}
#endif
#pragma GCC pop_options

#endif
/* EOF */
