/**
 ********************************************************************************
 * @file   DoutPort.h
 * @author Waqas Ehsan Butt
 * @date    25 September 2021
 * @copyright TarazTechnologies Pvt. Ltd.
 *
 * @brief Header file for Dout Port
 ********************************************************************************
 */
#ifndef DOUT_PORT_H
#define DOUT_PORT_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "DigitalPins.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/

 /*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * @brief Set the Dout pin as GPIO
 *
 * @param pinNo Dout Pin No ( Range 1 - 16)
 * @param state State of the pin
 * @return *pin Pointer to the Dout pin structure
 */
const digital_pin_t* Dout_SetAsIOPin(uint32_t pinNo, GPIO_PinState state);
/**
 * @brief Set the Dout pin as PWM
 *
 * @param pinNo Dout Pin No ( Range 1 - 16)
 * @param state State of the pin
 * @return *pin Pointer to the Dout pin structure
 */
const digital_pin_t* Dout_SetAsPWMPin(uint32_t pinNo);
/**
 * @brief Selects the Alternate Output Functionality. To configure as IO use Dout_SetAsIOPin(pinNo)
 *
 * @param pinNo Dout pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return *digital_pin_t pointer to the pin structure
 */
const digital_pin_t* Dout_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction);
/*!
 * @brief set the whole port as GPIO
 */
void Dout_SetPortAsGPIO(void);
/*!
 * @brief set the value of output port, sets all gpio ports at output
 * @param val value to be set
 */
void Dout_SetPortValue(uint32_t val);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
#endif
/* EOF */

