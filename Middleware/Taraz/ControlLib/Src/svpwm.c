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
#pragma GCC push_options
#pragma GCC optimize ("-Ofast")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "svpwm.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define ONE_BY_SQRT3				(1.0f/sqrtf(3))
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
 * @brief Limits the duty cycle value from 0-1
 */
static inline void Limit_Duty_0_1(float* duty)
{
	if (*duty > 1.f)
		*duty = 1.f;
	else if (*duty < 0)
		*duty = 0;
}

/**
 * @brief Get duty cycles of each leg using space vector PWM from LIB_3COOR_ALBE0_t
 * @param *alBe0 Alpha Beta Zero Coordinates
 * @param *duties Pointer to the array where duty cycles need to be updated. Duty Cycle range is between (0-1)
 */
void SVPWM_GenerateDutyCycles(LIB_3COOR_ALBE0_t *alBe0, float* duties)
{
	float shift = ONE_BY_SQRT3 * alBe0->alpha;
	float a = 2 * shift;
	float b = alBe0->beta - shift;
	float c = -alBe0->beta - shift;

	float max = a;
	float min = a;

	// min - max function
	if (a > b)
	{
		if (c > a)
			max = c;
		min = b > c ? c : b;
	}
	else
	{
		if (c < a)
			min = c;
		max = b > c ? b : c;
	}

	float pk = (min + max) / 2;
	// subtract 1 from pk
	// this compensates 0.5 offset in output duty cycle
	float sub = pk - 1;

	// Evaluate final duty cycles with limits
	duties[0] = ((a - sub) / 2);
	Limit_Duty_0_1(duties);

	duties[1] = ((b - sub) / 2);
	Limit_Duty_0_1(duties + 1);

	duties[2] = ((c - sub) / 2);
	Limit_Duty_0_1(duties + 2);
}
#pragma GCC pop_options
/* EOF */
