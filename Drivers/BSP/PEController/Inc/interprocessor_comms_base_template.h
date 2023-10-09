/**
 ********************************************************************************
 * @file 		interprocessor_comms_base_template.h
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
/**
 * @brief Defines the shared boolean variables between cores
 */
typedef enum
{
	SHARE_SAMPLE_BOOL,	/**< Defines Sample boolean shared variable */
	SHARE_BOOL_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_bools_t;
/**
 * @brief Defines the shared uint8_t variables between cores
 */
typedef enum
{
	SHARE_SAMPLE_U8,  /**< Defines Sample uint8_t shared variable */
	SHARE_U8_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_u8_t;
/**
 * @brief Defines the shared uint16_t variables between cores
 */
typedef enum
{
	SHARE_SAMPLE_U16,  /**< Defines Sample uint16_t shared variable */
	SHARE_U16_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_u16_t;
/**
 * @brief Defines the shared uint32_t variables between cores
 */
typedef enum
{
	SHARE_SAMPLE_U32,  /**< Defines Sample uint32_t shared variable */
	SHARE_U32_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_u32_t;
/**
 * @brief Defines the shared int8_t variables between cores
 */
typedef enum
{
	SHARE_SAMPLE_S8,  /**< Defines Sample int8_t shared variable */
	SHARE_S8_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_s8_t;
/**
 * @brief Defines the shared int16_t variables between cores
 */
typedef enum
{
	SHARE_SAMPLE_S16,  /**< Defines Sample int16_t shared variable */
	SHARE_S16_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_s16_t;
/**
 * @brief Defines the shared int32_t variables between cores
 */
typedef enum
{
	SHARE_SAMPLE_S32,  /**< Defines Sample int32_t shared variable */
	SHARE_S32_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_s32_t;
/**
 * @brief Defines the shared single precision variables between cores
 */
typedef enum
{
	SHARE_SAMPLE_FLOAT,  /**< Defines Sample single precision shared variable */
	SHARE_FLOAT_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_float_t;
/**
 * @brief Defines the shared bit accessible variables between cores
 */
typedef enum
{
	SHARE_SAMPLE_BIT_ACCESS,  /**< Defines Sample bit accessible shared variable */
	SHARE_BIT_ACCESS_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} shared_bit_access_t;
/**
 * @}
 */
/*******************************************************************************
 * Structures
 ******************************************************************************/

 /*******************************************************************************
 * Exported Variables
 ******************************************************************************/

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
