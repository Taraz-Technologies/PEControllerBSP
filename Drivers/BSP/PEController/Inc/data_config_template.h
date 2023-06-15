/**
 ********************************************************************************
 * @file 		data_config.h
 * @author 		Waqas Ehsan Butt
 * @date 		September 25, 2021
 *
 * @brief	Defines the data and its related information
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
#ifndef DATA_CONFIG_H
#define DATA_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup Data_Config Data Config
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "general_header.h"
#include "error_config.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef enum
{
	DTYPE_BOOL,
	DTYPE_U8,
	DTYPE_U16,
	DTYPE_U32,
	DTYPE_S8,
	DTYPE_S16,
	DTYPE_S32,
	DTYPE_FLOAT,
	DTYPE_U8_BIT,
	DTYPE_U16_BIT,
	DTYPE_U32_BIT,
	DTYPE_COUNT
} base_data_type_t;
/**
 * @brief Typical data union defining all possible values
 */
typedef union
{
	bool b;			/**< Data in boolean form */
	uint8_t u8;			/**< Data in unsigned 8-bit form */
	uint16_t u16;
	uint32_t u32;
	int8_t s8;
	int16_t s16;
	int32_t s32;
	float f;
	uint32_t bits;		/**< Data in bits form, 1st 16-bits are item index,
								last 16-bits are bit index */
} data_union_t;
typedef enum
{
	UNIT_V,
	UNIT_A,
	UNIT_W,
	UNIT_Hz,
	UNIT_COUNT,
	UNIT_NONE,
} data_units_t;
/*******************************************************************************
 * Structures
 ******************************************************************************/
typedef struct _data_param_info_t
{
	base_data_type_t type;
	data_units_t unit;
	uint8_t index;
	uint16_t arg;
	const char* name;
	device_err_t (*Getter)(struct _data_param_info_t* paramInfo, char* value, bool addUnit);
	device_err_t (*Setter)(struct _data_param_info_t* paramInfo, const char* value);
} data_param_info_t;
/*******************************************************************************
 * Exported Variables
 ******************************************************************************/
extern const char* unitTxts[UNIT_COUNT];
/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @defgroup DIN_Exported_Functions Functions
 * @{
 */
// save diagnostic state
#pragma GCC diagnostic push
// turn off the specific warning. Can also use "-Wall"
#pragma GCC diagnostic ignored "-Wunused-function"
extern device_err_t Default_GetDataParameter(data_param_info_t* _paramInfo, char* value, bool addUnit);
extern device_err_t Default_SetDataParameter(data_param_info_t* _paramInfo, const char* value);
/*******************************************************************************
 * Code
 ******************************************************************************/
static device_err_t GetDataParameter(data_param_info_t* _paramInfo, char* value, bool addUnit)
{
	if (_paramInfo == NULL)
		return ERR_ILLEGAL;
	if (_paramInfo->Getter == NULL)
		return Default_GetDataParameter(_paramInfo, value, addUnit);
	return _paramInfo->Getter(_paramInfo, value, addUnit);
}

static device_err_t SetDataParameter(data_param_info_t* _paramInfo, const char* value)
{
	if (_paramInfo == NULL)
		return ERR_ILLEGAL;
	if (_paramInfo->Setter == NULL)
		return Default_SetDataParameter(_paramInfo, value);
	return _paramInfo->Setter(_paramInfo, value);
}
#pragma GCC diagnostic pop
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
