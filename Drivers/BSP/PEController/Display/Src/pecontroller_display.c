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
#include "user_config.h"
#include "pecontroller_display.h"
#if DISPLAY == LCD_AFY800480A0 || DISPLAY == LCD_AFY800480B0
#include "LCD_AFY800480A0_B0.h"
#endif
#include "logo.h"
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
static void BSP_Display_ShowLayer(LTDC_LayerCfgTypeDef* pLayerCfg)
{
	if (HAL_LTDC_ConfigLayer(&hltdc, pLayerCfg, 0) != HAL_OK)
		Error_Handler();
}

/**
 * @brief Displays the first layer after applying configuration
 * @param framePtr Pointer to the frame to be displayed
 * @param config Pointer to the configurations for the layer
 */
void BSP_Display_ShowFirstLayer(const uint8_t* framePtr, image_config_t* config)
{
	LTDC_LayerCfgTypeDef pLayerCfg = {0};
	pLayerCfg.WindowX0 = config->xAlign == ALIGN_RIGHT_X ? 0 :
			(config->xAlign == ALIGN_LEFT_X ? DISPLAY_WIDTH - config->ImageWidth :
					(DISPLAY_WIDTH - config->ImageWidth) / 2);
	pLayerCfg.WindowX1 = pLayerCfg.WindowX0 + config->ImageWidth;
	pLayerCfg.WindowY0 = config->yAlign == ALIGN_DOWN_Y ? 0 :
			(config->yAlign == ALIGN_UP_Y ? DISPLAY_HEIGHT - config->ImageHeight :
					(DISPLAY_HEIGHT - config->ImageHeight) / 2);
	pLayerCfg.WindowY1 = pLayerCfg.WindowY0 + config->ImageHeight;

	/* Pixel Format configuration*/
	pLayerCfg.PixelFormat = config->PixelFormat;

	/* Start Address configuration : frame buffer is located at FLASH memory */
	pLayerCfg.FBStartAdress = (uint32_t)framePtr;

	/* Alpha constant (255 == totally opaque) */
	pLayerCfg.Alpha = 255;

	/* Default Color configuration (configure A,R,G,B component values) : no background color */
	pLayerCfg.Alpha0 = 255; /* fully transparent */
	pLayerCfg.Backcolor.Blue = LOGO_BACK_BLUE;
	pLayerCfg.Backcolor.Green = LOGO_BACK_GREEN;
	pLayerCfg.Backcolor.Red = LOGO_BACK_RED;

	/* Configure blending factors */
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

	/* Configure the number of lines and number of pixels per line */
	pLayerCfg.ImageWidth  = config->ImageWidth;
	pLayerCfg.ImageHeight = config->ImageHeight;
	BSP_Display_ShowLayer(&pLayerCfg);
}

/**
 * @brief Display the selected LOGO. Use the user_config.h file to select appropriate LOGO
 */
void BSP_Display_ShowLogo(void)
{
	image_config_t config = {0};
	config.xAlign = LOGO_ALIGN_X;
	config.yAlign = LOGO_ALIGN_Y;
	config.ImageHeight = LOGO_HEIGHT;
	config.ImageWidth = LOGO_WIDTH;
	config.PixelFormat = LOGO_FORMAT;
	BSP_Display_ShowFirstLayer(logo, &config);
}

static void ConfigLTDC(void)
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
	hltdc.Init.Backcolor.Blue = 255;
	hltdc.Init.Backcolor.Green = 255;
	hltdc.Init.Backcolor.Red = 255;
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

