/**
 ********************************************************************************
 * @file 		screen_main_app.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 23, 2023
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

#ifndef SCREEN_MAIN_APP_H_
#define SCREEN_MAIN_APP_H_

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup Display
 * @{
 */

/** @defgroup AppScreensCustomization Application Screen Handling
 * @brief Use these macros and functions to customize the appearance of the main screen for every application independently.
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "screen_base.h"
#include "stdlib.h"
#include "utility_lib.h"
#include "interprocessor_comms.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup PEDISPLAYSCREEN_Exported_Macros Macros
 * @{
 */
/**
 * @brief Number of control parameters/configurations to be displayed on main screen
 * @note Combined with @ref CONTROL_COL_COUNT, this calculates the rows required by the control area and 
 * thus computes the required sizes for each controllable parameter
 */
#define CONTROL_CONFS_COUNT					(1)
/**
 * @brief Number of monitored parameters/configurations to be displayed on main screen
 * @note Combined with @ref MONITOR_COL_COUNT, this calculates the rows required by the monitoring area and 
 * thus computes the required sizes for each monitor-able parameter
 */
#define MONITOR_CONFS_COUNT					(0)
/**
 * @brief Number of columns in controls area
 */
#define CONTROL_COL_COUNT					(1)
/**
 * @brief Number of columns in monitoring area
 */
#define MONITOR_COL_COUNT					(1)

// Use only if forcing row layout <Not recommended>, otherwise these will be auto-computed.
#if 0
/**
 * @brief Number of rows in controls view
 */
#define CONTROL_ROW_COUNT					(2)
/**
 * @brief Number of rows in monitoring view
 */
#define MONITOR_ROW_COUNT					(2)
#endif
/**
 * @brief Ratio of control area
 */
#define CONTROL_AREA_RATIO					(3)
/**
 * @brief Ratio of monitoring area
 */
#define MONITOR_AREA_RATIO					(1)
/**
 * @brief Ratio of measurement area on the main screen
 */
#define MEASUREMENT_AREA_RATIO				(5)
/**
 * @brief Ratio of application specific area on the main screen
 */
#define APP_AREA_RATIO						(3)

/**
 * @brief Font used by the displayed measurement value
 */
#define MEASUREMENT_VALUE_FONT				(lv_font_montserrat_22)
/**
 * @brief Font used by the displayed control parameters
 */
#define CONTROL_VALUE_FONT					(lv_font_montserrat_40)
/**
 * @brief Font used by the displayed monitoring parameters
 */
#define MONITOR_VALUE_FONT					(lv_font_montserrat_18)
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
 /** @defgroup PEDISPLAYSCREEN_Exported_Functions Functions
  * @{
  */
/**
 * @brief Creates the application dependent area of the main screen.
 * @param parent Parent lv_obj_t of the device.
 * @param row Row index for the item.
 * @param col Column index for the item.
 */
extern void AppControlGrid_Create(lv_obj_t* parent, int row, int col);
/**
 * @brief Refreshes the application dependent area of the main screen.
 */
extern void AppControlGrid_Refresh(void);
/**
 * @brief Touch detection of the application dependent area of the screen.
 * @return A new screen type if screen needs to be changed based upon the touch response.
 */
extern screen_type_t AppControlGrid_TouchDetect(void);
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
