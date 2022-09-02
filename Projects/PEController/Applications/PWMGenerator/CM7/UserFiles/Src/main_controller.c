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
			.deadtime = {.on = false }
	};
	pwm_slave_opts_t slaveOpts =
	{
			.syncStartTim1 = false,
			.syncRestetSrc = PWM_RST_HRTIM_MASTER_CMP1
	};
	pwm_config_t pwm =
	{
			.dutyMode = OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH,
			.lim = { .min = 0, .max = 1, .minMaxDutyCycleBalancing = false },
			.module = &inverterPWMModuleConfig,
			.slaveOpts = &slaveOpts
	};

	// First Inverted Pair (PWM 1-2, Ref Channel = PWM1) (Reset is triggered by master compare 1 module)
	DutyCycleUpdateFnc updateFnc = BSP_PWM_ConfigInvertedPair(1, &pwm);
	updateFnc(1, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(1);
	BSP_Dout_SetAsPWMPin(2);

	// Second Inverted Pair (PWM 3-4, Ref Channel = PWM3) (Reset is triggered by master compare 2 module)
	slaveOpts.syncRestetSrc = PWM_RST_HRTIM_MASTER_CMP2;
	updateFnc = BSP_PWM_ConfigInvertedPair(3, &pwm);
	updateFnc(3, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(3);
	BSP_Dout_SetAsPWMPin(4);

	// Third Inverted Pair (PWM 5-6, Ref Channel = PWM6) (Reset is triggered by master compare 2 module)
	updateFnc = BSP_PWM_ConfigInvertedPair(6, &pwm);
	updateFnc(5, .5f, &pwm);
	BSP_Dout_SetAsPWMPin(5);
	BSP_Dout_SetAsPWMPin(6);

	BSP_PWMOut_Enable(0xff , true);

	hrtim_opts_t opts =
	{
			.periodInUsecs = 35,
			.syncResetTim1 = false,
			.syncStartTim1 = true,
	};
	BSP_AuxTim_ConfigHRTIM(&opts);
	BSP_AuxTim_SetDutyShift(&opts, HRTIM_COMP1, 0);
	BSP_AuxTim_SetDutyShift(&opts, HRTIM_COMP2, .25f);
	BSP_AuxTim_SetDutyShift(&opts, HRTIM_COMP3, .5f);
	BSP_AuxTim_SetDutyShift(&opts, HRTIM_COMP4, .75f);

	BSP_AuxTim_ConfigTim3(35, 18);
	BSP_AuxTim_StartTim3(true);

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
