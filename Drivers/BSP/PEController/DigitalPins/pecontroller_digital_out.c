/**
 ********************************************************************************
 * @file    	pecontroller_digital_out.c
 * @author 		Waqas Ehsan Butt
 * @date    	September 2, 2021
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

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "pecontroller_digital_out.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define DOUT_COUNT						(16)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
/** Defines the pins associated with digital output port
 */
static const digital_pin_t doutPins[DOUT_COUNT] =
{
		{.GPIO = GPIOC, .pinMask = GPIO_PIN_6,},
		{.GPIO = GPIOC, .pinMask = GPIO_PIN_7,},
		{.GPIO = GPIOC, .pinMask = GPIO_PIN_8,},
		{.GPIO = GPIOA, .pinMask = GPIO_PIN_8,},
		{.GPIO = GPIOA, .pinMask = GPIO_PIN_9,},
		{.GPIO = GPIOA, .pinMask = GPIO_PIN_10},
		{.GPIO = GPIOA, .pinMask = GPIO_PIN_11},
		{.GPIO = GPIOA, .pinMask = GPIO_PIN_12},
		{.GPIO = GPIOG, .pinMask = GPIO_PIN_6},
		{.GPIO = GPIOG, .pinMask = GPIO_PIN_7},
		{.GPIO = GPIOE, .pinMask = GPIO_PIN_9},
		{.GPIO = GPIOE, .pinMask = GPIO_PIN_8},
		{.GPIO = GPIOJ, .pinMask = GPIO_PIN_11},
		{.GPIO = GPIOJ, .pinMask = GPIO_PIN_10},
		{.GPIO = GPIOJ, .pinMask = GPIO_PIN_9},
		{.GPIO = GPIOE, .pinMask = GPIO_PIN_12},
};
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Initialize the Pin with default parameters
 * @param pinNo Pin number of the specified pin
 * @param *GPIO_InitStruct Pointer to the GPIO Structure
 * @param state Output State
 * @return digital_pin_t* pointer to the pin structure
 */
static const digital_pin_t* InitPin(uint32_t pinNo, GPIO_InitTypeDef* GPIO_InitStruct, GPIO_PinState state)
{
	const digital_pin_t* pin = &doutPins[pinNo - 1];
	GPIO_InitStruct->Pin = pin->pinMask;
	GPIO_InitStruct->Pull = GPIO_NOPULL;
	GPIO_InitStruct->Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_WritePin(pin->GPIO, pin->pinMask, state);
	HAL_GPIO_Init(pin->GPIO, GPIO_InitStruct);
	return pin;
}

/**
 * @brief Set the Dout pin as GPIO
 * @param pinNo Dout Pin No ( Range 1 - 16)
 * @param state State of the pin
 * @return *pin Pointer to the Dout pin structure
 */
const digital_pin_t* BSP_Dout_SetAsIOPin(uint32_t pinNo, GPIO_PinState state)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	return InitPin(pinNo, &GPIO_InitStruct, state);
}

/**
 * @brief Set the Dout pin as PWM
 * @param pinNo Dout Pin No ( Range 1 - 16)
 * @return *pin Pointer to the Dout pin structure
 */
const digital_pin_t* BSP_Dout_SetAsPWMPin(uint32_t pinNo)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = pinNo < 4 ? GPIO_AF1_HRTIM1 : (pinNo < 11 ? GPIO_AF2_HRTIM1 : GPIO_AF1_TIM1);
	return InitPin(pinNo, &GPIO_InitStruct, GPIO_PIN_RESET);
}

/**
 * @brief Selects the Alternate Output Functionality. To configure as IO use Dout_SetAsIOPin(pinNo)
 * @param pinNo Dout pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return *digital_pin_t pointer to the pin structure
 */
const digital_pin_t* BSP_Dout_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = AlternateFunction;
	return InitPin(pinNo, &GPIO_InitStruct, GPIO_PIN_RESET);
}

/**
 * @brief Set the whole port as GPIO
 */
void BSP_Dout_SetPortAsGPIO(void)
{
	for(int i = 0; i < DOUT_COUNT; i++)
		BSP_Dout_SetAsIOPin(i + 1, GPIO_PIN_RESET);
}

/**
 * @brief Set the value of output port, sets all GPIO ports at output
 * @param val value to be set
 */
void BSP_Dout_SetPortValue(uint32_t val)
{
	for(int i = 0; i < DOUT_COUNT; i++)
	{
		const digital_pin_t* pin = &doutPins[i];
		HAL_GPIO_WritePin(pin->GPIO, pin->pinMask, (GPIO_PinState)(val & 0x1));
		val = val >> 1U;
	}
}

/**
 * @brief Toggles the selected pin
 * @param pinNo Dout Pin No (Range 1-16)
 */
void BSP_Dout_TogglePin(int pinNo)
{
	const digital_pin_t* pin = &doutPins[pinNo-1];
	HAL_GPIO_TogglePin(pin->GPIO, pin->pinMask);
}

/**
 * @brief Set the desired pin value to 1 for DOUT port
 * @param pinNo Dout Pin No (Range 1-16)
 */
void BSP_Dout_SetPin(int pinNo)
{
	const digital_pin_t* pin = &doutPins[pinNo-1];
	pin->GPIO->BSRR = pin->pinMask;
}

/**
 * @brief Set the desired pin value to 0 for DOUT port
 * @param pinNo Dout Pin No (Range 1-16)
 */
void BSP_Dout_ClearPin(int pinNo)
{
	const digital_pin_t* pin = &doutPins[pinNo-1];
	pin->GPIO->BSRR = (uint32_t)(pin->pinMask) << 16U;;
}

/* EOF */
