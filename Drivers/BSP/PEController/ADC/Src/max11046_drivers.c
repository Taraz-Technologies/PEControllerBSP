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
#include "user_config.h"
#include "max11046_drivers.h"
#include "shared_memory.h"
#if ENABLE_INTELLISENS
#include "intelliSENS.h"
#endif
/********************************************************************************
 * Defines
 *******************************************************************************/
#define READ_ADC_CH(tempData)			maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U; \
		*tempData++ = (uint16_t)MAX11046_GPIO->IDR; \
		maxRead_GPIO_Port->BSRR = maxRead_Pin;

#define MANUAL_RD_SWITCH			(0)
#define VIEW_OSCILLOSCOPE			(1)
#define USE_DMA						(!MANUAL_RD_SWITCH || VIEW_OSCILLOSCOPE)
#define COMPUTE_STATS				(1)
#define USE_CS_DMA					(0)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
/** module status
 */
static volatile bool moduleActive = false;
/** Current applied ADC configurations
 */
static adc_cont_config_t adcContConfig = { .conversionCycleTimeUs = 40, .callback = NULL };
/** Current ADC acquisition mode
 */
static adc_acq_mode_t acqType = ADC_MODE_CONT;
///////////////////////////////////////////
//static uint64_t intelliSENSDataPtr[4];
/** Defines the multipliers for each member of the ADC measurement
 * These values are used to convert ADC data to meaningful measurements according to the formula <b>value = (adcData - adcOffsets) * adcMultipiers</b>
 */
static adc_measures_t adcMultipiers = {0};
/** Defines the offsets for each member of the ADC measurement.
 * These values are used to convert ADC data to meaningful measurements according to the formula <b>value = (adcData - adcOffsets) * adcMultipiers</b>
 */
static adc_measures_t adcOffsets = {0};
static stats_t stats = {0};
static adc_info_t adcInfo = {0};
static adc_raw_data_t* rawData;
static adc_processed_data_t* processedData;
static TIM_HandleTypeDef htimCnv;			// TIM12
#if USE_DMA
static TIM_HandleTypeDef htimRead;			// TIM8
static DMA_HandleTypeDef hdma_rd_low;
static DMA_HandleTypeDef hdma_acq_data;
static DMA_HandleTypeDef hdma_rd_high;
#if !MANUAL_RD_SWITCH
uint32_t rdHigh = maxRead_Pin;
uint32_t rdLow = maxRead_Pin << 16;
#else
uint32_t rdHigh = maxRead_Pin;
uint32_t rdLow = maxRead_Pin << 16;
uint32_t toggle[] = {(1U << 11), (1U << 27)};
#endif
#if USE_CS_DMA
extern DMA_HandleTypeDef hdma_tim8_up;
extern DMA_HandleTypeDef hdma_dma_generator3;
uint32_t csList[] = {((uint32_t)maxCS1_Pin) | ((uint32_t)maxCS2_Pin << 16U), ((uint32_t)maxCS1_Pin) | ((uint32_t)maxCS2_Pin)};
uint16_t timList[] = {TIM_CR1_CEN | TIM_CR1_OPM_Pos, TIM_CR1_OPM_Pos};
#endif
#endif
/********************************************************************************
 * Global Variables
 *******************************************************************************/
/** Contains the latest values of the acquired ADC readings
 */
//adc_measures_t adcVals;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
#if MANUAL_RD_SWITCH
#pragma GCC push_options
#pragma GCC optimize ("-O0")   // optimization may effect acquisition
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

	maxCS2_GPIO_Port->BSRR = maxCS2_Pin | maxCS1_Pin;
}
#pragma GCC pop_options
#endif

#pragma GCC push_options
#pragma GCC optimize ("-Ofast")
/**
 * @brief Acquire measurements and convert to meaningful data for both ADCs
 * @param *dataPtr Pointer to where the data needs to be stored
 * @param *mults Pointer to the multiplier information
 * @param *offsets Pointer to the offset information
 */
