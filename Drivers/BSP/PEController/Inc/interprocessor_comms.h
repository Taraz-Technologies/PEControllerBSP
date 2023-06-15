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
/**
 * @brief
 *
 */
typedef enum
{
	MSG_SET_BOOL = 0, /**< MSG_SET_BOOL */
	MSG_SET_U8,       /**< MSG_SET_U8 */
	MSG_SET_S8,       /**< MSG_SET_S8 */
	MSG_SET_U16,      /**< MSG_SET_U16 */
	MSG_SET_S16,      /**< MSG_SET_S16 */
	MSG_SET_U32,      /**< MSG_SET_U32 */
	MSG_SET_S32,      /**< MSG_SET_S32 */
	MSG_SET_FLOAT,    /**< MSG_SET_FLOAT */
	MSG_SET_BITS,     /**< MSG_SET_BITS */
	MSG_CLR_BITS,     /**< MSG_CLR_BITS */
	MSG_TOGGLE_BITS,  /**< MSG_TOGGLE_BITS */
	MSG_GET_BOOL = 30,/**< MSG_GET_BOOL */
	MSG_GET_U8,       /**< MSG_GET_U8 */
	MSG_GET_S8,       /**< MSG_GET_S8 */
	MSG_GET_U16,      /**< MSG_GET_U16 */
	MSG_GET_S16,      /**< MSG_GET_S16 */
	MSG_GET_U32,      /**< MSG_GET_U32 */
	MSG_GET_S32,      /**< MSG_GET_S32 */
	MSG_GET_FLOAT,    /**< MSG_GET_FLOAT */
} interprocessor_msg_type_t;
typedef enum
{
	BITS_SET,
	BITS_CLR,
	BITS_TOGGLE,
	BITS_SET_COUNT
} bits_set_types_t;
/*******************************************************************************
 * Structures
 ******************************************************************************/
typedef struct
{
	interprocessor_msg_type_t type;
	uint8_t firstReg;
	int cmdIndex;
	int cmdLen;
	int responseIndex;
	int responseLen;
} interprocessor_msg_t;
typedef struct
{
	bool bools[SHARE_BOOL_COUNT];
	uint8_t u8s[SHARE_U8_COUNT];
	int8_t s8s[SHARE_S8_COUNT];
	uint16_t u16s[SHARE_U16_COUNT];
	int16_t s16s[SHARE_S16_COUNT];
	uint32_t u32s[SHARE_U32_COUNT];
	int32_t s32s[SHARE_S32_COUNT];
	float floats[SHARE_FLOAT_COUNT];
	uint32_t bitAccess[SHARE_BIT_ACCESS_COUNT];
} interprocessor_data_buffs_t;
typedef struct
{
	interprocessor_data_buffs_t dataBuffs;
	ring_buffer_t msgsRingBuff;
	ring_buffer_t cmdsRingBuff;
	ring_buffer_t responseRingBuff;
	volatile interprocessor_msg_t msgs[INTERPROCESSOR_MSGS_SIZE];
	data_union_t cmds[INTERPROCESSOR_CMD_BUFF_SIZE];
	data_union_t response[INTERPROCESSOR_RESPONSE_BUFF_SIZE];
} interprocessor_msg_data_t;
/*******************************************************************************
 * Exported Variables
 ******************************************************************************/

/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/

/** @defgroup DIN_Exported_Functions Functions
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

/**
 * @}
 */
#endif
/* EOF */
