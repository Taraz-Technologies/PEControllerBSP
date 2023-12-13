/**
 ********************************************************************************
 * @file 		p2p_comms_app.h
 * @author 		Waqas Ehsan Butt
 * @date 		September 25, 2021
 *
 * @brief	Controls the aplplication specific communication between the communication and controls processors.
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
#ifndef P2PCOMMS_APP_H
#define P2PCOMMS_APP_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup P2PCOMMS
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "general_header.h"
#include "ring_buffer.h"
#include "error_config.h"
#include "utility_lib.h"
#include "grid_tie_config.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
/** @defgroup P2PComms_Exported_Macros Macros
 * @{
 */
/**
 * @brief No of simultaneous inter-processor messages supported.
 */
#define P2P_COMMS_MSGS_SIZE					(64)
/**
 * @brief No of simultaneous inter-processor commands supported.
 */
#define P2P_COMMS_CMD_BUFF_SIZE				(1024)
/**
 * @brief No of simultaneous inter-processor responses supported.
 */
#define P2P_COMMS_RESPONSE_BUFF_SIZE			(1024)
/**
 * @}
 */
/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @defgroup P2PComms_Exported_Typedefs Type Definitions
 * @{
 */
/**
 * @brief Defines the shared boolean parameters between both processors
 */
typedef enum
{
	// Controls
	P2P_BOOST_STATE,
	P2P_INVERTER_STATE,
	// Monitors
	P2P_RELAY_STATUS,
	P2P_PLL_STATUS,
	P2P_BOOL_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_bools_t;
/**
 * @brief Defines the shared uint8_t parameters between both processors
 */
typedef enum
{
	P2P_SAMPLE_U8,
	P2P_U8_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_u8_t;
/**
 * @brief Defines the shared uint16_t parameters between both processors
 */
typedef enum
{
	P2P_SAMPLE_U16,
	P2P_U16_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_u16_t;
/**
 * @brief Defines the shared uint32_t parameters between both processors
 */
typedef enum
{
	P2P_SAMPLE_U32,
	P2P_U32_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_u32_t;
/**
 * @brief Defines the shared int8_t parameters between both processors
 */
typedef enum
{
	P2P_SAMPLE_S8,
	P2P_S8_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_s8_t;
/**
 * @brief Defines the shared int16_t parameters between both processors
 */
typedef enum
{
	P2P_SAMPLE_S16,
	P2P_S16_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_s16_t;
/**
 * @brief Defines the shared int32_t parameters between both processors
 */
typedef enum
{
	P2P_SAMPLE_S32,
	P2P_S32_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_s32_t;
/**
 * @brief Defines the single-precision floating point parameters between both processors
 */
typedef enum
{
	// Controls
	P2P_GRID_FREQ,
	P2P_GRID_VOLTAGE,
	P2P_REQ_RMS_CURRENT,
	P2P_LOUT_mH,
	// Monitors
	P2P_CURR_RMS_CURRENT,
	P2P_FLOAT_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_float_t;
/**
 * @brief Defines the bit accessible parameters between both processors
 */
typedef enum
{
	P2P_SAMPLE_BIT_ACCESS,
	P2P_BIT_ACCESS_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_bit_access_t;
#if IS_COMMS_CORE
/**
 * @brief Defines all the parameters shared in both cores which need a textual description.
 * @note The sequence here should match the sequence in @ref p2pCommsParams for
 * correct display of parameters.
 */
typedef enum
{
	P2P_PARAM_f_GRID,
	P2P_PARAM_V_GRID,
	P2P_PARAM_I_RMS_REQ,
	P2P_PARAM_L_OUT_mH,
	P2P_PARAM_I_RMS,
	P2P_PARAM_BOOST_EN,
	P2P_PARAM_INV_EN,
	P2P_PARAM_RELAY_STS,
	P2P_PARAM_PLL_STS,
	P2P_PARAM_COUNT,   /**< Not a type. Use this to get the total number of legal types */
} p2p_params_type_t;
#endif
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
extern state_update_request boostStateUpdateRequest;
extern state_update_request inverterStateUpdateRequest;
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
