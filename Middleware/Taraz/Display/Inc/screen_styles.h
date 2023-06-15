/**
 ********************************************************************************
 * @file 		screen_styles.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 12, 2023
 *
 * @brief    
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

#ifndef DISPLAY_SCREEN_STYLES_H_
#define DISPLAY_SCREEN_STYLES_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
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
	lv_color_t background;
	lv_color_t black;
	lv_color_t white;
	lv_color_t gray;
	lv_color_t lightFont;
	lv_color_t mediumFont;
	lv_color_t darkFont;
	lv_color_t btnBg1;
	lv_color_t btnBg2;
	lv_color_t btnBg3;
	lv_color_t lightTaraz;
	lv_color_t mediumTaraz;
	lv_color_t darkTaraz;
	lv_color_t on;
	lv_color_t off;
} lv_color_store_t;
typedef struct
{
	lv_style_t defaultGrid;
	lv_style_t thinMarginGrid;
	lv_style_t mediumMarginGrid;
	lv_style_t thickMarginGrid;
	lv_style_t defaultTextArea;
	lv_style_t btn1;
	lv_style_t btn2;
	lv_style_t btn3;
} lv_style_store;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/**
 * @brief General color store for use by the application
 */
extern lv_color_store_t lvColorStore;
/**
 * @brief General style store for use by the application
 */
extern lv_style_store lvStyleStore;
/**
 * @brief Grid component, either row / column for use with in the application
 */
extern lv_coord_t* singleRowCol;
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * Initializes the grid style according to the given basic properties
 * @param style Style to be initialized
 * @param pad Padding in pixels
 * @param margin Margin in rows for childs in pixels
 * @param border Border width in pixels
 * @param radius Radius of corners in pixels
 * @param bgColor Background color given in lv_color_t
 */
extern void BSP_Screen_InitGridStyle(lv_style_t* style, lv_coord_t pad, lv_coord_t margin, lv_coord_t border, lv_coord_t radius, lv_color_t* bgColor);

extern void BSP_Screen_InitLabelStyle(lv_style_t* style, const lv_font_t * font, lv_text_align_t align, lv_color_t* txtColor);
extern lv_color_t MakeColor(uint8_t r, uint8_t g, uint8_t b);
extern lv_obj_t* lv_grid_create_general(lv_obj_t* parent, lv_coord_t* cols, lv_coord_t* rows, lv_style_t* style, lv_color_t* bgColor, lv_event_cb_t event_cb, void * eventData);
extern lv_obj_t* lv_label_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData);
extern lv_obj_t* lv_btn_create_general(lv_obj_t* parent, lv_style_t* btnStyle, lv_style_t* lblStyle, const char* txt, lv_event_cb_t event_cb, void * eventData);
extern lv_obj_t* lv_textarea_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData);
extern lv_obj_t* lv_create_textfield_display(lv_obj_t* parent, lv_style_t* nameLblStyle, lv_style_t* fieldStyle, const char* fieldName,  const char* fieldValue, lv_event_cb_t event_cb, void * eventData, uint8_t row);
extern lv_obj_t* lv_create_field(lv_obj_t* parent, lv_style_t* nameLblStyle, const char* fieldName, lv_obj_t* field, uint8_t row);
extern void BSP_Screen_Styles_Init(void);
extern void BSP_Screen_InitBtnStyle(lv_style_t* style, lv_coord_t border, lv_coord_t radius, lv_color_t* bgColor);
extern lv_obj_t* lv_container_create_general(lv_obj_t* parent, lv_style_t* style, int row, int col, lv_event_cb_t event_cb, void * eventData);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
