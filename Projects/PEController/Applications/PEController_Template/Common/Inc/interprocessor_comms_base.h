/**
 ********************************************************************************
 * @file 		interprocessor_comms_base.h
 * @author 		Waqas Ehsan Butt
 * @date 		September 25, 2021
 *
 * @brief	Controls the communication between the communication and controls processors.
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
#ifndef INTERPROCESSOR_COMMS_BASE_H
#define INTERPROCESSOR_COMMS_BASE_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup Interprocessor
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "general_header.h"
#include "ring_buffer.h"
#include "error_config.h"
#include "utility_lib.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
/** @defgroup InterprocessorComms_Exported_Macros Macros
 * @{
 */
/**
 * @brief No of simultaneous inter-processor messages supported.
 */
#define INTERPROCESSOR_MSGS_SIZE					(64)
/**
 * @brief No of simultaneous inter-processor commands supported.
 */
#define INTERPROCESSOR_CMD_BUFF_SIZE				(1024)
/**
 * @brief No of simultaneous inter-processor responses supported.
 */
#define INTERPROCESSOR_RESPONSE_BUFF_SIZE			(1024)
/**
 * @}
 */
/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @defgroup InterprocessorComms_Exported_Typedefs Type Definitions
 * @{
 */
typedef enum
{
	SHARE_CONTROL_STATE,
	SHARE_BOOL_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_bools_t;
typedef enum
{
	SHARE_SAMPLE_U8,
	SHARE_U8_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_u8_t;
typedef enum
{
	SHARE_SAMPLE_U16,
	SHARE_U16_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_u16_t;
typedef enum
{
	SHARE_SAMPLE_U32,
	SHARE_U32_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_u32_t;
typedef enum
{
	SHARE_SAMPLE_S8,
	SHARE_S8_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_s8_t;
typedef enum
{
	SHARE_SAMPLE_S16,
	SHARE_S16_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_s16_t;
typedef enum
{
	SHARE_SAMPLE_S32,
	SHARE_S32_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_s32_t;
typedef enum
{
	SHARE_SAMPLE_FLOAT,
	SHARE_FLOAT_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_float_t;
typedef enum
{
	SHARE_SAMPLE_BIT_ACCESS,
	SHARE_BIT_ACCESS_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_bit_access_t;

/**
 * @}
 */
/*******************************************************************************
 * Structures
 ******************************************************************************/
#if IS_CONTROL_CORE
typedef struct
{
	bool state;
	volatile device_err_t err;
	volatile bool isPending;
} state_update_request;
#endif
 /*******************************************************************************
 * Exported Variables
 ******************************************************************************/
#if IS_CONTROL_CORE
extern state_update_request modeChangeRequest;
#endif
/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

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
