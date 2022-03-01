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
static volatile bool recompute = false;
/**
 * @brief Grid Tie Control Parameters
 */
static grid_tie_t gridTieConfig = {0};
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
	gridTieConfig.boostConfig.pinNo = 12;
	gridTieConfig.boostDiodePin = 11;
	GridTieControl_Init(&gridTieConfig, Inverter3Ph_ResetSignal);
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
		/* add the required measurements and current reference point */
		gridTieConfig.vCoor.abc.a = adcVals.V1;
		gridTieConfig.vCoor.abc.b = adcVals.V2;
		gridTieConfig.vCoor.abc.c = adcVals.V3;
		gridTieConfig.vdc = (adcVals.Vdc1 + adcVals.Vdc2) / 2.f;
		gridTieConfig.vpv = adcVals.V4;
		gridTieConfig.idc = -adcVals.Ih3;
		gridTieConfig.iCoor.abc.a = adcVals.Ie1;
		gridTieConfig.iCoor.abc.b = adcVals.Ie2;
		gridTieConfig.iCoor.abc.c = adcVals.Ie3;

		// slow increase to iref for better response
		if (gridTieConfig.state == GRID_TIE_INACTIVE)
			gridTieConfig.iRef = .1f;
		else if (gridTieConfig.iRef < 1.f)
			gridTieConfig.iRef += .0001f;

		// implement the control
		GridTieControl_Loop(&gridTieConfig);
		recompute = false;
	}
}

/* EOF */
