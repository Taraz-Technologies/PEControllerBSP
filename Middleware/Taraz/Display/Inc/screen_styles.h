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

/** @addtogroup BSP
 * @{
 */

/** @addtogroup Display
 * @{
 */

/** @defgroup DisplayStyles Styles
 * @details Contains the styles for general screen usage
 * @{
 */
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
/** @defgroup PEDISPLAYSTYLES_Exported_Structures Structures
  * @{
  */
/**
 * @brief Contains the available color definitions for the display
 */
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
	lv_color_t current;
	lv_color_t voltage;
	lv_color_t on;
	lv_color_t off;
} lv_color_store_t;
/**
 * @brief Contains the available style definitions for the display
 */
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
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup PEDISPLAYSTYLES_Exported_Variables Variables
  * @{
  */
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
/**
 * @}
 */
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup PEDISPLAYSTYLES_Exported_Functions Functions
  * @{
  */
/**
 * @brief Initializes the grid style according to the given basic properties
 * @param style Style to be initialized
 * @param pad Padding in pixels
 * @param margin Margin in rows for childs in pixels
 * @param border Border width in pixels
 * @param radius Radius of corners in pixels
 * @param bgColor Background color given in lv_color_t
 */
extern void BSP_Screen_InitGridStyle(lv_style_t* style, lv_coord_t pad, lv_coord_t margin, lv_coord_t border, lv_coord_t radius, lv_color_t* bgColor);
/**
 * @brief Initializes the label style
 * @param style Style to be initialized
 * @param font Text font. NULL if default font
 * @param align Text alignment
 * @param txtColor Text color. NULL if default color
 */
extern void BSP_Screen_InitLabelStyle(lv_style_t* style, const lv_font_t * font, lv_text_align_t align, lv_color_t* txtColor);
/**
 * @brief Make RGB color in selected format
 * @param r Red color
 * @param g Green color
 * @param b Blue color
 * @return RGB color
 */
extern lv_color_t MakeColor(uint8_t r, uint8_t g, uint8_t b);
/**
 * @brief Create general grid
 * @param parent Parent lv_obj_t
 * @param cols Column information
 * @param rows Row information
 * @param style Grid style. NULL if default style
 * @param bgColor Background color. NULL if default color
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Generated grid object
 */
extern lv_obj_t* lv_grid_create_general(lv_obj_t* parent, lv_coord_t* cols, lv_coord_t* rows, lv_style_t* style, lv_color_t* bgColor, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create general label
 * @param parent Parent lv_obj_t
 * @param style Label style. NULL if default style
 * @param txt Label text
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Generated label object
 */
extern lv_obj_t* lv_label_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create general button
 * @param parent Parent lv_obj_t
 * @param btnStyle Button style. NULL if default style
 * @param lblStyle Style for Label included in button. NULL if default style
 * @param txt Label text
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Generated button object
 */
extern lv_obj_t* lv_btn_create_general(lv_obj_t* parent, lv_style_t* btnStyle, lv_style_t* lblStyle, const char* txt, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create general textarea
 * @param parent Parent lv_obj_t
 * @param style Textarea style. NULL if default style
 * @param txt Label text
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Generated Textarea object
 */
extern lv_obj_t* lv_textarea_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create general text entry field with label and value
 * @param parent Parent lv_obj_t
 * @param nameLblStyle Label style for the name. NULL if default style
 * @param fieldStyle Label style for the field value. NULL if default style
 * @param fieldName Text representing field name
 * @param fieldValue Text representing field value
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @param row Row no in parent grid
 * @return Generated value text field
 */
extern lv_obj_t* lv_create_textfield_display(lv_obj_t* parent, lv_style_t* nameLblStyle, lv_style_t* fieldStyle, const char* fieldName,  const char* fieldValue, lv_event_cb_t event_cb, void * eventData, uint8_t row);
/**
 * @brief Create general entry field with label and a given field
 * @param parent Parent lv_obj_t
 * @param nameLblStyle Label style for the name. NULL if default style
 * @param fieldName Text representing field name
 * @param field Field value object
 * @param row Row no in parent grid
 * @return Generated value field
 */
extern lv_obj_t* lv_create_field(lv_obj_t* parent, lv_style_t* nameLblStyle, const char* fieldName, lv_obj_t* field, uint8_t row);
/**
 * @brief Initializes base screen styles
 */
extern void BSP_Screen_Styles_Init(void);
/**
 * @brief Initializes the button style
 * @param style Style to be initialized
 * @param border Button border width
 * @param radius Button radius width
 * @param bgColor Background color given in lv_color_t
 */
extern void BSP_Screen_InitBtnStyle(lv_style_t* style, lv_coord_t border, lv_coord_t radius, lv_color_t* bgColor);
/**
 * @brief Create general container
 * @param parent Parent lv_obj_t
 * @param style Container style. NULL if default style
 * @param row Row no in parent grid
 * @param col Column no in parent grid
 * @param event_cb Click event handler. Set to NULL if no click event needed
 * @param eventData Click event custom user data pointer
 * @return Container object
 */
extern lv_obj_t* lv_container_create_general(lv_obj_t* parent, lv_style_t* style, int row, int col, lv_event_cb_t event_cb, void * eventData);
/********************************************************************************
 * Code
 *******************************************************************************/

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
#endif 
/* EOF */
