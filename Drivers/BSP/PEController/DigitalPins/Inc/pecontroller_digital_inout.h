/**
 ********************************************************************************
 * @file 		DioPort.h
 * @author 		Waqas Ehsan Butt
 * @date 		Sept 25, 2021
 *
 * @brief	This file contains the basic definitions for the digital input/ouput pins
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
#ifndef DIO_PORT_H
#define DIO_PORT_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup PEController_Framework_Drivers
 * @{
 */

/** @addtogroup DigitalPins
 * @{
 */

/** @addtogroup DioPorts Digital Input Output Port
 * @brief This module contains the functionality and definitions for the digital input/output pins
 * @details List of functions
 * 	-# <b>@ref BSP_Dio_SetAsIOPin()</b> Set the Dio pin as GPIO
 * 	-# <b>@ref BSP_Dio_SetPinAlternateFunction()</b> Selects the Alternate Output Functionality. To configure as IO use Dio_SetAsIOPin(pinNo, state)
 * 	-# <b>@ref BSP_Dio_SetAsInputPort()</b> Set the IO Port as Input. Pin 9 is always output
 * 	-# <b>@ref BSP_Dio_SetAsOutputPort()</b> Set the IO Port as Output. Pin 9 is always output
 * 	-# <b>@ref BSP_Dio_SetPortValue()</b> Set the value of output port
 * 	-# <b>@ref BSP_Dio_GetPortValue()</b> Gets the value of the input port
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "pecontroller_digital_pins.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/**
 * @brief Digital IO state definitions
 */
typedef enum
{
	DIO_IN, /**< Define as input pin */
	DIO_OUT,/**< Define as output pin */
} dio_state_t;
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
 * @brief Set the Dio pin as GPIO
 * @param pinNo Dio Pin No ( Range 1 - 8)
 * @param state State of the pin
 * @return *pin Pointer to the Dio pin structure
 */
extern const digital_pin_t* BSP_Dio_SetAsIOPin(uint32_t pinNo, GPIO_PinState state);
/**
 * @brief Selects the Alternate Output Functionality. To configure as IO use Dio_SetAsIOPin(pinNo, state)
 * @param pinNo Dio pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return *digital_pin_t pointer to the pin structure
 */
extern const digital_pin_t* BSP_Dio_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction);
/**
 * @brief Set the IO Port as Input. Pin 9 is always output
 */
extern void BSP_Dio_SetAsInputPort(void);
/**
 * @brief Set the IO Port as Output. Pin 9 is always output
 * @note Initial value for output pin is 0
 */
extern void BSP_Dio_SetAsOutputPort(void);
/*!
 * @brief Set the value of output port
 * @param val- value to be set
 */
extern void BSP_Dio_SetPortValue(uint32_t val);
/*!
 * @brief Gets the value of the input port
 * @returns val- value on the port
 */
extern uint32_t BSP_Dio_GetPortValue(void);
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
