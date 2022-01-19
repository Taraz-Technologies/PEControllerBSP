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
#include "max11046_drivers.h"
#define ENABLE_INTELLISENS					(1)
#if ENABLE_INTELLISENS
#include "intelliSENS.h"
#endif
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
/** module status
 */
static volatile bool moduleActive = false;
/** Current applied ADC configurations
 */
static adc_cont_config_t adcContConfig = { .conversionCycleTimeUs = 40, .callback = NULL };
/** Current ADC acquisition mode
 */
static adc_acq_mode_t acqType = ADC_MODE_CONT;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
TIM_HandleTypeDef maxTimerHandle;
/** Contains the latest values of the acquired ADC readings
 */
adc_measures_t adcVals;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Acquire measurements for one of the two ADCs
 * @param *csGPIO Pointer to the GPIO of chip select for specific ADC
 * @param csPin Chip select pin number
 * @param *dataPtr pointer to where the data needs to be stored
 */
static inline void Measure_SingleADC(GPIO_TypeDef* csGPIO, uint16_t csPin, uint16_t* dataPtr)
{
	int i = 8;
	csGPIO->BSRR = (uint32_t)csPin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
		*dataPtr++ = (uint16_t)MAX11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
	}	while (--i);
	csGPIO->BSRR = csPin;
}

/**
 * @brief Acquire measurements for both ADCs
 * @param *dataPtr Pointer to where the data needs to be stored
 */
static inline void Measure_BothADCs(uint16_t* dataPtr)
{
	int i = 8;
#if ENABLE_INTELLISENS
	uint16_t* intelliSENSDataPtr = dataPtr;
#endif
	maxCS1_GPIO_Port->BSRR = (uint32_t)maxCS1_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
		*dataPtr++ = (uint16_t)MAX11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
	}	while (--i);
	maxCS1_GPIO_Port->BSRR = maxCS1_Pin;
	i = 8;
	maxCS2_GPIO_Port->BSRR = (uint32_t)maxCS2_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
		*dataPtr++ = (uint16_t)MAX11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
	}	while (--i);
#if ENABLE_INTELLISENS
	intelliSENS.SetADCData(intelliSENSDataPtr);
#endif
	maxCS2_GPIO_Port->BSRR = maxCS2_Pin;
}

// 8849____8237
/**
 * @brief Acquire measurements and convert to meaningful data for both ADCs
 * @param *dataPtr Pointer to where the data needs to be stored
 * @param *mults Pointer to the multiplier information
 * @param *offsets Pointer to the offset information
 */
static inline void MeasureConvert_BothADCs____(float* dataPtr, const float* mults, const float* offsets)
{
	int i = 8;
	uint16_t intelliSENSDataPtr[16];
	uint16_t* tempData = intelliSENSDataPtr;
	maxCS1_GPIO_Port->BSRR = (uint32_t)maxCS1_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
		*tempData = (uint16_t)MAX11046_GPIO->IDR;
		*dataPtr++ = (*tempData++ - *offsets++) * (*mults++);
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
	}	while (--i);
	maxCS1_GPIO_Port->BSRR = maxCS1_Pin;
	i = 8;
	maxCS2_GPIO_Port->BSRR = (uint32_t)maxCS2_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
		*tempData = (uint16_t)MAX11046_GPIO->IDR;
		*dataPtr++ = (*tempData++ - *offsets++) * (*mults++);
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
	}	while (--i);
	maxCS2_GPIO_Port->BSRR = maxCS2_Pin;
#if ENABLE_INTELLISENS
	intelliSENS.SetADCData(intelliSENSDataPtr);
#endif
}

//int i = 8, j = 8;

/* 731
	volatile uint32_t t1 = SysTick->VAL;
	int i = 8;
	uint16_t intelliSENSDataPtr[16];
	uint16_t* tempData = intelliSENSDataPtr;
	maxCS1_GPIO_Port->BSRR = (uint32_t)maxCS1_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
 *tempData++ = (uint16_t)MAX11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
	}	while (--i);
	maxCS1_GPIO_Port->BSRR = maxCS1_Pin;
	i = 8;
	maxCS2_GPIO_Port->BSRR = (uint32_t)maxCS2_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
 *tempData++ = (uint16_t)MAX11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
	}	while (--i);
	maxCS2_GPIO_Port->BSRR = maxCS2_Pin;
	tEnd = SysTick->VAL;
	if (t1 > tEnd)
		tempDiff1 = t1 - tEnd;
#if ENABLE_INTELLISENS
	intelliSENS.SetADCData(intelliSENSDataPtr);
#endif

 */

