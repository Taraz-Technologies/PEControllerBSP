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
};
static pwm_module_config_t boostPWMConfig =
{
		.alignment = EDGE_ALIGNED,
		.periodInUsec = PWM_PERIOD_Us,
		.deadtime = {0},
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
	gridTie->pll.cycleCount = (int)((1 / PWM_PERIOD_s) * 10);

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

	// Configure the interrupt for PWM Channel with highest priority
	BSP_PWM_Config_Interrupt(1, true, pwmResetCallback, 0);
}

static float GridTie_BoostControl(grid_tie_t* gridTie)
{
	/********************** Compute Boost Duty Cycle ********************/
	static float data_avg[8];
	static mov_avg_t filt = {.avg = 0, .count = 8, .dataPtr = data_avg};
	static pi_compensator_t boostPI = {.has_lmt = true, .Kp = .01f, .Ki = 1.1f, .dt = PWM_PERIOD_s, .
			Integral = 0, .max = 0.45f, .min = 0.f };
	float errAvg = MovingAverage_Compute(&filt, VBST_SET - gridTie->vdc);
	return PI_Compensate(&boostPI, errAvg);
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
	LIB_3COOR_ABC_t abc;
	iCoor->dq0.d *= (2.f/(gridTie->vdc));
	iCoor->dq0.q *= (2.f/(gridTie->vdc));
	Transform_abc_dq0(&abc, &iCoor->dq0, &iCoor->trigno, SRC_DQ0, PARK_SINE);
	// get duty cycles based on ABC coordinates
	inverterDuties[0] = (abc.a + 1) * .5f;
	inverterDuties[1] = (abc.b + 1) * .5f;
	inverterDuties[2] = (abc.c + 1) * .5f;
	/******************** Compute Inverter Duty Cycles ******************/

	Inverter3Ph_UpdateDuty(&gridTie->inverterConfig, inverterDuties);
}

/**
 * @brief This function computes new duty cycles for the inverter and boost in each cycle
 * @param gridTie Pointer to the grid tie structure
 */
void GridTieControl_Loop(grid_tie_t* gridTie)
{
	// get pointer to the coordinates
	pll_lock_t* pll = &gridTie->pll;
	float inverterDuties[3] = {.5f, .5f, .5f};
	float boostDuty = GridTie_BoostControl(gridTie);
	gridTie->boostConfig.dutyUpdateFnc(gridTie->boostConfig.pinNo, boostDuty, &gridTie->boostConfig.pwmConfig);

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
				gridTie->tempIndex = 0;
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
	inverterDuties[0] = .5f;
	inverterDuties[1] = .5f;
	inverterDuties[2] = .5f;
	Inverter3Ph_UpdateDuty(&gridTie->inverterConfig, inverterDuties);
}

#pragma GCC pop_options
/* EOF */
