/**
 ********************************************************************************
 * @file 		pecontroller_display.h
 * @author 		Waqas Ehsan Butt
 * @date 		November 26, 2021
 *
 * @brief    Header file for the PEController display module
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

#ifndef PEONTROLLER_DISPLAY_H
#define PEONTROLLER_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @defgroup Display Display
 * @brief This module defines the display
 * @{
 */

/** @defgroup DisplayDriver Driver
 * @brief Contains the declaration and procedures for the display driver
 * @details List of functions
 * 	-# <b>@ref BSP_Display_Init()</b> Initializes the display module
 * 	-# <b>@ref BSP_Display_DeInit()</b> De-initializes the display module
 * 	-# <b>@ref BSP_Display_ShowLayer()</b> Shows the configured layer on the LCD
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
/** @defgroup PEDISPLAY_Exported_Functions Functions
  * @{
  */
/**
 * @brief Initializes the display module
 */
extern void BSP_Display_Init(void);
/**
 * @brief De-initializes the display module
 */
extern void BSP_Display_DeInit(void);
/**
 * @brief Shows the configured layer on the LCD
 * @param *pLayerCfg Layer configuration to be applied to the LTDC module
 */
extern void BSP_Display_ShowLayer(LTDC_LayerCfgTypeDef* pLayerCfg);
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
/**
 * @}
 */
#endif 
/* EOF */
