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
/**
 * @brief
 *
 * @param pwmNo Channel no of the first PWM Channel in the pair (Valid Values 1,3,5,7,9,11,13)
 * 				 Channel1 = pwmNo
 * 				 Channel2 = pwmNo + 1
 * 				 Channel3 = pwmNo + 2
 * 				 Channel3 = pwmNo + 3
 * @param duty duty cycle to be applied to the pair (Range 0 - 1)
 * @param *config Pointer to a  pwm_pair_config_t structure that contains the configuration
 * 				   parameters for the PWM pair
 */
static void TnpcPWM_UpdatePair(uint32_t pwmNo, float duty, pwm_pair_config_t *config)
{
	duty = (fabsf(duty - 0.5f)) * 2;
	if(duty < 0)
	{
		PWMDriver_UpdatePair(pwmNo, 0, config);
		PWMDriver_UpdatePair(pwmNo + 2, duty, config);
	}
	else
	{
		PWMDriver_UpdatePair(pwmNo, duty, config);
		PWMDriver_UpdatePair(pwmNo + 2, 0, config);
	}
}

/**
 * @brief Configure the PWMs for a single leg of the inverter
 *
 * @param pwmNo Channel no of the initial switch of the leg.
 * @param *initConfig pointer to the initial configuration of the inverter
 * @param *handle  pointer to the handle of the inverter
 * @return UpdatePWMPair Returns the function pointer of the type UpdatePWMPair which needs to be called
 * 						  whenever the duty cycles of the leg needs to be updated
 */
static UpdatePWMPair ConfigSingleLeg(uint16_t pwmNo, inverter3Ph_init_config_t* initConfig, inverter3Ph_config_t* handle)
{
	UpdatePWMPair callback = NULL;
	callback = PWMDriver_ConfigPair(pwmNo, &handle->pairConfig);
	DoutController_SelectMode(pwmNo, PWM_FNC_PWM, GPIO_PIN_RESET);
	DoutController_SelectMode(pwmNo + 1, PWM_FNC_PWM, GPIO_PIN_RESET);
	/* for Tnpc use four switches */
	if (initConfig->legType == LEG_TNPC)
	{
		PWMDriver_ConfigPair(pwmNo + 2, &handle->pairConfig);
		callback = TnpcPWM_UpdatePair; 				/* use this function to update all 4 switch duty cycles */
		DoutController_SelectMode(pwmNo + 2, PWM_FNC_PWM, GPIO_PIN_RESET);
		DoutController_SelectMode(pwmNo + 3, PWM_FNC_PWM, GPIO_PIN_RESET);
	}
	return callback;
}

/**
 * @brief Initialize an inverter module
 *
 * @param *initConfig pointer to the initial configuration of the inverter
 * @return *inverter3Ph_config_t handle representing the inverter
 */
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
		handle->fncs[i] = ConfigSingleLeg(initConfig->pins[i], initConfig, handle);
		handle->fncs[i](initConfig->pins[i], 0.5f, &handle->pairConfig);
		handle->doutPins[i] = initConfig->pins[i];
		handle->pairConfig.interruptEnabled = false;			/* --todo-- */
	}

	/* disable pin setting should include multiple disable pins --todo-- */
	if (initConfig->dsblPin)
		handle->dsblePins = DoutController_SelectMode(initConfig->dsblPin, PWM_FNC_IO, GPIO_PIN_RESET);

	return handle;
}

/**
 * @brief Update the duty cycles of the inverter
 *
 * @param *config handle representing the inverter
 * @param *duties pointer to the three duty cycles of the inverter (Range 0-1)
 */
void Inverter3Ph_UpdateDuty(inverter3Ph_config_t* config, float* duties)
{
	for (int i = 0; i < 3; i++)
		config->fncs[i](config->doutPins[i], duties[i], &config->pairConfig);
}

/**
 * @brief Update the duty cycles of the inverter by using SPWM configuration
 *
 * @param *config handle representing the inverter
 * @param *theta angle of phase u in radians
 * @param modulationIndex modulation index to be used for the generation
 */
void Inverter3Ph_UpdateSPWM(inverter3Ph_config_t* config, float theta, float modulationIndex)
{
	float duties[3];
	GenerateSPWM(theta, modulationIndex, duties);
	Inverter3Ph_UpdateDuty(config, duties);
}


/* EOF */
