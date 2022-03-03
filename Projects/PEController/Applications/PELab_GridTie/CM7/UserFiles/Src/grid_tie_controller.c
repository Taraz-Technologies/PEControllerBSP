/**
 ********************************************************************************
 * @file    	grid_tie_controller.c
 * @author 		Waqas Ehsan Butt
 * @date    	December 7, 2021
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
//#pragma GCC optimize ("-Ofast")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "grid_tie_controller.h"
#include "pecontroller_digital_out.h"
#include "pecontroller_digital_in.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define DC_LINK_CAPACITANCE					(.00094f)
#define IL1_DISCON_VAL						(.1f)
#define L_BOOST								(.0025f)
#define EC1_DIV								(10.0f)
#define VBST_CONTROL_D0_LIMIT				(750.f)
#define IL1_LMT								(18)

#define BOOST_SIMPLE_PI			(1)
#define BOOST_MPC				(2)
#define BOOST_CASCADED			(3)
#define BOOST_METHOD 			(BOOST_MPC)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static pwm_module_config_t inverterPWMModuleConfig =
{
		.alignment = CENTER_ALIGNED,
		.periodInUsec = PWM_PERIOD_Us,
		.deadtime = {
				.on = true,
				.nanoSec = INVERTER_DEADTIME_ns,
		},
		.synchOnStart = true
};
static pwm_module_config_t boostPWMConfig =
{
		.alignment = CENTER_ALIGNED,
		.periodInUsec = PWM_PERIOD_Us,
		.synchOnStart = true
};
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
 * @brief Initialize the grid tie controller
 * @param gridTie Pointer to the grid tie structure
 * @param pwmResetCallback Function callback issued after each PWM completion
 */
void GridTieControl_Init(grid_tie_t* gridTie, PWMResetCallback pwmResetCallback)
{
	/***************** Configure Inverter *********************/
	inverter3Ph_config_t* inverterConfig = &gridTie->inverterConfig;
	inverterConfig->legType = LEG_DEFAULT;
	inverterConfig->pwmConfig.lim.min = 0;
	inverterConfig->pwmConfig.lim.max = 1;
	inverterConfig->pwmConfig.lim.minMaxDutyCycleBalancing = MIN_MAX_BALANCING_INVERTER;
	inverterConfig->pwmConfig.dutyMode = INVERTER_DUTY_MODE;
	inverterConfig->pwmConfig.module = &inverterPWMModuleConfig;
	Inverter3Ph_Init(inverterConfig);

	// configure PLL
	static float vDFiltData[PLL_FILT_SIZE] = {0};
	static float vQFiltData[PLL_FILT_SIZE] = {0};
	gridTie->pll.coords = &gridTie->vCoor;
	gridTie->pll.dFilt.dataPtr = vDFiltData;
	gridTie->pll.qFilt.dataPtr = vQFiltData;
	gridTie->pll.dFilt.count = PLL_FILT_SIZE;
	gridTie->pll.qFilt.count = PLL_FILT_SIZE;
	gridTie->pll.compensator.Kp = .001f;
	gridTie->pll.compensator.Ki = .8f;
	gridTie->pll.compensator.dt = PWM_PERIOD_s;
	gridTie->pll.qLockMax = 20;
	gridTie->pll.dLockMin = gridTie->pll.qLockMax * 10;
	gridTie->pll.cycleCount = (int)((1 / PWM_PERIOD_s) * 2);

	// configure Grid Tie Parameters
	gridTie->iQComp.Kp = 20.0f;
	gridTie->iQComp.Ki = 60.f;
	gridTie->iQComp.dt = PWM_PERIOD_s;
	gridTie->iDComp.Kp = 20.0f;
	gridTie->iDComp.Ki = 60.f;
	gridTie->iDComp.dt = PWM_PERIOD_s;
	gridTie->iRef = .1f;
	/***************** Configure Inverter *********************/

	/***************** Configure Boost *********************/
	independent_pwm_config_t* boostConfig = &gridTie->boostConfig;
	boostConfig->pwmConfig.lim.min = 0;
	boostConfig->pwmConfig.lim.max = BOOST_DUTYCYCLE_MAX;
	boostConfig->pwmConfig.module = &boostPWMConfig;
	boostConfig->dutyUpdateFnc = BSP_PWM_ConfigChannel(boostConfig->pinNo, &boostConfig->pwmConfig);
	boostConfig->dutyUpdateFnc(boostConfig->pinNo, 0.f, &boostConfig->pwmConfig);
	BSP_Dout_SetAsPWMPin(boostConfig->pinNo);
	/***************** Configure Boost *********************/

	BSP_Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_RESET);
	if(gridTie->boostDiodePin)
		BSP_Dout_SetAsIOPin(gridTie->boostDiodePin, GPIO_PIN_RESET);

	// Configure the interrupt for PWM Channel with highest priority
	BSP_PWM_Config_Interrupt(inverterConfig->s1PinNos[0], true, pwmResetCallback, 0);
}

