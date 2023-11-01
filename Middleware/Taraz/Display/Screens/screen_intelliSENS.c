/**
 ********************************************************************************
 * @file    	screen_intelliSENS.c
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
#include "lv_conf.h"
#include "user_config.h"
#include "pecontroller_intelliSENS.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
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
static volatile bool isActive;
static volatile uint8_t tag = TAG_NONE;
static const char* softwareDownloadLink = "www.taraztechnologies.com/products/intellisens/daq-application-software/";
static const char* qrCodeHeading = "Scan to visit intelliSENS";
static const char* sections[4] = {
		"Description:",
		"Usage:",
		"Licensing:",
		"License #:",
};
static const char* sectionValues[4] = {
		"The intelliSENS software is a ready-to-use application software"
		" designed to assist students, researchers and engineers in the field of power electronics"
		" to develop, monitor & test power systems by replacing multiple lab equipment, such as,"
		" oscilloscopes, spectrum analyzers, power analyzers and recorders.",

		"\t1. Set \"ENABLE_INTELLISENS\" in \"user_config.h\" to 1.\n"
		"\t2. Program the PEController and connect OTG port to PC.\n"
		"\t3. In \"intelliSENS Analysis\" connect to your PEController.",

		"For activation send license number to sales@taraztechnologies.com with the subject "
		"\"Quotation request for PEController intelliSENS activation\".",

		NULL};
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
static void Keypad_Clicked(lv_event_t * e)
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
	lv_btnmatrix_set_map(kb, map);
	lv_obj_set_style_bg_color(kb, lvColorStore.background, 0);
	lv_obj_set_style_text_color(kb, lvColorStore.lightTaraz, 0);
	lv_obj_set_style_bg_color(kb, lvColorStore.darkTaraz, LV_PART_ITEMS);
	lv_obj_set_style_text_color(kb, lvColorStore.white, LV_PART_ITEMS);
	lv_obj_set_style_border_color(kb, lvColorStore.lightTaraz, LV_PART_ITEMS);
	lv_obj_set_style_border_width(kb, 2, LV_PART_ITEMS);
	lv_obj_set_style_text_font(kb, &lv_font_montserrat_30, 0);
	lv_obj_set_width(kb, 200);
	lv_obj_add_event_cb(kb, Keypad_Clicked, LV_EVENT_CLICKED, NULL);
}

static void CreateInfo(lv_obj_t * parent)
{
	static lv_style_t lblStyleType;
	static lv_style_t lblStyleValue;
	static lv_style_t btnLblStyle;
	static bool init = false;
	if (!init)
	{
		BSP_Screen_InitLabelStyle(&lblStyleType, &lv_font_montserrat_22, LV_TEXT_ALIGN_LEFT, &lvColorStore.mediumTaraz);
		lv_style_set_text_decor(&lblStyleType, LV_TEXT_DECOR_UNDERLINE);
		BSP_Screen_InitLabelStyle(&lblStyleValue, &lv_font_montserrat_18, LV_TEXT_ALIGN_LEFT, &lvColorStore.white);
		BSP_Screen_InitLabelStyle(&btnLblStyle, &lv_font_montserrat_26, LV_TEXT_ALIGN_CENTER, &lvColorStore.white);
		init = true;
	}

	// main grid
	static lv_coord_t rows[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_FR(1), 80 , LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.thickMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

	int rowNo = 0;
	// Set the names and description
	for (int i = 1; i < 4; i++)
	{
		lv_obj_t* lblName = lv_label_create_general(grid, &lblStyleType, sections[i], NULL, NULL);
		lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, rowNo++, 1);
		if(sectionValues[i] == NULL)
			continue;
		lv_obj_t* lblDesc = lv_label_create_general(grid, &lblStyleValue, sectionValues[i], NULL, NULL);
		lv_obj_set_grid_cell(lblDesc, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, rowNo++, 1);
		lv_label_set_long_mode(lblDesc, LV_LABEL_LONG_WRAP);
		lv_obj_set_width(lblDesc, DISPLAY_WIDTH_RAM - ROW_WIDTH_QR - 20);
	}

	lv_obj_t* lblLicense = lv_label_create_general(grid, &lblStyleValue,
#if ENABLE_INTELLISENS
	intelliSENS_GetLicenseNumberString()
#else
	"Kindly enable intelliSENS feature from user_config.h file to view license number."
#endif
			, NULL, NULL);
	lv_obj_set_grid_cell(lblLicense, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, rowNo++, 1);
	lv_label_set_long_mode(lblLicense, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(lblLicense, DISPLAY_WIDTH_RAM - ROW_WIDTH_QR - 20);

	//
	Close_Create(grid, 12, 0);
}

static void CreateQRCode(lv_obj_t * parent)
{
	static lv_style_t lblStyleHeading;
	static lv_style_t lblStyleLink;
	static bool init = false;
	if (!init)
	{
		BSP_Screen_InitLabelStyle(&lblStyleHeading, &lv_font_montserrat_22, LV_TEXT_ALIGN_CENTER, &lvColorStore.white);
		BSP_Screen_InitLabelStyle(&lblStyleLink, &lv_font_montserrat_16, LV_TEXT_ALIGN_CENTER, &lvColorStore.white);
		init = true;
	}

	// main grid
	static lv_coord_t rows[] = {LV_GRID_FR(1), 265, LV_GRID_FR(1), 60, LV_GRID_TEMPLATE_LAST};
	rows[1] = intelliSENS_QR_info.height;
	lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.thickMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

	lv_obj_t* lblHeading = lv_label_create_general(grid, &lblStyleHeading, qrCodeHeading, NULL, NULL);
	lv_obj_set_grid_cell(lblHeading, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_END, 0, 1);
	lv_label_set_long_mode(lblHeading, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(lblHeading, ROW_WIDTH_QR - 20);

	lv_obj_t* lblLink = lv_label_create_general(grid, &lblStyleLink, softwareDownloadLink, NULL, NULL);
	lv_obj_set_grid_cell(lblLink, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_START, 2, 1);
	lv_label_set_long_mode(lblLink, LV_LABEL_LONG_WRAP);
	lv_obj_set_width(lblLink, ROW_WIDTH_QR - 20);
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
	CreateTitle(screenGrid, &gridInfo, "intelliSENS Application Usage");

	CreateInfo(screenGrid);
	CreateQRCode(screenGrid);
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
void IntellisensScreen_Init(screens_t* _screen)
{
	static ltdc_layer_info_t directLayer =
	{
			.xAlign = ALIGN_CENTER_X,
			.yAlign = ALIGN_CENTER_Y,
			.posX = DISPLAY_WIDTH - ROW_WIDTH_QR / 2,
			.posY = DISPLAY_HEIGHT / 2
	};
	directLayer.PixelFormat = intelliSENS_QR_info.pixelFormat;
	directLayer.width = intelliSENS_QR_info.width;
	directLayer.height = intelliSENS_QR_info.height;
	directLayer.data = intelliSENS_QR_info.data;

	// Make the screen
	// create the screen
	screen = lv_obj_create(NULL);

	_screen->Refresh = Refresh;
	_screen->Load = Load;
	_screen->Unload = Unload;
	_screen->directLayer = &directLayer;
	_screen->lvglLayer = &defaultLayer;
}

/* EOF */
