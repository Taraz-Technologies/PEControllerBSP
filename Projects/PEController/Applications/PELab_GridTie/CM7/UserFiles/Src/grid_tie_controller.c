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
#include "shared_memory.h"
#include "control_library.h"
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
/**
 * @brief PWM configuration for the inverter
 */
static pwm_module_config_t inverterPWMModuleConfig =
{
		.alignment = CENTER_ALIGNED,
		.f = PWM_FREQ_Hz,
		.deadtime = {
				.on = true,
				.nanoSec = INVERTER_DEADTIME_ns,
		},
};
/**
 * @brief PWM configuration for the boost switches
 */
static pwm_module_config_t boostPWMConfig =
{
		.alignment = CENTER_ALIGNED,
		.f = PWM_FREQ_Hz,
};
static tim_in_trigger_config_t timerTriggerIn =
{
		.src = TIM_TRG_SRC_TIM1,
		.type = TIM_TRGI_TYPE_RESET_AND_START
};
static tim_out_trigger_config_t timerTriggerOut =
{
		.type = TIM_TRGO_OUT_UPDATE
};
/**
 * @brief Compensator for PI
 */
pi_compensator_t boostPI = {
		.has_lmt = true,
		.Kp = KP_BOOST,
		.Ki = KI_BOOST,
		.dt = PWM_PERIOD_s,
		.Integral = 0,
		.max = BOOST_DUTYCYCLE_MAX,
		.min = 0.f };

avg_t iGenAvg = { .count = PWM_FREQ_Hz * 2, };
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
	inverterConfig->pwmConfig.slaveOpts = &timerTriggerIn;
	inverterConfig->pwmConfig.masterOpts = &timerTriggerOut;
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
	gridTie->pll.dLockMax = 390;
	gridTie->pll.cycleCount = (int)(PWM_PERIOD_s * 2);
	PLL_Init(&gridTie->pll);

	// configure Grid Tie Parameters
	gridTie->iQComp.Kp = KP_I;
	gridTie->iQComp.Ki = KI_I;
	gridTie->iQComp.dt = PWM_PERIOD_s;
	gridTie->iDComp.Kp = KP_I;
	gridTie->iDComp.Ki = KI_I;
	gridTie->iDComp.dt = PWM_PERIOD_s;

	// Set IOs as PWM
	for (int i = 0; i < BOOST_COUNT; i++)
	{
		BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[i]);
		BSP_Dout_SetAsPWMPin(gridTie->inverterConfig.s1PinNos[i] + 1);
	}
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
		boostConfig->pwmConfig.slaveOpts = &timerTriggerIn;
		boostConfig->pwmConfig.masterOpts = &timerTriggerOut;
		boostConfig->dutyUpdateFnc = BSP_PWM_ConfigChannel(boostConfig->pinNo, &boostConfig->pwmConfig);
		boostConfig->dutyUpdateFnc(boostConfig->pinNo, 0.f, &boostConfig->pwmConfig);
		BSP_Dout_SetAsPWMPin(boostConfig->pinNo);

		// Turn off the upper switch so that it behaves as a diode
		if(gridTie->boostDiodePin[i])
			BSP_Dout_SetAsIOPin(gridTie->boostDiodePin[i], GPIO_PIN_RESET);

		// Deactivate the boost at startup
		BSP_PWMOut_Enable((1 << (boostConfig->pinNo - 1)) , false);			// --TODO-- TNPC leg for boost??
	}
	gridTie->VbstSet = BOOST_VSET;
	/***************** Configure Boost *********************/
	// Configure the interrupt for PWM Channel with highest priority
	BSP_PWM_Config_Interrupt(inverterConfig->s1PinNos[0], true, pwmResetCallback, 1);

	Average_Reset(&iGenAvg);
}

