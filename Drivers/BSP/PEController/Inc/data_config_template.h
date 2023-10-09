/**
 ********************************************************************************
 * @file 		data_config_template.h
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
 * @brief Defines the data and its related information
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
/** @defgroup DATACONFIG_Exported_TypeDefs Type Definitions
* @{
*/
/**
 * @brief Defines the usable data types
 */
typedef enum
{
	DTYPE_BOOL,   /**< Boolean */
	DTYPE_U8,     /**< Unsigned 8-bit integer */
	DTYPE_S8,     /**< Signed 8-bit integer */
	DTYPE_U16,    /**< Unsigned 16-bit integer */
	DTYPE_S16,    /**< Signed 16-bit integer */
	DTYPE_U32,    /**< Unsigned 32-bit integer */
	DTYPE_S32,    /**< Signed 32-bit integer */
	DTYPE_FLOAT,  /**< Single precision floating point number  */
	DTYPE_BIT_ACCESS,/**< 32-bit, Bit accessible register */
	DTYPE_COUNT		/**< Not a type. Use this to get the total number of legal types */
} base_data_type_t;
/**
 * @brief Typical data union defining all possible values
 */
typedef union
{
	bool b;			/**< Data in boolean form */
	uint8_t u8;		/**< Data in unsigned 8-bit form */
	int8_t s8;		/**< Data in signed 8-bit form */
	uint16_t u16;	/**< Data in unsigned 16-bit form */
	int16_t s16;	/**< Data in signed 16-bit form */
	uint32_t u32;	/**< Data in unsigned 32-bit form */
	int32_t s32;	/**< Data in signed 32-bit form */
	float f;		/**< Data in single precision floating point form */
	uint32_t bits;		/**< Data in bits form, 1st 16-bits are item index,
								last 16-bits are bit index. */
} data_union_t;
/**
 * @brief Defines all available units.
 */
typedef enum
{
	UNIT_V,    /**< Volt */
	UNIT_A,    /**< Ampere */
	UNIT_W,    /**< Watt */
	UNIT_Hz,   /**< Hertz */
	UNIT_COUNT,/**< Not a type. Use this to get the total number of legal unit types. */
	UNIT_NONE, /**< Not a type. Use this to suggest that item is without units. */
} data_units_t;
/**
 * @}
 */
/*******************************************************************************
 * Structures
 ******************************************************************************/
/** @defgroup DATACONFIG_Exported_Strucutres Structures
* @{
*/
/**
 * @brief Contains the set of definitions required by a specific data parameter.
 */
typedef struct _data_param_info_t
{
	base_data_type_t type;		/**< Type of data */
	data_units_t unit;			/**< Unit of data */
	uint8_t index;				/**< --TODO-- */
	uint16_t arg;				/**< Custom argument used by the data parameter */
	const char* name;			/**< Name of the parameter */
	device_err_t (*Getter_InText)(struct _data_param_info_t* paramInfo, char* value, bool addUnit);/**
	 * @brief Callback that gets the value of a parameter in string format.
	 * @param paramInfo Information regarding the data parameter.
	 * @param value text representation of the value acquired.
	 * @param addUnit If <c>true</c> append the unit of the parameter at the end of the string result.
	 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
	 */
	device_err_t (*Setter_FromText)(struct _data_param_info_t* paramInfo, const char* value);/**
	 * @brief Callback that sets the value of a parameter in string format.
	 * @param paramInfo Information regarding the data parameter.
	 * @param value text representation of the parameter value.
	 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
	 */
	device_err_t (*Getter)(struct _data_param_info_t* paramInfo, data_union_t* value);/**
	 * @brief Callback that gets the value of a parameter
	 * @param paramInfo Information regarding the data parameter.
	 * @param value Location where parameter value is to be updated.
	 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
	 */
	device_err_t (*Setter)(struct _data_param_info_t* paramInfo, data_union_t value);/**
	 * @brief Callback that sets the value of a parameter
	 * @param paramInfo Information regarding the data parameter.
	 * @param value Parameter value to be applied
	 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
	 */
} data_param_info_t;
/**
 * @}
 */
/*******************************************************************************
 * Exported Variables
 ******************************************************************************/
/** @defgroup DATACONFIG_Exported_Variables Variables
* @{
*/
/**
* @brief Text representation of the units described in @ref data_units_t. Keep the order of texts same as definitions.
*/
extern const char* unitTxts[UNIT_COUNT];
/**
* @}
*/
/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @defgroup DATACONFIG_Exported_Functions Functions
 * @{
 */
// save diagnostic state
#pragma GCC diagnostic push
// turn off the specific warning. Can also use "-Wall"
#pragma GCC diagnostic ignored "-Wunused-function"
/**
 * @brief Default function to get the data parameters in textual format, according to the parameter info
 * @note This function is used whenever the data_param_info_t.Getter_InText function is not defined and @ref GetDataParameter_InText() is called.
 * @param _paramInfo Structure defining the parameter.
 * @param value Pointer to the location to be updated with the value
 * @param addUnit <c>true</c> if unit needs to be added with the text else false.
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
extern device_err_t Default_GetDataParameter_InText(data_param_info_t* _paramInfo, char* value, bool addUnit);
/**
 * @brief  Default function to set the data parameters from textual value, according to the parameter info
 * @note This function is used whenever the @ref data_param_info_t.Setter_FromText function is not defined and @ref SetDataParameter_FromText() is called.
 * @param _paramInfo Structure defining the parameter.
 * @param value Value to be set as string
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
extern device_err_t Default_SetDataParameter_FromText(data_param_info_t* _paramInfo, const char* value);
/**
 * @brief  Default function to get the data parameters according to the parameter info
 * @note This function is used whenever the @ref data_param_info_t.Getter function is not defined and @ref GetDataParameter() is called.
 * @param _paramInfo Structure defining the parameter.
 * @param value Pointer to the location to be updated with the value
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
extern device_err_t Default_GetDataParameter(data_param_info_t* _paramInfo, data_union_t* value);
/**
 * @brief  Default function to set the data parameters according to the parameter info
 * @note This function is used whenever the @ref data_param_info_t.Setter function is not defined and @ref SetDataParameter() is called.
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
#endif
/* EOF */
