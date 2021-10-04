/*!
@file DinManager.c
@brief
@details

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "DinController.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define DIN_COUNT						(16)
/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/

 /*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
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
/*******************************************************************************
 * Code
 ******************************************************************************/
const digital_pin_t* DinController_SelectMode(uint32_t pinNo)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	pinNo--;
	const digital_pin_t* pin = &dinPins[pinNo];
	GPIO_InitStruct.Pin = pin->pinMask;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(pin->GPIO, &GPIO_InitStruct);
	return pin;
}
/*!
 * @brief set the whole port as GPIO
 */
void DinController_SetPortAsGPIO(void)
{
	for(int i = 0; i < DIN_COUNT; i++)
		DinController_SelectMode(i + 1);
}
/*!
 * @brief set the value of output port, sets all gpio ports at output
 * @returns val- value on the port
 */
uint32_t DinController_GetPortValue(void)
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
