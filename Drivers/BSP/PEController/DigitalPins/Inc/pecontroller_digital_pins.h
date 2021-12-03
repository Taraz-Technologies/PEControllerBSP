/**
 ********************************************************************************
 * @file 		pecontroller_digital_pins.h
 * @author 		Waqas Ehsan Butt
 * @date 		November 25, 2021
 *
 * @brief	Controls the digital pins of the PEController
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
#ifndef PECONTROLLER_DIGITAL_PINS_H
#define PECONTROLLER_DIGITAL_PINS_H

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */

/** @defgroup DigitalPins Digital Pins
 * @brief This module contains the functionality and definitions for the digital pins
 * @note DigitalPins_Init() should be called before using any of the digital pins functionality
 * @{
 */

/** @defgroup DigitalPinsBase Digital Pins Info
 * @brief This module contains the base functionality and definitions for the digital pins
 * @{
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32h7xx_hal.h"
#include <stdbool.h>
/*******************************************************************************
 * Defines
 ******************************************************************************/
/** @defgroup DPINS_Exported_Macros Macros
  * @{
  */

/**
 * @}
 */
/*******************************************************************************
 * Typedefs
 ******************************************************************************/
/** @defgroup DPINS_Exported_Typedefs Type Definitions
  * @{
  */
/**
 * @brief PWM pin functionality definitions
 */
typedef enum
{
	PWM_FNC_PWM,/**< Select PWM functionality */
	PWM_FNC_IO  /**< Select IO functionality */
} pwm_fnc_t;
/**
 * @}
 */
/*******************************************************************************
 * Structures
 ******************************************************************************/
/** @defgroup DPINS_Exported_Structures Structures
  * @{
  */
/**
 * @brief Defines the basic structure for a digital pin
 */
typedef struct
{
	GPIO_TypeDef* GPIO;		/**< @brief Pointer to GPIO associated with the pin */
	uint16_t pinMask;		/**< @brief Pin mask to be used for configuration of the pin */
} digital_pin_t;
/**
 * @}
 */
/*******************************************************************************
 * Exported Variables
 ******************************************************************************/
/** @defgroup DPINS_Exported_Variables Variables
  * @{
  */

/**
 * @}
 */
/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/** @defgroup DPINS_Exported_Functions Functions
  * @{
  */
/**
 * @brief Initializes the digital pins
 */
extern void BSP_DigitalPins_Init(void);
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

