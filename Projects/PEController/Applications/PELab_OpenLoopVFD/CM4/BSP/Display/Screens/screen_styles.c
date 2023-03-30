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
static lv_color_t basicBtnColor;
static lv_color_t basicBtn2Color;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
lv_color_store_t lvColorStore;
lv_style_store lvStyleStore;
//lv_color_t lvColorBg;
//lv_color_t lvColorBlack;
//lv_color_t lvColorWhite;
//lv_color_t lvColorGray;
//lv_color_t lvColorDarkFont;
//lv_color_t lvColorMixFont;
//lv_color_t lvColorLightFont;
//lv_color_t lvColorTaraz;
//lv_style_t basicGridStyle;
//lv_style_t marginedGridStyle;
//lv_style_t basicBtnStyle;
//lv_style_t basicBtn2Style;
lv_coord_t* singleRowCol;
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

lv_obj_t* lv_create_textfield_display(lv_obj_t* parent, lv_style_t* nameLblStyle, lv_style_t* fieldStyle, const char* fieldName, const char* fieldValue, lv_event_cb_t event_cb, void * eventData, uint8_t row)
{
	lv_obj_t* lblName = lv_label_create_general(parent, nameLblStyle, fieldName, NULL, NULL);
	lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);

	lv_obj_t* taField = lv_textarea_create_general(parent, nameLblStyle, fieldValue, event_cb, eventData);
	lv_obj_set_grid_cell(taField, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	return taField;
}

lv_obj_t* lv_create_field(lv_obj_t* parent, lv_style_t* nameLblStyle, const char* fieldName, lv_obj_t* field, uint8_t row)
{
	lv_obj_t* lblName = lv_label_create_general(parent, nameLblStyle, fieldName, NULL, NULL);
	lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);

	lv_obj_set_grid_cell(field, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	return field;
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

lv_obj_t* lv_textarea_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData)
{
	lv_obj_t* ta = lv_textarea_create(parent);
	if (style != NULL)
		lv_obj_add_style(ta, style, 0);
	if (event_cb != NULL)
		lv_obj_add_event_cb(ta, event_cb, LV_EVENT_CLICKED, eventData);
	lv_textarea_set_one_line(ta, true);
	lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 0, 0);
	lv_textarea_add_text(ta, txt);
	return ta;
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


/**
 * Initializes the grid style according to the given basic properties
 * @param style Style to be initialized
 * @param pad Padding in pixels
 * @param margin Margin in rows for childs in pixels
 * @param border Border width in pixels
 * @param radius Radius of corners in pixels
 * @param bgColor Background color given in lv_color_t
 */
void BSP_Screen_InitGridStyle(lv_style_t* style, lv_coord_t pad, lv_coord_t margin, lv_coord_t border, lv_coord_t radius, lv_color_t* bgColor)
{
	lv_style_init(style);
	lv_style_set_radius(style, radius);
	lv_style_set_pad_all(style, pad);
	lv_style_set_pad_row(style, margin);
	lv_style_set_pad_column(style, margin);
	lv_style_set_border_width(style, border);
	if (bgColor != NULL)
		lv_style_set_bg_color(style, *bgColor);
}

void BSP_Style_SetSketch(lv_style_t* style, lv_coord_t pad, lv_coord_t border, lv_coord_t radius)
{
	lv_style_set_radius(style, radius);
	lv_style_set_pad_row(style, pad);
	lv_style_set_pad_column(style, pad);
	lv_style_set_pad_all(style, pad);
	lv_style_set_border_width(style, border);
}

void BSP_Screen_InitLabelStyle(lv_style_t* style, const lv_font_t * font, lv_text_align_t align, lv_color_t* txtColor)
{
	lv_style_init(style);
	lv_style_set_pad_all(style, 0);
	lv_style_set_text_font(style, font);
	lv_style_set_text_align(style, align);
	if (txtColor != NULL)
		lv_style_set_text_color(style, *txtColor);
}

void BSP_Screen_InitBtnStyle(lv_style_t* style, lv_coord_t border, lv_coord_t radius, lv_color_t* bgColor)
{
	lv_style_init(style);
	lv_style_set_radius(style, radius);
	lv_style_set_bg_opa(style, LV_OPA_100);
	if (bgColor != NULL)
		lv_style_set_bg_color(style, *bgColor);
	lv_style_set_border_opa(style, LV_OPA_40);
	lv_style_set_border_width(style, border);
	lv_style_set_shadow_width(style, 0);
	lv_style_set_border_color(style, lv_palette_main(LV_PALETTE_GREY));
	lv_style_set_outline_opa(style, LV_OPA_COVER);
	if (bgColor != NULL)
		lv_style_set_outline_color(style, *bgColor);
	lv_style_set_text_color(style, lv_color_black());
	lv_style_set_pad_all(style, 0);
}

void BSP_Styles_Init(void)
{
	// initialize colors
	lvColorStore.background = MakeColor(210, 210, 210);
	lvColorStore.black = MakeColor(0, 0, 0);
	lvColorStore.white = MakeColor(127, 127, 127);
	lvColorStore.gray = MakeColor(255, 255, 255);
	lvColorStore.darkFont = MakeColor(0, 0, 0);
	lvColorStore.mediumFont = MakeColor(127, 127, 127);
	lvColorStore.lightFont = MakeColor(255, 255, 255);
	lvColorStore.darkTaraz = MakeColor(0, 75, 75);
	lvColorStore.mediumTaraz = MakeColor(0, 155, 155);
	lvColorStore.lightTaraz = MakeColor(0, 200, 200);
	basicBtnColor = MakeColor(0, 180, 180);
	basicBtn2Color = MakeColor(30, 150, 120);
	static lv_coord_t singleCoord[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	singleRowCol = singleCoord;
	BSP_Screen_InitGridStyle(&lvStyleStore.defaultGrid, 0, 0, 0, 0, &lvColorStore.background);
	BSP_Screen_InitGridStyle(&lvStyleStore.thinMarginGrid, 2, 2, 0, 0, &lvColorStore.background);
	BSP_Screen_InitGridStyle(&lvStyleStore.thickMarginGrid, 8, 8, 0, 0, &lvColorStore.background);
	BSP_Screen_InitBtnStyle(&lvStyleStore.defaultBtn, 1, 10, &basicBtnColor);
	BSP_Screen_InitBtnStyle(&lvStyleStore.auxBtn, 1, 10, &basicBtn2Color);
}


#endif
/* EOF */
