/**
 ********************************************************************************
 * @file 		screen_data_app.h
 * @author 		Waqas Ehsan Butt
 * @date 		Jun 6, 2023
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

#ifndef SCREEN_DATA_APP_H_
#define SCREEN_DATA_APP_H_

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */
/** @addtogroup Display
 * @{
 */
/** @defgroup DisplayScreens Screens
 * @brief Contains the declaration and procedures for the display screens
 * @{
 */
/** @defgroup Data Data
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "image_dictionary.h"
#include "screen_base.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup PEDISPLAYSCREEN_Exported_Macros Macros
 * @{
 */
/**
 * @brief Number of setting groups to be displayed. Size of @ref settingWindows
 */
 #define SETTINGS_WINDOW_COUNT			(1)
/**
  * @}
  */
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
