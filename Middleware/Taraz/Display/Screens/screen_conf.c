/**
 ********************************************************************************
 * @file    	screen_conf.c
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
#include "pecontroller_adc.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define FIELD_ROW_HEIGHT				(45)
#define FIELD_VALUE_WIDTH				(120)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef enum
{
	CONF_MEASURE,
	CONF_PARAM,
	CONF_COUNT
} conf_type_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	lv_obj_t* itemContainer;
	lv_obj_t* focusItem;
	lv_obj_t* keyboard;
	lv_obj_t* paramGrid;
	lv_obj_t* paramName;
} screen_objs_t;
typedef struct
{
	lv_obj_t* type;
	lv_obj_t* unit;
	lv_obj_t* sensitivity;
	lv_obj_t* offset;
	lv_obj_t* freq;
} measure_objs_t;
typedef struct
{
	lv_obj_t* lblValue;
} var_objs_t;
/********************************************************************************
 * Static Variables
 *******************************************************************************/
static lv_obj_t* screen;
static volatile bool isActive;
static screen_objs_t screenObjs = {0};
static volatile uint8_t tag = TAG_NONE;
static conf_type_t confType = CONF_MEASURE;
static int measurementIndex = 0;
static data_param_info_t* paramInfo = NULL;
static measure_objs_t measureObjs = {0};
static var_objs_t varObjs = {0};
static int unitIndex, typeIndex;
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

static void Keypad_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
	lv_obj_t * obj = lv_event_get_target(e);
	lv_keyboard_t * keyboard = (lv_keyboard_t *)obj;
	tag = lv_btnmatrix_get_selected_btn(obj) ? TAG_CANCEL : TAG_OK;
}

static void Type_Toggle(lv_event_t* e)
{
	if (!isActive)
		return;
	if (measureObjs.type != NULL)
	{
		if(++typeIndex >= MEASURE_COUNT)
			typeIndex = 0;
		lv_label_set_text(measureObjs.type, measureTxts[typeIndex]);
	}
}
static void Unit_Toggle(lv_event_t* e)
{
	if (!isActive)
		return;
	if (measureObjs.unit != NULL)
	{
		if(++unitIndex >= UNIT_COUNT)
			unitIndex = 0;
		lv_label_set_text(measureObjs.unit, unitTxts[unitIndex]);
	}
}

static void TextArea_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
	screenObjs.focusItem = lv_event_get_target(e);
	lv_keyboard_set_textarea(screenObjs.keyboard, screenObjs.focusItem);
}

