/*! 
@file SharedMemory.h
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "general_header.h"
#include "adc_config.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define MEASURE_SAVE_COUNT		(32)
/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/
typedef struct
{
	uint8_t sts;
	int recordIndex;
	volatile adc_measures_t* lastDataPointer;
	adc_measures_t movAvg;
	adc_measures_t dataRecord[MEASURE_SAVE_COUNT];
} m4_to_m7_data_t;
 
typedef struct
{
	uint8_t sts;
	uint32_t periodUs;
	uint32_t avgCount;
} m7_to_m4_data_t;

// inter-core buffers
typedef struct
{
	m4_to_m7_data_t m4Tom7;
	m7_to_m4_data_t m7Tom4;
}shared_data_t;
 /*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * @brief Get values for the recent measurements from the shared memory
 * @param *adc Pointer to the ADC values to be filled
 */
extern void SharedMemory_GetRecentMeasurements(adc_measures_t* adc);
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern volatile shared_data_t * const sharedData;
/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
