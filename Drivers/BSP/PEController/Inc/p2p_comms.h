/**
 ********************************************************************************
 * @file 		p2p_comms.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 26, 2023
 *
 * @brief	Controls the general communication between the communication and controls processors.
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
#ifndef P2PCOMMS_H
#define P2PCOMMS_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @defgroup P2PCOMMS Processor to Processor Communication
 * @brief Controls the communication between the communication and controls processors.
 * @details The BSP has the following structure
 * -The CM4 core is responsible for the external communication such as Fiber, SPI, Ethernet, USB.
 * -The CM7 core is responsible for ADC acquistion and control implementation.
 * The CM4-CM7 communication provided is divided into two portions
 * -# <b>Data Buffers:</b> These are defined in @ref p2p_data_buffs_t and are shared between both cores.
 * Both cores can access these shared registers because they are in a shared accessible space.
 * Users can store parameters meant to be shared between both cores, such as current power or current,
 * in these data buffers
 * -# <b>Messaging from CM4 to CM7:</b> This is controlled using @ref p2p_msg_t.
 * These messages are used to update settings and parameter values in the CM7 core.
 * These values cannot be directly updated in the registers because they are critical for the
 * control system and cannot be changed without a specific control sequence from the CM7 core.
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "p2p_comms_app.h"
#include "state_storage_lib.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @defgroup P2PComms_Exported_Typedefs Type Definitions
 * @{
 */
/**
 * @brief Defines the available message types for processor to processor communications
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
} p2p_msg_type_t;
/**
 * @brief Defines types of bits management
 */
typedef enum
{
	BITS_SET,     /**< Select to set bits */
	BITS_CLR,     /**< Select to clear bits */
	BITS_TOGGLE,  /**< Select to toggle bits */
	BITS_SET_COUNT/**< Not a type. Use this to get the total number of legal types */
} bits_set_types_t;
/**
 * @}
 */
/*******************************************************************************
 * Structures
 ******************************************************************************/
/** @defgroup P2PComms_Exported_Structures Structures
 * @{
 */
/**
 * @brief Defines the processor to processor message in message queue.
 */
typedef struct
{
	p2p_msg_type_t type;	/*!< Message type */
	uint8_t firstReg;				/*!< First register to be written / read */
	int cmdIndex;					/*!< Data index in the command buffer @ref p2p_msg_data_t.cmds */
	int cmdLen;						/*!< Data length in command buffer @ref p2p_msg_data_t.cmds */
	int responseIndex;				/*!< Data index in the response buffer @ref p2p_msg_data_t.response */
	int responseLen;				/*!< Data length in response buffer @ref p2p_msg_data_t.response */
} p2p_msg_t;
/**
 * @brief Defines the shared data buffers between both processors.
 */
typedef struct
{
	bool bools[P2P_BOOL_COUNT];					/*!< Contains all shared boolean variables */
	uint8_t u8s[P2P_U8_COUNT];					/*!< Contains all shared uint8_t variables */
	int8_t s8s[P2P_S8_COUNT];					/*!< Contains all shared int8_t variables */
	uint16_t u16s[P2P_U16_COUNT];				/*!< Contains all shared uint16_t variables */
	int16_t s16s[P2P_S16_COUNT];				/*!< Contains all shared int16_t variables */
	uint32_t u32s[P2P_U32_COUNT];				/*!< Contains all shared uint32_t variables */
	int32_t s32s[P2P_S32_COUNT];				/*!< Contains all shared int32_t variables */
	float floats[P2P_FLOAT_COUNT];				/*!< Contains all shared single precisions variables */
	uint32_t bitAccess[P2P_BIT_ACCESS_COUNT];	/*!< Contains all shared bit accessible registers */
} p2p_data_buffs_t;
/**
 * @brief Defines the processor to processor messaging data.
 */
typedef struct
{
	p2p_data_buffs_t dataBuffs;							/*!< Shared data buffers for both processors */
	ring_buffer_t msgsRingBuff;							/*!< Ring buffer keeping message buffer info */
	ring_buffer_t cmdsRingBuff;							/*!< Ring buffer keeping command buffer info */
	ring_buffer_t responseRingBuff;						/*!< Ring buffer keeping response buffer info */
	volatile p2p_msg_t msgs[P2P_COMMS_MSGS_SIZE];		/*!< Message buffer */
	data_union_t cmds[P2P_COMMS_CMD_BUFF_SIZE];			/*!< Command buffer */
	data_union_t response[P2P_COMMS_RESPONSE_BUFF_SIZE];/*!< Response buffer */
} p2p_msg_data_t;
/**
 * @}
 */
/*******************************************************************************
 * Exported Variables
 ******************************************************************************/
/** @defgroup P2PComms_Exported_Variables Variables
 * @{
 */
