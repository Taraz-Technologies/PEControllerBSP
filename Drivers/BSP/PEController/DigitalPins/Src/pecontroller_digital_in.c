/**
 ********************************************************************************
 * @file    	pecontroller_digital_in.c
 * @author 		Waqas Ehsan Butt
 * @date    	September 25, 2021
 *
 * @brief   Controls the digital input port of the PEController
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
#include "pecontroller_digital_in.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define DIN_COUNT						(16)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
/** Defines the pins associated with digital input port
 */
static const digital_pin_t dinPins[DIN_COUNT] =
{
		{.GPIO = GPIOG, .pinMask = GPIO_PIN_1},
		{.GPIO = GPIOG, .pinMask = GPIO_PIN_4},
		{.GPIO = GPIOG, .pinMask = GPIO_PIN_5},
		{.GPIO = GPIOG, .pinMask = GPIO_PIN_8},
		{.GPIO = GPIOI, .pinMask = GPIO_PIN_1},
		{.GPIO = GPIOI, .pinMask = GPIO_PIN_3},
		{.GPIO = GPIOD, .pinMask = GPIO_PIN_4},
		{.GPIO = GPIOD, .pinMask = GPIO_PIN_5},
		{.GPIO = GPIOG, .pinMask = GPIO_PIN_9},
		{.GPIO = GPIOG, .pinMask = GPIO_PIN_15},
		{.GPIO = GPIOD, .pinMask = GPIO_PIN_12},
		{.GPIO = GPIOD, .pinMask = GPIO_PIN_11},
		{.GPIO = GPIOD, .pinMask = GPIO_PIN_13},
		{.GPIO = GPIOA, .pinMask = GPIO_PIN_0},
		{.GPIO = GPIOE, .pinMask = GPIO_PIN_7},
		{.GPIO = GPIOE, .pinMask = GPIO_PIN_10},
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
 * @brief Initialize the Din Pin with default parameters
 * @param pinNo Pin number of the specified pin
 * @param *GPIO_InitStruct Pointer to the GPIO Structure
 * @return digital_pin_t* pointer to the pin structure
 */
static const digital_pin_t* InitPin(uint32_t pinNo, GPIO_InitTypeDef* GPIO_InitStruct)
{
	const digital_pin_t* pin = &dinPins[pinNo - 1];
	GPIO_InitStruct->Pin = pin->pinMask;
	GPIO_InitStruct->Pull = GPIO_NOPULL;
	GPIO_InitStruct->Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(pin->GPIO, GPIO_InitStruct);
	return pin;
}

/**
 * @brief Sets the pin as input pin
 * @param pinNo Input pin No (Range 1-16)
 * @return digital_pin_t pointer to the pin structure
 */
const digital_pin_t* BSP_Din_SetAsIOPin(uint32_t pinNo)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	return InitPin(pinNo, &GPIO_InitStruct);
}
/**
 * @brief Selects the Alternate Input Functionality. To configure the input as GPIO use Din_SetAsIOPin(pinNo)
 * @param pinNo Input pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return digital_pin_t pointer to the pin structure
 */
const digital_pin_t* BSP_Din_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = AlternateFunction;
	return InitPin(pinNo, &GPIO_InitStruct);
}
/**
 * @brief Set the whole port as GPIO
 *
 */
void BSP_Din_SetPortGPIO(void)
{
	for(int i = 0; i < DIN_COUNT; i++)
		BSP_Din_SetAsIOPin(i + 1);
}
/**
 * @brief Get the value of the  Complete Input Port
 *
 * @return uint32_t Current Port Input Value
 */
uint32_t BSP_Din_GetPortValue(void)
{
	uint32_t val = 0;
	for(int i = DIN_COUNT-1; i > 0; i--)
	{
		const digital_pin_t* pin = &dinPins[i];
		val |= (uint32_t)HAL_GPIO_ReadPin(pin->GPIO, pin->pinMask);
		val = val << 1U;
	}
	val |= (uint32_t)HAL_GPIO_ReadPin(dinPins[0].GPIO, dinPins[0].pinMask);
	return val;
}


/* EOF */
