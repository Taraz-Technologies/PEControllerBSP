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
#include "open_loop_vf_config.h"
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
state_update_request inv1StateUpdateRequest = { .isPending = false };
state_update_request inv2StateUpdateRequest = { .isPending = false };
#endif
#if IS_COMMS_CORE

#endif
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
#if IS_STORAGE_CORE

bool P2PComms_IsStateStorageUpdateNeeded(p2p_data_buffs_t* dest, p2p_data_buffs_t* src)
{
	return dest->floats[SHARE_INV1_REQ_FREQ] != src->floats[SHARE_INV1_REQ_FREQ] ||
			dest->floats[SHARE_INV1_NOM_FREQ] != src->floats[SHARE_INV1_NOM_FREQ] ||
			dest->floats[SHARE_INV1_NOM_m] != src->floats[SHARE_INV1_NOM_m] ||
			dest->floats[SHARE_INV2_REQ_FREQ] != src->floats[SHARE_INV2_REQ_FREQ] ||
			dest->floats[SHARE_INV2_NOM_FREQ] != src->floats[SHARE_INV2_NOM_FREQ] ||
			dest->floats[SHARE_INV2_NOM_m] != src->floats[SHARE_INV2_NOM_m];
}

void P2PComms_UpdateStorableStates(p2p_data_buffs_t* dest, p2p_data_buffs_t* src)
{
	dest->floats[SHARE_INV1_REQ_FREQ] = src->floats[SHARE_INV1_REQ_FREQ] < MAX_OUTPUT_FREQ && src->floats[SHARE_INV1_REQ_FREQ] > MIN_OUTPUT_FREQ ? src->floats[SHARE_INV1_REQ_FREQ] : DEFAULT_OUTPUT_FREQ;
	dest->floats[SHARE_INV1_NOM_FREQ] = src->floats[SHARE_INV1_NOM_FREQ] < MAX_NOMINAL_FREQ && src->floats[SHARE_INV1_NOM_FREQ] > MIN_NOMINAL_FREQ ? src->floats[SHARE_INV1_NOM_FREQ] : DEFAULT_NOMINAL_FREQ;
	dest->floats[SHARE_INV1_NOM_m] = src->floats[SHARE_INV1_NOM_m] < MAX_NOMINAL_m && src->floats[SHARE_INV1_NOM_m] > MIN_NOMINAL_m ? src->floats[SHARE_INV1_NOM_m] : DEFAULT_NOMINAL_m;

	dest->floats[SHARE_INV2_REQ_FREQ] = src->floats[SHARE_INV2_REQ_FREQ] < MAX_OUTPUT_FREQ && src->floats[SHARE_INV2_REQ_FREQ] > MIN_OUTPUT_FREQ ? src->floats[SHARE_INV2_REQ_FREQ] : DEFAULT_OUTPUT_FREQ;
	dest->floats[SHARE_INV2_NOM_FREQ] = src->floats[SHARE_INV2_NOM_FREQ] < MAX_NOMINAL_FREQ && src->floats[SHARE_INV2_NOM_FREQ] > MIN_NOMINAL_FREQ ? src->floats[SHARE_INV2_NOM_FREQ] : DEFAULT_NOMINAL_FREQ;
	dest->floats[SHARE_INV2_NOM_m] = src->floats[SHARE_INV2_NOM_m] < MAX_NOMINAL_m && src->floats[SHARE_INV2_NOM_m] > MIN_NOMINAL_m ? src->floats[SHARE_INV2_NOM_m] : DEFAULT_NOMINAL_m;
}

void P2PComms_InitStatesFromStorage(uint32_t* data, bool isDataValid)
{
	// @note Make sure to only save/load controllable variables here
	p2p_data_buffs_t* src = (p2p_data_buffs_t*)data;
	p2p_data_buffs_t* dest = (p2p_data_buffs_t*)&INTER_CORE_DATA;

	// Put elements here which will always be default values here, such as enable etc which can't be 1 when the control starts
	dest->bitAccess[0] = 0;
	dest->bools[SHARE_INV1_STATE] = false;
	dest->bools[SHARE_INV2_STATE] = false;
	if (isDataValid)
		P2PComms_UpdateStorableStates(dest, src);
	// Set default values because the loaded values are invalid
	// @note Make sure to only save/load controllable variables here
	else
	{
		dest->floats[SHARE_INV1_REQ_FREQ] = DEFAULT_OUTPUT_FREQ;
		dest->floats[SHARE_INV1_NOM_FREQ] = DEFAULT_NOMINAL_FREQ;
		dest->floats[SHARE_INV1_NOM_m] = DEFAULT_NOMINAL_m;
		dest->floats[SHARE_INV2_REQ_FREQ] = DEFAULT_OUTPUT_FREQ;
		dest->floats[SHARE_INV2_NOM_FREQ] = DEFAULT_NOMINAL_FREQ;
		dest->floats[SHARE_INV2_NOM_m] = DEFAULT_NOMINAL_m;
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

device_err_t P2PComms_UpdateBool(uint8_t index, bool value)
{
	if (index == SHARE_INV1_STATE)
		return ProcessStateUpdateRequest(&inv1StateUpdateRequest, value);
	else if (index == SHARE_INV2_STATE)
		return ProcessStateUpdateRequest(&inv2StateUpdateRequest, value);
	return ERR_ILLEGAL;
}

device_err_t P2PComms_UpdateFloat(uint8_t index, float value)
{
	if (index == SHARE_INV1_REQ_FREQ || index == SHARE_INV2_REQ_FREQ )
	{
		if(value < MIN_OUTPUT_FREQ || value > MAX_OUTPUT_FREQ)
			return ERR_OUT_OF_RANGE;
	}
	else if (index == SHARE_INV1_NOM_FREQ || index == SHARE_INV1_NOM_FREQ)
	{
		if(value < MIN_NOMINAL_FREQ || value > MAX_NOMINAL_FREQ)
			return ERR_OUT_OF_RANGE;
	}
	else if (index == SHARE_INV1_NOM_m || index == SHARE_INV1_NOM_FREQ)
	{
		if(value < MIN_NOMINAL_m || value > MAX_NOMINAL_m)
			return ERR_OUT_OF_RANGE;
	}
	else
		return ERR_ILLEGAL;
	INTER_CORE_DATA.floats[index] = value;
	return ERR_OK;
}
#endif
/* EOF */
