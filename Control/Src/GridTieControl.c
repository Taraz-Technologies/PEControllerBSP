/**
 ********************************************************************************
 * @file    	GridTieControl.c
 * @author 		Waqas Ehsan Butt
 * @date    	Oct 13, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */
#pragma GCC push_options
#pragma GCC optimize ("-Ofast")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "GridTieControl.h"
#include "DoutPort.h"
#include <string.h>
/********************************************************************************
 * Defines
 *******************************************************************************/
#define GRID_RELAY_IO					(15)

#define SPWM							(0)
#define SVPWM							(1)
#define USE_DQ							(1)
#define TECH_1							(2)
#define PWM_TECH						(TECH_1)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	grid_tie_t gridTie;
	LIB_COOR_ALL_t vCoorL;
	LIB_3COOR_ABC_t abc;
	float duties[3];
} log_fault_t;
/********************************************************************************
 * Static Variables
 *******************************************************************************/
pi_data_t qPI = { .Integral = 0, .Kp = 16.f, .Ki = 160.f, .dt = 0.00004f };
pi_data_t dPI = { .Integral = 0, .Kp = 16.f, .Ki = 160.f, .dt = 0.00004f };
pi_data_t v1PI = { .Integral = 0, .Kp = 200.f, .Ki = .0f, .dt = 0.00004f, .has_lmt = true, .max = .5f, .min = -.5f };
pi_data_t v2PI = { .Integral = 0, .Kp = 200.f, .Ki = .0f, .dt = 0.00004f, .has_lmt = true, .max = .5f, .min = -.5f };
pi_data_t v3PI = { .Integral = 0, .Kp = 200.f, .Ki = .0f, .dt = 0.00004f, .has_lmt = true, .max = .5f, .min = -.5f };
static float dataBufferMemory[2 * 32] = {0};
static float* lastBuffPtr = &dataBufferMemory[2 * 32 - 1];
static float* nextBuffPtr = dataBufferMemory;
static low_pass_filter_t dFilt = {0};
static low_pass_filter_t qFilt = {0};
LIB_COOR_ALL_t vCoorL = {0};
LIB_3COOR_ABC_t abc = {0};
float maxDiff = 0;
float old = 0;
log_fault_t logItem[8];
int logCount = 0;
int logCountMod = 7;
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
			filt->size = 2;
		if (filt->size > 32)
			filt->size = 32;

		// get the filter buffer
		filt->data = GetBufferPtr(filt->size);
		if (filt->data == NULL)
			Error_Handler();

		filt->filt.count = filt->size;
		filt->filt.dataPtr = filt->data;
	}
}

int contCountMax = 0;
int contCount = 0;
volatile uint32_t endCount, piCount, dqCount, pllCount;

/**
 * @brief Evaluates the Duty Cycle Values using the Grid Tie Control
 *
 * @param *gridTie Pointer to the parameter structure
 * @param *duties resultant duty cycles for the inverter (Range 0 - 1)
 */
