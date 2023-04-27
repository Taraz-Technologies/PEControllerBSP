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
#include "monitoring_library.h"
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

typedef enum
{
	ERR_ILLEGAL,
	ERR_OUT_OF_RANGE,
	ERR_NOT_AVAILABLE,
	ERR_LAST,
} param_set_err_t;
typedef enum
{
	VAR_NONE = 0,
	VAR_BOOL,
	VAR_STRING,
	VAR_FLOAT,
	VAR_UFLOAT,
	VAR_INT,
	VAR_UINT,
} var_type_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	float rms;
	float avg;
	float max;
	float min;
	int index;
	int maxIndex;
} measure_ch_tempstats_t;
typedef struct
{
	float rms;
	float avg;
	float max;
	float min;
	float pkTopk;
} measure_ch_stats_t;
typedef struct
{
	measure_type_t type;
	const char* chName;
	const char* chUnit;
	measure_ch_stats_t stats;
	int channelIndex;
	float freq;
} disp_ch_measure_t;
typedef struct
{
	bool isUpdated;
	disp_ch_measure_t disp[16];
	measure_ch_tempstats_t tempStats[16];
	float* sensitivity;
	float* offsets;
} disp_measure_t;
typedef struct
{
	lv_obj_t* lblReading;
	lv_obj_t* lblValue;
} ch_display_t;
typedef struct
{
	var_type_t type;
	const char* name;
	const char* unit;
	void* value;
	lv_obj_t* lbl;
} disp_var_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
extern disp_var_t dispVars[5];
extern disp_measure_t chDispMeasures;
extern const char* measureTxts[MEASURE_COUNT];
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
