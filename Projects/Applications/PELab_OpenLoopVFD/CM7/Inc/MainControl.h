/*! 
@file MainControl.h
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
#ifndef MAIN_CONTROL_H
#define MAIN_CONTROL_H
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
 * @brief Initiates PWM for both inverters, and enable Disable signals and configure the relays
 */
void MainControl_Init(void);
/**
 * @brief Call this function to run both inverter PWMs
 */
void MainControl_Run(void);
/**
 * @brief Call this function to stop the inverters
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
