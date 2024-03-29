/**
 ********************************************************************************
 * @file    	screen_appinfo.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 16, 2023
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
#include "screen_base.h"
#include "screen_data_app.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/**
 * @brief Controls the width of the Area that displays the QR Code for the application
 */
#define ROW_WIDTH_QR				(250)
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
static bool isActive;
static volatile uint8_t tag = TAG_NONE;
static lv_obj_t* screenGrid = NULL;
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
static void Close_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
	tag = TAG_CANCEL;
}

static void Close_Create(lv_obj_t * parent, int row, int col)
{
	static const char* map[] = {LV_SYMBOL_OK, NULL};

	lv_obj_t* kb = lv_keyboard_create(parent);
	lv_obj_set_grid_cell(kb, LV_GRID_ALIGN_CENTER, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
	lv_obj_set_style_text_font(kb, &lv_font_montserrat_30, 0);
	lv_btnmatrix_set_map(kb, map);
	lv_obj_set_width(kb, 200);
	lv_obj_add_event_cb(kb, Close_Clicked, LV_EVENT_CLICKED, NULL);
}

static void CreateInfo(lv_obj_t * parent)
{
	static lv_style_t lblStyleType;
	static lv_style_t lblStyleValue;
	static lv_style_t btnLblStyle;
	static bool init = false;
	if (!init)
	{
		BSP_Screen_InitLabelStyle(&lblStyleType, &lv_font_montserrat_22, LV_TEXT_ALIGN_LEFT, &themeColors.btn);
		lv_style_set_text_decor(&lblStyleType, LV_TEXT_DECOR_UNDERLINE);
		BSP_Screen_InitLabelStyle(&lblStyleValue, &lv_font_montserrat_18, LV_TEXT_ALIGN_LEFT, NULL);
		BSP_Screen_InitLabelStyle(&btnLblStyle, &lv_font_montserrat_26, LV_TEXT_ALIGN_CENTER, NULL);
		init = true;
	}

	// main grid
	static lv_coord_t rows[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_FR(1), 80 , LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.thickMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

	int rowNo = 0;
	if (appInfoDisplay.appInfo)
	{
		lv_obj_t* lblName = lv_label_create_general(grid, &lblStyleType, "Application Information:", NULL, NULL);
		lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, rowNo++, 1);
		lv_obj_t* lblDesc = lv_label_create_general(grid, &lblStyleValue, appInfoDisplay.appInfo, NULL, NULL);
		lv_obj_set_grid_cell(lblDesc, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, rowNo++, 1);
		lv_label_set_long_mode(lblDesc, LV_LABEL_LONG_WRAP);
		lv_obj_set_width(lblDesc, DISPLAY_WIDTH_RAM - ROW_WIDTH_QR - 20);
	}
	if (appInfoDisplay.connectionInfo)
	{
		lv_obj_t* lblName = lv_label_create_general(grid, &lblStyleType, "Connection Information:", NULL, NULL);
		lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, rowNo++, 1);
		lv_obj_t* lblDesc = lv_label_create_general(grid, &lblStyleValue, appInfoDisplay.connectionInfo, NULL, NULL);
		lv_obj_set_grid_cell(lblDesc, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, rowNo++, 1);
		lv_label_set_long_mode(lblDesc, LV_LABEL_LONG_WRAP);
		lv_obj_set_width(lblDesc, DISPLAY_WIDTH_RAM - ROW_WIDTH_QR - 20);
	}
	if (appInfoDisplay.documentationLink)
	{
		lv_obj_t* lblName = lv_label_create_general(grid, &lblStyleType, "Documentation:", NULL, NULL);
		lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, rowNo++, 1);
		lv_obj_t* lblDesc = lv_label_create_general(grid, &lblStyleValue, appInfoDisplay.documentationLink, NULL, NULL);
		lv_obj_set_grid_cell(lblDesc, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, rowNo++, 1);
		lv_label_set_long_mode(lblDesc, LV_LABEL_LONG_WRAP);
		lv_obj_set_width(lblDesc, DISPLAY_WIDTH_RAM - ROW_WIDTH_QR - 20);
	}

	//
	Close_Create(grid, 7, 0);
}

static screen_type_t Refresh(void)
{
	if (isActive)
	{
		uint8_t tagBuff = tag;
		if (tagBuff != TAG_NONE)
		{
			tag = TAG_NONE;
			if (tagBuff == TAG_CANCEL)
				return SCREEN_PREVIOUS;
		}
	}
	return SCREEN_NONE;
}

static void CreateScreen()
{
	// create basic grid
	static lv_coord_t colsScreen[] = {LV_GRID_FR(1), ROW_WIDTH_QR, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsScreen[] = {60, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	screenGrid = lv_grid_create_general(screen, colsScreen, rowsScreen, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_size(screenGrid, DISPLAY_WIDTH_RAM, DISPLAY_HEIGHT_RAM);

	lv_grid_pos_info_t gridInfo = { .col = 0, .row = 0, .colSpan = 2, .rowSpan = 1 };
	CreateTitle(screenGrid, &gridInfo, "Application Information");
	CreateInfo(screenGrid);
}

static void Load(void)
{
	if (screenGrid != NULL)
	{
		lv_obj_del(screenGrid);
		screenGrid = NULL;
	}
	CreateScreen();
	lv_scr_load(screen);
	isActive = true;
}

static void Unload(void)
{
	isActive = false;
	if (screenGrid != NULL)
	{
		lv_obj_del(screenGrid);
		screenGrid = NULL;
	}
}

/**
 * @brief Initialize screen
 * @param _screen Pointer to fill the screen information
 */
void AppInfoScreen_Init(screens_t* _screen)
{
	static ltdc_layer_info_t directLayer;
	// display the image if required
	if (appInfoDisplay.img != NULL)
	{
		directLayer.xAlign = ALIGN_CENTER_X;
		directLayer.yAlign = ALIGN_CENTER_Y;
		directLayer.posX = DISPLAY_WIDTH - ROW_WIDTH_QR / 2;
		directLayer.posY = DISPLAY_HEIGHT / 2;

		directLayer.PixelFormat = appInfoDisplay.img->pixelFormat;
		directLayer.width = appInfoDisplay.img->width;
		directLayer.height = appInfoDisplay.img->height;
		directLayer.data = appInfoDisplay.img->data;
	}
	// Make the screen
	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	static lv_coord_t colsScreen[] = {LV_GRID_FR(1), ROW_WIDTH_QR, LV_GRID_TEMPLATE_LAST};
	lv_obj_t* screenGrid = lv_grid_create_general(screen, colsScreen, singleRowCol, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_size(screenGrid, DISPLAY_WIDTH_RAM, DISPLAY_HEIGHT_RAM);

	CreateInfo(screenGrid);

	_screen->Refresh = Refresh;
	_screen->Load = Load;
	_screen->Unload = Unload;
	_screen->directLayer = appInfoDisplay.img ? &directLayer : NULL;
	_screen->lvglLayer = &defaultLayer;
}


/* EOF */
