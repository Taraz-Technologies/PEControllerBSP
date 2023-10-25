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
		.appInfo =
				"This example controls a three phase inverter module as well as a boost converter for PELab (PLB-6PH). "
				"The boost converter starts regulation to provide a stabilized DC-Link voltage. "
				"Once stabilized, the relay connects the grid. "
				"The control implements a PLL algorithm to detect the phase of the grid voltages. "
				"After estimating the exact phase angle the inverter is enabled and starts injecting power in the grid. "
				"Set measurement sensitivity before connecting to the input or output. "
				"Make sure to provide the DC Link before connecting to the grid to avoid failure due to in-rush current.",

		.connectionInfo = NULL,

		.documentationLink = "www.taraztechnologies.com/help/pecontroller-three-phase-grid-tie-inverter/",

		.img = NULL,

		.appNo = "AN02",

		.appTitle = "Three-Phase Grid-Tie Inverter"
};
/**
 * @brief Assigns the image to be displayed on the splash screen
 */
image_info_t* splashImg = &taraz_logo_info;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/



/* EOF */
