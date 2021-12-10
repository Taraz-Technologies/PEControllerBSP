/**
 ********************************************************************************
 * @file 		grid_tie_config.h
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

#ifndef GRID_TIE_CONFIG_H_
#define GRID_TIE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup PELab6PH_GridTie
 * @{
 */

/** @addtogroup PELab6PH_GridTie_CM7
 * @{
 */

/** @addtogroup PELab6PH_GridTie_CM7_Configs
 * @{
 */

/** @defgroup PELab6PH_GridTie_CM7_ControlConfigs Control Configurations
 * @brief Defines the configurations used by the control system
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/

/********************************************************************************
 * Defines
 *******************************************************************************/
#define PWM_PERIOD_Us					(40)
#define PWM_PERIOD_s					(PWM_PERIOD_Us/1000000.f)
#define PWM_FREQ_KHz					(1000.f/PWM_PERIOD_Us)
#define PWM_FREQ_Hz						(1.f/PWM_PERIOD_s)
#define GRID_RELAY_IO					(15)
#define GRID_FREQ						(50)
#define L_OUT							(.0025f)
#define RELAY_TURN_ON_VBST				(650.f)
#define RELAY_TURN_OFF_VBST				(550.f)
#define VBST_SET						(720.f)
#define BOOST_DUTYCYCLE_MAX				(.5f)
#define INVERTER_DEADTIME_ns			(500)
#define MIN_MAX_BALANCING_INVERTER		(true)
#define INVERTER_DUTY_MODE				OUTPUT_DUTY_AT_PWMH

#define PLL_FILT_SIZE					(8)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */

#endif 
/* EOF */
