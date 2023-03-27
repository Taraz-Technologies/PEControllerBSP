/**
 ********************************************************************************
 * @file    	main_screen.c
 * @author 		Waqas Ehsan Butt
 * @date    	Mar 9, 2023
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
#include "main_screen.h"
#include "screen_styles.h"
#include "screen_data.h"
#include "stdlib.h"
#include "UtilityLib.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef enum
{
	VAR_BOOL,
	VAR_STRING,
	VAR_FLOAT,
	VAR_INT,
	VAR_UINT,
} var_type_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct _disp_var_t_
{
	const char* name;
	void* value;
	const char* unit;
	bool isCreated;
	var_type_t type;
	lv_obj_t* lbl;
	struct _disp_var_t_* next;
} disp_var_t;
/********************************************************************************
 * Static Variables
 *******************************************************************************/
static lv_obj_t* screen;
static lv_style_t cellGridStyle;
static lv_style_t chNameGridStyle, chValueTypeGridStyle, dispGridStyle, dispOuterGridStyle;
static lv_style_t chNameLblStyle, chReadingTypeLblStyle, chValueLblStyle;
static ch_display_t chDisplay[16];
static disp_var_t* dispVars = NULL;
static bool isActive;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
uint16_t XDisp;
uint16_t YDisp;
volatile int screenID = 0;
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
	//uint8_t index = (uint8_t)(e->user_data);
	screenID = 1;
}

