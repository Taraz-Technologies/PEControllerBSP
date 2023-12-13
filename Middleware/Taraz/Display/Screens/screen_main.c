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
#include "p2p_comms.h"
#include "screen_base.h"
#include "stdlib.h"
#include "utility_lib.h"
#include "screen_main_app.h"
#include "screen_data.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define DRAW_APP_SPECIFIC_VIEW		(1)

#define BASE_MEASURE_TAG		(20)
#define MEASURE_TAG(index)		TAG_ATTACH(index + BASE_MEASURE_TAG)
#define IS_MEASURE_TAG(tag)		(tag >= BASE_MEASURE_TAG && tag < (BASE_MEASURE_TAG + 16))
/**** TOUCH TAGS *****/
#define BASE_PARAM_TAG						(55)
#define PARAM_TAG(index)					TAG_ATTACH(index + BASE_PARAM_TAG)
#define IS_PARAM_TAG(tag)					(tag >= BASE_PARAM_TAG && tag < (BASE_PARAM_TAG + CONTROL_CONFS_COUNT))
#define GET_PARAM_INDEX(tag)				(tag - BASE_PARAM_TAG)
/**** TOUCH TAGS *****/

/**** VIEW ALIGNMENT *****/
#ifndef CONTROL_ROW_COUNT
#define CONTROL_ROW_COUNT 					(((CONTROL_CONFS_COUNT - 1) / CONTROL_COL_COUNT) + 1)
#endif

#ifndef MONITOR_ROW_COUNT
#define MONITOR_ROW_COUNT 					(((MONITOR_CONFS_COUNT - 1) / MONITOR_COL_COUNT) + 1)
#endif
/**** VIEW ALIGNMENT *****/

/**** VIEW ALIGNMENT *****/

#define DEFAULT_FLOAT_PRECISION				(2)

/**** DATA ITEM SIZING ****/
/**
 * @brief Cell Height for the Name of both control and monitor parameters
 */
#define NAME_CELL_HEIGHT					(50)
/**** DATA ITEM SIZING ****/

/**
 * @brief Get the measurement index from tag
 */
#define GET_MEASURE_INDEX(tag)	(tag - BASE_MEASURE_TAG)

#define MEASUREMENT_CH_NAME_FONT	(lv_font_montserrat_22)
#define MEASUREMENT_TYPE_FONT		(lv_font_montserrat_16)
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
static bool isNotFirstRefresh = false;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
main_screen_param_disp_t mainScreenParamDisp = {0};
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

// Main Screen Design for the application
#if DRAW_APP_SPECIFIC_VIEW

__weak bool IsToggleableParameter(data_param_info_t* _paramInfo)
{
	return _paramInfo->type == DTYPE_BOOL ||
			_paramInfo->type == DTYPE_BIT_ACCESS;
}

__weak bool MainScreen_GetToggleableParameterValue(data_param_info_t* _paramInfo)
{
	data_union_t value;
	GetDataParameter(_paramInfo, &value);
	return value.b;
}

__weak lv_obj_t* MainScreen_CreateToggleableContent(lv_obj_t* container, lv_style_t* nameStyle, const char* name)
{
	/******************************* Styling **************************************/
	static lv_style_t ledStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		lv_style_set_align(&ledStyle, LV_ALIGN_TOP_RIGHT);
		lv_style_set_x(&ledStyle, -6);
		lv_style_set_y(&ledStyle, 6);
		lv_style_set_width(&ledStyle, 14);
		lv_style_set_height(&ledStyle, 14);
		init = true;
	}

	/******************************* Styling **************************************/

	lv_obj_t* led = lv_led_create(container);
	lv_led_set_color(led, lv_color_make(11, 161, 51));
	lv_obj_add_style(led, &ledStyle, 0);
	lv_led_off(led);

	lv_obj_t* lblValue = lv_label_create_general(container, nameStyle, name, NULL, NULL);
	lv_obj_set_width(lblValue, lv_pct(100));
	lv_obj_align(lblValue, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_long_mode(lblValue, LV_LABEL_LONG_WRAP);

	return led;
}

