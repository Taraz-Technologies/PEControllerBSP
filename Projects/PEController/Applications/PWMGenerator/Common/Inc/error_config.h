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

/** @addtogroup Error_Config Error Config
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
typedef enum
{
	ERR_OK,
	ERR_INVALID_TEXT,
	ERR_ILLEGAL,
	ERR_OUT_OF_RANGE,
	ERR_NOT_AVAILABLE,
	ERR_COUNT,
} device_err_t;
/*******************************************************************************
 * Structures
 ******************************************************************************/
typedef struct
{
	const char* caption;
	const char* desc;
	const char* code;
} device_err_info_t;
 /*******************************************************************************
 * Exported Variables
 ******************************************************************************/
extern device_err_info_t errInfo[ERR_COUNT];
/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @defgroup DIN_Exported_Functions Functions
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

/**
 * @}
 */
#endif
/* EOF */
