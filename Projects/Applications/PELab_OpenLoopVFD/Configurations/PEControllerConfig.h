/*! 
@file PEControllerConfig.h
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
#ifndef PECONTROLLER_CONFIG_H
#define PECONTROLLER_CONFIG_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "STM32h7xx_hal.h"
#include <stdbool.h>
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define APP_PELAB_TNPC			(1)
#define APP_PELAB_MMC			(2)
#define APP_PELAB_VFD_3PH		(3)
#define APP_PELAB_VFD_6PH		(4)
#define APP_CUSTOM				(5)				
/*! @brief configure the type of application configuration from here */
#define APP_CONFIG				(APP_PELAB_VFD_6PH)

#if APP_CONFIG == APP_PELAB_TNPC
#define MODULE_COUNT			(4)
#elif APP_CONFIG == APP_PELAB_MMC
#define MODULE_COUNT			(3)
#elif APP_CONFIG == APP_PELAB_VFD_3PH
#define MODULE_COUNT			(1)
#elif APP_CONFIG == APP_PELAB_VFD_6PH
#define MODULE_COUNT			(2)
#else
	#error ("custom usage not yet defined!");	
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
