/*!
@file DIOManager.c
@brief
@details

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "DioController.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define DIO_COUNT						(8)  		// PIN9 is always output
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
static const digital_pin_t dIOPins[DIO_COUNT + 1] =
{
	{.GPIO = GPIOJ, .pinMask = GPIO_PIN_8},
	{.GPIO = GPIOH, .pinMask = GPIO_PIN_13},
	{.GPIO = GPIOJ, .pinMask = GPIO_PIN_6},
	{.GPIO = GPIOJ, .pinMask = GPIO_PIN_7},
	{.GPIO = GPIOK, .pinMask = GPIO_PIN_0},
	{.GPIO = GPIOK, .pinMask = GPIO_PIN_1},
	{.GPIO = GPIOB, .pinMask = GPIO_PIN_8},
	{.GPIO = GPIOB, .pinMask = GPIO_PIN_6},
	{.GPIO = GPIOD, .pinMask = GPIO_PIN_3},		// this pin is always output
};
static const digital_pin_t dirControlPin = {.GPIO = GPIOD, .pinMask = GPIO_PIN_14};
static dio_state_t dIOState = DIO_IN;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief Select mode for the pin */
const digital_pin_t* DIOController_SelectMode(uint32_t pinNo, GPIO_PinState state)
{
	pinNo--;
	static GPIO_InitTypeDef GPIO_InitStruct = {0};
	const digital_pin_t* pin = &dIOPins[pinNo];
	GPIO_InitStruct.Pin = pin->pinMask;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	if (dIOState == DIO_IN && pinNo < 8)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	}
	else
	{
		HAL_GPIO_WritePin(pin->GPIO, pin->pinMask, state);
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	}
  HAL_GPIO_Init(pin->GPIO, &GPIO_InitStruct);
	return pin;
}

/*! @brief set the IO pins as input, Pin 9 is always output */
void DIOController_SetAsInput(void)
{
	// set the io buffer as input
	HAL_GPIO_WritePin(dirControlPin.GPIO, dirControlPin.pinMask, GPIO_PIN_SET);
	dIOState = DIO_IN;
	static GPIO_InitTypeDef GPIO_InitStruct = {0};
	for (int i = 0; i < DIO_COUNT; i++)
	{
		const digital_pin_t* pin = &dIOPins[i];
		GPIO_InitStruct.Pin = pin->pinMask;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(pin->GPIO, &GPIO_InitStruct);
	}
}

/*!
@brief set the IO pins as output, Pin 9 is always output
@note Initial value for output pin is 0
*/
void DIOController_SetAsOutput(void)
{
	static GPIO_InitTypeDef GPIO_InitStruct = {0};
	for (int i = 0; i < DIO_COUNT; i++)
	{
		const digital_pin_t* pin = &dIOPins[i];
		GPIO_InitStruct.Pin = pin->pinMask;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_WritePin(pin->GPIO, pin->pinMask, GPIO_PIN_RESET);
		HAL_GPIO_Init(pin->GPIO, &GPIO_InitStruct);
	}
	// set the io buffer as input
	HAL_GPIO_WritePin(dirControlPin.GPIO, dirControlPin.pinMask, GPIO_PIN_RESET);
	dIOState = DIO_OUT;
}
/*!
 * @brief set the value of output port, sets all gpio ports at output
 * @param val- value to be set
 */
void DIOController_SetPortValue(uint32_t val)
{
	for(int i = 0; i < DIO_COUNT + 1; i++)
	{
		const digital_pin_t* pin = &dIOPins[i];
		HAL_GPIO_WritePin(pin->GPIO, pin->pinMask, (GPIO_PinState)(val & 0x1));
		val = val >> 1U;
	}
}
/*!
 * @brief set the value of output port, sets all gpio ports at output
 * @returns val- value on the port
 */
uint32_t DIOController_GetPortValue(void)
{
	uint32_t val = 0;
	for(int i = DIO_COUNT-1; i > 0; i--)
	{
		const digital_pin_t* pin = &dIOPins[i];
		val |= (uint32_t)HAL_GPIO_ReadPin(pin->GPIO, pin->pinMask);
		val = val << 1U;
	}
	val |= (uint32_t)HAL_GPIO_ReadPin(dIOPins[0].GPIO, dIOPins[0].pinMask);
	return val;
}
/* EOF */