static inline void CollectConvertData_BothADCs(float* dataPtr, const float* mults, const float* offsets)
{
	uint16_t* tempData = (uint16_t*)intelliSENSDataPtr;
	float* dataPtrOriginal = dataPtr;
#if MANUAL_RD_SWITCH
	CollectData_BothADCs(tempData);
#endif

#if ENABLE_INTELLISENS
	intelliSENS.SetADCData((uint64_t*)(intelliSENSDataPtr));
#endif

	int i = 15;
	do
	{
		*dataPtr++ =  (*tempData++ - *offsets++) * (*mults++);
	} while (i--);


#if COMPUTE_STATS
	Stats_Insert_Compute(dataPtrOriginal, &stats, 16);
#endif
}
#pragma GCC pop_options

#if 1 // Peripheral Initialization

static void GPIOs_Init(void)
{
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, maxCS1_Pin|maxCS2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOI, maxWrite_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(maxRead_GPIO_Port, maxRead_Pin, GPIO_PIN_SET);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*Configure GPIO pins : maxCS1_Pin maxCS2_Pin */
	GPIO_InitStruct.Pin = maxCS1_Pin|maxCS2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : maxWrite_Pin LCD_Disp_Pin */
	GPIO_InitStruct.Pin = maxWrite_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	/*Configure GPIO pin : maxRead_Pin */
	GPIO_InitStruct.Pin = maxRead_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(maxRead_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : maxBusy1_Pin */
	GPIO_InitStruct.Pin = maxBusy1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(maxBusy1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : maxBusy2_Pin */
	GPIO_InitStruct.Pin = maxBusy2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(maxBusy2_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : maxD0_Pin maxD1_Pin maxD2_Pin maxD3_Pin
                           maxD4_Pin maxD5_Pin maxD6_Pin maxD7_Pin
                           maxD8_Pin maxD9_Pin maxD10_Pin maxD11_Pin
                           maxD12_Pin maxD13_Pin maxD14_Pin maxD15_Pin */
	GPIO_InitStruct.Pin = maxD0_Pin|maxD1_Pin|maxD2_Pin|maxD3_Pin
			|maxD4_Pin|maxD5_Pin|maxD6_Pin|maxD7_Pin
			|maxD8_Pin|maxD9_Pin|maxD10_Pin|maxD11_Pin
			|maxD12_Pin|maxD13_Pin|maxD14_Pin|maxD15_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

static void ConversionTimer_Init(void)
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_TIM12_CLK_ENABLE();

	htimCnv.Instance = TIM12;
	htimCnv.Init.Prescaler = 0;
	htimCnv.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimCnv.Init.Period = (uint16_t)(MAX11046_CLK_Us * adcContConfig.conversionCycleTimeUs) - 1;
#if ENABLE_INTELLISENS
	intelliSENS.Init(adcContConfig.conversionCycleTimeUs, (const float*)&adcMultipiers, (const float*)&adcOffsets);
	intelliSENS.SetADCTicks(adcContConfig.conversionCycleTimeUs * 240);
#endif
	htimCnv.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimCnv.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&htimCnv) != HAL_OK)
		Error_Handler();
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = MAX11046_CLK_Us * 2;				// 2Usec pulse
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htimCnv, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_ConfigChannel(&htimCnv, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
		Error_Handler();

	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**TIM12 GPIO Configuration
	    PH6     ------> TIM12_CH1
	    PB15     ------> TIM12_CH2
	 */
	GPIO_InitStruct.Pin = maxCNV1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM12;
	HAL_GPIO_Init(maxCNV1_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = maxCNV2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM12;
	HAL_GPIO_Init(maxCNV2_GPIO_Port, &GPIO_InitStruct);

	MAX11046_TIMER->CCR1 = MAX11046_TIMER->CCR2 = MAX11046_CLK_Us * 2;				// 2Usec pulse
	if(acqType == ADC_MODE_SINGLE)
		HAL_TIM_OnePulse_Init(&htimCnv, TIM_OPMODE_SINGLE);
}

#if USE_DMA

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
	htimRead.Init.Period = 44;
	htimRead.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimRead.Init.RepetitionCounter = 7;
	htimRead.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htimRead) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_OnePulse_Init(&htimRead, TIM_OPMODE_SINGLE) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htimRead, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 9;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htimRead, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.Pulse = 15;
	if (HAL_TIM_PWM_ConfigChannel(&htimRead, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.Pulse = 32;
	if (HAL_TIM_PWM_ConfigChannel(&htimRead, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
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
	{
		Error_Handler();
	}

	// Custom configuration
	htimRead.Instance->DIER = 0xe00;
	htimRead.Instance->CCER |= (uint32_t)(1U << ((TIM_CHANNEL_1 | TIM_CHANNEL_2| TIM_CHANNEL_3) & 0x1FU));
	__HAL_TIM_MOE_ENABLE(&htimRead);
	__HAL_TIM_ENABLE_IT(&htimRead, TIM_IT_UPDATE);
	/* TIM8 interrupt Init */
	HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
}

static void DataCollectDMAs_Init(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_TIM8_CLK_ENABLE();

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
	hdma_rd_low.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_rd_low.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_rd_low) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC1],hdma_rd_low);

	/* TIM8_CH2 Init */
	hdma_acq_data.Instance = DMA1_Stream1;
	hdma_acq_data.Init.Request = DMA_REQUEST_TIM8_CH2;
	hdma_acq_data.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_acq_data.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_acq_data.Init.MemInc = DMA_MINC_ENABLE;
	hdma_acq_data.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_acq_data.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_acq_data.Init.Mode = DMA_CIRCULAR;
	hdma_acq_data.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_acq_data.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_acq_data) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC2],hdma_acq_data);

	/* TIM8_CH3 Init */
	hdma_rd_high.Instance = DMA1_Stream2;
	hdma_rd_high.Init.Request = DMA_REQUEST_TIM8_CH3;
	hdma_rd_high.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_rd_high.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_rd_high.Init.MemInc = DMA_MINC_DISABLE;
	hdma_rd_high.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_rd_high.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_rd_high.Init.Mode = DMA_CIRCULAR;
	hdma_rd_high.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_rd_high.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_rd_high) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC3],hdma_rd_high);

	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);

	// Enable the DMAs
	HAL_DMA_Start(&hdma_rd_low, (uint32_t)&rdLow, (uint32_t)&maxRead_GPIO_Port->BSRR, 16);
