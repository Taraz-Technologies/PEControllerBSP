/*! 
@file MAX11046App.c
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
#pragma O3
#pragma Otime
/*******************************************************************************
 * Includes
 ******************************************************************************/
#ifdef CORE_CM4
#include "raw_max11046.h"
#endif
#include "MAX11046App.h"
#ifdef CORE_CM7
#include "SharedMemory.h"
#include <string.h>
#endif
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define MAX_PERIOD_US			(1000)
#define MIN_PERIOD_US			(5)
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
#ifdef CORE_CM4
uint16_t* adcRawVals;
/*! @brief data callback storage */
static adcMeauresDataCallback callback = 0;
/*! @brief adc value conversion computers */
static adc_measures_t offsets;
static adc_measures_t mults;
#endif
adc_measures_t adcVals;
/*******************************************************************************
 * Code
 ******************************************************************************/
#ifdef CORE_CM4
/*!
@brief Processes the adc data acquired from the adc 
@details this function will call the control system module for evaluation
@param *u16Results- pointer to the values acquired
*/
void MAX11046App_Processing(uint16_t* u16Results)
{
	float* fResults = (float*)&adcVals;
	adcRawVals = u16Results;
	float* adcOffsets = (float*)&offsets;
	float* adcMults = (float*)&mults;
	
	/* convert the values */
	int i = 15;
	do
	{
		fResults[i] = (u16Results[i] - adcOffsets[i]) * adcMults[i];
	} while(i--);
	
	/* call user defined function for control system evaluation */
	if(callback)
		callback(&adcVals);
}

/*!
@brief Initialize the module for external ADC measurements
@param periodUs- period in Us for the continuous conversion mode
@param dataCallback- callback for the data processing
*/
void MAX11046App_Init(uint32_t periodUs, adcMeauresDataCallback dataCallback)
{
	adc_acq_mode_t mode = ADC_MODE_CONT;
	
	if(periodUs > MAX_PERIOD_US)
		periodUs = MAX_PERIOD_US;
	else if (periodUs < MIN_PERIOD_US)
		periodUs = MIN_PERIOD_US;
	
	adc_cont_config_t config = { 
		.conversionCycleTimeUs = periodUs, 
		.callback = MAX11046App_Processing };
	
	float* adcOffsets = (float*)&offsets;
	float* adcMults = (float*)&mults;
	
	/* set the offsets and multipliers according to the connections */
	for	(int i = 0; i < 16; i++)
	{
		adcOffsets[i] = 32768;
		adcMults[i] = 1;
	}
	
	mults.Ih1 = 50.0f / 32768;
	mults.Ih2 = 50.0f / 32768;
	mults.Ih3 = 50.0f / 32768;
	mults.Ie1 = 50.0f / 32768;
	mults.Ie2 = 50.0f / 32768;
	mults.Ie3 = 50.0f / 32768;
	
	mults.Vdc1 = 1000.0f / 32768;
	mults.V1 = 1000.0f / 32768;
	mults.V2 = 1000.0f / 32768;
	mults.Vdc2 = 1000.0f / 32768;
	mults.V3 = 1000.0f / 32768;
	mults.V4 = 1000.0f / 32768;
		
	callback = dataCallback;
	Raw_Max11046_Init(mode, &config);
}

/*! @brief start the measurements related to MAX11046 */
void MAX11046App_Run(void)
{
	Raw_Max11046_Run();
}
#endif
#ifdef CORE_CM7
void MAX11046_Poll(void)
{
	if(sharedData->m4Tom7.lastDataPointer != NULL)
		memcpy((void*)&adcVals, (void*)sharedData->m4Tom7.lastDataPointer, sizeof(adc_measures_t));
}
#endif
/* EOF */