device_err_t GridTie_EnableBoost(grid_tie_t* gridTie, bool en)
{
	if (gridTie->isBoostEnabled == en)
		return ERR_OK;
	//if (gridTie->isInverterEnabled)
	//return ERR_INVERTER_ACTIVE;
	// refresh the PI Controller
	boostPI.Integral = 0;
	for (int i = 0; i < BOOST_COUNT; i++)
		BSP_PWMOut_Enable((1 << (gridTie->boostConfig[i].pinNo - 1)) , en);
	// correct flags
	INTER_CORE_DATA.bools[SHARE_BOOST_STATE] = gridTie->isBoostEnabled = en;
	return ERR_OK;
}

device_err_t GridTie_EnableInverter(grid_tie_t* gridTie, bool en)
{
	if (gridTie->isInverterEnabled == en)
		return ERR_OK;

	Average_Reset(&iGenAvg);
	// refresh the PI Controllers
	gridTie->iQComp.Integral = 0;
	gridTie->iDComp.Integral = 0;
	if (en == false)
	{
		// Disable inverters
		Inverter3Ph_Activate(&gridTie->inverterConfig, en);
		// set flags
		INTER_CORE_DATA.bools[SHARE_INVERTER_STATE] = gridTie->isInverterEnabled = en;
		return ERR_OK;
	}
	else
	{
		//if (gridTie->isBoostEnabled == false)
		//return ERR_BOOST_DISABLED;
		if (gridTie->isRelayOn == false)
			return ERR_RELAY_OFF;
		if (gridTie->pll.status != PLL_LOCKED)
			return ERR_PLL_NOT_LOCKED;
		// Enable inverters
		Inverter3Ph_Activate(&gridTie->inverterConfig, en);
		// set flags
		INTER_CORE_DATA.bools[SHARE_INVERTER_STATE] = gridTie->isInverterEnabled = en;
		return ERR_OK;
	}
}

/**
 * Get the duty cycle for boost channels.
 * @param gridTie Grid Tie structure to be used.
 * @return Duty cycle to be applied.
 */
static float GridTie_BoostControl(grid_tie_t* gridTie)
{
	return PI_Compensate(&boostPI, gridTie->VbstSet - gridTie->vdc);
}

/**
 * @brief Generate the output duty cycles for the grid tie inverter
 * @param gridTie Grid Tie structure to be used.
 */
static void GridTie_GenerateOutput(grid_tie_t* gridTie)
{
	LIB_COOR_ALL_t* vCoor = &gridTie->vCoor;
	LIB_COOR_ALL_t* iCoor = &gridTie->iCoor;
	float inverterDuties[3];

	/******************** Compute Inverter Duty Cycles ******************/
	// As only real power is needed so voltage and current should be completely in phase
	memcpy(&iCoor->trigno, &vCoor->trigno, sizeof(vCoor->trigno));

	// Transform the current measurements to DQ coordinates
	Transform_abc_dq0(&iCoor->abc, &iCoor->dq0, &iCoor->trigno, SRC_ABC, PARK_SINE);
	if(Average_Compute(&iGenAvg, iCoor->dq0.d))
		INTER_CORE_DATA.floats[SHARE_CURR_RMS_CURRENT] = iGenAvg.avg / 1.414f;
	// Apply PI control to both DQ coordinates gridTie->dCompensator.dt
	LIB_COOR_ALL_t coor;

	// Get the required parameters
	float fGrid = INTER_CORE_DATA.floats[SHARE_GRID_FREQ];
	float lOut = INTER_CORE_DATA.floats[SHARE_LOUT];
	// convert to peak current
	gridTie->iRef = INTER_CORE_DATA.floats[SHARE_REQ_RMS_CURRENT] * 1.414f;

	coor.dq0.d = PI_Compensate(&gridTie->iDComp, gridTie->iRef - iCoor->dq0.d) + vCoor->dq0.d
			- TWO_PI * fGrid * lOut * iCoor->dq0.q / PWM_FREQ_Hz;
	coor.dq0.q = PI_Compensate(&gridTie->iQComp, 0             - iCoor->dq0.q) + vCoor->dq0.q
			+ TWO_PI * fGrid * lOut * iCoor->dq0.d / PWM_FREQ_Hz;

	// Divide by VDC for normalization
	coor.dq0.d /= gridTie->vdc;
	coor.dq0.q /= gridTie->vdc;

	// get the values in alpha beta coordinates
	Transform_alphaBeta0_dq0(&coor.alBe0, &coor.dq0, &iCoor->trigno, SRC_DQ0, PARK_SINE);
	// Get SVPWM signal
	SVPWM_GenerateDutyCycles(&coor.alBe0, inverterDuties);
	/******************** Compute Inverter Duty Cycles ******************/

	// generate the duty cycle for the inverter
	Inverter3Ph_UpdateDuty(&gridTie->inverterConfig, inverterDuties);
}

