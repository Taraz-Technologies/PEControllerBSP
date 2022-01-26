/**
 ********************************************************************************
 * @file    	MainControl.c
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
#include "control_library.h"
#include "adc_config.h"
#include "open_loop_vf_controller.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define OPEN_LOOP_VF_CONTROL			(1)
#define GRID_TIE_CONTROL				(2)

#define CONTROL_TYPE					(GRID_TIE_CONTROL)

#define PWM_PERIOD_Us					(40)
#define PWM_PERIOD_s					(PWM_PERIOD_Us/1000000.f)
#define PWM_FREQ_KHz					(1000.f/PWM_PERIOD_Us)
#define PWM_FREQ_Hz						(1.f/PWM_PERIOD_s)

#define RUN_ALL_FULL_BRIDGE				(0)
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
openloopvf_config_t openLoopVfConfig1 = {0};
openloopvf_config_t openLoopVfConfig2 = {0};
static volatile bool recompute = false;
extern HRTIM_HandleTypeDef hhrtim;
extern TIM_HandleTypeDef htim1;
#if RUN_ALL_FULL_BRIDGE
static pwm_module_config_t inverterPWMModuleConfig =
{
		.alignment = CENTER_ALIGNED,
		.periodInUsec = PWM_PERIOD_Us,
		.deadtime = {
				.on = true,
				.nanoSec = INVERTER_DEADTIME_ns,
		},
};
static pwm_config_t config =
{
		.dutyMode = OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH,
		.module = &inverterPWMModuleConfig,
		.lim = { .min = 0, .max = 1, .minMaxDutyCycleBalancing = false }
};
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Initiates PWM for both inverters, and enable Disable signals and configure the relays
 */
void MainControl_Init(void)
{
	BSP_DigitalPins_Init();
#if RUN_ALL_FULL_BRIDGE == 0
	BSP_Dout_SetAsIOPin(7, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(8, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(15, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(16, GPIO_PIN_RESET);
	openLoopVfConfig1.inverterConfig.s1PinNos[0] = 1;
	openLoopVfConfig1.inverterConfig.s1PinNos[1] = 3;
	openLoopVfConfig1.inverterConfig.s1PinNos[2] = 5;
	openLoopVfConfig1.inverterConfig.dsblPinCount = 0;
	OpenLoopVfControl_Init(&openLoopVfConfig1, Inverter3Ph_ResetSignal);

	openLoopVfConfig2.inverterConfig.s1PinNos[0] = 9;
	openLoopVfConfig2.inverterConfig.s1PinNos[1] = 11;
	openLoopVfConfig2.inverterConfig.s1PinNos[2] = 13;
	openLoopVfConfig2.inverterConfig.dsblPinCount = 0;
	OpenLoopVfControl_Init(&openLoopVfConfig2, NULL);

	// turn off relays and disable pins
#else
	for (int i = 1; i < 16; i+=4)
	{
		config.invertPol = false;
		DutyCycleUpdateFnc callback = BSP_PWM_ConfigInvertedPair(i, &config);
		callback(i, 0.5f, &config);
		config.invertPol = true;
		callback = BSP_PWM_ConfigInvertedPair(i+2, &config);
		callback(i+2, 0.5f, &config);
		BSP_Dout_SetAsPWMPin(i);
		BSP_Dout_SetAsPWMPin(i+1);
		BSP_Dout_SetAsPWMPin(i+2);
		BSP_Dout_SetAsPWMPin(i+3);
	}
#endif

}

/**
 * @brief Call this function to run both inverter PWMs
 */
void MainControl_Run(void)
{
	BSP_PWM_Start(0xffff);
}

/**
 * @brief Call this function to stop the inverters
 */
void MainControl_Stop(void)
{
	BSP_PWM_Stop(0xffff);
}

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
#if RUN_ALL_FULL_BRIDGE == 0
		OpenLoopVfControl_Loop(&openLoopVfConfig1);
		OpenLoopVfControl_Loop(&openLoopVfConfig2);
#endif
		recompute = false;
	}
}

/* EOF */
