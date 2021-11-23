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

/*******************************************************************************
 * Enums
 ******************************************************************************/
/**
 * @brief Definitions of Switch Legs
 */
typedef enum
{
	LEG_DEFAULT,            /**< Define the leg as a default leg */
	LEG_TNPC,               /**< Define the leg as a TNPC leg */
	LEG_RESERVED,			/**< Final Value Not Used */
} switch_leg_t;
/*******************************************************************************
 * Structures
 ******************************************************************************/
/**
 * @brief Defines the 3 Phase Inverter parameters
 */
typedef struct
{
	uint16_t s1PinNos[3];						/**< @brief Collection of the Pin numbers of the first PWM switch
													in a leg of the 3-Phase System. The remaining PWMs are the
													consecutive pins after this pin */
	uint16_t dsblPinNo;							/**< @brief Pin no of the first disable pin if any. If more than one
	 	 	 	 	 	 	 	 	 	 	 	 	 the remaining disable pins should be consecutively after this pin */
	uint16_t dsblPinCount;						/**< @brief No of disable pins used by the system.
													These pins will be used to disable the relevant gate drivers */
	switch_leg_t legType;						/**< @brief Type of switch legs used for switching the inverter */
	pwm_config_t pwmConfig;						/**< @brief The PWM configurations */
	DutyCycleUpdateFnc updateCallbacks[3];		/**< @brief These call backs are used by the drivers to update
													the duty cycles of all switches according to the configuration */
} inverter3Ph_config_t;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * @brief Initialize an inverter module
 *
 * @param *config Pointer to the Inverter Configurations
 * @return *inverter3Ph_config_t handle representing the inverter
 */
void Inverter3Ph_Init(inverter3Ph_config_t* config);
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
