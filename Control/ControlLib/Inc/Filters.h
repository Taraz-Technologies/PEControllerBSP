/**
 ********************************************************************************
 * @file 		Filters.h
 * @author 		Waqas Ehsan Butt
 * @date 		July 10, 2021
 *
 * @brief	This file contains the functionality and definitions for different filters
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
#ifndef FILTERS_H
#define FILTERS_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Control_Library
 * @{
 */

/** @defgroup Filters DSP Filters
 * @brief This module contains the functionality and definitions for different filters
 * @details Following is the list of available filters
 * 	-# <b>Moving Average Filter</b> mov_avg_t
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/

/*******************************************************************************
 * Structures
 ******************************************************************************/
/**
 * @brief Defines the parameters used by a moving average filter
 */
typedef struct
{
	float avg;		/**< @brief Mean observed in samples */
	bool stable;	/**< @brief If the average is not for full count this will be false */
	float* dataPtr;	/**< @brief Pointer to the data array */
	int count;		/**< @brief No of samples per computation */
	int index;		/**< @brief Initialize to zero. Used internally for detecting current data position */
} mov_avg_t;
 /*******************************************************************************
 * Exported Variables
 ******************************************************************************/

/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/**
 * @brief Computes the moving average
 * @param *filt Pointer to the filter
 * @param val Current value
 * @return float Resultant value of the moving average filter
 */
extern float MovingAverage_Compute(mov_avg_t* filt, float val);
/**
 * @brief Resets the moving average filter
 * @param *filt Pointer to the filter
 */
extern void MovingAverage_Reset(mov_avg_t* filt);
/*******************************************************************************
 * Code
 ******************************************************************************/


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
