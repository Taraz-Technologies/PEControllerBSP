/**
 ********************************************************************************
 * @file   OpenLoopVfControl.c
 * @author Waqas Ehsan Butt
 * @date    Oct 5, 2021
 * @copyright TarazTechnologies Pvt. Ltd.
 *
 * @brief   Open Loop V/f Control
 ********************************************************************************
 */
#pragma GCC push_options
#pragma GCC optimize ("O3")
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "GeneralHeader.h"
#include "Control.h"
#include "OpenLoopVfControl.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/

/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Open loop V/f control implementation
 *
 * @param *config pointer to the control parameters
 * @param *duties output duty cycles for all 3 phases (Range 0 - 1)
 *
 * @description Computes the duty cycle for the next cycle.
 * 				Here the frequency starts from the initial value and keeps increasing till
 * 				it reaches the reqFreq value with constant acceleration.
 * 				The modulation index is acquired by using the nominal frequency and modulation
 * 				index along with the current frequency to apply simple open loop V/F control
 */
void OpenLoopVfControl_GetDuties(openloopvf_config_t* config, float* duties)
{
	// adjust the frequency with given acceleration
	if(config->freq < config->reqFreq)
	{
		config->freq *= config->acceleration;
		if(config->freq > config->reqFreq)
			config->freq = config->reqFreq;
	}
	if(config->freq > config->reqFreq)
	{
		config->freq /= config->acceleration;
		if(config->freq < config->reqFreq)
			config->freq = config->reqFreq;
	}

	// compute the current modulation index
	config->modulationIndex = (config->nominalModulationIndex / config->nominalFreq) * config->freq;
	float stepSize = (TWO_PI * config->freq) / config->pwmFreq;
	config->theta += stepSize;
	if(config->theta > TWO_PI)
		config->theta -= TWO_PI;

	// generate SPWM according to the theta and modulation index
	GenerateSPWM(config->theta, config->modulationIndex, duties);
}

#pragma GCC pop_options
/* EOF */