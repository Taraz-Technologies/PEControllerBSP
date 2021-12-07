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
#include "shared_memory.h"
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
volatile shared_data_t * const sharedData = (shared_data_t *)0x38001000;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Get values for the recent measurements from the shared memory
 * @param *adc Pointer to the ADC values to be filled
 */
void SharedMemory_GetRecentMeasurements(adc_measures_t* adc)
{
	if(sharedData->m4Tom7.lastDataPointer != NULL)
		memcpy((void*)adc, (void*)sharedData->m4Tom7.lastDataPointer, sizeof(adc_measures_t));
}


/* EOF */