static void ControlRelays(grid_tie_t* gridTie)
{
	// Relay Control depending On Vboost
	if (gridTie->isRelayOn)
	{
		// Turn off relay if goes beyond acceptable voltage
		if (gridTie->vdc < RELAY_TURN_ON_VDC)
		{
			gridTie->isRelayOn = INTER_CORE_DATA.bools[SHARE_RELAY_STATUS] = false;
			gridTie->tempIndex = 0;
			for (int i = 0; i < GRID_RELAY_COUNT; i++)
				BSP_Dout_SetAsIOPin(GRID_RELAY_IO + i, GPIO_PIN_RESET);
		}
	}
	else
	{
		// wait till boost voltage goes up
		if (gridTie->vdc < RELAY_TURN_OFF_VDC)			// --FIXME-- Change with Grid Voltage
			gridTie->tempIndex = 0;
		// wait for stabilization of boost
		else if (++gridTie->tempIndex == (int)PWM_FREQ_Hz)
		{
			gridTie->isRelayOn = INTER_CORE_DATA.bools[SHARE_RELAY_STATUS] = true;
			for (int i = 0; i < GRID_RELAY_COUNT; i++)
				BSP_Dout_SetAsIOPin(GRID_RELAY_IO + i, GPIO_PIN_SET);
			gridTie->tempIndex = 0;
		}
	}
}

/**
 * @brief This function computes new duty cycles for the inverter and boost in each cycle
 * @param gridTie Pointer to the grid tie structure
 */
void GridTieControl_Loop(grid_tie_t* gridTie)
{
	// get pointer to the coordinates
	pll_lock_t* pll = &gridTie->pll;

	// Compute and apply boost duty cycle
	// Only compute and apply if the boost is already enabled
	if (gridTie->isBoostEnabled)
	{
		float boostDuty = GridTie_BoostControl(gridTie);
		if (gridTie->vdc > 800.f)				// --FIXME-- Limit should be on Vdc
			boostDuty = 0;

		for (int i = 0; i < BOOST_COUNT; i++)
			gridTie->boostConfig[i].dutyUpdateFnc(gridTie->boostConfig[i].pinNo, boostDuty, &gridTie->boostConfig[i].pwmConfig);
	}

	// Control relays for protection of the circuitry
	ControlRelays(gridTie);

	// Implement phase lock loop
	Pll_LockGrid(pll);
	INTER_CORE_DATA.bools[SHARE_PLL_STATUS] = gridTie->pll.status == PLL_LOCKED;

	// Generate inverter PWM is enabled and not faulty
	if (gridTie->isInverterEnabled)
	{
		// Update status of grid tie depending upon dc-link and PLL
		if(gridTie->isRelayOn == false || gridTie->pll.status != PLL_LOCKED)
		{
			Inverter3Ph_Activate(&gridTie->inverterConfig, false);
			gridTie->isInverterEnabled = INTER_CORE_DATA.bools[SHARE_INVERTER_STATE] = false;
			Average_Reset(&iGenAvg);
		}
		else
			// compute and generate the duty cycle for inverter
			GridTie_GenerateOutput(gridTie);
	}
}

#pragma GCC pop_options
/* EOF */