#if BOOST_METHOD == BOOST_MPC
static float BoostVsetOffsetCompensation(float error, float setPoint, bool boostFault)
{
	static pi_compensator_t boostVsetOffsetPI = {.has_lmt = true, .dt = PWM_PERIOD_s, .Integral = 0, };
	if (boostFault)
		boostVsetOffsetPI.Integral = 0;
	//float errAbs = fabsf(error);
	boostVsetOffsetPI.Kp = .5f;
	boostVsetOffsetPI.Ki = 50.f;
	boostVsetOffsetPI.max = 20; // (setPoint * 0.12f) /200.0f;
	boostVsetOffsetPI.min = -boostVsetOffsetPI.max;
	return PI_Compensate(&boostVsetOffsetPI, error);
}
#endif

static float GridTie_BoostControl(grid_tie_t* gridTie, bool boostFault)
{
	/********************** Compute Boost Duty Cycle ********************/
#if BOOST_METHOD == BOOST_SIMPLE_PI
	static float data_avg[8];
	static mov_avg_t filt = {.avg = 0, .count = 8, .dataPtr = data_avg};
	static pi_compensator_t boostPI = {.has_lmt = true, .Kp = .01f, .Ki = .2f, .dt = PWM_PERIOD_s, .
			Integral = 0, .max = BOOST_DUTYCYCLE_MAX, .min = 0.f };
	float errAvg = MovingAverage_Compute(&filt, VBST_SET - gridTie->vdc);
	if (boostFault)
	{
		boostPI.Integral = 0;
		return 0;
	}
	return PI_Compensate(&boostPI, errAvg);
#endif
	/********************** Compute Boost Duty Cycle ********************/

	/********************** Compute Boost Duty Cycle ********************/
#if BOOST_METHOD == BOOST_MPC
	const float C1 = DC_LINK_CAPACITANCE * 0.5f;
	const float Tc = PWM_PERIOD_s;
	const float iL1ZeroVal = IL1_DISCON_VAL;
	float D0 = gridTie->boostD0;
	float Eout = 0;
	float Vrec = gridTie->vpv;
	float iL1 = gridTie->idc;
	float Vbst = gridTie->vdc;
	float L = L_BOOST;
	float M = (2.0f * L) / Tc;
	float iL1lmt = IL1_LMT;
	float Vref = VBST_SET;
	float Eloss = 0; 									/* stateStoreMulti->ElossBoost = BoostCompensation(Vref - Vbst, Vref); */
	float Ec1;											/* Energy estimate in dc link capacitor */
	float Ein;											/* Energy requirement for the next cycle */
	float D = 0;  										/* Computed duty cycle */
	float iL1ref = 0;									/* Average current computed for the next cycle */
	float a = (Vrec) / L;
	float b = (Vrec - Vbst)/L;
	/* Estimate iL1 @ the next cycle */
	iL1 = iL1+(a*D0*Tc)+(b*Tc*(1-D0));
	/* clamp to zero as iL1 is unidirectional */
	if(iL1 < 0)
		iL1 = 0;
	float piError2 = (Vref - Vbst);
	Vref += BoostVsetOffsetCompensation(piError2, Vref, boostFault);
	float diffAbsVrefVbst = fabsf(Vref - Vbst);
	Ec1 = (diffAbsVrefVbst / EC1_DIV) * 0.5f * C1 * ((Vref * Vref) - (Vbst * Vbst));
	Ein = Ec1 + Eloss + Eout; // Eout comes from output full bridge.
	iL1ref = Vrec > 0.001f ? Ein / (Vrec * Tc) : 0;
	if (iL1ref > iL1lmt)
		iL1ref = iL1lmt; // Limit max L1 current
	if (iL1ref < 0.00001f)
		iL1ref = 0.0f; // Limit min L1 current
	/* get duty for discontinuous mode */
	if (iL1 < iL1ZeroVal)
	{
		if(Vrec > 0.01f && (Vbst - Vrec) > 0.01f)
			D = sqrtf ((M * iL1ref)/(Vrec * (1+( Vrec /(Vbst - Vrec)))));
	}
	else
	{
		float divider = (Tc*((1.5f*a)-b));
		if(divider != 0)
			D = (iL1ref-iL1-(Tc*b))/divider;
		else
			D = 0.01f;
	}
	/* limit the value of duty such that the iL1lmt is not violated */
	if(((Vrec*D*Tc)/L)+iL1 > iL1lmt )
		D = (iL1lmt-iL1)*L/(Vrec*Tc);

	/* turn off for the next cycle if iL1 is too high */
	if (iL1 > 1.2f * iL1lmt)
		D = 0;

	/* turn off for the next cycle if vbst is too high */
	if (Vbst > VBST_CONTROL_D0_LIMIT)
		D = 0;

	if (boostFault)
		D = 0;

	return D;
#endif
	/********************** Compute Boost Duty Cycle ********************/

	/********************** Compute Boost Duty Cycle ********************/
#if BOOST_METHOD == BOOST_CASCADED
	float iLRef = gridTie->iLRef;
	static pi_compensator_t boostPI = {.has_lmt = false, .Kp = 1.f, .Ki = 50.f, .dt = PWM_PERIOD_s, .Integral = 0, };
	float div = gridTie->vdc > .01f ? gridTie->vdc : .01f;
	float refIntegral = boostPI.Integral;
	float pi = PI_Compensate(&boostPI, iLRef - gridTie->idc);
	float duty = (gridTie->vpv - pi) / div;
	if (duty < 0)
	{
		//boostPI.Integral = refIntegral;
		duty = 0;
	}
	if (duty > BOOST_DUTYCYCLE_MAX)
	{
		//boostPI.Integral = refIntegral;
		duty = BOOST_DUTYCYCLE_MAX;
	}

	float vgd = gridTie->vCoor.dq0.d > .01f ? gridTie->vCoor.dq0.d : .01f;
	float igd1 = (2/3.f) * (iLRef * gridTie->vpv / vgd);

	static float data_avg[8];
	static mov_avg_t filt = {.avg = 0, .count = 8, .dataPtr = data_avg};
	static pi_compensator_t boostPIV = {.has_lmt = false, .Kp = .01f, .Ki = .2f, .dt = PWM_PERIOD_s, .Integral = 0, };
	refIntegral = boostPIV.Integral;
	gridTie->iRef = MovingAverage_Compute(&filt, igd1) + PI_Compensate(&boostPIV, VBST_SET - gridTie->vdc);
	if (gridTie->iRef < -10)
	{
		boostPIV.Integral = refIntegral;
		gridTie->iRef = -10;
	}
	if (gridTie->iRef > 10)
	{
		boostPIV.Integral = refIntegral;
		gridTie->iRef = 10;
	}
	return duty;
#endif
	/********************** Compute Boost Duty Cycle ********************/
}

