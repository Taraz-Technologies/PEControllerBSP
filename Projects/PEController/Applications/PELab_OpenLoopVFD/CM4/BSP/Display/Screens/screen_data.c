/**
 ********************************************************************************
 * @file    	screen_data.c
 * @author 		Waqas Ehsan Butt
 * @date    	Mar 14, 2023
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

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
#if LCD_DATA_MONITORING
#include "screen_data.h"
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
disp_measure_t chDispMeasures;
disp_var_t dispVars[5];
const char* measureTxts[5] = {"RMS", "Avg", "Max", "Min", "Pk-Pk"};
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Update all measurements required in the GUI interface
 * @note Make sure that this function is called every time a new set of values is acquired by the ADC drivers.
 * @param adcData Pointer to the raw data provided by the ADC unit
 */
void BSP_Display_UpdateMeasurements(float* dataPtr)
{
#if 0
	for (int i = 0; i < 16; i++)
	{
		disp_param_t* param = &chDisplayParams[i];
		if (param->srcType == PARAM_SRC_MEASUREMENT)
		{
			param_measures_t* measure = &param->src.measure;
			param_measures_temp_vars_t* temps = &(measure->temps);
			float data = dataPtr[measure->channelIndex];//((adcData[measure->channelIndex] - 32768.f) * measure->sensitivity) - measure->offset;
			switch (measure->type)
			{
			case DISP_RMS:
				temps->temp += data * data;
				break;
			case DISP_AVG:
				temps->temp += data;
				break;
			case DISP_MAX:
				if (temps->temp1 < data)
					temps->temp1 = data;
				break;
			case DISP_MIN:
				if (temps->temp1 > data)
					temps->temp1 = data;
				break;
			case DISP_PkToPk:
				if (temps->temp1 < data)
					temps->temp1 = data;
				if (temps->temp2 > data)
					temps->temp2 = data;
				break;
			default:
				temps->temp = 0;
				break;
			}
			if (--temps->index <= 0)
			{
				switch (measure->type)
				{
				case DISP_RMS:
					temps->temp = sqrtf(temps->temp / temps->maxIndex);
					break;
				case DISP_AVG:
					temps->temp = temps->temp / temps->maxIndex;
					break;
				case DISP_PkToPk:
					temps->temp = temps->temp1 - temps->temp2;
					temps->temp1 = -2147483647;
					temps->temp2 = 2147483647;
					break;
				case DISP_MAX:
					temps->temp = temps->temp1;
					temps->temp1 = -2147483647;
					break;
				case DISP_MIN:
					temps->temp = temps->temp1;
					temps->temp1 = 2147483647;
					break;
				default:
					break;
				}
				measure->value = temps->temp; //((temps->temp - 32768.f) * measure->sensitivity) - measure->offset;
				temps->temp = 0;
				param->isUpdated = true;
				temps->index = temps->maxIndex;
			}

		}
	}
#elif 0
	for (int i = 0; i < 16; i++)
	{
		disp_param_t* param = &chDisplayParams[i];
		if (param->srcType == PARAM_SRC_MEASUREMENT)
		{
			param_measures_t* measure = &param->src.measure;
			param_measures_temp_vars_t* temps = &(measure->temps);
			float data = dataPtr[measure->channelIndex];//((adcData[measure->channelIndex] - 32768.f) * measure->sensitivity) - measure->offset;
			temps->temp += data * data;
			if (--temps->index <= 0)
			{
				temps->temp = sqrtf(temps->temp / temps->maxIndex);
				measure->value = temps->temp; //((temps->temp - 32768.f) * measure->sensitivity) - measure->offset;
				temps->temp = 0;
				param->isUpdated = true;
				temps->index = temps->maxIndex;
			}

		}
	}

#elif 0
	static int ret = 4;
	if (--ret > 0)
		return;
	int i = 16;
	while (i--)
	{
		disp_param_t* param = &chDisplayParams[i];
		param_measures_t* measure = &param->src.measure;
		param_measures_temp_vars_t* temps = &(measure->temps);
		temps->temp  += dataPtr[i] * dataPtr[i];
		if (--temps->index <= 0)
		{
			temps->temp = sqrtf(temps->temp / temps->maxIndex);
			measure->value = temps->temp;
			temps->temp = 0;
			param->isUpdated = true;
			temps->index = temps->maxIndex;
		}
	}
	ret = 4;
#else
	static int ret = 4;
	if (--ret > 0)
		return;
	int i = 16;
	while (i--)
	{
		chDispMeasures.tempStats[i].rms += (dataPtr[i] * dataPtr[i]);
		chDispMeasures.tempStats[i].avg += dataPtr[i];
		if (chDispMeasures.tempStats[i].max < dataPtr[i])
			chDispMeasures.tempStats[i].max = dataPtr[i];
		if (chDispMeasures.tempStats[i].min > dataPtr[i])
			chDispMeasures.tempStats[i].min = dataPtr[i];
		if (--chDispMeasures.tempStats[i].index <= 0)
		{
			// get new values
			chDispMeasures.disp[i].stats.rms = sqrtf(chDispMeasures.tempStats[i].rms / chDispMeasures.tempStats[i].maxIndex);
			chDispMeasures.disp[i].stats.avg = chDispMeasures.tempStats[i].avg / chDispMeasures.tempStats[i].maxIndex;
			chDispMeasures.disp[i].stats.max = chDispMeasures.tempStats[i].max;
			chDispMeasures.disp[i].stats.min = chDispMeasures.tempStats[i].min;
			chDispMeasures.disp[i].stats.pkTopk = chDispMeasures.tempStats[i].max - chDispMeasures.tempStats[i].min;

			// reset temp stats
			chDispMeasures.tempStats[i].rms = 0;
			chDispMeasures.tempStats[i].avg = 0;
			chDispMeasures.tempStats[i].max = -4294967296;
			chDispMeasures.tempStats[i].min = 4294967296;
			chDispMeasures.tempStats[i].index = chDispMeasures.tempStats[i].maxIndex;
			chDispMeasures.isUpdated = true;
		}
	}
	ret = 4;
#endif
}

#endif
/* EOF */
