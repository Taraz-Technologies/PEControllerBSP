/**
 ********************************************************************************
 * @file 		screen_data.h
 * @author 		Waqas Ehsan Butt
 * @date 		Mar 14, 2023
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

#ifndef DISPLAY_SCREENS_SCREEN_DATA_H_
#define DISPLAY_SCREENS_SCREEN_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
#if LCD_DATA_MONITORING
#include "general_header.h"
#include "lvgl.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
/**
 * @brief Defines the parameter sources for the GUI display screens
 */
typedef enum
{
	PARAM_SRC_MEASUREMENT,			/**< PARAM_SRC_MEASUREMENT */
	PARAM_SRC_FLOAT_VALUE,   		/**< PARAM_SRC_RO_VALUE */
	PARAM_SRC_UINT_VALUE,    		/**< PARAM_SRC_RW_VALUE */
	PARAM_SRC_INT_VALUE,    		/**< PARAM_SRC_RW_VALUE */
	PARAM_SRC_BINARY_VALUE,			/**< PARAM_SRC_BINARY_VALUE */
	PARAM_SRC_SELECTABLE_VALUE, 	/**< PARAM_SRC_SELECTABLE_VALUE */
	PARAM_SRC_LAST,					/**< PARAM_SRC_SELECTABLE_VALUE */
} param_src_type_t;
/**
 * @brief Defines the available measurements for the GUI display screens
 */
typedef enum
{
	DISP_RMS = 0,         /**< DISP_RMS */
	DISP_AVG = 1,         /**< DISP_AVG */
	DISP_MAX = 2,         /**< DISP_MAX */
	DISP_MIN = 3,         /**< DISP_MIN */
	DISP_PkToPk = 4,      /**< DISP_PkToPk */
	DISP_LAST = 5,		  /**< DISP_LAST */
} param_measure_type_t;
typedef enum
{
	ERR_ILLEGAL,
	ERR_OUT_OF_RANGE,
	ERR_NOT_AVAILABLE,
	ERR_LAST,
} param_set_err_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	int index;
	int maxIndex;
	float temp1;
	float temp2;
	float temp;
} param_measures_temp_vars_t;
typedef struct
{
	param_measure_type_t type;
	int channelIndex;
	param_measures_temp_vars_t temps;
	float value;
	float freq;
	float sensitivity;
	float offset;
} param_measures_t;
typedef struct
{
	void* ptrToValue;
	char* selectionPtr;
	//param_set_err_t SetEnteredValue(void*);
} param_value_t;
typedef union
{
	param_measures_t measure;
	param_value_t value;
} param_src_t;
typedef struct
{
	param_src_type_t srcType;
	param_src_t src;
	bool isUpdated;
	char* srcName;
} disp_param_t;
typedef struct
{
	lv_obj_t* lblName;
	lv_obj_t* lblReading;
	lv_obj_t* lblValue;
} ch_display_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
extern disp_param_t chDisplayParams[16];
extern const char* measureTxts[5];
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/

#endif
#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
