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
		.appInfo = "This application serves as a template to demonstrate the usage of PEControllerBSP. "
				"It can be used as a starting point to enhance functionality by incorporating various control and communication schemes.",

		.connectionInfo = NULL,

		.documentationLink = "www.taraztechnologies.com/Downloads/Software/PEControllerBSP/index.html",

		.img = NULL,
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
