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
lv_color_t MakeColor(uint8_t r, uint8_t g, uint8_t b)
{
	//LV_COLOR_MAKE
	lv_color_t bgColor;
	bgColor.ch.red = (r >> 3) & 0x1f;
	bgColor.ch.green = (g >> 2) & 0x3f;
	bgColor.ch.blue = (b >> 3) & 0x1f;
	return bgColor;
}

lv_obj_t* lv_btn_create_general(lv_obj_t* parent, lv_style_t* btnStyle, lv_style_t* lblStyle, const char* txt, lv_event_cb_t event_cb, void * eventData)
{
	lv_obj_t* btn = lv_btn_create(parent);
	if (btnStyle != NULL)
		lv_obj_add_style(btn, btnStyle, 0);
	if (event_cb != NULL)
		lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, eventData);

	lv_obj_t* lbl = lv_label_create_general(btn, lblStyle, txt, event_cb, eventData);
	lv_obj_center(lbl);
	return btn;
}

lv_obj_t* lv_label_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData)
{
	lv_obj_t* lbl = lv_label_create(parent);
	if (style != NULL)
		lv_obj_add_style(lbl, style, 0);
	if (event_cb != NULL)
		lv_obj_add_event_cb(lbl, event_cb, LV_EVENT_CLICKED, eventData);
	lv_label_set_text(lbl, txt != NULL ? txt : "");
	return lbl;
}

lv_obj_t* lv_grid_create_general(lv_obj_t* parent, lv_coord_t* cols, lv_coord_t* rows, lv_style_t* style, lv_color_t* bgColor, lv_event_cb_t event_cb, void * eventData)
{
	lv_obj_t* grid = lv_obj_create(parent);
	lv_obj_set_grid_dsc_array(grid, cols, rows);
	lv_obj_set_layout(grid, LV_LAYOUT_GRID);
	//if (bgColor != NULL)
	//	lv_style_set_bg_color(style, *bgColor);
	if (style != NULL)
		lv_obj_add_style(grid, style, 0);
	if (event_cb != NULL)
		lv_obj_add_event_cb(grid, event_cb, LV_EVENT_CLICKED, eventData);
	return grid;
}


void BSP_Style_GetDefault(lv_style_t* style)
{
	lv_style_init(style);
}

void BSP_Style_SetSketch(lv_style_t* style, lv_coord_t pad, lv_coord_t border, lv_coord_t radius)
{
	lv_style_set_radius(style, radius);
	lv_style_set_pad_row(style, pad);
	lv_style_set_pad_column(style, pad);
	lv_style_set_pad_all(style, pad);
	lv_style_set_border_width(style, border);
}
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