static void Numpad_Create(lv_obj_t * parent)
{
	static const char* map[] = {"7", "8", "9" , LV_SYMBOL_BACKSPACE ,"\n",
			"4", "5", "6" , LV_SYMBOL_LEFT ,"\n",
			"1", "2", "3" , LV_SYMBOL_RIGHT ,"\n",
			"+/-", "0", "." , "",
			NULL};
	screenObjs.keyboard = lv_keyboard_create(parent);
	lv_obj_set_grid_cell(screenObjs.keyboard, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
	lv_obj_set_style_text_font(screenObjs.keyboard, &lv_font_montserrat_26, 0);
	//lv_keyboard_set_mode(screenObjs.keyboard, LV_KEYBOARD_MODE_NUMBER);
	lv_btnmatrix_set_map(screenObjs.keyboard, map);
	lv_btnmatrix_set_btn_width(screenObjs.keyboard, 13, 2);
	lv_obj_set_style_bg_color(screenObjs.keyboard, lvColorStore.background, 0);
	lv_obj_set_style_text_color(screenObjs.keyboard, lvColorStore.darkTaraz, 0);
	lv_obj_set_style_bg_color(screenObjs.keyboard, lvColorStore.darkTaraz, LV_PART_ITEMS);
	lv_obj_set_style_text_color(screenObjs.keyboard, lvColorStore.white, LV_PART_ITEMS);
	lv_obj_set_style_border_color(screenObjs.keyboard, lvColorStore.lightTaraz, LV_PART_ITEMS);
	lv_obj_set_style_border_width(screenObjs.keyboard, 2, LV_PART_ITEMS);
}

static void OkClose_Create(lv_obj_t * parent, int row, int col)
{
	static const char* map[] = {LV_SYMBOL_OK, LV_SYMBOL_CLOSE,
								NULL};
	static lv_style_t btnStyle;

	lv_obj_t* kb = lv_keyboard_create(parent);
	lv_obj_set_grid_cell(kb, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
	lv_btnmatrix_set_map(kb, map);
	lv_obj_set_style_bg_color(kb, lvColorStore.background, 0);
	lv_obj_set_style_text_color(kb, lvColorStore.lightTaraz, 0);
	lv_obj_set_style_bg_color(kb, lvColorStore.darkTaraz, LV_PART_ITEMS);
	lv_obj_set_style_text_color(kb, lvColorStore.white, LV_PART_ITEMS);
	lv_obj_set_style_border_color(kb, lvColorStore.lightTaraz, LV_PART_ITEMS);
	lv_obj_set_style_border_width(kb, 2, LV_PART_ITEMS);
	lv_obj_set_style_text_font(kb, &lv_font_montserrat_30, 0);
	//lv_keyboard_def_event_cb()
	lv_obj_add_event_cb(kb, Keypad_Clicked, LV_EVENT_CLICKED, NULL);
}

static void StaticForm_Create(lv_obj_t * parent)
{
	const lv_coord_t btnWidth = 180;
	static lv_style_t btnLblStyle;
	static lv_style_t lblStyleName;
	static lv_style_t nameContainerStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitLabelStyle(&btnLblStyle, &lv_font_montserrat_26, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		BSP_Screen_InitLabelStyle(&lblStyleName, &lv_font_montserrat_30, LV_TEXT_ALIGN_CENTER, &lvColorStore.white);
		BSP_Screen_InitGridStyle(&nameContainerStyle, 0, 0, 2, 10, &lvColorStore.background);
		lv_style_set_border_color(&nameContainerStyle, lvColorStore.gray);
		init = true;
	}

	// name and container for configuration data
	lv_obj_t* nameContainerArea = lv_grid_create_general(parent, singleRowCol, singleRowCol, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(nameContainerArea, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_t * nameContainer = lv_container_create_general(nameContainerArea, &nameContainerStyle, 0, 0, NULL, NULL);
	screenObjs.paramName = lv_label_create_general(nameContainer, &lblStyleName, "", NULL, NULL);
	lv_obj_align(screenObjs.paramName, LV_ALIGN_CENTER, 0, 0);

	static lv_coord_t rows[] = {LV_GRID_FR(1), 80, LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.thinMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

	screenObjs.itemContainer = lv_grid_create_general(grid, singleRowCol, singleRowCol, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(screenObjs.itemContainer, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	OkClose_Create(grid, 1, 0);
	/*
	// buttons
	static lv_coord_t cols[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* gridBtns = lv_grid_create_general(grid, cols, singleRowCol, &lvStyleStore.thickMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(gridBtns, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
	/*

	lv_obj_t* okBtn = lv_btn_create_general(gridBtns, &lvStyleStore.btn2, &btnLblStyle, "Save", Close_Clicked, TAG_ATTACH(TAG_OK));
	lv_obj_set_grid_cell(okBtn, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_width(okBtn, btnWidth);
	lv_obj_t* cancelBtn = lv_btn_create_general(gridBtns, &lvStyleStore.btn2, &btnLblStyle, "Cancel", Close_Clicked, TAG_ATTACH(TAG_CANCEL));
	lv_obj_set_grid_cell(cancelBtn, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_width(cancelBtn, btnWidth);
	*/
}

/**
 * @brief Load the relevant fields and configurations for the desired measurement channel
 * @note Make sure to call this function before switching to the configuration screen
 * @param _measurementIndex Channel index for which the measurement configuration is required. Starts from index 0
 */
void ConfigScreen_LoadMeasurement(int _measurementIndex)
{
	if (screenObjs.itemContainer == NULL)
		return;
	measurementIndex = _measurementIndex;
	confType = CONF_MEASURE;
	static lv_style_t gridStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&gridStyle, 12, 12, 0, 0, &lvColorStore.background);
		init = true;
	}

	if (screenObjs.paramGrid != NULL)
	{
		lv_obj_del(screenObjs.paramGrid);
		screenObjs.paramGrid = NULL;
	}

	static lv_coord_t rows[] = {FIELD_ROW_HEIGHT, FIELD_ROW_HEIGHT, FIELD_ROW_HEIGHT,
			FIELD_ROW_HEIGHT, FIELD_ROW_HEIGHT, FIELD_ROW_HEIGHT,
			LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	screenObjs.paramGrid = lv_grid_create_general(screenObjs.itemContainer, singleRowCol, rows, &gridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(screenObjs.paramGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	if(screenObjs.paramName != NULL)
		lv_label_set_text(screenObjs.paramName, dispMeasures.chNames[measurementIndex]);
	lv_ta_field_data_t field =
	{
			.isTextArea = false,
			.colorFieldName = false,
			.colWidths = { LV_GRID_FR(1), FIELD_VALUE_WIDTH }
	};
	field.nameTxt = "Type";
	field.valueTxt = measureTxts[(uint8_t)dispMeasures.chMeasures[measurementIndex].type];
	lv_default_text_field(screenObjs.paramGrid, &field, 0, 0, Type_Toggle, NULL);
	measureObjs.type = field.valueField;
	field.nameTxt = "Units";
	field.valueTxt = unitTxts[(uint8_t)dispMeasures.adcInfo->units[measurementIndex]];
	lv_default_text_field(screenObjs.paramGrid, &field, 1, 0, Unit_Toggle, NULL);
	measureObjs.unit = field.valueField;
	char txt[12];
	field.isTextArea = true;
	field.valueTxt = txt;
	(void)ftoa_custom(ADC_INFO.sensitivity[measurementIndex], txt, 7, 4);
	field.nameTxt = "Sensitivity";
	lv_default_text_field(screenObjs.paramGrid, &field, 2, 0, TextArea_Clicked, NULL);
	screenObjs.focusItem = measureObjs.sensitivity = field.valueField;
	(void)ftoa_custom(ADC_INFO.offsets[measurementIndex], txt, 7, 4);
	field.nameTxt = "Offset";
	lv_default_text_field(screenObjs.paramGrid, &field, 3, 0, TextArea_Clicked, NULL);
	measureObjs.offset = field.valueField;
	(void)ftoa_custom(ADC_INFO.freq[measurementIndex], txt, 7, 1);
	field.nameTxt = "Fundamental Frequency";
	lv_default_text_field(screenObjs.paramGrid, &field, 4, 0, TextArea_Clicked, NULL);
	measureObjs.freq = field.valueField;

	lv_keyboard_set_textarea(screenObjs.keyboard, screenObjs.focusItem);
}

/**
 * @brief Load the relevant fields and configurations for the desired parameter
 * @note Make sure to call this function before switching to the configuration screen
 * @param _paramInfo Pointer to the desired parameter to be configured.
 * @param val Current value of the parameter
 */
void ConfigScreen_LoadParam(data_param_info_t* _paramInfo, char* val)
{
	if (screenObjs.itemContainer == NULL)
		return;
	static bool init = false;
	confType = CONF_PARAM;
	static lv_style_t gridStyle;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&gridStyle, 12, 12, 0, 0, &lvColorStore.background);
		init = true;
	}

	if (screenObjs.paramGrid != NULL)
	{
		lv_obj_del(screenObjs.paramGrid);
		screenObjs.paramGrid = NULL;
	}

	static lv_coord_t rows[] = {FIELD_ROW_HEIGHT, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	screenObjs.paramGrid = lv_grid_create_general(screenObjs.itemContainer, singleRowCol, rows, &gridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(screenObjs.paramGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	if(screenObjs.paramName != NULL)
		lv_label_set_text(screenObjs.paramName, _paramInfo->name);
	lv_ta_field_data_t field =
	{
			.isTextArea = true,
			.colorFieldName = false,
			.nameTxt = "Value",
			.valueTxt = val,
			.colWidths = { LV_GRID_FR(1), FIELD_VALUE_WIDTH }
	};
	lv_default_text_field(screenObjs.paramGrid, &field, 0, 0, TextArea_Clicked, NULL);
	screenObjs.focusItem = varObjs.lblValue = field.valueField;// lv_create_textfield_display(screenObjs.paramGrid, &lblStyle, &lvStyleStore.defaultTextArea, "Value", val, TextArea_Clicked, NULL, 0);

	lv_keyboard_set_textarea(screenObjs.keyboard, screenObjs.focusItem);
	paramInfo = _paramInfo;
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
		//
		isActive = false;
		lv_obj_del(screenObjs.paramGrid);
		screenObjs.paramGrid = NULL;
	}
}

static device_err_t UpdateMeasurementSettings(void)
{
	float freq, sensitivity, offset;
	bool isValid = atof_custom(lv_textarea_get_text(measureObjs.freq), &freq) &&
			atof_custom(lv_textarea_get_text(measureObjs.sensitivity), &sensitivity) &&
			atof_custom(lv_textarea_get_text(measureObjs.offset), &offset);
	if (isValid == false)
		return ERR_INVALID_TEXT;
	device_err_t err = BSP_ADC_UpdateConfig(dispMeasures.adcInfo, dispMeasures.adcInfo->fs, measurementIndex, freq, sensitivity, offset, (data_units_t)unitIndex);
	dispMeasures.chMeasures[measurementIndex].type = (measure_type_t)typeIndex;
	return err;
}

static device_err_t UpdateParameter(void)
{
	if(paramInfo != NULL)
		return SetDataParameter_FromText(paramInfo, lv_textarea_get_text(varObjs.lblValue));
	return ERR_OK;
}

static screen_type_t Refresh(void)
{
	if (isActive && tag != TAG_NONE)
	{
		uint8_t tagBuff = tag;
		tag = TAG_NONE;
		if (tagBuff == TAG_CANCEL)
			return SCREEN_PREVIOUS;
		if (tagBuff == TAG_OK)
		{
			device_err_t err = ERR_OK;
			if (confType == CONF_MEASURE)
				err = UpdateMeasurementSettings();
			else if (confType == CONF_PARAM)
				err = UpdateParameter();
			if (err == ERR_OK)
				return SCREEN_PREVIOUS;
			DisplayMessage(errInfo[err].caption, errInfo[err].desc);
			return SCREEN_NONE;
		}
	}
	return SCREEN_NONE;
}

/**
 * @brief Initialize screen
 * @param _screen Pointer to fill the screen information
 */
void ConfigScreen_Init(screens_t* _screen)
{
	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	static lv_coord_t colsScreen[] = {LV_GRID_FR(1), 350, LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsScreen[] = {60, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* screenGrid = lv_grid_create_general(screen, colsScreen, rowsScreen, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_size(screenGrid, DISPLAY_WIDTH_RAM, DISPLAY_HEIGHT_RAM);

	Numpad_Create(screenGrid);
	StaticForm_Create(screenGrid);

	_screen->Refresh = Refresh;
	_screen->Load = Load;
	_screen->Unload = Unload;
	_screen->directLayer = NULL;
	_screen->lvglLayer = &defaultLayer;
}

/* EOF */
