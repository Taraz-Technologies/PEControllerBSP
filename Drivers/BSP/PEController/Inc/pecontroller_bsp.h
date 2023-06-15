/**
 ********************************************************************************
 * @file 		pecontroller_bsp.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 26, 2021
 *
 * @brief    
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

#ifndef PECONTROLLER_BSP_H_
#define PECONTROLLER_BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "stm32h7xx_hal.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
/********************************************************************************
 * Defines
 *******************************************************************************/
#define maxCS1_Pin GPIO_PIN_2
#define maxCS1_GPIO_Port GPIOE
#define maxCS2_Pin GPIO_PIN_3
#define maxCS2_GPIO_Port GPIOE
#define maxWrite_Pin GPIO_PIN_8
#define maxWrite_GPIO_Port GPIOI
#define maxRead_Pin GPIO_PIN_13
#define maxRead_GPIO_Port GPIOC
#define maxBusy1_Pin GPIO_PIN_14
#define maxBusy1_GPIO_Port GPIOC
#define maxBusy2_Pin GPIO_PIN_15
#define maxBusy2_GPIO_Port GPIOC
#define maxD0_Pin GPIO_PIN_0
#define maxD0_GPIO_Port GPIOF
#define maxD1_Pin GPIO_PIN_1
#define maxD1_GPIO_Port GPIOF
#define maxD2_Pin GPIO_PIN_2
#define maxD2_GPIO_Port GPIOF
#define maxD3_Pin GPIO_PIN_3
#define maxD3_GPIO_Port GPIOF
#define maxD4_Pin GPIO_PIN_4
#define maxD4_GPIO_Port GPIOF
#define maxD5_Pin GPIO_PIN_5
#define maxD5_GPIO_Port GPIOF
#define maxD6_Pin GPIO_PIN_6
#define maxD6_GPIO_Port GPIOF
#define maxD7_Pin GPIO_PIN_7
#define maxD7_GPIO_Port GPIOF
#define maxD8_Pin GPIO_PIN_8
#define maxD8_GPIO_Port GPIOF
#define maxD9_Pin GPIO_PIN_9
#define maxD9_GPIO_Port GPIOF
#define maxD10_Pin GPIO_PIN_10
#define maxD10_GPIO_Port GPIOF
#define CTP_I2C_SCL_Pin GPIO_PIN_4
#define CTP_I2C_SCL_GPIO_Port GPIOH
#define CTP_I2C_SDA_Pin GPIO_PIN_5
#define CTP_I2C_SDA_GPIO_Port GPIOH
#define maxD11_Pin GPIO_PIN_11
#define maxD11_GPIO_Port GPIOF
#define maxD12_Pin GPIO_PIN_12
#define maxD12_GPIO_Port GPIOF
#define maxD13_Pin GPIO_PIN_13
#define maxD13_GPIO_Port GPIOF
#define maxD14_Pin GPIO_PIN_14
#define maxD14_GPIO_Port GPIOF
#define maxD15_Pin GPIO_PIN_15
#define maxD15_GPIO_Port GPIOF
#define maxCNV1_Pin GPIO_PIN_6
#define maxCNV1_GPIO_Port GPIOH
#define maxCNV2_Pin GPIO_PIN_15
#define maxCNV2_GPIO_Port GPIOB
#define CTP_INT_Pin GPIO_PIN_2
#define CTP_INT_GPIO_Port GPIOG
#define CTP_RST_Pin GPIO_PIN_3
#define CTP_RST_GPIO_Port GPIOG
#define LCD_Disp_Pin GPIO_PIN_2
#define LCD_Disp_GPIO_Port GPIOI

#define MAX11046_GPIO GPIOF
#define ADC_TIMER TIM12
#define MAX11046_CLK_Us	240					// --todo-- configure 240 seperately

/*********** Device Constants **************/
#define TOTAL_MEASUREMENT_COUNT			(16)
/*********** Device Constants **************/

#define TCritical						__attribute__((section (".tCritical")))

// Inputs
#define IS_CONTROL_CORE							(defined(CORE_CM7))
#define IS_COMMS_CORE							(defined(CORE_CM4))
#define IS_STORAGE_CORE							(defined(CORE_CM4))
#define IS_ADC_STATS_CORE						(defined(CORE_CM4))
#define ADC_BULK_STATS							(1)
#define IS_ADC_CM7								(1)
#define MAX11046_ENABLE							(1)
#define IS_DMA_ADC_DATA_COLLECTION_SUPERIOR		(0) 				// Running from DMA takes more time in both cases.
#define IS_LOCAL_STORAGE_FASTER					(defined(CORE_CM7)) // Local storage is only faster for the CM4 core.

// Computations
#define IS_ADC_CM4						(!IS_ADC_CM7)
#define IS_ADC_CORE						((IS_ADC_CM7 && defined(CORE_CM7)) || (IS_ADC_CM4 && defined(CORE_CM4)))

/************** Debugging *****************/
#define ENABLE_PROFILING				(1)
#if ENABLE_PROFILING
#define PROFILE_GPIO_PORT				(GPIOB)
#define PROFILE_GPIO_Pin				GPIO_PIN_2
#endif
/************** Debugging *****************/

/************** HELPERS *******************/
#define SET_Pin(GPIO, PinMask)			GPIO->BSRR = PinMask;
#define CLR_Pin(GPIO, PinMask)			GPIO->BSRR = (PinMask << 16U);
/************** HELPERS *******************/
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
extern void Error_Handler(void);
/********************************************************************************
 * Code
 *******************************************************************************/
static inline void BSP_GPIOStruct_DefaultConfig(GPIO_InitTypeDef* GPIO_InitStruct, uint32_t pin, uint32_t mode, uint32_t alternate)
{
	GPIO_InitStruct->Pin = pin;
	GPIO_InitStruct->Mode = mode;
	GPIO_InitStruct->Pull = GPIO_NOPULL;
	GPIO_InitStruct->Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct->Alternate = alternate;
}

static inline void BSP_GPIOStruct_IOConfig(GPIO_InitTypeDef* GPIO_InitStruct, uint32_t pin, uint32_t mode, uint32_t speed)
{
	GPIO_InitStruct->Pin = pin;
	GPIO_InitStruct->Mode = mode;
	GPIO_InitStruct->Pull = GPIO_NOPULL;
	GPIO_InitStruct->Speed = speed;
}

#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
