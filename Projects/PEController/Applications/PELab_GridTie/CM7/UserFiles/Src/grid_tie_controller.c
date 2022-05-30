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
#pragma GCC optimize ("-Ofast")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "grid_tie_controller.h"
#include "pecontroller_digital_out.h"
#include "pecontroller_digital_in.h"
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
// define the compensator for PI controller
pi_compensator_t boostPI = {
		.has_lmt = true,
		.Kp = BOOST_KP,
		.Ki = BOOST_KI,
		.dt = PWM_PERIOD_s,
		.Integral = 0,
		.max = BOOST_DUTYCYCLE_MAX,
		.min = 0.f };
/********************************************************************************
 * Global Variables
 *******************************************************************************/
float inverterDuties[3];
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
	gridTie->pll.coords = &gridTie->vCoor;
	gridTie->pll.compensator.Kp = KP_PLL;
	gridTie->pll.compensator.Ki = KI_PLL;
	gridTie->pll.compensator.dt = PWM_PERIOD_s;
	gridTie->pll.expectedGridFreq = 50;
	gridTie->pll.qLockMax = 20;
	gridTie->pll.dLockMin = 255;
	gridTie->pll.dLockMax = 375;
	gridTie->pll.cycleCount = (int)((1 / PWM_PERIOD_s) * 2);
	PLL_Init(&gridTie->pll);

	// configure Grid Tie Parameters
	gridTie->iQComp.Kp = KP_I;
	gridTie->iQComp.Ki = KI_I;
	gridTie->iQComp.dt = PWM_PERIOD_s;
	gridTie->iDComp.Kp = KP_I;
	gridTie->iDComp.Ki = KI_I;
	gridTie->iDComp.dt = PWM_PERIOD_s;

	// Set IOs as PWM
	BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[0]);
	BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[0] + 1);
	BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[1]);
	BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[1] + 1);
	BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[2]);
	BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[2] + 1);
	/***************** Configure Inverter *********************/

	// Turn off Grid Relays
	for (int i = 0; i < GRID_RELAY_COUNT; i++)
		BSP_Dout_SetAsIOPin(GRID_RELAY_IO + i, GPIO_PIN_RESET);

	/***************** Configure Boost *********************/
	for (int i = 0; i < BOOST_COUNT; i++)
	{
		independent_pwm_config_t* boostConfig = gridTie->boostConfig + i;
		boostConfig->pwmConfig.lim.min = 0;
		boostConfig->pwmConfig.lim.max = BOOST_DUTYCYCLE_MAX;
		boostConfig->pwmConfig.module = &boostPWMConfig;
		boostConfig->dutyUpdateFnc = BSP_PWM_ConfigChannel(boostConfig->pinNo, &boostConfig->pwmConfig);
		boostConfig->dutyUpdateFnc(boostConfig->pinNo, 0.f, &boostConfig->pwmConfig);
		BSP_Dout_SetAsPWMPin(boostConfig->pinNo);

		// Turn off the upper switch so that it behaves as a diode
		if(gridTie->boostDiodePin[i])
			BSP_Dout_SetAsIOPin(gridTie->boostDiodePin[i], GPIO_PIN_RESET);
	}
	gridTie->VbstSet = BOOST_VSET;
	/***************** Configure Boost *********************/
	// Configure the interrupt for PWM Channel with highest priority
	BSP_PWM_Config_Interrupt(inverterConfig->s1PinNos[0], true, pwmResetCallback, 0);
}

/**
 * Get the duty cycle for boost channels
 * @param gridTie
 * @return duty cycle to be applied
 */
static float GridTie_BoostControl(grid_tie_t* gridTie)
{
	return PI_Compensate(&boostPI, gridTie->VbstSet - gridTie->vdc);
}

/**
 * @brief Generate the output duty cycles for the grid tie inverter
 * @param gridTie
 * @param inverterDuties
 */
static void GridTie_GenerateOutput(grid_tie_t* gridTie, float* inverterDuties, bool disable)
{
	LIB_COOR_ALL_t* vCoor = &gridTie->vCoor;
	LIB_COOR_ALL_t* iCoor = &gridTie->iCoor;

	/******************** Compute Inverter Duty Cycles ******************/
	// As only real power is needed so voltage and current should be completely in phase
	memcpy(&iCoor->trigno, &vCoor->trigno, sizeof(vCoor->trigno));


	// Transform the current measurements to DQ coordinates
	Transform_abc_dq0(&iCoor->abc, &iCoor->dq0, &iCoor->trigno, SRC_ABC, PARK_SINE);

	// Apply PI control to both DQ coordinates gridTie->dCompensator.dt

	LIB_COOR_ALL_t coor;
	coor.dq0.d = PI_Compensate(&gridTie->iDComp, gridTie->iRef - iCoor->dq0.d) + vCoor->dq0.d
			- TWO_PI * GRID_FREQ * L_OUT * iCoor->dq0.q;
	coor.dq0.q = PI_Compensate(&gridTie->iQComp, 0             - iCoor->dq0.q) + vCoor->dq0.q
			+ TWO_PI * GRID_FREQ * L_OUT * iCoor->dq0.d;

	// Divide by VDC for normalization
	coor.dq0.d /= gridTie->vdc;
	coor.dq0.q /= gridTie->vdc;

	// if disabled reset integral term
	if (disable)
	{
		gridTie->iQComp.Integral = 0;
		gridTie->iDComp.Integral = 0;
	}

	// get the values in alpha beta coordinates
	Transform_alphaBeta0_dq0(&coor.alBe0, &coor.dq0, &iCoor->trigno, SRC_DQ0, PARK_SINE);
	/*
	Transform_abc_dq0(&coor.abc, &coor.dq0, &iCoor->trigno, SRC_DQ0, PARK_SINE);

	inverterDuties[0] = coor.abc.a + .5f;
	inverterDuties[1] = coor.abc.b + .5f;
	inverterDuties[2] = coor.abc.c + .5f;
	*/
	// Get SVPWM signal
	// SVPWM_GenerateDutyCycles(&coor.alBe0, inverterDuties);
	ComputeDuty_SVPWM_FromAlBe0(&coor.alBe0, inverterDuties);
	/******************** Compute Inverter Duty Cycles ******************/
}

