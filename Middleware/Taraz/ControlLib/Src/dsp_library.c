/**
 ********************************************************************************
 * @file    	dsp_library.c
 * @author 		Waqas Ehsan Butt
 * @date    	Nov 26, 2021
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
#include "dsp_library.h"
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
	pi->Integral += (pi->Ki * err * pi->dt) ;
#if PI_COMPENSATOR_LIMIT_CAPABLE
	if (pi->has_lmt)
	{
		if(pi->Integral > pi->max)
			pi->Integral = pi->max;
		if(pi->Integral < pi->min)
			pi->Integral = pi->min;
	}
#endif
	float result = pi->Kp * err + pi->Integral;
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

/**
 * @brief Computes the moving average
 * @param *filt Pointer to the filter
 * @param val Current value
 * @return float Resultant value of the moving average filter
 */
float MovingAverage_Compute(mov_avg_t* filt, float val)
{
	filt->dataPtr[filt->index] = val;					/* add new avgFilt */
	filt->avg = 0;
	if(filt->stable == false)									/* data is considered stable if the whole array is finished at least once */
	{
		filt->index++;

		for (int i = 0; i < filt->index; i++)		/* average will only contain data till current index */
			filt->avg += filt->dataPtr[i];
		filt->avg /= filt->index;

		if(filt->index >= filt->count)					/* if data completed mark the data stable */
		{
			filt->stable = true;
			filt->index = 0;
		}
	}
	else																			/* data is stable compute average over the whole data */
	{
		for (int i = 0; i < filt->count; i++)
			filt->avg += filt->dataPtr[i];
		filt->avg /= filt->count;
		if(++filt->index >= filt->count)
			filt->index = 0;
	}
	return filt->avg;
}

/**
 * @brief Resets the moving average filter
 * @param *filt Pointer to the filter
 */
void MovingAverage_Reset(mov_avg_t* filt)
{
	filt->avg = 0;
	filt->index = 0;
	for(int i = 0; i < filt->count; i++)
		filt->dataPtr[i] = 0;
	filt->stable = false;
}


#pragma GCC pop_options
/* EOF */
