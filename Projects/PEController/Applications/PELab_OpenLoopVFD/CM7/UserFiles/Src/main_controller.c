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
#include "open_loop_vf_controller.h"
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
openloopvf_config_t openLoopVfConfig1 = {0};
#if VFD_COUNT == 2
openloopvf_config_t openLoopVfConfig2 = {0};
#endif
static adc_mode_t adcMode = ADC_MODE_MONITORING;
/*******************************************************************************
 * Code
 ******************************************************************************/
#if IS_ADC_CORE
static void ADC_Callback(adc_measures_t* result)
{
	if (inv1StateUpdateRequest.isPending)
	{
		OpenLoopVfControl_Activate(&openLoopVfConfig1, inv1StateUpdateRequest.state);
		INTER_CORE_DATA.bools[SHARE_INV1_STATE] = inv1StateUpdateRequest.state;
		inv1StateUpdateRequest.err = ERR_OK;
		inv1StateUpdateRequest.isPending = false;
	}
	if (inv2StateUpdateRequest.isPending)
	{
#if VFD_COUNT == 2
		OpenLoopVfControl_Activate(&openLoopVfConfig2, inv2StateUpdateRequest.state);
#endif
		INTER_CORE_DATA.bools[SHARE_INV2_STATE] = inv2StateUpdateRequest.state;
		inv2StateUpdateRequest.err = ERR_OK;
		inv2StateUpdateRequest.isPending = false;
	}
	// Switch between Monitoring and control mode
	if (openLoopVfConfig1.inverterConfig.state == INVERTER_ACTIVE
#if VFD_COUNT == 2
			|| openLoopVfConfig2.inverterConfig.state == INVERTER_ACTIVE
#endif
			)
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
	MainControl_Loop();
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

	openLoopVfConfig1.inverterConfig.s1PinNos[0] = VFD1_PIN1;
	openLoopVfConfig1.inverterConfig.s1PinNos[1] = openLoopVfConfig1.inverterConfig.s1PinNos[0] + LEG_SWITCH_COUNT;
	openLoopVfConfig1.inverterConfig.s1PinNos[2] = openLoopVfConfig1.inverterConfig.s1PinNos[1] + LEG_SWITCH_COUNT;
#if HAS_DUPLICATE_SWITCH
	openLoopVfConfig1.inverterConfig.s1PinDuplicate = openLoopVfConfig1.inverterConfig.s1PinNos[2] + LEG_SWITCH_COUNT;
#endif
	openLoopVfConfig1.nominalFreq = INTER_CORE_DATA.floats[SHARE_INV1_NOM_FREQ];
	openLoopVfConfig1.nominalModulationIndex = INTER_CORE_DATA.floats[SHARE_INV1_NOM_m];
	openLoopVfConfig1.outputFreq = INTER_CORE_DATA.floats[SHARE_INV1_REQ_FREQ];
	openLoopVfConfig1.acceleration = INTER_CORE_DATA.floats[SHARE_INV1_ACCELERATION];
	openLoopVfConfig1.currentDir = openLoopVfConfig1.dir = INTER_CORE_DATA.bools[SHARE_INV1_DIRECTION];
	OpenLoopVfControl_Init(&openLoopVfConfig1, NULL);

#if VFD_COUNT == 2
	openLoopVfConfig2.inverterConfig.s1PinNos[0] = VFD2_PIN1;
	openLoopVfConfig2.inverterConfig.s1PinNos[1] = openLoopVfConfig2.inverterConfig.s1PinNos[0] + LEG_SWITCH_COUNT;
	openLoopVfConfig2.inverterConfig.s1PinNos[2] = openLoopVfConfig2.inverterConfig.s1PinNos[1] + LEG_SWITCH_COUNT;
#if HAS_DUPLICATE_SWITCH
	openLoopVfConfig2.inverterConfig.s1PinDuplicate = openLoopVfConfig2.inverterConfig.s1PinNos[2] + LEG_SWITCH_COUNT;
#endif
	openLoopVfConfig2.nominalFreq = INTER_CORE_DATA.floats[SHARE_INV2_NOM_FREQ];
	openLoopVfConfig2.nominalModulationIndex = INTER_CORE_DATA.floats[SHARE_INV2_NOM_m];
	openLoopVfConfig2.outputFreq = INTER_CORE_DATA.floats[SHARE_INV2_REQ_FREQ];
	openLoopVfConfig2.acceleration = INTER_CORE_DATA.floats[SHARE_INV2_ACCELERATION];
	openLoopVfConfig2.currentDir = openLoopVfConfig2.dir = INTER_CORE_DATA.bools[SHARE_INV2_DIRECTION];
	OpenLoopVfControl_Init(&openLoopVfConfig2, NULL);
#endif

#if RELAY_COUNT > 0
	BSP_Dout_SetAsIOPin(15, GPIO_PIN_SET);
	BSP_Dout_SetAsIOPin(16, GPIO_PIN_SET);
#if RELAY_COUNT == 4
	BSP_Dout_SetAsIOPin(13, GPIO_PIN_SET);
	BSP_Dout_SetAsIOPin(14, GPIO_PIN_SET);
#endif
#endif

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
void MainControl_Loop(void)
{
	openLoopVfConfig1.nominalFreq = INTER_CORE_DATA.floats[SHARE_INV1_NOM_FREQ];
	openLoopVfConfig1.outputFreq = INTER_CORE_DATA.floats[SHARE_INV1_REQ_FREQ];
	openLoopVfConfig1.nominalModulationIndex = INTER_CORE_DATA.floats[SHARE_INV1_NOM_m];
	openLoopVfConfig1.acceleration = INTER_CORE_DATA.floats[SHARE_INV1_ACCELERATION];
	openLoopVfConfig1.dir = INTER_CORE_DATA.bools[SHARE_INV1_DIRECTION];
	OpenLoopVfControl_Loop(&openLoopVfConfig1);
	INTER_CORE_DATA.floats[SHARE_INV1_FREQ] = openLoopVfConfig1.currentFreq;
	INTER_CORE_DATA.floats[SHARE_INV1_m] = openLoopVfConfig1.currentModulationIndex;
#if VFD_COUNT == 2
	openLoopVfConfig2.nominalFreq = INTER_CORE_DATA.floats[SHARE_INV2_NOM_FREQ];
	openLoopVfConfig2.outputFreq = INTER_CORE_DATA.floats[SHARE_INV2_REQ_FREQ];
	openLoopVfConfig2.nominalModulationIndex = INTER_CORE_DATA.floats[SHARE_INV2_NOM_m];
	openLoopVfConfig2.acceleration = INTER_CORE_DATA.floats[SHARE_INV2_ACCELERATION];
	openLoopVfConfig2.dir = INTER_CORE_DATA.bools[SHARE_INV2_DIRECTION];
	OpenLoopVfControl_Loop(&openLoopVfConfig2);
	INTER_CORE_DATA.floats[SHARE_INV2_FREQ] = openLoopVfConfig2.currentFreq;
	INTER_CORE_DATA.floats[SHARE_INV2_m] = openLoopVfConfig2.currentModulationIndex;
#endif
}

/* EOF */
