/*! 
@file DoutConfig.h
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
#ifndef DOUT_CONFIG_H
#define DOUT_CONFIG_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "PEControllerConfig.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define DOUT_OUTPUT 		(0)
#define DOUT_PWM 			(1)
#define DOUT_PWM_COMP 		(2)
#define DOUT_DISABLED 		(3)

#if APP_CONFIG == APP_PELAB_TNPC || APP_CONFIG == APP_PELAB_MMC
#if MODULE_COUNT > 4 || MODULE_COUNT <= 0
#error ("MODULE_COUNT incorrect");
#else
#define DOUT1_MODE			(DOUT_PWM)
#define DOUT2_MODE			(DOUT_PWM_COMP)
#define DOUT3_MODE			(DOUT_PWM)
#define DOUT4_MODE			(DOUT_PWM_COMP)
#if MODULE_COUNT > 1
#define DOUT5_MODE			(DOUT_PWM)
#define DOUT6_MODE			(DOUT_PWM_COMP)
#define DOUT7_MODE			(DOUT_PWM)
#define DOUT8_MODE			(DOUT_PWM_COMP)
#elif MODULE_COUNT > 2
#define DOUT9_MODE			(DOUT_PWM)
#define DOUT10_MODE			(DOUT_PWM_COMP)
#define DOUT11_MODE			(DOUT_PWM)
#define DOUT12_MODE			(DOUT_PWM_COMP)
#elif MODULE_COUNT > 3
#define DOUT13_MODE			(DOUT_PWM)
#define DOUT14_MODE			(DOUT_PWM_COMP)
#define DOUT15_MODE			(DOUT_PWM)
#define DOUT16_MODE			(DOUT_PWM_COMP)
#endif
#endif
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

/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