#if MANUAL_RD_SWITCH
	HAL_DMA_Start(&hdma_acq_data, (uint32_t)&toggle[0], (uint32_t)&maxRead_GPIO_Port->BSRR, 2);
#else
	HAL_DMA_Start(&hdma_acq_data, (uint32_t)&MAX11046_GPIO->IDR, (uint32_t)&rawData->dataRecord[recordIndex << 4], 16);
#endif
	HAL_DMA_Start(&hdma_rd_high, (uint32_t)&rdHigh, (uint32_t)&maxRead_GPIO_Port->BSRR, 16);
#if USE_CS_DMA
	HAL_DMA_Start(&hdma_tim8_up, (uint32_t)&csList[0], (uint32_t)&maxCS1_GPIO_Port->BSRR, 2);
	HAL_DMA_Start(&hdma_dma_generator3, (uint32_t)&timList[0], (uint32_t)&TIM8->CR1, 2);
#endif
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

void DMA1_Stream2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_rd_high);
}

#endif

/**
 * @brief Configures the sensitivities and offsets for all measurements
 */
static void ConfigureMeasurements(void)
{
	adcInfo.offsets = (float*)&adcOffsets;
	adcInfo.sensitivty = (float*)&adcMultipiers;
	///////////// --TODO-- configure the measurement frequency etc
	adcInfo.stats = &stats;

	// implementation of Custom PEControllers is user controlled
#if	PECONTROLLER_CONFIG == PEC_CUSTOM
	for (int i = 0; i < 16; i++)
	{
		adcConfig.sensitivty[i] = 10.f / (32768.f);
		adcConfig.offsets[i] = 32768;
	}
#else
	for (int i = 0; i < MEASUREMENT_COUNT_CURRENT; i++)
	{
		adcConfig.sensitivty[i] = 10000.f / (CURRENT_SENSITIVITY_mVA * 32768.f);
		adcConfig.offsets[i] = 32768;
	}
	for (int i = 0; i < MEASUREMENT_COUNT_VOLTAGE; i++)
	{
		adcConfig.sensitivty[i + 8] = 1000 / 32768.f;
		adcConfig.offsets[i + 8] = 32768;
	}
#endif
}

