/**
 ********************************************************************************
 * @file 		DoutPort.h
 * @author 		Waqas Ehsan Butt
 * @date 		Sept 25, 2021
 *
 * @brief	This file contains the basic definitions for the digital ouput pins
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
#ifndef DOUT_PORT_H
#define DOUT_PORT_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup PEController_Framework_Drivers
 * @{
 */

/** @addtogroup DigitalPins
 * @{
 */

/** @addtogroup DoutPorts Digital Output Port
 * @brief This module contains the functionality and definitions for the digital output pins
 * @details List of functions
 * 	-# <b>Dout_SetAsIOPin()</b> Set the Dout pin as GPIO
 * 	-# <b>Dout_SetAsPWMPin()</b> Set the Dout pin as PWM
 * 	-# <b>Dout_SetPinAlternateFunction()</b> Selects the Alternate Output Functionality. To configure as IO use Dout_SetAsIOPin(pinNo)
 * 	-# <b>Dout_SetPortAsGPIO()</b> Set the whole port as GPIO
 * 	-# <b>Dout_SetPortValue()</b> Set the value of output port
 * 	-# <b>Dout_TogglePin()</b> Toggles the selected pin
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
 * @brief Set the Dout pin as GPIO
 * @param pinNo Dout Pin No ( Range 1 - 16)
 * @param state State of the pin
 * @return *pin Pointer to the Dout pin structure
 */
extern const digital_pin_t* Dout_SetAsIOPin(uint32_t pinNo, GPIO_PinState state);
/**
 * @brief Set the Dout pin as PWM
 * @param pinNo Dout Pin No ( Range 1 - 16)
 * @return *pin Pointer to the Dout pin structure
 */
extern const digital_pin_t* Dout_SetAsPWMPin(uint32_t pinNo);
/**
 * @brief Selects the Alternate Output Functionality. To configure as IO use Dout_SetAsIOPin(pinNo)
 * @param pinNo Dout pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return *digital_pin_t pointer to the pin structure
 */
extern const digital_pin_t* Dout_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction);
/**
 * @brief Set the whole port as GPIO
 */
extern void Dout_SetPortAsGPIO(void);
/**
 * @brief Set the value of output port, sets all gpio ports at output
 * @param val value to be set
 */
extern void Dout_SetPortValue(uint32_t val);
/**
 * @brief Toggles the selected pin
 * @param pinNo Dout Pin No (Range 1-16)
 */
extern void Dout_TogglePin(int pinNo);
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

