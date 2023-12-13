/**
 ********************************************************************************
 * @file    	state_storage_lib.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 26, 2023
 *
 * @brief   
 ********************************************************************************
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
 * @details
 * This file uses two memory sectors to record the required states of the program.
 * First call @ref StateStorage_Init() to initialize the library according to the
 * application requirements. Periodically call StateStorage_Refresh() to update the
 * application state storage whenever needed.
 ********************************************************************************
 */

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "state_storage_lib.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define EMPTY_WORD						(0xFFFFFFFF)
#define HEADER_WORD_SIZE				(2)
#define FOOTER_WORD_SIZE				(1)
#define FLASH_BYTE_ALIGNMENT			(32)
#define STORAGE_HEADER_VALUE			(0xA5A5A5A5)
#define STORAGE_FOOTER_VALUE			(0x5A5A5A5A)

// Computations
#define EXTRA_WORDS						(HEADER_WORD_SIZE + FOOTER_WORD_SIZE)
#define FLASH_WORD_ALIGNMENT			(FLASH_BYTE_ALIGNMENT / 4)
#define GET_FLASH_ALIGNED_SIZE(size)	((size + EXTRA_WORDS - 1) + (FLASH_WORD_ALIGNMENT - ((size + EXTRA_WORDS - 1) % FLASH_WORD_ALIGNMENT)))
#define GET_LOCAL_LEN(size)				(size ? size + 2 : 0)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static state_storage_config_t* config;
static int sectorIndex = -1;
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
static int GetSectorWordSize(flash_sector_config_t* sector)
{
	// Error if sector alignment error
	if (sector->byteCount % 4)
		Error_Handler();
	return sector->byteCount / 4;
}

static inline bool IsSectorEmpty(flash_sector_config_t* sector)
{
	return sector->addr[0] == EMPTY_WORD;
}

static bool IsSectorErased(flash_sector_config_t* sector)
{
	int wordSize = GetSectorWordSize(sector);
	uint32_t* addr = sector->addr;
	while (wordSize--)
	{
		if(*addr++ != EMPTY_WORD)
			return false;
	}
	return true;
}

static void EraseSector(flash_sector_config_t* sector)
{
	sector->index = 0;
	FLASH_Erase_Sector(sector->sectorNo, sector->bank, FLASH_VOLTAGE_RANGE_4);
}

static void EraseSectorIfRequired(flash_sector_config_t* sector)
{
	sector->index = 0;
	if (IsSectorErased(sector) == false)
		EraseSector(sector);
}

static int GetSectorWordCount(flash_sector_config_t* sector)
{
	int wordSize = GetSectorWordSize(sector);
	while (wordSize--)
	{
		if(sector->addr[wordSize] != EMPTY_WORD)
			break;
	}
	return wordSize + 1;
}

static int GetSectorWordsLeft(flash_sector_config_t* sector)
{
	int wordSize = GetSectorWordSize(sector);
	return wordSize - sector->index;
}

static int HasEnoughSpace(flash_sector_config_t* sector, int wordSize)
{
	return GetSectorWordsLeft(sector) > GET_FLASH_ALIGNED_SIZE(wordSize);
}

static bool IsFirstSectorPacketValid(flash_sector_config_t* _sector)
{
	uint32_t* addr0 = _sector->addr;
	uint32_t packageSize = GET_FLASH_ALIGNED_SIZE(STORE_WORD_SIZE);


	return (addr0[0] != STORE_WORD_SIZE ||
			addr0[packageSize - 1] != STORE_WORD_SIZE ||
			addr0[1] != 0 ||
			addr0[2] != STORAGE_HEADER_VALUE) ? false : true; //addr0[STORE_WORD_SIZE + 3] != STORAGE_FOOTER_VALUE
}

