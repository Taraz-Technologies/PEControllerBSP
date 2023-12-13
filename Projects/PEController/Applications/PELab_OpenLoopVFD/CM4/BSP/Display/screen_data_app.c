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
static data_param_info_t* firstWindowParams[] = { &p2pCommsParams[P2P_PARAM_f_NOM_INV1], &p2pCommsParams[P2P_PARAM_m_NOM_INV1], &p2pCommsParams[P2P_PARAM_a_INV1], };
static data_param_info_t* secondWindowParams[] = { &p2pCommsParams[P2P_PARAM_f_NOM_INV2], &p2pCommsParams[P2P_PARAM_m_NOM_INV2], &p2pCommsParams[P2P_PARAM_a_INV2], };
/********************************************************************************
 * Global Variables
 *******************************************************************************/
/**
 * @brief This information will be displayed in the application information screen
 */
appinfo_display_t appInfoDisplay =
{
		.appInfo = "This is a demonstration of Open-loop V/f Control of a Three-Phase Inverter."
				"\n 1. Set the frequency of operation for the inverter."
				"\n 2. Set the direction of motor."
				"\n 3. Use the settings button to set additional parameters."
				"\n 4. Control the Inverters using Enable buttons."
				"\n 5. Update the values of frequency and direction to view the effect of acceleration.",

		.connectionInfo = NULL,

		.documentationLink = "www.taraztechnologies.com/Downloads/Software/PEControllerBSP/index.html",

		.img = &bsp_docs_QR_info,

		.appTitle = "Open-Loop V/f Control"
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
				.title = "Inverter 1 Configuration",
				.paramPointers = firstWindowParams,
				.paramCount = 3
		},
		{
				.title = "Inverter 2 Configuration",
				.paramPointers = secondWindowParams,
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
