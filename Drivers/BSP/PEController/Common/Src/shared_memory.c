/*! 
@file SharedMemory.c
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "shared_memory.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
 
/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/

 /*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile shared_data_t * const sharedData = (shared_data_t *)0x38001000;
/*******************************************************************************
 * Code
 ******************************************************************************/
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
