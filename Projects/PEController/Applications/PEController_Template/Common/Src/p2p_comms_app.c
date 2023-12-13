/**
 ********************************************************************************
 * @file    	p2p_comms_app.c
 * @author 		Waqas Ehsan Butt
 * @date    	April 23, 2021
 *
 * @brief   Controls the application specific connection between the communication and controls processors.
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
#include "p2p_comms_app.h"
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

/********************************************************************************
 * Global Variables
 *******************************************************************************/
#if IS_CONTROL_CORE
state_update_request modeChangeRequest = { .isPending = false };
#endif
#if IS_COMMS_CORE
/**
 * @brief Describes all the parameters shared in both cores which need a textual description.
 * @note The sequence here should match the sequence in @ref p2p_params_type_t for
 * correct display of parameters.
 */
data_param_info_t p2pCommsParams[P2P_PARAM_COUNT] =
{
		{ .name = "8-bit Unsigned", .index = P2P_SAMPLE_U8, .type = DTYPE_U8, .arg = 0, .unit = UNIT_Hz },
		{ .name = "16-bit Unsigned", .index = P2P_SAMPLE_U16, .type = DTYPE_U16, .arg = 0, .unit = UNIT_Hz},
		{ .name = "32-bit Unsigned", .index = P2P_SAMPLE_U32, .type = DTYPE_U32, .arg = 0, .unit = UNIT_V },
		{ .name = "8-bit Signed", .index = P2P_SAMPLE_S8, .type = DTYPE_S8, .arg = 0, .unit = UNIT_NONE },
		{ .name = "16-bit Signed", .index = P2P_SAMPLE_S16, .type = DTYPE_S16,  .arg = 0, .unit = UNIT_NONE },
		{ .name = "32-bit Signed", .index = P2P_SAMPLE_S32, .type = DTYPE_S32,  .arg = 0, .unit = UNIT_NONE },
		{ .name = "Status Bit 1", .index = P2P_SAMPLE_BIT_ACCESS, .type = DTYPE_BIT_ACCESS,  .arg = (1U << 0), },
		{ .name = "Status Bit 2", .index = P2P_SAMPLE_BIT_ACCESS, .type = DTYPE_BIT_ACCESS,  .arg = (1U << 1), },
		{ .name = "Control State", .index = P2P_CONTROL_STATE, .type = DTYPE_BOOL,  .arg = 0, },
		{ .name = "Floating Number", .index = P2P_SAMPLE_FLOAT, .type = DTYPE_FLOAT, .arg = 1, .unit = UNIT_Hz},

};
#endif
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
#if IS_STORAGE_CORE
/**
 * @brief Check if the flash needs to be updated.
 * @param dest Destination
 * @param src Source
 * @return <c>true</c> if update required else <c>false</c>.
 */
bool P2PComms_IsStateStorageUpdateNeeded(p2p_data_buffs_t* dest, p2p_data_buffs_t* src)
{
	return dest->floats[P2P_SAMPLE_U8] != src->floats[P2P_SAMPLE_U8] ||
			dest->floats[P2P_SAMPLE_S8] != src->floats[P2P_SAMPLE_S8] ||
			dest->floats[P2P_SAMPLE_U16] != src->floats[P2P_SAMPLE_U16] ||
			dest->floats[P2P_SAMPLE_S16] != src->floats[P2P_SAMPLE_S16] ||
			dest->bools[P2P_SAMPLE_U32] != src->bools[P2P_SAMPLE_U32] ||
			dest->floats[P2P_SAMPLE_S32] != src->floats[P2P_SAMPLE_S32] ||
			dest->floats[P2P_SAMPLE_FLOAT] != src->floats[P2P_SAMPLE_FLOAT];
}
#pragma GCC push_options
#pragma GCC optimize ("-O0")
/**
 * @brief Update all store-able states from source to destination.
 * @param dest Destination
 * @param src Source
 */
void P2PComms_UpdateStorableStates(p2p_data_buffs_t* dest, p2p_data_buffs_t* src)
{
	dest->floats[P2P_SAMPLE_U8] = src->floats[P2P_SAMPLE_U8];
	dest->floats[P2P_SAMPLE_S8] = src->floats[P2P_SAMPLE_S8];
	dest->floats[P2P_SAMPLE_U16] = src->floats[P2P_SAMPLE_U16];
	dest->floats[P2P_SAMPLE_S16] = src->floats[P2P_SAMPLE_S16];
	dest->bools[P2P_SAMPLE_U32] = src->bools[P2P_SAMPLE_U32];
	dest->floats[P2P_SAMPLE_S32] = src->floats[P2P_SAMPLE_S32];
	dest->floats[P2P_SAMPLE_FLOAT] = src->floats[P2P_SAMPLE_FLOAT];
}
#pragma GCC pop_options

/**
 * @brief Initialize the states from the flash storage.
 * @param data Data pointer.
 * @param isDataValid <c>true</c> if the flash data is valid.
 */
void P2PComms_InitStatesFromStorage(uint32_t* data, bool isDataValid)
{
	// @note Make sure to only save/load controllable variables here
	p2p_data_buffs_t* src = (p2p_data_buffs_t*)data;
	p2p_data_buffs_t* dest = (p2p_data_buffs_t*)&INTER_CORE_DATA;

	// Put elements here which will always be default values here, such as enable etc which can't be 1 when the control starts
	dest->bitAccess[0] = 0;
	dest->bools[P2P_CONTROL_STATE] = false;
	if (isDataValid)
		P2PComms_UpdateStorableStates(dest, src);
	// Set default values because the loaded values are invalid
	// @note Make sure to only save/load controllable variables here
	else
	{
		dest->u8s[P2P_SAMPLE_U8] = 0;
		dest->s8s[P2P_SAMPLE_S8] = 0;
		dest->u16s[P2P_SAMPLE_U16] = 0;
		dest->s16s[P2P_SAMPLE_S16] = 0;
		dest->u32s[P2P_SAMPLE_U32] = 0;
		dest->s32s[P2P_SAMPLE_S32] = 0;
		dest->floats[P2P_SAMPLE_FLOAT] = 0;
	}
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
/**
 * @brief Update a boolean parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
device_err_t P2PComms_UpdateBool(uint8_t index, bool value)
{
	if (index == P2P_CONTROL_STATE)
		return ProcessStateUpdateRequest(&modeChangeRequest, value);
	return ERR_ILLEGAL;
}
#endif
/* EOF */
