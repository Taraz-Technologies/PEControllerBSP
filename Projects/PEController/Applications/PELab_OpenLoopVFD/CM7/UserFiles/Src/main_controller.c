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
#include "user_config.h"
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
openloopvf_config_t openLoopVfConfig1 = {0};
// Other PELab configurations don't support multiple inverter configurations
#if PECONTROLLER_CONFIG == PLB_6PH || PECONTROLLER_CONFIG == PLB_MMC
openloopvf_config_t openLoopVfConfig2 = {0};
#endif
static volatile bool recompute = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Initiates PWM for both inverters, and enable Disable signals and configure the relays
 */
void MainControl_Init(void)
{
	BSP_DigitalPins_Init();
#if PECONTROLLER_CONFIG == PLB_6PH || PECONTROLLER_CONFIG == PLB_3PH
	openLoopVfConfig1.inverterConfig.s1PinNos[0] = 1;
	openLoopVfConfig1.inverterConfig.s1PinNos[1] = 3;
	openLoopVfConfig1.inverterConfig.s1PinNos[2] = 5;
#ifdef PELAB_VERSION
#if	PELAB_VERSION < 4
	BSP_Dout_SetAsIOPin(13, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(14, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(15, GPIO_PIN_SET);
	BSP_Dout_SetAsIOPin(16, GPIO_PIN_SET);
#else
	BSP_Dout_SetAsIOPin(13, GPIO_PIN_SET);
	BSP_Dout_SetAsIOPin(14, GPIO_PIN_SET);
	BSP_Dout_SetAsIOPin(15, GPIO_PIN_SET);
	BSP_Dout_SetAsIOPin(16, GPIO_PIN_SET);
#endif
#endif
#elif PECONTROLLER_CONFIG == PLB_MMC
	openLoopVfConfig1.inverterConfig.s1PinNos[0] = 1;
	openLoopVfConfig1.inverterConfig.s1PinNos[1] = 3;
	openLoopVfConfig1.inverterConfig.s1PinNos[2] = 5;
	// Deactivate last leg as it is not used in the application
	BSP_Dout_SetAsIOPin(7, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(8, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(15, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(16, GPIO_PIN_RESET);
#elif PECONTROLLER_CONFIG == PLB_TNPC
	openLoopVfConfig1.inverterConfig.s1PinNos[0] = 1;
	openLoopVfConfig1.inverterConfig.s1PinNos[1] = 5;
	openLoopVfConfig1.inverterConfig.s1PinNos[2] = 9;
	// Deactivate the fourth leg as it is not required
	BSP_Dout_SetAsIOPin(13, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(14, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(15, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(16, GPIO_PIN_RESET);
#endif
	OpenLoopVfControl_Init(&openLoopVfConfig1, Inverter3Ph_ResetSignal);

// Other PELab configurations don't support multiple inverter configurations
#if PECONTROLLER_CONFIG == PLB_6PH
	openLoopVfConfig2.inverterConfig.s1PinNos[0] = 7;
	openLoopVfConfig2.inverterConfig.s1PinNos[1] = 9;
	openLoopVfConfig2.inverterConfig.s1PinNos[2] = 11;
	OpenLoopVfControl_Init(&openLoopVfConfig2, NULL);
#elif PECONTROLLER_CONFIG == PLB_MMC
	openLoopVfConfig2.inverterConfig.s1PinNos[0] = 9;
	openLoopVfConfig2.inverterConfig.s1PinNos[1] = 11;
	openLoopVfConfig2.inverterConfig.s1PinNos[2] = 13;
	OpenLoopVfControl_Init(&openLoopVfConfig2, NULL);
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
		OpenLoopVfControl_Loop(&openLoopVfConfig1);
// Other PELab configurations don't support multiple inverter configurations
#if PECONTROLLER_CONFIG == PLB_6PH || PECONTROLLER_CONFIG == PLB_MMC
		OpenLoopVfControl_Loop(&openLoopVfConfig2);
#endif
		recompute = false;
	}
}

/* EOF */
