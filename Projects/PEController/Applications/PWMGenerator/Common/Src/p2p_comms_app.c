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
#if IS_COMMS_CORE
/**
 * @brief Describes all the parameters shared in both cores which need a textual description.
 * @note The sequence here should match the sequence in @ref p2p_params_type_t for
 * correct display of parameters.
 */
data_param_info_t p2pCommsParams[P2P_PARAM_COUNT] =
{
		{ .name = "PWM Phase Shift (0 - 180 degrees)", .index = P2P_PARAM_PHASE_SHIFT, .type = DTYPE_FLOAT, .arg = 0, .unit = UNIT_NONE },
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
	return dest->floats[P2P_PWM_PHASE_SHIFT] != src->floats[P2P_PWM_PHASE_SHIFT];
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
	dest->floats[P2P_PWM_PHASE_SHIFT] = src->floats[P2P_PWM_PHASE_SHIFT] <= 180 && src->floats[P2P_PWM_PHASE_SHIFT] >= 0 ? src->floats[P2P_PWM_PHASE_SHIFT] : 90;
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
	dest->bools[0] = false;
	if (isDataValid)
		P2PComms_UpdateStorableStates(dest, src);
	// Set default values because the loaded values are invalid
	// @note Make sure to only save/load controllable variables here
	else
	{
		dest->floats[P2P_PWM_PHASE_SHIFT] = 90;
	}
}

#endif

#if IS_CONTROL_CORE


/**
 * @brief Update a single-precision floating parameter share in both cores.
 * @note A weak implementation of this function is provided. User can create a custom implementation if needed.
 * @param index Register index.
 * @param value Desired value.
 * @return device_err_t If successful <c>ERR_OK</c> else some other error.
 */
device_err_t P2PComms_UpdateFloat(uint8_t index, float value)
{
	if (index == P2P_PWM_PHASE_SHIFT)
	{
		if (value < 0 || value > 180)
			return ERR_OUT_OF_RANGE;
	}
	INTER_CORE_DATA.floats[index] = value;
	return ERR_OK;
}
#endif
/* EOF */
