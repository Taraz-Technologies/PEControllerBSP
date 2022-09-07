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
#include "grid_tie_controller.h"
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
/**
 * @brief Used to indicate the requirement of new duty cycle computations.
 */
static volatile bool recompute = false;
/**
 * @brief Grid Tie Control Parameters
 */
grid_tie_t gridTieConfig = {0};
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

	// configure the grid tie inverter parameters
	gridTieConfig.inverterConfig.s1PinNos[0] = 1;
	gridTieConfig.inverterConfig.s1PinNos[1] = 3;
	gridTieConfig.inverterConfig.s1PinNos[2] = 5;
	gridTieConfig.boostConfig[0].pinNo = 8;
	gridTieConfig.boostDiodePin[0] = 7;
	gridTieConfig.boostConfig[1].pinNo = 10;
	gridTieConfig.boostDiodePin[1] = 9;
	gridTieConfig.boostConfig[2].pinNo = 12;
	gridTieConfig.boostDiodePin[2] = 11;
	GridTieControl_Init(&gridTieConfig, Inverter3Ph_ResetSignal);
	gridTieConfig.iRef = 3;
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
		/* add the required measurements and current reference point */
		gridTieConfig.vCoor.abc.a = adcVals.V1;
		gridTieConfig.vCoor.abc.b = adcVals.V2;
		gridTieConfig.vCoor.abc.c = adcVals.V3;
		gridTieConfig.vdc = (adcVals.Vdc1);
		gridTieConfig.iCoor.abc.a = adcVals.Ie1;
		gridTieConfig.iCoor.abc.b = adcVals.Ie2;
		gridTieConfig.iCoor.abc.c = adcVals.Ie3;

		// implement the control
		GridTieControl_Loop(&gridTieConfig);
		recompute = false;
	}
}

/* EOF */
