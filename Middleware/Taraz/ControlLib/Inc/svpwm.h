/**
 ********************************************************************************
 * @file 		SVPWM.h
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

#ifndef SVPWM_H_
#define SVPWM_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Control_Library
 * @{
 */

/** @defgroup SVPWM SVPWM
 * @brief Contains the declaration and procedures for SVPWM generation
 * @details List of functions
 * 	-# <b>@ref ComputeDuty_SVPWM_FromAlBe0() :</b> Get duty cycles of each leg using space vector PWM from LIB_3COOR_ALBE0_t
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "transforms.h"
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
/** @defgroup SVPWM_Exported_Functions Functions
  * @{
  */
/**
 * @brief Get duty cycles of each leg using space vector PWM from LIB_3COOR_ALBE0_t
 * @param *alBe0 Alpha Beta Coordinates
 * @param *duties Pointer to the array where duty cycles need to be updated
 */
extern void ComputeDuty_SVPWM_FromAlBe0(LIB_3COOR_ALBE0_t *alBe0, float* duties);
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


#endif 
/* EOF */
