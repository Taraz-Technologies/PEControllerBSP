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
#define LCD_AFY800480A0 	(0)
#define LCD_AFY800480B0		(1)
#define DISPLAY				(LCD_AFY800480A0)

#define PELAB_3PH			(0)
#define PELAB_6PH			(1)
#define PELAB_TNPC			(2)
#define PELAB_MMC			(3)
#define PELAB_CUSTOM		(4)
#define MEASUREMENT_UNIT	(PELAB_6PH)

#ifdef __cplusplus
}
#endif


#endif
/* EOF */
