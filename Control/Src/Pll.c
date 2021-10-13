/**
 ********************************************************************************
 * @file    	Pll.c
 * @author 		Waqas Ehsan Butt
 * @date    	Oct 7, 2021
 * @copyright 	TarazTechnologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */
#pragma GCC push_options
#pragma GCC optimize ("O3")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "Pll.h"
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
static float dataBufferMemory[FILTER_COUNT * FILTER_MAX_SIZE] = {0};
static float* lastBuffPtr = &dataBufferMemory[FILTER_COUNT * FILTER_MAX_SIZE - 1];
static float* nextBuffPtr = dataBufferMemory;
static pi_data_t qPI = { .Integral = 0, .Kp = 0.001f, .Ki = 0.08f, .dt = 0.00004f };
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
 * @brief Get pointer to the acquired buffer of given size
 * @param size Size of the buffer
 * @return float* Pointer to the buffer location
 */
static float* GetBufferPtr(int size)
{
	if (lastBuffPtr < (nextBuffPtr + size - 1))
		return NULL;
	float* result = nextBuffPtr;
	nextBuffPtr += size;
	return result;
}

/**
 * @brief Initializes the low pass filter
 *
 * @param *filt pointer to the low pass filter
 */
static void InitFilterIfNeeded(low_pass_filter_t* filt)
{
	// initialize data buffer if not already initialized
	if (filt->data == NULL)
	{
		// filter size should be between limits
		if (filt->size <= 0)
			filt->size = FILTER_DEFAULT_SIZE;
		if (filt->size > FILTER_MAX_SIZE)
			filt->size = FILTER_MAX_SIZE;

		// get the filter buffer
		filt->data = GetBufferPtr(filt->size);
		if (filt->data == NULL)
			Error_Handler();

		filt->filt.count = filt->size;
		filt->filt.dataPtr = filt->data;
	}
}

/**
 * @brief Initializes the low pass filter
 *
 * @param *comp pointer to the PI compensator
 */
static void InitCompensatorIfNeeded(pi_data_t* comp)
{
	if (comp->Ki == 0 && comp->Kp == 0)
	{
		comp->Ki = qPI.Ki;
		comp->Kp = qPI.Kp;
	}
	if(comp->dt <= 0)
		comp->dt = qPI.dt;
}

/**
 * @brief Apply low pass filters on the DQ parameters to normalize them
 *
 * @param pll pointer to the PLL configuration
 */
static void ApplyLowPassFilters_DQ(pll_lock_t* pll)
{
	pll->coords.dq0.q = MovingAverage_Float_Evaluate(&pll->qFilt.filt, pll->coords.dq0.q);
	pll->coords.dq0.d = MovingAverage_Float_Evaluate(&pll->dFilt.filt, pll->coords.dq0.d);
}


static pll_states_t IsPLLSynched(pll_lock_t* pll)
{
	pll_info_t* info = &pll->info;
	pll->prevStatus = pll->status;

	if(info->acceptableMax <= 0)
		info->acceptableMax = ACCEPTABLE_Q;
	if(info->maxIndex <= 0)
		info->maxIndex = MAX_Q_DATA_INDEX;

	float absQ = fabsf(pll->coords.dq0.q);
	if(absQ > info->tempCycleMax)
		info->tempCycleMax = absQ;

#if EVALUATE_D_STATS
	if (pll->coords.dq0.d < info->dMin)
		info->dMin = pll->coords.dq0.d;
	if (pll->coords.dq0.d > info->dMax)
		info->dMax = pll->coords.dq0.d;
#endif

	info->index++;

	if(info->index > info->maxIndex)
	{
		info->cycleMax = info->tempCycleMax;
		if(info->cycleMax < info->acceptableMax)
			pll->status = PLL_PENDING;
		info->index = 0;
		info->tempCycleMax = 0;
#if EVALUATE_D_STATS
		info->dDiff = info->dMax - info->dMin;
		info->dMid = (info->dMax + info->dMin) / 2.f;
		info->dMax = -200000;
		info->dMin = 200000;
#endif
	}

	// lock to the phase once the phase is very low
	if(pll->status == PLL_PENDING)
	{
		if (fabsf(pll->coords.abc.a) < (pll->dFilt.filt.avg) / 40)
			pll->status = PLL_LOCKED;
	}

	return pll->status;
}

/**
 * @brief Lock the grid voltages using Pll
 *
 * @param pll Pointer to the data structure
 * @return pll_states_t PLL_LOCKED if grid phase successfully locked
 */
pll_states_t Pll_LockGrid(pll_lock_t* pll)
{
	LIB_COOR_ALL_t* coords = &pll->coords;

	// Initialize the filters and Compensator if not already initialized
	InitFilterIfNeeded(&pll->dFilt);
	InitFilterIfNeeded(&pll->qFilt);
	InitCompensatorIfNeeded(&pll->compensator);

	// evaluate DQ0 coordinates from the ABC coordinates
	Transform_abc_dq0(&coords->abc, &coords->dq0, &coords->sinCosAngle, SRC_ABC, PARK_SINE);

	ApplyLowPassFilters_DQ(pll);

	float thetaShift = EvaluatePI(&pll->compensator, -coords->dq0.q);
	coords->sinCosAngle.theta -= thetaShift;
	coords->sinCosAngle.theta = AdjustTheta(coords->sinCosAngle.theta);

	Transform_theta_sincos(&coords->sinCosAngle);

	return IsPLLSynched(pll);
}

#pragma GCC pop_options
/* EOF */
