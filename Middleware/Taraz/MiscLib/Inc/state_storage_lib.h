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
/** @addtogroup Misc_Library
 * @{
 */

/** @defgroup Storage_Config State Storage
 * @brief This module saves/restores the state information for each module/client.
 * @details The storage has to be initialized before use and the callbacks @ref state_storage_client_t.InitStatesFromStorage
 * and @ref state_storage_client_t.RefreshStates need to be defined for each client.
 * The following code is a sample method for using this module effectively.
@code
//Configure the client states' structure @ref state_storage_client_t for each client.
client.dataWordLen = xxxxx; // Set it to the number of 32-bit memory units required.
client.InitStatesFromStorage = xxxxx; // Set the callback to the local function which can initiate local states.
client.RefreshStates = xxxxx; // Set the callback to the local function which can refresh local states storage if states are updated .
//Configure the flash sectors in @ref flash_sector_config_t for both sectors.
//Configure the client count in @ref state_storage_config_t
storage.clientCount = xxxxx;
//Initialize storage
StateStorage_Init(&storageConfig);

//Poll the storage to update states
StateStorage_Refresh();
@endcode
 * @{
 */
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
/** @defgroup STATESTORAGE_Exported_Structures Structures
 * @{
 */
/**
 * @brief Contains the entities to be defined by each storage client. The storage module uses these members to update the storage states.
 */
typedef struct
{
	void* arg;									/**< @brief Custom arguments used by the relevant client.  */
	uint32_t dataWordLen;						/**< @brief No of 32-bit words required for the client storage. */
	void (*InitStatesFromStorage)(uint32_t* data, bool isDataValid);	/**< @brief This callback initiates the storage for the relevant client.
@code
//Sample Definition
//static void InitStatesFromStorage(uint32_t* data, bool isDataValid)
//{
//	// @note Make sure to only save/load controllable variables here
//	dest_data_t* src = (dest_data_t*)data;
//	src_data_t* dest = (src_data_t*)&srcData;
//	// Put elements here which will always be default values here, such as enable etc which can't be 1 when the control starts
//	if (isDataValid)
//		src->value = dest->value;
//	// Set default values because the loaded values are invalid
//	// @note Make sure to only save/load controllable variables here
//	else
//		src->value = 0;
//}
@endcode
		 	 	 	 	 	 	 	 	 	 	 @param data Buffer containing stored states at startup used for data initialization by the client.
		 	 	 	 	 	 	 	 	 	 	 @param isDataValid Contains the validity state of the data. If not valid, start with default values. */
	uint32_t (*RefreshStates)(uint32_t* data, uint32_t* indexPtr);		/**< @brief This callback is used to refresh the states in the data buffer for the client.
@code
//Sample Definition
//static uint32_t RefreshStates(uint32_t* data, uint32_t* indexPtr)
//{
//	uint32_t len = 0;
//	dest_data_t* dest = (dest_data_t*)data;
//	src_data_t* src = (src_data_t*)&srcData;
//	// @note Only update values and signal to update if values have been changed
//	if (dest->value != src->value)
//	{
//		dest->value = src->value;
//		len = storageWordLen;
//	}
//	*indexPtr = 0;
//	return len;
//}
@endcode
	 	 	 	 	 	 	 	 	 	 	 	 @param data Buffer to be updated by the client if states have been updated.
	 	 	 	 	 	 	 	 	 	 	 	 @param indexPtr Pointer for the initial index of the client, used in case of multiple blocks of state storages by a single client.
	 	 	 	 	 	 	 	 	 	 	 	 @return uint32_t Word count if the data is updated by the client, else 0 */
} state_storage_client_t;
/**
 * @brief Defines the flash sectors for configuring storage.
@code
//Sample Definition (Should be kept same for the same controller and memory configuration)
//storageConfig.sectors[0].sectorNo = FLASH_SECTOR_TOTAL - 2;
//storageConfig.sectors[1].sectorNo = FLASH_SECTOR_TOTAL - 1;
//for (int i = 0; i < 2; i++)
//{
//	storageConfig.sectors[i].bank = FLASH_BANK_1;
//	storageConfig.sectors[i].byteCount = FLASH_SECTOR_SIZE;
//	storageConfig.sectors[i].addr =  (uint32_t*)(FLASH_BANK1_BASE + (storageConfig.sectors[i].sectorNo * FLASH_SECTOR_SIZE));
//}
@endcode
 */
