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
#include "screen_styles.h"
#include "screen_data.h"
#include "message_screen.h"
#include "config_screen.h"
#include "main_screen.h"
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
static char* chNames[] =
{
		"CH1", "CH2", "CH3", "CH4",
		"CH5", "CH6", "CH7", "CH8",
		"CH9", "CH10", "CH11", "CH12",
		"CH13", "CH14", "CH15", "CH16",
};
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
void ScreenManager_Init(void)
{
	for (int i = 0; i < 16; i++)
	{
		chDisplayParams[i].srcName = chNames[i];
		chDisplayParams[i].srcType = PARAM_SRC_MEASUREMENT;
		chDisplayParams[i].src.measure.type = DISP_RMS;
	}
	MainScreen_Init();
	MainScreen_Load();
}

void ScreenManager_Poll(void)
{
	MainScreen_Refresh();
}



#endif
/* EOF */
