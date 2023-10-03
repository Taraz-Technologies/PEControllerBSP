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
#include "interprocessor_comms.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup SHAREDMEM_Exported_Macros Macros
 * @{
 */
/**
 * @brief Shortcut for accessing raw ADC data.
 */
#define RAW_ADC_DATA				(sharedData->rawAdcData)
/**
 * @brief Shortcut for converted/processed raw ADC data.
 */
#define PROCESSED_ADC_DATA			(sharedData->processedAdcData)
/**
 * @brief Shortcut for accessing ADC related information.
 */
#define ADC_INFO					(PROCESSED_ADC_DATA.info)
/**
 * @brief Shortcut for accessing messages between CM4 and CM7 core.
 */
#define CORE_MSGS					(sharedData->interProcessorMsgs)
/**
 * @brief Shortcut for accessing data shared between CM4 and CM7 core.
 */
#define INTER_CORE_DATA				(sharedData->interProcessorMsgs.dataBuffs)
/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup SHAREDMEM_Exported_Structures Structures
 * @{
 */
/**
 * @brief Buffer containing all shared data between both cores
 */
typedef struct
{
	volatile bool isStateStorageInitialized;		/**< Flag indicating if the state storage module has restored states. */
	adc_raw_data_t rawAdcData;						/**< Raw ADC data */
	adc_processed_data_t processedAdcData;			/**< Converted ADC data */
	interprocessor_msg_data_t interProcessorMsgs;	/**< Structure handling the parameters and commjunications between CM4 and CM7 core. */
} shared_data_t;
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
 * @brief Initialize the shared memory
 */
extern void SharedMemory_Init(void);
/**
 * @brief Refresh the shared memory
 */
extern void SharedMemory_Refresh(void);
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

