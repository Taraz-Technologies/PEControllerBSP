/**
 ********************************************************************************
 * @file    	screen_data_app.c
 * @author 		Waqas Ehsan Butt
 * @date    	Jun 6, 2023
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
#include "screen_data_app.h"
#include "p2p_comms.h"
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
static data_param_info_t* settingsParam[] = { &p2pCommsParams[P2P_PARAM_f_GRID], &p2pCommsParams[P2P_PARAM_V_GRID], &p2pCommsParams[P2P_PARAM_L_OUT_mH], };
/********************************************************************************
 * Global Variables
 *******************************************************************************/
/**
 * @brief This information will be displayed in the application information screen
 */
appinfo_display_t appInfoDisplay =
{
		.appInfo =
				"1. Configure the measurements."
				"\n2. Charge the DC Link before connecting to the grid to avoid failure due to in-rush current."
				"\n3. Enable the boost converter to provide stable DC-Link."
				"\n4. Once stabilized, the relay connects the grid. "
				"\n5. The phase of the grid is locked by PLL. "
				"\n6. After the sequence is complete the inverter can be turned on.",

		.connectionInfo = NULL,

		.documentationLink = "www.taraztechnologies.com/Downloads/Software/PEControllerBSP/index.html",

		.img = &bsp_docs_QR_info,

		.appTitle = "Three-Phase Grid-Tie Inverter"
};
/**
 * @brief Assigns the image to be displayed on the splash screen
 */
image_info_t* splashImg = &taraz_logo_info;
/**
 * @brief This represents list of groups of settings to be configured in the application
 */
data_param_group_t settingWindows[SETTINGS_WINDOW_COUNT] =
{
		{
				.title = "Grid Tie Configuration",
				.paramPointers = settingsParam,
				.paramCount = 3
		}
};
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/



/* EOF */
