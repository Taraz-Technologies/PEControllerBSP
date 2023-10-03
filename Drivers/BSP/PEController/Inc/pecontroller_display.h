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
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "screen_manager.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup PEDISPLAY_Exported_Typedefs Typedefs
  * @{
  */
/**
 * @brief Pixel width of the Display
 */
#define DISPLAY_WIDTH				(800)
/**
 * @brief Pixel height of the Display
 */
#define DISPLAY_HEIGHT				(480)
/**
 * @brief Width of display used by the RAM
 */
#define DISPLAY_WIDTH_RAM 			(DISPLAY_WIDTH)
/**
 * @brief Height of display used by the RAM
 */
#define DISPLAY_HEIGHT_RAM			(DISPLAY_HEIGHT)
/**
 * @brief Set the display buffer size to 1/10th the size of the screen
 */
#define LVGL_BUFF_SIZE				((DISPLAY_WIDTH_RAM * DISPLAY_HEIGHT_RAM * 10) / 100)
/**
 * @brief LTDC layer used by LVGL
 */
#define LVGL_LAYER					(0)
/**
 * @brief LTDC layer used by direct display
 */
#define DIRECT_LAYER				(1)
/**
 * @brief Display format used by the lvgl layer in LTDC
 */
#define RAM_PIXEL_FORMAT			(LTDC_PIXEL_FORMAT_L8)
/**
  * @}
  */
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup PEDISPLAY_Exported_Variables Variables
  * @{
  */
/**
 * @brief Frame buffer used for storage of a display frma in RAM.
 */
extern uint8_t frame_buff[DISPLAY_HEIGHT_RAM][DISPLAY_WIDTH_RAM];
/**
 * @}
 */
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
