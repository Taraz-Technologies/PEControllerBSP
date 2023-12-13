/**
 * @file lv_theme_taraz.h
 *
 */

#ifndef LV_THEME_TARAZ_H
#define LV_THEME_TARAZ_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../../Third_Party/lvgl/src/core/lv_obj.h"

#if 1

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
	lv_color_t bg;
	lv_color_t btn;
	lv_color_t border;
	lv_color_t text;
	lv_color_t alt_btn;
	lv_color_t alt_border;
	lv_color_t alt_text;
	lv_color_t current;
	lv_color_t voltage;
	lv_color_t gray;
} theme_colors_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the theme
 * @param color_primary the primary color of the theme
 * @param color_secondary the secondary color for the theme
 * @param font pointer to a font to use.
 * @return a pointer to reference this theme later
 */
lv_theme_t * lv_theme_taraz_init(lv_disp_t * disp, lv_color_t color_primary, lv_color_t color_secondary, bool dark,
                                   const lv_font_t * font);

/**
 * Get default theme
 * @return a pointer to default theme, or NULL if this is not initialized
 */
lv_theme_t * lv_theme_taraz_get(void);

/**
 * Check if default theme is initialized
 * @return true if default theme is initialized, false otherwise
 */
bool lv_theme_taraz_is_inited(void);

/**********************
 *      MACROS
 **********************/
extern theme_colors_t themeColors;
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_THEME_DEFAULT_H*/
