/**
 ********************************************************************************
 * @file    	p2p_comms.c
 * @author 		Waqas Ehsan Butt
 * @date    	April 23, 2021
 *
 * @brief   Controls the communication between the communication and controls processors.
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

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "p2p_comms.h"
#include "shared_memory.h"
#include "utility_lib.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
#if IS_COMMS_CORE
/**
 * @brief Describes the limits of each type of parameters
 * @note The order should strictly match @ref base_data_type_t
 */
static uint8_t p2pItemsConfig[DTYPE_COUNT] =
{
		P2P_BOOL_COUNT,
		P2P_U8_COUNT,
		P2P_U16_COUNT,
		P2P_U32_COUNT,
		P2P_S8_COUNT,
		P2P_S16_COUNT,
		P2P_S32_COUNT,
		P2P_FLOAT_COUNT,
		P2P_BIT_ACCESS_COUNT
};
#endif
#if IS_STORAGE_CORE
static uint32_t storageWordLen = 0;
#endif
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
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
__weak device_err_t P2PComms_SingleUpdateRequest_Blocking(p2p_msg_type_t type, uint8_t index, data_union_t value)
{
	// Disable IRQ to avoid clash
	__disable_irq();

	volatile p2p_msg_t* msg = &CORE_MSGS.msgs[CORE_MSGS.msgsRingBuff.wrIndex];
	msg->type = type;
	msg->firstReg = index;
	msg->cmdIndex = CORE_MSGS.cmdsRingBuff.wrIndex;
	CORE_MSGS.cmds[msg->cmdIndex] = value;
	msg->cmdLen = 1;
	msg->responseIndex = msg->responseLen = -1;
	RingBuffer_Write((ring_buffer_t*)&CORE_MSGS.msgsRingBuff);
	RingBuffer_Write((ring_buffer_t*)&CORE_MSGS.cmdsRingBuff);

	// Enable IRQ after process done
	__enable_irq();

	int temp = -1;
	while (temp == -1)
	{
		osDelay(1);
		temp = msg->responseIndex;
	}

	return (device_err_t)CORE_MSGS.response[msg->responseIndex].u8;
}
/**
 * @brief Validates that the parameter is valid.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Desired parameter.
 * @return <c>true</c> if valid else <c>false</c>.
 */
