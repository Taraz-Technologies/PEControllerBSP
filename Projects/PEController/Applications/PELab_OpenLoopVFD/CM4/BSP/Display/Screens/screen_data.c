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
disp_param_t chDisplayParams[16];
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
void BSP_Display_UpdateMeasurements(uint16_t* adcData)
{
	for (int i = 0; i < 16; i++)
	{
		disp_param_t* param = &chDisplayParams[i];
		if (param->srcType == PARAM_SRC_MEASUREMENT)
		{
			param_measures_t* measure = &param->src.measure;
			param_measures_temp_vars_t* temps = &(measure->temps);
			switch (measure->type)
			{
			case DISP_RMS:
				temps->temp += adcData[measure->channelIndex] * adcData[measure->channelIndex];
				break;
			case DISP_AVG:
				temps->temp += adcData[measure->channelIndex];
				break;
			case DISP_MAX:
				if (temps->temp1 < adcData[measure->channelIndex])
					temps->temp1 = adcData[measure->channelIndex];
				break;
			case DISP_MIN:
				if (temps->temp1 > adcData[measure->channelIndex])
					temps->temp1 = adcData[measure->channelIndex];
				break;
			case DISP_PkToPk:
				if (temps->temp1 < adcData[measure->channelIndex])
					temps->temp1 = adcData[measure->channelIndex];
				if (temps->temp2 > adcData[measure->channelIndex])
					temps->temp2 = adcData[measure->channelIndex];
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
					temps->temp = sqrtf(temps->temp);
					break;
				case DISP_AVG:
					temps->temp = temps->temp / temps->maxIndex;
					break;
				case DISP_PkToPk:
					temps->temp = temps->temp1 - temps->temp2;
					temps->temp1 = 0;
					temps->temp2 = 0;
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
				measure->value = ((temps->temp - 32768.f) * measure->sensitivity) - measure->offset;
				temps->temp = 0;
				param->isUpdated = true;
				temps->index = temps->maxIndex;
			}

		}
	}
}

#endif
/* EOF */
