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
	BSP_Dout_SetPortAsGPIO();
	BSP_Dout_SetPortValue(0);
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
