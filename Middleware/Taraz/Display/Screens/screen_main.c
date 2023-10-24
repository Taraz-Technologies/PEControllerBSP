/**
 ********************************************************************************
 * @file    	screen_main.c
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
#include "screen_base.h"
#include "stdlib.h"
#include "utility_lib.h"
#include "interprocessor_comms.h"
#include "screen_main_app.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define BASE_MEASURE_TAG		(20)
#define MEASURE_TAG(index)		TAG_ATTACH(index + BASE_MEASURE_TAG)
#define IS_MEASURE_TAG(tag)		(tag >= BASE_MEASURE_TAG && tag < (BASE_MEASURE_TAG + 16))
/**
 * @brief Get the measurement index from tag
 */
#define GET_MEASURE_INDEX(tag)	(tag - BASE_MEASURE_TAG)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef struct
{
	measure_type_t lastType;
	data_units_t lastUnit;
	lv_obj_t* lblReading;
	lv_obj_t* lblValue;
	lv_obj_t* gridMeasure;
	lv_obj_t* gridValue;
	lv_obj_t* gridName;
} ch_disp_t;
/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static lv_obj_t* screen;
static ch_disp_t chDisplay[16];
static volatile bool isActive;
static volatile uint8_t tag = TAG_NONE;
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
static void event_handler(lv_event_t * e)
{
	if (!isActive)
		return;
	tag = GET_TAG(e);
}
static void SelectColor(int i)
{
	char txt[10];
	lv_color_t* color;
	//lv_color_t colorBlock;
	if(chDisplay[i].lastUnit == UNIT_V)
	{
		txt[0] = 'V';
		color = &lvColorStore.voltage;
		//colorBlock = MakeColor(50, 80, 110);
	}
	else
	{
		txt[0] = 'A';
		color = &lvColorStore.current;
		//colorBlock = MakeColor(110, 90, 90);
	}
	txt[1] = ' ';
	CopyString(txt+2, measureTxts[(uint8_t)chDisplay[i].lastType]);
	lv_label_set_text(chDisplay[i].lblReading, txt);
	lv_obj_set_style_bg_color(chDisplay[i].gridMeasure, *color, 0);
	lv_obj_set_style_bg_color(chDisplay[i].gridName, *color, 0);
	//lv_obj_set_style_bg_color(chDisplay[i].gridValue, colorBlock, 0);
}

/**
 * @brief Create a cell for measurement display on main grid
 * @param parent
 * @param index
 */
static void MeasurementCell_Create(lv_obj_t * parent, int index)
{
	static lv_style_t cellGridStyle;
	static lv_style_t chNameContainerStyle, chValueContainerStyle;
	static lv_style_t chNameLblStyle, chReadingTypeLblStyle, chValueLblStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		//
		BSP_Screen_InitGridStyle(&cellGridStyle, 2, 0, 0, 5, &lvColorStore.voltage);
		BSP_Screen_InitGridStyle(&chValueContainerStyle, 0, 0, 0, 0, &lvColorStore.background);
		BSP_Screen_InitGridStyle(&chNameContainerStyle, 0, 0, 0, 0, &lvColorStore.voltage);
		// Initialize the basic grid cell label styles
		BSP_Screen_InitLabelStyle(&chValueLblStyle, &MEASUREMENT_VALUE_FONT, LV_TEXT_ALIGN_CENTER, &lvColorStore.white);
		BSP_Screen_InitLabelStyle(&chReadingTypeLblStyle, &lv_font_montserrat_16, LV_TEXT_ALIGN_LEFT, &lvColorStore.white);
		BSP_Screen_InitLabelStyle(&chNameLblStyle, &lv_font_montserrat_22, LV_TEXT_ALIGN_CENTER, &lvColorStore.white);
		init = true;
	}

	int col = index % 4;
	int row = index / 4;

	ch_disp_t* disp = &chDisplay[index];

	disp->lastType = dispMeasures.chMeasures[index].type;
	disp->lastUnit = UNIT_V;

	// Set main grid element
	static lv_coord_t rows[] = {LV_GRID_FR(10), LV_GRID_FR(4), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * grid = lv_grid_create_general(parent, singleRowCol, rows, &cellGridStyle, NULL, event_handler, MEASURE_TAG(index));
	disp->gridMeasure = grid;
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	char txtVal[10];
	const char* txtRead = NULL;
	txtRead = measureTxts[(uint8_t)disp->lastType];
	ftoa_custom(0, txtVal, 4, 1);

	lv_obj_t * containerValue = lv_container_create_general(grid, &chValueContainerStyle, 0, 0, event_handler, MEASURE_TAG(index));
	disp->gridValue = containerValue;
	disp->lblValue = lv_label_create_general(containerValue, &chValueLblStyle, txtVal, NULL, NULL);
	disp->lblReading = lv_label_create_general(containerValue, &chReadingTypeLblStyle, txtRead, NULL, NULL);
	lv_obj_align(disp->lblValue, LV_ALIGN_CENTER, 0, -8);
	lv_obj_align(disp->lblReading, LV_ALIGN_BOTTOM_MID, 0, -6);

	// set the name portion
	lv_obj_t* containerName = lv_container_create_general(grid, &chNameContainerStyle, 1, 0, event_handler, MEASURE_TAG(index));
	disp->gridName = containerName;
	lv_obj_t* lblName = lv_label_create_general(containerName, &chNameLblStyle, dispMeasures.chNames[index], NULL, NULL);
	lv_obj_align(lblName, LV_ALIGN_CENTER, 0, 0);

	SelectColor(index);
}

/**
 * @brief Create the measurement grid
 * @param parent
 */
