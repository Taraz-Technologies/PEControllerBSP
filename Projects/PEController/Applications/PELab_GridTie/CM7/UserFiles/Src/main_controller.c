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
#include "pecontroller_adc.h"
#include "main_controller.h"
#include "pecontroller_digital_in.h"
#include "shared_memory.h"
#include "pecontroller_timers.h"
#include "interprocessor_comms.h"
#include "grid_tie_controller.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Enums
 ******************************************************************************/
typedef enum
{
	ADC_MODE_MONITORING,
	ADC_MODE_CONTROL,
} adc_mode_t;
/*******************************************************************************
 * Structures
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/**
 * @brief Grid Tie Control Parameters
 */
grid_tie_t gridTieConfig = {0};
static adc_mode_t adcMode = ADC_MODE_MONITORING;
/*******************************************************************************
 * Code
 ******************************************************************************/
#if IS_ADC_CORE
static void ADC_Callback(adc_measures_t* result)
{
	if (boostStateUpdateRequest.isPending)
	{
		boostStateUpdateRequest.err = GridTie_EnableBoost(&gridTieConfig, boostStateUpdateRequest.state);
		boostStateUpdateRequest.isPending = false;
	}
	if (inverterStateUpdateRequest.isPending)
	{
		inverterStateUpdateRequest.err = GridTie_EnableInverter(&gridTieConfig, inverterStateUpdateRequest.state);
		inverterStateUpdateRequest.isPending = false;
	}
	// Switch between Monitoring and control mode
	if (gridTieConfig.isBoostEnabled || gridTieConfig.isInverterEnabled)
	{
		if (adcMode == ADC_MODE_MONITORING)
		{
			(void) BSP_ADC_Stop();
			// If timer 1 is used can be triggered based on timer 1
			tim_in_trigger_config_t _slaveConfig = { .type = TIM_TRGI_TYPE_RST, .src = TIM_TRG_SRC_TIM1 };
			(void)BSP_ADC_SetInputOutputTrigger(&_slaveConfig, NULL, CONTROL_FREQUENCY_Hz);
			(void) BSP_ADC_Run();
			adcMode = ADC_MODE_CONTROL;
		}
	}
	else
	{
		if (adcMode == ADC_MODE_CONTROL)
		{
			(void) BSP_ADC_Stop();
			(void)BSP_ADC_SetInputOutputTrigger(NULL, NULL, MONITORING_FREQUENCY_Hz);
			(void) BSP_ADC_Run();
			adcMode = ADC_MODE_MONITORING;
		}
	}
	MainControl_Loop(result);
}
#endif
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
	BSP_Dout_SetPortAsGPIO();
	BSP_Dout_SetPortValue(0);

	gridTieConfig.inverterConfig.s1PinNos[0] = 1;
	gridTieConfig.inverterConfig.s1PinNos[1] = 3;
	gridTieConfig.inverterConfig.s1PinNos[2] = 5;
	gridTieConfig.boostConfig[0].pinNo = 8;
	gridTieConfig.boostDiodePin[0] = 7;
	gridTieConfig.boostConfig[1].pinNo = 10;
	gridTieConfig.boostDiodePin[1] = 9;
	gridTieConfig.boostConfig[2].pinNo = 12;
	gridTieConfig.boostDiodePin[2] = 11;
	GridTieControl_Init(&gridTieConfig, NULL);

	MainControl_Run();

#if IS_ADC_CORE
	adc_cont_config_t adcConfig = {
			.callback = ADC_Callback,
			.fs = MONITORING_FREQUENCY_Hz };
	BSP_ADC_Init(ADC_MODE_CONT, &adcConfig, &RAW_ADC_DATA, &PROCESSED_ADC_DATA);
	(void) BSP_ADC_Run();
#endif
}

/**
 * @brief Call this function to start generating PWM signals
 */
void MainControl_Run(void)
{
	BSP_PWM_Start(0xffff, false);
}

/**
 * @brief Call this function to stop the control loop from generating PWM signals
 */
void MainControl_Stop(void)
{
	BSP_PWM_Stop(0xffff, false);
}

/**
 * @brief Call this function to process the control loop.
 * If the new computation request is available new duty cycle values are computed and applied to all inverter legs
 */
void MainControl_Loop(adc_measures_t* result)
{
	/* add the required measurements and current reference point */
	gridTieConfig.vCoor.abc.a = result->Ch13;
	gridTieConfig.vCoor.abc.b = result->Ch14;
	gridTieConfig.vCoor.abc.c = result->Ch15;
	gridTieConfig.vdc = result->Ch9;
	gridTieConfig.iCoor.abc.a = result->Ch1;
	gridTieConfig.iCoor.abc.b = result->Ch2;
	gridTieConfig.iCoor.abc.c = result->Ch3;

	// implement the control
	GridTieControl_Loop(&gridTieConfig);
}

/* EOF */
