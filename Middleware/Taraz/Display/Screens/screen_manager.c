/**
 ********************************************************************************
 * @file    	screen_manager.c
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
#include "screen_base.h"
#include "screen_manager.h"
#include "stdlib.h"
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
static screens_t screens[SCREEN_COUNT] = { 0 };
static screen_type_t screenIdx = SCREEN_NONE;
static DisplayLayer dispLayer = NULL;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
static const char* chNames[] =
{
		"CH1", "CH2", "CH3", "CH4",
		"CH5", "CH6", "CH7", "CH8",
		"CH9", "CH10", "CH11", "CH12",
		"CH13", "CH14", "CH15", "CH16",
};
static int changeMode = 0;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
/**
 * @brief Initialize screen
 * @param *screen Pointer to fill the screen information
 */
extern void SplashScreen_Init(screens_t* screen);
/**
 * @brief Initialize screen
 * @param *screen Pointer to fill the screen information
 */
extern void MainScreen_Init(screens_t* screen);
/**
 * @brief Initialize screen
 * @param *screen Pointer to fill the screen information
 */
extern void ConfigScreen_Init(screens_t* screen);
/**
 * @brief Initialize screen
 * @param *screen Pointer to fill the screen information
 */
extern void IntellisensScreen_Init(screens_t* screen);
/**
 * @brief Initialize screen
 * @param *screen Pointer to fill the screen information
 */
extern void AppInfoScreen_Init(screens_t* screen);
/********************************************************************************
 * Code
 *******************************************************************************/
static void Screens_Init(void)
{
	SplashScreen_Init(&screens[SCREEN_SPLASH]);
	MainScreen_Init(&screens[SCREEN_MAIN]);
	ConfigScreen_Init(&screens[SCREEN_CONF]);
	IntellisensScreen_Init(&screens[SCREEN_intelliSENS]);
	AppInfoScreen_Init(&screens[SCREEN_APPINFO]);
}

static void Measurements_Init(adc_info_t* _adcInfo)
{
	for(int i = 0; i < TOTAL_MEASUREMENT_COUNT; i++)
		dispMeasures.chMeasures[i].chIndex = i;
	dispMeasures.adcInfo = _adcInfo;
	dispMeasures.chNames = chNames;
}

/**
 * @brief Initialize the screen manager
 * @note This function is automatically called by the BSP. No need to call this function externally
 * @param _dispLayer This function will be called to change the layer information
 * @param _adcInfo This pointer points to the ADC information structure
 */
void ScreenManager_Init(DisplayLayer _dispLayer ,adc_info_t* _adcInfo)
{
	dispLayer = _dispLayer;
	Measurements_Init(_adcInfo);

	BSP_Screen_Styles_Init();
	Screens_Init();

	screenIdx = SCREEN_SPLASH;
	screens[screenIdx].Load();
	dispLayer(screens[screenIdx].lvglLayer, LVGL_LAYER);
	dispLayer(screens[screenIdx].directLayer, DIRECT_LAYER);
}

/**
 * @brief This function refreshes and manages the screens for display.
 */
void ScreenManager_Refresh(void)
{
	if (changeMode == 0)
	{
		static screen_type_t idxPrevious = SCREEN_MAIN;
		screen_type_t idxNext = screens[screenIdx].Refresh();
		if (idxNext == SCREEN_NONE)
			return;

		if (idxNext == SCREEN_PREVIOUS)
			idxNext = idxPrevious;

		idxPrevious = screenIdx;
		writeAtScreenEnd = false;
		changeMode = 1;

		if(screens[screenIdx].Unload != NULL)
			screens[screenIdx].Unload();

		screenIdx = idxNext;

		if(screens[screenIdx].Load != NULL)
			screens[screenIdx].Load();

		dispLayer(NULL, LVGL_LAYER);
		dispLayer(NULL, DIRECT_LAYER);
	}
	else if (writeAtScreenEnd)
	{
		dispLayer(screens[screenIdx].lvglLayer, LVGL_LAYER);
		dispLayer(screens[screenIdx].directLayer, DIRECT_LAYER);
		changeMode = 0;
	}
}

/* EOF */
