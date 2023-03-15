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
#if LCD_DATA_MONITORING
#include "config_screen.h"
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
/********************************************************************************
 * Global Variables
 *******************************************************************************/
static char* numTxts[] =
{
		"7", "8", "9",
		"4", "5", "6",
		"1", "2", "3",
		"-", "0", ".",
};
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/


void ConfigScreen_Init(void)
{

}

void ConfigScreen_Load(char* caption, char* heading, char* msg, bool okBtn, bool cancelBtn)
{
	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	lv_obj_t* screenGrid = lv_obj_create(screen);
	static lv_coord_t colsScreen[] = {LV_GRID_FR(SETTING_WIDTH), LV_GRID_FR(NUMPAD_WIDTH), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsScreen[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_size(screenGrid, 800, 480);
	lv_obj_set_grid_dsc_array(screenGrid, colsScreen, rowsScreen);
	lv_obj_set_layout(screenGrid, LV_LAYOUT_GRID);
	lv_obj_add_style(screenGrid, &screenGridStyle, 0);

	// create buttons grid
	lv_obj_t* numpadGrid = lv_obj_create(screenGrid);
	static lv_coord_t colsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_grid_cell(numpadGrid, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_grid_dsc_array(numpadGrid, colsMon, rowsMon);
	lv_obj_set_layout(numpadGrid, LV_LAYOUT_GRID);
	lv_obj_add_style(numpadGrid, &monGridStyle, 0);
	// create all cells
	for (int i = 0; i < 12; i++)
	{
		lv_obj_t* btn = lv_obj_create(numpadGrid);
		int col = i % 3;
		int row = i / 3;
		lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
		//lv_obj_add_event_cb(btn, event_handler, LV_EVENT_ALL, NULL);
		//lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
		label = lv_label_create(btn);
		lv_label_set_text(label, numTxts[i]);
		lv_obj_center(label);
	}


	// create control grid --TODO--
	lv_scr_load(screen);
}

void ConfigScreen_Unload(void)
{
	lv_obj_del(screen);
}

#endif
/* EOF */