static void GridTie_GenerateOutput(grid_tie_t* gridTie, float* inverterDuties)
{
	LIB_COOR_ALL_t* vCoor = &gridTie->vCoor;
	LIB_COOR_ALL_t* iCoor = &gridTie->iCoor;

	/******************** Compute Inverter Duty Cycles ******************/
	// As only real power is needed so voltage and current should be completely in phase
	memcpy(&iCoor->trigno, &vCoor->trigno, sizeof(vCoor->trigno));

	// Transform the current measurements to DQ coordinates
	Transform_abc_dq0(&iCoor->abc, &iCoor->dq0, &iCoor->trigno, SRC_ABC, PARK_SINE);

	// Apply PI control to both DQ coordinates gridTie->dCompensator.dt
	iCoor->dq0.d = PI_Compensate(&gridTie->iDComp, gridTie->iRef - iCoor->dq0.d) + vCoor->dq0.d
			- TWO_PI * GRID_FREQ * L_OUT * iCoor->dq0.q;

	iCoor->dq0.q = PI_Compensate(&gridTie->iQComp, 0             - iCoor->dq0.q) + vCoor->dq0.q
			+ TWO_PI * GRID_FREQ * L_OUT * iCoor->dq0.d;

	// get ABCD coordinates from DQ coordinates
	/** Model 1 **/ /*
	// get ABCD coordinates from DQ coordinates
	LIB_3COOR_ABC_t abc;
	iCoor->dq0.d *= (2.f/(gridTie->vdc));
	iCoor->dq0.q *= (2.f/(gridTie->vdc));
	Transform_abc_dq0(&abc, &iCoor->dq0, &iCoor->trigno, SRC_DQ0, PARK_SINE);
	// get duty cycles based on ABC coordinates
	inverterDuties[0] = (abc.a + 1) * .5f;
	inverterDuties[1] = (abc.b + 1) * .5f;
	inverterDuties[2] = (abc.c + 1) * .5f;
	/**//** Model 1 **/

	/** Model 2 **/
	// get ABCD coordinates from DQ coordinates
	LIB_3COOR_ABC_t abc;
	Transform_abc_dq0(&abc, &iCoor->dq0, &iCoor->trigno, SRC_DQ0, PARK_SINE);
	// get duty cycles based on ABC coordinates
	inverterDuties[0] = (abc.a / gridTie->vdc) + .5f;
	inverterDuties[1] = (abc.b / gridTie->vdc) + .5f;
	inverterDuties[2] = (abc.c / gridTie->vdc) + .5f;
	/**//** Model 2 **/
	/******************** Compute Inverter Duty Cycles ******************/

	Inverter3Ph_UpdateDuty(&gridTie->inverterConfig, inverterDuties);
}
volatile float boostDutyNo = 0;
volatile int boostCount = 0;
/**
 * @brief This function computes new duty cycles for the inverter and boost in each cycle
 * @param gridTie Pointer to the grid tie structure
 */
