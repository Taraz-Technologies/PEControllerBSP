/**
 ********************************************************************************
 * @file 		screen_base.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 17, 2023
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

#ifndef SCREEN_BASE_H_
#define SCREEN_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup Display
 * @{
 */

/** @addtogroup DisplayScreens
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "pecontroller_display.h"
#include "screen_data.h"
#include "screen_styles.h"
#include "app_image_dictionary.h"
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
/** @defgroup PEDISPLAYSCREEN_Exported_Functions Functions
  * @{
  */
/**
 * @brief Display a message on screen
 * @param _titleTxt Title of the message
 * @param _msgTxt Content of the message
 */
extern void DisplayMessage(const char* _titleTxt, const char* _msgTxt);
/**
 * @brief Load the relevant fields and configurations for the desired measurement channel
 * @note Make sure to call this function before switching to the configuration screen
 * @param _measurementIndex Channel index for which the measurement configuration is required. Starts from index 0
 */
extern void ConfigScreen_LoadMeasurement(int _measurementIndex);
/**
 * @brief Load the relevant fields and configurations for the desired parameter
 * @note Make sure to call this function before switching to the configuration screen
 * @param _paramInfo Pointer to the desired parameter to be configured.
 * @param val Current value of the parameter
 */
extern void ConfigScreen_LoadParam(data_param_info_t* _paramInfo, char* val);
extern void ConfigScreen_LoadSettings(data_param_group_t* _paramGroups, int groupCount);
extern void ScreenEvents_Handler(lv_event_t * e);
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
