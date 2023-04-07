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
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup ADCConfig_Exported_Macros Macros
  * @{
  */
#if PECONTROLLER_CONFIG == PLB_CUSTOM

#elif PECONTROLLER_CONFIG == PLB_MMC
/**
 * @brief Current measurements count
 */
#define MEASUREMENT_COUNT_CURRENT		(8)
/**
 * @brief Voltage measurements count
 */
#define MEASUREMENT_COUNT_VOLTAGE		(8)
#elif PECONTROLLER_CONFIG == PLB_TNPC
/**
 * @brief Current measurements count
 */
#define MEASUREMENT_COUNT_CURRENT		(6)
/**
 * @brief Voltage measurements count
 */
#define MEASUREMENT_COUNT_VOLTAGE		(6)
#elif PECONTROLLER_CONFIG == PLB_6PH
/**
 * @brief Current measurements count
 */
#define MEASUREMENT_COUNT_CURRENT		(6)
/**
 * @brief Voltage measurements count
 */
#define MEASUREMENT_COUNT_VOLTAGE		(6)
#elif PECONTROLLER_CONFIG == PLB_3PH
/**
 * @brief Current measurements count
 */
#define MEASUREMENT_COUNT_CURRENT		(3)
/**
 * @brief Voltage measurements count
 */
#define MEASUREMENT_COUNT_VOLTAGE		(3)
#endif
/**
 * @brief Total no of measurements in the PEController
 */
#define TOTAL_MEASUREMENT_COUNT			(16)
/**
 * @brief Number of measurement logs to be kept
 */
#define MEASURE_SAVE_COUNT				(32)
/**
 * @brief Number of raw measurement logs to be kept for communication systems
 */
#define RAW_MEASURE_SAVE_COUNT			(256)
/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup ADCConfig_Exported_Typedefs Type Definitions
  * @{
  */
#if PECONTROLLER_CONFIG == PEC_CUSTOM
/**
 * @brief Measurement structure definitions
 */
typedef struct
{
	float Ch1;				/**< @brief Measurement for Channel No 1 */
	float Ch2;				/**< @brief Measurement for Channel No 2 */
	float Ch3;				/**< @brief Measurement for Channel No 3 */
	float Ch4;				/**< @brief Measurement for Channel No 4 */
	float Ch5;				/**< @brief Measurement for Channel No 5 */
	float Ch6;				/**< @brief Measurement for Channel No 6 */
	float Ch7;				/**< @brief Measurement for Channel No 7 */
	float Ch8;				/**< @brief Measurement for Channel No 8 */
	float Ch9;				/**< @brief Measurement for Channel No 9 */
	float Ch10;				/**< @brief Measurement for Channel No 10 */
	float Ch11;				/**< @brief Measurement for Channel No 11 */
	float Ch12;				/**< @brief Measurement for Channel No 12 */
	float Ch13;				/**< @brief Measurement for Channel No 13 */
	float Ch14;				/**< @brief Measurement for Channel No 14 */
	float Ch15;				/**< @brief Measurement for Channel No 15 */
	float Ch16;				/**< @brief Measurement for Channel No 16 */
} adc_measures_t;
#elif PECONTROLLER_CONFIG == PLB_MMC
/**
 * @brief Measurement structure definitions
 */
typedef struct
{
	float Ia2;				/**< @brief Current of Leg A2 */
	float Ib2;				/**< @brief Current of Leg B2 */
	float Ic2;				/**< @brief Current of Leg C2 */
	float Id2;				/**< @brief Current of Leg D2 */
	float Ie2;				/**< @brief Current of Leg E2 */
	float If2;				/**< @brief Current of Leg F2 */
	float Ig2;				/**< @brief Current of Leg G2 */
	float Ih2;				/**< @brief Current of Leg H2 */
	float Vdc1;				/**< @brief DC Link voltage for first inverter module */
	float Vdc2;				/**< @brief DC Link voltage for second inverter module */
	float Vdc3;				/**< @brief DC Link voltage for third inverter module */
	float Vdc4;				/**< @brief DC Link voltage for fourth inverter module */
	float V1;				/**< @brief External voltage V1 */
	float V2;				/**< @brief External voltage V2 */
	float V3;				/**< @brief External voltage V3 */
	float V4;				/**< @brief External voltage V4 */
} adc_measures_t;
#elif PECONTROLLER_CONFIG == PLB_TNPC
/**
 * @brief Measurement structure definitions
 */
typedef struct
{
	float Ia2;				/**< @brief Current of Leg A2 */
	float Ic2;				/**< @brief Current of Leg C2 */
	float Ie2;				/**< @brief Current of Leg E2 */
	float Ig2;				/**< @brief Current of Leg G2 */
	float I1;				/**< @brief External current I1 */
	float I2;				/**< @brief External current I2 */
	float Reserved1;		/**< @brief RESERVED */
	float Reserved2;		/**< @brief RESERVED */
	float Vdc1;				/**< @brief DC Link voltage for first inverter module */
	float Vdc2;				/**< @brief DC Link voltage for second inverter module */
	float Vdc3;				/**< @brief DC Link voltage for second inverter module */
	float Vdc4;				/**< @brief DC Link voltage for second inverter module */
	float V1;				/**< @brief External voltage V1 */
	float V2;				/**< @brief External voltage V2 */
	float Reserved3;		/**< @brief RESERVED */
	float Reserved4;		/**< @brief RESERVED */
} adc_measures_t;
#elif PECONTROLLER_CONFIG == PLB_6PH
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
#elif PECONTROLLER_CONFIG == PLB_3PH
/**
 * @brief Measurement structure definitions
 */
typedef struct
{
	float Ih1;				/**< @brief Current of Leg H1 */
	float Ih2;				/**< @brief Current of Leg H2 */
	float Ih3;				/**< @brief Current of Leg H3 */
	float Reserved1;		/**< @brief RESERVED */
	float Reserved2;		/**< @brief RESERVED */
	float Reserved3;		/**< @brief RESERVED */
	float Reserved4;		/**< @brief RESERVED */
	float Reserved5;		/**< @brief RESERVED */
	float Vdc1;				/**< @brief DC Link voltage for first inverter module */
	float V1;				/**< @brief External voltage V1 */
	float V2;				/**< @brief External voltage V2 */
	float Reserved6;		/**< @brief RESERVED */
	float Reserved7;		/**< @brief RESERVED */
	float Reserved8;		/**< @brief RESERVED */
	float Reserved9;		/**< @brief RESERVED */
	float Reserved10;		/**< @brief RESERVED */
} adc_measures_t;
#endif

/**
 * @brief Structure containing statistics for single channel of the ADC
 */
typedef struct
{
	float rms;				/**< @brief RMS measurement */
	float avg;				/**< @brief Average measurement */
	float max;				/**< @brief Maximum measurement */
	float min;				/**< @brief Minimum measurement */
	float pkTopk;			/**< @brief Peak to peak measurement */
} adc_ch_stats_t;

/**
 * @brief Structure containing temporary statistics for single channel of the ADC
 */
typedef struct
{
	float rms;				/**< @brief RMS measurement */
	float avg;				/**< @brief Average measurement */
	float max;				/**< @brief Maximum measurement */
	float min;				/**< @brief Minimum measurement */
	int index;				/**< @brief Current index of measurement */
	int sampleCount;		/**< @brief No of samples to be used for statistics computation */
} adc_ch_temp_stats_t;

typedef struct
{
	float* offsets;
	float* sensitivty;
	adc_ch_stats_t* stats;
	adc_ch_temp_stats_t* tempStats;
} adc_info_t;

typedef struct
{
	int recordIndex;
	uint16_t dataRecord[RAW_MEASURE_SAVE_COUNT * TOTAL_MEASUREMENT_COUNT] __attribute__ ((aligned (8)));
} adc_raw_data_t;

typedef struct
{
	volatile bool isNewDataAvaialble;
	int recordIndex;
	volatile adc_measures_t* lastDataPointer;
	adc_measures_t dataRecord[MEASURE_SAVE_COUNT];
	adc_info_t* info;
} adc_processed_data_t;
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