static int GetInitialSector(void)
{
	bool isSectorValid[2];
	int _sectorIndex = -1;
	int sectorCount[2];
	isSectorValid[0] = IsFirstSectorPacketValid(&config->sectors[0]);
	isSectorValid[1] = IsFirstSectorPacketValid(&config->sectors[1]);

	// if both sectors have valid data select the one to use and erase the other one if required
	if (isSectorValid[0] && isSectorValid[1])
	{
		sectorCount[0] = GetSectorWordCount(&config->sectors[0]);
		sectorCount[1] = GetSectorWordCount(&config->sectors[1]);

		_sectorIndex = sectorCount[0] < sectorCount[1] ? 0 : 1;
		EraseSectorIfRequired(&config->sectors[_sectorIndex == 0 ? 1 : 0]);
	}
	else if (isSectorValid[0])
	{
		_sectorIndex = 0;
		EraseSectorIfRequired(&config->sectors[1]);
	}
	else if (isSectorValid[1])
		{
			_sectorIndex = 1;
			EraseSectorIfRequired(&config->sectors[0]);
		}
	else
	{
		EraseSectorIfRequired(&config->sectors[0]);
		EraseSectorIfRequired(&config->sectors[1]);
	}
	return _sectorIndex;
}

static void FillDataFromSector(flash_sector_config_t* sector)
{
	uint32_t* addr = sector->addr;
	uint32_t* endAddr = addr + (sector->byteCount / 4);
	while(addr < endAddr)
	{
		if (*addr == EMPTY_WORD)
			break;
		/****** Basic Validation *****/
		uint32_t* addr0 = addr;
		int dataLen = *addr++;
		// Skip further writes in sector if invalid start and end length,
		// as both should match
		if (dataLen > STORE_WORD_SIZE || dataLen <= 0)
		{
			addr = endAddr;
			break;
		}
		uint32_t packageSize = GET_FLASH_ALIGNED_SIZE(dataLen);
		if (addr0[packageSize - 1] != dataLen)
		{
			addr = endAddr;
			break;
		}
		uint32_t index = *addr++;
		// skip sector
		if (index > (STORE_WORD_SIZE - dataLen))
		{
			addr = endAddr;
			addr = addr0 + packageSize;
			continue;
		}
		/****** Basic Validation *****/
		uint32_t* storeLoc = config->store;
		storeLoc += index;
		while (dataLen--)
			*storeLoc++ = *addr++;
		addr = addr0 + packageSize;
	}
	sector->index = addr - sector->addr;
}

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
void StateStorage_Init(state_storage_config_t* _config)
{
	config = _config;

	// force the initial values of each variable to zero.
	memset((void*)config->store, 0, STORE_BYTE_SIZE);
	config->sectors[0].index = config->sectors[1].index = 0;

	sectorIndex = GetInitialSector();
	bool isDataValid = sectorIndex != -1;
	if (isDataValid)
		FillDataFromSector(&config->sectors[sectorIndex]);
	else
		sectorIndex = 0;

	uint32_t* storeLoc = config->store;
	for (int i = 0; i < config->clientCount; i++)
	{
		if (config->clients[i].InitStatesFromStorage == NULL ||
				config->clients[i].RefreshStates == NULL ||
				config->clients[i].dataWordLen == 0)
			Error_Handler();
		// confirm if header and footer are correct
		if (isDataValid)
		{
			isDataValid = (storeLoc[0] == STORAGE_HEADER_VALUE)  &&
					(storeLoc[config->clients[i].dataWordLen + 1] == STORAGE_FOOTER_VALUE);
		}
		// restore states if valid
		config->clients[i].InitStatesFromStorage(storeLoc + 1, isDataValid);
		// size should include a local header and footer also
		storeLoc += GET_LOCAL_LEN(config->clients[i].dataWordLen);
	}

	// Invalid store size, kindly increase store size
	if ((storeLoc - (uint32_t*)config->store) > STORE_WORD_SIZE)
		Error_Handler();
}

