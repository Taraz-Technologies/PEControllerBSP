/**
 ********************************************************************************
 * @file 		pecontroller_display.h
 * @author 		Waqas Ehsan Butt
 * @date 		November 26, 2021
 *
 * @brief    Header file for the PEController display module
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

#ifndef PEONTROLLER_DISPLAY_H
#define PEONTROLLER_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @defgroup Display Display
 * @brief This module defines the display
 * @{
 */

/** @defgroup DisplayDriver Driver
 * @brief Contains the declaration and procedures for the display driver
 * @details List of functions
 * 	-# <b>@ref BSP_Display_Init()</b> Initializes the display module
 * 	-# <b>@ref BSP_Display_DeInit()</b> De-initializes the display module
 * 	-# <b>@ref BSP_Display_ShowLayer()</b> Shows the configured layer on the LCD
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/**
 * @brief Pixel width of the Display
 */
#define DISPLAY_WIDTH				(800)
/**
 * @brief Pixel height of the Display
 */
#define DISPLAY_HEIGHT				(480)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/**
 * @brief X-Axis alignment for the image
 */
typedef enum
{
	ALIGN_LEFT_X, /**< ALIGN_LEFT_X */
	ALIGN_RIGHT_X,/**< ALIGN_RIGHT_X */
	ALIGN_CENTER_X/**< ALIGN_CENTER_X */
} image_align_x_t;
/**
 * @brief Y-Axis alignment for the image
 */
typedef enum
{
	ALIGN_UP_Y,    /**< ALIGN_UP_Y */
	ALIGN_DOWN_Y,  /**< ALIGN_DOWN_Y */
	ALIGN_CENTER_Y /**< ALIGN_CENTER_Y */
} image_align_y_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
/**
 * @brief Defines the configuration structure of images to be displayed
 */
typedef struct
{
	image_align_x_t xAlign;				 /**< @brief X-Axis alignment of image on display */
	image_align_y_t yAlign;				 /**< @brief Y-Axis alignment of image on display */
	uint32_t ImageWidth;                 /*!< Configures the color frame buffer line length.
	                                            This parameter must be a number between 1 and @ref DISPLAY_WIDTH */
	uint32_t ImageHeight;                /*!< Configures the color frame buffer line length.
	                                            This parameter must be a number between 1 and @ref DISPLAY_HEIGHT */
	uint32_t PixelFormat;                /*!< Specifies the pixel format.
                                            This parameter can be one of value of @ref LTDC_Pixelformat */
} image_config_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup PEDISPLAY_Exported_Functions Functions
  * @{
  */
/**
 * @brief Initializes the display module
 */
extern void BSP_Display_Init(void);
/**
 * @brief De-initializes the display module
 */
extern void BSP_Display_DeInit(void);
/**
 * @brief Displays the first layer after applying configuration
 * @param framePtr Pointer to the frame to be displayed
 * @param config Pointer to the configurations for the layer
 */
extern void BSP_Display_ShowFirstLayer(const uint8_t* framePtr, image_config_t* config);
/**
 * @brief Display the selected LOGO. Use the user_config.h file to select appropriate LOGO
 */
extern void BSP_Display_ShowLogo(void);
/********************************************************************************
 * Code
 *******************************************************************************/



/**
 * @}
 */
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