__weak bool P2PComms_IsParameterValid(data_param_info_t* _paramInfo)
{
	// Error if no parameter found
	// Error if parameter type not in range
	// Error if parameter index out of range
	if (_paramInfo == NULL || _paramInfo->type >= DTYPE_COUNT || _paramInfo->index >= p2pItemsConfig[_paramInfo->type])
		return false;
	return true;
}
/**
 * @brief Get value of a parameter shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Desired parameter.
 * @param value Writes the acquired value at this pointer location
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_GetValue(data_param_info_t* _paramInfo, data_union_t* value)
{
	if (!P2PComms_IsParameterValid(_paramInfo))
		return ERR_ILLEGAL;

	switch (_paramInfo->type)
	{
	case DTYPE_BOOL: value->b = INTER_CORE_DATA.bools[_paramInfo->index]; break;
	case DTYPE_U8: value->u8 = INTER_CORE_DATA.u8s[_paramInfo->index]; break;
	case DTYPE_U16: value->u16 = INTER_CORE_DATA.u16s[_paramInfo->index]; break;
	case DTYPE_U32: value->u32 = INTER_CORE_DATA.u32s[_paramInfo->index]; break;
	case DTYPE_S8: value->s8 = INTER_CORE_DATA.s8s[_paramInfo->index]; break;
	case DTYPE_S16: value->s16 = INTER_CORE_DATA.s16s[_paramInfo->index]; break;
	case DTYPE_S32: value->s32 = INTER_CORE_DATA.s32s[_paramInfo->index]; break;
	case DTYPE_FLOAT: value->f = INTER_CORE_DATA.floats[_paramInfo->index]; break;
	// if no bit is selected returns complete value, else returns the value of only required bits
	// The argument should be the bit mask for required bit
	case DTYPE_BIT_ACCESS:
		if (_paramInfo->arg == 0) value->bits = INTER_CORE_DATA.bitAccess[_paramInfo->index];
		else value->bits = INTER_CORE_DATA.bitAccess[_paramInfo->index] & _paramInfo->arg;
		break;
	default: return ERR_ILLEGAL;
	}
	return ERR_OK;
}
/**
 * @brief Update value of a parameter shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Desired parameter.
 * @param value Value to be updated.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateValue(data_param_info_t* _paramInfo, data_union_t value)
{
	if (!P2PComms_IsParameterValid(_paramInfo))
		return ERR_ILLEGAL;

	switch (_paramInfo->type)
	{
	case DTYPE_BOOL: return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_BOOL, _paramInfo->index, value);
	case DTYPE_U8: return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_U8, _paramInfo->index, value);
	case DTYPE_U16: return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_U16, _paramInfo->index, value);
	case DTYPE_U32: return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_U32, _paramInfo->index, value);
	case DTYPE_S8: return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_S8, _paramInfo->index, value);
	case DTYPE_S16: return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_S16, _paramInfo->index, value);
	case DTYPE_S32: return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_S32, _paramInfo->index, value);
	case DTYPE_FLOAT: return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_FLOAT, _paramInfo->index, value);
	case DTYPE_BIT_ACCESS:
		if (value.bits > BITS_SET_COUNT)
			return ERR_ILLEGAL;
		bits_set_types_t type =  (bits_set_types_t)value.bits;
		p2p_msg_type_t msgType = (type == BITS_SET ? MSG_SET_BITS : (type == BITS_CLR ? MSG_CLR_BITS : MSG_TOGGLE_BITS));
		data_union_t arg;
		arg.bits = _paramInfo->arg;
		return P2PComms_SingleUpdateRequest_Blocking(msgType, _paramInfo->index, arg);
	default:  return ERR_ILLEGAL;
	}
}
/**
 * @brief Get the value of a parameter (shared between both processors) in string format.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Relevant parameter information.
 * @param value text value to be updated.
 * @param addUnit If <c>true</c> append the unit of the parameter at the end of the string result.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_GetStringValue(data_param_info_t* _paramInfo, char* value, bool addUnit)
{
	data_union_t dataVal;
	device_err_t err = P2PComms_GetValue(_paramInfo, &dataVal);
	value[0] = 0;
	if (err != ERR_OK)
		return err;

	int len = 0;
	switch (_paramInfo->type)
	{
	case DTYPE_BOOL: (void) btoa_custom(dataVal.b, value); break;
	case DTYPE_U8: len = utoa_custom(dataVal.u8, value); break;
	case DTYPE_U16: len = utoa_custom(dataVal.u16, value); break;
	case DTYPE_U32: len = utoa_custom(dataVal.u32, value); break;
	case DTYPE_S8: len = itoa_custom(dataVal.s8, value); break;
	case DTYPE_S16: len = itoa_custom(dataVal.s16, value); break;
	case DTYPE_S32: len = itoa_custom(dataVal.s32, value); break;
	case DTYPE_FLOAT: len = ftoa_custom(dataVal.f, value, 7, _paramInfo->arg); break;
	case DTYPE_BIT_ACCESS:
		// --TODO-- Add bit modifications through strings
		return ERR_ILLEGAL;
	default: return ERR_ILLEGAL;
	}

	// if should display unit and has valid unit
	if (addUnit && _paramInfo->unit < UNIT_COUNT)
		strcat_custom(value, unitTxts[_paramInfo->unit], len, false);
	return ERR_OK;
}
/**
 * @brief Update the value of a parameter (shared between both processors) from a string value.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Relevant parameter information
 * @param value Text value used for updating the parameter
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateFromString(data_param_info_t* _paramInfo, const char* value)
{
	if (!P2PComms_IsParameterValid(_paramInfo))
		return ERR_ILLEGAL;
	data_union_t val;
	int32_t tempInt;
	switch (_paramInfo->type)
	{
	case DTYPE_BOOL:
		if (!atob_custom(value, &val.b)) return ERR_INVALID_TEXT;
		return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_BOOL, _paramInfo->index, val);
	case DTYPE_U8:
		if (!atou_custom(value, &val.u32)) return ERR_INVALID_TEXT;
		if (val.u32 > UCHAR_MAX) return ERR_OUT_OF_RANGE;
		return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_U8, _paramInfo->index, val);
	case DTYPE_U16:
		if (!atou_custom(value, &val.u32)) return ERR_INVALID_TEXT;
		if (val.u32 > USHRT_MAX) return ERR_OUT_OF_RANGE;
		return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_U16, _paramInfo->index, val);
	case DTYPE_U32:
		if (!atou_custom(value, &val.u32)) return ERR_INVALID_TEXT;
		return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_U32, _paramInfo->index, val);
	case DTYPE_S8:
		if (!atoi_custom(value, &tempInt)) return ERR_INVALID_TEXT;
		if (tempInt > SCHAR_MAX || tempInt < SCHAR_MIN) return ERR_OUT_OF_RANGE;
		val.s8 = tempInt;
		return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_S8, _paramInfo->index, val);
	case DTYPE_S16:
		if (!atoi_custom(value, &tempInt)) return ERR_INVALID_TEXT;
		if (tempInt > SHRT_MAX || tempInt < SHRT_MIN) return ERR_OUT_OF_RANGE;
		val.s16 = tempInt;
		return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_S16, _paramInfo->index, val);
	case DTYPE_S32:
		if (!atoi_custom(value, &val.s32)) return ERR_INVALID_TEXT;
		return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_S32, _paramInfo->index, val);
	case DTYPE_FLOAT:
		if (atof_custom(value, &val.f) == false) return ERR_INVALID_TEXT;
		return P2PComms_SingleUpdateRequest_Blocking(MSG_SET_FLOAT, _paramInfo->index, val);
	case DTYPE_BIT_ACCESS:
		// --TODO-- Add bit modifications through strings
		return ERR_ILLEGAL;
	default: return ERR_ILLEGAL;
	}
	return ERR_ILLEGAL;
}
/**
 * @brief Default function to get the data parameters in textual format, according to the parameter info.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Structure defining the parameter.
 * @param value Pointer to the location to be updated with the value
 * @param addUnit <c>true</c> if unit needs to be added with the text else false.
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
__weak device_err_t Default_GetDataParameter_InText(data_param_info_t* _paramInfo, char* value, bool addUnit)
{
	return P2PComms_GetStringValue(_paramInfo, value, addUnit);
}
/**
 * @brief  Default function to set the data parameters from textual value, according to the parameter info.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Structure defining the parameter.
 * @param value Value to be set as string
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
__weak device_err_t Default_SetDataParameter_FromText(data_param_info_t* _paramInfo, const char* value)
{
	return P2PComms_UpdateFromString(_paramInfo, value);
}
/**
 * @brief Default function to get the value of a parameter in string format.
 * @note This function is used whenever the @ref data_param_info_t.Getter function is not defined and @ref GetDataParameter() is called.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Information regarding the data parameter.
 * @param value text representation of the value acquired.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t Default_GetDataParameter(data_param_info_t* _paramInfo, data_union_t* value)
{
	return P2PComms_GetValue(_paramInfo, value);
}
/**
 * @brief Default function to set the value of a parameter from string format.
 * @note This function is used whenever the @ref data_param_info_t.Setter function is not defined and @ref SetDataParameter() is called.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param _paramInfo Information regarding the data parameter.
 * @param value text representation of the parameter value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t Default_SetDataParameter(data_param_info_t* _paramInfo, data_union_t value)
{
	return P2PComms_UpdateValue(_paramInfo, value);
}

#endif

#if IS_STORAGE_CORE

/**
 * @brief This function updates the state storage if needed. Call it periodically.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param data Data pointer
 * @param indexPtr Index of next data
 * @return Size of data that is updated. (Unit is 4-bytes)
 */
