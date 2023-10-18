/**
 ********************************************************************************
 * @file    	screen_styles.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 12, 2023
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
/**
 * @brief General color store for use by the application
 */
lv_color_store_t lvColorStore;
/**
 * @brief General style store for use by the application
 */
lv_style_store lvStyleStore;
/**
 * @brief Grid component, either row / column for use with in the application
 */
lv_coord_t* singleRowCol;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Make RGB color in selected format
 * @param r Red color
 * @param g Green color
 * @param b Blue color
 * @return RGB color
 */
lv_color_t MakeColor(uint8_t r, uint8_t g, uint8_t b)
{
	//LV_COLOR_MAKE
	lv_color_t bgColor;
	bgColor.ch.red = (r >> 3) & 0x1f;
	bgColor.ch.green = (g >> 2) & 0x3f;
	bgColor.ch.blue = (b >> 3) & 0x1f;
	return bgColor;
}
/**
 * @brief Create general text entry field with label and value
 * @param parent Parent lv_obj_t
 * @param nameLblStyle Label style for the name. NULL if default style
 * @param fieldStyle Label style for the field value. NULL if default style
 * @param fieldName Text representing field name
 * @param fieldValue Text representing field value
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @param row Row no in parent grid
 * @return Generated value text field
 */
lv_obj_t* lv_create_textfield_display(lv_obj_t* parent, lv_style_t* nameLblStyle, lv_style_t* fieldStyle, const char* fieldName, const char* fieldValue, lv_event_cb_t event_cb, void * eventData, uint8_t row)
{
	lv_obj_t* lblName = lv_label_create_general(parent, nameLblStyle, fieldName, NULL, NULL);
	lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);

	lv_obj_t* taField = lv_textarea_create_general(parent, nameLblStyle, fieldValue, event_cb, eventData);
	lv_obj_set_grid_cell(taField, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	return taField;
}
/**
 * @brief Create general entry field with label and a given field
 * @param parent Parent lv_obj_t
 * @param nameLblStyle Label style for the name. NULL if default style
 * @param fieldName Text representing field name
 * @param field Field value object
 * @param row Row no in parent grid
 * @return Generated value field
 */
lv_obj_t* lv_create_field(lv_obj_t* parent, lv_style_t* nameLblStyle, const char* fieldName, lv_obj_t* field, uint8_t row)
{
	lv_obj_t* lblName = lv_label_create_general(parent, nameLblStyle, fieldName, NULL, NULL);
	lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, row, 1);

	lv_obj_set_grid_cell(field, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	return field;
}
/**
 * @brief Create general button
 * @param parent Parent lv_obj_t
 * @param btnStyle Button style. NULL if default style
 * @param lblStyle Style for Label included in button. NULL if default style
 * @param txt Label text
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Generated button object
 */
lv_obj_t* lv_btn_create_general(lv_obj_t* parent, lv_style_t* btnStyle, lv_style_t* lblStyle, const char* txt, lv_event_cb_t event_cb, void * eventData)
{
	lv_obj_t* btn = lv_btn_create(parent);
	if (btnStyle != NULL)
		lv_obj_add_style(btn, btnStyle, 0);
	if (event_cb != NULL)
		lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, eventData);
	lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);

	lv_obj_t* lbl = lv_label_create_general(btn, lblStyle, txt, event_cb, eventData);
	lv_obj_center(lbl);
	return btn;
}
/**
 * @brief Create general label
 * @param parent Parent lv_obj_t
 * @param style Label style. NULL if default style
 * @param txt Label text
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Generated label object
 */
lv_obj_t* lv_label_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData)
{
	lv_obj_t* lbl = lv_label_create(parent);
	if (style != NULL)
		lv_obj_add_style(lbl, style, 0);
	if (event_cb != NULL)
		lv_obj_add_event_cb(lbl, event_cb, LV_EVENT_CLICKED, eventData);
	lv_label_set_text(lbl, txt != NULL ? txt : "");
	lv_obj_clear_flag(lbl, LV_OBJ_FLAG_CLICK_FOCUSABLE);
	if (event_cb != NULL)
		lv_obj_add_flag(lbl, LV_OBJ_FLAG_CLICKABLE);
	return lbl;
}
/**
 * @brief Create general textarea
 * @param parent Parent lv_obj_t
 * @param style Textarea style. NULL if default style
 * @param txt Label text
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Generated Textarea object
 */
lv_obj_t* lv_textarea_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData)
{
	lv_obj_t* ta = lv_textarea_create(parent);
	if (style != NULL)
		lv_obj_add_style(ta, style, 0);
	if (event_cb != NULL)
		lv_obj_add_event_cb(ta, event_cb, LV_EVENT_CLICKED, eventData);
	lv_textarea_set_align(ta, LV_TEXT_ALIGN_RIGHT);
	lv_textarea_set_text(ta, txt);
	lv_textarea_set_one_line(ta, true);
	lv_obj_set_align(lv_textarea_get_label(ta), LV_ALIGN_RIGHT_MID);
	return ta;
}
/**
 * @brief Create general grid
 * @param parent Parent lv_obj_t
 * @param cols Column information
 * @param rows Row information
 * @param style Grid style. NULL if default style
 * @param bgColor Background color. NULL if default color
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Generated grid object
 */
