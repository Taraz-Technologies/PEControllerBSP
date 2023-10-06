/**
 ********************************************************************************
 * @file 		error_config.h
 * @author 		Waqas Ehsan Butt
 * @date 		September 25, 2021
 *
 * @brief	Defines the errors and its related information
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
#ifndef ERROR_CONFIG_H
#define ERROR_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup Error_Config Error Configuration 
 * @brief Defines the errors and its related information 
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "general_header.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @defgroup ERRORCONFIG_Exported_TypeDefs Type Definitions
* @{
*/
/**
 * @brief Defines the types of errors
 */
typedef enum
{
	ERR_OK,           /**< No Error */
	ERR_INVALID_TEXT, /**< Text is invalid */
	ERR_ILLEGAL,      /**< Value is illegal */
	ERR_OUT_OF_RANGE, /**< Value is out of defined range */
	ERR_NOT_AVAILABLE,/**< Parameter not available */
	ERR_COUNT,        /**< Not an error. Use this to get the total number of legal error types. */
} device_err_t;
/**
 * @}
 */
/*******************************************************************************
 * Structures
 ******************************************************************************/
/** @defgroup ERRORCONFIG_Exported_Strucutres Structures
* @{
*/
/**
 * @brief Describes the errors defined in @ref device_err_t.
 */
typedef struct
{
	const char* caption;	/**< Error caption */
	const char* desc;		/**< Error description */
	const char* code;		/**< Error code */
} device_err_info_t;
/**
 * @}
 */
 /*******************************************************************************
 * Exported Variables
 ******************************************************************************/
 /** @defgroup ERRORCONFIG_Exported_Variables Variables
 * @{
 */
/**
 * @brief Details of the errors described in @ref device_err_info_t. Keep the order of descriptions same as definitions.
 */
extern device_err_info_t errInfo[ERR_COUNT];
/**
 * @}
 */
/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @defgroup ERRORCONFIG_Exported_Functions Functions
  * @{
  */

/*******************************************************************************
 * Code
 ******************************************************************************/

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
