/**
 ********************************************************************************
 * @file    	screen_main_app.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 23, 2023
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
#include "screen_main_app.h"
#include "shared_memory.h"
#include "data_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
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

#define DEFAULT_FLOAT_PRECISION				(2)

/**** DATA ITEM SIZING ****/
/**
 * @brief Cell Height for the Name of both control and monitor parameters
 */
#define NAME_CELL_HEIGHT					(35)
/**** DATA ITEM SIZING ****/
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
#if CONTROL_CONFS_COUNT > 0
	data_union_t controlVals[CONTROL_CONFS_COUNT];
	lv_obj_t* lblsControl[CONTROL_CONFS_COUNT];
#endif
#if MONITOR_CONFS_COUNT > 0
	data_union_t monitorVals[MONITOR_CONFS_COUNT];
	lv_obj_t* lblsMonitor[MONITOR_CONFS_COUNT];
#endif
} param_disp_t;
/********************************************************************************
 * Static Variables
 *******************************************************************************/
#if CONTROL_CONFS_COUNT > 0
static data_param_info_t controlConfs[CONTROL_CONFS_COUNT] =
{
		{ .name = "iReq (RMS)", .index = SHARE_REQ_RMS_CURRENT, .type = DTYPE_FLOAT, .arg = 1, .unit = UNIT_A },
		{ .name = "Grid Freq.", .index = SHARE_GRID_FREQ, .type = DTYPE_FLOAT, .arg = 1, .unit = UNIT_Hz },
		{ .name = "Grid Voltage", .index = SHARE_GRID_VOLTAGE, .type = DTYPE_FLOAT, .arg = 1, .unit = UNIT_V },
		{ .name = "Output Inductance", .index = SHARE_LOUT, .type = DTYPE_FLOAT, .arg = 4, .unit = UNIT_NONE },
		{ .name = "Boost Control", .index = SHARE_BOOST_STATE, .type = DTYPE_BOOL},
		{ .name = "Inverter Control", .index = SHARE_INVERTER_STATE, .type = DTYPE_BOOL},
};
#endif
#if MONITOR_CONFS_COUNT > 0
static data_param_info_t monitorConfs[MONITOR_CONFS_COUNT] =
{
		{ .name = "iGen (RMS)", .index = SHARE_CURR_RMS_CURRENT, .type = DTYPE_FLOAT, .arg = 1, .unit = UNIT_A },
		{ .name = "Relay Status", .index = SHARE_RELAY_STATUS, .type = DTYPE_BOOL },
		{ .name = "PLL Status", .index = SHARE_PLL_STATUS, .type = DTYPE_BOOL },
};
#endif
static param_disp_t paramDisp = {0};
static volatile bool isActive;
static volatile uint8_t tag = TAG_NONE;
static bool isNotFirstRefresh = false;
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
extern void ConfigScreen_LoadParam(data_param_info_t* _paramConf, char* val);
/********************************************************************************
 * Code
 *******************************************************************************/
static void event_handler(lv_event_t * e)
{
	tag = GET_TAG(e);
}

static bool IsToggleableParameter(data_param_info_t* _paramInfo)
{
	return _paramInfo->type == DTYPE_BOOL ||
			_paramInfo->type == DTYPE_BIT_ACCESS;
}

static bool GetToggleableParameterValue(data_param_info_t* _paramInfo)
{
	data_union_t value;
	GetDataParameter(_paramInfo, &value);
	return value.b;
}

static void GetToggleableParmaeterTextValue(data_param_info_t* _paramInfo, char* txt)
{
	data_union_t value;
	GetDataParameter(_paramInfo, &value);
	btoa_custom(value.b, txt);
}

static lv_color_t GetToggleableParmaeterColor(data_param_info_t* _paramInfo)
{
	return GetToggleableParameterValue(_paramInfo) ? lvColorStore.on : lvColorStore.off;
}