typedef struct
{
	uint32_t sectorNo;		/**< Sector number */
	uint32_t index;			/**< Sector index in the flash bank */
	uint32_t byteCount;		/**< No of bytes in a sector */
	uint32_t bank;			/**< Flash Bank */
	uint32_t* addr;			/**< Sector start address */
} flash_sector_config_t;
/**
 * @brief Defines the state storage configuration.
 */
typedef struct
{
	flash_sector_config_t sectors[2];	/**< Both sectors' information */
	int clientCount;					/**< No of clients for the module */
	state_storage_client_t* clients;	/**< Client configuration */
	uint32_t store[STORE_WORD_SIZE];	/**< Data storage for clients' states */
} state_storage_config_t;
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup STATESTORAGE_Exported_Functions Functions
 * @{
 */
/**
 * @brief This function initializes the state storage according to the application requirements.
 * @details This function utilizes two distinct sectors to retrieve the states of all system components.
 * By using a ping-pong approach, only one sector is active at a time for updating the states.
 * The first packet within each sector either remains empty or contains the states of all system components.
 * If the first packet is not present, the sector is considered empty and will be promptly erased if it hasn't
 * been already. Following the initial packet, subsequent partial data packets are used to update the system states.
 * @code
static state_storage_config_t storageConfig = {0};
// Initialize both sectors
storageConfig.sectors[0].sectorNo = FLASH_SECTOR_TOTAL - 2;
storageConfig.sectors[1].sectorNo = FLASH_SECTOR_TOTAL - 1;
for (int i = 0; i < 2; i++)
{
	storageConfig.sectors[i].bank = FLASH_BANK_1;
	storageConfig.sectors[i].byteCount = FLASH_SECTOR_SIZE;
	storageConfig.sectors[i].addr =  (uint32_t*)(FLASH_BANK1_BASE + (storageConfig.sectors[i].sectorNo * FLASH_SECTOR_SIZE));
}
// Initialize the clients
static state_storage_client_t storageClients[2];
storageConfig.clientCount = 3;
storageConfig.clients = storageClients;
storageClients[0].arg = (void*)&CLIENT_DATA;
//Following are sample definitions for the functions to be defined in the client.
//static void InitStatesFromStorage(uint32_t* data, bool isDataValid)
//{
//	// @note Make sure to only save/load controllable variables here
//	dest_data_t* src = (dest_data_t*)data;
//	src_data_t* dest = (src_data_t*)&srcData;
//	// Put elements here which will always be default values here, such as enable etc which can't be 1 when the control starts
//	if (isDataValid)
//		src->value = dest->value;
//	// Set default values because the loaded values are invalid
//	// @note Make sure to only save/load controllable variables here
//	else
//		src->value = 0;
//}
//static uint32_t RefreshStates(uint32_t* data, uint32_t* indexPtr)
//{
//	uint32_t len = 0;
//	dest_data_t* dest = (dest_data_t*)data;
//	src_data_t* src = (src_data_t*)&srcData;
//	// @note Only update values and signal to update if values have been changed
//	if (dest->value != src->value)
//	{
//		dest->value = src->value;
//		len = storageWordLen;
//	}
//	*indexPtr = 0;
//	return len;
//}
//Client1_ConfigStorage(state_storage_client_t* _config)
//{
//	_config->InitStatesFromStorage = InitStatesFromStorage;
//	_config->RefreshStates = RefreshStates;
//	_config->dataWordLen = STORAGE_WORD_LEN;
//}
Client1_ConfigStorage(&storageClients[0]);
Client2_ConfigStorage(&storageClients[1]);
StateStorage_Init(&storageConfig);
@endcode
 * @param _config System configuration for the storage.
 */
extern void StateStorage_Init(state_storage_config_t* _config);
/**
 * @brief Refreshes the storage state if required.
 * @details Poll this function periodically to refresh the stored states for all parameters. If the index in the
 * sector is 0 or the packet cannot fit within the remaining space in the sector, the \"isFirstSectorPacket\" flag
 * is set. This flag ensures that all parameters are completely flushed to the beginning of the sector.
 * To obtain the necessary data, each client is prompted to refresh their states using the @ref state_storage_client_t.RefreshStates() function.
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
/**
 * @}
 */
#endif 
/**
 * @}
 */
/**
 * @}
 */
/* EOF */
