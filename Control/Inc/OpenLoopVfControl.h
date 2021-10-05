/**
 ********************************************************************************
 * @file OpenLoopVfControl.h
 * @author Waqas Ehsan Butt
 * @date Oct 5, 2021
 * @copyright TarazTechnologies Pvt. Ltd.
 *
 * @brief
 ********************************************************************************
 */

#ifndef OPEN_LOOP_VF_CONTROL_H
#define OPEN_LOOP_VF_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/

/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	float pwmFreq;
	float nominalModulationIndex;
	float nominalFreq;
	float acceleration;
	float reqFreq;
	float freq;
	float theta;
	float modulationIndex;
} openloopvf_config_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
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
void OpenLoopVfControl_GetDuties(openloopvf_config_t* config, float* duties);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
