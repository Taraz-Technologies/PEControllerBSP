/**
 ********************************************************************************
 * @file 		SPWM.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 25, 2021
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

#ifndef SPWM_H_
#define SPWM_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Control_Library
 * @{
 */

/** @defgroup SPWM SPWM
 * @brief This module contains the functionality and definitions for SPWM generation
 * @details List of functions
 * 	-# <b>@ref ComputeDuty_SPWM() :</b> Get duty cycles of each leg using sinousidal PWM
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "transforms.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup SPWM_Exported_Macros Macros
  * @{
  */

/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup SPWM_Exported_Typedefs Type Definitions
  * @{
  */

/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup SPWM_Exported_Structures Structures
  * @{
  */

/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup SPWM_Exported_Variables Variables
  * @{
  */

/**
 * @}
 */
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup SPWM_Exported_Functions Functions
  * @{
  */
/**
 * @brief Get duty cycles of each leg using sinousidal PWM
 * @param theta Current angle of Phase A in radians
 * @param modulationIndex Modulation index for the PWM
 * @param *duties Pointer to the array where duty cycles need to be updated.
 */
extern void ComputeDuty_SPWM(float theta, float modulationIndex, float* duties);
/**
 * @}
 */
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

#endif 
/* EOF */