void GridTieControl_Loop(grid_tie_t* gridTie)
{
	// get pointer to the coordinates
	pll_lock_t* pll = &gridTie->pll;
	float inverterDuties[3] = {.5f, .5f, .5f};

	bool inverterBridgeFault = BSP_Din_GetPinState(9) | BSP_Din_GetPinState(10) | BSP_Din_GetPinState(11) | BSP_Din_GetPinState(13);
	bool boostBridgeFault = BSP_Din_GetPinState(12) | BSP_Din_GetPinState(14) | BSP_Din_GetPinState(15) | BSP_Din_GetPinState(16);
	float boostDuty = GridTie_BoostControl(gridTie, boostBridgeFault);
	if (boostDuty > 0)
	{
		boostDutyNo = boostDuty;
		boostCount++;
	}
	gridTie->boostD0 = gridTie->boostConfig.dutyUpdateFnc(gridTie->boostConfig.pinNo, boostDuty, &gridTie->boostConfig.pwmConfig);
	if (Pll_LockGrid(pll) == PLL_LOCKED)
	{
		// if not stabilized yet stabilize
		if (gridTie->state == GRID_TIE_INACTIVE)
		{
			if (gridTie->vdc < RELAY_TURN_ON_VBST)
				gridTie->tempIndex = 0;
			else if (++gridTie->tempIndex == (int)PWM_FREQ_Hz)
			{
				gridTie->state = GRID_TIE_ACTIVE;
				BSP_Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_SET);
				BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[0]);
				BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[0] + 1);
				BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[1]);
				BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[1] + 1);
				BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[2]);
				BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[2] + 1);
				gridTie->tempIndex = 0;
				return;
			}
		}
		// if boost voltage very low then disable system
		else if (gridTie->vdc < RELAY_TURN_OFF_VBST)
		{
			gridTie->state = GRID_TIE_INACTIVE;
			BSP_Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_RESET);
			gridTie->tempIndex = 0;
		}
		else
		{
			GridTie_GenerateOutput(gridTie, inverterDuties);
			return;
		}
	}
	else
	{
		if (gridTie->state != GRID_TIE_INACTIVE)
			gridTie->state = GRID_TIE_INACTIVE;
		BSP_Dout_SetAsIOPin(GRID_RELAY_IO, GPIO_PIN_RESET);
		gridTie->tempIndex = 0;
	}
	gridTie->iQComp.Integral = 0;
	gridTie->iDComp.Integral = 0;
	BSP_Dout_SetAsIOPin(gridTie->inverterConfig.s1PinNos[0], GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(gridTie->inverterConfig.s1PinNos[0] + 1, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(gridTie->inverterConfig.s1PinNos[1], GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(gridTie->inverterConfig.s1PinNos[1] + 1, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(gridTie->inverterConfig.s1PinNos[2], GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(gridTie->inverterConfig.s1PinNos[2] + 1, GPIO_PIN_RESET);
	Inverter3Ph_UpdateDuty(&gridTie->inverterConfig, inverterDuties);
}

#pragma GCC pop_options
/* EOF */
