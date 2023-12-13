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
state_update_request boostStateUpdateRequest = { .isPending = false };
state_update_request inverterStateUpdateRequest = { .isPending = false };
#endif
#if IS_COMMS_CORE
/**
 * @brief Describes all the parameters shared in both cores which need a textual description.
 * @note The sequence here should match the sequence in @ref p2p_params_type_t for
 * correct display of parameters.
 */
data_param_info_t p2pCommsParams[P2P_PARAM_COUNT] =
{
		{ .name = "Grid Frequency", .index = P2P_GRID_FREQ, .type = DTYPE_FLOAT, .arg = 1, .unit = UNIT_Hz },
		{ .name = "Grid Voltage (P-N)", .index = P2P_GRID_VOLTAGE, .type = DTYPE_FLOAT, .arg = 1, .unit = UNIT_V},
		{ .name = "iReq (RMS)", .index = P2P_REQ_RMS_CURRENT, .type = DTYPE_FLOAT, .arg = 2, .unit = UNIT_A },
		{ .name = "Filter Inductance (mH)", .index = P2P_LOUT_mH, .type = DTYPE_FLOAT,  .arg = 2, .unit = UNIT_NONE},
		{ .name = "iGen (RMS)", .index = P2P_CURR_RMS_CURRENT, .type = DTYPE_FLOAT,  .arg = 2, .unit = UNIT_A },
		{ .name = "Enable Boost", .index = P2P_BOOST_STATE, .type = DTYPE_BOOL },
		{ .name = "Enable Inverter", .index = P2P_INVERTER_STATE, .type = DTYPE_BOOL},
		{ .name = "Precharge Relay Status", .index = P2P_RELAY_STATUS, .type = DTYPE_BOOL },
		{ .name = "PLL Status", .index = P2P_PLL_STATUS, .type = DTYPE_BOOL},
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
	return dest->floats[P2P_GRID_FREQ] != src->floats[P2P_GRID_FREQ] ||
			dest->floats[P2P_GRID_VOLTAGE] != src->floats[P2P_GRID_VOLTAGE] ||
			dest->floats[P2P_REQ_RMS_CURRENT] != src->floats[P2P_REQ_RMS_CURRENT] ||
			dest->floats[P2P_LOUT_mH] != src->floats[P2P_LOUT_mH];
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
	dest->floats[P2P_GRID_FREQ] = src->floats[P2P_GRID_FREQ] < MAX_GRID_FREQ && src->floats[P2P_GRID_FREQ] > MIN_GRID_FREQ ? src->floats[P2P_GRID_FREQ] : DEFAULT_GRID_FREQ;
	dest->floats[P2P_GRID_VOLTAGE] = src->floats[P2P_GRID_VOLTAGE] < MAX_GRID_VOLTAGE && src->floats[P2P_GRID_VOLTAGE] > MIN_GRID_VOLTAGE ? src->floats[P2P_GRID_VOLTAGE] : DEFAULT_GRID_VOLTAGE;
	dest->floats[P2P_REQ_RMS_CURRENT] = src->floats[P2P_REQ_RMS_CURRENT] < MAX_CURRENT_INJ && src->floats[P2P_REQ_RMS_CURRENT] > MIN_CURRENT_INJ ? src->floats[P2P_REQ_RMS_CURRENT] : DEFAULT_CURRENT_INJ;
	dest->floats[P2P_LOUT_mH] = src->floats[P2P_LOUT_mH] < MAX_LOUT_mH && src->floats[P2P_LOUT_mH] > MIN_LOUT_mH ? src->floats[P2P_LOUT_mH] : DEFAULT_LOUT_mH;
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
	dest->bools[P2P_BOOST_STATE] = false;
	dest->bools[P2P_INVERTER_STATE] = false;
	dest->bools[P2P_RELAY_STATUS] = false;
	dest->bools[P2P_PLL_STATUS] = false;
	dest->floats[P2P_CURR_RMS_CURRENT] = 0;
	if (isDataValid)
		P2PComms_UpdateStorableStates(dest, src);
	// Set default values because the loaded values are invalid
	// @note Make sure to only save/load controllable variables here
	else
	{
		dest->floats[P2P_GRID_FREQ] = DEFAULT_GRID_FREQ;
		dest->floats[P2P_GRID_VOLTAGE] = DEFAULT_GRID_VOLTAGE;
		dest->floats[P2P_REQ_RMS_CURRENT] = DEFAULT_CURRENT_INJ;
		dest->floats[P2P_LOUT_mH] = DEFAULT_LOUT_mH;
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
	if (index == P2P_BOOST_STATE)
		return ProcessStateUpdateRequest(&boostStateUpdateRequest, value);
	else if (index == P2P_INVERTER_STATE)
		return ProcessStateUpdateRequest(&inverterStateUpdateRequest, value);

	return ERR_ILLEGAL;
}

/**
 * @brief Update a single-precision floating parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
device_err_t P2PComms_UpdateFloat(uint8_t index, float value)
{
	if (index == P2P_GRID_FREQ)
	{
		if(value < MIN_GRID_FREQ || value > MAX_GRID_FREQ)
			return ERR_OUT_OF_RANGE;
	}
	else if (index == P2P_LOUT_mH)
	{
		if(value < MIN_LOUT_mH || value > MAX_LOUT_mH)
			return ERR_OUT_OF_RANGE;
	}
	else if (index == P2P_GRID_VOLTAGE)
	{
		if(value < MIN_GRID_VOLTAGE || value > MAX_GRID_VOLTAGE)
			return ERR_OUT_OF_RANGE;
	}
	else if (index == P2P_REQ_RMS_CURRENT)
	{
		if(value < MIN_CURRENT_INJ || value > MAX_CURRENT_INJ)
			return ERR_OUT_OF_RANGE;
	}
	else if (index >= P2P_CURR_RMS_CURRENT)
		return ERR_ILLEGAL;
	INTER_CORE_DATA.floats[index] = value;
	return ERR_OK;
}
#endif
/* EOF */
