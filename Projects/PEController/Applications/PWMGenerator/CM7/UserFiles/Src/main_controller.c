/**
 ********************************************************************************
 * @file    	main_controller.c
 * @author  	Waqas Ehsan Butt
 * @date    	October 5, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief  Main Controller for this Application
 ********************************************************************************
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "general_header.h"
#include "user_config.h"
#include "control_library.h"
#include "adc_config.h"
#include "main_controller.h"
#include "pecontroller_digital_in.h"
#include "pecontroller_pwm.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structures
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void Inverter3Ph_ResetSignal(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern adc_measures_t adcVals;
static volatile bool recompute = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Initialize the main control loop
 */
void MainControl_Init(void)
{
	// Configure digital IOs
	BSP_DigitalPins_Init();
	// Activate input port
	BSP_Din_SetPortGPIO();
	// latch zero to the output state till PWM signals are enabled
	BSP_Dout_SetPortValue(0);
	BSP_Dout_SetPortAsGPIO();

	pwm_module_config_t inverterPWMModuleConfig =
	{
			.alignment = EDGE_ALIGNED,
			.periodInUsec = 40,
			.deadtime = {.on = true, .nanoSec = 1000 }
	};
	pwm_slave_opts_t slaveOpts =
	{
			.syncSrc = PWM_SYNC_SRC_HRTIM_MASTER_CMP1,
			.syncType = TIM_SLAVEMODE_COMBINED_RESETTRIGGER,
	};
	pwm_config_t pwm =
	{
			.dutyMode = OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH,
			.lim = { .min = 0, .max = 1, .minMaxDutyCycleBalancing = false },
			.module = &inverterPWMModuleConfig,
			.slaveOpts = &slaveOpts
	};

	/****************** Inverted Pair synched to CMP1 of Master HRTIM ***********************/
	/** Creates an inverted PWM Pair which resets whenever the
	 * value matches compare 1 value of the master HRTIM */
	slaveOpts.syncSrc = PWM_SYNC_SRC_HRTIM_MASTER_CMP1;
	DutyCycleUpdateFnc updateFnc = BSP_PWM_ConfigInvertedPair(1, &pwm);
	updateFnc(1, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(1);
	BSP_Dout_SetAsPWMPin(2);
	/****************** Inverted Pair synched to CMP1 of Master HRTIM ***********************/

	/****** 2 Inverted Pairs Operating as a HBridge synched to CMP2 of Master HRTIM  *******/
	/** Creates a PWM sequence for the H-Bridges where switches 3, 6 get the same signal
	 * whereas switches 4,5 get the inverted signal. The phase shift for the H-Bridge is controlled
	 * via the value of compare 2 unit of master HRTIM */
	slaveOpts.syncSrc = PWM_SYNC_SRC_HRTIM_MASTER_CMP2;
	updateFnc = BSP_PWM_ConfigInvertedPair(3, &pwm);
	updateFnc(3, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(3);
	BSP_Dout_SetAsPWMPin(4);

	updateFnc = BSP_PWM_ConfigInvertedPair(6, &pwm);
	updateFnc(6, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(5);
	BSP_Dout_SetAsPWMPin(6);
	/****** 2 Inverted Pairs Operating as a HBridge synched to CMP2 of Master HRTIM  *******/

	/********************* Inverted Pair synched to FiberTx TIM3 ************************/
	/** Creates an inverted PWM Pair which resets whenever the
	 * Timer 3 falling edge is detected */
	slaveOpts.syncSrc = PWM_SYNC_SRC_TIM3;
	updateFnc = BSP_PWM_ConfigInvertedPair(7, &pwm);
	updateFnc(7, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(7);
	BSP_Dout_SetAsPWMPin(8);
	/********************* Inverted Pair synched to FiberTx TIM3 ************************/

	/****** 4 Inverted Pairs Operating as 2 HBridge synched to Fiber-Rx TIM2  *******/
	/** This portion shows the capability of synchronizing with another PEController
	 * via synch or fiber connection, while simultaneously showing the synchronization between
	 * HRTIM PWMs and TIM1 PWMs */
	BSP_TIM2_ConfigFiberRx(TIM_SLAVEMODE_COMBINED_RESETTRIGGER, TIM_TRIGGERPOLARITY_FALLING);
	slaveOpts.syncSrc = PWM_SYNC_SRC_TIM2;
	updateFnc = BSP_PWM_ConfigInvertedPair(9, &pwm);
	updateFnc(9, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(9);
	BSP_Dout_SetAsPWMPin(10);

	updateFnc = BSP_PWM_ConfigInvertedPair(12, &pwm);
	updateFnc(12, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(11);
	BSP_Dout_SetAsPWMPin(12);

	updateFnc = BSP_PWM_ConfigInvertedPair(13, &pwm);
	updateFnc(13, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(13);
	BSP_Dout_SetAsPWMPin(14);

	updateFnc = BSP_PWM_ConfigInvertedPair(16, &pwm);
	updateFnc(16, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(15);
	BSP_Dout_SetAsPWMPin(16);
	/****** 4 Inverted Pairs Operating as 2 HBridge synched to Fiber-Rx TIM2  *******/

	// Enables the output for all the PWM channels
	BSP_PWMOut_Enable(0xffff , true);

	// Set time period a little different so that synchronization is visible
	hrtim_opts_t opts =
	{
			.periodInUsecs = 38,
			.syncSrc = PWM_SYNC_SRC_NONE,
	};
	// configure the master HRTIM
	BSP_MasterHRTIM_Config(&opts);
	// Use these function to configure phase shifts by controlling the values for the compare units
	BSP_MasterHRTIM_SetShiftPercent(&opts, HRTIM_COMP1, 0);			// resets the timer aligned with master HRTIM
	BSP_MasterHRTIM_SetShiftPercent(&opts, HRTIM_COMP2, .25f);		// resets the timer with a 90 degrees phase shift with master HRTIM
	BSP_MasterHRTIM_SetShiftPercent(&opts, HRTIM_COMP3, .5f);		// resets the timer with a 180 degrees phase shift with master HRTIM
	BSP_MasterHRTIM_SetShiftPercent(&opts, HRTIM_COMP4, .75f);		// resets the timer with a 270 degrees phase shift with master HRTIM

	// Configure and connect the Fiber synchronization between PEControllers
	BSP_TIM3_ConfigFiberTx(opts.periodInUsecs, 18);
	// Start both fiber and HRTIM master timers
	BSP_TIM3_FiberTxStart(true);
}

/**
 * @brief Call this function to start generating PWM signals
 */
void MainControl_Run(void)
{
	BSP_PWM_Start(0xffff);
}

/**
 * @brief Call this function to stop the control loop from generating PWM signals
 */
void MainControl_Stop(void)
{
	BSP_PWM_Stop(0xffff);
}

/**
 * @brief Used to signal the computation for new duty cycle
 */
static void Inverter3Ph_ResetSignal(void)
{
	recompute = true;
}

/**
 * @brief Call this function to process the control loop.
 * If the new computation request is available new duty cycle values are computed and applied to all inverter legs
 */
void MainControl_Loop(void)
{
	if(recompute)
	{
		recompute = false;
	}
}

/* EOF */
