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
#define PECONTROLLER_CONFIG		(PLB_MMC)

#if PECONTROLLER_CONFIG != PEC_CUSTOM
/**
 * @brief Select the correct version of PELAB for your configuration
 */
#define PELAB_VERSION			(4)
/**
 * @brief For correct measurement sensitivity of channels in mV/A should be provided here
 */
#define CURRENT_SENSITIVITY_mVA	(400)
#endif
/********** SYSTEM CONFIGURATION *************/

/********** DISPLAY CONFIGURATION ************/
#define LCD_AFY800480A0 		(0)
#define LCD_AFY800480B0			(1)
/**
 * @brief Select LCD variant installed on PEController
 */
#define DISPLAY					(LCD_AFY800480B0)

#define LOGO_DEFAULT			(0)
#define LOGO_CUSTOM				(1)
/**
 * @brief Select the LOGO to be displayed on PEController
 */
#define LOGO_TYPE				(LOGO_DEFAULT)

/**
 * @brief Selects the display brightness value between 0-100
 */
#define LCD_BRIGHTNESS			(100)
/********** DISPLAY CONFIGURATION ************/

/******** MEASUREMENT CONFIGURATION ***********/
/**
 * @brief Enable intelliSENS support for PEController
 * @note Make sure that the binary intelliSENS.elf is also loaded in run configuration and correct linker file is selected
 */
#define ENABLE_INTELLISENS		(1)
/**
 * @brief Sampling Time in micro-seconds
 * @note Minimum setting value is 10 (100KHz) with intelliSENS but may vary if additional logic is added.
 * Minimum setting value is 8 (125KHz) without intelliSENS software
 */
#define SAMPLING_TIME_US		(20)
/******** MEASUREMENT CONFIGURATION ***********/

#ifdef __cplusplus
}
#endif


#endif
/* EOF */
