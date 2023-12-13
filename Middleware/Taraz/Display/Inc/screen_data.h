/**
 ********************************************************************************
 * @file 		screen_data.h
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

#ifndef DISPLAY_SCREEN_DATA_H_
#define DISPLAY_SCREEN_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup Display
 * @{
 */

/** @addtogroup DisplayScreens
 * @{
 */

/** @defgroup Data Data
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "monitoring_library.h"
#include "adc_config.h"
#include "shared_memory.h"
#include "state_storage_lib.h"
#include "image_dictionary.h"
#include "screen_data_app.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup PEDISPLAYSCREEN_Exported_Macros Macros
  * @{
  */
/**
 * @brief Use this tag to indicate no touch tag
 */
#define TAG_NONE				(0)
/**
 * @brief Use this tag to indicate touch on OK button
 */
#define TAG_OK					(1)
/**
 * @brief Use this tag to indicate touch on Cancel button
 */
#define TAG_CANCEL				(2)
/**
 * @brief Use this tag to indicate touch on intelliSENS button
 */
#define TAG_intelliSENS			(3)
/**
 * @brief Use this tag to indicate touch on application information button
 */
#define TAG_APPINFO				(4)
/**
 * @brief Use this tag to indicate touch on help button
 */
#define TAG_HELP				(5)
/**
 * @brief Use this tag to indicate touch on settings button
 */
#define TAG_SETTINGS			(6)
/**
 * @brief Use this tag to indicate touch on Left button
 */
#define TAG_LEFT				(7)
/**
 * @brief Use this tag to indicate touch on Right button
 */
#define TAG_RIGHT				(8)
/**
 * @brief Use this to attach tag
 */
#define TAG_ATTACH(x)			((void*)x)
/**
 * @brief Use this to get tag from attached event argument
 */
