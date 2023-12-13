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
#include "clut_data.h"
#include "lvgl.h"
#include "pecontroller_ts.h"
#include "screen_data.h"
#include "screen_manager.h"
#include "shared_memory.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/**
 * @brief Because the LCD is installed in reverse direction, 180 degrees rotation is required
 */
#define LV_ROTATION			(LV_DISP_ROT_180)
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
/**
 * @brief This layer info can be assigned to any layer when the relevant LTDC layer is needed to be kept empty
 */
static ltdc_layer_info_t nullLayerInfo =
{
		.data = (const uint8_t*)frame_buff,
		.posX = 0,
		.posY = 0,
		.width = 1,
		.height = 1,
		.PixelFormat = RAM_PIXEL_FORMAT,
		.xAlign = ALIGN_LEFT_X,
		.yAlign = ALIGN_UP_Y,
};
/********************************************************************************
 * Global Variables
 *******************************************************************************/
/**
 * @brief Frame buffer stored in ram used for drawing the graphics on screen by the LTDC module
 */
uint8_t frame_buff[DISPLAY_HEIGHT_RAM][DISPLAY_WIDTH_RAM] __attribute__((section(".FrameBuffer")));
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
static void ReadTouchPad(struct _lv_indev_drv_t * indev, lv_indev_data_t * data)
{
	TS_StateTypeDef* state = BSP_TS_GetState();
	if(state->touchDetected) {
		data->state = LV_INDEV_STATE_PRESSED;
		data->point.x = state->touchX;
		data->point.y = state->touchY;
	} else {
		data->state = LV_INDEV_STATE_RELEASED;
	}
}

static void FlushLVGLScreen(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
	int32_t x, y;
	for(y = area->y1; y <= area->y2; y++) {
		for(x = area->x1; x <= area->x2; x++) {
			frame_buff[y][x] = color_map[((uint16_t)(color_p->full))];
			color_p++;
		}
	}
	writeAtScreenEnd = (area->x2 > DISPLAY_WIDTH - 30);
	lv_disp_flush_ready(disp); /* Indicate you are ready with the flushing*/
}

/**
 * @brief Configure relevant clocks used by the drivers
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
 * @brief Configure the IOs used by the drivers
 * @retval None
 */
static void ConfigIO(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

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
 * @brief
 */
static void ConfigLVGL(void)
{
	lv_init();
	static lv_disp_draw_buf_t disp_buf;
	static lv_color_t lv_buff[LVGL_BUFF_SIZE];
	lv_disp_draw_buf_init(&disp_buf, lv_buff, NULL, LVGL_BUFF_SIZE);

	static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv); /*Basic initialization*/
	disp_drv.flush_cb = FlushLVGLScreen; /*Set your driver function*/
	disp_drv.draw_buf = &disp_buf; /*Assign the buffer to the display*/
	disp_drv.hor_res = DISPLAY_WIDTH; /*Set the horizontal resolution of the display*/
	disp_drv.ver_res = DISPLAY_HEIGHT; /*Set the vertical resolution of the display*/
	disp_drv.rotated = LV_ROTATION;
	disp_drv.sw_rotate = 1;
	lv_disp_t* disp = lv_disp_drv_register(&disp_drv); /*Finally register the driver*/
	disp->theme = lv_theme_taraz_init(disp, lv_color_make(0, 100, 100), lv_color_make(0, 200, 200), true, LV_FONT_DEFAULT);

	static lv_indev_drv_t indev_drv; /*Descriptor of a input device driver*/
	lv_indev_drv_init(&indev_drv); /*Basic initialization*/
	indev_drv.type = LV_INDEV_TYPE_POINTER; /*Touch pad is a pointer-like device*/
	indev_drv.read_cb = ReadTouchPad; /*Set your driver function*/
	lv_indev_drv_register(&indev_drv);
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
	hltdc.Init.Backcolor.Blue = 20;
	hltdc.Init.Backcolor.Green = 20;
	hltdc.Init.Backcolor.Red = 20;
	if (HAL_LTDC_Init(&hltdc) != HAL_OK)
		Error_Handler();
	HAL_LTDC_ConfigCLUT(&hltdc, (uint32_t *)clut_data, 256, LVGL_LAYER);
	HAL_LTDC_EnableCLUT(&hltdc, LVGL_LAYER);
}

static void LayerDisplay(ltdc_layer_info_t* layerInfo, int layerIdx)
{
	LTDC_LayerCfgTypeDef pLayerCfg;
	if (layerInfo == NULL)
		layerInfo = &nullLayerInfo;

	// remodeling of positions for inverted images
	uint16_t posX = layerInfo->posX;
	uint16_t posY = layerInfo->posY;
#if (LV_ROTATION == LV_DISP_ROT_180)
	switch (layerInfo->xAlign)
	{
	case ALIGN_LEFT_X:
		posX = DISPLAY_WIDTH - layerInfo->posX - layerInfo->width;
		break;
	case ALIGN_CENTER_X:
		posX = DISPLAY_WIDTH - layerInfo->posX - layerInfo->width / 2;
		break;
	default:
		break;
	}

	switch (layerInfo->yAlign)
	{
	case ALIGN_UP_Y:
		posY = DISPLAY_HEIGHT - layerInfo->posY - layerInfo->height;
		break;
	case ALIGN_CENTER_Y:
		posY = DISPLAY_HEIGHT - layerInfo->posY - layerInfo->height / 2;
		break;
	default:
		break;
	}
#endif

	pLayerCfg.WindowX0 = posX;
	pLayerCfg.WindowX1 = posX + layerInfo->width;
	pLayerCfg.WindowY0 = posY;
	pLayerCfg.WindowY1 = posY + layerInfo->height;

	pLayerCfg.PixelFormat = layerInfo->PixelFormat;
	pLayerCfg.FBStartAdress = (uint32_t)layerInfo->data;
	pLayerCfg.Alpha = 255;
	pLayerCfg.Alpha0 = 0; /* fully transparent */
	pLayerCfg.Backcolor.Blue = 0;
	pLayerCfg.Backcolor.Green = 0;
	pLayerCfg.Backcolor.Red = 0;

	/* Configure blending factors */
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;;
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;;

	/* Configure the number of lines and number of pixels per line */
	pLayerCfg.ImageWidth  = layerInfo->width;
	pLayerCfg.ImageHeight = layerInfo->height;
	if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, layerIdx) != HAL_OK)
		Error_Handler();
}

/**
 * @brief Initializes the display module
 */
void BSP_Display_Init(void)
{
	ConfigClock();
	ConfigIO();
	ConfigLVGL();
	ConfigLTDC();
	ScreenManager_Init(LayerDisplay, (adc_info_t*)&ADC_INFO);
}

/**
 * @brief De-initializes the display module
 */
static void BSP_Display_DeInit(void)
{
	// Disable clocks
	__HAL_RCC_LTDC_CLK_DISABLE();

	// Disable IOs
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

