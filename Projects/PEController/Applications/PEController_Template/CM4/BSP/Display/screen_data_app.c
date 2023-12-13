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
static data_param_info_t* firstWindowParams[] = { &p2pCommsParams[P2P_PARAM_U8_SAMPLE], &p2pCommsParams[P2P_PARAM_S8_SAMPLE], &p2pCommsParams[P2P_PARAM_U16_SAMPLE], &p2pCommsParams[P2P_PARAM_U32_SAMPLE], };
static data_param_info_t* secondWindowParams[] = { &p2pCommsParams[P2P_PARAM_S16_SAMPLE], &p2pCommsParams[P2P_PARAM_S32_SAMPLE], };
/********************************************************************************
 * Global Variables
 *******************************************************************************/
/**
 * @brief This information will be displayed in the application information screen
 */
appinfo_display_t appInfoDisplay =
{
		.appInfo = "This application serves as a template to demonstrate the usage of PEControllerBSP. "
				"It can be used as a starting point to enhance functionality by incorporating various control and communication schemes.",

		.connectionInfo = NULL,

		.documentationLink = "www.taraztechnologies.com/Downloads/Software/PEControllerBSP/index.html",

		.img = &bsp_docs_QR_info,

		.appTitle = "Template Project"
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
				.title = "Settings Page 1",
				.paramPointers = firstWindowParams,
				.paramCount = 4
		},
		{
				.title = "Settings Page 2",
				.paramPointers = secondWindowParams,
				.paramCount = 2
		}
};
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/



/* EOF */
