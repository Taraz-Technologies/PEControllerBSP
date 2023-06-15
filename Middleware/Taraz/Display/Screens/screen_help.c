/**
 ********************************************************************************
 * @file    	screen_help.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 24, 2023
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
static const char* softwareDownloadLink = "www.taraztechnologies.com/Downloads/Software/PEControllerBSP/index.html";
static const char* qrCodeHeading = "Scan to visit documentation";
static const char* sections[2] = {
		"Description:",
		"Documentation:",
};
static const char* sectionValues[2] = {
		"The PEController's Board Support Package (BSP) offers a comprehensive framework"
		" that greatly simplifies application development for users. In addition to this,"
		" the BSP includes demo projects that aim to familiarize users with the drivers "
		"and demonstrate their proper usage. The BSP firmware for the PEController is an "
		"open source project hosted on GitHub. For the latest version of the framework, "
		"visit \"https://github.com/Taraz-Technologies/PEControllerBSP\".",

		"The documentation for the framework is generated using \"Doxygen\" software, "
		"and can be accessed by scanning the QR code.", };
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
	tag = GET_TAG(e);
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
		BSP_Screen_InitLabelStyle(&lblStyleValue, &lv_font_montserrat_18, LV_TEXT_ALIGN_LEFT, &lvColorStore.darkFont);
		BSP_Screen_InitLabelStyle(&btnLblStyle, &lv_font_montserrat_26, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		init = true;
	}

	// main grid
	static lv_coord_t rows[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_CONTENT, LV_GRID_CONTENT,
			LV_GRID_FR(1), 60 , LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.thickMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	int rowNo = 0;
	// Set the names and description
	for (int i = 0; i < 2; i++)
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

	//
	lv_obj_t* closeBtn = lv_btn_create_general(grid, &lvStyleStore.btn2, &btnLblStyle, "Close", Close_Clicked, TAG_ATTACH(TAG_CANCEL));
	lv_obj_set_grid_cell(closeBtn, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_STRETCH, 12, 1);
	lv_obj_set_width(closeBtn, 200);
}

static void CreateQRCode(lv_obj_t * parent)
{
	static lv_style_t lblStyleHeading;
	static lv_style_t lblStyleLink;
	static bool init = false;
	if (!init)
	{
		BSP_Screen_InitLabelStyle(&lblStyleHeading, &lv_font_montserrat_22, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		BSP_Screen_InitLabelStyle(&lblStyleLink, &lv_font_montserrat_16, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		init = true;
	}

	// main grid
	static lv_coord_t rows[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	rows[1] = bsp_docs_QR_info.height;
	lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.thickMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

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
	screenGrid = lv_grid_create_general(screen, colsScreen, singleRowCol, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_size(screenGrid, DISPLAY_WIDTH_RAM, DISPLAY_HEIGHT_RAM);

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
void HelpScreen_Init(screens_t* _screen)
{
	static ltdc_layer_info_t directLayer =
	{
			.xAlign = ALIGN_CENTER_X,
			.yAlign = ALIGN_CENTER_Y,
			.posX = DISPLAY_WIDTH - ROW_WIDTH_QR / 2,
			.posY = DISPLAY_HEIGHT / 2
	};
	directLayer.PixelFormat = bsp_docs_QR_info.pixelFormat;
	directLayer.width = bsp_docs_QR_info.width;
	directLayer.height = bsp_docs_QR_info.height;
	directLayer.data = bsp_docs_QR_info.data;

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
