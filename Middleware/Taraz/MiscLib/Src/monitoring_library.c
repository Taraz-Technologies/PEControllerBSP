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
void Stats_Insert_Compute(float* data, stats_t* stats, int count)
{
	while (count--)
	{
		// Compute the temporary values
		stats->tempData.rms += ((*data) * (*data));
		stats->tempData.avg += (*data);
		if (stats->tempData.max < *data)
			stats->tempData.max = *data;
		if (stats->tempData.min > *data)
			stats->tempData.min = *data;

		// Compute results if required
		if (--stats->samplesLeft <= 0)
		{
			// get new values
			stats->result.rms = stats->tempData.rms / stats->sampleCount;
			stats->result.avg = stats->tempData.avg / stats->sampleCount;
			stats->result.max = stats->tempData.max;
			stats->result.min = stats->tempData.min;
			stats->result.pkTopk = stats->tempData.max - stats->tempData.min;

			// reset temp stats
			stats->tempData.rms = 0;
			stats->tempData.avg = 0;
			stats->tempData.max = -4294967296;
			stats->tempData.min = 4294967296;
			stats->samplesLeft = stats->sampleCount = 10000;
			stats->isUpdated = 0xFFFFFFFF;
		}
		stats++;
		data++;
	}
}
#pragma GCC pop_options
#endif
/* EOF */
