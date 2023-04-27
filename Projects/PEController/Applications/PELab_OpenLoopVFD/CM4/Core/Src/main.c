/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_config.h"
#include "max11046_drivers.h"
#include "shared_memory.h"
#include "pecontroller_display.h"
#include <string.h>
#include "user_config.h"
#if ENABLE_INTELLISENS
#include "intelliSENS.h"
#endif
#include "lvgl.h"
#include "pecontroller_ts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim17;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for lvglGUITask */
osThreadId_t lvglGUITaskHandle;
const osThreadAttr_t lvglGUITask_attributes = {
  .name = "lvglGUITask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for lvglTickTask */
osThreadId_t lvglTickTaskHandle;
const osThreadAttr_t lvglTickTask_attributes = {
  .name = "lvglTickTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for tsTask */
osThreadId_t tsTaskHandle;
const osThreadAttr_t tsTask_attributes = {
  .name = "tsTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for touchSemaphore */
osSemaphoreId_t touchSemaphoreHandle;
osStaticSemaphoreDef_t touchSemaphoreControlBlock;
const osSemaphoreAttr_t touchSemaphore_attributes = {
  .name = "touchSemaphore",
  .cb_mem = &touchSemaphoreControlBlock,
  .cb_size = sizeof(touchSemaphoreControlBlock),
};
/* USER CODE BEGIN PV */
// intelliSENS
#if ENABLE_INTELLISENS
osThreadId_t intelliSENSTaskHandle;
const osThreadAttr_t intelliSENSTask_attributes = {
		.name = "intelliSENSTask",
		.stack_size = 128 * 16,
		.priority = (osPriority_t) osPriorityHigh,
};
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_TIM17_Init(void);
void StartDefaultTask(void *argument);
void StartLvglGUITask(void *argument);
void StartLvglTickTask(void *argument);
void StartTSTask(void *argument);

/* USER CODE BEGIN PFP */
void ScreenManager_Poll(void);
#if ENABLE_INTELLISENS
void StartintelliSENSTask(void *argument);
#endif

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
#if ADC_CORE == ADC_CM4
	memset((void*)&(sharedData->m4Tom7), 0, sizeof(m4_to_m7_data_t));
	sharedData->m4Tom7.processedAdcData.lastDataPointer = sharedData->m4Tom7.processedAdcData.dataRecord;
#endif
  /* USER CODE END 1 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
	/*HW semaphore Clock enable*/
	__HAL_RCC_HSEM_CLK_ENABLE();
	/* Activate HSEM notification for Cortex-M4*/
	HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
	/*
  Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
  perform system initialization (system clock config, external memory configuration.. )
	 */
	HAL_PWREx_ClearPendingEvent();
	HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);
	/* Clear HSEM flag */
	__HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
#if ADC_CORE == ADC_CM4
	adc_cont_config_t adcConfig = {
			.callback = NULL,
			.conversionCycleTimeUs = sharedData->m7Tom4.periodUs };
	BSP_MAX11046_Init(ADC_MODE_CONT, &adcConfig, &sharedData->m4Tom7.rawAdcData, &sharedData->m4Tom7.processedAdcData);
#endif
	BSP_Display_Init();
	BSP_Display_ShowLogo();
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);			// LCD Brightness
#if ADC_CORE == ADC_CM4
	BSP_MAX11046_Run();
#endif
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of touchSemaphore */
  touchSemaphoreHandle = osSemaphoreNew(1, 1, &touchSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of lvglGUITask */
  lvglGUITaskHandle = osThreadNew(StartLvglGUITask, NULL, &lvglGUITask_attributes);

  /* creation of lvglTickTask */
  lvglTickTaskHandle = osThreadNew(StartLvglTickTask, NULL, &lvglTickTask_attributes);

  /* creation of tsTask */
  tsTaskHandle = osThreadNew(StartTSTask, NULL, &tsTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
#if ENABLE_INTELLISENS
	intelliSENSTaskHandle = osThreadNew(StartintelliSENSTask, NULL, &intelliSENSTask_attributes);
#endif
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 10;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 960;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 950;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
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
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim17, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */
	sConfigOC.Pulse = (htim17.Init.Prescaler - 1) * LCD_BRIGHTNESS;
	if (HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		Error_Handler();
  /* USER CODE END TIM17_Init 2 */
  HAL_TIM_MspPostInit(&htim17);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, maxCS1_Pin|maxCS2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, maxWrite_Pin|LCD_Disp_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(maxRead_GPIO_Port, maxRead_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS1_GPIO_Port, CS1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS2_GPIO_Port, CS2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, RD_Pin|VAL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : maxCS1_Pin maxCS2_Pin */
  GPIO_InitStruct.Pin = maxCS1_Pin|maxCS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : maxWrite_Pin LCD_Disp_Pin */
  GPIO_InitStruct.Pin = maxWrite_Pin|LCD_Disp_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : maxRead_Pin */
  GPIO_InitStruct.Pin = maxRead_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(maxRead_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : maxBusy1_Pin */
  GPIO_InitStruct.Pin = maxBusy1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(maxBusy1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : maxBusy2_Pin */
  GPIO_InitStruct.Pin = maxBusy2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
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

  /*Configure GPIO pin : CS1_Pin */
  GPIO_InitStruct.Pin = CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CS2_Pin */
  GPIO_InitStruct.Pin = CS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RD_Pin VAL_Pin */
  GPIO_InitStruct.Pin = RD_Pin|VAL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
#if ENABLE_INTELLISENS
void StartintelliSENSTask(void *argument)
{
	for(;;)
	{
		intelliSENS.Poll();
		osDelay(1);
	}
}
#endif
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartLvglGUITask */
/**
 * @brief Function implementing the lvglGUITask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLvglGUITask */
void StartLvglGUITask(void *argument)
{
  /* USER CODE BEGIN StartLvglGUITask */
	/* Infinite loop */
	for(;;)
	{
		//if(osSemaphoreAcquire(touchSemaphoreHandle, 0) == osOK)
		//{
		//ScreenManager_Poll();
		//osSemaphoreRelease(touchSemaphoreHandle);
		//}
		osDelay(200);
	}
  /* USER CODE END StartLvglGUITask */
}

/* USER CODE BEGIN Header_StartLvglTickTask */
/**
 * @brief Function implementing the lvglTickTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLvglTickTask */
void StartLvglTickTask(void *argument)
{
  /* USER CODE BEGIN StartLvglTickTask */
	/* Infinite loop */
	for(;;)
	{
		//if(osSemaphoreAcquire(touchSemaphoreHandle, 0) == osOK)
		//{
		static int i = 0;
		if (++i > 4)
		{
			ScreenManager_Poll();
			i = 0;
		}
		lv_tick_inc(20);
		lv_timer_handler();
		//osSemaphoreRelease(touchSemaphoreHandle);
		//}
		osDelay(20);
	}
  /* USER CODE END StartLvglTickTask */
}

/* USER CODE BEGIN Header_StartTSTask */
/**
 * @brief Function implementing the tsTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTSTask */
void StartTSTask(void *argument)
{
  /* USER CODE BEGIN StartTSTask */
	while(BSP_TS_Init(800, 480) != TS_OK)
		osDelay(100);
	/* Infinite loop */
	for(;;)
	{
		BSP_TS_Poll();
		osDelay(10);
	}
  /* USER CODE END StartTSTask */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
