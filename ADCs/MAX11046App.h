/*! 
@file MAX11046App.h
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
 */
#ifndef MAX11046_APP_H
#define MAX11046_APP_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdint.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
typedef struct _adc_measures_ adc_measures_t;
/*!
@brief callback for result submission
@param *result- pointer to the control system data
 */
typedef void (*adcMeauresDataCallback)(adc_measures_t* result);
/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/
/*! @brief measurements representing the control system variables */
typedef struct _adc_measures_
{
	float Ih1;
	float Ih2;
	float Ih3;
	float Ie1;
	float Ie2;
	float Ie3;
	float Reserved4;
	float Reserved5;
	float Vdc1;
	float V1;
	float V2;
	float Vdc2;
	float V3;
	float V4;
	float Reserved9;
	float Reserved10;
} adc_measures_t;
 /*******************************************************************************
 * Prototypes
 ******************************************************************************/
#ifdef CORE_CM4
/*!
@brief Initialize the module for control related measurements
@param periodUs- period in Us for the continuous conversion mode
@param dataCallback- callback for the data processing
*/
void MAX11046App_Init(uint32_t periodUs, adcMeauresDataCallback dataCallback);
/*! @brief start measuring the control related measuremnts */
void MAX11046App_Run(void);
/*! @brief stop measuring and reset the timer */
void MAX11046App_StopAndResetTimer(void);
#else
void MAX11046_Poll(void);
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern adc_measures_t adcVals;
/*******************************************************************************
 * Code
 ******************************************************************************/


#if defined(__cplusplus)
}
#endif
#endif
/* EOF */
