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
#include "p2p_comms.h"
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
/**
 * @brief Call this function to process the control loop.
 * @param result ADC conversion data
 */
static void MainControl_Loop(adc_measures_t* result);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static adc_mode_t adcMode = ADC_MODE_MONITORING;
/*******************************************************************************
 * Code
 ******************************************************************************/
#if IS_ADC_CORE
static void ADC_Callback(adc_measures_t* result)
{
	if (modeChangeRequest.isPending)
	{
		INTER_CORE_DATA.bools[P2P_CONTROL_STATE] = modeChangeRequest.state;
		modeChangeRequest.err = ERR_OK;
		modeChangeRequest.isPending = false;
	}
	// Switch between Monitoring and control mode
	if (INTER_CORE_DATA.bools[P2P_CONTROL_STATE])
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
 * @param result ADC conversion data
 */
static void MainControl_Loop(adc_measures_t* result)
{

}

/* EOF */
