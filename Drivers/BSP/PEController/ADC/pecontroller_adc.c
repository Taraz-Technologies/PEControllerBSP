/**
 ********************************************************************************
 * @file    	pecontroller_adc.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 29, 2023
 *
 * @brief   Controls the ADC functionality.
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
 ********************************************************************************
 */

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
#include "general_header.h"

#include "error_config.h"
#include "data_config.h"
#include "ring_buffer.h"
#include "state_storage_lib.h"
#include "adc_config.h"
#include "pecontroller_adc.h"
#if MAX11046_ENABLE
#include "max11046_drivers.h"
#endif
#include "monitoring_library.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define MIN_SENSITIVITY					(.001f)
#define MAX_SENSITIVITY					(10.f)
#define MIN_OFFSET						(-1000.f)
#define MAX_OFFSET						(1000.f)
#define MIN_FREQ						(1.f)
#define MAX_FREQ						(500.f)
#define DEFAULT_FREQ					(50.f)
#define DEFAULT_SENSITIVITY				(1.f)
#define DEFAULT_OFFSET					(0.f)
#define DEFAULT_UNIT					(UNIT_V)

#define STORAGE_WORD_LEN				((TOTAL_MEASUREMENT_COUNT * 3) + ((TOTAL_MEASUREMENT_COUNT / sizeof(uint32_t)) * sizeof(uint8_t)))
#define GET_SAMPLE_COUNT(_fs, _f)		(((uint32_t)_fs) - ((uint32_t)_fs) % ((uint32_t)_f)) / 2
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
#if IS_ADC_STATS_CORE && ADC_BULK_STATS
temp_stats_data_t tempStats[TOTAL_MEASUREMENT_COUNT] = {0};
#endif
#if (IS_ADC_STATS_CORE && ADC_BULK_STATS) || IS_STORAGE_CORE || IS_ADC_CORE
static adc_processed_data_t* processedAdcData = NULL;
#endif
#if IS_ADC_CORE
static adc_raw_data_t* rawAdcData = NULL;
#if USE_LOCAL_ADC_STORAGE
static ring_buffer_t adcProcessedRingBuff = { .rdIndex = 0, .wrIndex = 0, .modulo = RAW_MEASURE_SAVE_COUNT - 1 };
#if !EN_DMA_ADC_DATA_COLLECTION
static ring_buffer_t adcRawRingBuff = { .rdIndex = 0, .wrIndex = 0, .modulo = MEASURE_SAVE_COUNT - 1 };
#endif
#endif
#endif
/********************************************************************************
 * Global Variables
 *******************************************************************************/
#if IS_ADC_CORE
/**
 * @brief Offsets to be applied to the ADC readings
 */
float adcOffsets[TOTAL_MEASUREMENT_COUNT] = {0};
/**
 * @brief Sensitivities for the ADC readings
 */
float adcSensitivity[TOTAL_MEASUREMENT_COUNT] = {0};
#if USE_LOCAL_ADC_STORAGE
#if !EN_DMA_ADC_DATA_COLLECTION
/**
 * @brief Local buffer containing raw ADC data to enhance ADC performance
 */
uint16_t adcLocalRawStorage[LOCAL_ADC_STORAGE_COUNT][TOTAL_MEASUREMENT_COUNT] = {0};
#endif
/**
 * @brief Local buffer containing converted ADC data to enhance ADC performance
 */
float adcLocalConvStorage[LOCAL_ADC_STORAGE_COUNT][TOTAL_MEASUREMENT_COUNT] = {0};
/**
 * @brief Ring buffer to contain ADC read and write locations
 */
ring_buffer_t adcLocalIndexRingBuff = { .rdIndex = 0, .wrIndex = 0, .modulo = LOCAL_ADC_STORAGE_COUNT - 1 };
#endif
#endif
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
#if IS_ADC_CORE
#pragma GCC push_options
#pragma GCC optimize ("-Ofast")
/**
 * @brief Updates the data parameters and data sharing items.
 * @note Should be called frequently to avoid data missing.
 */
