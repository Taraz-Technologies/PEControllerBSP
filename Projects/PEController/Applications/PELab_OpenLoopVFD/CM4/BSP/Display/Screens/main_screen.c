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
static lv_style_t screenGridStyle, monGridStyle;
static lv_style_t cellGridStyle;
static lv_style_t chNameGridStyle, chValueTypeGridStyle;
static lv_style_t chNameLblStyle, chReadingTypeLblStyle, chValueLblStyle;
static ch_display_t chDisplay[16];
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/*!
 * @brief shift the provided string
 *
 * @param txt pointer to the shifted txt index
 */
void ShiftStringOneLeft(char* txt)
{
	do
	{
		*txt = *(txt + 1);
		txt++;
	}while(*txt != 0);
}
/*!
 * @brief convert a floating value to string with specified precision, minimum 1 before dot digit required
 *
 * @param txt pointer to the text
 * @param precision precison of the value
 * @param val value to be converted
 * @retval
 */
 char GetStringFromAbsFloat_PreciseDigits(char* txt, int beforeDotDigits, int afterDotDigits, float val)
{
	char* oldLoc = txt;
	int div = 1;
	// only for positive nos
	if (val < 0)
		val *= -1;
	float addVal = 0.5f;

	// get the valid part only
	for(int i = 0; i < afterDotDigits; i++)
		div *= 10;
	addVal /= div;
	val *= div;
	// get valid divider
	for(int i = 0; i < beforeDotDigits - 1; i++)
		div *= 10;

	// discard other digits
	int IntVal = (int)val;

	//
	for(int i = 0; i < beforeDotDigits; i++)
	{
		*txt++ = IntVal / div + '0';
		IntVal %= div;
		div /= 10;
	}

	if(afterDotDigits > 0)
	{
		*txt++ = '.';
		for(int i = 0; i < afterDotDigits; i++)
		{
			*txt++ = IntVal / div + '0';
			IntVal %= div;
			div /= 10;
		}
	}

	*txt = 0;
	return txt - oldLoc;
}
/*!
 * @brief convert a floating value to string with specified precision
 *
 * @param txt pointer to the text
 * @param precision precison of the value
 * @param val value to be converted
 * @retval
 */
 char GetStringFromFloat_PreciseDigits(char* txt, int beforeDotDigits, int afterDotDigits, float val)
{
	if(val < 0)
	{
		char c = GetStringFromAbsFloat_PreciseDigits(&txt[1], beforeDotDigits, afterDotDigits, val);
		bool isNegZero = true;
		for(char a = c; a > 0; a--)
		{
			if(txt[a] != '0' && txt[a] != '.')
			{
				isNegZero = false;
				break;
			}
		}
		if(isNegZero == true)
		{
			ShiftStringOneLeft(txt);
			return c;
		}
		else
		{
			txt[0] = '-';
			return c + 1;
		}
	}
	else
		return GetStringFromAbsFloat_PreciseDigits(txt, beforeDotDigits, afterDotDigits, val);
}

