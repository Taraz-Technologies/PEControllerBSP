/**
 ********************************************************************************
 * @file    	screen_data.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 12, 2023
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
#include "pecontroller_display.h"
#include "screen_data.h"
#include "state_storage_lib.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define DEFAULT_MEASURE_TYPE				(MEASURE_RMS)
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
const char* measureTxts[MEASURE_COUNT] = {"RMS", "Avg", "Max", "Min", "Pk-Pk"};
const char* unitTxts[UNIT_COUNT] = {"V", "A", "W", "Hz"};
disp_measure_t dispMeasures = { 0 };
ltdc_layer_info_t defaultLayer =
{
		.data = (uint8_t*)frame_buff,
		.posX = 0,
		.posY = 0,
		.width = DISPLAY_WIDTH_RAM,
		.height = DISPLAY_HEIGHT_RAM,
		.PixelFormat = 0,
		.xAlign = ALIGN_LEFT_X,
		.yAlign = ALIGN_UP_Y,
		.PixelFormat = RAM_PIXEL_FORMAT
};
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
static void InitStatesFromStorage(uint32_t* data, bool isDataValid)
{
	uint8_t* localData = (uint8_t*)data;
	if (isDataValid)
	{
		for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
		{
			if (localData[i] < MEASURE_COUNT)
				dispMeasures.chMeasures[i].type = (measure_type_t)localData[i];
			else
			{
				dispMeasures.chMeasures[i].type = DEFAULT_MEASURE_TYPE;
				localData[i] = dispMeasures.chMeasures[i].type;
			}
		}
	}
	else
	{
		for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
		{
			dispMeasures.chMeasures[i].type = DEFAULT_MEASURE_TYPE;
			localData[i] = dispMeasures.chMeasures[i].type;
		}
	}
}
static uint32_t RefreshStates(uint32_t* data, uint32_t* indexPtr)
{
	uint32_t len = 0;
	uint8_t* localData = (uint8_t*)data;
	for (int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
	{
		if (localData[i] != dispMeasures.chMeasures[i].type)
		{
			localData[i] = dispMeasures.chMeasures[i].type;
			len = (TOTAL_MEASUREMENT_COUNT / sizeof(uint32_t)) * sizeof(uint8_t);
		}
	}
	*indexPtr = 0;
	return len;
}

void ScreenData_ConfigStorage(state_storage_client_t* _config)
{
	_config->InitStatesFromStorage = InitStatesFromStorage;
	_config->RefreshStates = RefreshStates;
	_config->dataWordLen = (TOTAL_MEASUREMENT_COUNT / sizeof(uint32_t)) * sizeof(uint8_t);
}


/* EOF */