#define GET_TAG(e)				((uint32_t)(e->user_data))
/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup PEDISPLAYSCREEN_Exported_Typedefs Typedefs
  * @{
  */
/**
 * @brief Defines the available screen types
 */
typedef enum
{
	SCREEN_SPLASH = 0, /**< Splash Screen */
	SCREEN_MAIN,       /**< Main Screen */
	SCREEN_CONF,       /**< Configuration Screen */
	SCREEN_APPINFO,    /**< Application Information Screen */
	SCREEN_intelliSENS,/**< intelliSENS Information Screen */
	SCREEN_COUNT,      /**< Not a type. Use this to get the total number of legal types */
	SCREEN_NONE,       /**< Invalid Screen */
	SCREEN_PREVIOUS,   /**< Previous Screen */
} screen_type_t;
/**
 * @brief X-Axis alignment for the image
 */
typedef enum
{
	ALIGN_LEFT_X, /**< Horizontal Left Alignment */
	ALIGN_RIGHT_X,/**< Horizontal Right Alignment */
	ALIGN_CENTER_X/**< Horizontal Center Alignment */
} image_align_x_t;
/**
 * @brief Y-Axis alignment for the image
 */
typedef enum
{
	ALIGN_UP_Y,    /**< Vertical Up Alignment */
	ALIGN_DOWN_Y,  /**< Vertical Down Alignment */
	ALIGN_CENTER_Y /**< Vertical Center Alignment */
} image_align_y_t;
/**
  * @}
  */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup PEDISPLAYSCREEN_Exported_Structures Structures
  * @{
  */
/**
 * @brief Contains the information regarding a LTDC layer
 */
typedef struct
{
	const uint8_t* data;				/**< @brief Layer data pointer */
	uint16_t posX;						/**< @brief Horizontal position of the layer */
	uint16_t posY;						/**< @brief Vertical position of the layer */
	uint16_t width;						/**< @brief Layer width */
	uint16_t height;					/**< @brief Layer height */
	image_align_x_t xAlign;				/**< @brief X-Axis alignment of image on display */
	image_align_y_t yAlign;				/**< @brief Y-Axis alignment of image on display */
	uint32_t PixelFormat;               /**< @brief Specifies the pixel format.
                                            This parameter can be one of value of LTDC_Pixelformat */
} ltdc_layer_info_t;
/**
 * @brief Contains the information regarding a measurement channel to be displayed
 */
typedef struct
{
	measure_type_t type;				/**< @brief Type of measurement */
	int chIndex;						/**< @brief Measurement channel index of the measurement */
} disp_ch_measure_t;
/**
 * @brief Contains the information regarding all measurement channels to be displayed
 */
typedef struct
{
	disp_ch_measure_t chMeasures[TOTAL_MEASUREMENT_COUNT];	/**< @brief Contains all channel measurement informations */
	adc_info_t* adcInfo;	/**< @brief Information of the ADC unit */
	const char** chNames;	/**< @brief Names of all measurement channels */
} disp_measure_t;
/**
 * @brief Contains the information regarding a specific screen
 */
typedef struct
{
	screen_type_t (*Refresh)(void);		/**< @brief This function refreshes the screen data and indicates screen switching if required
	 	 	 	 	 	 	 	 	 	 	 @returns If it is required to switch screen, returns target screen.
	 	 	 	 	 	 	 	 	 	 	 If it is required to switch to previous screen returns SCREEN_PREVIOUS.
	 	 	 	 	 	 	 	 	 	 	 If there is no need to switch screen returns SCREEN _NONE. */
	void (*Load)(void);					/**< @brief Call this function in the new screen before switching to the new screen. */
	void (*Unload)(void);				/**< @brief Call this function in the old screen before calling @ref Load() in the new screen. */
	ltdc_layer_info_t* directLayer;		/**< @brief The information for the directly displayed layer in LTDC module. */
	ltdc_layer_info_t* lvglLayer;		/**< @brief The information for the LVGL layer in LTDC module. */
} screens_t;
/**
 * @brief Represents a data parameter group
 */
typedef struct
{
	const char* title;					/**< @brief Title of the group */
	data_param_info_t** paramPointers;	/**< @brief Parameters of the group */
	int paramCount;						/**< @brief Parameter count in the group */
} data_param_group_t;
/**
* @brief Contains the information of the application required by the display unit.
*/
typedef struct
{
	const char* appInfo;							/**< @brief Text representing the application information.
															Set to NULL if no need to display this parameter.*/
	const char* connectionInfo;						/**< @brief Text representing the connection information for the application.
															Set to NULL if no need to display this parameter.*/
	const char* documentationLink;					/**< @brief Text representing the documentation link for the application.
															Set to NULL if no need to display this parameter.*/
	image_info_t* img;								/**< @brief Image displayed in the application screen.
															Set to NULL if no image to display.*/
	const char* appTitle;							/**< @brief Text representing the application title. */
} appinfo_display_t;
/**
  * @}
  */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup PEDISPLAYSCREEN_Exported_Variables Variables
  * @{
  */
/**
 * @brief Display related information of the channel measurements
 */
extern disp_measure_t dispMeasures;
/**
 * @brief Text representing measurement statistics
 */
extern const char* measureTxts[MEASURE_COUNT];
/**
 * @brief Text representing measurement units
 */
extern const char* unitTxts[UNIT_COUNT];
/**
 * @brief Default layer which can be pointed to, if the LTDC layer needs to be empty
 */
extern ltdc_layer_info_t defaultLayer;
/**
 * @brief This variable is used by the system to make sure that the whole screen is refreshed before changing to avoid glitching.
 */
extern bool writeAtScreenEnd;
/**
 * @brief This information will be displayed in the application information screen
 * @note The information is application dependent and can be edited in <b>screen_appinfo_data.c</b>.
 * The file is located in \"CM4/BSP/Display\" folder in the relevant project
 */
extern appinfo_display_t appInfoDisplay;
/**
 * @brief Assigns the image to be displayed on the splash screen
 */
extern image_info_t* splashImg;
#if SETTINGS_WINDOW_COUNT
/**
 * @brief This represents list of groups of settings to be configured in the application
 */
extern data_param_group_t settingWindows[SETTINGS_WINDOW_COUNT];
#endif
/**
  * @}
  */
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup PEDISPLAYSCREEN_Exported_Functions Functions
  * @{
  */
/**
 * @brief Configure the storage for the screen data
 * @param _config Configuration for the storage
 */
extern void ScreenData_ConfigStorage(state_storage_client_t* _config);
/**
 * @brief Load the relevant fields and configurations for the desired parameter
 * @note Make sure to call this function before switching to the configuration screen
 * @param _paramInfo Pointer to the desired parameter to be configured.
 * @param val Current value of the parameter
 */
extern void ConfigScreen_LoadParam(data_param_info_t* _paramInfo, char* val);
/**
 * @brief Load settings in the configuration screen.
 * @param _paramGroups List of parameter groups.
 * @param _groupCount No of parameter groups.
 */
extern void ConfigScreen_LoadSettings(data_param_group_t* _paramGroups, int _groupCount);
/********************************************************************************
 * Code
 *******************************************************************************/

/**
  * @}
  */
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" }
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
/**
  * @}
  */
#endif 
/* EOF */
