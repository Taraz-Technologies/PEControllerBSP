/**
 ********************************************************************************
 * @file 		max11046_drivers.h
 * @author 		Waqas Ehsan Butt
 * @date 		November 30, 2021
 *
 * @brief    Controls the max11046 functionality
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

#ifndef MAX11046_DRIVERS_H_
#define MAX11046_DRIVERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup ADC
 * @{
 */

/** @defgroup MAX11046 Max11046
 * @brief Contains the declaration and procedures for MAX11046 ADC.
 * @details List of functions
 * 	-# <b>@ref BSP_MAX11046_Init() :</b> Initializes the MAX11046 drivers.
 * 	-# <b>@ref BSP_MAX11046_DeInit() :</b> De-initialize the MAX11046 drivers.
 * 	-# <b>@ref BSP_MAX11046_Run() :</b> Performs the conversion.
 * 	-# <b>@ref BSP_MAX11046_Stop() :</b> Stops the ADC data collection module, only effective for ADC_MODE_CONT.
 * 	-# <b>@ref BSP_MAX11046_SetInputOutputTrigger() :</b> Sets the input and output trigger functions for the ADC.
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#if MAX11046_ENABLE && IS_ADC_CORE
#include "pecontroller_adc.h"
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
/** @defgroup MAX11046_Exported_Functions Functions
 * @{
 */
/**
 * @brief Initializes the MAX11046 drivers
 * @param type ADC_MODE_SINGLE or ADC_MODE_CONT for single or continuous conversions respectively
 * @param contConfig adc_cont_config_t contains the continuous transfer configuration
 * @param rawAdcData Pointer to the raw ADC data container
 * @param processedAdcData Pointer to the processed ADC data container
 */
extern void BSP_MAX11046_Init(adc_acq_mode_t type, adc_cont_config_t* contConfig, volatile adc_raw_data_t* rawAdcData, volatile adc_processed_data_t* processedAdcData);
/**
 * @brief Starts the ADC conversion
 * @details For single conversion mode performs and provides the results in a blocking way.
 * For continuous conversion mode starts the conversions, after each conversion adc_cont_config_t.callback will be called
 * @return adc_measures_t_t* For single conversion mode returns the pointer to the acquired values. Returns NULL for continuous conversion mode
 */
extern adc_measures_t* BSP_MAX11046_Run(void);
/**
 * @brief Stops the ADC data collection module, only effective for ADC_MODE_CONT
 */
extern void BSP_MAX11046_Stop(void);
/**
 * @brief De-initialize the MAX11046 drivers
 */
extern void BSP_MAX11046_DeInit(void);
/**
 * @brief Sets the input and output trigger functions for the ADC.
 * @note Currently only the @ref TIM_TRGI_TYPE_RST, @ref TIM_TRGI_TYPE_START and @ref TIM_TRGI_TYPE_RESET_AND_START
 * functionalities are available for this timer synchronization. Delays can't be added in master or slave mode triggering through OC1.
 * --TODO-- Add list of possible timers for triggering in different modes.
 * @param _slaveConfig Configuration for the slave. Send NULL if no slave functionality needed.
 * @param _masterConfig Configuration for the master. Send NULL if no master functionality needed.
 * @param _fs Required sampling frequency of the ADC converter
 * @return timer_trigger_src_t Trigger source if configuration required as master else returns NULL.
 */
extern timer_trigger_src_t BSP_MAX11046_SetInputOutputTrigger(tim_in_trigger_config_t* _slaveConfig, tim_out_trigger_config_t* _masterConfig, float _fs);
/********************************************************************************
 * Code
 *******************************************************************************/


/**
 * @}
 */
#endif
#ifdef __cplusplus
}
#endif
/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
#endif 
/* EOF */
