/**
 ********************************************************************************
 * @file 		state_storage_lib.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 26, 2023
 *
 * @brief    
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

#ifndef STATE_STORAGE_LIB_H_
#define STATE_STORAGE_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define STORE_WORD_SIZE				(128)

// Computations
#define STORE_BYTE_SIZE				(STORE_WORD_SIZE * 4)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	void* arg;
	uint32_t dataWordLen;
	void (*InitStatesFromStorage)(uint32_t* data, bool isDataValid);
	uint32_t (*RefreshStates)(uint32_t* data, uint32_t* indexPtr);
} state_storage_client_t;
typedef struct
{
	uint32_t sectorNo;
	uint32_t index;
	uint32_t byteCount;
	uint32_t bank;
	uint32_t* addr;
} flash_sector_config_t;
typedef struct
{
	flash_sector_config_t sectors[2];
	int clientCount;
	state_storage_client_t* clients;
	uint32_t store[STORE_WORD_SIZE];
} state_storage_config_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief This function initializes the state storage according to the application requirements.
 * @details This function utilizes two distinct sectors to retrieve the states of all system components.
 * By using a ping-pong approach, only one sector is active at a time for updating the states.
 * The first packet within each sector either remains empty or contains the states of all system components.
 * If the first packet is not present, the sector is considered empty and will be promptly erased if it hasn't
 * been already. Following the initial packet, subsequent partial data packets are used to update the system states.
 * @param _config System configuration for the storage.
 */
extern void StateStorage_Init(state_storage_config_t* _config);
/**
 * @brief Refreshes the storage state if required.
 * @details Poll this function periodically to refresh the stored states for all parameters. If the index in the
 * sector is 0 or the packet cannot fit within the remaining space in the sector, the \"isFirstSectorPacket\" flag
 * is set. This flag ensures that all parameters are completely flushed to the beginning of the sector.
 * To obtain the necessary data, each client is prompted to refresh their states using the @ref RefreshStates() function.
 * It is the responsibility of the client to provide the refreshed data, if necessary, and return the size of
 * the updated data. Finally, the partial data will be flushed to the storage flash.
 */
extern void StateStorage_Refresh(void);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
