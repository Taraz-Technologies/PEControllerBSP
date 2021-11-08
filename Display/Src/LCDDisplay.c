/*!
@file LCDDisplay.c
@brief
@details

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "GeneralHeader.h"
#include "RGB565_480x272.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define LCD_AFY800480A0
//#define LCD_AFY800480B0
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
static LTDC_HandleTypeDef hltdc;
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Configure the clock for LTDC module
 * @retval None
 */
static void ConfigClock(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLL3.PLL3M = 2;
	PeriphClkInitStruct.PLL3.PLL3N = 21;
	PeriphClkInitStruct.PLL3.PLL3P = 2;
	PeriphClkInitStruct.PLL3.PLL3Q = 2;
#if defined(LCD_AFY800480A0)
	PeriphClkInitStruct.PLL3.PLL3R = 10;
#elif defined(LCD_AFY800480B0)
	PeriphClkInitStruct.PLL3.PLL3R = 7;
#else
#error ("No appropriate LCD Module is selected!");
#endif
	PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
	PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
	PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
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

void ConfigLTDC(void)
{
	LTDC_LayerCfgTypeDef pLayerCfg = {0};
	hltdc.Instance = LTDC;
	hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
#if defined(LCD_AFY800480A0)
	hltdc.Init.HorizontalSync = 6;
	hltdc.Init.VerticalSync = 6;
	hltdc.Init.AccumulatedHBP = 13;
	hltdc.Init.AccumulatedVBP = 13;
	hltdc.Init.AccumulatedActiveW = 813;
	hltdc.Init.AccumulatedActiveH = 493;
	hltdc.Init.TotalWidth = 819;
	hltdc.Init.TotalHeigh = 499;
#elif defined(LCD_AFY800480B0)
	hltdc.Init.HorizontalSync = 19;
	hltdc.Init.VerticalSync = 9;
	hltdc.Init.AccumulatedHBP = 65;
	hltdc.Init.AccumulatedVBP = 32;
	hltdc.Init.AccumulatedActiveW = 865;
	hltdc.Init.AccumulatedActiveH = 512;
	hltdc.Init.TotalWidth = 1075;
	hltdc.Init.TotalHeigh = 534;
#endif
	hltdc.Init.Backcolor.Blue = 0;
	hltdc.Init.Backcolor.Green = 255;
	hltdc.Init.Backcolor.Red = 0;
	if (HAL_LTDC_Init(&hltdc) != HAL_OK)
		Error_Handler();
	/* Layer1 Configuration ------------------------------------------------------*/
	/* Windowing configuration */
	/* In this case all the active display area is used to display a picture then :
	       Horizontal start = horizontal synchronization + Horizontal back porch = 43
	       Vertical start   = vertical synchronization + vertical back porch     = 12
	       Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1
	       Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
	pLayerCfg.WindowX0 = 0;
	pLayerCfg.WindowX1 = 800;
	pLayerCfg.WindowY0 = 0;
	pLayerCfg.WindowY1 = 320;

	/* Pixel Format configuration*/
	pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;

	/* Start Address configuration : frame buffer is located at FLASH memory */
	pLayerCfg.FBStartAdress = (uint32_t)&image_lcd;

	/* Alpha constant (255 == totally opaque) */
	pLayerCfg.Alpha = 255;

	/* Default Color configuration (configure A,R,G,B component values) : no background color */
	pLayerCfg.Alpha0 = 255; /* fully transparent */
	pLayerCfg.Backcolor.Blue = 255;
	pLayerCfg.Backcolor.Green = 255;
	pLayerCfg.Backcolor.Red = 255;

	/* Configure blending factors */
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

	/* Configure the number of lines and number of pixels per line */
	pLayerCfg.ImageWidth  = 800;
	pLayerCfg.ImageHeight = 320;
	if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
		Error_Handler();
	/* USER CODE END LTDC_Init 2 */
}

/**
 * @brief LCD Display Initialization
 * This function configures the hardware resources for the LCD Display
 * @retval None
 */
void LCDDisplay_Init(void)
{
	ConfigClock();
	ConfigIO();
	ConfigLTDC();
}

/**
 * @brief LCD Display DeInitialization
 * This function releases the LCD Display resources
 * @retval None
 */
void LCDDisplay_DeInit()
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