uint32_t ts[10] = {0};

/**
 * @brief This function computes new duty cycles for the inverter and boost in each cycle
 * @param gridTie Pointer to the grid tie structure
 */
void GridTieControl_Loop(grid_tie_t* gridTie)
{
	// get pointer to the coordinates
	pll_lock_t* pll = &gridTie->pll;

	uint32_t temp1, temp2;

	temp1 = SysTick->VAL;
	// Compute and apply boost duty cycle
	float boostDuty = GridTie_BoostControl(gridTie);
	if (gridTie->VbstSet > 800.f)
		boostDuty = 0;
	temp2 = SysTick->VAL;
	if (temp2 < temp1)
		ts[0] = temp1 - temp2;
	temp1 = SysTick->VAL;
	for (int i = 0; i < BOOST_COUNT; i++)
		gridTie->boostConfig[i].dutyUpdateFnc(gridTie->boostConfig[i].pinNo, boostDuty, &gridTie->boostConfig[i].pwmConfig);
	temp2 = SysTick->VAL;
	if (temp2 < temp1)
		ts[1] = temp1 - temp2;

	temp1 = SysTick->VAL;
	// Relay Control depending On Vboost
	if (gridTie->isVbstStable)
	{
		// Turn off relay if goes beyond acceptable voltage
		if (gridTie->vdc < RELAY_TURN_OFF_VBST)
		{
			gridTie->isVbstStable = false;
			gridTie->tempIndex = 0;
			for (int i = 0; i < GRID_RELAY_COUNT; i++)
				BSP_Dout_SetAsIOPin(GRID_RELAY_IO + i, GPIO_PIN_RESET);
		}
	}
	else
	{
		// wait till boost voltage goes up
		if (gridTie->vdc < RELAY_TURN_ON_VBST)
			gridTie->tempIndex = 0;
		// wait for stabilization of boost
		else if (++gridTie->tempIndex == (int)PWM_FREQ_Hz)
		{
			gridTie->isVbstStable = true;
			for (int i = 0; i < GRID_RELAY_COUNT; i++)
				BSP_Dout_SetAsIOPin(GRID_RELAY_IO + i, GPIO_PIN_SET);
			gridTie->tempIndex = 0;
		}
	}
	temp2 = SysTick->VAL;
	if (temp2 < temp1)
		ts[2] = temp1 - temp2;

	temp1 = SysTick->VAL;
	// Implement phase lock loop
	Pll_LockGrid(pll);
	temp2 = SysTick->VAL;
	if (temp2 < temp1)
		ts[3] = temp1 - temp2;
	//pll->status = PLL_LOCKED;

	temp1 = SysTick->VAL;
	// Update status of grid tie depending upon dc-link and pll
	if(gridTie->state == GRID_TIE_INACTIVE)
	{
		if (gridTie->isVbstStable && pll->status == PLL_LOCKED)
		{
			gridTie->state = GRID_TIE_ACTIVE;
			Inverter3Ph_Activate(&gridTie->inverterConfig, true);
		}
	}
	else
	{
		if (!gridTie->isVbstStable || pll->status != PLL_LOCKED)
		{
			gridTie->state = GRID_TIE_INACTIVE;
			Inverter3Ph_Activate(&gridTie->inverterConfig, false);
		}
	}
	temp2 = SysTick->VAL;
	if (temp2 < temp1)
		ts[4] = temp1 - temp2;

	temp1 = SysTick->VAL;
	// compute and generate the duty cycle for inverter
	GridTie_GenerateOutput(gridTie, inverterDuties, gridTie->state == GRID_TIE_INACTIVE);
	temp2 = SysTick->VAL;
	if (temp2 < temp1)
		ts[5] = temp1 - temp2;

	/*
	static bool st = false;
	if (st)
	{
		st = false;
		inverterDuties[0] = .2f; inverterDuties[1] = .45f; inverterDuties[2] = .7f;
	}
	else
	{
		st = true;
		inverterDuties[0] = .8f; inverterDuties[1] = .2f; inverterDuties[2] = .3f;
	}
	*/

	temp1 = SysTick->VAL;
	Inverter3Ph_UpdateDuty(&gridTie->inverterConfig, inverterDuties);
	temp2 = SysTick->VAL;
	if (temp2 < temp1)
		ts[6] = temp1 - temp2;
}

#pragma GCC pop_options
/* EOF */
