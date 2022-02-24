/**
 ********************************************************************************
 * @file    	SVPWM.c
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
#include "svpwm.h"
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
 * @brief Get duty cycles of each leg using space vector PWM from LIB_3COOR_ALBE0_t
 * @param *alBe0 Alpha Beta Coordinates
 * @param *duties Pointer to the array where duty cycles need to be updated
 */
void ComputeDuty_SVPWM_FromAlBe0(LIB_3COOR_ALBE0_t *alBe0, float* duties)
{
	// hypotd (Get the value of m)
	float a = fabsf(alBe0->alpha);
	float m = fabsf(alBe0->beta);

	if (a < m)
	{
		a  = a / m;
		m = sqrtf(a * a + 1) * m;
	}
	else if (a > m)
	{
		m = m / a;
		m = sqrtf(m * m + 1) * a;
	}

	if (m < 0)
		m = 0;
	else if (m > 1)
		m = 1;

	// get the angle 0-2pi
	float wt = 0;
	if (alBe0->alpha == 0)
	{
		if (alBe0->beta > 0)
			wt = PI/2;
		else if (alBe0->beta < 0)
			wt = -PI/2;
		else
			wt = 0;
	}
	else
		wt = atan2f(alBe0->beta, alBe0->alpha);
	wt = Transform_Theta_0to2pi(wt);

	// get sector (1 - 6)
	int n = (int)(wt * 3/PI) + 1;

	// get wtn for the sector 0-pi/6
	float wtn = wt - (n - 1) * PI/3;

	// get SVPWM timing
	float p1 = m * cosf(wtn + PI/6);
	float p2 = m * sinf(wtn);
	float p0 = (1 - p2 - p1)/2;

	// set duty cycles
	duties[0] = p0;
	duties[1] = p0;
	duties[2] = p0;

	float tAll = p0 + p1 + p2;
	float t1 = p0 + p1;
	float t2 = p0 + p2;

	if (n == 1)
	{
		duties[0] = tAll;
		duties[1] = t2;
	}
	else if (n == 2)
	{
		duties[0] = t1;
		duties[1] = tAll;
	}
	else if (n == 3)
	{
		duties[1] = tAll;
		duties[2] = t2;
	}
	else if (n == 4)
	{
		duties[1] = t1;
		duties[2] = tAll;
	}
	else if (n == 5)
	{
		duties[2] = tAll;
		duties[0] = t2;
	}
	else if (n == 6)
	{
		duties[2] = t1;
		duties[0] = tAll;
	}
	else
	{
		duties[0] = 0;
	}
}

/* EOF */
