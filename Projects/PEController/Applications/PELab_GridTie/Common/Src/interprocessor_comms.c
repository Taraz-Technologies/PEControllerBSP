/**
 ********************************************************************************
 * @file    	interprocessor_comms.c
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
#include "interprocessor_comms.h"
#include "shared_memory.h"
#include "utility_lib.h"
#include "grid_tie_config.h"
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
static uint8_t sharedItemsConfig[DTYPE_COUNT] =
{
		SHARE_BOOL_COUNT,
		SHARE_U8_COUNT,
		SHARE_U16_COUNT,
		SHARE_U32_COUNT,
		SHARE_S8_COUNT,
		SHARE_S16_COUNT,
		SHARE_S32_COUNT,
		SHARE_FLOAT_COUNT,
		SHARE_BIT_ACCESS_COUNT
};
#endif
#if IS_STORAGE_CORE
static uint32_t storageWordLen = 0;
#endif
/********************************************************************************
 * Global Variables
 *******************************************************************************/
#if IS_CONTROL_CORE
state_update_request boostStateUpdateRequest = { .isPending = false };
state_update_request inverterStateUpdateRequest = { .isPending = false };
#endif
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
#if IS_COMMS_CORE
device_err_t SingleUpdateRequest_Blocking(interprocessor_msg_type_t type, uint8_t index, data_union_t value)
{
	// Disable IRQ to avoid clash
	__disable_irq();

	volatile interprocessor_msg_t* msg = &CORE_MSGS.msgs[CORE_MSGS.msgsRingBuff.wrIndex];
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

static bool IsParameterValid(data_param_info_t* _paramInfo)
{
	// Error if no parameter found
	// Error if parameter type not in range
	// Error if parameter index out of range
	if (_paramInfo == NULL || _paramInfo->type >= DTYPE_COUNT || _paramInfo->index >= sharedItemsConfig[_paramInfo->type])
		return false;
	return true;
}

device_err_t Interprocessor_GetValue(data_param_info_t* _paramInfo, data_union_t* value)
{
	if (!IsParameterValid(_paramInfo))
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

device_err_t Interprocessor_UpdateValue(data_param_info_t* _paramInfo, data_union_t value)
{
	if (!IsParameterValid(_paramInfo))
		return ERR_ILLEGAL;

	switch (_paramInfo->type)
	{
	case DTYPE_BOOL: return SingleUpdateRequest_Blocking(MSG_SET_BOOL, _paramInfo->index, value);
	case DTYPE_U8: return SingleUpdateRequest_Blocking(MSG_SET_U8, _paramInfo->index, value);
	case DTYPE_U16: return SingleUpdateRequest_Blocking(MSG_SET_U16, _paramInfo->index, value);
	case DTYPE_U32: return SingleUpdateRequest_Blocking(MSG_SET_U32, _paramInfo->index, value);
	case DTYPE_S8: return SingleUpdateRequest_Blocking(MSG_SET_S8, _paramInfo->index, value);
	case DTYPE_S16: return SingleUpdateRequest_Blocking(MSG_SET_S16, _paramInfo->index, value);
	case DTYPE_S32: return SingleUpdateRequest_Blocking(MSG_SET_S32, _paramInfo->index, value);
	case DTYPE_FLOAT: return SingleUpdateRequest_Blocking(MSG_SET_FLOAT, _paramInfo->index, value);
	case DTYPE_BIT_ACCESS:
		if (value.bits > BITS_SET_COUNT)
			return ERR_ILLEGAL;
		bits_set_types_t type =  (bits_set_types_t)value.bits;
		interprocessor_msg_type_t msgType = (type == BITS_SET ? MSG_SET_BITS : (type == BITS_CLR ? MSG_CLR_BITS : MSG_TOGGLE_BITS));
		data_union_t arg;
		arg.bits = _paramInfo->arg;
		return SingleUpdateRequest_Blocking(msgType, _paramInfo->index, arg);
	default:  return ERR_ILLEGAL;
	}
}
device_err_t Interprocessor_GetStringValue(data_param_info_t* _paramInfo, char* value, bool addUnit)
{
	data_union_t dataVal;
	device_err_t err = Interprocessor_GetValue(_paramInfo, &dataVal);
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
device_err_t Interprocessor_UpdateFromString(data_param_info_t* _paramInfo, const char* value)
{
	if (!IsParameterValid(_paramInfo))
		return ERR_ILLEGAL;
	data_union_t val;
	int32_t tempInt;
	switch (_paramInfo->type)
	{
	case DTYPE_BOOL:
		if (!atob_custom(value, &val.b)) return ERR_INVALID_TEXT;
		return SingleUpdateRequest_Blocking(MSG_SET_BOOL, _paramInfo->index, val);
	case DTYPE_U8:
		if (!atou_custom(value, &val.u32)) return ERR_INVALID_TEXT;
		if (val.u32 > UCHAR_MAX) return ERR_OUT_OF_RANGE;
		return SingleUpdateRequest_Blocking(MSG_SET_U8, _paramInfo->index, val);
	case DTYPE_U16:
		if (!atou_custom(value, &val.u32)) return ERR_INVALID_TEXT;
		if (val.u32 > USHRT_MAX) return ERR_OUT_OF_RANGE;
		return SingleUpdateRequest_Blocking(MSG_SET_U16, _paramInfo->index, val);
	case DTYPE_U32:
		if (!atou_custom(value, &val.u32)) return ERR_INVALID_TEXT;
		return SingleUpdateRequest_Blocking(MSG_SET_U32, _paramInfo->index, val);
	case DTYPE_S8:
		if (!atoi_custom(value, &tempInt)) return ERR_INVALID_TEXT;
		if (tempInt > SCHAR_MAX || tempInt < SCHAR_MIN) return ERR_OUT_OF_RANGE;
		val.s8 = tempInt;
		return SingleUpdateRequest_Blocking(MSG_SET_S8, _paramInfo->index, val);
	case DTYPE_S16:
		if (!atoi_custom(value, &tempInt)) return ERR_INVALID_TEXT;
		if (tempInt > SHRT_MAX || tempInt < SHRT_MIN) return ERR_OUT_OF_RANGE;
		val.s16 = tempInt;
		return SingleUpdateRequest_Blocking(MSG_SET_S16, _paramInfo->index, val);
	case DTYPE_S32:
		if (!atoi_custom(value, &val.s32)) return ERR_INVALID_TEXT;
		return SingleUpdateRequest_Blocking(MSG_SET_S32, _paramInfo->index, val);
	case DTYPE_FLOAT:
		if (atof_custom(value, &val.f) == false) return ERR_INVALID_TEXT;
		return SingleUpdateRequest_Blocking(MSG_SET_FLOAT, _paramInfo->index, val);
	case DTYPE_BIT_ACCESS:
		// --TODO-- Add bit modifications through strings
		return ERR_ILLEGAL;
	default: return ERR_ILLEGAL;
	}
	return ERR_ILLEGAL;
}
/**
 * @brief Default function to get the data parameters in textual format, according to the parameter info
 * @param _paramInfo Structure defining the parameter.
 * @param value Pointer to the location to be updated with the value
 * @param addUnit <c>true</c> if unit needs to be added with the text else false.
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
device_err_t Default_GetDataParameter_InText(data_param_info_t* _paramInfo, char* value, bool addUnit)
{
	return Interprocessor_GetStringValue(_paramInfo, value, addUnit);
}
/**
 * @brief  Default function to set the data parameters from textual value, according to the parameter info
 * @param _paramInfo Structure defining the parameter.
 * @param value Value to be set as string
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
device_err_t Default_SetDataParameter_FromText(data_param_info_t* _paramInfo, const char* value)
{
	return Interprocessor_UpdateFromString(_paramInfo, value);
}
/**
 * @brief  Default function to get the data parameters according to the parameter info
 * @param _paramInfo Structure defining the parameter.
 * @param value Pointer to the location to be updated with the value
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
device_err_t Default_GetDataParameter(data_param_info_t* _paramInfo, data_union_t* value)
{
	return Interprocessor_GetValue(_paramInfo, value);
}
/**
 * @brief  Default function to set the data parameters according to the parameter info
 * @param _paramInfo Structure defining the parameter.
 * @param value Value to be set
 * @return <c>ERR_OK</c> if no error else appropriate error thrown
 */
device_err_t Default_SetDataParameter(data_param_info_t* _paramInfo, data_union_t value)
{
	return Interprocessor_UpdateValue(_paramInfo, value);
}

#endif

#if IS_STORAGE_CORE

static void UpdateStorableValues(interprocessor_data_buffs_t* src, interprocessor_data_buffs_t* dest)
{
	dest->floats[SHARE_GRID_FREQ] = src->floats[SHARE_GRID_FREQ] < MAX_GRID_FREQ &&
			src->floats[SHARE_GRID_FREQ] > MIN_GRID_FREQ ?
					src->floats[SHARE_GRID_FREQ] : DEFAULT_GRID_FREQ;
	dest->floats[SHARE_GRID_VOLTAGE] = src->floats[SHARE_GRID_VOLTAGE] < MAX_GRID_VOLTAGE &&
			src->floats[SHARE_GRID_VOLTAGE] > MIN_GRID_VOLTAGE ?
					src->floats[SHARE_GRID_VOLTAGE] : DEFAULT_GRID_VOLTAGE;
	dest->floats[SHARE_REQ_RMS_CURRENT] = src->floats[SHARE_REQ_RMS_CURRENT] < MAX_CURRENT_INJ &&
			src->floats[SHARE_REQ_RMS_CURRENT] > MIN_CURRENT_INJ ?
					src->floats[SHARE_REQ_RMS_CURRENT] : DEFAULT_CURRENT_INJ;
	dest->floats[SHARE_LOUT] = src->floats[SHARE_LOUT] < MAX_LOUT &&
			src->floats[SHARE_LOUT] > MIN_LOUT ?
					src->floats[SHARE_LOUT] : DEFAULT_LOUT;
}

static void InitStatesFromStorage(uint32_t* data, bool isDataValid)
{
	// @note Make sure to only save/load controllable variables here
	interprocessor_data_buffs_t* src = (interprocessor_data_buffs_t*)data;
	interprocessor_data_buffs_t* dest = (interprocessor_data_buffs_t*)&INTER_CORE_DATA;

	// Put elements here which will always be default values here, such as enable etc which can't be 1 when the control starts
	dest->bitAccess[0] = 0;
	dest->bools[SHARE_BOOST_STATE] = false;
	dest->bools[SHARE_INVERTER_STATE] = false;
	dest->bools[SHARE_RELAY_STATUS] = false;
	dest->bools[SHARE_PLL_STATUS] = false;
	dest->floats[SHARE_CURR_RMS_CURRENT] = 0;
	if (isDataValid)
		UpdateStorableValues(src, dest);
	// Set default values because the loaded values are invalid
	// @note Make sure to only save/load controllable variables here
	else
	{
		dest->floats[SHARE_GRID_FREQ] = DEFAULT_GRID_FREQ;
		dest->floats[SHARE_GRID_VOLTAGE] = DEFAULT_GRID_VOLTAGE;
		dest->floats[SHARE_REQ_RMS_CURRENT] = DEFAULT_CURRENT_INJ;
		dest->floats[SHARE_LOUT] = DEFAULT_LOUT;
	}
}
static uint32_t RefreshStates(uint32_t* data, uint32_t* indexPtr)
{
	uint32_t len = 0;
	interprocessor_data_buffs_t* dest = (interprocessor_data_buffs_t*)data;
	interprocessor_data_buffs_t* src = (interprocessor_data_buffs_t*)&INTER_CORE_DATA;

	// @note Only update values and signal to update if values have been changed
	if (dest->floats[SHARE_GRID_FREQ] != src->floats[SHARE_GRID_FREQ] ||
			dest->floats[SHARE_GRID_VOLTAGE] != src->floats[SHARE_GRID_VOLTAGE] ||
			dest->floats[SHARE_REQ_RMS_CURRENT] != src->floats[SHARE_REQ_RMS_CURRENT] ||
			dest->floats[SHARE_LOUT] != src->floats[SHARE_LOUT])
	{
		UpdateStorableValues(src, dest);
		len = 1;
	}

	if (len != 0)
		len = storageWordLen;
	*indexPtr = 0;
	return len;
}

/**
 * @brief Configures the storage client information.
 * @param _config Pointer to the relevant configuration to be filled.
 */
void BSP_InterProcessorMsgs_ConfigStorage(state_storage_client_t* _config)
{
	_config->InitStatesFromStorage = InitStatesFromStorage;
	_config->RefreshStates = RefreshStates;
	_config->dataWordLen = storageWordLen = (sizeof(interprocessor_data_buffs_t) / 4) + 1;
}

#endif

#if IS_CONTROL_CORE

static device_err_t ProcessStateUpdateRequest(state_update_request* req, bool value)
{
	req->state = value;
	req->err = ERR_COUNT;
	req->isPending = true;
	while (req->isPending);
	return req->err;
}

static device_err_t UpdateBool(uint8_t index, bool value)
{
	if (index == SHARE_BOOST_STATE)
		return ProcessStateUpdateRequest(&boostStateUpdateRequest, value);
	else if (index == SHARE_INVERTER_STATE)
		return ProcessStateUpdateRequest(&inverterStateUpdateRequest, value);

	return ERR_ILLEGAL;
}

static device_err_t UpdateU8(uint8_t index, uint8_t value)
{
	INTER_CORE_DATA.u8s[index] = value;
	return ERR_OK;
}

static device_err_t UpdateU16(uint8_t index, uint16_t value)
{
	INTER_CORE_DATA.u16s[index] = value;
	return ERR_OK;
}

static device_err_t UpdateU32(uint8_t index, uint32_t value)
{
	INTER_CORE_DATA.u32s[index] = value;
	return ERR_OK;
}

static device_err_t UpdateS8(uint8_t index, int8_t value)
{
	INTER_CORE_DATA.s8s[index] = value;
	return ERR_OK;
}

static device_err_t UpdateS16(uint8_t index, int16_t value)
{
	INTER_CORE_DATA.s16s[index] = value;
	return ERR_OK;
}
static device_err_t UpdateS32(uint8_t index, int32_t value)
{
	INTER_CORE_DATA.s32s[index] = value;
	return ERR_OK;
}

static device_err_t UpdateFloat(uint8_t index, float value)
{
	if (index == SHARE_GRID_FREQ)
	{
		if(value < MIN_GRID_FREQ || value > MAX_GRID_FREQ)
			return ERR_OUT_OF_RANGE;
	}
	else if (index == SHARE_LOUT)
	{
		if(value < MIN_LOUT || value > MAX_LOUT)
			return ERR_OUT_OF_RANGE;
	}
	else if (index == SHARE_GRID_VOLTAGE)
	{
		if(value < MIN_GRID_VOLTAGE || value > MAX_GRID_VOLTAGE)
			return ERR_OUT_OF_RANGE;
	}
	else if (index == SHARE_REQ_RMS_CURRENT)
	{
		if(value < MIN_CURRENT_INJ || value > MAX_CURRENT_INJ)
			return ERR_OUT_OF_RANGE;
	}
	else if (index >= SHARE_CURR_RMS_CURRENT)
		return ERR_ILLEGAL;
	INTER_CORE_DATA.floats[index] = value;
	return ERR_OK;
}

static device_err_t SetBits(uint8_t index, uint8_t value)
{
	// if the bits are already set no need to reset them
	if ((INTER_CORE_DATA.bitAccess[index] & value) == value)
		return ERR_OK;

	if (index < SHARE_BIT_ACCESS_COUNT)
		INTER_CORE_DATA.bitAccess[index] |= value;
	return ERR_OK;
}

static device_err_t ClearBits(uint8_t index, uint8_t value)
{
	// if the bits are already clear no need to clear them
	if ((INTER_CORE_DATA.bitAccess[index] & value) == 0)
		return ERR_OK;

	if (index < SHARE_BIT_ACCESS_COUNT)
		INTER_CORE_DATA.bitAccess[index] &= ~value;
	return ERR_OK;
}

static device_err_t ToggleBits(uint8_t index, uint8_t value)
{
	if (index < SHARE_BIT_ACCESS_COUNT)
		INTER_CORE_DATA.bitAccess[index] ^= value;
	return ERR_OK;
}

void InterprocessorComms_ProcessPendingRequests(void)
{
	if (RingBuffer_IsEmpty((ring_buffer_t*)&CORE_MSGS.msgsRingBuff))
		return;

	device_err_t err = ERR_OK;
	interprocessor_msg_t* msg = (interprocessor_msg_t*)&CORE_MSGS.msgs[CORE_MSGS.msgsRingBuff.rdIndex];
	uint8_t index = msg->firstReg;
	if (msg->cmdLen == 1)
	{
		data_union_t* value = (data_union_t*)&CORE_MSGS.cmds[msg->cmdIndex];
		switch (msg->type)
		{
		case MSG_SET_BOOL: err = UpdateBool(index, value->b); break;
		case MSG_SET_U8: err = UpdateU8(index, value->u8); break;
		case MSG_SET_U16: err = UpdateU16(index, value->u16); break;
		case MSG_SET_U32: err = UpdateU32(index, value->u32); break;
		case MSG_SET_S8: err = UpdateS8(index, value->s8); break;
		case MSG_SET_S16: err = UpdateS16(index, value->s16); break;
		case MSG_SET_S32: err = UpdateS32(index, value->s32); break;
		case MSG_SET_FLOAT: err = UpdateFloat(index, value->f); break;
		case MSG_SET_BITS: err = SetBits(index, value->bits); break;
		case MSG_CLR_BITS: err = ClearBits(index, value->bits); break;
		case MSG_TOGGLE_BITS: err = ToggleBits(index, value->bits); break;
		default: err = ERR_ILLEGAL; break;
		}
	}

	msg->responseLen = 1;
	CORE_MSGS.response[CORE_MSGS.responseRingBuff.wrIndex].u8 = (device_err_t)err;
	msg->responseIndex = CORE_MSGS.responseRingBuff.wrIndex;

	RingBuffer_Write((ring_buffer_t*)&CORE_MSGS.responseRingBuff);
	RingBuffer_Read((ring_buffer_t*)&CORE_MSGS.msgsRingBuff);

}

void InterprocessorComms_InitData(void)
{
	CORE_MSGS.msgsRingBuff.modulo = INTERPROCESSOR_MSGS_SIZE - 1;
	CORE_MSGS.cmdsRingBuff.modulo = INTERPROCESSOR_CMD_BUFF_SIZE - 1;
	CORE_MSGS.responseRingBuff.modulo = INTERPROCESSOR_RESPONSE_BUFF_SIZE - 1;
	RingBuffer_Reset((ring_buffer_t*)&CORE_MSGS.msgsRingBuff);
	RingBuffer_Reset((ring_buffer_t*)&CORE_MSGS.cmdsRingBuff);
	RingBuffer_Reset((ring_buffer_t*)&CORE_MSGS.responseRingBuff);
}

#endif
/* EOF */