#if CONTROL_CONFS_COUNT > 0
__weak lv_obj_t* MainScreen_CreateControlCell(lv_obj_t* parent, int index, data_param_info_t* _paramInfo, const char* txtValue)
{
	/******************************* Styling **************************************/
	static lv_style_t paramGridStyle, paramNameStyle, paramValueStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&paramGridStyle, 0, 0, 2, 10, &lv_theme_taraz_get()->color_primary);
		lv_style_set_border_color(&paramGridStyle, lv_theme_taraz_get()->color_secondary);
		BSP_Screen_InitLabelStyle(&paramNameStyle, &lv_font_montserrat_14, LV_TEXT_ALIGN_CENTER, NULL);
		BSP_Screen_InitLabelStyle(&paramValueStyle, &CONTROL_VALUE_FONT, LV_TEXT_ALIGN_CENTER, NULL);
		init = true;
	}

	/******************************* Styling **************************************/
	int col = index % CONTROL_COL_COUNT;
	int row = index / CONTROL_COL_COUNT;

	if (IsToggleableParameter(_paramInfo))
	{
		lv_obj_t* container = lv_container_create_general(parent, &paramGridStyle, row, col, event_handler, PARAM_TAG(index));
		return MainScreen_CreateToggleableContent(container, &paramValueStyle, _paramInfo->name);
	}
	else
	{
		static lv_coord_t rows[] = {LV_GRID_FR(1), NAME_CELL_HEIGHT, LV_GRID_TEMPLATE_LAST};
		lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &paramGridStyle, NULL, event_handler, PARAM_TAG(index));
		lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

		lv_obj_t* lblValue = lv_label_create_general(grid, &paramValueStyle, txtValue, NULL, NULL);
		lv_obj_set_grid_cell(lblValue, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
		lv_obj_center(lblValue);

		lv_obj_t* lblName = lv_label_create_general(grid, &paramNameStyle, _paramInfo->name, NULL, NULL);
		lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
		lv_label_set_long_mode(lblName, LV_LABEL_LONG_WRAP);

		return lblValue;
	}
}

__weak void MainScreen_CreateControlArea(lv_obj_t* parent, int row, int col)
{
	/******************************* Styling **************************************/
	static lv_style_t paramGridStyle;
	static bool init = false;
	static lv_coord_t rows[CONTROL_ROW_COUNT + 1];
	static lv_coord_t cols[CONTROL_COL_COUNT + 1];
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&paramGridStyle, 5, 5, 0, 5, NULL);
		for (int i = 0; i < CONTROL_ROW_COUNT; i++)
			rows[i] = LV_GRID_FR(1);
		rows[CONTROL_ROW_COUNT] = LV_GRID_TEMPLATE_LAST;
		for (int i = 0; i < CONTROL_COL_COUNT; i++)
			cols[i] = LV_GRID_FR(1);
		cols[CONTROL_COL_COUNT] = LV_GRID_TEMPLATE_LAST;
		init = true;
	}
	/******************************* Styling **************************************/
	lv_obj_t* grid = lv_grid_create_general(parent, cols, rows, &paramGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	char txt[20];
	for (int i = 0; i < CONTROL_CONFS_COUNT; i++)
	{
		if (!IsToggleableParameter(mainScreenControlConfs[i]))
			GetDataParameter_InText(mainScreenControlConfs[i], txt, true);
		mainScreenParamDisp.lblsControl[i] = MainScreen_CreateControlCell(grid, i, mainScreenControlConfs[i], txt);
	}
}
#endif

