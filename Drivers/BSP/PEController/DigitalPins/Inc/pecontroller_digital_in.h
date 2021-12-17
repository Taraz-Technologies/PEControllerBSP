/**
 ********************************************************************************
 * @file 		pecontroller_digital_in.h
 * @author 		Waqas Ehsan Butt
 * @date 		September 25, 2021
 *
 * @brief	Controls the digital input port of the PEController
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
#ifndef PECONTROLLER_DIGITAL_IN_H
#define PECONTROLLER_DIGITAL_IN_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup DigitalPins
 * @{
 */

/** @addtogroup DinPorts Digital Input Port
 * @brief Contains the declaration and procedures for the digital input pins
 * @details List of functions
 * 	-# <b>@ref BSP_Din_SetAsIOPin() :</b> Sets the pin as input pin
 * 	-# <b>@ref BSP_Din_SetPinAlternateFunction() :</b> Selects the Alternate Input Functionality.
 * 														To configure the input as GPIO use @ref BSP_Din_SetAsIOPin()
 * 	-# <b>@ref BSP_Din_SetPortGPIO() :</b> Set the whole port as GPIO
 * 	-# <b>@ref BSP_Din_GetPortValue() :</b> Get the value of the  Complete Input Port
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

/*******************************************************************************
 * Structures
 ******************************************************************************/

 /*******************************************************************************
 * Exported Variables
 ******************************************************************************/

/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @defgroup DIN_Exported_Functions Functions
  * @{
  */
/**
 * @brief Sets the pin as input pin
 * @param pinNo Input pin No (Range 1-16)
 * @return digital_pin_t pointer to the pin structure
 */
extern const digital_pin_t* BSP_Din_SetAsIOPin(uint32_t pinNo);
/**
 * @brief Selects the Alternate Input Functionality. To configure the input as GPIO use Din_SetAsIOPin(pinNo)
 * @param pinNo Input pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return digital_pin_t pointer to the pin structure
 */
extern const digital_pin_t* BSP_Din_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction);
/**
 * @brief Set the whole port as GPIO
 */
extern void BSP_Din_SetPortGPIO(void);
/**
 * @brief Get the value of the  Complete Input Port
 * @return uint32_t Current Port Input Value
 */
extern uint32_t BSP_Din_GetPortValue(void);
/**
 * @brief Get the value of the input pins
 * @param pinNo Pin no from (1-16)
 *
 * @return uint32_t Value of pin should be either 0 or not 0
 */
extern uint32_t BSP_Din_GetPinState(uint32_t pinNo);
/*******************************************************************************
 * Code
 ******************************************************************************/

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
