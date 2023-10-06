/**
 ********************************************************************************
 * @file 		user_config.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 26, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief
 ********************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 Taraz Technologies Pvt. Ltd.</center></h2>
 * <h3><center>All rights reserved.</center></h3>
 *
 * <center>This software component is licensed by Taraz Technologies under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.opensource.org/licenses/BSD-3-Clause</center>
 *
 *******************************************************************************
 */

#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */
/** @defgroup user_config User Configuration
 * @brief Defines the user configurations for a specific application
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/

/********************************************************************************
 * Defines
 *******************************************************************************/
/********** SYSTEM CONFIGURATION *************/
#define PEC_CUSTOM				(0)
#define PLB_MMC					(1)
#define PLB_TNPC				(2)
#define PLB_6PH					(3)
#define PLB_3PH					(4)
/**
 * @brief Defines the Configuration for the PEController. Select @ref PEC_CUSTOM for independent PEControllers
 */
#define PECONTROLLER_CONFIG		(PLB_6PH)

#if PECONTROLLER_CONFIG != PEC_CUSTOM
/**
 * @brief Select the correct version of PELAB for your configuration
 */
#define PELAB_VERSION			(4)
#endif
/********** SYSTEM CONFIGURATION *************/

/********** DISPLAY CONFIGURATION ************/
#define LCD_AFY800480A0 		(0)
#define LCD_AFY800480B0			(1)
/**
 * @brief Select LCD variant installed on PEController
 */
#define DISPLAY					(LCD_AFY800480B0)
/**
 * @brief Selects the display brightness value between 0-100
 */
#define LCD_BRIGHTNESS			(100)
/********** DISPLAY CONFIGURATION ************/

/******** MEASUREMENT CONFIGURATION ***********/
/**
 * @brief Enable intelliSENS support for PEController
 * @note There is a separate license for enabling each device with intelliSENS.
 * If not already enabled contact support for a new license.
 * Otherwise if intelliSENS based monitoring is not required you can set this to 0 which will improve the LCD performance.
 */
#define ENABLE_INTELLISENS		(1)
/**
 * @brief Use this frequency when no control loop is enabled to get high bandwidth measurements. Max value is 100K.
 */
#define MONITORING_FREQUENCY_Hz		(40000)
/**
 * @brief Use this frequency when control loop is enabled to get low bandwidth measurements. Max value is 100K and is dependent upon the control performance.
 */
#define CONTROL_FREQUENCY_Hz		(40000)
/******** MEASUREMENT CONFIGURATION ***********/

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