void BSP_ADC_RefreshData(void)
{
	int i = TOTAL_MEASUREMENT_COUNT;
	while (i--)
	{
		adcSensitivity[i] = (10.f / 32768.f) / processedAdcData->info.sensitivity[i];
		adcOffsets[i] = 32768.f + (processedAdcData->info.offsets[i] / adcSensitivity[i]);
	}

#if USE_LOCAL_ADC_STORAGE
	// Get available count for writing available with one write option
	int countSrc = RingBuffer_GetPendingReadCountsTillEnd(&adcLocalIndexRingBuff);

	// Get smallest size of destination transfer available
	int countProcessed = RingBuffer_GetCountTillSize(&adcProcessedRingBuff);
#if !EN_DMA_ADC_DATA_COLLECTION
	int countRaw = RingBuffer_GetCountTillSize(&adcRawRingBuff);
	int count = countProcessed < countSrc ?
			(countRaw < countProcessed ? countRaw : countProcessed) :
			(countRaw < countSrc ? countRaw : countSrc);
	memcpy((void*)&rawAdcData->dataRecord[adcRawRingBuff.rdIndex * TOTAL_MEASUREMENT_COUNT],
			(void*)adcLocalRawStorage[adcLocalIndexRingBuff.rdIndex], count * TOTAL_MEASUREMENT_COUNT * sizeof(uint16_t));
	RingBuffer_Read_Count(&adcRawRingBuff, count);
	rawAdcData->recordIndex = adcRawRingBuff.rdIndex;
#else
	int count = countProcessed < countSrc ? countProcessed : countSrc;
#endif

	memcpy((void*)&processedAdcData->dataRecord[adcProcessedRingBuff.rdIndex],
			(void*)adcLocalConvStorage[adcLocalIndexRingBuff.rdIndex], count * TOTAL_MEASUREMENT_COUNT * sizeof(float));
	RingBuffer_Read_Count(&adcProcessedRingBuff, count);
	processedAdcData->recordIndex = adcProcessedRingBuff.rdIndex;

	RingBuffer_Read_Count(&adcLocalIndexRingBuff, count);
#endif
}
/**
 * @brief Set default parameters for the ADC
 * @param _processedAdcData Pointer to the processed ADC data container
 * @param _rawAdcData Pointer to the raw ADC data container
 */
void BSP_ADC_SetDefaultParams(adc_processed_data_t* _processedAdcData, adc_raw_data_t* _rawAdcData)
{
	_processedAdcData->recordIndex = 0;
	_rawAdcData->recordIndex = 0;
	processedAdcData = _processedAdcData;
	rawAdcData = _rawAdcData;
	BSP_ADC_RefreshData();
}
#pragma GCC pop_options
/**
 * @brief Initializes the ADC drivers.
 * @param _type ADC_MODE_SINGLE or ADC_MODE_CONT for single or continuous conversions respectively
 * @param _contConfig adc_cont_config_t contains the continuous transfer configuration
 * @param _rawAdcData Pointer to the raw ADC data container
 * @param _processedAdcData Pointer to the processed ADC data container
 */
void BSP_ADC_Init(adc_acq_mode_t _type, adc_cont_config_t* _contConfig, volatile adc_raw_data_t* _rawAdcData, volatile adc_processed_data_t* _processedAdcData)
{
#if MAX11046_ENABLE
	BSP_ADC_RefreshData();
	BSP_MAX11046_Init(_type, _contConfig, _rawAdcData, _processedAdcData);
#else
#error "Invalid ADC."
#endif
}

/**
 * @brief Starts the ADC conversion
 * @details For single conversion mode performs and provides the results in a blocking way.
 * For continuous conversion mode starts the conversions, after each conversion adc_cont_config_t.callback will be called
 * @return adc_measures_t_t* For single conversion mode returns the pointer to the acquired values. Returns NULL for continuous conversion mode
 */
adc_measures_t* BSP_ADC_Run(void)
{
#if MAX11046_ENABLE
	return BSP_MAX11046_Run();
#else
#error "Invalid ADC."
#endif
}

/**
 * @brief Stops the ADC conversions
 */
void BSP_ADC_Stop(void)
{
#if MAX11046_ENABLE
	BSP_MAX11046_Stop();
#else
#error "Invalid ADC.";
#endif
}

/**
 * @brief De-initialize the ADC drivers
 */
void BSP_ADC_DeInit(void)
{
#if MAX11046_ENABLE
	BSP_MAX11046_DeInit();
#else
#error "Invalid ADC.";
#endif
}

