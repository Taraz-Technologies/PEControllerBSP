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
#include "screen_base.h"
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
 * @brief Contains the available style definitions for the display
 */
typedef struct
{
	lv_style_t defaultGrid;					/**< @brief Default grid style */
	lv_style_t thinMarginGrid;				/**< @brief Thin margin grid style */
	lv_style_t mediumMarginGrid;			/**< @brief Medium margin grid style */
	lv_style_t thickMarginGrid;				/**< @brief Thick margin grid style */
} lv_style_store;
/**
 * @brief Contains the required items for displaying the fields with a name and value
 */
typedef struct
{
	bool isTextArea;						/**< @brief If editable field value, set this to <c>true</c>. */
	bool colorFieldName;					/**< @brief If <c>true</c> colors the field name, else colors the field value. */
	lv_obj_t* nameField;					/**< @brief lv_obj_t representing the name label. */
	lv_obj_t* valueField;					/**< @brief lv_obj_t representing the value label. */
	lv_obj_t* container;					/**< @brief lv_obj_t representing the overall container. */
	const char* nameTxt;					/**< @brief Name of the field. */
	const char* valueTxt;					/**< @brief Value of the field. */
	lv_coord_t colWidths[2];				/**< @brief Width of both columns, name and value. */
} lv_ta_field_data_t;
/**
 * @brief Contains the grid position
 */
typedef struct
{
	int row;			/**< @brief First grid row from which the object should start. */
	int col;			/**< @brief First grid column from which the object should start.  */
	int rowSpan;		/**< @brief Grid row span. Number of rows that the object is allowed to take. */
	int colSpan;		/**< @brief Grid column span. Number of columns that the object is allowed to take. */
} lv_grid_pos_info_t;
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
 * @brief Initializes the grid style according to the given basic properties.
 * @param style Style to be initialized.
 * @param pad Padding in pixels.
 * @param margin Margin in rows for children in pixels.
 * @param border Border width in pixels.
 * @param radius Radius of corners in pixels.
 * @param bgColor Background color. NULL if default color.
 */
extern void BSP_Screen_InitGridStyle(lv_style_t* style, lv_coord_t pad, lv_coord_t margin, lv_coord_t border, lv_coord_t radius, lv_color_t* bgColor);
/**
 * @brief Initializes the label style.
 * @param style Style to be initialized.
 * @param font Text font. NULL if default font.
 * @param align Text alignment.
 * @param txtColor Text color. NULL if default color.
 */
extern void BSP_Screen_InitLabelStyle(lv_style_t* style, const lv_font_t * font, lv_text_align_t align, lv_color_t* txtColor);
/**
 * @brief Initializes the Text Area Style.
 * @param style Style to be initialized.
 * @param font Text font. NULL if default font.
 * @param txtColor Text color. NULL if default color.
 */
extern void BSP_Screen_InitTextAreaStyle(lv_style_t* style, const lv_font_t * font, lv_color_t* txtColor);
/**
 * @brief Create a text field with name and value in default style.
 * @param parent Parent object in which field is to be placed.
 * @param field Field information data.
 * @param row Row number in the parent.
 * @param col Column number in the parent.
 * @param event_cb cb Click event handler. Set to NULL if no click event needed.
 * @param eventData Click event custom user data pointer.
 */
extern void lv_create_default_text_field(lv_obj_t* parent, lv_ta_field_data_t* field, int row, int col, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create a text field with name and value with given styles.
 * @param parent Parent object in which field is to be placed.
 * @param nameLblStyle Label style for the field name. NULL if default style.
 * @param valueStyle Textarea style for the field value. NULL if default style.
 * @param fieldName Name of the field in text.
 * @param fieldValue Value of the field in text.
 * @param event_cb cb Click event handler. Set to NULL if no click event needed.
 * @param eventData Click event custom user data pointer.
 * @param row Row number in the parent.
 * @return Generated textarea object.
 */
extern lv_obj_t* lv_create_textfield_display(lv_obj_t* parent, lv_style_t* nameLblStyle, lv_style_t* valueStyle, const char* fieldName, const char* fieldValue, lv_event_cb_t event_cb, void * eventData, uint8_t row);
/**
 * @brief Create general entry field with label and a given field.
 * @param parent Parent object in which field is to be placed.
 * @param nameLblStyle Label style for the field name. NULL if default style.
 * @param fieldName Name of the field in text.
 * @param field lv_obj_t inserted in place of field value.
 * @param row Row number in the parent.
 */
extern void lv_create_field(lv_obj_t* parent, lv_style_t* nameLblStyle, const char* fieldName, lv_obj_t* field, uint8_t row);
/**
 * @brief Create a general button.
 * @param parent Parent object in which field is to be placed.
 * @param btnStyle Button style for the field name. NULL if default style.
 * @param lblStyle Label style for the field name. NULL if default style.
 * @param txt Text in the button.
 * @param event_cb cb Click event handler. Set to NULL if no click event needed.
 * @param eventData Click event custom user data pointer.
 * @return Generated button object.
 */
extern lv_obj_t* lv_btn_create_general(lv_obj_t* parent, lv_style_t* btnStyle, lv_style_t* lblStyle, const char* txt, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create a general label.
 * @param parent Parent object in which field is to be placed.
 * @param style Label style. NULL if default style.
 * @param txt Label text.
 * @param event_cb cb Click event handler. Set to NULL if no click event needed.
 * @param eventData Click event custom user data pointer.
 * @return Generated label object.
 */
extern lv_obj_t* lv_label_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create general textarea.
 * @param parent Parent object in which field is to be placed.
 * @param style Textarea style. NULL if default style.
 * @param txt Label text.
 * @param event_cb cb Click event handler. Set to NULL if no click event needed.
 * @param eventData Click event custom user data pointer.
 * @return Generated Textarea object.
 */
extern lv_obj_t* lv_textarea_create_general(lv_obj_t* parent, lv_style_t* style, const char* txt, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create general grid.
 * @param parent Parent object in which field is to be placed.
 * @param cols Column definitions.
 * @param rows Row definitions.
 * @param style Grid style. NULL if default style.
 * @param bgColor Background color. NULL if default color.
 * @param event_cb cb Click event handler. Set to NULL if no click event needed.
 * @param eventData Click event custom user data pointer.
 * @return Generated grid object.
 */
extern lv_obj_t* lv_grid_create_general(lv_obj_t* parent, lv_coord_t* cols, lv_coord_t* rows, lv_style_t* style, lv_color_t* bgColor, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create general container.
 * @param parent Parent object in which field is to be placed.
 * @param style Container style. NULL if default style.
 * @param row Row definitions.
 * @param col Column definitions.
 * @param event_cb cb Click event handler. Set to NULL if no click event needed.
 * @param eventData Click event custom user data pointer.
 * @return Generated container object.
 */
extern lv_obj_t* lv_container_create_general(lv_obj_t* parent, lv_style_t* style, int row, int col, lv_event_cb_t event_cb, void * eventData);
/**
 * @brief Create page title.
 * @param parent Parent object in which field is to be placed.
 * @param gridInfo Grid position related information.
 * @param title Title text.
 * @return Generated title label.
 */
extern lv_obj_t* CreateTitle(lv_obj_t* parent, lv_grid_pos_info_t* gridInfo, const char* title);
/**
 * @brief Initializes base screen styles
 */
extern void BSP_Screen_Styles_Init(void);
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
