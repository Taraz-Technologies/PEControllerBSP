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
#include "UtilityLib.h"
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
static lv_style_t numpadGridStyle;
static lv_style_t numpadLblStyle;
static lv_obj_t* labelx;
static bool isActive;
extern volatile int screenID;
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
	if (!isActive)
		return;
	uint8_t index = (uint8_t)e->user_data;
	if ((index % 4) != 3)
		lv_label_set_text(labelx, numTxts[index]);
	screenID = 0;
}

void ConfigScreen_Init(void)
{
	// Initialize the basic grid cell label styles
	BSP_Screen_InitLabelStyle(&numpadLblStyle, &lv_font_montserrat_40, LV_TEXT_ALIGN_CENTER, &lvColorDarkFont);
	BSP_Screen_InitGridStyle(&numpadGridStyle, 2, 0, 0, 0, &lvColorBg);

	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	static lv_coord_t colsScreen[] = {LV_GRID_FR(1), 400, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsScreen[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* screenGrid = lv_grid_create_general(screen, colsScreen, rowsScreen, &basicGridStyle, NULL, NULL, NULL);
	lv_obj_set_size(screenGrid, 800, 480);

	// create buttons grid
	static lv_coord_t colsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* numpadGrid = lv_grid_create_general(screenGrid, colsMon, rowsMon, &basicGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(numpadGrid, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	// create all cells
	static lv_coord_t colsBtn[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsBtn[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	for (int i = 0; i < 16; i++)
	{
		int col = i % 4;
		int row = i / 4;
		lv_obj_t* btnGrid = lv_grid_create_general(numpadGrid, colsBtn, rowsBtn, &numpadGridStyle, NULL, NULL, NULL);
		lv_obj_set_grid_cell(btnGrid, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
		lv_obj_t* btn = lv_btn_create_general(btnGrid, NULL, &numpadLblStyle, numTxts[i], event_handler, (void*)i);
		lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
		//lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
	}

	labelx = lv_label_create_general(screenGrid, &numpadLblStyle, "", NULL, NULL);
	lv_obj_set_grid_cell(labelx, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
}

void ConfigScreen_Load(void)
{
	//if (isActive)
	//	return;


	// create control grid --TODO--
	lv_scr_load(screen);
	isActive = true;
}

void ConfigScreen_Unload(void)
{
	if (isActive)
	{
		//lv_obj_del(screen);
		isActive = false;
	}
}

#endif
/* EOF */
