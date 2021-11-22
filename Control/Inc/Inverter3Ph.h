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
#include "DoutPort.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define INVERTER_3PH_COUNT								(2)
/*******************************************************************************
 * Enums
 ******************************************************************************/
/**
 * @brief Types of legs supported by the drivers
 *
 */
typedef enum
{
	LEG_DEFAULT,
	LEG_TNPC,
	LEG_RESERVED,			/* Add all leg types before this enumeration */
} switch_leg_t;
/*******************************************************************************
 * Structs
 ******************************************************************************/
typedef struct
{
	digital_pin_t* phasePins[3];
	const digital_pin_t* dsblePins;
	uint16_t doutPins[3];
	pwm_config_t pairConfig;
	PWMPairUpdateCallback fncs[3];
} inverter3Ph_config_t;
typedef struct
{
	uint16_t pins[3]; // uH, vH, wH,
	uint16_t dsblPin;
	uint16_t periodInUs;
	bool interruptEnabled;
	switch_leg_t legType;
	PWMResetCallback resetCallback;
	//	void* callbackFnc();   use for interrupt driven
	bool minMaxDutyCycleBalancing;
	pwm_alignment_t alignment;
	uint16_t deadtimeInNanosec;
	bool deadtimeEnable;
} inverter3Ph_init_config_t;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * @brief Initialize an inverter module
 *
 * @param *initConfig pointer to the initial configuration of the inverter
 * @return *inverter3Ph_config_t handle representing the inverter
 */
inverter3Ph_config_t* Inverter3Ph_Init(inverter3Ph_init_config_t* initConfig);
/**
 * @brief Update the duty cycles of the inverter by using SPWM configuration
 *
 * @param *config handle representing the inverter
 * @param *theta angle of phase u in radians
 * @param modulationIndex modulation index to be used for the generation
 */
void Inverter3Ph_UpdateSPWM(inverter3Ph_config_t* config, float theta, float modulationIndex);
/**
 * @brief Update the duty cycles of the inverter
 *
 * @param *config handle representing the inverter
 * @param *duties pointer to the three duty cycles of the inverter (Range 0-1)
 */
void Inverter3Ph_UpdateDuty(inverter3Ph_config_t* config, float* duties);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
