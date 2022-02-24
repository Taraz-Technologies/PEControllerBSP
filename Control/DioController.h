/*!
@file DIOManager.h
@brief
@details

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
#ifndef DIO_CONTROLLER_H
#define DIO_CONTROLLER_H
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
/*! @brief Select mode for the pin */
const digital_pin_t* DIOController_SelectMode(uint32_t pinNo, GPIO_PinState state);
/*! @brief set the IO pins as input, Pin 9 is always output */
void DIOController_SetAsInput(void);
/*!
@brief set the IO pins as output, Pin 9 is always output
@note Initial value for output pin is 0
*/
void DIOController_SetAsOutput(void);
/*!
 * @brief set the value of output port, sets all gpio ports at output
 * @param val- value to be set
 */
void DIOController_SetPortValue(uint32_t val);
/*!
 * @brief set the value of output port, sets all gpio ports at output
 * @returns val- value on the port
 */
uint32_t DIOController_GetPortValue(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
