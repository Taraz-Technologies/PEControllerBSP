/**
 ********************************************************************************
 * @file 		DisplayDriver.h
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

#ifndef DISPLAYDRIVER_H_
#define DISPLAYDRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup Components
 * @{
 */

/** @defgroup DisplayDriver Display
 * @brief This module contains the base functionality and definitions for Display
 * @details List of functions
 * 	-# <b>@ref BSP_Display_Init()</b> Initializes the display module
 * 	-# <b>@ref BSP_Display_DeInit()</b> De-initializes the display module
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
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
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief Initializes the display module
 */
extern void BSP_Display_Init(void);
/**
 * @brief De-initializes the display module
 */
extern void BSP_Display_DeInit(void);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
