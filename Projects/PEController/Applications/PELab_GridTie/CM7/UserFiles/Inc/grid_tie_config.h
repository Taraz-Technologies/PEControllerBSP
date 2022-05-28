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
/**
 * @brief PWM Time Period in micro-seconds
 */
#define PWM_PERIOD_Us					(25)
/**
 * @brief Frequency of the grid in Hz
 */
#define GRID_FREQ						(50)
/**
 * @brief Series Output Inductance in Henry
 */
#define L_OUT							(.0025f)
/**
 * @brief Number of Boost Switches to control
 * @note The boost switches should start from @ref BOOST1_IO and use consecutive legs
 */
#define BOOST_COUNT						(2)
/**
 * @brief First boost switch
 */
#define BOOST1_IO						(10)
/**
 * @brief Set point voltage for Boost
 */
#define BOOST_VSET						(720)
#define BOOST_KP						(0.002f)
#define BOOST_KI						(0.1f)
#define KP_I							(60)
#define KI_I							(1000)
#define BOOST_DUTYCYCLE_MAX				(.95f)
/**
 * @brief Number of connected relays to grid
 * @note The relay IO should start from @ref GRID_RELAY_IO and use consecutive IOs after that
 */
#define GRID_RELAY_COUNT				(2)
/**
 * @brief First grid relay IO
 */
#define GRID_RELAY_IO					(15)

#define RELAY_TURN_ON_VBST				(620.f)
#define RELAY_TURN_OFF_VBST				(530.f)
#define INVERTER_DEADTIME_ns			(200)
#define MIN_MAX_BALANCING_INVERTER		(false)
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