#if MONITOR_CONFS_COUNT > 0
__weak lv_obj_t* MainScreen_CreateMonitorCell(lv_obj_t* parent, int index, data_param_info_t* _paramInfo, const char* txtValue)
{
	/******************************* Styling **************************************/
	static lv_style_t paramGridStyle, paramNameStyle, paramValueStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&paramGridStyle, 0, 0, 2, 10, NULL);
		lv_style_set_border_color(&paramGridStyle, themeColors.gray);
		BSP_Screen_InitLabelStyle(&paramNameStyle, &lv_font_montserrat_14, LV_TEXT_ALIGN_CENTER, NULL);
		BSP_Screen_InitLabelStyle(&paramValueStyle, &MONITOR_VALUE_FONT, LV_TEXT_ALIGN_CENTER, NULL);
		init = true;
	}

	/******************************* Styling **************************************/
	int col = index % MONITOR_COL_COUNT;
	int row = index / MONITOR_COL_COUNT;

	if (IsToggleableParameter(_paramInfo))
	{
		lv_obj_t* container = lv_container_create_general(parent, &paramGridStyle, row, col, NULL, NULL);
		return MainScreen_CreateToggleableContent(container, &paramValueStyle, _paramInfo->name);
	}
	else
	{
		static lv_coord_t rows[] = {LV_GRID_FR(1), NAME_CELL_HEIGHT, LV_GRID_TEMPLATE_LAST};
		lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &paramGridStyle, NULL, NULL, NULL);
		lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

		lv_obj_t* lblValue = lv_label_create_general(grid, &paramValueStyle, txtValue, NULL, NULL);
		lv_obj_set_grid_cell(lblValue, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
		lv_obj_center(lblValue);

		lv_obj_t* lblName = lv_label_create_general(grid, &paramNameStyle, _paramInfo->name, NULL, NULL);
		lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
		lv_label_set_long_mode(lblName, LV_LABEL_LONG_WRAP);

		return lblValue;
	}
}

__weak void MainScreen_CreateMonitorArea(lv_obj_t* parent, int row, int col)
{
	if (MONITOR_CONFS_COUNT == 0)
		return;
	/******************************* Styling **************************************/
	static lv_style_t paramGridStyle;
	static bool init = false;
	static lv_coord_t rows[MONITOR_ROW_COUNT + 1];
	static lv_coord_t cols[MONITOR_COL_COUNT + 1];
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&paramGridStyle, 5, 5, 0, 5, NULL);
		for (int i = 0; i < MONITOR_ROW_COUNT; i++)
			rows[i] = LV_GRID_FR(1);
		rows[MONITOR_ROW_COUNT] = LV_GRID_TEMPLATE_LAST;
		for (int i = 0; i < MONITOR_COL_COUNT; i++)
			cols[i] = LV_GRID_FR(1);
		cols[MONITOR_COL_COUNT] = LV_GRID_TEMPLATE_LAST;
		init = true;
	}
	/******************************* Styling **************************************/
	lv_obj_t* grid = lv_grid_create_general(parent, cols, rows, &paramGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	char txt[20];
	for (int i = 0; i < MONITOR_CONFS_COUNT; i++)
	{
		if (!IsToggleableParameter(mainScreenMonitorConfs[i]))
			GetDataParameter_InText(mainScreenMonitorConfs[i], txt, true);
		mainScreenParamDisp.lblsMonitor[i] = MainScreen_CreateMonitorCell(grid, i, mainScreenMonitorConfs[i], txt);
	}
}
#endif
/**
 * @brief Creates the application dependent area of the main screen.
 * @param parent Parent lv_obj_t of the device.
 * @param row Row index for the item.
 * @param col Column index for the item.
 */
__weak void MainScreen_CreateAppArea(lv_obj_t* parent, int row, int col)
{
	/******************************* Styling **************************************/
	static lv_style_t btnsGridStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&btnsGridStyle, 5, 5, 0, 0, NULL);
		init = true;
	}
	/******************************* Styling **************************************/

	/********************************* Base ***************************************/
	// create control grid
#if CONTROL_CONFS_COUNT > 0 && MONITOR_CONFS_COUNT > 0
	static lv_coord_t rows[] = {LV_GRID_FR(MONITOR_AREA_RATIO), LV_GRID_FR(CONTROL_AREA_RATIO), LV_GRID_TEMPLATE_LAST};