/* 877
	volatile uint32_t t1 = SysTick->VAL;
	static int i = 8;
	static int j = 8;
	uint16_t intelliSENSDataPtr[16];
	uint16_t* tempData = intelliSENSDataPtr;
	maxCS1_GPIO_Port->BSRR = (uint32_t)maxCS1_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
 *tempData++ = (uint16_t)MAX11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
	}	while (--i);
	maxCS1_GPIO_Port->BSRR = maxCS1_Pin;
	maxCS2_GPIO_Port->BSRR = (uint32_t)maxCS2_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
 *tempData++ = (uint16_t)MAX11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;
	}	while (--j);
	maxCS2_GPIO_Port->BSRR = maxCS2_Pin;
	tEnd = SysTick->VAL;
	if (t1 > tEnd)
		tempDiff1 = t1 - tEnd;
#if ENABLE_INTELLISENS
	intelliSENS.SetADCData(intelliSENSDataPtr);
#endif
	i = j = 8;
 */
#define READ_ADC_CH(tempData)			maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U; \
		*tempData++ = (uint16_t)MAX11046_GPIO->IDR; \
		maxRead_GPIO_Port->BSRR = maxRead_Pin;

static inline void Measure_JJJ(uint16_t* tempData)
{
	// ADC1
	maxCS1_GPIO_Port->BSRR = (uint32_t)maxCS1_Pin << 16U;

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

	maxCS2_GPIO_Port->BSRR = maxCS2_Pin;
}

//#pragma GCC push_options
//#pragma GCC optimize ("-Ofast")

uint64_t intelliSENSDataPtr[4];
/**
 * @brief Acquire measurements and convert to meaningful data for both ADCs
 * @param *dataPtr Pointer to where the data needs to be stored
 * @param *mults Pointer to the multiplier information
 * @param *offsets Pointer to the offset information
 */
static inline void MeasureConvert_BothADCs(float* dataPtr, const float* mults, const float* offsets)
{
	uint16_t* tempData = (uint16_t*)intelliSENSDataPtr;
	Measure_JJJ(tempData);

#if ENABLE_INTELLISENS
	intelliSENS.SetADCData((uint64_t*)(intelliSENSDataPtr));
#endif

	int i = 15;
	do
	{
		*dataPtr++ =  (*tempData++ - *offsets++) * (*mults++);
	} while (i--);
}
//#pragma GCC pop_options

static void Timer_Config(void)
{
	TIM_OC_InitTypeDef sConfigOC = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_TIM12_CLK_ENABLE();

	maxTimerHandle.Instance = TIM12;
	maxTimerHandle.Init.Prescaler = 0;
	maxTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	maxTimerHandle.Init.Period = (uint16_t)(MAX11046_CLK_Us * adcContConfig.conversionCycleTimeUs);
#if ENABLE_INTELLISENS
	intelliSENS.Init(adcContConfig.conversionCycleTimeUs, (const float*)&adcMultipiers, (const float*)&adcOffsets);
	intelliSENS.SetADCTicks(adcContConfig.conversionCycleTimeUs * 240);
#endif
	maxTimerHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	maxTimerHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&maxTimerHandle) != HAL_OK)
		Error_Handler();
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = MAX11046_CLK_Us * 2;				// 2Usec pulse
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&maxTimerHandle, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		Error_Handler();
	if (HAL_TIM_PWM_ConfigChannel(&maxTimerHandle, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
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
		HAL_TIM_OnePulse_Init(&maxTimerHandle, TIM_OPMODE_SINGLE);
}

/**
 * @brief Initializes the MAX11046 drivers
 * @param type- ADC_MODE_SINGLE or ADC_MODE_CONT for single or continuous conversions respectively
 * @param *contConfig- adc_cont_config_t contains the continuous transfer configuration
 */
void BSP_MAX11046_Init(adc_acq_mode_t type, adc_cont_config_t* contConfig)
{
	// DeInitialize if already initialized
	if(moduleActive)
		BSP_MAX11046_DeInit();

#if ENABLE_INTELLISENS
	intelliSENS_Configure();
#endif

	acqType = type;
	adcContConfig.conversionCycleTimeUs = contConfig->conversionCycleTimeUs;
	adcContConfig.callback = contConfig->callback;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, maxCS1_Pin|maxCS2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOI, maxWrite_Pin|LCD_Disp_Pin, GPIO_PIN_SET);
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

	Timer_Config();

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
		HAL_TIM_OnePulse_Start(&maxTimerHandle,TIM_OPMODE_SINGLE);
		return &adcVals;
	}
	else
	{
		// enable PWM to produce continuous data
		HAL_TIM_PWM_Start(&maxTimerHandle,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&maxTimerHandle,TIM_CHANNEL_2);
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
		HAL_TIM_PWM_Stop(&maxTimerHandle,TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&maxTimerHandle,TIM_CHANNEL_2);
	}
	else
	{
		HAL_TIM_OnePulse_Stop(&maxTimerHandle,TIM_CHANNEL_1);
		HAL_TIM_OnePulse_Stop(&maxTimerHandle,TIM_CHANNEL_2);
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

	HAL_TIM_PWM_DeInit(&maxTimerHandle);
	moduleActive = false;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == maxBusy1_Pin)
	{
		MeasureConvert_BothADCs((float*)&adcVals, (const float*)&adcMultipiers, (const float*)&adcOffsets);
		if(adcContConfig.callback)
			adcContConfig.callback(&adcVals);
	}
}

/* EOF */
