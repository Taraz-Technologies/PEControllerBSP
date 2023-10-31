/**
 ********************************************************************************
 * @file    	app_screen_data.c
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
#include "app_screen_data.h"
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

/********************************************************************************
 * Global Variables
 *******************************************************************************/
/**
 * @brief This information will be displayed in the application information screen
 * @note The information is application dependent and can be edited in @ref screen_appinfo_data.c.
 * The file is located in \"CM4/BSP/Display\" folder in the relevant project
 */
appinfo_display_t appInfoDisplay =
{
		.appInfo = "This example controls one/multiple three phase inverter modules using the PWM signals of the PEController."
				" The three phase signal starts from the INITIAL_FREQ and gradually increases dependent upon the value of ACCELERATION to reach the OUTPUT_FREQ. "
				"A constant V/f is maintained through out this process using the ratio NOMINAL_FREQ / NOMINAL_MODULATION_INDEX. "
				"These configurations are available in the file open_loop_vf_config.h",

		.connectionInfo = NULL,

		.documentationLink = "www.taraztechnologies.com/Downloads/Software/PEControllerBSP/index.html",

		.img = &bsp_docs_QR_info,

		.appTitle = "Open-Loop V/f Control"
};
/**
 * @brief Assigns the image to be displayed on the splash screen
 */
image_info_t* splashImg = &taraz_logo_info;
static data_param_info_t* firstWindowParams[] = { &p2pCommsParams[P2P_PARAM_f_NOM_INV1], &p2pCommsParams[P2P_PARAM_m_NOM_INV1], &p2pCommsParams[P2P_PARAM_a_INV1], };
static data_param_info_t* secondWindowParams[] = { &p2pCommsParams[P2P_PARAM_f_NOM_INV2], &p2pCommsParams[P2P_PARAM_m_NOM_INV2], &p2pCommsParams[P2P_PARAM_a_INV2], };
data_param_group_t settingWindows[2] =
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
