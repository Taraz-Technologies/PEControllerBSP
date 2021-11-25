/**
 ********************************************************************************
 * @file 		Compensators.h
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

#ifndef COMPENSATORS_H_
#define COMPENSATORS_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Control_Library
 * @{
 */

/** @defgroup Compensators Compensators
 * @brief This module contains the functionality and definitions for different compensators
 * @details Following is the list of available compensators
 * 	-# <b>Discreet PI Compensator</b> pi_compensator_t
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "GeneralHeader.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#if MONITOR
/**
 * @brief Set to 1 to enable monitoring of PI compensator in debug window
 */
#define MONITOR_PI					(0)
#endif
/**
 * @brief Enable this macro if PI compensator should possess capability of limiting the
 * output and integral portion
 * @note If 1, PI result will be limited by max and min values defined in @ref pi_compensator_t
 * if has_lmt is true, otherwise the result will not be limited
 */
#define PI_COMPENSATOR_LIMIT_CAPABLE		(1)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/**
 * @brief Defines the parameters used by a PI compensator
 */
typedef struct
{
#if PI_COMPENSATOR_LIMIT_CAPABLE
	bool has_lmt;		/**< @brief Enable/disable the limiting capability for the PI compensator */
	float max;			/**< @brief Maximum value limit for the PI compensator */
	float min;			/**< @brief Minimum value limit for the PI compensator */
#endif
	float Kp;			/**< @brief Kp parameter for the PI compensator */
	float Ki;			/**< @brief Ki parameter for the PI compensator */
	float dt;			/**< @brief Time interval in seconds for the PI compensator */
	float Integral;		/**< @brief Integral term for the PI compensator. Should be zero at startup */
#if MONITOR_PI
	float err;			/**< @brief Variable for monitoring the instantaneous error while debugging */
	float result;		/**< @brief Variable for monitoring the instantaneous result while debugging */
#endif
} pi_compensator_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief Evaluates the result for the PI compensation
 * @param *pi Pointer to the PI compensator structure
 * @param err Current value of error
 * @return float Result of the PI compensation of current cycle
 */
extern float PI_Compensate(pi_compensator_t* pi, float err);
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
