/**
 ********************************************************************************
 * @file 		adc_config.h
 * @author 		Waqas Ehsan Butt
 * @date 		November 30, 2021
 *
 * @brief   Defines the application dependent parameters of the PEController ADC
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

#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @defgroup ADC ADC
 * @brief This module contains all ADC related definitions
 * @{
 */

/** @defgroup ADC_Config Configuration
 * @brief This module contains the configurations for the PEController ADC
 * @details The following definitions are required by the user
 * 	-# <b>@ref adc_measures_t :</b> Measurement structure definition
 * 	-# <b>@ref adcMultipiers :</b> Defines the multipliers for each member of the ADC measurement
 * These values are used to convert ADC data to meaningful measurements according to the formula <b>value = (adcData - adcOffsets) * adcMultipiers</b>
 * 	-# <b>@ref adcOffsets :</b> Defines the offsets for each member of the ADC measurement
 * These values are used to convert ADC data to meaningful measurements according to the formula <b>value = (adcData - adcOffsets) * adcMultipiers</b>
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/

/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup ADCConfig_Exported_Macros Macros
  * @{
  */

/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup ADCConfig_Exported_Typedefs Type Definitions
  * @{
  */
/**
 * @brief Measurement structure definitions
 */
typedef struct
{
	float Ih1;				/**< @brief Current of Leg H1 */
	float Ih2;				/**< @brief Current of Leg H2 */
	float Ih3;				/**< @brief Current of Leg H3 */
	float Ie1;				/**< @brief Current of Leg E1 */
	float Ie2;				/**< @brief Current of Leg E2 */
	float Ie3;				/**< @brief Current of Leg E3 */
	float Reserved1;		/**< @brief RESERVED */
	float Reserved2;		/**< @brief RESERVED */
	float Vdc1;				/**< @brief DC Link voltage for first inverter module */
	float V1;				/**< @brief External voltage V1 */
	float V2;				/**< @brief External voltage V2 */
	float Vdc2;				/**< @brief DC Link voltage for second inverter module */
	float V3;				/**< @brief External voltage V3 */
	float V4;				/**< @brief External voltage V4 */
	float Reserved3;		/**< @brief RESERVED */
	float Reserved4;		/**< @brief RESERVED */
} adc_measures_t;
/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup ADCConfig_Exported_Structures Structures
  * @{
  */

/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup ADCConfig_Exported_Variables Variables
  * @{
  */
/** Defines the multipliers for each member of the ADC measurement
 * These values are used to convert ADC data to meaningful measurements according to the formula <b>value = (adcData - adcOffsets) * adcMultipiers</b>
 */
static const adc_measures_t adcMultipiers = {
		.Ih1 = 25.0f / 32768, .Ih2 = 25.0f / 32768, .Ih3 = 25.0f / 32768,
		.Ie1 = 25.0f / 32768, .Ie2 = 25.0f / 32768, .Ie3 = 25.0f / 32768,
		.Vdc1 = 1000.f / 32768, .V1 = 1000.f / 32768, .V2 = 1000.f / 32768,
		.Vdc2 = 1000.f / 32768, .V3 = 1000.f / 32768, .V4 = 1000.f / 32768,
		.Reserved1 = 0, .Reserved2 = 0, .Reserved3 = 0, .Reserved4 = 0
};
/** Defines the offsets for each member of the ADC measurement.
 * These values are used to convert ADC data to meaningful measurements according to the formula <b>value = (adcData - adcOffsets) * adcMultipiers</b>
 */
static const adc_measures_t adcOffsets = {
		.Ih1 = 32768, .Ih2 = 32768, .Ih3 = 32768,
		.Ie1 = 32768, .Ie2 = 32768, .Ie3 = 32768,
		.Vdc1 = 32768, .V1 = 32768, .V2 = 32768,
		.Vdc2 = 32768, .V3 = 32768, .V4 = 32768,
		.Reserved1 = 32768, .Reserved2 = 32768, .Reserved3 = 32768, .Reserved4 = 32768
};
/**
 * @}
 */
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup ADCConfig_Exported_Functions Functions
  * @{
  */
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