static void PutDataInFlash(uint32_t* _storeLoc, uint32_t _index, uint32_t _wordSize, bool _isFirstSectorPacket)
{
	uint32_t packetWordSize = GET_FLASH_ALIGNED_SIZE(_wordSize);
	if (_isFirstSectorPacket)
	{
		if (config->sectors[sectorIndex].index != 0)
			sectorIndex = sectorIndex == 0 ? 1 : 0;
	}

	if (packetWordSize % FLASH_WORD_ALIGNMENT)
		Error_Handler();

	uint32_t packCount = packetWordSize / FLASH_WORD_ALIGNMENT;

	// Create first word
	uint32_t data[FLASH_WORD_ALIGNMENT];
	data[0] = _wordSize;
	data[1] = _index;
	uint32_t len = (FLASH_WORD_ALIGNMENT - HEADER_WORD_SIZE);
	if (packCount == 1)
	{
		len = _wordSize;
		data[FLASH_WORD_ALIGNMENT - 1] = _wordSize;
	}
	uint32_t* storeLoc = _storeLoc;
	for (uint32_t i = 2; i < len + 2; i++)
		data[i] = *storeLoc++;
	uint32_t dataLeft = _wordSize - len;
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, (uint32_t)(config->sectors[sectorIndex].addr + config->sectors[sectorIndex].index), (uint32_t)data);
	config->sectors[sectorIndex].index += FLASH_WORD_ALIGNMENT;

	for (uint32_t packNo = 1; packNo < packCount; packNo++)
	{
		uint32_t len = FLASH_WORD_ALIGNMENT;
		if(packNo == packCount - 1)
		{
			len = dataLeft;
			data[FLASH_WORD_ALIGNMENT - 1] = _wordSize;
		}
		for (uint32_t i = 0; i < len; i++)
			data[i] = *storeLoc++;
		dataLeft -= len;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, (uint32_t)(config->sectors[sectorIndex].addr + config->sectors[sectorIndex].index), (uint32_t)data);
		config->sectors[sectorIndex].index += FLASH_WORD_ALIGNMENT;
	}

	if (_isFirstSectorPacket)
		EraseSector(&config->sectors[sectorIndex == 0 ? 1 : 0]);
}

static uint32_t RefreshStatesLocal(uint32_t* storeLoc, state_storage_client_t* client, uint32_t* index)
{
	uint32_t wordSize = client->RefreshStates(storeLoc + 1, index);
	if (storeLoc[0] != STORAGE_HEADER_VALUE || storeLoc[client->dataWordLen + 1] != STORAGE_FOOTER_VALUE)
	{
		storeLoc[0] = STORAGE_HEADER_VALUE;
		storeLoc[client->dataWordLen + 1] = STORAGE_FOOTER_VALUE;
		*index = 0;
		return GET_LOCAL_LEN(client->dataWordLen);
	}
	return GET_LOCAL_LEN(wordSize);
}

/**
 * @brief Refreshes the storage state if required.
 * @details Poll this function periodically to refresh the stored states for all parameters. If the index in the
 * sector is 0 or the packet cannot fit within the remaining space in the sector, the \"isFirstSectorPacket\" flag
 * is set. This flag ensures that all parameters are completely flushed to the beginning of the sector.
 * To obtain the necessary data, each client is prompted to refresh their states using the @ref state_storage_client_t.RefreshStates() function.
 * It is the responsibility of the client to provide the refreshed data, if necessary, and return the size of
 * the updated data. Finally, the partial data will be flushed to the storage flash.
 */
void StateStorage_Refresh(void)
{
	if (sectorIndex == -1)
		Error_Handler();
	uint32_t* storeLoc = config->store;
	bool isFirstSectorPacket = config->sectors[sectorIndex].index == 0;
	uint32_t index = 0;
	uint32_t localIndex = 0;
	for (int i = 0; i < config->clientCount; i++)
	{
		localIndex = 0;
		uint32_t wordSize = RefreshStatesLocal(storeLoc, &config->clients[i], &localIndex);
		if (wordSize > 0 && !isFirstSectorPacket)
		{
			if (HasEnoughSpace(&config->sectors[sectorIndex], wordSize) == false)
				isFirstSectorPacket = true;
			else
				PutDataInFlash(storeLoc, index + localIndex, wordSize, false);		// one index for local header
		}
		// size includes local header and footer
		storeLoc += GET_LOCAL_LEN(config->clients[i].dataWordLen);
		index += GET_LOCAL_LEN(config->clients[i].dataWordLen);
	}
	if (isFirstSectorPacket)
		PutDataInFlash(config->store, 0, STORE_WORD_SIZE, true);
}

/* EOF */
