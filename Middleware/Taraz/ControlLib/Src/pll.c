/**
 ********************************************************************************
 * @file    	pll.c
 * @author 		Waqas Ehsan Butt
 * @date    	October 7, 2021
 *
 * @brief   
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
#include "pll.h"
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
 * @brief Checks if the PLL is currently locked
 * @param *pll Pointer to the PLL structure
 * @return pll_states_t state of the PLL in this cycle
 */
static pll_states_t IsPLLSynched(pll_lock_t* pll)
{
	pll_info_t* info = &pll->info;
	pll->prevStatus = pll->status;

	// replace temporary max of Q is exceeds
	float absQ = fabsf(pll->coords->dq0.q);
	if(absQ > info->tempQMax)
		info->tempQMax = absQ;

	// replace temporary min of D if exceeds
	if (pll->coords->dq0.d < info->tempDMin)
		info->tempDMin = pll->coords->dq0.d;


	// replace temporary max of D if exceeds
	if (pll->coords->dq0.d > info->tempDMax)
		info->tempDMax = pll->coords->dq0.d;

	// increase index
	info->index++;

	if (pll->status == PLL_LOCKED)
	{
		// if grid is lost disable pll lock
		if (info->tempQMax > (pll->qLockMax * 2.f) || info->tempDMin < (pll->dLockMin) || info->tempDMax > (pll->dLockMax))
		{
			pll->status = PLL_INVALID;
			info->index = 0;
			info->tempQMax = 0;
			info->tempDMin = 200000;
			info->tempDMax = -200000;
		}
	}
	// check PLL status
	else if(info->index > pll->cycleCount && pll->status != PLL_LOCKED)
	{
#if MONITOR_PLL
		info->qMax = info->tempQMax;
		info->dMin = info->tempDMin;
#endif
		if (info->tempQMax < pll->qLockMax && info->tempDMin > pll->dLockMin && info->tempDMax < pll->dLockMax)
			pll->status = pll->status == PLL_LOCKED ? PLL_LOCKED : PLL_PENDING;
		else
			pll->status = PLL_INVALID;

		info->index = 0;
		info->tempQMax = 0;
		info->tempDMin = 200000;
		info->tempDMax = -200000;
	}

	// lock to the phase once the phase is very low
	if(pll->status == PLL_PENDING)
	{
		if (fabsf(pll->coords->abc.a) < (pll->dFilt.avg) / 40)
			pll->status = PLL_LOCKED;
	}

	return pll->status;
}

/**
 * @brief Lock the grid voltages using PLL
 *
 * @param pll Pointer to the data structure
 * @return pll_states_t PLL_LOCKED if grid phase successfully locked
 */
pll_states_t Pll_LockGrid(pll_lock_t* pll)
{
	// should point to valid pll structure
	if(pll == NULL)
		Error_Handler();

	// should point to valid coordinates
	LIB_COOR_ALL_t* coords = pll->coords;
	if (coords == NULL)
		Error_Handler();

	// Assign data locations for filters if not already assigned
	if (pll->dFilt.dataPtr == NULL)
	{
		pll->dFilt.dataPtr = (float*)malloc(pll->dFilt.count);
		pll->qFilt.dataPtr = (float*)malloc(pll->qFilt.count);

		if (pll->dFilt.dataPtr == NULL || pll->qFilt.dataPtr == NULL)
			Error_Handler();
	}

	// Fault if compensator time interval not set
	if (pll->compensator.dt <= 0)
		Error_Handler();

	// evaluate DQ0 coordinates from the ABC coordinates
	Transform_abc_dq0(&coords->abc, &coords->dq0, &coords->trigno, SRC_ABC, PARK_SINE);

	// apply low pass filters
	pll->coords->dq0.q = MovingAverage_Compute(&pll->qFilt, pll->coords->dq0.q);
	pll->coords->dq0.d = MovingAverage_Compute(&pll->dFilt, pll->coords->dq0.d);

	float thetaShift = PI_Compensate(&pll->compensator, -coords->dq0.q);
	coords->trigno.wt = ShiftTheta_0to2pi(coords->trigno.wt, -thetaShift);
	Transform_wt_sincos(&coords->trigno);

	return IsPLLSynched(pll);
}

/* EOF */
