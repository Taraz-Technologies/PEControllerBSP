/**
 ********************************************************************************
 * @file 		intelliSENS.h
 * @author 		Waqas Ehsan Butt
 * @date 		Dec 28, 2021
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

#ifndef INTELLISENS_H_
#define INTELLISENS_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	/**
	 * @brief Add the ADC data to USB buffer for logging
	 * @param data pointer to the measurement data
	 * @return bool <c>true</c> if added successfully else <c>false</c>. This function will not add data if not requested by the intelliSENS Application
	 */
	bool (*SetADCData)(uint64_t* data);
	/**
	 * @brief Initialize the intelliSENS library
	 * @param periodInUs ADC conversion interval in micro-seconds
	 * @param mults ADC sensitivities
	 * @param offsets ADC offsets
	 */
	void (*Init)(int periodInUs, const float* mults, const float* offsets);
	/**
	 * @brief Poll the intelliSENS library to send any pending data
	 */
	void (*Poll)(void);
	void (*SetADCTicks)(uint16_t ticks);
}intelliSENS_t;

/********************************************************************************
 * Exported Variables
 *******************************************************************************/
extern intelliSENS_t intelliSENS;
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
extern void intelliSENS_Configure(void);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
