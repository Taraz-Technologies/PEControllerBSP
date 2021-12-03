/**
 ********************************************************************************
 * @file 		logo_display_template.h
 * @author 		Waqas Ehsan Butt
 * @date 		December 3, 2021
 *
 * @brief	Header file for the image display
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

#ifndef LOGO_DISPLAY_H
#define LOGO_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup Display
 * @{
 */

/** @defgroup DisplayDriverLogo Logo
 * @brief This module contains the base functionality and definitions for displaying static image as logo
 * @details Use @ref DisplayDefaultImage() to display the default image.
 * This function can be modified to display custom images as per requirement
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "pecontroller_display.h"
#include "taraz_logo.h"
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
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Displays the default image. Call this function after calling @ref BSP_Display_Init()
 */
static void DisplayDefaultImage(void)
{
	LTDC_LayerCfgTypeDef pLayerCfg = {0};
	pLayerCfg.WindowX0 = 0;
	pLayerCfg.WindowX1 = 800;
	pLayerCfg.WindowY0 = 0;
	pLayerCfg.WindowY1 = 320;

	/* Pixel Format configuration*/
	pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;

	/* Start Address configuration : frame buffer is located at FLASH memory */
	pLayerCfg.FBStartAdress = (uint32_t)&taraz_logo;

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
	BSP_Display_ShowLayer(&pLayerCfg);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */

#endif
/* EOF */
