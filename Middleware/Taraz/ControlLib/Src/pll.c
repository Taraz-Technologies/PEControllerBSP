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
#pragma GCC push_options
#pragma GCC optimize ("-Ofast")
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
 * @brief Initialize the PLL structure.
 * @param *pll Structure to be initialized.
 */
void PLL_Init(pll_lock_t* pll)
{
	// should point to valid pll structure
	if(pll == NULL)
		Error_Handler();

	// should point to valid coordinates
	LIB_COOR_ALL_t* coords = pll->coords;
	if (coords == NULL)
		Error_Handler();

	// Fault if compensator time interval not set
	if (pll->compensator.dt <= 0)
		Error_Handler();

	// inital value of the integral
	pll->compensator.Integral = TWO_PI * pll->expectedGridFreq * pll->compensator.dt;
}

/**
 * @brief Checks if the PLL is currently locked.
 * @param *pll Pointer to the PLL structure.
 * @return pll_states_t state of the PLL in this cycle.
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
	if(info->index > pll->cycleCount)
	{
#if MONITOR_PLL
		info->qMax = info->tempQMax;
		info->dMin = info->tempDMin;
#endif
		if (pll->status != PLL_LOCKED)
		{
			if (info->tempQMax < pll->qLockMax && info->tempDMin > pll->dLockMin && info->tempDMax < pll->dLockMax)
				pll->status = pll->status == PLL_LOCKED ? PLL_LOCKED : PLL_PENDING;
			else
				pll->status = PLL_INVALID;
		}

		info->index = 0;
		info->tempQMax = 0;
		info->tempDMin = 200000;
		info->tempDMax = -200000;
	}

	// lock to the phase once the phase is very low
	if(pll->status == PLL_PENDING)
	{
		if (fabsf(pll->coords->abc.a) < (pll->coords->dq0.d) / 40)
			pll->status = PLL_LOCKED;
	}

	return pll->status;
}

/**
 * @brief Lock the grid voltages using PLL
 * @param pll Pointer to the data structure
 * @return pll_states_t PLL_LOCKED if grid phase successfully locked
 */
pll_states_t Pll_LockGrid(pll_lock_t* pll)
{
	LIB_COOR_ALL_t* coords = pll->coords;
	// implement abc to dq0 transform
	Transform_abc_dq0(&coords->abc, &coords->dq0, &coords->trigno, SRC_ABC, PARK_SINE);

	// implement PI on dq results
	// KP * q + Integrator * KI * q;
	float omega = PI_Compensate(&pll->compensator, coords->dq0.q);

	// freq = pi / 2 * omega;
	// magnitude = (d*d + q*q) ^ 0.5
	coords->trigno.wt = ShiftTheta_0to2pi(coords->trigno.wt, omega * pll->compensator.dt);
	Transform_wt_sincos(&coords->trigno);

	return IsPLLSynched(pll);
}
#pragma GCC pop_options
/* EOF */
