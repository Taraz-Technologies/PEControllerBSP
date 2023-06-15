/**
 ********************************************************************************
 * @file    	error_config.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 30, 2023
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
#include "error_config.h"
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
device_err_info_t errInfo[ERR_COUNT] =
{
		{.caption = "No Error", .desc = "No pending errors detected." },
		{.caption = "Invalid Format", .desc = "The entered value is not correctly formatted. "
				"Kindly, provide the value in correct format." },
		{.caption = "Illegal Value", .desc = "The entered value is illegal. "
				"Kindly, provide a legal value for this parameter."  },
		{.caption = "Out of Range", .desc = "The entered value of the parameter is outside the allowed range. "
				"Kindly, provide a value with-in allowed parameter range." },
		{.caption = "Functionality Unavailable", .desc = "The desired functionality is currently unavailable. "
				"Kindly, contact support if error persists." },
		{.caption = "Relay Turned Off", .desc = "Unable to activate the inverter because the relays are currently off. "
						"Relays automatically turn on when the DC link voltage is stabilized to a safe limit." },
		{.caption = "Boost Disabled", .desc = "Unable to activate the inverter because the Boost converter is off." },
		{.caption = "PLL not Locked", .desc = "Unable to activate the inverter because the PLL is not locked accurately. " },
		{.caption = "Inverter Active", .desc = "Unable to turn off the Boost converter while the inverter is active. " },
};
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/



/* EOF */
