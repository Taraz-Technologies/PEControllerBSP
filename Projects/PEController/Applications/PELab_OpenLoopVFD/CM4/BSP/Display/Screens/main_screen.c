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

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static lv_obj_t* screen;
static lv_style_t varCellGridStyle, varGridStyle;
static ch_display_t chDisplay[16];
//static disp_var_t* dispVars = NULL;
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

/**
 * @brief Create a cell for measurement display on main grid
 * @param parent
 * @param index
 */
static void MeasurementCell_Create(lv_obj_t * parent, int index)
{
	static lv_style_t cellGridStyle;
	static lv_style_t chNameGridStyle, chValueTypeGridStyle;
	static lv_style_t chNameLblStyle, chReadingTypeLblStyle, chValueLblStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		lv_color_t bgColor = MakeColor(20, 155, 155);
		BSP_Screen_InitGridStyle(&cellGridStyle, 1, 0, 0, 5, &bgColor);
		bgColor = MakeColor(20, 200, 50);
		BSP_Screen_InitGridStyle(&chValueTypeGridStyle, 0, 0, 0, 0, &bgColor);
		bgColor = MakeColor(0, 155, 155);
		BSP_Screen_InitGridStyle(&chNameGridStyle, 0, 0, 0, 0, &bgColor);
		// Initialize the basic grid cell label styles
		BSP_Screen_InitLabelStyle(&chValueLblStyle, &lv_font_montserrat_26, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		//lv_style_set_text_decor(&chValueLblStyle, LV_TEXT_DECOR_UNDERLINE);
		BSP_Screen_InitLabelStyle(&chReadingTypeLblStyle, &lv_font_montserrat_14, LV_TEXT_ALIGN_RIGHT, &lvColorStore.darkFont);
		lv_style_set_pad_left(&chReadingTypeLblStyle, 2);
		BSP_Screen_InitLabelStyle(&chNameLblStyle, &lv_font_montserrat_22, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		init = true;
	}

	int col = index % 4;
	int row = index / 4;

	ch_display_t* disp = &chDisplay[index];

	// Set main grid element
	//static lv_coord_t cols[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rows[] = {LV_GRID_FR(5), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * grid = lv_grid_create_general(parent, singleRowCol, rows, &cellGridStyle, NULL, event_handler, (void*)index);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

	// Set value and type portion
	//static lv_coord_t colsValueType[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsValueType[] = {LV_GRID_FR(4), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * gridValueType = lv_grid_create_general(grid, singleRowCol, rowsValueType, &chValueTypeGridStyle, NULL, event_handler, (void*)index);
	lv_obj_set_grid_cell(gridValueType, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	char txtVal[10];
	const char* txtRead = NULL;
	txtRead = measureTxts[(uint8_t)chDispMeasures.disp[index].type];
	ftoa_custom(0, txtVal, 4, 1);

	disp->lblValue = lv_label_create_general(gridValueType, &chValueLblStyle, txtVal, event_handler, (void*)index);
	lv_obj_set_grid_cell(disp->lblValue, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(disp->lblValue);

	disp->lblReading = lv_label_create_general(gridValueType, &chReadingTypeLblStyle, txtRead, event_handler, (void*)index);
	lv_obj_set_grid_cell(disp->lblReading, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
	lv_obj_center(disp->lblReading);

	// set the name portion
	static lv_coord_t colsName[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsName[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * gridName = lv_grid_create_general(grid, colsName, rowsName, &chNameGridStyle, NULL, event_handler, (void*)index);
	lv_obj_set_grid_cell(gridName, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

	lv_obj_t* lblName = lv_label_create_general(gridName, &chNameLblStyle, chDispMeasures.disp[index].chName, event_handler, (void*)index);
	lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(lblName);
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
	lv_obj_t* grid = lv_grid_create_general(parent, cols, rows, &lvStyleStore.thinMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	// create all cells
	for (int i = 0; i < 16; i++)
		MeasurementCell_Create(grid, i);
}

/**
 * @brief
 * @param parent
 * @param var
 * @param index
 */
static void VariableCell_Create(lv_obj_t* parent, disp_var_t* var, int index)
{
	//	// Set main grid element
	//	static lv_coord_t colsMain[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	//	static lv_coord_t rowsMain[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	//	lv_obj_t * grid = lv_grid_create_general(parent, colsMain, rowsMain, &varCellGridStyle, NULL, NULL, NULL);
	//	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, index, 1);
	//
	//	lv_obj_t* lblName = lv_label_create_general(grid, &chValueLblStyle, var->name, NULL, NULL);
	//	lv_obj_set_grid_cell(lblName, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	//	lv_obj_center(lblName);
	//
	//	char txt[15];
	//	int i = ftoa_custom(*((float*)var->value), txt, 4, 1);
	//	InsertUnit(txt, var->unit, i);
	//
	//	var->lbl = lv_label_create_general(grid, &chValueLblStyle, txt, NULL, NULL);
	//	lv_obj_set_grid_cell(var->lbl, LV_GRID_ALIGN_END, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	//	lv_obj_center(var->lbl);
}

static void ControlVariables_Create(lv_obj_t* parent)
{
	static lv_style_t gridStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&gridStyle, 0, 2, 0, 0, &lvColorStore.background);
		init = true;
	}

	// create monitoring grid
	static lv_coord_t cols[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rows[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, cols, rows, &gridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	// create all cells
	for (int i = 0; i < 16; i++)
		MeasurementCell_Create(grid, i);
}

static void StartStopControl_Create(lv_obj_t* parent)
{
	static lv_style_t gridStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitGridStyle(&gridStyle, 5, 5, 0, 0, &lvColorStore.background);
		init = true;
	}
	static lv_coord_t cols[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, cols, singleRowCol, &gridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 2, 1);

	lv_obj_t* startBtn = lv_btn_create_general(grid, &lvStyleStore.defaultBtn, NULL, "Start", NULL, NULL);
	lv_obj_set_grid_cell(startBtn, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	lv_obj_t* stopBtn = lv_btn_create_general(grid, &lvStyleStore.defaultBtn, NULL, "Stop", NULL, NULL);
	lv_obj_set_grid_cell(stopBtn, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
}

static void ControlGrid_Create(lv_obj_t* parent)
{
	// create control grid
	static lv_coord_t rows[] = {200, 200, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	static lv_coord_t colsVar[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsVar[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* varGrid = lv_grid_create_general(grid, colsVar, rowsVar, &varGridStyle, NULL, NULL, NULL);
	lv_obj_set_grid_cell(varGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	StartStopControl_Create(grid);

	/*
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
		VariableCell_Create(varGrid, var, index++);
		var = var->next;
	}
	*/

	//lv_obj_t* btn = lv_btn_create_general(grid, NULL, &chValueLblStyle, "Click!", NULL, NULL);
	//lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
	//lv_obj_set_size(btn, 180, 70);
}

/**
 * @brief Initialize screen related styles
 */
static void InitStyles(void)
{
	// Initialize the basic grid cell container styles
	lv_color_t bgColor = MakeColor(0, 155, 155);
	BSP_Screen_InitGridStyle(&varCellGridStyle, 3, 0, 0, 10, &bgColor);
	BSP_Screen_InitGridStyle(&varGridStyle, 3, 3, 0, 10, &lvColorStore.background);
}

void MainScreen_Init(void)
{
	// Make the screen
	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	static lv_coord_t colsScreen[] = {LV_GRID_FR(MONITORING_VIEW_WIDTH), LV_GRID_FR(CONTROL_VIEW_WIDTH), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* screenGrid = lv_grid_create_general(screen, colsScreen, singleRowCol, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_size(screenGrid, 800, 480);

	MeasurementGrid_Create(screenGrid);
	ControlGrid_Create(screenGrid);
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
		if (chDispMeasures.isUpdated)
		{
			chDispMeasures.isUpdated = false;
			for (int i = 0; i < 14; i++)
			{
				param_measure_type_t type = chDispMeasures.disp[i].type;
				float* stats = ((float*)&chDispMeasures.disp[i].stats);
				char txt[10];
				int len = ftoa_custom(stats[(uint8_t)type], txt, 4, 1);
				strcat_custom(txt, chDispMeasures.disp[i].chUnit, len, false);
				lv_label_set_text(chDisplay[i].lblValue, txt);
			}
		}
		lv_label_set_text_fmt(chDisplay[14].lblValue, "%d", XDisp);
		lv_label_set_text_fmt(chDisplay[15].lblValue, "%d", YDisp);
	}
}

#endif
/* EOF */
