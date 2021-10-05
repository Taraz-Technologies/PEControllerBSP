/**
 ********************************************************************************
 * @file   DinPort.h
 * @author Waqas Ehsan Butt
 * @date    25 September 2021
 * @copyright TarazTechnologies Pvt. Ltd.
 *
 * @brief Header file for Din Port
 ********************************************************************************
 */
#ifndef DIN_PORT_H
#define DIN_PORT_H
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
 * @brief Selects the Input Mode. Currently only the GPIO mode is supported by this function
 *
 * @param pinNo Input pin No (Range 1-16)
 * @return digital_pin_t pointer to the pin structure
 */
const digital_pin_t* Din_SetAsIOPin(uint32_t pinNo);
/**
 * @brief Selects the Alternate Input Functionality. To configure the input as GPIO use Din_SetAsIOPin(pinNo)
 *
 * @param pinNo Input pin No (Range 1-16)
 * @return digital_pin_t pointer to the pin structure
 */
const digital_pin_t* Din_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction);
/**
 * @brief Set the whole port as GPIO
 *
 */
void Din_SetPortGPIO(void);
/**
 * @brief Get the value of the  Complete Input Port
 *
 * @return uint32_t Current Port Input Value
 */
uint32_t Din_GetPortValue(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
