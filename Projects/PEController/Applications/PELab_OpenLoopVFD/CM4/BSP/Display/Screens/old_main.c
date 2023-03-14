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
#if 0
#include "logo.h"
#include "lvgl.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	lv_obj_t* lblName;
	lv_obj_t* lblReading;
	lv_obj_t* lblValue;
} ch_display_t;
/********************************************************************************
 * Static Variables
 *******************************************************************************/
static ch_display_t chDisplay[16];
static lv_style_t gridStyle;
static lv_style_t style;
static lv_style_t style1;
static lv_style_t gridStyleBox;

static const char* names[16] = {
		"Vdc1", "Va" , "Vb", "Vc",
		"Vdc2", "Vpv", "V1", "V2",
		"Idc1", "Ia", "Ib", "Ic",
		"Idc2", "I1", "I2", "I3"};
static const char* readings[16] = {
		"RMS", "Avg" , "Pk-Pk", "RMS",
		"Avg" , "Pk-Pk", "NA", "Pk-Pk",
		"RMS", "NA", "NA", "NA",
		"NA", "NA", "NA", "NA"};
/********************************************************************************
 * Global Variables
 *******************************************************************************/
static lv_obj_t * lv_screen_main = NULL;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
static void GUI_InitCellGridStyle(lv_style_t* style)
{
	lv_coord_t pad = 0;
	lv_coord_t border = 0;
	lv_coord_t radius = 3;
	lv_style_init(style);
	lv_style_set_radius(style, radius);
	lv_style_set_pad_row(style, pad);
	lv_style_set_pad_column(style, pad);
	lv_style_set_pad_left(style, pad);
	lv_style_set_pad_right(style, pad);
	lv_style_set_pad_top(style, pad);
	lv_style_set_pad_bottom(style, pad);
	lv_style_set_border_width(style, border);
	lv_style_set_bg_opa(style, LV_OPA_COVER);
	lv_style_set_bg_color(style, lv_palette_lighten(LV_PALETTE_GREEN, 2));
}

static void grid_style_init(void)
{
	lv_style_t* style0 = &gridStyle;
	lv_coord_t pad = 0;
	lv_coord_t border = 3;
	lv_style_init(style0);
	lv_style_set_radius(style0, 0);
	lv_style_set_pad_row(style0, pad);
	lv_style_set_pad_column(style0, pad);
	lv_style_set_pad_left(style0, pad);
	lv_style_set_pad_right(style0, pad);
	lv_style_set_pad_top(style0, pad);
	lv_style_set_pad_bottom(style0, pad);
	lv_style_set_border_width(style0, border);

	lv_style_init(&style);
	lv_style_set_radius(&style, 5);
	lv_style_set_bg_opa(&style, LV_OPA_COVER);
	lv_style_set_bg_color(&style, lv_palette_lighten(LV_PALETTE_BLUE, 2));
	//lv_style_set_border_width(&style, 2);
	//lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_BLUE));
	lv_style_set_pad_all(&style, 0);
	lv_style_set_text_color(&style, lv_palette_main(LV_PALETTE_BLUE));
	//lv_style_set_text_letter_space(&style, 5);
	//lv_style_set_text_line_space(&style, 20);
	//lv_style_set_text_decor(&style, LV_TEXT_DECOR_UNDERLINE);
	lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);


	lv_style_init(&style1);
	lv_style_set_radius(&style1, 5);
	lv_style_set_bg_opa(&style1, LV_OPA_COVER);
	lv_style_set_bg_color(&style1, lv_palette_lighten(LV_PALETTE_GREEN, 2));
	lv_style_set_pad_all(&style1, 0);
	lv_style_set_text_align(&style1, LV_TEXT_ALIGN_CENTER);

	GUI_InitCellGridStyle(&gridStyleBox);
}


static void draw_ch_data(lv_obj_t * grid, int index, const char* name, const char* reading, char* value)
{
	int col = index % 4;
	int row = index / 4;
	ch_display_t* disp = &chDisplay[index];

	static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t * grid_in = lv_obj_create(grid);
	lv_obj_set_grid_cell(grid_in, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
	lv_obj_set_grid_dsc_array(grid_in, col_dsc, row_dsc);
	//lv_obj_set_size(grid_in, 360, 272);
	lv_obj_align(grid_in, LV_ALIGN_TOP_LEFT, 0, 0);
	lv_obj_set_layout(grid_in, LV_LAYOUT_GRID);
	lv_obj_add_style(grid_in, &gridStyle, 0);

	static lv_coord_t col_dsc2[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc2[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	lv_obj_t* gridBox1 = lv_obj_create(grid_in);
	lv_obj_set_grid_cell(gridBox1, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
	lv_obj_set_grid_dsc_array(gridBox1, col_dsc2, row_dsc2);
	lv_obj_set_layout(gridBox1, LV_LAYOUT_GRID);
	lv_obj_add_style(gridBox1, &gridStyleBox, 0);

	disp->lblReading = lv_label_create(gridBox1);
	//lv_obj_add_style(disp->lblReading, &style1, 0);
	lv_obj_set_grid_cell(disp->lblReading, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
	lv_label_set_text(disp->lblReading, reading);
	lv_obj_center(disp->lblReading);


	disp->lblName = lv_label_create(grid_in);
	lv_obj_add_style(disp->lblName, &style, 0);
	lv_obj_set_grid_cell(disp->lblName, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
	lv_label_set_text(disp->lblName, name);
	lv_obj_center(disp->lblName);
}

static void draw_data_view(void)
{
	static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

	grid_style_init();

	if (!lv_screen_main)
		lv_screen_main = lv_obj_create(NULL);

	lv_obj_t * grid = lv_obj_create(lv_screen_main);
	lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
	lv_obj_set_size(grid, 600, 480);
	lv_obj_set_style_border_width(grid, 0, 0);
	lv_obj_align(grid, LV_ALIGN_TOP_LEFT, 0, 0);
	lv_obj_set_layout(grid, LV_LAYOUT_GRID);
	lv_obj_add_style(grid, &gridStyle, 0);

	for (int i = 0; i < 16; i++)
		draw_ch_data(grid, i, names[i], readings[i], NULL);
}

void MainScreen_Draw(void)
{
	draw_data_view();
}

void MainScreen_Update(void)
{
	static int mod = 0;
	static bool mainLoad = false;
	if(mainLoad)
	{
		++mod;
		for (int k = 0; k < 16; k++)
			lv_label_set_text(chDisplay[k].lblReading, mod % 2 ? "15.5V" : "15.0V");
	}
	else
	{
		static int activate = 0;
		if(++activate >= 10)
		{
			//lv_scr_load_anim(lv_screen_main, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 0, true);
			lv_scr_load(lv_screen_main);
			mainLoad = true;
			//activate = 0;
		}
	}
}

#endif
/* EOF */