#else
	static lv_coord_t rows[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
#endif
	lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
	/********************************* Base ***************************************/

	/******************************* Parameters *********************************/
#if CONTROL_CONFS_COUNT > 0 && MONITOR_CONFS_COUNT > 0
	MainScreen_CreateMonitorArea(grid, 0, 0);
	MainScreen_CreateControlArea(grid, 1, 0);
#elif CONTROL_CONFS_COUNT > 0
	MainScreen_CreateControlArea(grid, 0, 0);
#elif MONITOR_CONFS_COUNT > 0
	MainScreen_CreateMonitorArea(grid, 0, 0);
#endif
	/******************************* Parameters *********************************/
}

/**
 * @brief Refreshes the application dependent area of the main screen.
 */
__weak void MainScreen_RefreshAppArea(void)
{
	char txt[20];
#if CONTROL_CONFS_COUNT > 0
	for (int i = 0; i < CONTROL_CONFS_COUNT; i++)
	{
		// No need to refresh area if nothing changed
		if (isNotFirstRefresh)
		{
			data_union_t value;
			GetDataParameter(mainScreenControlConfs[i], &value);
			if (mainScreenParamDisp.controlVals[i].u32 == value.u32)
				continue;
			mainScreenParamDisp.controlVals[i].u32 = value.u32;
		}

		// Refresh area
		if (IsToggleableParameter(mainScreenControlConfs[i]))
		{
			if (MainScreen_GetToggleableParameterValue(mainScreenControlConfs[i]))
				lv_led_on(mainScreenParamDisp.lblsControl[i]);
			else
				lv_led_off(mainScreenParamDisp.lblsControl[i]);
		}
		else
		{
			GetDataParameter_InText(mainScreenControlConfs[i], txt, true);
			lv_label_set_text(mainScreenParamDisp.lblsControl[i], txt);
		}
	}
#endif
#if MONITOR_CONFS_COUNT > 0
	for (int i = 0; i < MONITOR_CONFS_COUNT; i++)
	{
		// No need to refresh area if nothing changed
		if (isNotFirstRefresh)
		{
			data_union_t value;
			GetDataParameter(mainScreenMonitorConfs[i], &value);
			if (mainScreenParamDisp.monitorVals[i].u32 == value.u32)
				continue;
			mainScreenParamDisp.monitorVals[i].u32 = value.u32;
		}

		// Refresh area
		if (IsToggleableParameter(mainScreenMonitorConfs[i]))
		{
			if (MainScreen_GetToggleableParameterValue(mainScreenMonitorConfs[i]))
				lv_led_on(mainScreenParamDisp.lblsMonitor[i]);
			else
				lv_led_off(mainScreenParamDisp.lblsMonitor[i]);
		}
		else
		{
			GetDataParameter_InText(mainScreenMonitorConfs[i], txt, true);
			lv_label_set_text(mainScreenParamDisp.lblsMonitor[i], txt);
		}
	}
#endif
	isNotFirstRefresh = true;
}

/**
 * @brief Touch detection of the application dependent area of the screen.
 * @param _tag Tag if any detected on the generic main screen.
 * @return A new screen type if screen needs to be changed based upon the touch response.
 */
