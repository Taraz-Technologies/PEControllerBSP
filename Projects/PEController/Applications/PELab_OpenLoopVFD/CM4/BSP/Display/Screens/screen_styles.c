/**
 ********************************************************************************
 * @file    	screen_styles.c
 * @author 		Waqas Ehsan Butt
 * @date    	Mar 14, 2023
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
#include "user_config.h"
#if LCD_DATA_MONITORING
#include "screen_styles.h"
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
 * Initializes the grid style according to the given basic properties
 * @param style Style to be initialized
 * @param pad Padding in pixels
 * @param border Border width in pixels
 * @param radius Radius of corners in pixels
 * @param bgColor Background color given in lv_color_t
 */
void BSP_Screen_InitGridStyle(lv_style_t* style, lv_coord_t pad, lv_coord_t border, lv_coord_t radius, lv_color_t bgColor)
{
	lv_style_init(style);
	lv_style_set_radius(style, radius);
	lv_style_set_pad_row(style, pad);
	lv_style_set_pad_column(style, pad);
	lv_style_set_pad_all(style, pad);
	lv_style_set_border_width(style, border);
	if (bgColor.full != 0)
		lv_style_set_bg_color(style, bgColor);
}

void BSP_Screen_InitLabelStyle(lv_style_t* style, const lv_font_t * font, lv_text_align_t align, lv_color_t txtColor)
{
	lv_style_init(style);
	lv_style_set_pad_all(style, 0);
	lv_style_set_text_font(style, font);
	lv_style_set_text_align(style, align);
	lv_style_set_text_color(style, txtColor);
}

#endif
/* EOF */