void GridTieControl_GetDuties(grid_tie_t* gridTie, float* duties)
{
	pll_lock_t* pll = &gridTie->pll;
	LIB_COOR_ALL_t* vCoor = &gridTie->vCoor;
	LIB_COOR_ALL_t* iCoor = &gridTie->iCoor;
	if (pll->coords == NULL)
		pll->coords = vCoor;

	//InitFilterIfNeeded(&qFilt);
	//InitFilterIfNeeded(&dFilt);
	uint32_t ticks = HAL_GetTick();
	uint32_t t1 = SysTick->VAL;
	if (Pll_LockGrid(pll) == PLL_LOCKED)
	{
		uint32_t t2 = SysTick->VAL;
		if (ticks == HAL_GetTick())
			pllCount = t1 - t2;
		if(pll->prevStatus != PLL_LOCKED)
		{
			Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_SET);
			//old = vCoor->sinCosAngle.wt;
		}

		memcpy(&vCoorL, vCoor, sizeof(vCoorL));
#if PWM_TECH == TECH_1

		memcpy(&iCoor->sinCosAngle, &vCoor->sinCosAngle, sizeof(vCoor->sinCosAngle));
		/*
		float absDiff = fabsf(old - vCoor->sinCosAngle.wt);
		if (absDiff < PI)
		{
			if (absDiff > maxDiff)
				maxDiff = absDiff;
		}
		old = vCoor->sinCosAngle.wt;
		*/

		// copy coordinates of grid voltages
		//iCoor->sinCosAngle.wt = vCoor->sinCosAngle.wt;

		ticks = HAL_GetTick();
		t1 = SysTick->VAL;
		Transform_abc_dq0(&iCoor->abc, &iCoor->dq0, &iCoor->sinCosAngle, SRC_ABC, PARK_SINE);
		//Transform_abc_dq0(&vCoorL.abc, &vCoorL.dq0, &vCoorL.sinCosAngle, SRC_ABC, PARK_SINE);
		t2 = SysTick->VAL;
		if (ticks == HAL_GetTick())
			dqCount = t1 - t2;

		ticks = HAL_GetTick();
		t1 = SysTick->VAL;
		iCoor->dq0.d = EvaluatePI(&dPI, gridTie->iRef - iCoor->dq0.d) - TWO_PI * 50 * qPI.dt * iCoor->dq0.q + vCoorL.dq0.d;
		iCoor->dq0.q = EvaluatePI(&qPI, 0             - iCoor->dq0.q) + TWO_PI * 50 * qPI.dt * iCoor->dq0.d + vCoorL.dq0.q;
		t2 = SysTick->VAL;
		if (ticks == HAL_GetTick())
			piCount = t1 - t2;

		ticks = HAL_GetTick();
		t1 = SysTick->VAL;
		Transform_abc_dq0(&abc, &iCoor->dq0, &iCoor->sinCosAngle, SRC_DQ0, PARK_SINE);

		duties[0] = (abc.a / gridTie->vdc) + .5f;
		duties[1] = (abc.b / gridTie->vdc) + .5f;
		duties[2] = (abc.c / gridTie->vdc) + .5f;
		t2 = SysTick->VAL;
		if (ticks == HAL_GetTick())
			endCount = t1 - t2;
		/*
		log_fault_t* ptr = &logItem[logCount];
		logCount = (logCount + 1) & logCountMod;
		memcpy(&ptr->gridTie, gridTie, sizeof(ptr->gridTie));
		memcpy(&ptr->abc, &abc, sizeof(ptr->abc));
		memcpy(&ptr->vCoorL, &vCoorL, sizeof(ptr->vCoorL));
		memcpy(ptr->duties, duties, 4 * 3);

		if (fabsf(iCoor->abc.a) > 18)
		{
			if (contCount == 0)
				iCoor->abc.a = 18;
			if(++contCount > contCountMax)
				contCountMax = contCount;
		}
		else
			contCount = 0;

		*/

#else
#if USE_DQ
		iCoor->sinCosAngle.wt = vCoor->sinCosAngle.wt;
		iCoor->sinCosAngle.wt = AdjustTheta(iCoor->sinCosAngle.wt);
		Transform_theta_sincos(&iCoor->sinCosAngle);

		// Get DQ0
		Transform_abc_dq0(&iCoor->abc, &iCoor->dq0, &iCoor->sinCosAngle, SRC_ABC, PARK_SINE);

		// averaging
		//iCoor->dq0.q = MovingAverage_Float_Evaluate(&qFilt.filt, iCoor->dq0.q);
		//iCoor->dq0.d = MovingAverage_Float_Evaluate(&dFilt.filt, iCoor->dq0.d);

		// Apply PIs
		iCoor->dq0.q = EvaluatePI(&qPI, -iCoor->dq0.q);
		iCoor->dq0.d = EvaluatePI(&dPI, gridTie->iRef - iCoor->dq0.d);

		// Get ALBE0
		Transform_alphaBeta0_dq0(&iCoor->alBe0, &iCoor->dq0, &iCoor->sinCosAngle, SRC_DQ0, PARK_SINE);

#if PWM_TECH == SVPWM
		GetSVPWM_FromAlBe(&gridTie->iCoor.alBe0, duties);
#else
		GenerateSPWM(gridTie->iCoor.sinCosAngle.wt, 0.5f, duties);
#endif
#else
		iCoor->sinCosAngle.wt = vCoor->sinCosAngle.wt;

		float iRefa = gridTie->iRef * (vCoor->abc.a / 340); //sinf(iCoor->sinCosAngle.wt);
		float iRefb = gridTie->iRef * (vCoor->abc.b / 340); //sinf(iCoor->sinCosAngle.wt + TWO_PI/3);
		float iRefc = gridTie->iRef * (vCoor->abc.c / 340); //sinf(iCoor->sinCosAngle.wt - TWO_PI/3);

		duties[0] = EvaluatePI(&v1PI, iRefa-iCoor->abc.a);
		duties[1] = EvaluatePI(&v2PI, iRefb-iCoor->abc.b);
		duties[2] = EvaluatePI(&v3PI, iRefc-iCoor->abc.c);

		for (int i = 0; i < 3; i++)
		{
			if(duties[i] < -.5f)
				duties[i] = -.5f;
			else if (duties[i] > .5f)
				duties[i] = .5f;

			duties[i] = duties[i] + .5f;
		}
#endif
#endif
	}
	else
	{
		if(pll->prevStatus == PLL_LOCKED)
			Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_RESET);
		duties[0] = 0.5f; duties[1] = 0.5f; duties[2] = 0.5f;
	}
}

#pragma GCC pop_options
/* EOF */
