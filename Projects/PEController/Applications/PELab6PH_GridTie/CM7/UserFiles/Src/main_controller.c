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
#include "grid_tie_controller.h"
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
extern HRTIM_HandleTypeDef hhrtim;
extern TIM_HandleTypeDef htim1;;
pwm_config_t pwmConfig = {0};
/**
 * @brief Grid Tie Control Parameters
 */
static grid_tie_t gridTieConfig = {0};
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Initiates PWM for both inverters, and enable Disable signals and configure the relays
 */
void MainControl_Init(void)
{
	BSP_DigitalPins_Init();


	BSP_Dout_SetAsIOPin(7, GPIO_PIN_RESET);		// should be zero because this switch behaves as a diode

	gridTieConfig.inverterConfig.s1PinNos[0] = 1;
	gridTieConfig.inverterConfig.s1PinNos[1] = 3;
	gridTieConfig.inverterConfig.s1PinNos[2] = 5;
	gridTieConfig.inverterConfig.dsblPinCount = 0;
	gridTieConfig.boostConfig.pinNo = 8;
	GridTieControl_Init(&gridTieConfig, Inverter3Ph_ResetSignal);
	// turn off relays and disable pins
	BSP_Dout_SetAsIOPin(13, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(14, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(15, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(16, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(11, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(12, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(9, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(10, GPIO_PIN_RESET);

	/*
	pwmConfig.dutyMode = OUTPUT_DUTY_AT_PWMH;
	pwmConfig.lim.min = 0;
	pwmConfig.lim.max = 0.6;
	pwmConfig.lim.minMaxDutyCycleBalancing = false;
	static pwm_module_config_t moduleConfig = {0};
	pwmConfig.module = &moduleConfig;
	pwmConfig.module->alignment = CENTER_ALIGNED;
	pwmConfig.module->deadtime.nanoSec = 1000;
	pwmConfig.module->deadtime.on = true;
	pwmConfig.module->periodInUsec = 40;
	BSP_PWM_ConfigInvertedPair(1, &pwmConfig);
	BSP_PWM_UpdatePairDuty(1, .3f, &pwmConfig);
	BSP_Dout_SetAsPWMPin(1);
	BSP_Dout_SetAsPWMPin(2);
	*/

	/*
	pwmConfig.dutyMode = OUTPUT_DUTY_MINUS_DEADTIME_AT_PWMH;
	pwmConfig.lim.min = 0;
	pwmConfig.lim.max = .6f;
	pwmConfig.lim.minMaxDutyCycleBalancing = true;
	static pwm_module_config_t moduleConfig = {0};
	pwmConfig.module = &moduleConfig;
	pwmConfig.module->alignment = CENTER_ALIGNED;
	pwmConfig.module->deadtime.nanoSec = 1000;
	pwmConfig.module->deadtime.on = true;
	pwmConfig.module->periodInUsec = 40;
	BSP_PWM_ConfigChannel(12, &pwmConfig);
	BSP_PWM_UpdateChannelDuty(12, .8f, &pwmConfig);
	BSP_Dout_SetAsPWMPin(12);
	BSP_Dout_SetAsPWMPin(11);
	//BSP_Dout_SetAsIOPin(11, GPIO_PIN_RESET);
	*/

}

/**
 * @brief Call this function to run both inverter PWMs
 */
void MainControl_Run(void)
{
	BSP_PWM_Start(0x0Bf);
	//BSP_PWM_Start(0x083f);
}

/**
 * @brief Call this function to stop the inverters
 */
void MainControl_Stop(void)
{
	BSP_PWM_Stop(0x0Bf);
	//BSP_PWM_Stop(0x083f);
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
		/* add the required measurements and current reference point */
		gridTieConfig.vCoor.abc.a = adcVals.V1;
		gridTieConfig.vCoor.abc.b = adcVals.V2;
		gridTieConfig.vCoor.abc.c = adcVals.V3;
		gridTieConfig.vdc = adcVals.Vdc1;
		gridTieConfig.iCoor.abc.a = adcVals.Ih1;
		gridTieConfig.iCoor.abc.b = adcVals.Ih2;
		gridTieConfig.iCoor.abc.c = adcVals.Ih3;

		if (gridTieConfig.state == GRID_TIE_INACTIVE)
			gridTieConfig.iRef = .1f;
		else if (gridTieConfig.iRef < 5.f)
			gridTieConfig.iRef += .0001f;

		GridTieControl_Loop(&gridTieConfig);
		recompute = false;
	}
}

/* EOF */