static lv_obj_t* CreateToggleableContent(lv_obj_t* container, lv_style_t* nameStyle, const char* name)
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
	lv_led_set_color(led, lvColorStore.on);
	lv_obj_add_style(led, &ledStyle, 0);
	lv_led_off(led);

	lv_obj_t* lblValue = lv_label_create_general(container, nameStyle, name, NULL, NULL);
	lv_obj_set_width(lblValue, lv_pct(100));
	lv_obj_align(lblValue, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_long_mode(lblValue, LV_LABEL_LONG_WRAP);

	return led;
}
#if CONTROL_CONFS_COUNT > 0
static lv_obj_t* Control_Cell_Create(lv_obj_t* parent, int index, data_param_info_t* _paramInfo, const char* txtValue)
{
	/******************************* Styling **************************************/
	static lv_style_t paramGridStyle, paramNameStyle, paramValueStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&paramGridStyle, 0, 0, 2, 10, &lvColorStore.btnBg2);
		BSP_Screen_InitLabelStyle(&paramNameStyle, &lv_font_montserrat_14, LV_TEXT_ALIGN_CENTER, &lvColorStore.mediumFont);
		BSP_Screen_InitLabelStyle(&paramValueStyle, &CONTROL_VALUE_FONT, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		init = true;
	}

	/******************************* Styling **************************************/
	int col = index % CONTROL_COL_COUNT;
	int row = index / CONTROL_COL_COUNT;

	if (IsToggleableParameter(_paramInfo))
	{
		lv_obj_t* container = lv_container_create_general(parent, &paramGridStyle, row, col, event_handler, PARAM_TAG(index));
		return CreateToggleableContent(container, &paramValueStyle, _paramInfo->name);
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

static void ControlParams_Create(lv_obj_t* parent, int row, int col)
{
	/******************************* Styling **************************************/
	static lv_style_t paramGridStyle;
	static bool init = false;
	static lv_coord_t rows[CONTROL_ROW_COUNT + 1];
	static lv_coord_t cols[CONTROL_COL_COUNT + 1];
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&paramGridStyle, 5, 5, 0, 5, &lvColorStore.background);
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
		if (!IsToggleableParameter(&controlConfs[i]))
			GetDataParameter_InText(&controlConfs[i], txt, true);
		paramDisp.lblsControl[i] = Control_Cell_Create(grid, i, &controlConfs[i], txt);
	}
}
#endif
#if MONITOR_CONFS_COUNT > 0
static lv_obj_t* Monitor_Cell_Create(lv_obj_t* parent, int index, data_param_info_t* _paramInfo, const char* txtValue)
{
	/******************************* Styling **************************************/
	static lv_style_t paramGridStyle, paramNameStyle, paramValueStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&paramGridStyle, 0, 0, 2, 10, &lvColorStore.background);
		lv_style_set_border_color(&paramGridStyle, lvColorStore.gray);
		BSP_Screen_InitLabelStyle(&paramNameStyle, &lv_font_montserrat_14, LV_TEXT_ALIGN_CENTER, &lvColorStore.mediumFont);
		BSP_Screen_InitLabelStyle(&paramValueStyle, &MONITOR_VALUE_FONT, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		init = true;
	}

	/******************************* Styling **************************************/
	int col = index % MONITOR_COL_COUNT;
	int row = index / MONITOR_COL_COUNT;

	if (IsToggleableParameter(_paramInfo))
	{
		lv_obj_t* container = lv_container_create_general(parent, &paramGridStyle, row, col, NULL, NULL);
		return CreateToggleableContent(container, &paramValueStyle, _paramInfo->name);
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

static void MonitorParams_Create(lv_obj_t* parent, int row, int col)
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
		BSP_Screen_InitGridStyle(&paramGridStyle, 5, 5, 0, 5, &lvColorStore.background);
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
		if (!IsToggleableParameter(&monitorConfs[i]))
			GetDataParameter_InText(&monitorConfs[i], txt, true);
		paramDisp.lblsMonitor[i] = Monitor_Cell_Create(grid, i, &monitorConfs[i], txt);
	}
}
#endif
/**
 * @brief Creates the application dependent area of the main screen.
 * @param parent Parent lv_obj_t of the device.
 * @param row Row index for the item.
 * @param col Column index for the item.
 */
