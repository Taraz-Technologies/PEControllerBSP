/**
 ********************************************************************************
 * @file 		main_controller.h
 * @author 		Waqas Ehsan Butt
 * @date 		December 7, 2021
 *
 * @brief
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
#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

/** @addtogroup ApplicationExamples Application Examples
 * @{
 */

/** @defgroup PWMGeneration PWM Generation
 * @brief Describes the different use cases for PWM generation and synchronization
 * @details
 * The following use cases are presented.
 * -# <b>Switch 1-2</b>: Presents the scheme to synchronize the PWM generation which
 * resets whenever Compare 1 unit of the master HRTIM is encountered.
 * -# <b>Switch 3-6</b>: Presents the scheme to create a H-Bridge where switches 3,6
 * get same waveform while switches 4,5 get the inverted PWM. Moreover the phase
 * shift is added between 3 and 1 which can be controlled via the value of
 * compare 2 and compare 2 units.
 * -# <b>Switch 7-8</b>: Presents the scheme to synchronize a HRTIM submodule to
 * the Fiber Tx signal and generating the Fiber Tx signal to synchronize slave PEControllers.
 * -# <b>Switch 9-16</b>: Presents 2 H-Bridges with 8 switches showing the mechanism to synchronize
 * with Fiber Rx signal and also synchronizing TIM1 and HRTIM submodules.
 * @{
 */
/**
 * @}
 */
/**
 * @}
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/

 /*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * @brief Initialize the main control loop
 */
void MainControl_Init(void);
/**
 * @brief Call this function to start generating PWM signals
 */
void MainControl_Run(void);
/**
 * @brief Call this function to stop the control loop from generating PWM signals
 */
void MainControl_Stop(void);
/**
 * @brief Call this function to process the control loop.
 * If the new computation request is available new duty cycle values are computed and applied to all inverter legs
 */
void MainControl_Loop(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
