/**
 ********************************************************************************
 * @file 		dsp_library.h
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

#ifndef DSP_LIBRARY_H_
#define DSP_LIBRARY_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Control_Library
 * @{
 */

/** @defgroup DSP_Library DSP Library
 * @brief Contains the declaration and procedures for Digital Signal Processing Library
 * @details The following digital signal processing units are currently available in this library
 * 	-# <b>Moving Average Filter:</b> @ref mov_avg_t defines the filter unit. Use @ref MovingAverage_Compute()
 * 									to compute the moving average and @ref MovingAverage_Reset() to reset the filter
 * 	-# <b>PI Compensator:</b> @ref pi_compensator_t defines the compensator unit. Use @ref PI_Compensate()
 * 									to compute the value for the compensation
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup DSPLib_Exported_Macros Macros
  * @{
  */
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
/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup DSPLib_Exported_Structures Structures
  * @{
  */
/**
 * @brief Defines the parameters used by a moving average filter
 */
typedef struct
{
	float avg;		/**< @brief Mean observed in samples */
	bool stable;	/**< @brief If data is complete for moving average computation this value will be <c>true</c> else <c>false</c> */
	float* dataPtr;	/**< @brief Pointer to the data array */
	int count;		/**< @brief No of samples per computation */
	int index;		/**< @brief Initialize to zero. Used internally for detecting current data position */
} mov_avg_t;
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
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @addtogroup DSPLib_Exported_Functions Functions
  * @{
  */
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
