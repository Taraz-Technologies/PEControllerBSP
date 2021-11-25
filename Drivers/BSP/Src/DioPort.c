/**
 ********************************************************************************
 * @file   DioPort.c
 * @author Waqas Ehsan Butt
 * @date    25 September 2021
 * @copyright TarazTechnologies Pvt. Ltd.
 *
 * @brief This file controls the IO Port of the PEController module
 ********************************************************************************
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "DioPort.h"
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
/**
 * @brief Initialize the Pin with default parameters
 * @param pinNo Pin number of the specified pin
 * @param *GPIO_InitStruct Pointer to the GPIO Structure
 * @param state Output State
 * @return digital_pin_t* pointer to the pin structure
 */
static const digital_pin_t* InitPin(uint32_t pinNo, GPIO_InitTypeDef* GPIO_InitStruct, GPIO_PinState state)
{
	const digital_pin_t* pin = &dIOPins[pinNo - 1];
	GPIO_InitStruct->Pin = pin->pinMask;
	GPIO_InitStruct->Pull = GPIO_NOPULL;
	GPIO_InitStruct->Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_WritePin(pin->GPIO, pin->pinMask, state);
	HAL_GPIO_Init(pin->GPIO, GPIO_InitStruct);
	return pin;
}
/**
 * @brief Set the Dio pin as GPIO
 * @param pinNo Dio Pin No ( Range 1 - 8)
 * @param state State of the pin
 * @return *pin Pointer to the Dio pin structure
 */
const digital_pin_t* Dio_SetAsIOPin(uint32_t pinNo, GPIO_PinState state)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = dIOState == DIO_IN ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT_PP;
	return InitPin(pinNo, &GPIO_InitStruct, state);
}

/**
 * @brief Selects the Alternate Output Functionality. To configure as IO use Dio_SetAsIOPin(pinNo, state)
 * @param pinNo Dio pin No (Range 1-16)
 * @param AlternateFunction Alternate Functionality to be used
 * @return *digital_pin_t pointer to the pin structure
 */
const digital_pin_t* Dio_SetPinAlternateFunction(uint32_t pinNo, uint32_t AlternateFunction)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = AlternateFunction;
	return InitPin(pinNo, &GPIO_InitStruct, GPIO_PIN_RESET);
}
/**
 * @brief Set the IO Port as Input. Pin 9 is always output
 *
 */
void Dio_SetAsInputPort(void)
{
	// set the io buffer as input
	HAL_GPIO_WritePin(dirControlPin.GPIO, dirControlPin.pinMask, GPIO_PIN_SET);
	dIOState = DIO_IN;
	static GPIO_InitTypeDef GPIO_InitStruct = {0};
	for (int i = 0; i < DIO_COUNT; i++)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		InitPin(i + 1, &GPIO_InitStruct, GPIO_PIN_RESET);
	}
	GPIO_InitStruct.Pin = dirControlPin.pinMask;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(dirControlPin.GPIO, &GPIO_InitStruct);
}
/**
 * @brief Set the IO Port as Output. Pin 9 is always output
 * @note Initial value for output pin is 0
 *
 */
void Dio_SetAsOutputPort(void)
{
	static GPIO_InitTypeDef GPIO_InitStruct = {0};
	for (int i = 0; i < DIO_COUNT; i++)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		InitPin(i + 1, &GPIO_InitStruct, GPIO_PIN_RESET);
	}
	// set the io buffer as input
	HAL_GPIO_WritePin(dirControlPin.GPIO, dirControlPin.pinMask, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = dirControlPin.pinMask;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(dirControlPin.GPIO, &GPIO_InitStruct);
	dIOState = DIO_OUT;
}
/*!
 * @brief Set the value of output port
 * @param val- value to be set
 */
void Dio_SetPortValue(uint32_t val)
{
	for(int i = 0; i < DIO_COUNT + 1; i++)
	{
		const digital_pin_t* pin = &dIOPins[i];
		HAL_GPIO_WritePin(pin->GPIO, pin->pinMask, (GPIO_PinState)(val & 0x1));
		val = val >> 1U;
	}
}
/*!
 * @brief Gets the value of the input port
 * @returns val- value on the port
 */
uint32_t Dio_GetPortValue(void)
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