void AppControlGrid_Create(lv_obj_t* parent, int row, int col)
{
	/******************************* Styling **************************************/
	static lv_style_t btnsGridStyle;
	static lv_style_t paramValueLblStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&btnsGridStyle, 5, 5, 0, 0, &lvColorStore.background);
		BSP_Screen_InitLabelStyle(&paramValueLblStyle, &lv_font_montserrat_30, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
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
	MonitorParams_Create(grid, 0, 0);
	ControlParams_Create(grid, 1, 0);
#elif CONTROL_CONFS_COUNT > 0
	ControlParams_Create(grid, 0, 0);
#elif MONITOR_CONFS_COUNT > 0
	MonitorParams_Create(grid, 0, 0);
#endif
	/******************************* Parameters *********************************/
}

/**
 * @brief Refreshes the application dependent area of the main screen.
 */
void AppControlGrid_Refresh(void)
{
	char txt[20];
#if CONTROL_CONFS_COUNT > 0
	for (int i = 0; i < CONTROL_CONFS_COUNT; i++)
	{
		// No need to refresh area if nothing changed
		if (isNotFirstRefresh)
		{
			data_union_t value;
			GetDataParameter(&controlConfs[i], &value);
			if (paramDisp.controlVals[i].u32 == value.u32)
				continue;
			paramDisp.controlVals[i].u32 = value.u32;
		}

		// Refresh area
		if (IsToggleableParameter(&controlConfs[i]))
		{
			if (GetToggleableParameterValue(&controlConfs[i]))
				lv_led_on(paramDisp.lblsControl[i]);
			else
				lv_led_off(paramDisp.lblsControl[i]);
		}
		else
		{
			GetDataParameter_InText(&controlConfs[i], txt, true);
			lv_label_set_text(paramDisp.lblsControl[i], txt);
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
			GetDataParameter(&monitorConfs[i], &value);
			if (paramDisp.monitorVals[i].u32 == value.u32)
				continue;
			paramDisp.monitorVals[i].u32 = value.u32;
		}

		// Refresh area
		if (IsToggleableParameter(&monitorConfs[i]))
		{
			if (GetToggleableParameterValue(&monitorConfs[i]))
				lv_led_on(paramDisp.lblsMonitor[i]);
			else
				lv_led_off(paramDisp.lblsMonitor[i]);
		}
		else
		{
			GetDataParameter_InText(&monitorConfs[i], txt, true);
			lv_label_set_text(paramDisp.lblsMonitor[i], txt);
		}
	}
#endif
	isNotFirstRefresh = true;
}

/**
 * @brief Touch detection of the application dependent area of the screen.
 * @return A new screen type if screen needs to be changed based upon the touch response.
 */
screen_type_t AppControlGrid_TouchDetect(void)
{
	uint8_t tagBuff = tag;
	tag = TAG_NONE;
	if (IS_PARAM_TAG(tagBuff))
	{
		int index = GET_PARAM_INDEX(tagBuff);
		char txt[20];
#if CONTROL_CONFS_COUNT > 0
		// Get value and toggle if boolean
		if (controlConfs[index].type == DTYPE_BOOL)
		{
			data_union_t value;
			device_err_t err = GetDataParameter(&controlConfs[index], &value);
			if (err != ERR_OK)
			{
				DisplayMessage(errInfo[err].caption, errInfo[err].desc);
				return SCREEN_NONE;
			}
			value.b = value.b ? false : true;
			err = SetDataParameter(&controlConfs[index], value);
			if (err != ERR_OK)
			{
				DisplayMessage(errInfo[err].caption, errInfo[err].desc);
				return SCREEN_NONE;
			}
		}
		// directly toggle if bit accessible
		else if(controlConfs[index].type == DTYPE_BIT_ACCESS)
		{
			data_union_t value;
			value.bits = BITS_TOGGLE;
			device_err_t err = SetDataParameter(&controlConfs[index], value);
			if (err != ERR_OK)
			{
				DisplayMessage(errInfo[err].caption, errInfo[err].desc);
				return SCREEN_NONE;
			}
		}
		else
		{
			GetDataParameter_InText(&controlConfs[index], txt, false);
			ConfigScreen_LoadParam(&controlConfs[index], txt);
			return SCREEN_CONF;
		}
#endif
	}
	return SCREEN_NONE;
}
/* EOF */
