/**
 ********************************************************************************
 * @file    	config_screen.c
 * @author 		Waqas Ehsan Butt
 * @date    	Mar 14, 2023
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
#if 1
#include "config_screen.h"
#include "screen_styles.h"
#include "UtilityLib.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef struct
{
	lv_obj_t* itemGrid;
	lv_obj_t* focusItem;
	lv_obj_t* keyboard;
} screen_objs_t;
/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static lv_obj_t* screen;
static lv_obj_t* labelx;
static bool isActive;
extern volatile int screenID;
static screen_objs_t screenObjs = {0};

/********************************************************************************
 * Global Variables
 *******************************************************************************/
static const char* numTxts[] =
{
		"7", "8", "9",
		"4", "5", "6",
		"1", "2", "3",
		"-", "0", ".",
};
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
extern uint16_t YDisp;
/********************************************************************************
 * Code
 *******************************************************************************/
static void CloseForm_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
	screenID = 0;
}

static void DeleteBtn_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
	lv_obj_t* obj =  screenObjs.focusItem;
	if (obj != NULL)
	{
		if (e->user_data == NULL)
			lv_textarea_del_char(obj);
		else
			lv_textarea_set_text(obj, "");
	}
}

static void NumericBtn_Clicked(lv_event_t * e)
{
	if (!isActive)
		return;
	lv_obj_t* obj =  screenObjs.focusItem;
	if (obj != NULL)
		lv_textarea_add_text(obj, e->user_data);
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
#if 0
	static lv_style_t btnLblStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitLabelStyle(&btnLblStyle, &lv_font_montserrat_40, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		init = true;
	}

	// create buttons grid
	static lv_coord_t colRows[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, colRows, colRows, &lvStyleStore.thinMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	// create all numeric cells
	for (int i = 0; i < 12; i++)
	{
		lv_obj_t* btn = lv_btn_create_general(grid, &lvStyleStore.btn2, &btnLblStyle, numTxts[i], NumericBtn_Clicked, (void*)numTxts[i]);
		lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, i % 3, 1, LV_GRID_ALIGN_STRETCH, i / 3, 1);
	}

	// Create the deletion buttons
	lv_obj_t* ceBtn = lv_btn_create_general(grid, &lvStyleStore.btn2, &btnLblStyle, "CE", DeleteBtn_Clicked, NULL);
	lv_obj_set_grid_cell(ceBtn, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_STRETCH, 0, 2);
	lv_obj_t* cBtn = lv_btn_create_general(grid, &lvStyleStore.btn2, &btnLblStyle, "C", DeleteBtn_Clicked, &btnLblStyle);
	lv_obj_set_grid_cell(cBtn, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_STRETCH, 2, 2);
#else
	static const char* map[] = {"7", "8", "9" , LV_SYMBOL_KEYBOARD ,"\n",
			"4", "5", "6" , LV_SYMBOL_BACKSPACE ,"\n",
			"1", "2", "3" , LV_SYMBOL_LEFT ,"\n",
			"+/-", "0", "." , LV_SYMBOL_RIGHT,
			NULL};
	screenObjs.keyboard = lv_keyboard_create(parent);
	lv_obj_set_grid_cell(screenObjs.keyboard, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	//lv_keyboard_set_mode(screenObjs.keyboard, LV_KEYBOARD_MODE_NUMBER);
	lv_btnmatrix_set_map(screenObjs.keyboard, map);
	lv_obj_set_style_bg_color(screenObjs.keyboard, lvColorStore.background, 0);
#endif
}

static void FormActionBtns_Create(lv_obj_t * parent)
{
#if 1
	const lv_coord_t btnWidth = 180;
	static lv_style_t btnLblStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitLabelStyle(&btnLblStyle, &lv_font_montserrat_26, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		init = true;
	}

	static lv_coord_t cols[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(parent, cols, singleRowCol, &lvStyleStore.thickMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

	lv_obj_t* okBtn = lv_btn_create_general(grid, &lvStyleStore.btn2, &btnLblStyle, "Save", CloseForm_Clicked, &btnLblStyle);
	lv_obj_set_grid_cell(okBtn, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_width(okBtn, btnWidth);
	lv_obj_t* cancelBtn = lv_btn_create_general(grid, &lvStyleStore.btn2, &btnLblStyle, "Cancel", CloseForm_Clicked, NULL);
	lv_obj_set_grid_cell(cancelBtn, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_width(cancelBtn, btnWidth);
#else
	static const char* map[] = {"Save", "Cancel", NULL};
	lv_obj_t * btnm = lv_btnmatrix_create(parent);
	lv_obj_set_grid_cell(btnm, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
	lv_btnmatrix_set_map(btnm, map);
#endif
}

static void StaticForm_Create(lv_obj_t * parent)
{
	static lv_coord_t rows[] = {LV_GRID_FR(1), 80, LV_GRID_TEMPLATE_LAST};
	screenObjs.itemGrid = lv_grid_create_general(parent, singleRowCol, rows, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(screenObjs.itemGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	FormActionBtns_Create(screenObjs.itemGrid);
}

void ConfigScreen_LoadMeasurement(int measurementIndex)
{
	if (screenObjs.itemGrid == NULL)
		return;
	static lv_style_t lblStyle;
	static bool init = false;
	// initialize styles once
	if (!init)
	{
		BSP_Screen_InitLabelStyle(&lblStyle, NULL, LV_TEXT_ALIGN_CENTER, &lvColorStore.darkFont);
		init = true;
	}

	static lv_coord_t rows[] = {60, 60, 60, 60, 60, 60, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t cols[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* grid = lv_grid_create_general(screenObjs.itemGrid, cols, rows, &lvStyleStore.thickMarginGrid, NULL, NULL, NULL);
	lv_obj_set_grid_cell(grid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

	disp_ch_measure_t* measure = chDispMeasures.disp + measurementIndex;
	screenObjs.focusItem = lv_create_textfield_display(grid, &lblStyle, &lvStyleStore.defaultTextArea, "Name", measure->chName, TextArea_Clicked, NULL, 0);

	lv_obj_t * dd = lv_dropdown_create(grid);
	lv_dropdown_set_options_static(dd, measureTxts[0]);
	for (int i = 1; i < MEASURE_COUNT; i++)
		lv_dropdown_add_option(dd, measureTxts[i], i);
	lv_dropdown_set_selected(dd, measure->type);
	lv_obj_clear_flag(dd, LV_OBJ_FLAG_CLICK_FOCUSABLE);

	lv_create_field(grid, &lblStyle, "Type", dd, 1);

	lv_create_textfield_display(grid, &lblStyle, &lvStyleStore.defaultTextArea, "Sensitivity", "0", TextArea_Clicked, NULL, 2);
	lv_create_textfield_display(grid, &lblStyle, &lvStyleStore.defaultTextArea, "Offset", "0", TextArea_Clicked, NULL, 3);
}

void ConfigScreen_LoadVariable(void)
{
	//static const char * btns[] ={"Close", ""};
	//lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Error!", "Encountered an error while setting the required value. Do you want to set the value so that it can not be done in by anything else", btns, false);
	//lv_obj_center(mbox1);
}

void ConfigScreen_Init(void)
{
	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	static lv_coord_t colsScreen[] = {LV_GRID_FR(1), 350, LV_GRID_TEMPLATE_LAST};
	lv_obj_t* screenGrid = lv_grid_create_general(screen, colsScreen, singleRowCol, &lvStyleStore.defaultGrid, NULL, NULL, NULL);
	lv_obj_set_size(screenGrid, 800, 480);

	Numpad_Create(screenGrid);
	StaticForm_Create(screenGrid);
	ConfigScreen_LoadMeasurement(0);
	ConfigScreen_LoadVariable();
	//ConfigScreen_LoadVariable();
}



void ConfigScreen_Load(void)
{
	//if (isActive)
	//	return;


	// create control grid --TODO--
	lv_scr_load(screen);
	isActive = true;
}

void ConfigScreen_Unload(void)
{
	if (isActive)
	{
		//lv_obj_del(screen);
		isActive = false;
	}
}

#endif
/* EOF */