/**
 * @brief Sets the input and output trigger functions for the ADC.
 * @note Currently only the @ref TIM_TRGI_TYPE_RST, @ref TIM_TRGI_TYPE_START and @ref TIM_TRGI_TYPE_RESET_AND_START
 * functionalities are available for this timer synchronization. Delays can't be added in master or slave mode triggering through OC1.
 * --TODO-- Add list of possible timers for triggering in different modes.
 * @param _slaveConfig Configuration for the slave. Send NULL if no slave functionality needed.
 * @param _masterConfig Configuration for the master. Send NULL if no master functionality needed.
 * @param _fs Required sampling frequency of the ADC converter
 * @return timer_trigger_src_t Trigger source if configuration required as master else returns NULL.
 */
timer_trigger_src_t BSP_ADC_SetInputOutputTrigger(tim_in_trigger_config_t* _slaveConfig, tim_out_trigger_config_t* _masterConfig, float _fs)
{
#if MAX11046_ENABLE
	return BSP_MAX11046_SetInputOutputTrigger(_slaveConfig, _masterConfig, _fs);
#else
#error "Invalid ADC.";
#endif
}

#endif

#if IS_COMMS_CORE

/**
 * @brief Update the configurations for the ADC
 * @param _info ADC information
 * @param _fs ADC sampling frequency
 * @param _channelIndex ADC channel index
 * @param _freq Desired frequency
 * @param _sensitivity Desired sensitivity
 * @param _offset Desired offset
 * @param unit Desired unit
 * @return <c>ERR_OK</c> if setting successful else @ref device_err_t.
 */
device_err_t BSP_ADC_UpdateConfig(adc_info_t* _info, float _fs, int _channelIndex, float _freq, float _sensitivity, float _offset, data_units_t _unit)
{
	if (_freq > MAX_FREQ || _freq < MIN_FREQ)
		return ERR_OUT_OF_RANGE;
	if (_sensitivity > MAX_SENSITIVITY || _sensitivity < MIN_SENSITIVITY)
		return ERR_OUT_OF_RANGE;
	if (_offset > MAX_OFFSET || _offset < MIN_OFFSET)
		return ERR_OUT_OF_RANGE;
	if (_unit >= UNIT_COUNT)
		return ERR_OUT_OF_RANGE;
	if (_channelIndex >= TOTAL_MEASUREMENT_COUNT)
		return ERR_NOT_AVAILABLE;
	_info->freq[_channelIndex] = _freq;
	_info->sensitivity[_channelIndex] = _sensitivity;
	_info->offsets[_channelIndex] = _offset;
	_info->units[_channelIndex] = _unit;
#if IS_ADC_STATS_CORE && ADC_BULK_STATS
	tempStats[_channelIndex].sampleCount = GET_SAMPLE_COUNT(_fs, _freq);
#endif
	return ERR_OK;
}

#endif

#if IS_ADC_STATS_CORE && ADC_BULK_STATS

/**
 * @brief Call this function periodically to compute the statistics of the signals
 * @param _processedAdcData Pointer to the processed data structure.
 * @param _fs Sampling frequency of the ADC.
 */
void BSP_ADC_ComputeStatsInBulk(adc_processed_data_t* _processedAdcData, float _fs)
{
	static ring_buffer_t ringBuffLocal = { .wrIndex = 0, .rdIndex = 0, .modulo = MEASURE_SAVE_COUNT - 1 };
	static bool init = false;
	static float fs;

	if (init == false)
	{
		if (processedAdcData == NULL)
			processedAdcData = _processedAdcData;
		Stats_Reset(tempStats, (stats_data_t*)processedAdcData->info.stats, 1000, TOTAL_MEASUREMENT_COUNT);
		fs = _fs;
		for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
		{
			tempStats[i].sampleCount = tempStats[i].samplesLeft = GET_SAMPLE_COUNT(_fs, processedAdcData->info.freq[i]);
		}
		init = true;
	}
	if (_fs != fs)
	{
		fs = _fs;
		Stats_Reset(tempStats, NULL, 1000, 16);
		for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
			tempStats[i].sampleCount = tempStats[i].samplesLeft = GET_SAMPLE_COUNT(fs, processedAdcData->info.freq[i]);
	}

	ringBuffLocal.wrIndex = processedAdcData->recordIndex;
	int pend = RingBuffer_GetPendingReadCount(&ringBuffLocal);


	if(pend)
	{
		int straightCount = RingBuffer_GetCountTillSize(&ringBuffLocal);
		float* data = (float*)&processedAdcData->dataRecord[ringBuffLocal.rdIndex];

		Stats_Compute_MultiSample_16ch(data, tempStats, (stats_data_t*)processedAdcData->info.stats, pend < straightCount ? pend : straightCount);
		if (pend > straightCount)
			Stats_Compute_MultiSample_16ch((float*)&processedAdcData->dataRecord[0], tempStats, (stats_data_t*)processedAdcData->info.stats, pend - straightCount);

		ringBuffLocal.rdIndex = ringBuffLocal.wrIndex;
	}
}

