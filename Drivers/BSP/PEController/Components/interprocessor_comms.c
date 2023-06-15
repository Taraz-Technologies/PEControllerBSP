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

/********************************************************************************
 * Global Variables
 *******************************************************************************/
#if IS_CONTROL_CORE
mode_change_request_t modeChangeRequest = { .isPending = false };
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

device_err_t Interprocessor_UpdateFromString(data_param_info_t* _paramInfo, const char* value)
{
	if (_paramInfo == NULL)
		return ERR_ILLEGAL;
	device_err_t err = ERR_ILLEGAL;
	data_union_t val;
	switch (_paramInfo->type)
	{
	case DTYPE_BOOL:
		break;
	case DTYPE_U8:
		break;
	case DTYPE_U16:
		break;
	case DTYPE_U32:
		break;
	case DTYPE_S8:
		break;
	case DTYPE_S16:
		break;
	case DTYPE_S32:
		break;
	case DTYPE_FLOAT:
		if (_paramInfo->index >= SHARE_FLOAT_COUNT)
			return ERR_ILLEGAL;
		if (atof_custom(value, &val.f) == false)
			return ERR_INVALID_TEXT;
		err = SingleUpdateRequest_Blocking(MSG_SET_FLOAT, _paramInfo->index, val);
		break;
	case DTYPE_U8_BIT:
		if (_paramInfo->index >= SHARE_U8_COUNT)
			return ERR_ILLEGAL;
		if (_paramInfo->index == SHARE_POWER_MODE)
		{
			if (atob_custom(value, &val.b) == false)
				return ERR_INVALID_TEXT;
			interprocessor_msg_type_t type = val.u8 ? MSG_SET_U8_BITS : MSG_CLR_U8_BITS;
			val.u8 = 1 << _paramInfo->arg;
			err = SingleUpdateRequest_Blocking(type, _paramInfo->index, val);
		}
		break;
	case DTYPE_U16_BIT:
		break;
	case DTYPE_U32_BIT:
		break;
	default:
		break;
	}
	return err;
}

device_err_t Interprocessor_GetStringValue(data_param_info_t* _paramInfo, char* value, bool addUnit)
{
	if (_paramInfo == NULL)
		return ERR_ILLEGAL;
	value[0] = 0;
	device_err_t err = ERR_ILLEGAL;
	int len;
	switch (_paramInfo->type)
	{
	case DTYPE_BOOL:
		if (_paramInfo->index >= SHARE_BOOL_COUNT)
			return ERR_ILLEGAL;
		(void) btoa_custom(CORE_MSGS.bools[_paramInfo->index], value);
		err = ERR_OK;
		break;
	case DTYPE_U8:
		break;
	case DTYPE_U16:
		break;
	case DTYPE_U32:
		break;
	case DTYPE_S8:
		break;
	case DTYPE_S16:
		break;
	case DTYPE_S32:
		break;
	case DTYPE_FLOAT:
		if (_paramInfo->index >= SHARE_FLOAT_COUNT)
			return ERR_ILLEGAL;
		len = ftoa_custom(CORE_MSGS.floats[_paramInfo->index], value, 7, _paramInfo->arg);
		if (addUnit && _paramInfo->unit < UNIT_COUNT)
			strcat_custom(value, unitTxts[_paramInfo->unit], len, false);
		err = ERR_OK;
		break;
	case DTYPE_U8_BIT:
		if (_paramInfo->index >= SHARE_U8_COUNT)
			return ERR_ILLEGAL;
		if (_paramInfo->index == SHARE_POWER_MODE)
			(void) btoa_custom(((CORE_MSGS.u8s[_paramInfo->index] >> _paramInfo->arg) & 0x1), value);
		err = ERR_OK;
		break;
	case DTYPE_U16_BIT:
		break;
	case DTYPE_U32_BIT:
		break;
	default:
		break;
	}
	return err;
}

