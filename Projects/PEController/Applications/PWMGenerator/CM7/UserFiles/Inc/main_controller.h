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
 * -# <b>PWM 1-2 (LEG 1)</b>: Presents the scheme to synchronize the PWM generation which
 * resets whenever Compare 1 unit of the master HRTIM is encountered.
 * -# <b>PWM 3-6 (LEG 2, 3)</b>: Presents the scheme to create a H-Bridge where switches 3,6
 * get same waveform while switches 4,5 get the inverted PWM. Moreover the phase
 * shift is added between 3 and 1 which can be controlled via the value of
 * compare 1 and compare 2 units of master HRTIM. In this example the phase shift is updated every MainControl_Loop().
 * -# <b>PWM 7-8 (LEG 4)</b>: Presents the scheme to synchronize a HRTIM submodule to
 * the Fiber Tx signal and generating the Fiber Tx signal to synchronize slave PEControllers.
 * -# <b>PWM 9-16 (LEG 5, 8)</b>: Presents 2 H-Bridges with 8 switches showing the mechanism to synchronize
 * with Fiber Rx signal as slave and also synchronizing TIM1 and HRTIM submodules.
 * The phase shift can be controlled from the display screen. Value should be between 0-180 degrees<br><br>
 *
 *
 * \image html "PWM Generation Description.png"
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
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
