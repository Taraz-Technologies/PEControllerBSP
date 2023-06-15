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
#define TAG_NONE				(0)
#define TAG_OK					(1)
#define TAG_CANCEL				(2)
#define TAG_intelliSENS			(3)
#define TAG_APPINFO				(4)
#define TAG_HELP				(5)
#define TAG_ATTACH(x)			((void*)x)
#define GET_TAG(e)				((uint8_t)(e->user_data))
/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef enum
{
	SCREEN_SPLASH = 0,
	SCREEN_MAIN,
	SCREEN_CONF,
	SCREEN_APPINFO,
	SCREEN_intelliSENS,
	SCREEN_HELP,
	SCREEN_COUNT,
	SCREEN_NONE,
	SCREEN_PREVIOUS,
} screen_type_t;
/**
 * @brief X-Axis alignment for the image
 */
typedef enum
{
	ALIGN_LEFT_X, /**< ALIGN_LEFT_X */
	ALIGN_RIGHT_X,/**< ALIGN_RIGHT_X */
	ALIGN_CENTER_X/**< ALIGN_CENTER_X */
} image_align_x_t;
/**
 * @brief Y-Axis alignment for the image
 */
typedef enum
{
	ALIGN_UP_Y,    /**< ALIGN_UP_Y */
	ALIGN_DOWN_Y,  /**< ALIGN_DOWN_Y */
	ALIGN_CENTER_Y /**< ALIGN_CENTER_Y */
} image_align_y_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	const uint8_t* data;
	uint16_t posX;
	uint16_t posY;
	uint16_t width;
	uint16_t height;
	image_align_x_t xAlign;				 /**< @brief X-Axis alignment of image on display */
	image_align_y_t yAlign;				 /**< @brief Y-Axis alignment of image on display */
	uint32_t PixelFormat;                /*!< Specifies the pixel format.
                                            This parameter can be one of value of @ref LTDC_Pixelformat */
} ltdc_layer_info_t;
typedef struct
{
	measure_type_t type;
	int chIndex;
} disp_ch_measure_t;
typedef struct
{
	disp_ch_measure_t chMeasures[TOTAL_MEASUREMENT_COUNT];
	adc_info_t* adcInfo;
	const char** chNames;
} disp_measure_t;
typedef struct
{
	screen_type_t (*Refresh)(void);
	void (*Load)(void);
	void (*Unload)(void);
	ltdc_layer_info_t* directLayer;
	ltdc_layer_info_t* lvglLayer;
} screens_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
extern disp_measure_t dispMeasures;
extern const char* measureTxts[MEASURE_COUNT];
extern const char* unitTxts[UNIT_COUNT];
extern ltdc_layer_info_t defaultLayer;
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
extern void ScreenData_ConfigStorage(state_storage_client_t* _config);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
