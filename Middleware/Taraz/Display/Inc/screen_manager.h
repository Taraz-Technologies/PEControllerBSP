/**
 ********************************************************************************
 * @file 		screen_manager.h
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

#ifndef SCREEN_MANAGER_H_
#define SCREEN_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "adc_config.h"
#include "screen_data.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef void (*DisplayLayer)(ltdc_layer_info_t* layerInfo, int layerIdx);
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
 * @brief Initialize the screen manager
 * @note This function is automatically called by the BSP. No need to call this function externally
 * @param _dispLayer This function will be called to change the layer information
 * @param _adcInfo This pointer points to the ADC information structure
 */
void ScreenManager_Init(DisplayLayer _dispLayer ,adc_info_t* _adcInfo);
/**
 * @brief This function refreshes and manages the screens for display.
 */
void ScreenManager_Refresh(void);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
