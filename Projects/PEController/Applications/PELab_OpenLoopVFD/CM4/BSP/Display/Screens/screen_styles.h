/**
 ********************************************************************************
 * @file 		screen_styles.h
 * @author 		Waqas Ehsan Butt
 * @date 		Mar 14, 2023
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

#ifndef DISPLAY_SCREENS_SCREEN_STYLES_H_
#define DISPLAY_SCREENS_SCREEN_STYLES_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
#if LCD_DATA_MONITORING
#include "general_header.h"
#include "lvgl.h"
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
/**
 * Initializes the grid style according to the given basic properties
 * @param style Style to be initialized
 * @param pad Padding in pixels
 * @param border Border width in pixels
 * @param radius Radius of corners in pixels
 * @param bgColor Background color given in lv_color_t
 */
extern void BSP_Screen_InitGridStyle(lv_style_t* style, lv_coord_t pad, lv_coord_t border, lv_coord_t radius, lv_color_t bgColor);

extern void BSP_Screen_InitLabelStyle(lv_style_t* style, const lv_font_t * font, lv_text_align_t align, lv_color_t txtColor);
extern lv_color_t MakeColor(uint8_t r, uint8_t g, uint8_t b);
/********************************************************************************
 * Code
 *******************************************************************************/

#endif
#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
