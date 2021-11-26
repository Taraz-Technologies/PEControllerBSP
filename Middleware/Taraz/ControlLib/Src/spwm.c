/**
 ********************************************************************************
 * @file    	SPWM.c
 * @author 		Waqas Ehsan Butt
 * @date    	Nov 25, 2021
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
#include "SPWM.h"
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

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Adjust the duty cycles for Sinusoidal PWM
 * @param theta Current angle of Phase A in radians
 * @param modulationIndex Modulation index for the PWM
 * @param *duties Resultant SPWM duty cycles.
 */
void GenerateSPWM(float theta, float modulationIndex, float* duties)
{
	// get the equivalent duty cycle
	float resThetas[3] = { theta , theta + (TWO_PI/3), theta - (TWO_PI/3)};
	for (int i = 0; i < 3; i++)
	{
		theta = Transform_Theta_0to2pi(resThetas[i]);
		duties[i] = ((sinf(theta) * modulationIndex) * 0.5f) + 0.5f;
	}
}


/* EOF */
