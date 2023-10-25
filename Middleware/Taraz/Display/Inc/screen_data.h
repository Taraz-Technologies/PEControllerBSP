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
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "monitoring_library.h"
#include "adc_config.h"
#include "shared_memory.h"
#include "state_storage_lib.h"
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
 * @brief Use this tag to indicate touch on Ok button
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
 * @brief Use this to attach tag
 */
#define TAG_ATTACH(x)			((void*)x)
/**
 * @brief Use this to get tag from attached event argument
 */
#define GET_TAG(e)				((uint8_t)(e->user_data))
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
	SCREEN_HELP,       /**< Help Screen */
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
                                            This parameter can be one of value of @ref LTDC_Pixelformat */
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

extern bool writeAtScreenEnd;
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
#endif 
/* EOF */
