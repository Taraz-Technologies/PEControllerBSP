/**
 ********************************************************************************
 * @file    	monitoring_library.c
 * @author 		Waqas Ehsan Butt
 * @date    	Apr 7, 2023
 *
 * @brief   
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
#ifndef MONITORING_LIB_C_
#define MONITORING_LIB_C_
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "monitoring_library.h"
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

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
#pragma GCC push_options
#pragma GCC optimize ("-Ofast")

/**
 * @brief Insert new data for computation of statistics. If samples are enough statistics will be computed.
 * @param data Pointer to the first element of the new data array
 * @param stats Pointer to the first element of the statistics array
 * @param count Number of consecutive statistics computations
 */
uint32_t Stats_Compute(float* data, temp_stats_data_t* tempStats, stats_data_t* stats, int count)
{
	uint32_t result = 0;
	while (count--)
	{
		// Compute results if required
		if (tempStats->samplesLeft-- > 0)
		{
			// Compute the temporary values
			tempStats->rms += ((*data) * (*data));
			tempStats->avg += (*data);
			if (tempStats->max < *data)
				tempStats->max = *data;
			if (tempStats->min > *data)
				tempStats->min = *data;
		}
		else
		{
			// get new values
			tempStats->samplesLeft = tempStats->sampleCount = 10000; // --TODO--
			stats->rms = sqrtf(tempStats->rms / tempStats->sampleCount);
			stats->avg = tempStats->avg / tempStats->sampleCount;
			stats->max = tempStats->max;
			stats->min = tempStats->min;
			stats->pkTopk = stats->max - stats->min;

			// reset temp stats
			tempStats->rms = tempStats->avg = 0;
			tempStats->max = -4294967296;
			tempStats->min = 4294967296;
			result = (uint32_t)~(0);
			stats++;
		}
		data++;
		tempStats++;
	}
	return result;
}

#pragma GCC pop_options
#endif
/* EOF */
