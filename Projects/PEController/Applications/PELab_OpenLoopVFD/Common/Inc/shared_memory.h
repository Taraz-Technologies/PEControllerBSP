/**
 ********************************************************************************
 * @file 		shared_memory.h
 * @author 		Waqas Ehsan Butt
 * @date 		Dec 3, 2021
 *
 * @brief	Contains shared memory structures for the PEController
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

#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @defgroup Common Common
 * @brief This module contains the base functionality and definitions for common files
 * @{
 */

/** @defgroup SharedMemory Shared Memory
 * @brief This module contains the base functionality and definitions for shared memory between both cores
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "adc_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup SHAREDMEM_Exported_Macros Macros
  * @{
  */

/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup SHAREDMEM_Exported_Typedefs Type Definitions
  * @{
  */

/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup SHAREDMEM_Exported_Structures Structures
  * @{
  */
/**
 * @brief Defines parameters of Cortex-M7 core to be shared with Cortex-M4
 */
typedef struct
{
#if ADC_CORE == ADC_CM4
	volatile uint8_t sts;							/**< @brief Status variable */
	uint32_t periodUs;								/**< @brief Period of the controller in micro-seconds.
													CM7 can control the adc conversion rate from this variable */
	uint32_t avgCount;								/**< @brief Averaging count for the moving average filter */
#else
	adc_raw_data_t rawAdcData;
	adc_processed_data_t processedAdcData;
#endif
} m7_to_m4_data_t;
/**
 * @brief Defines parameters of Cortex-M4 core to be shared with Cortex-M7
 */
typedef struct
{
#if ADC_CORE == ADC_CM4
	adc_raw_data_t rawAdcData;
	adc_processed_data_t processedAdcData;
#else
	volatile uint8_t sts;								/**< @brief Status variable */
#endif
} m4_to_m7_data_t;
/**
 * @brief Buffer containing all shared data between both cores
 */
typedef struct
{
	m4_to_m7_data_t m4Tom7;					/**< @brief Defines parameters of Cortex-M4 core to be shared with Cortex-M7 */
	m7_to_m4_data_t m7Tom4;					/**< @brief Defines parameters of Cortex-M7 core to be shared with Cortex-M4 */
}shared_data_t;
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup SHAREDMEM_Exported_Variables Variables
  * @{
  */
/** Pointer to the shared data variable
 */
extern volatile shared_data_t * const sharedData;
/**
 * @}
 */
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup SHAREDMEM_Exported_Functions Functions
  * @{
  */
/**
 * @brief Get values for the recent measurements from the shared memory
 * @param *adc Pointer to the ADC values to be filled
 */
extern void SharedMemory_GetRecentMeasurements(adc_measures_t* adc);
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

/**
 * @}
 */

#endif
/* EOF */

