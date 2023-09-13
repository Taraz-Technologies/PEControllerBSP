/**
 ********************************************************************************
 * @file 		interprocessor_comms.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 26, 2023
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
#ifndef INTERPROCESSOR_COMMS_H
#define INTERPROCESSOR_COMMS_H

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
#include "interprocessor_comms_base.h"
#include "state_storage_lib.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @defgroup InterprocessorComms_Exported_Typedefs Type Definitions
 * @{
 */
/**
 * @brief Defines the available message types for interprocessor communications
 */
typedef enum
{
	MSG_SET_BOOL = 0, /**< Message to set boolean parameters */
	MSG_SET_U8,       /**< Message to set uint8_t parameters */
	MSG_SET_S8,       /**< Message to set int8_t parameters */
	MSG_SET_U16,      /**< Message to set uint16_t parameters */
	MSG_SET_S16,      /**< Message to set int16_t parameters */
	MSG_SET_U32,      /**< Message to set uint32_t parameters */
	MSG_SET_S32,      /**< Message to set int32_t parameters */
	MSG_SET_FLOAT,    /**< Message to set single precision parameters */
	MSG_SET_BITS,     /**< Message to set bits in a register */
	MSG_CLR_BITS,     /**< Message to clear bits in a register */
	MSG_TOGGLE_BITS,  /**< Message to toggle bits in a register */
	MSG_GET_BOOL = 30,/**< Message to get boolean parameter value */
	MSG_GET_U8,       /**< Message to get uint8_t parameter value */
	MSG_GET_S8,       /**< Message to get int8_t parameter value */
	MSG_GET_U16,      /**< Message to get uint16_t parameter value */
	MSG_GET_S16,      /**< Message to get int16_t parameter value */
	MSG_GET_U32,      /**< Message to get uint32_t parameter value */
	MSG_GET_S32,      /**< Message to get int32_t parameter value */
	MSG_GET_FLOAT,    /**< Message to get single precision parameter value */
} interprocessor_msg_type_t;
/**
 * @brief
 */
typedef enum
{
	BITS_SET,     /**< BITS_SET */
	BITS_CLR,     /**< BITS_CLR */
	BITS_TOGGLE,  /**< BITS_TOGGLE */
	BITS_SET_COUNT/**< BITS_SET_COUNT */
} bits_set_types_t;
/**
 * @}
 */
/*******************************************************************************
 * Structures
 ******************************************************************************/
/** @defgroup InterprocessorComms_Exported_Structures Structures
 * @{
 */
/**
 * @brief Defines the interprocessor message in message queue.
 */
typedef struct
{
	interprocessor_msg_type_t type;	/*!< Message type */
	uint8_t firstReg;				/*!< First register to be written / read */
	int cmdIndex;					/*!< Data index in the command buffer @ref interprocessor_msg_data_t.cmds */
	int cmdLen;						/*!< Data length in command buffer @ref interprocessor_msg_data_t.cmds */
	int responseIndex;				/*!< Data index in the response buffer @ref interprocessor_msg_data_t.response */
	int responseLen;				/*!< Data length in response buffer @ref interprocessor_msg_data_t.response */
} interprocessor_msg_t;
/**
 * @brief Defines the shared data buffers between both processors.
 */
typedef struct
{
	bool bools[SHARE_BOOL_COUNT];					/*!< Contains all shared boolean variables */
	uint8_t u8s[SHARE_U8_COUNT];					/*!< Contains all shared uint8_t variables */
	int8_t s8s[SHARE_S8_COUNT];						/*!< Contains all shared int8_t variables */
	uint16_t u16s[SHARE_U16_COUNT];					/*!< Contains all shared uint16_t variables */
	int16_t s16s[SHARE_S16_COUNT];					/*!< Contains all shared int16_t variables */
	uint32_t u32s[SHARE_U32_COUNT];					/*!< Contains all shared uint32_t variables */
	int32_t s32s[SHARE_S32_COUNT];					/*!< Contains all shared int32_t variables */
	float floats[SHARE_FLOAT_COUNT];				/*!< Contains all shared single precisions variables */
	uint32_t bitAccess[SHARE_BIT_ACCESS_COUNT];		/*!< Contains all shared bit accessable registers */
} interprocessor_data_buffs_t;
/**
 * @brief Defines the interprocessor messaging data.
 */
typedef struct
{
	interprocessor_data_buffs_t dataBuffs;								/*!< Shared data buffers for both processors */
	ring_buffer_t msgsRingBuff;											/*!< Ring buffer keeping message buffer info */
	ring_buffer_t cmdsRingBuff;											/*!< Ring buffer keeping command buffer info */
	ring_buffer_t responseRingBuff;										/*!< Ring buffer keeping response buffer info */
	volatile interprocessor_msg_t msgs[INTERPROCESSOR_MSGS_SIZE];		/*!< Message buffer */
	data_union_t cmds[INTERPROCESSOR_CMD_BUFF_SIZE];					/*!< Command buffer */
	data_union_t response[INTERPROCESSOR_RESPONSE_BUFF_SIZE];			/*!< Response buffer */
} interprocessor_msg_data_t;
/**
 * @}
 */
/*******************************************************************************
 * Exported Variables
 ******************************************************************************/

/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @defgroup InterprocessorComms_Exported_Functions Functions
 * @{
 */
#if IS_COMMS_CORE
extern device_err_t Interprocessor_UpdateFromString(data_param_info_t* _paramInfo, const char* value);
extern device_err_t Interprocessor_GetStringValue(data_param_info_t* _paramInfo, char* value, bool addUnit);
#endif
#if IS_CONTROL_CORE
extern void InterprocessorComms_ProcessPendingRequests(void);
extern void InterprocessorComms_InitData(void);
#endif
#if IS_STORAGE_CORE
/**
 * @brief Configures the storage client information.
 * @param _config Pointer to the relevant configuration to be filled.
 */
extern void BSP_InterProcessorMsgs_ConfigStorage(state_storage_client_t* _config);
#endif
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