#if IS_COMMS_CORE
/**
 * @brief Describes all the parameters shared in both cores which need a textual description.
 * @note The sequence here should match the sequence in @ref p2p_params_type_t for
 * correct display of parameters.
 */
extern data_param_info_t p2pCommsParams[P2P_PARAM_COUNT];
#endif
/**
 * @}
 */
/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @defgroup P2PComms_Exported_Functions Functions
 * @{
 */
#if IS_COMMS_CORE
/**
 * @brief Update a parameter in control.
 * @note This is a blocking call and will return only after request is completed.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param type Type of message to be delivered.
 * @param index Register index.
 * @param value Data value to be updated.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_SingleUpdateRequest_Blocking(p2p_msg_type_t type, uint8_t index, data_union_t value);
/**
 * @brief Validates that the parameter is valid.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Desired parameter.
 * @return <c>true</c> if valid else <c>false</c>.
 */
extern bool P2PComms_IsParameterValid(data_param_info_t* _paramInfo);
/**
 * @brief Get value of a parameter shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Desired parameter.
 * @param value Writes the acquired value at this pointer location
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_GetValue(data_param_info_t* _paramInfo, data_union_t* value);
/**
 * @brief Update value of a parameter shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Desired parameter.
 * @param value Value to be updated.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateValue(data_param_info_t* _paramInfo, data_union_t value);
/**
 * @brief Update the value of a parameter (shared between both processors) from a string value.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Relevant parameter information
 * @param value Text value used for updating the parameter
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateFromString(data_param_info_t* _paramInfo, const char* value);
/**
 * @brief Get the value of a parameter (shared between both processors) in string format.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Relevant parameter information.
 * @param value text value to be updated.
 * @param addUnit If <c>true</c> append the unit of the parameter at the end of the string result.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_GetStringValue(data_param_info_t* _paramInfo, char* value, bool addUnit);
#endif
#if IS_CONTROL_CORE
/**
 * @brief Update a boolean parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateBool(uint8_t index, bool value);
/**
 * @brief Update a uint8_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateU8(uint8_t index, uint8_t value);
/**
 * @brief Update a uint16_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateU16(uint8_t index, uint16_t value);
/**
 * @brief Update a uint32_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateU32(uint8_t index, uint32_t value);
/**
 * @brief Update a int8_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateS8(uint8_t index, int8_t value);
/**
 * @brief Update a int16_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateS16(uint8_t index, int16_t value);
/**
 * @brief Update a int32_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateS32(uint8_t index, int32_t value);
/**
 * @brief Update a single-precision floating parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_UpdateFloat(uint8_t index, float value);
/**
 * @brief Set bits in bit accessible registers shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Bits to be set.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_SetBits(uint8_t index, uint8_t value);
/**
 * @brief Clear bits in bit accessible registers shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Bits to be cleared.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_ClearBits(uint8_t index, uint8_t value);
/**
 * @brief Toggle bits in bit accessible registers shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Bits to be toggled.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
extern device_err_t P2PComms_ToggleBits(uint8_t index, uint8_t value);
/**
 * @brief Process the pending request for interprocessor communications.
 * @note Call this function frequently to make sure that interprocessor communications work flawlessly.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 */
extern void P2PComms_ProcessPendingRequests(void);
/**
 * @brief Initialize the buffers and storage for the interprocessor communications.
 */
extern void P2PComms_InitData(void);
#endif
#if IS_STORAGE_CORE
/**
 * @brief Check if the flash needs to be updated.
 * @param dest Destination
 * @param src Source
 * @return <c>true</c> if update required else <c>false</c>.
 */
extern bool P2PComms_IsStateStorageUpdateNeeded(p2p_data_buffs_t* dest, p2p_data_buffs_t* src);
/**
 * @brief Update all store-able states from source to destination.
 * @param dest Destination
 * @param src Source
 */
extern void P2PComms_UpdateStorableStates(p2p_data_buffs_t* dest, p2p_data_buffs_t* src);
/**
 * @brief Initialize the states from the flash storage.
 * @param data Data pointer.
 * @param isDataValid <c>true</c> if the flash data is valid.
 */
extern void P2PComms_InitStatesFromStorage(uint32_t* data, bool isDataValid);
/**
 * @brief This function updates the state storage if needed. Call it periodically.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param data Data pointer
 * @param indexPtr Index of next data
 * @return Size of data that is updated. (Unit is 4-bytes)
 */
extern uint32_t P2PComms_RefreshStates(uint32_t* data, uint32_t* indexPtr);
/**
 * @brief Configures the storage client information.
 * @param _config Pointer to the relevant configuration to be filled.
 */
extern void P2PComms_ConfigStorage(state_storage_client_t* _config);
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
