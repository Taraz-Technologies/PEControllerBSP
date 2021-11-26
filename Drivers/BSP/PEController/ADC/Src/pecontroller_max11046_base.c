/*! 
@file raw_max11046.c
@brief 
@details 

@author Hamza Naeem Kakahel
@copyright Taraz Technologies Pvt. Ltd.
*/
#ifdef CORE_CM4
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include "pecontroller_max11046_base.h"
#include "main.h"
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
static volatile bool moduleActive = false;
static adc_cont_config_t adcContConfig = { .conversionCycleTimeUs = 40, .callback = NULL };
static adc_acq_mode_t acqType = ADC_MODE_CONT;
uint16_t u16Data[16];

TIM_HandleTypeDef maxTimerHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static inline void AcquireMeasurements(GPIO_TypeDef* csGPIO, uint16_t csPin, uint16_t* dataPtr)
{
	int i = 8;
	csGPIO->BSRR = (uint32_t)csPin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
		*dataPtr++ = (uint16_t)MAX_11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;		
	}	while (--i);
	csGPIO->BSRR = csPin;
}

static inline void AcquireAllMeasurements(uint16_t* dataPtr)
{
	int i = 8;
	maxCS1_GPIO_Port->BSRR = (uint32_t)maxCS1_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
		*dataPtr++ = (uint16_t)MAX_11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;		
	}	while (--i);
	maxCS1_GPIO_Port->BSRR = maxCS1_Pin;
	i = 8;
	maxCS2_GPIO_Port->BSRR = (uint32_t)maxCS2_Pin << 16U;
	do
	{
		maxRead_GPIO_Port->BSRR = (uint32_t)maxRead_Pin << 16U;
		*dataPtr++ = (uint16_t)MAX_11046_GPIO->IDR;
		maxRead_GPIO_Port->BSRR = maxRead_Pin;		
	}	while (--i);
	maxCS2_GPIO_Port->BSRR = maxCS2_Pin;
}

/*! @brief Get the data results */
static inline void ReadResults(void)
{
	AcquireAllMeasurements(u16Data);
//	AcquireMeasurements(maxCS2_GPIO_Port, maxCS2_Pin, u16Data + 8);
//	AcquireMeasurements(maxCS1_GPIO_Port, maxCS1_Pin, u16Data);
}


/*! @brief Deinitialize the max 11046 module */
void BSP_MAX11046_Base_DeInit(void)
{
	// clear the flag, then enable neg edge interrupt, then assign callback to relevant port pin
	HAL_GPIO_DeInit(maxBusy1_GPIO_Port,maxBusy1_Pin);
	HAL_GPIO_DeInit(maxBusy2_GPIO_Port,maxBusy2_Pin);
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy1_Pin);
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy2_Pin);
	
	HAL_TIM_PWM_DeInit(&maxTimerHandle);
	moduleActive = false;
} 

static void Timer_Config(void)
{
	  /* USER CODE BEGIN TIM12_Init 0 */

  /* USER CODE END TIM12_Init 0 */
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM12_Init 1 */

  /* USER CODE END TIM12_Init 1 */
  maxTimerHandle.Instance = TIM12;
  maxTimerHandle.Init.Prescaler = 0;
  maxTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  maxTimerHandle.Init.Period = (uint16_t)(240 * adcContConfig.conversionCycleTimeUs);		// --todo-- configure 240 seperately
  maxTimerHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  maxTimerHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&maxTimerHandle) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = TIMER_PULSE;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&maxTimerHandle, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&maxTimerHandle, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM12_Init 2 */
	
  /* USER CODE END TIM12_Init 2 */
  HAL_TIM_MspPostInit(&maxTimerHandle);
	
	MAX_11046_TIMER->CCR1 = MAX_11046_TIMER->CCR2 = TIMER_PULSE;
	if(acqType == ADC_MODE_SINGLE)
		HAL_TIM_OnePulse_Init(&maxTimerHandle, TIM_OPMODE_SINGLE);
}

/*!
@brief Initializes the raw max11046 module 
@param type- ADC_MODE_SINGLE or ADC_MODE_CONT for single or continuous conversions respectively
@param *contConfig- adc_cont_config_t contains the continuous transfer configuration
*/
void BSP_MAX11046_Base_Init(adc_acq_mode_t type, adc_cont_config_t* contConfig)
{
	// DeInitialize if already initialized
	if(moduleActive)
		Raw_Max11046_DeInit();
	
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

/*! 
@brief Perform the conversion
@details For single conversion mode performs and provides the results in a blocking way.
For continuous conversion mode starts the conversions, after each conversion adc_cont_config_t.callback will be called
@returns uint16_t*- For single conversion mode returns the pointer to the eight acquired values. Returns NULL for continuous conversion mode
*/
uint16_t* BSP_MAX11046_Base_Run(void)
{
		/* EXTI interrupt init*/
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy1_Pin);
	__HAL_GPIO_EXTI_CLEAR_IT(maxBusy2_Pin);
	 HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	if(acqType == ADC_MODE_SINGLE)
	{
		// wait for the pending operations to end
		while(HAL_GPIO_ReadPin(maxBusy1_GPIO_Port,maxBusy1_Pin));
	
		HAL_TIM_OnePulse_Start(&maxTimerHandle,TIM_OPMODE_SINGLE);
//		ResPending = true;
//		while(ResPending);
		return u16Data;
	}
	else
	{
		// enable pwm to produce continuous data
		HAL_TIM_PWM_Start(&maxTimerHandle,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&maxTimerHandle,TIM_CHANNEL_2);
		return NULL;
	}
}
/*! @brief Stops the adc data collection module, only effective for ADC_MODE_CONT */
void BSP_MAX11046_Base_Stop(void)
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

//static void MaxTimerInit()
//{
//	MX_TIM12_Init();
//	if(acqType != ADC_MODE_CONT)
//		HAL_TIM_OnePulse_Init(&maxTimerHandle,TIM_OPMODE_SINGLE);
//	MAX_11046_TIMER->ARR = (uint16_t)(240 * adcContConfig.conversionCycleTimeUs);
//	MAX_11046_TIMER->CCR1 = MAX_11046_TIMER->CCR2 = TIMER_PULSE;
//	
//}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == maxBusy1_Pin)
	{
		ReadResults();
		if(adcContConfig.callback)
			adcContConfig.callback(u16Data);
	}
}
 
#endif
/* EOF */