__weak uint32_t P2PComms_RefreshStates(uint32_t* data, uint32_t* indexPtr)
{
	uint32_t len = 0;
	p2p_data_buffs_t* dest = (p2p_data_buffs_t*)data;
	p2p_data_buffs_t* src = (p2p_data_buffs_t*)&INTER_CORE_DATA;

	// @note Only update values and signal to update if values have been changed
	if (P2PComms_IsStateStorageUpdateNeeded(dest, src))
	{
		P2PComms_UpdateStorableStates(dest, src);
		len = storageWordLen;
	}

	*indexPtr = 0;
	return len;
}

/**
 * @brief Configures the storage client information.
 * @param _config Pointer to the relevant configuration to be filled.
 */
void P2PComms_ConfigStorage(state_storage_client_t* _config)
{
	_config->InitStatesFromStorage = P2PComms_InitStatesFromStorage;
	_config->RefreshStates = P2PComms_RefreshStates;
	_config->dataWordLen = storageWordLen = (sizeof(p2p_data_buffs_t) / 4) + 1;
}

#endif

#if IS_CONTROL_CORE
/**
 * @brief Update a boolean parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateBool(uint8_t index, bool value)
{
	INTER_CORE_DATA.bools[index] = value;
	return ERR_OK;
}
/**
 * @brief Update a uint8_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateU8(uint8_t index, uint8_t value)
{
	INTER_CORE_DATA.u8s[index] = value;
	return ERR_OK;
}
/**
 * @brief Update a uint16_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateU16(uint8_t index, uint16_t value)
{
	INTER_CORE_DATA.u16s[index] = value;
	return ERR_OK;
}
/**
 * @brief Update a uint32_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateU32(uint8_t index, uint32_t value)
{
	INTER_CORE_DATA.u32s[index] = value;
	return ERR_OK;
}
/**
 * @brief Update a int8_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateS8(uint8_t index, int8_t value)
{
	INTER_CORE_DATA.s8s[index] = value;
	return ERR_OK;
}
/**
 * @brief Update a int16_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateS16(uint8_t index, int16_t value)
{
	INTER_CORE_DATA.s16s[index] = value;
	return ERR_OK;
}
/**
 * @brief Update a int32_t parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateS32(uint8_t index, int32_t value)
{
	INTER_CORE_DATA.s32s[index] = value;
	return ERR_OK;
}
/**
 * @brief Update a single-precision floating parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_UpdateFloat(uint8_t index, float value)
{
	INTER_CORE_DATA.floats[index] = value;
	return ERR_OK;
}
/**
 * @brief Set bits in bit accessible registers shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Bits to be set.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_SetBits(uint8_t index, uint8_t value)
{
	// if the bits are already set no need to reset them
	if ((INTER_CORE_DATA.bitAccess[index] & value) == value)
		return ERR_OK;

	if (index < P2P_BIT_ACCESS_COUNT)
		INTER_CORE_DATA.bitAccess[index] |= value;
	return ERR_OK;
}
/**
 * @brief Clear bits in bit accessible registers shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Bits to be cleared.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_ClearBits(uint8_t index, uint8_t value)
{
	// if the bits are already clear no need to clear them
	if ((INTER_CORE_DATA.bitAccess[index] & value) == 0)
		return ERR_OK;

	if (index < P2P_BIT_ACCESS_COUNT)
		INTER_CORE_DATA.bitAccess[index] &= ~value;
	return ERR_OK;
}
/**
 * @brief Toggle bits in bit accessible registers shared in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Bits to be toggled.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
__weak device_err_t P2PComms_ToggleBits(uint8_t index, uint8_t value)
{
	if (index < P2P_BIT_ACCESS_COUNT)
		INTER_CORE_DATA.bitAccess[index] ^= value;
	return ERR_OK;
}
/**
 * @brief Process the pending request for interprocessor communications.
 * @note Call this function frequently to make sure that interprocessor communications work flawlessly.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 */
