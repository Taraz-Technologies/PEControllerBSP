/*!
@file Inverter3Ph.h
@brief
@details

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
#ifndef INVERTER_3PH_H
#define INVERTER_3PH_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "GeneralHeader.h"
#include "PWMDriver.h"
#include "DoutController.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define INVERTER_3PH_COUNT								(2)
/*******************************************************************************
 * Enums
 ******************************************************************************/
typedef void (*Inverter3Ph_ResetCallback)(void);
/*******************************************************************************
 * Structs
 ******************************************************************************/
typedef struct
{
	dout_pin_t* phasePins[3];
	const dout_pin_t* dsblePins;
	uint16_t doutPins[3];
	pwm_pair_config_t pairConfig;
	UpdatePWMPair fncs[3];
} inverter3Ph_config_t;
typedef struct
{
	uint16_t pins[3]; // uH, vH, wH,
	uint16_t dsblPin;
	uint16_t periodInUs;
	bool interruptEnabled;
	Inverter3Ph_ResetCallback resetCallback;
//	void* callbackFnc();   use for interrupt driven
	pwm_alignment_t alignment;
	uint16_t deadtimeInNanosec;
	bool deadtimeEnable;
} inverter3Ph_init_config_t;
 /*******************************************************************************
 * Prototypes
 ******************************************************************************/
inverter3Ph_config_t* Inverter3Ph_Init(inverter3Ph_init_config_t* initConfig);
void Inverter3Ph_UpdateSPWM(inverter3Ph_config_t* config, float theta, float modulationIndex);
void Inverter3Ph_UpdateDuty(inverter3Ph_config_t* config, float* duties);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