lv_obj_t* lv_grid_create_general(lv_obj_t* parent, lv_coord_t* cols, lv_coord_t* rows, lv_style_t* style, lv_color_t* bgColor, lv_event_cb_t event_cb, void * eventData)
{
	lv_obj_t* grid = lv_obj_create(parent);
	lv_obj_set_grid_dsc_array(grid, cols, rows);
	lv_obj_set_layout(grid, LV_LAYOUT_GRID);
	//if (bgColor != NULL)
		//lv_style_set_bg_color(style, *bgColor);
	if (style != NULL)
		lv_obj_add_style(grid, style, 0);
	if (event_cb != NULL)
		lv_obj_add_event_cb(grid, event_cb, LV_EVENT_CLICKED, eventData);
	return grid;
}
/**
 * @brief Create general container
 * @param parent Parent lv_obj_t
 * @param style Container style. NULL if default style
 * @param row Row no in parent grid
 * @param col Column no in parent grid
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Container object
 */
lv_obj_t* lv_container_create_general(lv_obj_t* parent, lv_style_t* style, int row, int col, lv_event_cb_t event_cb, void * eventData)
{
	lv_obj_t* container = lv_obj_create(parent);
	if (style != NULL)
		lv_obj_add_style(container, style, 0);
	if (event_cb != NULL)
		lv_obj_add_event_cb(container, event_cb, LV_EVENT_CLICKED, eventData);
	if (row >= 0)
		lv_obj_set_grid_cell(container, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
	return container;
}
/**
 * @brief Initializes the grid style according to the given basic properties
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
/**
 * @brief Initializes the label style
 * @param style Style to be initialized
 * @param font Text font. NULL if default font
 * @param align Text alignment
 * @param txtColor Text color. NULL if default color
 */
void BSP_Screen_InitLabelStyle(lv_style_t* style, const lv_font_t * font, lv_text_align_t align, lv_color_t* txtColor)
{
	lv_style_init(style);
	lv_style_set_pad_all(style, 0);
	if (font != NULL)
		lv_style_set_text_font(style, font);
	lv_style_set_text_align(style, align);
	if (txtColor != NULL)
		lv_style_set_text_color(style, *txtColor);
}
/**
 * @brief Initializes the button style
 * @param style Style to be initialized
 * @param border Button border width
 * @param radius Button radius width
 * @param bgColor Background color given in lv_color_t
 */
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

void BSP_Screen_InitTextAreaStyle(lv_style_t* style, const lv_font_t * font, lv_color_t* txtColor)
{
	lv_style_init(style);
	lv_style_set_pad_all(style, 100);
	if (font != NULL)
		lv_style_set_text_font(style, font);
	if (txtColor != NULL)
		lv_style_set_text_color(style, *txtColor);
}
/**
 * @brief Initializes base screen styles
 */
void BSP_Screen_Styles_Init(void)
{
	// initialize colors
	lvColorStore.background = MakeColor(10, 40, 60);
	lvColorStore.black = MakeColor(0, 0, 0);
	lvColorStore.gray = MakeColor(127, 127, 127);
	lvColorStore.white = MakeColor(255, 255, 255);
	lvColorStore.darkFont = MakeColor(0, 0, 0);
	lvColorStore.mediumFont = MakeColor(127, 127, 127);
	lvColorStore.lightFont = MakeColor(255, 255, 255);
	lvColorStore.darkTaraz = MakeColor(0, 100, 100);
	lvColorStore.mediumTaraz = MakeColor(0, 155, 155);
	lvColorStore.lightTaraz = MakeColor(0, 200, 200);
	lvColorStore.btnBg1 = MakeColor(0, 180, 180);
	lvColorStore.btnBg2 = MakeColor(180, 180, 180);
	lvColorStore.btnBg3 = MakeColor(210, 210, 210);
	lvColorStore.current = MakeColor(250, 90, 0);
	lvColorStore.voltage = MakeColor(0, 70, 160);
	lvColorStore.on = MakeColor(11, 161, 51);
	lvColorStore.off = MakeColor(207, 52, 43);
	static lv_coord_t singleCoord[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	singleRowCol = singleCoord;
	BSP_Screen_InitGridStyle(&lvStyleStore.defaultGrid, 0, 0, 0, 0, &lvColorStore.background);
	BSP_Screen_InitGridStyle(&lvStyleStore.thinMarginGrid, 2, 2, 0, 0, &lvColorStore.background);
	BSP_Screen_InitGridStyle(&lvStyleStore.mediumMarginGrid, 5, 5, 0, 0, &lvColorStore.background);
	BSP_Screen_InitGridStyle(&lvStyleStore.thickMarginGrid, 8, 8, 0, 0, &lvColorStore.background);
	BSP_Screen_InitTextAreaStyle(&lvStyleStore.defaultTextArea, NULL, &lvColorStore.darkFont);
	BSP_Screen_InitBtnStyle(&lvStyleStore.btn1, 1, 10, &lvColorStore.btnBg1);
	BSP_Screen_InitBtnStyle(&lvStyleStore.btn2, 1, 10, &lvColorStore.btnBg2);
	BSP_Screen_InitBtnStyle(&lvStyleStore.btn3, 1, 10, &lvColorStore.btnBg3);
}


/* EOF */