__weak screen_type_t MainScreen_AppTouchDetect(uint8_t _tag)
{
	uint8_t tagBuff = _tag;
	_tag = TAG_NONE;
	if (IS_PARAM_TAG(tagBuff))
	{
		int index = GET_PARAM_INDEX(tagBuff);
		char txt[20];
#if CONTROL_CONFS_COUNT > 0
		// Get value and toggle if boolean
		if (mainScreenControlConfs[index]->type == DTYPE_BOOL)
		{
			data_union_t value;
			device_err_t err = GetDataParameter(mainScreenControlConfs[index], &value);
			if (err != ERR_OK)
			{
				DisplayMessage(errInfo[err].caption, errInfo[err].desc);
				return SCREEN_NONE;
			}
			value.b = value.b ? false : true;
			err = SetDataParameter(mainScreenControlConfs[index], value);
			if (err != ERR_OK)
			{
				DisplayMessage(errInfo[err].caption, errInfo[err].desc);
				return SCREEN_NONE;
			}
		}
		// directly toggle if bit accessible
		else if(mainScreenControlConfs[index]->type == DTYPE_BIT_ACCESS)
		{
			data_union_t value;
			value.bits = BITS_TOGGLE;
			device_err_t err = SetDataParameter(mainScreenControlConfs[index], value);
			if (err != ERR_OK)
			{
				DisplayMessage(errInfo[err].caption, errInfo[err].desc);
				return SCREEN_NONE;
			}
		}
		else
		{
			GetDataParameter_InText(mainScreenControlConfs[index], txt, false);
			ConfigScreen_LoadParam(mainScreenControlConfs[index], txt);
			return SCREEN_CONF;
		}
#endif
	}
	return SCREEN_NONE;
}
#endif

/**
 * @brief select appropriate color for the measurement according to the unit
 * @param i Measurement Index
 */
static void SelectMeasurementColor(int i)
{
	char txt[10];
	lv_color_t color;
	if(chDisplay[i].lastUnit == UNIT_V)
	{
		txt[0] = 'V';
		color = themeColors.voltage;
	}
	else
	{
		txt[0] = 'A';
		color = themeColors.current;
	}
	txt[1] = ' ';
	CopyString(txt+2, measureTxts[(uint8_t)chDisplay[i].lastType]);
	lv_label_set_text(chDisplay[i].lblReading, txt);
	lv_obj_set_style_bg_color(chDisplay[i].gridMeasure, color, 0);
	lv_obj_set_style_bg_color(chDisplay[i].gridName, color, 0);
}

/**
 * @brief Create a cell for measurement display on main grid
 * @param parent
 * @param index
 */
