/**
 ********************************************************************************
 * @file    	pecontroller_display.c
 * @author 		Waqas Ehsan Butt
 * @date    	December 3, 2021
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

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "pecontroller_display.h"
#include "user_config.h"
#if DISPLAY == LCD_AFY800480A0 || DISPLAY == LCD_AFY800480B0
#include "LCD_AFY800480A0_B0.h"
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
/** Handle for the LTDC module
 */
static LTDC_HandleTypeDef hltdc;
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
 * @brief Configure the clock for LTDC module
 * @retval None
 */
static void ConfigClock(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct =PLL3Clk;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		Error_Handler();

	__HAL_RCC_LTDC_CLK_ENABLE();

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOK_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
}

/**
 * @brief Configure the IOs for LCD
 * @retval None
 */
static void ConfigIO(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/**LTDC GPIO Configuration
	    PE4     ------> LTDC_B0
	    PE5     ------> LTDC_G0
	    PE6     ------> LTDC_G1
	    PI9     ------> LTDC_VSYNC
	    PI10     ------> LTDC_HSYNC
	    PI11     ------> LTDC_G6
	    PH2     ------> LTDC_R0
	    PH3     ------> LTDC_R1
	    PI15     ------> LTDC_G2
	    PE13     ------> LTDC_DE
	    PE14     ------> LTDC_CLK
	    PE15     ------> LTDC_R7
	    PH8     ------> LTDC_R2
	    PH9     ------> LTDC_R3
	    PH10     ------> LTDC_R4
	    PH11     ------> LTDC_R5
	    PH12     ------> LTDC_R6
	    PD10     ------> LTDC_B3
	    PK2     ------> LTDC_G7
	    PH14     ------> LTDC_G3
	    PH15     ------> LTDC_G4
	    PI0     ------> LTDC_G5
	    PG10     ------> LTDC_B2
	    PG12     ------> LTDC_B1
	    PI4     ------> LTDC_B4
	    PI5     ------> LTDC_B5
	    PI6     ------> LTDC_B6
	    PI7     ------> LTDC_B7
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_13
			|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_4
			|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9
			|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_14
			|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/**
 * @brief Shows the configured layer on the LCD
 * @param *pLayerCfg Layer configuration to be applied to the LTDC module
 */
void BSP_Display_ShowLayer(LTDC_LayerCfgTypeDef* pLayerCfg)
{
	if (HAL_LTDC_ConfigLayer(&hltdc, pLayerCfg, 0) != HAL_OK)
		Error_Handler();
}

void ConfigLTDC(void)
{
	hltdc.Instance = LTDC;
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
	hltdc.Init.HorizontalSync = HORIZONTAL_SYNC;
	hltdc.Init.VerticalSync = VERTICAL_SYNC;
	hltdc.Init.AccumulatedHBP = ACCUMULATED_HBP;
	hltdc.Init.AccumulatedVBP = ACCUMULATED_VBP;
	hltdc.Init.AccumulatedActiveW = ACCUMULATED_ACTIVE_W;
	hltdc.Init.AccumulatedActiveH = ACCUMULATED_ACTIVE_H;
	hltdc.Init.TotalWidth = TOTAL_WIDTH;
	hltdc.Init.TotalHeigh = TOTAL_HEIGHT;
	hltdc.Init.Backcolor.Blue = 0;
	hltdc.Init.Backcolor.Green = 255;
	hltdc.Init.Backcolor.Red = 0;
	if (HAL_LTDC_Init(&hltdc) != HAL_OK)
		Error_Handler();
}

/**
 * @brief Initializes the display module
 */
void BSP_Display_Init(void)
{
	ConfigClock();
	ConfigIO();
	ConfigLTDC();
}

/**
 * @brief De-initializes the display module
 */
void BSP_Display_DeInit(void)
{
	__HAL_RCC_LTDC_CLK_DISABLE();

	HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_13
			|GPIO_PIN_14|GPIO_PIN_15);

	HAL_GPIO_DeInit(GPIOI, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_15
			|GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
			|GPIO_PIN_7);

	HAL_GPIO_DeInit(GPIOH, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9
			|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_14
			|GPIO_PIN_15);

	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_10);

	HAL_GPIO_DeInit(GPIOK, GPIO_PIN_2);

	HAL_GPIO_DeInit(GPIOG, GPIO_PIN_10|GPIO_PIN_12);
}


/* EOF */