#endif

#if IS_STORAGE_CORE

static void InitStatesFromStorage(uint32_t* data, bool isDataValid)
{
	float* localData = (float*)data;
	adc_info_t* info = &processedAdcData->info;
	if (isDataValid)
	{
		// Get decimal values
		for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
		{
			int baseIndex = i * 3;
			info->freq[i] = (localData[baseIndex] <= MAX_FREQ && localData[baseIndex] >= MIN_FREQ) ?
					localData[baseIndex] : DEFAULT_FREQ;
			baseIndex++;
			info->sensitivity[i] = (localData[baseIndex] <= MAX_SENSITIVITY && localData[baseIndex] >= MIN_SENSITIVITY) ?
					localData[baseIndex] : DEFAULT_SENSITIVITY;
			baseIndex++;
			info->offsets[i] = (localData[baseIndex] <= MAX_OFFSET && localData[baseIndex] >= MIN_OFFSET) ?
					localData[baseIndex] : DEFAULT_OFFSET;
		}

		// Get enumeration values
		uint8_t* localDataU8 = (uint8_t*)(localData + (TOTAL_MEASUREMENT_COUNT * 3));
		for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
			info->units[i] = localDataU8[i] < UNIT_COUNT ? localDataU8[i] : DEFAULT_UNIT;

	}
	else
	{
		// Set default values because the values are invalid
		for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
		{
			info->freq[i] = DEFAULT_FREQ;
			info->sensitivity[i] = DEFAULT_SENSITIVITY;
			info->offsets[i] = DEFAULT_OFFSET;
			info->units[i] = DEFAULT_UNIT;
		}
	}
}
static uint32_t RefreshStates(uint32_t* data, uint32_t* indexPtr)
{
	uint32_t len = 0;
	float* localData = (float*)data;
	adc_info_t* info = &processedAdcData->info;
	// Set decimal values
	for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
	{
		int baseIndex = i * 3;
		if (info->freq[i] != localData[baseIndex])
		{
			localData[baseIndex] = info->freq[i];
			len++;
		}
		baseIndex++;
		if (info->sensitivity[i] != localData[baseIndex])
		{
			localData[baseIndex] = info->sensitivity[i];
			len++;
		}
		baseIndex++;
		if (info->offsets[i] != localData[baseIndex])
		{
			localData[baseIndex] = info->offsets[i];
			len++;
		}
	}
	// Set enumeration values
	uint8_t* localDataU8 = (uint8_t*)(localData + (TOTAL_MEASUREMENT_COUNT * 3));
	for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
	{
		if (info->units[i] != localDataU8[i])
		{
			localDataU8[i] = info->units[i];
			len++;
		}
	}

	if (len != 0)
		len = STORAGE_WORD_LEN;
	*indexPtr = 0;
	return len;
}

/**
 * @brief Configures the storage client information.
 * @note Before calling this function set the @ref _config->arg parameter to system @ref processedAdcData structure.
 * @param _config Pointer to the relevant configuration to be filled.
 */
void BSP_ADC_ConfigStorage(state_storage_client_t* _config)
{
	processedAdcData = (adc_processed_data_t*)_config->arg;
	_config->InitStatesFromStorage = InitStatesFromStorage;
	_config->RefreshStates = RefreshStates;
	_config->dataWordLen = STORAGE_WORD_LEN;
}

#endif

/* EOF */