static void MeasurementGrid_Create(lv_obj_t* parent)
{
	// create monitoring grid
	static lv_coord_t cols[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rows[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, cols, rows, &lvStyleStore.mediumMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
	// create all cells
	for (int i = 0; i < 16; i++)
		MeasurementCell_Create(grid, i);
}

static void Title_Create(lv_obj_t * parent)
{
	static lv_style_t padGridStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		//
		BSP_Screen_InitGridStyle(&padGridStyle, 5, 5, 0, 0, &lvColorStore.background);
		//lv_style_set_pad_row(&padGridStyle, 10);
		lv_style_set_pad_bottom(&padGridStyle, 10);
		lv_style_set_pad_top(&padGridStyle, 10);
		init = true;
	}

	lv_obj_t* titleCellArea = lv_grid_create_general(parent, singleRowCol, singleRowCol, &padGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(titleCellArea, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_ta_field_data_t field =
	{
		.isTextArea = false,
		.colorFieldName = true,
		.nameTxt = "AN01",
		.valueTxt = "Open-Loop V/f Control",
		//.nameTxt = "AN02",
		//.valueTxt = "Three-Phase Grid-Tie Inverter",
		.colWidths = { 70, LV_GRID_FR(1) }
	};
	lv_default_text_field(titleCellArea, &field, 0, 0, event_handler, TAG_ATTACH(TAG_APPINFO));

}

static void Header_Create(lv_obj_t* parent)
{
	static lv_style_t titleContainerStyle;
	static lv_style_t lblStyleName;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&titleContainerStyle, 5, 5, 0, 0, &lvColorStore.background);
		BSP_Screen_InitLabelStyle(&lblStyleName, &lv_font_montserrat_24, LV_TEXT_ALIGN_LEFT, &lvColorStore.white);
		lv_style_set_text_align(&lblStyleName, LV_TEXT_ALIGN_LEFT);
		init = true;
	}

	Title_Create(parent);

	lv_container_create_general(parent, &lvStyleStore.defaultGrid, 0, 1, event_handler, TAG_ATTACH(TAG_intelliSENS));
}

static void ControlGrid_Create(lv_obj_t* parent)
{
	AppControlGrid_Create(parent, 1, 1);
}

static void Load(void)
{
	lv_scr_load(screen);
	isActive = true;
}

static void Unload(void)
{
	if (isActive)
	{
		isActive = false;
		//lv_obj_del(screen);
	}
}

static screen_type_t Refresh(void)
{
	if (isActive)
	{
		if (tag != TAG_NONE)
		{
			uint8_t tagBuff = tag;
			tag = TAG_NONE;
			if (IS_MEASURE_TAG(tagBuff))
			{
				uint8_t index = GET_MEASURE_INDEX(tagBuff);
				ConfigScreen_LoadMeasurement(index);
				return SCREEN_CONF;
			}
			if (tagBuff == TAG_intelliSENS)
				return SCREEN_intelliSENS;
			if (tagBuff == TAG_APPINFO)
				return SCREEN_APPINFO;
			if (tagBuff == TAG_HELP)
				return SCREEN_HELP;
		}

		screen_type_t scr = AppControlGrid_TouchDetect();
		if (scr!=SCREEN_NONE)
			return scr;

		AppControlGrid_Refresh();
		{
			for (int i = 0; i < 16; i++)
			{
				measure_type_t type = dispMeasures.chMeasures[i].type;
				float* stats = ((float*)&dispMeasures.adcInfo->stats[i]);
				char txt[10];
				ftoa_custom(stats[(uint8_t)type], txt, 4, 1);
				lv_label_set_text(chDisplay[i].lblValue, txt);

				if (chDisplay[i].lastType == dispMeasures.chMeasures[i].type && chDisplay[i].lastUnit == dispMeasures.adcInfo->units[i])
					continue;

				chDisplay[i].lastType = dispMeasures.chMeasures[i].type;
				chDisplay[i].lastUnit = dispMeasures.adcInfo->units[i];

				SelectColor(i);
			}
		}
	}
	return SCREEN_NONE;
}

/**
 * @brief Initialize screen
 * @param _screen Pointer to fill the screen information
 */
void MainScreen_Init(screens_t* _screen)
{
	static ltdc_layer_info_t directLayer =
	{
			.xAlign = ALIGN_RIGHT_X,
			.yAlign = ALIGN_UP_Y,
			.posY = 12
	};
	directLayer.posX = 10;//(MEASUREMENT_AREA_RATIO * DISPLAY_WIDTH_RAM) / (MEASUREMENT_AREA_RATIO + APP_AREA_RATIO) + 10;
	directLayer.PixelFormat = intelliSENS_logo_info.pixelFormat;
	directLayer.width = intelliSENS_logo_info.width;
	directLayer.height = intelliSENS_logo_info.height;
	directLayer.data = intelliSENS_logo_info.data;

	// Make the screen
	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	static lv_coord_t rowsScreen[] = {60, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t colsScreen[] = {LV_GRID_FR(MEASUREMENT_AREA_RATIO), LV_GRID_FR(APP_AREA_RATIO), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* screenGrid = lv_grid_create_general(screen, colsScreen, rowsScreen, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_size(screenGrid, DISPLAY_WIDTH_RAM, DISPLAY_HEIGHT_RAM);

	Header_Create(screenGrid);
	MeasurementGrid_Create(screenGrid);
	ControlGrid_Create(screenGrid);

	_screen->Refresh = Refresh;
	_screen->Load = Load;
	_screen->Unload = Unload;
	_screen->directLayer = &directLayer;
	_screen->lvglLayer = &defaultLayer;
}

/* EOF */