static void MonitoringCell_Create(lv_obj_t * parent, int index)
{
	int col = index % 4;
	int row = index / 4;

	ch_display_t* disp = &chDisplay[index];

	// Set main grid element
	static lv_coord_t colsMain[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsMain[] = {LV_GRID_FR(5), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * gridMain = lv_obj_create(parent);
	lv_obj_set_grid_cell(gridMain, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
	lv_obj_set_grid_dsc_array(gridMain, colsMain, rowsMain);
	lv_obj_set_layout(gridMain, LV_LAYOUT_GRID);
	lv_obj_add_style(gridMain, &cellGridStyle, 0);

	// Set value and type portion
	static lv_coord_t colsValueType[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsValueType[] = {LV_GRID_FR(4), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * gridValueType = lv_obj_create(gridMain);
	lv_obj_set_grid_cell(gridValueType, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_grid_dsc_array(gridValueType, colsValueType, rowsValueType);
	lv_obj_set_layout(gridValueType, LV_LAYOUT_GRID);
	lv_obj_add_style(gridValueType, &chValueTypeGridStyle, 0);

	disp->lblValue = lv_label_create(gridValueType);
	lv_obj_set_grid_cell(disp->lblValue, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(disp->lblValue);
	lv_obj_add_style(disp->lblValue, &chValueLblStyle, 0);

	disp->lblReading = lv_label_create(gridValueType);
	lv_obj_set_grid_cell(disp->lblReading, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
	lv_obj_center(disp->lblReading);
	lv_obj_add_style(disp->lblReading, &chReadingTypeLblStyle, 0);

	// set the name portion
	static lv_coord_t colsName[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsName[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * gridName = lv_obj_create(gridMain);
	lv_obj_set_grid_cell(gridName, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
	lv_obj_set_grid_dsc_array(gridName, colsName, rowsName);
	lv_obj_set_layout(gridName, LV_LAYOUT_GRID);
	lv_obj_add_style(gridName, &chNameGridStyle, 0);

	disp->lblName = lv_label_create(gridName);
	lv_obj_set_grid_cell(disp->lblName, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_obj_center(disp->lblName);
	lv_obj_add_style(disp->lblName, &chNameLblStyle, 0);

	lv_label_set_text(disp->lblName, chDisplayParams[index].srcName);
	if (chDisplayParams[index].srcType == PARAM_SRC_MEASUREMENT)
	{
		lv_label_set_text(disp->lblReading, measureTxts[(uint8_t)chDisplayParams[index].src.measure.type]);
		char txt[10];
		GetStringFromFloat_PreciseDigits(txt, 4, 1, chDisplayParams[index].src.measure.value);
		lv_label_set_text(disp->lblValue, txt);
	}	// configure for other things --TODO--
}

void MainScreen_Init(void)
{
	BSP_Screen_InitGridStyle(&screenGridStyle, 0, 0, 0, lv_palette_lighten(LV_PALETTE_GREY, 3));

	// Initialize monitoring grid
	BSP_Screen_InitGridStyle(&monGridStyle, 0, 0, 0, lv_palette_lighten(LV_PALETTE_GREY, 3));

	// Initialize monitoring cell main grid
	BSP_Screen_InitGridStyle(&cellGridStyle, 2, 0, 0, lv_palette_lighten(LV_PALETTE_GREY, 2));

	// Initialize the basic grid cell container styles
	BSP_Screen_InitGridStyle(&chValueTypeGridStyle, 0, 0, 4, lv_palette_lighten(LV_PALETTE_GREEN, 1));
	BSP_Screen_InitGridStyle(&chNameGridStyle, 0, 0, 4, lv_palette_lighten(LV_PALETTE_BLUE, 1));

	// Initialize the basic grid cell label styles
	BSP_Screen_InitLabelStyle(&chValueLblStyle, &lv_font_montserrat_26, LV_TEXT_ALIGN_CENTER, lv_palette_darken(LV_PALETTE_GREY, 4));
	BSP_Screen_InitLabelStyle(&chReadingTypeLblStyle, &lv_font_montserrat_14, LV_TEXT_ALIGN_RIGHT, lv_palette_darken(LV_PALETTE_GREY, 4));
	lv_style_set_pad_left(&chReadingTypeLblStyle, 2);
	BSP_Screen_InitLabelStyle(&chNameLblStyle, &lv_font_montserrat_22, LV_TEXT_ALIGN_CENTER, lv_palette_darken(LV_PALETTE_GREY, 4));
}

void MainScreen_Load(void)
{
	// create the screen
	screen = lv_obj_create(NULL);

	// create basic grid
	lv_obj_t* screenGrid = lv_obj_create(screen);
	static lv_coord_t colsScreen[] = {LV_GRID_FR(MONITORING_VIEW_WIDTH), LV_GRID_FR(CONTROL_VIEW_WIDTH), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsScreen[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_size(screenGrid, 800, 480);
	lv_obj_set_grid_dsc_array(screenGrid, colsScreen, rowsScreen);
	lv_obj_set_layout(screenGrid, LV_LAYOUT_GRID);
	lv_obj_add_style(screenGrid, &screenGridStyle, 0);

	// create monitoring grid
	lv_obj_t* monGrid = lv_obj_create(screenGrid);
	static lv_coord_t colsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t rowsMon[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_set_grid_cell(monGrid, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_grid_dsc_array(monGrid, colsMon, rowsMon);
	lv_obj_set_layout(monGrid, LV_LAYOUT_GRID);
	lv_obj_add_style(monGrid, &monGridStyle, 0);
	// create all cells
	for (int i = 0; i < 16; i++)
		MonitoringCell_Create(monGrid, i);


	// create control grid --TODO--

	lv_scr_load(screen);
}

void MainScreen_Unload(void)
{
	lv_obj_del(screen);
}



void MainScreen_Refresh(void)
{
	for (int i = 0; i < 16; i++)
	{
		if (chDisplayParams[i].isUpdated)
		{
			chDisplayParams[i].isUpdated = false;
			char txt[10];
			GetStringFromFloat_PreciseDigits(txt, 4, 1, chDisplayParams[i].src.measure.value);
			lv_label_set_text_fmt(chDisplay[i].lblValue, txt);
		}
	}
}

#endif
/* EOF */