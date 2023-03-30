/**
 ********************************************************************************
 * @file    	message_screen.c
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
#include "message_screen.h"
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
static lv_obj_t* screen;
static lv_color_t lvColorHeadingBg = LV_COLOR_MAKE(0, 155, 155);
static lv_style_t headingGridStyle;
static lv_style_t msgLblStyle, headingLblStyle, btnLblStyle;
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
void MessageScreen_Init(void)
{
	BSP_Screen_InitGridStyle(&headingGridStyle, 0, 0, 0, 10, &lvColorHeadingBg);
	// Initialize the basic grid cell label styles
	BSP_Screen_InitLabelStyle(&headingLblStyle, &lv_font_montserrat_40, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
	BSP_Screen_InitLabelStyle(&msgLblStyle, &lv_font_montserrat_30, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
	lv_style_set_pad_all(&msgLblStyle, 10);
	lv_style_set_text_line_space(&msgLblStyle, 10);
	BSP_Screen_InitLabelStyle(&btnLblStyle, &lv_font_montserrat_26, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
}

void MessageScreen_Load(char* heading, char* msg, bool hasOk, bool hasCancel)
{
	// create the screen
	screen = lv_obj_create(NULL);
	// create basic grid
	lv_obj_t* screenGrid = lv_obj_create(screen);
	static lv_coord_t colsScreen[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsScreen[] = {100, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_size(screenGrid, 800, 480);
	lv_obj_set_grid_dsc_array(screenGrid, colsScreen, rowsScreen);
	lv_obj_set_layout(screenGrid, LV_LAYOUT_GRID);
	lv_obj_add_style(screenGrid, &lvStyleStore.defaultGrid, 0);

	// create monitoring grid
	lv_obj_t* headingGrid = lv_obj_create(screenGrid);
	static lv_coord_t rowsHeading[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_grid_cell(headingGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_grid_dsc_array(headingGrid, colsScreen, rowsHeading);
	lv_obj_set_layout(headingGrid, LV_LAYOUT_GRID);
	lv_obj_add_style(headingGrid, &headingGridStyle, 0);

	//
	lv_obj_t * headingLbl = lv_label_create(headingGrid);
	lv_obj_set_grid_cell(headingLbl, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(headingLbl);
	lv_obj_add_style(headingLbl, &headingLblStyle, 0);
	lv_label_set_text(headingLbl, heading);

	// Message Grid
	lv_obj_t* msgGrid = lv_obj_create(screenGrid);
	static lv_coord_t colsMsgs[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsMsgs[] = {LV_GRID_FR(1), 120, LV_GRID_TEMPLATE_LAST};
	lv_obj_set_grid_cell(msgGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
	lv_obj_set_grid_dsc_array(msgGrid, colsMsgs, rowsMsgs);
	lv_obj_set_layout(msgGrid, LV_LAYOUT_GRID);
	lv_obj_add_style(msgGrid, &lvStyleStore.defaultGrid, 0);

	//
	/*
	lv_obj_t * captionLbl = lv_label_create(msgGrid);
	lv_obj_set_grid_cell(captionLbl, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(captionLbl);
	lv_obj_add_style(captionLbl, &captionLblStyle, 0);
	lv_label_set_text(captionLbl, caption);
	*/

	lv_obj_t * msgLbl = lv_label_create(msgGrid);
	lv_obj_set_grid_cell(msgLbl, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_START, 0, 1);
	lv_obj_center(msgLbl);
	lv_obj_add_style(msgLbl, &msgLblStyle, 0);
	lv_label_set_long_mode(msgLbl, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(msgLbl, 780);
	lv_label_set_text(msgLbl, msg);

	if (hasOk)
	{
		lv_obj_t * okBtn = lv_btn_create_general(msgGrid, NULL, &btnLblStyle, "OK", NULL, NULL);
		lv_obj_set_grid_cell(okBtn, LV_GRID_ALIGN_CENTER, 0, hasCancel ? 1 : 2, LV_GRID_ALIGN_CENTER, 1, 1);
		lv_obj_set_size(okBtn, 240, 70);
		lv_obj_center(okBtn);
	}

	if (hasCancel)
	{
		lv_obj_t * cancelBtn = lv_btn_create_general(msgGrid, NULL, &btnLblStyle, "Cancel", NULL, NULL);
		lv_obj_set_grid_cell(cancelBtn, LV_GRID_ALIGN_CENTER, hasOk ? 1 : 0, hasOk ? 1 : 2, LV_GRID_ALIGN_CENTER, 1, 1);
		lv_obj_set_size(cancelBtn, 240, 70);
		lv_obj_center(cancelBtn);
	}

	lv_scr_load(screen);
}

void MessageScreen_Unload(void)
{
	lv_obj_del(screen);
}

#endif
/* EOF */
