/**
 ********************************************************************************
 * @file    	shared_memory.c
 * @author 		Waqas Ehsan Butt
 * @date    	December 3, 2021
 *
 * @brief   Contains shared memory structures for the PEController
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
#include "p2p_comms.h"
#include "shared_memory.h"
#include "pecontroller_adc.h"
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
/** Pointer to the shared data variable
 */
volatile shared_data_t * const sharedData = (shared_data_t *)0x38000000;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Initialize the shared memory
 */
void SharedMemory_Init(void)
{
#if !IS_STORAGE_CORE
	sharedData->isStateStorageInitialized = false;
#endif
#if IS_CONTROL_CORE
	P2PComms_InitData();
#endif
#if IS_ADC_CORE
	BSP_ADC_SetDefaultParams((adc_processed_data_t*)&PROCESSED_ADC_DATA, (adc_raw_data_t*)&RAW_ADC_DATA);
#endif
}

/**
 * @brief Refresh the shared memory
 */
void SharedMemory_Refresh(void)
{
#if IS_CONTROL_CORE
	P2PComms_ProcessPendingRequests();
#endif
#if IS_ADC_CORE
	BSP_ADC_RefreshData();
#endif
}

/* EOF */


