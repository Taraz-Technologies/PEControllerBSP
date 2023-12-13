/**
 ********************************************************************************
 * @file 		adc_config.h
 * @author 		Waqas Ehsan Butt
 * @date 		November 30, 2021
 *
 * @brief   Defines the application dependent parameters of the PEController ADC
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

#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @defgroup ADC ADC
 * @brief This module contains all ADC related definitions
 * @{
 */

/** @defgroup ADC_Config Configuration
 * @brief This module contains the configurations for the PEController ADC
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
#include "monitoring_library.h"
#include "data_config.h"
#include "ring_buffer.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup ADCConfig_Exported_Macros Macros
  * @{
  */
/**
 * @brief Number of measurement logs to be kept
 * @note Ring buffer should be 2 ^ n
 */
#define MEASURE_SAVE_COUNT				(256)
/**
 * @brief Number of raw measurement logs to be kept for communication systems
 * @note Ring buffer should be 2 ^ n
 */
#define RAW_MEASURE_SAVE_COUNT			(256)
/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup ADCConfig_Exported_Typedefs Type Definitions
  * @{
  */
/**
 * @brief Measurement structure definitions
 */
typedef struct
{
	float Ch1;				/**< @brief Measurement for Channel No 1 */
	float Ch2;				/**< @brief Measurement for Channel No 2 */
	float Ch3;				/**< @brief Measurement for Channel No 3 */
	float Ch4;				/**< @brief Measurement for Channel No 4 */
	float Ch5;				/**< @brief Measurement for Channel No 5 */
	float Ch6;				/**< @brief Measurement for Channel No 6 */
	float Ch7;				/**< @brief Measurement for Channel No 7 */
	float Ch8;				/**< @brief Measurement for Channel No 8 */
	float Ch9;				/**< @brief Measurement for Channel No 9 */
	float Ch10;				/**< @brief Measurement for Channel No 10 */
	float Ch11;				/**< @brief Measurement for Channel No 11 */
	float Ch12;				/**< @brief Measurement for Channel No 12 */
	float Ch13;				/**< @brief Measurement for Channel No 13 */
	float Ch14;				/**< @brief Measurement for Channel No 14 */
	float Ch15;				/**< @brief Measurement for Channel No 15 */
	float Ch16;				/**< @brief Measurement for Channel No 16 */
} adc_measures_t;
/**
 * @brief Information structure for the ADC
 */
typedef struct
{
	data_units_t units[TOTAL_MEASUREMENT_COUNT]; 		/**< @brief Selected units of each ADC channel.*/
	float offsets[TOTAL_MEASUREMENT_COUNT];				/**< @brief Offset values of each ADC channel.*/
	float sensitivity[TOTAL_MEASUREMENT_COUNT];			/**< @brief Sensitivity values of each ADC channel.*/
	float freq[TOTAL_MEASUREMENT_COUNT];				/**< @brief Signal frequencies of each ADC channel, used to compute the statistics of each channel.*/
	stats_data_t stats[TOTAL_MEASUREMENT_COUNT];		/**< @brief Signal statistics of each ADC channel.*/
	float fs;											/**< @brief Current sampling rate of the ADC */
} adc_info_t;
/**
 * @brief Contains the stored raw/unconverted ADC results.
 */
typedef struct
{
	volatile int recordIndex;							/**< @brief Record index for the raw ADC results in the dataRecord buffer.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 @note The index increases the location in the buffer with an increment of 16.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	e.g. if recordIndex is 1, start index in the dataRecord will be 1*16 = 16. */
	uint16_t dataRecord[RAW_MEASURE_SAVE_COUNT * TOTAL_MEASUREMENT_COUNT] __attribute__ ((aligned (8)));	/**< @brief Buffer containing the raw ADC data. */
} adc_raw_data_t;
/**
 * @brief Contains the stored converted ADC results for all channels and there relevant statisitics.
 */
typedef struct
{
	volatile int recordIndex;							/**< @brief Record index for the processed ADC results in the dataRecord buffer. */
	adc_measures_t dataRecord[MEASURE_SAVE_COUNT];		/**< @brief Buffer containing the processed ADC data. */
	adc_info_t info;									/**< @brief ADC Information. */
} adc_processed_data_t;
/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/



#ifdef __cplusplus
}
#endif

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
#endif 
/* EOF */
