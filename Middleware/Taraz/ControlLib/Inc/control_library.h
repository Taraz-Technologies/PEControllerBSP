/**
 ********************************************************************************
 * @file 		ControlLib.h
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

#ifndef CONTROL_LIB_H_
#define CONTROL_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup Control_Library Control Library
 * @brief This library module contains the functionality and definitions for different control theories
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "transforms.h"
#include "dsp_library.h"
#include "pll.h"
#include "spwm.h"
#include "svpwm.h"
#include "inverter_3phase.h"
#include "open_loop_vf_controller.h"
#include "grid_tie_controller.h"
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

/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif 
/* EOF */
