/*!
@file Inverter3Ph.c
@brief file for control of three phase inverter
@details

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "DoutController.h"
#include "PWMDriver.h"
#include "Inverter3Ph.h"
#include "Control.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define PI    										(3.141593f)
#define TWO_PI 										(2 * PI)
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
static inverter3Ph_config_t handles[INVERTER_3PH_COUNT];
static int handleCounter = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
inverter3Ph_config_t* Inverter3Ph_Init(inverter3Ph_init_config_t* initConfig)
{
	/* if not enough inverter slots return null */
	if((handleCounter + 1) > INVERTER_3PH_COUNT)
		return NULL;

	/* an even output pin should be used as inverted signals are required for the pwm */
	if((initConfig->pins[0] % 2 == 0) || (initConfig->pins[1] % 2 == 0) || (initConfig->pins[2] % 2 == 0))
		return NULL;

	inverter3Ph_config_t* handle = &handles[handleCounter++];

	/* check module usage */
	bool pwm1_10_needed = initConfig->pins[0] < 11 ||  initConfig->pins[1] < 11 || initConfig->pins[2] < 11;
	bool pwm11_16_needed = initConfig->pins[0] > 10 ||  initConfig->pins[1] > 10 || initConfig->pins[2] > 10;

	handle->pairConfig.alignment = initConfig->alignment;
	handle->pairConfig.dtEnabled = initConfig->deadtimeEnable;
	handle->pairConfig.dtInNanoSec = initConfig->deadtimeInNanosec;
	handle->pairConfig.periodInUsec = initConfig->periodInUs;
	handle->pairConfig.interruptEnabled = initConfig->interruptEnabled;

	// configure PWM
	for (int i = 0; i < 3; i++)
	{
		handle->fncs[i] = PWMDriver_ConfigPair(initConfig->pins[i], &handle->pairConfig);
		PWMDriver_UpdatePair(initConfig->pins[i], 0.5f, &handle->pairConfig);
		handle->doutPins[i] = initConfig->pins[i];
		handle->pairConfig.interruptEnabled = false;
	}

	/* select pin functionality at end */
	for (int i = 0; i < 3; i++)
	{
		DoutController_SelectMode(initConfig->pins[i], PWM_FNC_PWM, GPIO_PIN_RESET);
		DoutController_SelectMode(initConfig->pins[i]+1, PWM_FNC_PWM, GPIO_PIN_RESET);
	}
	if (initConfig->dsblPin)
		handle->dsblePins = DoutController_SelectMode(initConfig->dsblPin, PWM_FNC_IO, GPIO_PIN_RESET);

	return handle;
}

void Inverter3Ph_UpdateSPWM(inverter3Ph_config_t* config, float theta, float modulationIndex)
{
	float duties[3];
	GenerateSPWM(theta, modulationIndex, duties);
	for (int i = 0; i < 3; i++)
		config->fncs[i](config->doutPins[i], duties[i], &config->pairConfig);
}

void Inverter3Ph_UpdateDuty(inverter3Ph_config_t* config, float* duties)
{
	for (int i = 0; i < 3; i++)
		config->fncs[i](config->doutPins[i], duties[i], &config->pairConfig);
}

/* EOF */
