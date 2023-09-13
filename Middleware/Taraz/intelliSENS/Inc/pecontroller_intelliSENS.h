/**
 ********************************************************************************
 * @file 		intelliSENS_drivers.h
 * @author 		Waqas Ehsan Butt
 * @date 		Dec 28, 2021
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

#ifndef _PECONTROLLER_INTELLISENS_H
#define _PECONTROLLER_INTELLISENS_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @defgroup intelliSENS intelliSENS
 * @brief This module contains the procedure to connect with the intelliSENS drivers for
 * sending data to the PC for monitoring purposes.
 * @details In the main file call the @ref intelliSENS_Init() function.
 * The function @ref intelliSENS_GetLicenseNumberString() gets the license information
 * for the PEController.
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
/** @defgroup intelliSENS_Exported_Functions Functions
  * @{
  */
/**
 * @brief Initialize the intelliSENS library
 * @note <b>The following resources are used by the intelliSENS library. Make sure that they are not used in the main program.
 * -# DMA2_Stream0
 * -# LPTIM2
 * -# MDMA_Channel0 (Only in-case of CM4 core) </b>
 * @param _mults Pointer to the multipliers for the ADC.
 * @param _offsets Pointer to the offsets for the ADC.
 * @param _dataCenter Pointer to the data center array. The array should have 2^n members.
 * @param _dataCenterSize Size of the data center array. The array should contain 2^n elements where n is a positive integer.
 * @param _ticks Number of ticks for ADC.
 * @param _wrIndex Pointer to the write index for data center array.
 */
extern void intelliSENS_Init(float* _mults, float* _offsets, uint16_t* _dataCenter, int _dataCenterSize, uint16_t _ticks, volatile int* _wrIndex);
/**
 * @brief Provides the license number of the device in string format
 * @return Pointer to the license number string
 */
extern const char* intelliSENS_GetLicenseNumberString(void);
/********************************************************************************
 * Code
 *******************************************************************************/

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
