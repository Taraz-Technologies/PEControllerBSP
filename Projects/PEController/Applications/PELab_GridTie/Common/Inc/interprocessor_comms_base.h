/**
 ********************************************************************************
 * @file 		pecontroller_digital_in.h
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
#define INTERPROCESSOR_MSGS_SIZE					(64)
#define INTERPROCESSOR_CMD_BUFF_SIZE				(1024)
#define INTERPROCESSOR_RESPONSE_BUFF_SIZE			(1024)
/*******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef enum
{
	// Controls
	SHARE_BOOST_STATE,
	SHARE_INVERTER_STATE,
	// Monitors
	SHARE_RELAY_STATUS,
	SHARE_PLL_STATUS,
	SHARE_BOOL_COUNT,
} shared_bools_t;
typedef enum
{
	SHARE_SAMPLE_U8,
	SHARE_U8_COUNT,
} shared_u8_t;
typedef enum
{
	SHARE_SAMPLE_U16,
	SHARE_U16_COUNT,
} shared_u16_t;
typedef enum
{
	SHARE_SAMPLE_U32,
	SHARE_U32_COUNT,
} shared_u32_t;
typedef enum
{
	SHARE_SAMPLE_S8,
	SHARE_S8_COUNT,
} shared_s8_t;
typedef enum
{
	SHARE_SAMPLE_S16,
	SHARE_S16_COUNT,
} shared_s16_t;
typedef enum
{
	SHARE_SAMPLE_S32,
	SHARE_S32_COUNT,
} shared_s32_t;
typedef enum
{
	// Controls
	SHARE_GRID_FREQ,
	SHARE_GRID_VOLTAGE,
	SHARE_REQ_RMS_CURRENT,
	SHARE_LOUT,
	// Monitors
	SHARE_CURR_RMS_CURRENT,
	SHARE_FLOAT_COUNT,
} shared_float_t;
typedef enum
{
	SHARE_SAMPLE_BIT_ACCESS,
	SHARE_BIT_ACCESS_COUNT,
} shared_bit_access_t;
#if IS_CONTROL_CORE
typedef struct
{
	bool state;
	volatile device_err_t err;
	volatile bool isPending;
} state_update_request;
#endif
/*******************************************************************************
 * Structures
 ******************************************************************************/

 /*******************************************************************************
 * Exported Variables
 ******************************************************************************/
#if IS_CONTROL_CORE
extern state_update_request boostStateUpdateRequest;
extern state_update_request inverterStateUpdateRequest;
#endif
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