__weak void P2PComms_ProcessPendingRequests(void)
{
	if (RingBuffer_IsEmpty((ring_buffer_t*)&CORE_MSGS.msgsRingBuff))
		return;

	device_err_t err = ERR_OK;
	p2p_msg_t* msg = (p2p_msg_t*)&CORE_MSGS.msgs[CORE_MSGS.msgsRingBuff.rdIndex];
	uint8_t index = msg->firstReg;
	if (msg->cmdLen == 1)
	{
		data_union_t* value = (data_union_t*)&CORE_MSGS.cmds[msg->cmdIndex];
		switch (msg->type)
		{
		case MSG_SET_BOOL: err = P2PComms_UpdateBool(index, value->b); break;
		case MSG_SET_U8: err = P2PComms_UpdateU8(index, value->u8); break;
		case MSG_SET_U16: err = P2PComms_UpdateU16(index, value->u16); break;
		case MSG_SET_U32: err = P2PComms_UpdateU32(index, value->u32); break;
		case MSG_SET_S8: err = P2PComms_UpdateS8(index, value->s8); break;
		case MSG_SET_S16: err = P2PComms_UpdateS16(index, value->s16); break;
		case MSG_SET_S32: err = P2PComms_UpdateS32(index, value->s32); break;
		case MSG_SET_FLOAT: err = P2PComms_UpdateFloat(index, value->f); break;
		case MSG_SET_BITS: err = P2PComms_SetBits(index, value->bits); break;
		case MSG_CLR_BITS: err = P2PComms_ClearBits(index, value->bits); break;
		case MSG_TOGGLE_BITS: err = P2PComms_ToggleBits(index, value->bits); break;
		default: err = ERR_ILLEGAL; break;
		}
	}

	msg->responseLen = 1;
	CORE_MSGS.response[CORE_MSGS.responseRingBuff.wrIndex].u8 = (device_err_t)err;
	msg->responseIndex = CORE_MSGS.responseRingBuff.wrIndex;

	RingBuffer_Write((ring_buffer_t*)&CORE_MSGS.responseRingBuff);
	RingBuffer_Read((ring_buffer_t*)&CORE_MSGS.msgsRingBuff);

}
/**
 * @brief Initialize the buffers and storage for the interprocessor communications.
 */
void P2PComms_InitData(void)
{
	CORE_MSGS.msgsRingBuff.modulo = P2P_COMMS_MSGS_SIZE - 1;
	CORE_MSGS.cmdsRingBuff.modulo = P2P_COMMS_CMD_BUFF_SIZE - 1;
	CORE_MSGS.responseRingBuff.modulo = P2P_COMMS_RESPONSE_BUFF_SIZE - 1;
	RingBuffer_Reset((ring_buffer_t*)&CORE_MSGS.msgsRingBuff);
	RingBuffer_Reset((ring_buffer_t*)&CORE_MSGS.cmdsRingBuff);
	RingBuffer_Reset((ring_buffer_t*)&CORE_MSGS.responseRingBuff);
}

#endif
/* EOF */
