/*!
@file DoutManager.c
@brief
@details

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "DoutController.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define DOUT_COUNT						(16)
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
static const dout_pin_t doutPins[DOUT_COUNT] =
{
	{ .pin = {.GPIO = GPIOC, .pinMask = GPIO_PIN_6,}, .AlternateFnc = GPIO_AF1_HRTIM1 },
	{ .pin = {.GPIO = GPIOC, .pinMask = GPIO_PIN_7,}, .AlternateFnc = GPIO_AF1_HRTIM1 },
	{ .pin = {.GPIO = GPIOC, .pinMask = GPIO_PIN_8,}, .AlternateFnc = GPIO_AF1_HRTIM1 },
	{ .pin = {.GPIO = GPIOA, .pinMask = GPIO_PIN_8,}, .AlternateFnc = GPIO_AF2_HRTIM1 },
	{ .pin = {.GPIO = GPIOA, .pinMask = GPIO_PIN_9,}, .AlternateFnc = GPIO_AF2_HRTIM1 },
	{ .pin = {.GPIO = GPIOA, .pinMask = GPIO_PIN_10}, .AlternateFnc = GPIO_AF2_HRTIM1 },
	{ .pin = {.GPIO = GPIOA, .pinMask = GPIO_PIN_11}, .AlternateFnc = GPIO_AF2_HRTIM1 },
	{ .pin = {.GPIO = GPIOA, .pinMask = GPIO_PIN_12}, .AlternateFnc = GPIO_AF2_HRTIM1 },
	{ .pin = {.GPIO = GPIOG, .pinMask = GPIO_PIN_6}, .AlternateFnc = GPIO_AF2_HRTIM1 },
	{ .pin = {.GPIO = GPIOG, .pinMask = GPIO_PIN_7}, .AlternateFnc = GPIO_AF2_HRTIM1 },
	{ .pin = {.GPIO = GPIOE, .pinMask = GPIO_PIN_9}, .AlternateFnc = GPIO_AF1_TIM1 },
	{ .pin = {.GPIO = GPIOE, .pinMask = GPIO_PIN_8}, .AlternateFnc = GPIO_AF1_TIM1 },
	{ .pin = {.GPIO = GPIOJ, .pinMask = GPIO_PIN_11}, .AlternateFnc = GPIO_AF1_TIM1 },
	{ .pin = {.GPIO = GPIOJ, .pinMask = GPIO_PIN_10}, .AlternateFnc = GPIO_AF1_TIM1 },
	{ .pin = {.GPIO = GPIOJ, .pinMask = GPIO_PIN_9}, .AlternateFnc = GPIO_AF1_TIM1 },
	{ .pin = {.GPIO = GPIOE, .pinMask = GPIO_PIN_12}, .AlternateFnc = GPIO_AF1_TIM1 },
};
/*******************************************************************************
 * Code
 ******************************************************************************/
const dout_pin_t* DoutController_SelectMode(uint32_t pinNo, pwm_fnc_t fnc, GPIO_PinState state)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	pinNo--;
	const dout_pin_t* pin = &doutPins[pinNo];
	GPIO_InitStruct.Pin = pin->pin.pinMask;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	if (fnc == PWM_FNC_IO)
	{
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_WritePin(pin->pin.GPIO, pin->pin.pinMask, state);
	}
	else
	{
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = pin->AlternateFnc;
	}
  HAL_GPIO_Init(pin->pin.GPIO, &GPIO_InitStruct);
	return pin;
}
/*!
 * @brief set the whole port as GPIO
 */
void DoutController_SetPortAsGPIO(void)
{
	for(int i = 0; i < DOUT_COUNT; i++)
		DoutController_SelectMode(i + 1, PWM_FNC_IO, GPIO_PIN_RESET);
}
/*!
 * @brief set the value of output port, sets all gpio ports at output
 * @param val- value to be set
 */
void DoutController_SetPortValue(uint32_t val)
{
	for(int i = 0; i < DOUT_COUNT; i++)
	{
		const dout_pin_t* pin = &doutPins[i];
		HAL_GPIO_WritePin(pin->pin.GPIO, pin->pin.pinMask, (GPIO_PinState)(val & 0x1));
		val = val >> 1U;
	}
}

/* EOF */