static void MonitoringCell_Create(lv_obj_t * parent, int index)
{
	int col = index % 4;
	int row = index / 4;

	ch_display_t* disp = &chDisplay[index];

	// Set main grid element
	static lv_coord_t colsMain[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsMain[] = {LV_GRID_FR(5), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * gridMain = lv_grid_create_general(parent, colsMain, rowsMain, &cellGridStyle, NULL, event_handler, (void*)index);
	lv_obj_set_grid_cell(gridMain, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	// Set value and type portion
	static lv_coord_t colsValueType[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsValueType[] = {LV_GRID_FR(4), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * gridValueType = lv_grid_create_general(gridMain, colsValueType, rowsValueType, &chValueTypeGridStyle, NULL, event_handler, (void*)index);
	lv_obj_set_grid_cell(gridValueType, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	char txtVal[10];
	const char* txtRead = NULL;
	if (chDisplayParams[index].srcType == PARAM_SRC_MEASUREMENT)
	{
		txtRead = measureTxts[(uint8_t)chDisplayParams[index].src.measure.type];
		ftoa_custom(chDisplayParams[index].src.measure.value, txtVal, 4, 1);
	}	// configure for other things --TODO--

	disp->lblValue = lv_label_create_general(gridValueType, &chValueLblStyle, txtVal, event_handler, (void*)index);
	lv_obj_set_grid_cell(disp->lblValue, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(disp->lblValue);

	disp->lblReading = lv_label_create_general(gridValueType, &chReadingTypeLblStyle, txtRead, event_handler, (void*)index);
	lv_obj_set_grid_cell(disp->lblReading, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
	lv_obj_center(disp->lblReading);

	// set the name portion
	static lv_coord_t colsName[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsName[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * gridName = lv_grid_create_general(gridMain, colsName, rowsName, &chNameGridStyle, NULL, event_handler, (void*)index);
	lv_obj_set_grid_cell(gridName, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

	disp->lblName = lv_label_create_general(gridName, &chNameLblStyle, chDisplayParams[index].srcName, event_handler, (void*)index);
	lv_obj_set_grid_cell(disp->lblName, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(disp->lblName);
}

static void VariableCell_Create(lv_obj_t* parent, disp_var_t* var, int index)
{
	// Set main grid element
	static lv_coord_t colsMain[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsMain[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * grid = lv_grid_create_general(parent, colsMain, rowsMain, &dispGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, index, 1);

	lv_obj_t* lblName = lv_label_create_general(grid, &chValueLblStyle, var->name, NULL, NULL);
	lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(lblName);

	char txt[15];
	int i = ftoa_custom(*((float*)var->value), txt, 4, 1);

	if (var != NULL)
		txt[i++] = ' ';

	int unitIndex = 0;
	while (var->unit[unitIndex] != 0)
		txt[i++] = var->unit[unitIndex++];

	var->lbl = lv_label_create_general(grid, &chValueLblStyle, txt, NULL, NULL);
	lv_obj_set_grid_cell(var->lbl, LV_GRID_ALIGN_END, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(var->lbl);
}

void MainScreen_Init(void)
{
	// Initialize monitoring cell main grid
	BSP_Screen_InitGridStyle(&cellGridStyle, 2, 0, 0, 0, &lvColorBg);

	// Initialize the basic grid cell container styles
	lv_color_t bgColor = MakeColor(20, 155, 80);
	BSP_Screen_InitGridStyle(&chValueTypeGridStyle, 0, 0, 0, 6, &bgColor);
	bgColor = MakeColor(0, 155, 155);
	BSP_Screen_InitGridStyle(&chNameGridStyle, 0, 0, 0, 6, &bgColor);
	BSP_Screen_InitGridStyle(&dispOuterGridStyle, 3, 3, 0, 10, &lvColorBg);
	bgColor = MakeColor(0, 155, 155);
	BSP_Screen_InitGridStyle(&dispGridStyle, 3, 0, 0, 10, &bgColor);

	// Initialize the basic grid cell label styles
	BSP_Screen_InitLabelStyle(&chValueLblStyle, &lv_font_montserrat_26, LV_TEXT_ALIGN_CENTER, &lvColorDarkFont);
	//lv_style_set_text_decor(&chValueLblStyle, LV_TEXT_DECOR_UNDERLINE);
	BSP_Screen_InitLabelStyle(&chReadingTypeLblStyle, &lv_font_montserrat_14, LV_TEXT_ALIGN_RIGHT, &lvColorDarkFont);
	lv_style_set_pad_left(&chReadingTypeLblStyle, 2);
	BSP_Screen_InitLabelStyle(&chNameLblStyle, &lv_font_montserrat_22, LV_TEXT_ALIGN_CENTER, &lvColorDarkFont);

	// Make the screen
	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	static lv_coord_t colsScreen[] = {LV_GRID_FR(MONITORING_VIEW_WIDTH), LV_GRID_FR(CONTROL_VIEW_WIDTH), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsScreen[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* screenGrid = lv_grid_create_general(screen, colsScreen, rowsScreen, &basicGridStyle, NULL, NULL, NULL);
	lv_obj_set_size(screenGrid, 800, 480);

	// create monitoring grid
	static lv_coord_t colsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* monGrid = lv_grid_create_general(screenGrid, colsMon, rowsMon, &basicGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(monGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	// create all cells
	for (int i = 0; i < 16; i++)
		MonitoringCell_Create(monGrid, i);

	// create control grid
	static lv_coord_t colsCont[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsCont[] = {300, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* contGrid = lv_grid_create_general(screenGrid, colsCont, rowsCont, &basicGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(contGrid, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	static lv_coord_t colsControlDisp[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsControlDisp[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* dispGrid = lv_grid_create_general(contGrid, colsControlDisp, rowsControlDisp, &dispOuterGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(dispGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	static float vals[] = { 380, 50, 25, 10 };
	static disp_var_t vars[4] =
	{
			{ .name = "Vnominal", .value = &vals[0], .type = VAR_FLOAT, .unit = "V", .next = &vars[1] },
			{ .name = "fnominal", .value = &vals[1], .type = VAR_FLOAT, .unit = "Hz", .next = &vars[2] },
			{ .name = "frequired", .value = &vals[2], .type = VAR_FLOAT, .unit = "Hz", .next = &vars[3] },
			{ .name = "fcurrent", .value = &vals[3], .type = VAR_FLOAT, .unit = "Hz", .next = NULL },
	};

	dispVars = vars;
	disp_var_t* var = dispVars;
	int index = 0;
	while (var != NULL)
	{
		VariableCell_Create(dispGrid, var, index++);
		var = var->next;
	}

	lv_obj_t* btn = lv_btn_create_general(contGrid, NULL, &chValueLblStyle, "Click!", NULL, NULL);
	lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
	lv_obj_set_size(btn, 180, 70);
}

void MainScreen_Load(void)
{
	//if(isActive)
	//	return;

	lv_scr_load(screen);
	isActive = true;
}

void MainScreen_Unload(void)
{
	if (isActive)
	{
		isActive = false;
		//lv_obj_del(screen);
	}
}

void MainScreen_Refresh(void)
{
	if (isActive)
	{
		for (int i = 0; i < 14; i++)
		{
			if (chDisplayParams[i].isUpdated)
			{
				chDisplayParams[i].isUpdated = false;
				char txt[10];
				ftoa_custom(chDisplayParams[i].src.measure.value, txt, 4, 1);
				lv_label_set_text(chDisplay[i].lblValue, txt);
			}
		}
		lv_label_set_text_fmt(chDisplay[14].lblValue, "%d", XDisp);
		lv_label_set_text_fmt(chDisplay[15].lblValue, "%d", YDisp);
	}
}

#endif
/* EOF */