device_err_t Interprocessor_GetValue(data_param_info_t* _paramInfo, data_union_t* value)
{
	if (_paramInfo == NULL)
		return ERR_ILLEGAL;
	device_err_t err = ERR_ILLEGAL;
	switch (_paramInfo->type)
	{
	case DTYPE_BOOL:
		if (_paramInfo->index >= SHARE_BOOL_COUNT)
			return ERR_ILLEGAL;
		value->b = CORE_MSGS.bools[_paramInfo->index];
		err = ERR_OK;
		break;
	case DTYPE_U8:
		break;
	case DTYPE_U16:
		break;
	case DTYPE_U32:
		break;
	case DTYPE_S8:
		break;
	case DTYPE_S16:
		break;
	case DTYPE_S32:
		break;
	case DTYPE_FLOAT:
		if (_paramInfo->index >= SHARE_FLOAT_COUNT)
			return ERR_ILLEGAL;
		value->f = CORE_MSGS.floats[_paramInfo->index];
		err = ERR_OK;
		break;
	case DTYPE_U8_BIT:
		if (_paramInfo->index >= SHARE_U8_COUNT)
			return ERR_ILLEGAL;
		if (_paramInfo->index == SHARE_POWER_MODE)
			value->u8 = ((CORE_MSGS.u8s[_paramInfo->index] >> _paramInfo->arg) & 0x1);
		err = ERR_OK;
		break;
	case DTYPE_U16_BIT:
		break;
	case DTYPE_U32_BIT:
		break;
	default:
		break;
	}
	return err;
}

device_err_t Default_SetDataParameter(data_param_info_t* _paramInfo, const char* value)
{
	return Interprocessor_UpdateFromString(_paramInfo, value);
}
device_err_t Default_GetDataParameter(data_param_info_t* _paramInfo, char* value, bool addUnit)
{
	return Interprocessor_GetStringValue(_paramInfo, value, addUnit);
}

#endif
#if IS_CONTROL_CORE

static device_err_t UpdateFloat(uint8_t index, float value)
{
	CORE_MSGS.floats[index] = value;
	return ERR_OK;
}

static device_err_t UpdateU8(uint8_t index, uint8_t value)
{
	// if the mode is already set return OK
	if (CORE_MSGS.u8s[index] == value)
		return ERR_OK;

	if (index == SHARE_POWER_MODE)
	{
		modeChangeRequest.mode = value;
		modeChangeRequest.err = ERR_COUNT;
		modeChangeRequest.isPending = true;

		while (modeChangeRequest.isPending);

		return modeChangeRequest.err;
	}
	if (index < SHARE_U8_COUNT)
		CORE_MSGS.u8s[index] = value;
	return ERR_OK;
}


static device_err_t SetU8Bits(uint8_t index, uint8_t value)
{
	// if the bits are already set no need to reset them
	if ((CORE_MSGS.u8s[index] & value) == value)
		return ERR_OK;

	if (index == SHARE_POWER_MODE)
	{
		modeChangeRequest.mode = CORE_MSGS.u8s[index] | value;
		modeChangeRequest.err = ERR_COUNT;
		modeChangeRequest.isPending = true;

		while (modeChangeRequest.isPending);

		return modeChangeRequest.err;
	}
	if (index < SHARE_U8_COUNT)
		CORE_MSGS.u8s[index] |= value;
	return ERR_OK;
}

static device_err_t ClearU8Bits(uint8_t index, uint8_t value)
{
	// if the bits are already clear no need to clear them
	if ((CORE_MSGS.u8s[index] & value) == 0)
		return ERR_OK;

	if (index == SHARE_POWER_MODE)
	{
		modeChangeRequest.mode = CORE_MSGS.u8s[index] & ~value;
		modeChangeRequest.err = ERR_COUNT;
		modeChangeRequest.isPending = true;

		while (modeChangeRequest.isPending);

		return modeChangeRequest.err;
	}
	if (index < SHARE_U8_COUNT)
		CORE_MSGS.u8s[index] &= ~value;
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
		case MSG_SET_FLOAT:
			err = UpdateFloat(index, value->f);
			break;
		case MSG_SET_U8:
			err = UpdateU8(index, value->u8);
			break;
		case MSG_SET_U8_BITS:
			err = SetU8Bits(index, value->u8);
			break;
		case MSG_CLR_U8_BITS:
			err = ClearU8Bits(index, value->u8);
			break;
		default:
			break;
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
