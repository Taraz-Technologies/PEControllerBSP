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
	CONF_SETTINGS,
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
typedef struct
{
	lv_obj_t* container;
	lv_obj_t* value;
} conf_field_info_t;
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
static int unitIndex;
static int typeIndex;
static data_param_group_t* paramGroups;
static int groupCount;
static conf_field_info_t* settingObjs;
static int settingsCount = 0;
static int currentSettingIndex = 0;
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
static void StyleKeypadButtons(lv_obj_t* item)
{
	lv_obj_set_style_bg_color(item, lvColorStore.background, 0);
	lv_obj_set_style_text_color(item, lvColorStore.darkTaraz, 0);
	lv_obj_set_style_bg_color(item, lvColorStore.darkTaraz, LV_PART_ITEMS);
	lv_obj_set_style_text_color(item, lvColorStore.white, LV_PART_ITEMS);
	lv_obj_set_style_border_color(item, lvColorStore.lightTaraz, LV_PART_ITEMS);
	lv_obj_set_style_border_width(item, 2, LV_PART_ITEMS);
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
	lv_btnmatrix_set_map(screenObjs.keyboard, map);
	lv_btnmatrix_set_btn_width(screenObjs.keyboard, 13, 2);
	StyleKeypadButtons(screenObjs.keyboard);
}

static void Close_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
	lv_obj_t * obj = lv_event_get_target(e);
	tag = lv_btnmatrix_get_selected_btn(obj) ? TAG_CANCEL : TAG_OK;
}

static void OkClose_Create(lv_obj_t * parent, int row, int col)
{
	static const char* map[] = {LV_SYMBOL_OK, LV_SYMBOL_CLOSE,
			NULL};

	lv_obj_t* kb = lv_keyboard_create(parent);
	lv_obj_set_grid_cell(kb, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
	lv_btnmatrix_set_map(kb, map);
	lv_obj_set_style_text_font(kb, &lv_font_montserrat_30, 0);
	lv_obj_add_event_cb(kb, Close_Clicked, LV_EVENT_CLICKED, NULL);
	StyleKeypadButtons(kb);
}

static void LeftRight_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
	lv_obj_t * obj = lv_event_get_target(e);
	tag = lv_btnmatrix_get_selected_btn(obj) ? TAG_RIGHT : TAG_LEFT;
}

