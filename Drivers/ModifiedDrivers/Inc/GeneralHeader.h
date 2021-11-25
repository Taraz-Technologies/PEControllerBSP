/**
 ********************************************************************************
 * @file 		GeneralHeader.h
 * @author 		Waqas Ehsan Butt
 * @date 		Nov 10, 2021
 *
 * @brief General header file defining basic items to include in every component file
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
#ifndef GENERAL_HEADER_H
#define GENERAL_HEADER_H

/** @defgroup PEController_Framework_Drivers
 * @brief This module defines the framework drivers for PEController
 * @{
 */

/**
 * @}
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "stm32h7xx_hal.h"
#include <stdbool.h>
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedefs
 ******************************************************************************/

/*******************************************************************************
 * Structures
 ******************************************************************************/

 /*******************************************************************************
 * Exported Variables
 ******************************************************************************/

/*******************************************************************************
 * Global Function Prototypes
 ******************************************************************************/
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
extern void Error_Handler(void);
/*******************************************************************************
 * Code
 ******************************************************************************/

#endif
/* EOF */
