/**
 ********************************************************************************
 * @file 		LCD_AFY800480A0_B0.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 26, 2021
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

#ifndef LCD_AFY800480A0_B0_H_
#define LCD_AFY800480A0_B0_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup Components
 * @{
 */

/** @defgroup LCD_AFY800480A0_B0 LCD_AFY800480A0/B0
 * @brief This module contains the base functionality and definitions for LCDs LCD_AFY800480A0 & LCD_AFY800480B0
 * @{
 */

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
#ifndef DISPLAY
#error ("No appropriate LCD Module is selected!");
#endif
#if DISPLAY == LCD_AFY800480A0 || DISPLAY == LCD_AFY800480B0
/********************************************************************************
 * Defines
 *******************************************************************************/
#if DISPLAY == LCD_AFY800480A0
#define PLL3R_VAL			 (10)
#define	HORIZONTAL_SYNC 	 (6)
#define	VERTICAL_SYNC		 (6)
#define	ACCUMULATED_HBP		 (13)
#define	ACCUMULATED_VBP		 (13)
#define	ACCUMULATED_ACTIVE_W (813)
#define	ACCUMULATED_ACTIVE_H (493)
#define	TOTAL_WIDTH			 (819)
#define	TOTAL_HEIGHT		 (499)
#elif DISPLAY == LCD_AFY800480B0
#define PLL3R_VAL			 (7)
#define	HORIZONTAL_SYNC 	 (19)
#define	VERTICAL_SYNC		 (9)
#define	ACCUMULATED_HBP		 (65)
#define	ACCUMULATED_VBP		 (32)
#define	ACCUMULATED_ACTIVE_W (865)
#define	ACCUMULATED_ACTIVE_H (512)
#define	TOTAL_WIDTH			 (1075)
#define	TOTAL_HEIGHT		 (534)
#endif
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/
#if DISPLAY == LCD_AFY800480A0
const RCC_PeriphCLKInitTypeDef PLL3Clk =
{
		.PeriphClockSelection = RCC_PERIPHCLK_LTDC,
		.PLL3 = { .PLL3N = 21, .PLL3P = 2, .PLL3Q = 2, .PLL3RGE = RCC_PLL3VCIRANGE_3,
				.PLL3VCOSEL = RCC_PLL3VCOWIDE, .PLL3FRACN = 0, .PLL3R = 10 },
};
#elif DISPLAY == LCD_AFY800480B0
const RCC_PeriphCLKInitTypeDef PLL3Clk =
{
		.PeriphClockSelection = RCC_PERIPHCLK_LTDC,
		.PLL3 = { .PLL3N = 21, .PLL3P = 2, .PLL3Q = 2, .PLL3RGE = RCC_PLL3VCIRANGE_3,
				.PLL3VCOSEL = RCC_PLL3VCOWIDE, .PLL3FRACN = 0, .PLL3R = 7 },
};
#endif
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

#endif 
#endif
/* EOF */
