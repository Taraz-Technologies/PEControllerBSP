/**
 ********************************************************************************
 * @file 		open_loop_vf_controller.h
 * @author 		Waqas Ehsan Butt
 * @date 		December 7, 2021
 *
 * @brief    
 ********************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 Taraz Technologies Pvt. Ltd.</center></h2>
 * <h3><center>All rights reserved.</center></h3>
 *
 * <center>This software component is licensed by Taraz Technologies under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the License. You may obtain 
 * a copy of the License at:
 *                        www.opensource.org/licenses/BSD-3-Clause</center>
 *
 ********************************************************************************
 */

#ifndef OPEN_LOOP_VF_CONTROLLER_H_
#define OPEN_LOOP_VF_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup ApplicationExamples Application Examples
 * @{
 */

/** @defgroup OpenLoopVf Open Loop V/f Example
 * @brief Describes the open loop v/f control implementation for the PEController
 * @details
 * This example controls one/multiple three phase inverter modules using the PWM signals of the PEController.
 * The three phase signal starts from the <b>INITIAL_FREQ</b> and gradually increases dependent upon the value of <b>ACCELERATION</b> to reach the <b>OUTPUT_FREQ</b>.
 * A constant V/f is maintained through out this process using the ratio <b>NOMINAL_FREQ</b> / <b>NOMINAL_MODULATION_INDEX</b>.
 * These configurations are available in the file <b>"open_loop_vf_config.h"</b><br><br>
 * The example can be used for the following configurations of PELab. Select your desired configuration from file <b>"user_config.h"</b>
 * PELab Configuration  | PWM Signals Inverter 1 | PWM Signals Inverter 2
 * -------------------- | ------------------- | -------------------
 * PLB-6PH  			| Leg 1: Q1, Q2<br>Leg 2: Q3, Q4<br>Leg 3: Q5, Q6<br> | Leg 1: Q7, Q8<br>Leg 2: Q9, Q10<br>Leg 3: Q11, Q12<br>
 * PLB-3PH  			| Leg 1: Q1, Q2<br>Leg 2: Q3, Q4<br>Leg 3: Q5, Q6<br> | -
 * PLB-MMC  			| Leg 1: Q1, Q2<br>Leg 2: Q3, Q4<br>Leg 3: Q5, Q6<br>Leg 4: Q7, Q8*<br> | Leg 1: Q9, Q10<br>Leg 2: Q11, Q12<br>Leg 3: Q13, Q14<br>Leg 4: Q15, Q16*<br>
 * PLB-TNPC  			| Leg 1: Q1, Q2, Q3, Q4<br>Leg 2: Q5, Q6, Q7, Q8<br>Leg 3: Q9, Q10, Q11, Q12<br>Leg 4: Q13, Q14, Q15, Q16*<br> | -
 * *<b>Leg 4</b> is a duplicate leg of leg 3 and is used in only some configurations to fully test all included power modules<br>
 * <br>
 * Initializes the algorithm using @ref OpenLoopVfControl_Init().
 * Once initialized constantly polls using @ref OpenLoopVfControl_Loop().
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "control_library.h"
#include "open_loop_vf_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup OPENLOOPVF_Exported_Structures Structures
  * @{
  */
/**
 * @brief Defines the parameters required by the open loop v/f controller
 */
typedef struct
{
	bool dir;								/**< @brief Direction of the three-phase signals */
	bool currentDir;						/**< @brief Current direction of the three-phase signals */
	inverter_state_t requestedState;		/**< @brief Contains the requested state for the inverter */
	float pwmFreq;							/**< @brief PWM frequency used by the inverter */
	float nominalFreq;						/**< @brief Nominal frequency is used to compute modulation index at different frequencies */
	float nominalModulationIndex;			/**< @brief Nominal modulation index is used to compute modulation index at different frequencies */
	float outputFreq;						/**< @brief Required output frequency at which the program needs to settle */
	float acceleration;						/**< @brief Acceleration with which the frequency should be changed */
	float wt;								/**< @brief Current angle of phase U used internally (measured in radians) */
	float currentFreq;						/**< @brief Current frequency output at the inverter. Computed internally */
	float currentModulationIndex;			/**< @brief Current modulation index output at the inverter. Computed internally */
	inverter3Ph_config_t inverterConfig;	/**< @brief Output inverter configuration */
} openloopvf_config_t;
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup OPENLOOPVF_Exported_Functions Functions
  * @{
  */
/**
 * @brief Initialize the grid tie controller
 * @param config Pointer to the configuration structure
 * @param pwmResetCallback Function callback issued after each PWM completion
 */
extern void OpenLoopVfControl_Init(openloopvf_config_t* config, PWMResetCallback pwmResetCallback);
/**
 * @brief This function computes new duty cycles for the inverter in each cycle
 * @param config Pointer to the inverter structure
 */
extern void OpenLoopVfControl_Loop(openloopvf_config_t* config);
/**
 * @brief Activate/Deactivate the inverter
 * @param config Pointer to the inverter structure
 * @param activate en <c>true</c> if needs to be enabled else <c>false</c>
 */
extern void OpenLoopVfControl_Activate(openloopvf_config_t* config, bool activate);
/********************************************************************************
 * Code
 *******************************************************************************/


/**
 * @}
 */
#ifdef __cplusplus
}
#endif
/**
 * @}
 */
/**
 * @}
 */
#endif 
/* EOF */
