/**
 ********************************************************************************
 * @file   DioPort.h
 * @author Waqas Ehsan Butt
 * @date    25 September 2021
 * @copyright TarazTechnologies Pvt. Ltd.
 *
 * @brief This file controls the IO Port of the PEController module
 ********************************************************************************
 */
#ifndef DIO_PORT_H
#define DIO_PORT_H
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
typedef enum
{
	DIO_IN,
	DIO_OUT,
} dio_state_t;
/*******************************************************************************
 * Structs
 ******************************************************************************/

 /*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * @brief Set the Dio pin as GPIO
 *
 * @param pinNo Dio Pin No ( Range 1 - 8)
 * @param state State of the pin
 * @return *pin Pointer to the Dout pin structure
 */
const digital_pin_t* Dio_SetAsIOPin(uint32_t pinNo, GPIO_PinState state);
/**
 * @brief Selects the Alternate Output Functionality. To configure as IO use Dio_SetAsIOPin(pinNo, state)
 *
 * @param pinNo Dio pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return *digital_pin_t pointer to the pin structure
 */
const digital_pin_t* Dio_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction);
/**
 * @brief Set the IO Port as Input. Pin 9 is always output
 *
 */
void Dio_SetAsInputPort(void);
/**
 * @brief Set the IO Port as Output. Pin 9 is always output
 * @note Initial value for output pin is 0
 *
 */
void Dio_SetAsOutputPort(void);
/*!
 * @brief set the value of output port, sets all gpio ports at output
 * @param val- value to be set
 */
void Dio_SetPortValue(uint32_t val);
/*!
 * @brief set the value of output port, sets all gpio ports at output
 * @returns val- value on the port
 */
uint32_t Dio_GetPortValue(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
