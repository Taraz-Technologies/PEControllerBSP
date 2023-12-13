/**
 ********************************************************************************
 * @file    	screen_message.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 30, 2023
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
#include "error_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define MSG_BOX_WIDTH			(LV_HOR_RES * 0.6f)
#define MSG_BOX_HEIGHT			(LV_VER_RES * 0.55f)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/

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
	if (e->user_data != NULL)
		lv_obj_del((lv_obj_t*)e->user_data);
}

/**
 * @brief Display a message on screen
 * @param _titleTxt Title of the message
 * @param _msgTxt Content of the message
 */
void DisplayMessage(const char* _titleTxt, const char* _msgTxt)
{
	// --TODO-- Disable touch for other sections
	// --FIXME-- Message Stacking may cause memory overflow
	// Create the error window
	lv_obj_t* win = lv_win_create(lv_layer_top(), 60);
	// Set the position and size of the error window
	lv_obj_set_width(win, MSG_BOX_WIDTH);
	lv_obj_set_height(win, MSG_BOX_HEIGHT);
	lv_obj_align(win, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_color(win, lv_theme_taraz_get()->color_secondary, 0);
	lv_obj_set_style_border_width(win, 2, 0);
	lv_obj_set_style_radius(win, 10, 0);
	lv_obj_set_style_bg_color(lv_win_get_header(win), lv_theme_taraz_get()->color_primary, 0);

	// TITLE
	//char txt[50];
	lv_obj_t* title = lv_win_add_title(win, _titleTxt == NULL ? " Error" : _titleTxt);
	lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_set_style_pad_left(title, 15, 0);
	lv_obj_set_style_text_font(title, &lv_font_montserrat_26, 0);

	/*Add control button to the header*/
	lv_obj_t * closeBtn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 50);           /*Add close button and use built-in close action*/
	lv_obj_set_style_bg_color(closeBtn, themeColors.bg, 0);
	lv_obj_set_style_border_color(closeBtn, lv_theme_taraz_get()->color_secondary, 0);
	//lv_obj_set_style_border_width(closeBtn, 2, 0);
	lv_obj_add_event_cb(closeBtn, Close_Clicked, LV_EVENT_CLICKED, win);
	lv_obj_add_event_cb(lv_win_get_header(win), Close_Clicked, LV_EVENT_CLICKED, win);

	// content
	lv_obj_set_style_bg_color(lv_win_get_content(win), themeColors.bg, 0);
	lv_obj_set_style_pad_top(lv_win_get_content(win), 0, 0);
	// text message
	lv_obj_t * txt = lv_label_create(lv_win_get_content(win));
	lv_label_set_text(txt, _msgTxt == NULL ? "Unspecified Error": _msgTxt);
	lv_obj_set_style_text_font(txt, &lv_font_montserrat_22, 0);
	lv_obj_set_style_text_align(txt, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_width(txt, MSG_BOX_WIDTH - 50);
	lv_label_set_long_mode(txt, LV_LABEL_LONG_WRAP);
	lv_obj_align(txt, LV_ALIGN_CENTER, 0, 0);
}

#if 0
void display_message_box(const char * title, const char * message)
{
	static const char * btns[] ={"Close", ""};
	lv_msgbox_t* mbox = (lv_msgbox_t*) lv_msgbox_create(screen, title, message, btns, false); // Create the message box

	static lv_style_t titleStyle;
	lv_style_init(&titleStyle);
	lv_style_set_bg_color(&titleStyle, lvColorStore.background);
	lv_style_set_pad_all(&titleStyle, 1);
	lv_style_set_text_font(&titleStyle, &lv_font_montserrat_30);
	lv_style_set_text_align(&titleStyle, LV_TEXT_ALIGN_CENTER);
	lv_style_set_text_color(&titleStyle, lvColorStore.darkFont);
	lv_obj_add_style(mbox->title, &titleStyle, 0);

	static lv_style_t msgStyle;
	lv_style_init(&msgStyle);
	lv_style_set_bg_color(&msgStyle, lvColorStore.darkTaraz);
	lv_style_set_pad_all(&msgStyle, 1);
	lv_style_set_pad_bottom(&msgStyle, 30);
	lv_style_set_text_font(&msgStyle, &lv_font_montserrat_22);
	lv_style_set_text_align(&msgStyle, LV_TEXT_ALIGN_CENTER);
	lv_style_set_text_color(&msgStyle, lvColorStore.mediumFont);
	lv_obj_add_style(mbox->content, &msgStyle, 0);

	//lv_obj_add_style(mbox->btns, &lvStyleStore.btn2, 0);
	lv_btnmatrix_set_btn_width(mbox->btns, 0, 150);
	lv_obj_align(mbox->btns, LV_ALIGN_CENTER, 0, 0);

	//lv_msgbox_set_style_title(mbox, &style);

	lv_obj_set_width((lv_obj_t*)mbox, LV_HOR_RES / 2);
	lv_obj_align((lv_obj_t*)mbox, LV_ALIGN_CENTER, 0, 0);
	//lv_obj_add_style(mbox, &style, 0);


	//lv_msgbox_set_text(mbox, message); // Set the message text
	//lv_msgbox_add_btn(mbox, "Close", LV_SYMBOL_OK); // Add a close button

	//lv_obj_set_event_cb(lv_mbox_get_btn(mbox, NULL), message_box_btn_click_action); // Set the button callback

	// Align the message box to the center of the screen

	//lv_msgbox_set_style_text(mbox, &lv_style_plain); // Set the style of the message box text
	//lv_msgbox_set_style_btn_rel(mbox, &lv_style_btn_rel); // Set the style of the message box button

	//lv_msgbox_set_anim_time(mbox, 100); // Set the animation time for the message box

	//lv_msgbox_set_text(mbox, message); // Set the message box text
	//lv_msgbox_set_title(mbox, title); // Set the message box title
}
#endif

/* EOF */