/**
 * @brief Initializes the MAX11046 drivers
 * @param type- ADC_MODE_SINGLE or ADC_MODE_CONT for single or continuous conversions respectively
 * @param *contConfig- adc_cont_config_t contains the continuous transfer configuration
 */
void BSP_MAX11046_Init(adc_acq_mode_t type, adc_cont_config_t* contConfig, adc_raw_data_t* rawAdcData, adc_processed_data_t* processedAdcData)
{
	// DeInitialize if already initialized
	if(moduleActive)
		BSP_MAX11046_DeInit();

	// set parameters
	rawData = rawAdcData;
	processedData = processedAdcData;
	ConfigureMeasurements();

#if ENABLE_INTELLISENS
	intelliSENS_Configure();
#endif

	acqType = type;
	adcContConfig.conversionCycleTimeUs = contConfig->conversionCycleTimeUs;
	adcContConfig.callback = contConfig->callback;

	GPIOs_Init();
	DataCollectDMAs_Init();
	ReadTimer_Init();
	ConversionTimer_Init();

	moduleActive = true;
}

/**
 * @brief Performs the conversion
 * @details For single conversion mode performs and provides the results in a blocking way.
 * For continuous conversion mode starts the conversions, after each conversion adc_cont_config_t.callback will be called
 * @return uint16_t*- For single conversion mode returns the pointer to the eight acquired values. Returns NULL for continuous conversion mode
 */
adc_measures_t* BSP_MAX11046_Run(void)
{
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
		return &adcVals;
	}
	else
	{
		// enable PWM to produce continuous data
		HAL_TIM_PWM_Start(&htimCnv,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htimCnv,TIM_CHANNEL_2);
		return NULL;
	}
}

/**
 * @brief Stops the ADC data collection module, only effective for ADC_MODE_CONT
 */
void BSP_MAX11046_Stop(void)
{
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

static bool reset = true;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == maxBusy1_Pin)
	{
#if MANUAL_RD_SWITCH
		CollectConvertData_BothADCs((float*)&adcVals, (const float*)&adcMultipiers, (const float*)&adcOffsets);
		if(adcContConfig.callback)
			adcContConfig.callback(&adcVals);
#endif
#if USE_DMA
		reset = true;
#if MANUAL_RD_SWITCH
		GPIOB->BSRR = (1U << 2);
		GPIOA->BSRR = (1U << (15 + 16));
#else
		__HAL_DMA_DISABLE(&hdma);
		hdma_acq_data.Instance->M0AR = (uint32_t)&rawData->dataRecord[rawData->recordIndex << 4];
		__HAL_DMA_ENABLE(&hdma);
		maxCS1_GPIO_Port->BSRR = ((uint32_t)maxCS2_Pin << 0) | ((uint32_t)maxCS1_Pin << 16U);
#endif
		__HAL_TIM_ENABLE(&htimRead);
#endif
	}
}
#if USE_DMA
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
void TIM8_UP_TIM13_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htimRead, TIM_IT_UPDATE);
	if (reset == true)
	{
		reset = false;
#if MANUAL_RD_SWITCH
		GPIOB->BSRR = (1U << (2 + 16));
		GPIOA->BSRR = (1U << 15);
#else
		maxCS1_GPIO_Port->BSRR = ((uint32_t)maxCS1_Pin << 0) | ((uint32_t)maxCS2_Pin << 16U);
#endif
		__HAL_TIM_ENABLE(&htimRead);
	}
	else
	{
#if MANUAL_RD_SWITCH
		GPIOB->BSRR = (1U << (2));
		GPIOA->BSRR = (1U << (15));
#else
		maxCS1_GPIO_Port->BSRR = ((uint32_t)maxCS2_Pin << 0) | ((uint32_t)maxCS1_Pin << 0);
		CollectConvertData_BothADCs((float*)&processedData->dataRecord[processedData->recordIndex], adcConfig.sensitivity, adcConfig.offsets);
		if(adcContConfig.callback)
			adcContConfig.callback(&adcVals);
		rawData->recordIndex = (rawData->recordIndex + 1) % RAW_MEASURE_SAVE_COUNT;
		data->lastDataPointer = &processedData->dataRecord[processedData->recordIndex];
		processedData->recordIndex = (processedData->recordIndex + 1) % MEASURE_SAVE_COUNT;
#endif
	}
}
#endif


/* EOF */