static void MeasurementCell_Create(lv_obj_t * parent, int index)
{
	static lv_style_t cellGridStyle;
	static lv_style_t chNameLblStyle, chReadingTypeLblStyle, chValueLblStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		//
		BSP_Screen_InitGridStyle(&cellGridStyle, 2, 0, 0, 5, NULL);
		// Initialize the basic grid cell label styles
		BSP_Screen_InitLabelStyle(&chValueLblStyle, &MEASUREMENT_VALUE_FONT, LV_TEXT_ALIGN_CENTER, NULL);
		BSP_Screen_InitLabelStyle(&chReadingTypeLblStyle, &MEASUREMENT_TYPE_FONT, LV_TEXT_ALIGN_LEFT, NULL);
		BSP_Screen_InitLabelStyle(&chNameLblStyle, &MEASUREMENT_CH_NAME_FONT, LV_TEXT_ALIGN_CENTER, NULL);
		init = true;
	}

	int col = index % 4;
	int row = index / 4;

	ch_disp_t* disp = &chDisplay[index];

	disp->lastType = dispMeasures.chMeasures[index].type;
	disp->lastUnit = UNIT_V;

	// Set main grid element
	static lv_coord_t rows[] = {LV_GRID_FR(10), LV_GRID_FR(4), LV_GRID_TEMPLATE_LAST};
	disp->gridMeasure = lv_grid_create_general(parent, singleRowCol, rows, &cellGridStyle, NULL, event_handler, MEASURE_TAG(index));
	lv_obj_set_grid_cell(disp->gridMeasure, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	char txtVal[10];
	const char* txtRead = NULL;
	txtRead = measureTxts[(uint8_t)disp->lastType];
	ftoa_custom(0, txtVal, 4, 1);

	lv_obj_t * containerValue = lv_container_create_general(disp->gridMeasure, &lvStyleStore.defaultGrid, 0, 0, event_handler, MEASURE_TAG(index));
	disp->gridValue = containerValue;
	disp->lblValue = lv_label_create_general(containerValue, &chValueLblStyle, txtVal, NULL, NULL);
	disp->lblReading = lv_label_create_general(containerValue, &chReadingTypeLblStyle, txtRead, NULL, NULL);
	lv_obj_align(disp->lblValue, LV_ALIGN_CENTER, 0, -8);
	lv_obj_align(disp->lblReading, LV_ALIGN_BOTTOM_MID, 0, -6);

	// set the name portion
	disp->gridName = lv_container_create_general(disp->gridMeasure, &lvStyleStore.defaultGrid, 1, 0, event_handler, MEASURE_TAG(index));
	lv_obj_t* lblName = lv_label_create_general(disp->gridName, &chNameLblStyle, dispMeasures.chNames[index], NULL, NULL);
	lv_obj_align(lblName, LV_ALIGN_CENTER, 0, 0);

	SelectMeasurementColor(index);
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

/**
 * @brief Create the header for the main page
 * @param parent
 */
static void Header_Create(lv_obj_t* parent)
{
	static lv_style_t padGridStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		//
		BSP_Screen_InitGridStyle(&padGridStyle, 5, 5, 0, 0, NULL);
		lv_style_set_pad_top(&padGridStyle, 10);
		init = true;
	}

	lv_obj_t* titleCellArea = lv_grid_create_general(parent, singleRowCol, singleRowCol, &padGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(titleCellArea, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_ta_field_data_t field = {
			.isTextArea = false, .colorFieldName = true, .nameTxt = LV_SYMBOL_SETTINGS, .valueTxt = appInfoDisplay.appTitle, .colWidths = { 60, LV_GRID_FR(1) }
	};
	lv_create_default_text_field(titleCellArea, &field, 0, 0, NULL, NULL);
	lv_obj_align(field.nameField, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_text_font(field.nameField, &lv_font_montserrat_28, 0);
	lv_obj_add_flag(field.nameField, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(field.valueField, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(field.nameField, event_handler, LV_EVENT_CLICKED, TAG_ATTACH(TAG_SETTINGS));
	lv_obj_add_event_cb(field.valueField, event_handler, LV_EVENT_CLICKED, TAG_ATTACH(TAG_APPINFO));

	// Attach tag to intelliSENS
	lv_container_create_general(parent, &lvStyleStore.defaultGrid, 0, 1, event_handler, TAG_ATTACH(TAG_intelliSENS));
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
	}
}

static void MeasurementArea_Refresh(void)
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

		SelectMeasurementColor(i);
	}
}

static screen_type_t Refresh(void)
{
	if (isActive)
	{
		uint8_t tagBuff = tag;
		if (tagBuff != TAG_NONE)
		{
			tag = TAG_NONE;
			if (IS_MEASURE_TAG(tagBuff))
			{
				uint8_t index = GET_MEASURE_INDEX(tagBuff);
				ConfigScreen_LoadMeasurement(index);
				return SCREEN_CONF;
			}
#if SETTINGS_WINDOW_COUNT
			if (tagBuff == TAG_SETTINGS)
			{
				ConfigScreen_LoadSettings(&settingWindows[0], SETTINGS_WINDOW_COUNT);
				return SCREEN_CONF;
			}
#endif
			if (tagBuff == TAG_intelliSENS)
				return SCREEN_intelliSENS;
			if (tagBuff == TAG_APPINFO)
				return SCREEN_APPINFO;
		}

		screen_type_t scr = MainScreen_AppTouchDetect(tagBuff);
		if (scr!=SCREEN_NONE)
			return scr;

		MainScreen_RefreshAppArea();
		MeasurementArea_Refresh();
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
	directLayer.posX = 10;
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
	MainScreen_CreateAppArea(screenGrid, 1, 1);

	_screen->Refresh = Refresh;
	_screen->Load = Load;
	_screen->Unload = Unload;
	_screen->directLayer = &directLayer;
	_screen->lvglLayer = &defaultLayer;
}

/* EOF */
