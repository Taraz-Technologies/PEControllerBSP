/**
 ********************************************************************************
 * @file    	config_screen.c
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
#if 1
#include "config_screen.h"
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
static lv_style_t screenGridStyle, numpadGridStyle;
static lv_style_t numpadLblStyle;
static lv_obj_t* labelx;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
static char* numTxts[] =
{
		"7", "8", "9", "CE",
		"4", "5", "6", "C",
		"1", "2", "3", "X",
		"-", "0", ".", "OK",
};
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
extern uint16_t YDisp;
/********************************************************************************
 * Code
 *******************************************************************************/
static void event_handler(lv_event_t * e)
{
	uint8_t index = (uint8_t)e->user_data;
	if (index % 4 != 3)
		lv_label_set_text(labelx, numTxts[index]);
}

void ConfigScreen_Init(void)
{
	lv_color_t bgColor = MakeColor(155, 155, 155);
	BSP_Screen_InitGridStyle(&screenGridStyle, 0, 0, 0, bgColor);

	// Initialize monitoring grid
	bgColor = MakeColor(155, 155, 155);
	BSP_Screen_InitGridStyle(&numpadGridStyle, 2, 0, 0, bgColor);

	// Initialize the basic grid cell label styles
	bgColor = MakeColor(0, 0, 0);
	BSP_Screen_InitLabelStyle(&numpadLblStyle, &lv_font_montserrat_40, LV_TEXT_ALIGN_CENTER, bgColor);
}

void ConfigScreen_Load(void)
{
	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	lv_obj_t* screenGrid = lv_obj_create(screen);
	static lv_coord_t colsScreen[] = {LV_GRID_FR(1), 400, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsScreen[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_size(screenGrid, 800, 480);
	lv_obj_set_grid_dsc_array(screenGrid, colsScreen, rowsScreen);
	lv_obj_set_layout(screenGrid, LV_LAYOUT_GRID);
	lv_obj_add_style(screenGrid, &screenGridStyle, 0);

	// create buttons grid
	lv_obj_t* numpadGrid = lv_obj_create(screenGrid);
	static lv_coord_t colsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_grid_cell(numpadGrid, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_grid_dsc_array(numpadGrid, colsMon, rowsMon);
	lv_obj_set_layout(numpadGrid, LV_LAYOUT_GRID);
	lv_obj_add_style(numpadGrid, &numpadGridStyle, 0);
	// create all cells
	for (int i = 0; i < 16; i++)
	{
		lv_obj_t* btn = lv_btn_create(numpadGrid);
		int col = i % 4;
		int row = i / 4;
		lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
		//lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
		lv_obj_t* label = lv_label_create(btn);
		lv_obj_add_event_cb(btn, event_handler, LV_EVENT_RELEASED, (void*)i);
		lv_label_set_text(label, numTxts[i]);
		lv_obj_center(label);
		lv_obj_add_style(label, &numpadLblStyle, 0);
	}


	labelx = lv_label_create(screenGrid);
	lv_obj_set_grid_cell(labelx, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_label_set_text(labelx, "");
	lv_obj_center(labelx);
	lv_obj_add_style(labelx, &numpadLblStyle, 0);

	// create control grid --TODO--
	lv_scr_load(screen);
}

void ConfigScreen_Unload(void)
{
	lv_obj_del(screen);
}

#endif
/* EOF */
