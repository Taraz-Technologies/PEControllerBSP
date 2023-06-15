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

/** @addtogroup Data_Config Data Configuration
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
	DTYPE_S8,
	DTYPE_U16,
	DTYPE_S16,
	DTYPE_U32,
	DTYPE_S32,
	DTYPE_FLOAT,
	DTYPE_BIT_ACCESS,
	DTYPE_COUNT
} base_data_type_t;
/**
 * @brief Typical data union defining all possible values
 */
typedef union
{
	bool b;				/**< Data in boolean form */
	uint8_t u8;			/**< Data in unsigned 8-bit form */
	int8_t s8;
	uint16_t u16;
	int16_t s16;
	uint32_t u32;
	int32_t s32;
	float f;
	uint32_t bits;
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
/**
 * @brief Describes a data parameter configuration
 */
typedef struct _data_param_info_t
{
	base_data_type_t type;
	data_units_t unit;
	uint8_t index;
	uint16_t arg;
	const char* name;
	device_err_t (*Getter_InText)(struct _data_param_info_t* paramInfo, char* value, bool addUnit);		/**< @brief Function used to get a parameter in string format.*/
	device_err_t (*Setter_FromText)(struct _data_param_info_t* paramInfo, const char* value);			/**< @brief Function used to set a parameter from a formatted text.*/
	device_err_t (*Getter)(struct _data_param_info_t* paramInfo, data_union_t* value);					/**< @brief Function used to get value of a parameter.*/
	device_err_t (*Setter)(struct _data_param_info_t* paramInfo, data_union_t value);					/**< @brief Function used to set value of a parameter.*/
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
/**
 * @brief Default function to get the data parameters in textual format, according to the parameter info
 * @param _paramInfo Structure defining the parameter.
 * @param value Pointer to the location to be updated with the value
 * @param addUnit <c>true</c> if unit needs to be added with the text else false.
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
extern device_err_t Default_GetDataParameter_InText(data_param_info_t* _paramInfo, char* value, bool addUnit);
/**
 * @brief  Default function to set the data parameters from textual value, according to the parameter info
 * @param _paramInfo Structure defining the parameter.
 * @param value Value to be set as string
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
extern device_err_t Default_SetDataParameter_FromText(data_param_info_t* _paramInfo, const char* value);
/**
 * @brief  Default function to get the data parameters according to the parameter info
 * @param _paramInfo Structure defining the parameter.
 * @param value Pointer to the location to be updated with the value
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
extern device_err_t Default_GetDataParameter(data_param_info_t* _paramInfo, data_union_t* value);
/**
 * @brief  Default function to set the data parameters according to the parameter info
 * @param _paramInfo Structure defining the parameter.
 * @param value Value to be set
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
extern device_err_t Default_SetDataParameter(data_param_info_t* _paramInfo, data_union_t value);
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Gets the data parameters in textual format, according to the parameter info. If appropriate function not found, calls the default function.
 * @param _paramInfo Structure defining the parameter.
 * @param value Pointer to the location to be updated with the value
 * @param addUnit <c>true</c> if unit needs to be added with the text else false.
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
static device_err_t GetDataParameter_InText(data_param_info_t* _paramInfo, char* value, bool addUnit)
{
	if (_paramInfo == NULL)
		return ERR_ILLEGAL;
	if (_paramInfo->Getter_InText == NULL)
		return Default_GetDataParameter_InText(_paramInfo, value, addUnit);
	return _paramInfo->Getter_InText(_paramInfo, value, addUnit);
}
/**
 * @brief Sets the data parameters from textual value, according to the parameter info. If appropriate function not found, calls the default function.
 * @param _paramInfo Structure defining the parameter.
 * @param value Value to be set as string
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
static device_err_t SetDataParameter_FromText(data_param_info_t* _paramInfo, const char* value)
{
	if (_paramInfo == NULL)
		return ERR_ILLEGAL;
	if (_paramInfo->Setter_FromText == NULL)
		return Default_SetDataParameter_FromText(_paramInfo, value);
	return _paramInfo->Setter_FromText(_paramInfo, value);
}
/**
 * @brief Gets the data parameters according to the parameter info. If appropriate function not found, calls the default function.
 * @param _paramInfo Structure defining the parameter.
 * @param value Pointer to the location to be updated with the value
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
static device_err_t GetDataParameter(data_param_info_t* _paramInfo, data_union_t* value)
{
	if (_paramInfo == NULL)
		return ERR_ILLEGAL;
	if (_paramInfo->Getter == NULL)
		return Default_GetDataParameter(_paramInfo, value);
	return _paramInfo->Getter(_paramInfo, value);
}
/**
 * @brief Sets the data parameters according to the parameter info. If appropriate function not found, calls the default function.
 * @param _paramInfo Structure defining the parameter.
 * @param value Value to be set
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
static device_err_t SetDataParameter(data_param_info_t* _paramInfo, data_union_t value)
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
