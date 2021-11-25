/**
 ********************************************************************************
 * @file 		DinPort.h
 * @author 		Waqas Ehsan Butt
 * @date 		Sept 25, 2021
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
#ifndef DIN_PORT_H
#define DIN_PORT_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup PEController_Framework_Drivers
 * @{
 */

/** @addtogroup DigitalPins
 * @{
 */

/** @addtogroup DinPorts Digital Input Port
 * @brief This module contains the functionality and definitions for the digital input pins
 * @details List of functions
 * 	-# <b>Din_SetAsIOPin()</b> Sets the pin as input pin
 * 	-# <b>Din_SetPinAlternateFunction()</b> Selects the Alternate Input Functionality. To configure the input as GPIO use Din_SetAsIOPin(pinNo)
 * 	-# <b>Din_SetPortGPIO()</b> Set the whole port as GPIO
 * 	-# <b>Din_GetPortValue()</b> Get the value of the  Complete Input Port
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "DigitalPins.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/

/*******************************************************************************
 * Structures
 ******************************************************************************/

 /*******************************************************************************
 * Exported Variables
 ******************************************************************************/

/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/**
 * @brief Sets the pin as input pin
 * @param pinNo Input pin No (Range 1-16)
 * @return digital_pin_t pointer to the pin structure
 */
extern const digital_pin_t* Din_SetAsIOPin(uint32_t pinNo);
/**
 * @brief Selects the Alternate Input Functionality. To configure the input as GPIO use Din_SetAsIOPin(pinNo)
 * @param pinNo Input pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return digital_pin_t pointer to the pin structure
 */
extern const digital_pin_t* Din_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction);
/**
 * @brief Set the whole port as GPIO
 */
extern void Din_SetPortGPIO(void);
/**
 * @brief Get the value of the  Complete Input Port
 * @return uint32_t Current Port Input Value
 */
extern uint32_t Din_GetPortValue(void);
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

/**
 * @}
 */
#endif
/* EOF */
