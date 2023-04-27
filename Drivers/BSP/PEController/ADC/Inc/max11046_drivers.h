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
 * @brief Contains the declaration and procedures for MAX11046 ADC
 * @details List of functions
 * 	-# <b>@ref BSP_MAX11046_Init() :</b> Initializes the MAX11046 drivers
 * 	-# <b>@ref BSP_MAX11046_DeInit() :</b> De-initialize the MAX11046 drivers
 * 	-# <b>@ref BSP_MAX11046_Run() :</b> Performs the conversion
 * 	-# <b>@ref BSP_MAX11046_Stop() :</b> Stops the ADC data collection module, only effective for ADC_MODE_CONT
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "adc_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup MAX11046_Exported_Typedefs Type Definitions
 * @{
 */
/**
 * @brief ADC acquisition mode definitions
 */
typedef enum _adc_acq_mode_
{
	ADC_MODE_SINGLE,	/**< @brief Used for single conversion mode */
	ADC_MODE_CONT   	/**< @brief Used for continuous conversion mode */
} adc_acq_mode_t;
/**
 * @brief Callback for ADC results
 * @param *result Pointer to the most recent ADC results
 */
typedef void (*adcMeauresDataCallback)(adc_measures_t* result);
/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup MAX11046_Exported_Structures Structures
 * @{
 */
/**
 * @brief Defines the parameters for continuous conversions
 */
typedef struct
{
	int conversionCycleTimeUs;			/**< @brief Period In micro-seconds */
	adcMeauresDataCallback callback;	/**< @brief Callback function called when results are ready */
} adc_cont_config_t;

/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup MAX11046_Exported_Variables Variables
 * @{
 */

/**
 * @}
 */
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup MAX11046_Exported_Functions Functions
 * @{
 */
/**
 * @brief Initializes the MAX11046 drivers
 * @param type- ADC_MODE_SINGLE or ADC_MODE_CONT for single or continuous conversions respectively
 * @param *contConfig- adc_cont_config_t contains the continuous transfer configuration
 */
extern void BSP_MAX11046_Init(adc_acq_mode_t type, adc_cont_config_t* contConfig, volatile adc_raw_data_t* rawAdcData, volatile adc_processed_data_t* processedAdcData);
/**
 * @brief Performs the conversion
 * @details For single conversion mode performs and provides the results in a blocking way.
 * For continuous conversion mode starts the conversions, after each conversion adc_cont_config_t.callback will be called
 * @return adc_measures_t_t*- For single conversion mode returns the pointer to the acquired values. Returns NULL for continuous conversion mode
 */
extern volatile adc_measures_t* BSP_MAX11046_Run(void);
/**
 * @brief Stops the ADC data collection module, only effective for ADC_MODE_CONT
 */
extern void BSP_MAX11046_Stop(void);
/**
 * @brief De-initialize the MAX11046 drivers
 */
extern void BSP_MAX11046_DeInit(void);

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
/**
 * @}
 */
#endif 
/* EOF */
