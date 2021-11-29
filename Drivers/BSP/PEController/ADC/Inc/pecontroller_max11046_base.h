/**
 ********************************************************************************
 * @file 		pecontroller_max11046_base.h
 * @author 		Hamza Naeem Kakakhel
 * 				Waqas Ehsan Butt
 * @date 		Jun 10, 2021
 *
 * @brief	This file contains the basic definitions for the digital input pins
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
#ifndef PECONTTROLLER_MAX11046_BASE_H
#define PECONTTROLLER_MAX11046_BASE_H
#ifdef CORE_CM4
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdint.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define MAX_11046_GPIO GPIOF
#define MAX_11046_TIMER TIM12

#define TIMER_PULSE 480U

/*!
@brief callback for result submission in continuous mode
@param *result- pointer to the raw result data (u16)
 */
typedef void (*resultCallback)(uint16_t* result);

/*******************************************************************************
 * Enums
 ******************************************************************************/
/*! @brief max 11046 acquistion mode */
typedef enum _adc_acq_mode_
{
	ADC_MODE_SINGLE,
	ADC_MODE_CONT
} adc_acq_mode_t;
/*******************************************************************************
 * Structs
 ******************************************************************************/
typedef struct _adc_cont_config_
{
	int conversionCycleTimeUs;
	resultCallback callback;
} adc_cont_config_t;
 /*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif


/*!
@brief Initializes the raw max11046 module 
@param type- ADC_MODE_SINGLE or ADC_MODE_CONT for single or continuous conversions respectively
@param *contConfig- adc_cont_config_t contains the continuous transfer configuration
*/
void BSP_MAX11046_Base_Init(adc_acq_mode_t type, adc_cont_config_t* contConfig);
/*! @brief Deinitialize the max 11046 module */
void BSP_MAX11046_Base_DeInit(void);
/*! 
@brief Perform the conversion
@details For single conversion mode performs and provides the results in a blocking way.
For continuous conversion mode starts the conversions, after each conversion adc_cont_config_t.callback will be called
@returns uint16_t*- For single conversion mode returns the pointer to the eight acquired values. Returns NULL for continuous conversion mode
*/
uint16_t* BSP_MAX11046_Base_Run(void);
/*! @brief Stops the adc data collection module, only effective for ADC_MODE_CONT */
void BSP_MAX11046_Base_Stop(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(__cplusplus)
}
#endif
#endif
#endif
/* EOF */
/* Nothing beyond this*/