static void LeftRight_Create(lv_obj_t * parent, int row, int col)
{
	static const char* map[] = {LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT,
			NULL};

	lv_obj_t* kb = lv_keyboard_create(parent);
	lv_obj_set_grid_cell(kb, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
	lv_btnmatrix_set_map(kb, map);
	lv_obj_set_style_text_font(kb, &lv_font_montserrat_30, 0);
	lv_obj_add_event_cb(kb, LeftRight_Clicked, LV_EVENT_CLICKED, NULL);
	StyleKeypadButtons(kb);
}

static void StaticForm_Create(lv_obj_t * parent)
{
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
	static lv_coord_t cols[] = {120, LV_GRID_FR(1), 120, LV_GRID_TEMPLATE_LAST};
	lv_obj_t* nameContainerArea = lv_grid_create_general(parent, cols, singleRowCol, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(nameContainerArea, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_t * nameContainer = lv_container_create_general(nameContainerArea, &nameContainerStyle, 0, 1, NULL, NULL);
	screenObjs.paramName = lv_label_create_general(nameContainer, &lblStyleName, "", NULL, NULL);
	lv_obj_align(screenObjs.paramName, LV_ALIGN_CENTER, 0, 0);
	LeftRight_Create(nameContainerArea, 0, 2);

	static lv_coord_t rows[] = {LV_GRID_FR(1), 80, LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.thinMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

	screenObjs.itemContainer = lv_grid_create_general(grid, singleRowCol, singleRowCol, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(screenObjs.itemContainer, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	OkClose_Create(grid, 1, 0);
}

static void CreateParamGrid(void)
{
	if (screenObjs.paramGrid != NULL)
	{
		lv_obj_del(screenObjs.paramGrid);
		screenObjs.paramGrid = NULL;
	}

	static lv_style_t gridStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&gridStyle, 12, 12, 0, 0, &lvColorStore.background);
		init = true;
	}
	static lv_coord_t rows[] = {FIELD_ROW_HEIGHT, FIELD_ROW_HEIGHT, FIELD_ROW_HEIGHT,
			FIELD_ROW_HEIGHT, FIELD_ROW_HEIGHT, FIELD_ROW_HEIGHT,
			LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	screenObjs.paramGrid = lv_grid_create_general(screenObjs.itemContainer, singleRowCol, rows, &gridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(screenObjs.paramGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
}

static void SetPageTitle(const char* name)
{
	if(screenObjs.paramName != NULL)
		lv_label_set_text(screenObjs.paramName, name);
}

static lv_obj_t* AddField(const char* name, const char* value, bool isWriteable, lv_obj_t** container, int row, lv_event_cb_t e, void * eventData)
{
	static lv_ta_field_data_t field =
	{
			.colorFieldName = false,
			.colWidths = { LV_GRID_FR(1), FIELD_VALUE_WIDTH }
	};
	field.nameTxt = name;
	field.valueTxt = value;
	field.isTextArea = isWriteable;
	lv_default_text_field(screenObjs.paramGrid, &field, row, 0, e, eventData);
	*container = field.container;
	return field.valueField;
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

	CreateParamGrid();
	SetPageTitle(dispMeasures.chNames[measurementIndex]);
	char txt[12];
	// Type
	typeIndex = (uint8_t)dispMeasures.chMeasures[measurementIndex].type;
	measureObjs.type = AddField("Type", measureTxts[typeIndex], false, NULL, 0, Type_Toggle, NULL);
	// Units
	unitIndex = (uint8_t)dispMeasures.adcInfo->units[measurementIndex];
	measureObjs.unit = AddField("Unit", measureTxts[typeIndex], false, NULL, 1, Unit_Toggle, NULL);
	// Sensitivity
	(void)ftoa_custom(ADC_INFO.sensitivity[measurementIndex], txt, 7, 4);
	screenObjs.focusItem = measureObjs.sensitivity = AddField("Sensitivity", txt, true, NULL, 2, TextArea_Clicked, NULL);
	(void)ftoa_custom(ADC_INFO.offsets[measurementIndex], txt, 7, 4);
	measureObjs.offset = AddField("Offset", txt, true, NULL, 3, TextArea_Clicked, NULL);
	(void)ftoa_custom(ADC_INFO.freq[measurementIndex], txt, 7, 1);
	measureObjs.freq = AddField("Fundamental Frequency", txt, true, NULL, 4, TextArea_Clicked, NULL);

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
	confType = CONF_PARAM;

	CreateParamGrid();
	SetPageTitle(_paramInfo->name);
	screenObjs.focusItem = varObjs.lblValue = AddField("Value", val, true, NULL, 0, TextArea_Clicked, NULL);
	lv_keyboard_set_textarea(screenObjs.keyboard, screenObjs.focusItem);
	paramInfo = _paramInfo;
}

static void EditableParam_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
	screenObjs.focusItem = lv_event_get_target(e);
	lv_keyboard_set_textarea(screenObjs.keyboard, screenObjs.focusItem);
}

static void ToggleableParam_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
}

static bool IsEditableParamField(data_param_info_t* param)
{
	return param->type == DTYPE_BOOL ? false : true;
}

static void AddParamField(int row, data_param_info_t* param, conf_field_info_t* confField)
{
	char value[20];
	GetDataParameter_InText(param, value, false);

	if (IsEditableParamField(param) == false)
		confField->value = AddField(param->name, value, false, &confField->container, row, ToggleableParam_Clicked, param);
	else
		confField->value = AddField(param->name, value, true, &confField->container, row, EditableParam_Clicked, param);
}

static void HideAllSettings(void)
{
	if (settingObjs == NULL)
		return;

	for (int i = 0; i < settingsCount; i++)
		lv_obj_add_flag(settingObjs[i].container, LV_OBJ_FLAG_HIDDEN);
}

static void ShowSpecificSettingWindow(int index)
{
	HideAllSettings();

	int firstSetting = 0;
	for (int i = 0; i < index; i++)
		firstSetting += paramGroups[i].paramCount;

	SetPageTitle(paramGroups[index].title);

	currentSettingIndex = index;
	for (int i = 0; i < paramGroups[index].paramCount; i++)
		lv_obj_clear_flag(settingObjs[firstSetting++].container, LV_OBJ_FLAG_HIDDEN);
}

static void FillAllSettings(void)
{
	if (settingObjs != NULL)
		free(settingObjs);
	settingsCount = 0;

	for (int i = 0; i < groupCount; i++)
		settingsCount += paramGroups[i].paramCount;

	settingObjs = (conf_field_info_t*) malloc(sizeof(conf_field_info_t) * settingsCount);

	// Create and hide all
	int index = 0;
	for (int i = 0; i < groupCount; i++)
	{
		for (int j = 0; j < paramGroups[i].paramCount; j++)
		{
			AddParamField(j, paramGroups[i].paramPointers[j], &settingObjs[index]);
			lv_obj_add_flag(settingObjs[index++].container, LV_OBJ_FLAG_HIDDEN);
		}
	}
}

void ConfigScreen_LoadSettings(data_param_group_t* _paramGroups, int _groupCount)
{
	if (screenObjs.itemContainer == NULL)
		return;
	confType = CONF_SETTINGS;

	paramGroups = _paramGroups;
	groupCount = _groupCount;
	CreateParamGrid();
	FillAllSettings();
	ShowSpecificSettingWindow(0);
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
		if (settingObjs != NULL)
			free(settingObjs);
		settingObjs = NULL;
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

static device_err_t UpdateSettings(void)
{
	if (paramGroups == NULL || groupCount <= 0 || settingObjs == NULL || settingsCount < 0)
		return ERR_OK;

	int index = 0;
	for (int i = 0; i < groupCount; i++)
	{
		for (int j = 0; j < paramGroups[i].paramCount; j++)
		{
			data_param_info_t* param = paramGroups[i].paramPointers[i];
			device_err_t err = SetDataParameter_FromText(param, IsEditableParamField(param) == false ? lv_label_get_text(settingObjs[index++].value) : lv_textarea_get_text(settingObjs[index++].value));
			if (err != ERR_OK)
				return err;
		}
	}

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
		else if (tagBuff == TAG_RIGHT && confType == CONF_SETTINGS)
			ShowSpecificSettingWindow(currentSettingIndex < groupCount - 1 ? currentSettingIndex + 1 : 0);
		else if (tagBuff == TAG_LEFT && confType == CONF_SETTINGS)
			ShowSpecificSettingWindow(currentSettingIndex = currentSettingIndex > 0 ? currentSettingIndex - 1 : groupCount - 1);
		else if (tagBuff == TAG_OK)
		{
			device_err_t err = ERR_OK;
			if (confType == CONF_MEASURE)
				err = UpdateMeasurementSettings();
			else if (confType == CONF_PARAM)
				err = UpdateParameter();
			else if (confType == CONF_SETTINGS)
				err = UpdateSettings();
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
