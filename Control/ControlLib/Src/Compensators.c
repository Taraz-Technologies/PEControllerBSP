/**
 ********************************************************************************
 * @file    	Compensators.c
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
#include "Compensators.h"
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
 * @brief Evaluates the result for the PI compensation
 * @param *pi Pointer to the PI compensator structure
 * @param err Current value of error
 * @return float Result of the PI compensation of current cycle
 */
float PI_Compensate(pi_compensator_t* pi, float err)
{
#if MONITOR_PI
	pi->err = err;
#endif
	pi->Integral += (err * pi->dt) ;
	float integralTerm = (pi->Ki * pi->Integral);
#if PI_COMPENSATOR_LIMIT_CAPABLE
	if (pi->has_lmt)
	{
		if(integralTerm > pi->max)
			pi->Integral = pi->max / pi->Ki;
		if(integralTerm < pi->min)
			pi->Integral = pi->min / pi->Ki;
	}
#endif
	float result = pi->Kp * err + (pi->Ki * pi->Integral);
#if PI_COMPENSATOR_LIMIT_CAPABLE
	if (pi->has_lmt)
	{
		if(result > pi->max)
			result = pi->max;
		if(result < pi->min)
			result = pi->min;
	}
#endif
#if MONITOR_PI
	pi->result = result;
#endif
	return result;
}


/* EOF */
