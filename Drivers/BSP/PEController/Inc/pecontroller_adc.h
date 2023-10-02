/**
 ********************************************************************************
 * @file 		pecontroller_adc.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 29, 2023
 *
 * @brief    Controls the max11046 functionality
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

#ifndef PECONTROLLER_ADC_H
#define PECONTROLLER_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup ADC
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "adc_config.h"
#include "error_config.h"
#if IS_CONTROL_CORE
#include "pecontroller_timers.h"
#endif
#if IS_STORAGE_CORE
#include "state_storage_lib.h"
#endif
/********************************************************************************
 * Defines
 *******************************************************************************/
#define USE_LOCAL_ADC_STORAGE				(IS_LOCAL_STORAGE_FASTER)
#if USE_LOCAL_ADC_STORAGE
#define LOCAL_ADC_STORAGE_COUNT				(32)
#endif
#define EN_DMA_ADC_DATA_COLLECTION			(IS_DMA_ADC_DATA_COLLECTION_SUPERIOR)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup ADC_Exported_Typedefs Type Definitions
 * @{
 */
/**
 * @brief ADC acquisition mode definitions
 */
typedef enum _adc_acq_mode_
{
	ADC_MODE_SINGLE,	/**< @brief Used for single conversion mode */
	ADC_MODE_CONT   	/**< @brief Used for continuous conversion mode */
} adc_acq_mode_t;
/**
 * @brief Callback for ADC results
 * @param *result Pointer to the most recent ADC results
 */
typedef void (*adcMeauresDataCallback)(adc_measures_t* result);
/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup ADC_Exported_Structures Structures
 * @{
 */
/**
 * @brief Defines the parameters for continuous conversions
 */
typedef struct
{
	float fs;							/**< @brief Sampling Frequency for the ADC */
	adcMeauresDataCallback callback;	/**< @brief Callback function called when results are ready */
} adc_cont_config_t;

/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup ADC_Exported_Variables Variables
 * @{
 */
#if IS_ADC_CORE
/**
 * @brief Offsets to be applied to the ADC readings
 */
extern float adcOffsets[TOTAL_MEASUREMENT_COUNT];
/**
 * @brief Sensitivities for the ADC readings
 */
extern float adcSensitivity[TOTAL_MEASUREMENT_COUNT];
#if IS_LOCAL_STORAGE_FASTER
#if !EN_DMA_ADC_DATA_COLLECTION
/**
 * @brief Local buffer containing raw ADC data to enhance ADC performance
 */
extern uint16_t adcLocalRawStorage[LOCAL_ADC_STORAGE_COUNT][TOTAL_MEASUREMENT_COUNT];
#endif
/**
 * @brief Local buffer containing converted ADC data to enhance ADC performance
 */
extern float adcLocalConvStorage[LOCAL_ADC_STORAGE_COUNT][TOTAL_MEASUREMENT_COUNT];
/**
 * @brief Ring buffer to contain ADC read and write locations
 */
extern ring_buffer_t adcLocalIndexRingBuff;
#endif
#endif
/**
 * @}
 */
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup ADC_Exported_Functions Functions
 * @{
 */
#if IS_ADC_CORE
/**
 * @brief Set default parameters for the ADC
 * @param processedAdcData Pointer to the processed ADC data container
 * @param rawAdcData Pointer to the raw ADC data container
 */
extern void BSP_ADC_SetDefaultParams(adc_processed_data_t* processedAdcData, adc_raw_data_t* rawAdcData);
/**
 * @brief Updates the data parameters and data sharing items.
 * @note Should be called frequently to avoid data missing.
 */
extern void BSP_ADC_RefreshData(void);
/**
 * @brief Initializes the ADC drivers.
 * @param _type ADC_MODE_SINGLE or ADC_MODE_CONT for single or continuous conversions respectively
 * @param _contConfig adc_cont_config_t contains the continuous transfer configuration
 * @param _rawAdcData Pointer to the raw ADC data container
 * @param _processedAdcData Pointer to the processed ADC data container
 */
extern void BSP_ADC_Init(adc_acq_mode_t _type, adc_cont_config_t* _contConfig, volatile adc_raw_data_t* _rawAdcData, volatile adc_processed_data_t* _processedAdcData);
/**
 * @brief Starts the ADC conversion
 * @details For single conversion mode performs and provides the results in a blocking way.
 * For continuous conversion mode starts the conversions, after each conversion adc_cont_config_t.callback will be called
 * @return adc_measures_t_t* For single conversion mode returns the pointer to the acquired values. Returns NULL for continuous conversion mode
 */
extern adc_measures_t* BSP_ADC_Run(void);
/**
 * @brief Stops the ADC conversions
 */
extern void BSP_ADC_Stop(void);
/**
 * @brief De-initialize the ADC drivers
 */
extern void BSP_ADC_DeInit(void);
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
extern timer_trigger_src_t BSP_ADC_SetInputOutputTrigger(tim_in_trigger_config_t* _slaveConfig, tim_out_trigger_config_t* _masterConfig, float _fs);
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
 * @param _unit Desired unit
 * @return <c>ERR_OK</c> if setting successful else @ref device_err_t.
 */
extern device_err_t BSP_ADC_UpdateConfig(adc_info_t* _info, float _fs, int _channelIndex, float _freq, float _sensitivity, float _offset, data_units_t _unit);
#endif
#if IS_ADC_STATS_CORE && ADC_BULK_STATS
/**
 * @brief Call this function periodically to compute the statistics of the signals
 * @param _processedAdcData Pointer to the processed data structure.
 * @param _fs Sampling frequency of the ADC.
 */
extern void BSP_ADC_ComputeStatsInBulk(adc_processed_data_t* _processedAdcData, float _fs);
#endif
#if IS_STORAGE_CORE
/**
 * @brief Configures the storage client information.
 * @note Before calling this function set the @ref _config->arg parameter to system @ref processedAdcData structure.
 * @param _config Pointer to the relevant configuration to be filled.
 */
extern void BSP_ADC_ConfigStorage(state_storage_client_t* _config);
#endif
/********************************************************************************
 * Code
 *******************************************************************************/


/**
 * @}
 */
#ifdef __cplusplus
}
#endif
/**
 * @}
 */
/**
 * @}
 */
#endif 
/* EOF */
