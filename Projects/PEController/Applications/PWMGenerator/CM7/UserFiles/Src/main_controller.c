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
#include "pecontroller_aux_tim.h"
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
			.deadtime = {.on = false },
			.synchOnStart = false
	};
	pwm_config_t pwm =
	{
			.dutyMode = OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH,
			.lim = { .min = 0, .max = 1, .minMaxDutyCycleBalancing = false },
			.module = &inverterPWMModuleConfig
	};

	// configure the PWM functions
	for (int i = 0; i < 4; i++)
	{
		int index = i * 2 + 1;
		DutyCycleUpdateFnc fnc = BSP_PWM_ConfigInvertedPair(index, &pwm);
		fnc(index, .5f, &pwm);
		BSP_Dout_SetAsPWMPin(index);
		BSP_Dout_SetAsPWMPin(index+1);
	}

	BSP_PWMOut_Enable(0xff , true);

	hrtim_opts_t opts =
	{
			.periodInUsecs = 30,
			.syncResetTim1 = false,
			.syncStartTim1 = true,
	};
	BSP_AuxTim_ConfigHRTIM(&opts);
	BSP_AuxTim_SetDutyShift(&opts, HRTIM_COMP1, 0);
	BSP_AuxTim_SetDutyShift(&opts, HRTIM_COMP2, .5f);
	BSP_AuxTim_SetDutyShift(&opts, HRTIM_COMP3, .75);
	BSP_AuxTim_SetDutyShift(&opts, HRTIM_COMP4, .25);

	//BSP_AuxTim_ConfigTim3(30, 18);
	//BSP_AuxTim_StartTim3();
	//BSP_AuxTim_ConfigTim2(100, TIM_SLAVEMODE_COMBINED_RESETTRIGGER, TIM_TRIGGERPOLARITY_FALLING);
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
