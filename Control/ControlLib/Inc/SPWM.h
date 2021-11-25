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
 * @details Following is the list of available functions
 * 	-# <b>@ref GenerateSPWM():</b> Adjust the duty cycles for Sinusoidal PWM
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "Transformations.h"
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
 * @brief Adjust the duty cycles for Sinusoidal PWM
 * @param theta Current angle of Phase A in radians
 * @param modulationIndex Modulation index for the PWM
 * @param *duties Resultant SPWM duty cycles.
 */
extern void GenerateSPWM(float theta, float modulationIndex, float* duties);
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
