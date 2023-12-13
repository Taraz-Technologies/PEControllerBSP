/**
 ********************************************************************************
 * @file    	screen_splash.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 15, 2023
 *
 * @brief   
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
#include "screen_base.h"
#include "screen_data_app.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define SPLASH_TIME_ms			(2000)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/

/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
uint32_t tickStart = 0;
/********************************************************************************
 * Code
 *******************************************************************************/
static void Load(void)
{
	tickStart = HAL_GetTick();
}

/**
 * @brief Refresh the splash screen
 * @return Type of screen if needs to switch to another screen
 */
static screen_type_t Refresh(void)
{
	if((HAL_GetTick() - tickStart) < SPLASH_TIME_ms)
		return SCREEN_NONE;
	return SCREEN_MAIN;
}

/**
 * @brief Initialize screen
 * @param _screen Pointer to fill the screen information
 */
void SplashScreen_Init(screens_t* _screen)
{
	_screen->Refresh = Refresh;
	_screen->Load = Load;
	_screen->Unload = NULL;
	if (splashImg == NULL)
		_screen->directLayer = NULL;
	else
	{
		static ltdc_layer_info_t directLayer =
		{
				.xAlign = ALIGN_CENTER_X,
				.yAlign = ALIGN_CENTER_Y,
				.posX = DISPLAY_WIDTH / 2,
				.posY = DISPLAY_HEIGHT / 2
		};
		directLayer.PixelFormat = splashImg->pixelFormat;
		directLayer.width = splashImg->width;
		directLayer.height = splashImg->height;
		directLayer.data = splashImg->data;
		_screen->directLayer = &directLayer;
	}
	_screen->lvglLayer = NULL;
}
/* EOF */
