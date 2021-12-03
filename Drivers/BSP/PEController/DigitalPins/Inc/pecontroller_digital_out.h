/**
 ********************************************************************************
 * @file 		pecontroller_digital_out.h
 * @author 		Waqas Ehsan Butt
 * @date 		September 25, 2021
 *
 * @brief	Controls the digital output port of the PEController
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
#ifndef PECONTROLLER_DIGITAL_OUT_H
#define PECONTROLLER_DIGITAL_OUT_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @addtogroup DigitalPins
 * @{
 */

/** @addtogroup DoutPorts Digital Output Port
 * @brief This module contains the functionality and definitions for the digital output pins
 * @details List of functions
 * 	-# <b>@ref BSP_Dout_SetAsIOPin() :</b> Set the Dout pin as GPIO
 * 	-# <b>@ref BSP_Dout_SetAsPWMPin() :</b> Set the Dout pin as PWM
 * 	-# <b>@ref BSP_Dout_SetPinAlternateFunction()</b> Selects the Alternate Output Functionality.
 * 												To configure as IO use @ref BSP_Dout_SetAsIOPin()
 * 	-# <b>@ref BSP_Dout_SetPortAsGPIO() :</b> Set the whole port as GPIO
 * 	-# <b>@ref BSP_Dout_SetPortValue() :</b> Set the value of output port
 * 	-# <b>@ref BSP_Dout_TogglePin() :</b> Toggles the selected pin
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "pecontroller_digital_pins.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
/** @defgroup DOUT_Exported_Macros Macros
  * @{
  */

/**
 * @}
 */
/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @defgroup DOUT_Exported_Typedefs Type Definitions
  * @{
  */

/**
 * @}
 */
/*******************************************************************************
 * Structures
 ******************************************************************************/
/** @defgroup DOUT_Exported_Structures Structures
  * @{
  */

/**
 * @}
 */
/*******************************************************************************
 * Exported Variables
 ******************************************************************************/
/** @defgroup DOUT_Exported_Variables Variables
  * @{
  */

/**
 * @}
 */
 /*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @defgroup DOUT_Exported_Functions Functions
  * @{
  */
/**
 * @brief Set the Dout pin as GPIO
 * @param pinNo Dout Pin No ( Range 1 - 16)
 * @param state State of the pin
 * @return *pin Pointer to the Dout pin structure
 */
extern const digital_pin_t* BSP_Dout_SetAsIOPin(uint32_t pinNo, GPIO_PinState state);
/**
 * @brief Set the Dout pin as PWM
 * @param pinNo Dout Pin No ( Range 1 - 16)
 * @return *pin Pointer to the Dout pin structure
 */
extern const digital_pin_t* BSP_Dout_SetAsPWMPin(uint32_t pinNo);
/**
 * @brief Selects the Alternate Output Functionality. To configure as IO use Dout_SetAsIOPin(pinNo)
 * @param pinNo Dout pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return *digital_pin_t pointer to the pin structure
 */
extern const digital_pin_t* BSP_Dout_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction);
/**
 * @brief Set the whole port as GPIO
 */
extern void BSP_Dout_SetPortAsGPIO(void);
/**
 * @brief Set the value of output port, sets all GPIO ports at output
 * @param val value to be set
 */
extern void BSP_Dout_SetPortValue(uint32_t val);
/**
 * @brief Toggles the selected pin
 * @param pinNo Dout Pin No (Range 1-16)
 */
extern void BSP_Dout_TogglePin(int pinNo);
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

