/**
 ********************************************************************************
 * @file    	screen_manager.c
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
#include "shared_memory.h"
#include "screen_styles.h"
#include "screen_data.h"
#include "message_screen.h"
#include "config_screen.h"
#include "main_screen.h"
#include "max11046_drivers.h"
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
static const char* chNames[] =
{
		"CH1", "CH2", "CH3", "CH4",
		"CH5", "CH6", "CH7", "CH8",
		"CH9", "CH10", "CH11", "CH12",
		"CH13", "CH14", "CH15", "CH16",
};

static const char* chUnits[] =
{
		"A", "A", "A", "A",
		"A", "A", "A", "A",
		"V", "V", "V", "V",
		"V", "V", "V", "V",
};

static measure_type_t chMeasurementType[] =
{
		MEASURE_RMS, MEASURE_AVG, MEASURE_MAX, MEASURE_MIN,
		MEASURE_PkToPk, MEASURE_RMS, MEASURE_RMS, MEASURE_RMS,
		MEASURE_RMS, MEASURE_RMS, MEASURE_RMS, MEASURE_RMS,
		MEASURE_AVG, MEASURE_AVG, MEASURE_AVG, MEASURE_AVG,
};
/********************************************************************************
 * Global Variables
 *******************************************************************************/
extern volatile int screenID;
static int screenID0 = 0;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
void ScreenManager_Init(void)
{
	BSP_Styles_Init();

	for (int i = 0; i < 16; i++)
	{
		chDispMeasures.disp[i].chName = chNames[i];
		chDispMeasures.disp[i].chUnit = chUnits[i];
		chDispMeasures.disp[i].type = chMeasurementType[i];
		chDispMeasures.disp[i].channelIndex = i;
#if ADC_CORE == ADC_CM4
		chDispMeasures.disp[i].stats = &sharedData->m4Tom7.processedAdcData.info->stats[i];
#else
		chDispMeasures.disp[i].stats = &sharedData->m7Tom4.processedAdcData.info->stats[i];
#endif
	}
	// --TODO-- chDispMeasures.offsets = ((float*)&adcOffsets);
	// --TODO-- chDispMeasures.sensitivity = ((float*)&adcMultipiers);

	for (int i = 0; i < 5; i++)
	{
		dispVars[i].type = VAR_FLOAT;
		dispVars[i].name = "";
		dispVars[i].unit = "";
	}

	MainScreen_Init();
	ConfigScreen_Init();
	MessageScreen_Init();


	MainScreen_Load();
	screenID0 = 0;

	//HAL_Delay(40000);
	//MessageScreen_Init();
	//MessageScreen_Load("Error: 0x2015", "Unable to set desired value. Input value is out of limit. Kindly, enter a new value", true, true);
	//ConfigScreen_Init();
	//ConfigScreen_Load();
}

void ScreenManager_Poll(void)
{
	int screenID1 = screenID;
	if (screenID1 == screenID0)
	{
		if(screenID0 == 0)
			MainScreen_Refresh();
	}
	else
	{
		/*
		if(screenID0 == 0)
			MainScreen_Unload();
		else if(screenID0 == 1)
			ConfigScreen_Unload();
		 */
		screenID0 = screenID1;
		if(screenID0 == 0)
			MainScreen_Load();
		else if(screenID0 == 1)
			ConfigScreen_Load();
	}
}


#endif
/* EOF */
