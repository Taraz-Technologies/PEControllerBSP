/**
 ********************************************************************************
 * @file    	intelliSENS.c
 * @author 		Waqas Ehsan Butt
 * @date    	Jan 17, 2022
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

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
#if ENABLE_INTELLISENS
#include "intelliSENS.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define INIT_FNC			((void (*) (int periodInUs, const float* mults, const float* offsets))0x0000000008103b5d)
#define SET_DATA_FNC		((bool (*) (uint64_t* data))0x0000000008103b6d)
#define POLL_FNC			((void (*) (void))0x0000000008103c01)
#define SET_TICKS_FNC		((void (*) (uint16_t ticks))0x0000000008103dc1)

#define HAL_INIT_FNC		((void (*) (void))0x000000000810052d)
#define HAL_TICK_FNC		((void (*) (void))0x000000000810060d)

#define USB_IRQ_FNC			((void (*) (void* handle))0x0000000008100c15)
#define USB_IRQ_HANDLE		((void*)0x0000000010044b94)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static bool moduleEnabled = false;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
intelliSENS_t intelliSENS;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
void intelliSENS_Configure(void)
{
	intelliSENS.Init = INIT_FNC;
	intelliSENS.Poll = POLL_FNC;
	intelliSENS.SetADCData = SET_DATA_FNC;
	intelliSENS.SetADCTicks = SET_TICKS_FNC;
	if (!moduleEnabled)
	{
		HAL_INIT_FNC();
		moduleEnabled = true;
	}
}

void HAL_IncTick(void)
{
  uwTick += (uint32_t)uwTickFreq;
  if (moduleEnabled)
	  HAL_TICK_FNC();
}

/**
  * @brief This function handles USB On The Go HS global interrupt.
  */
void OTG_HS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_HS_IRQn 0 */

  /* USER CODE END OTG_HS_IRQn 0 */
	USB_IRQ_FNC(USB_IRQ_HANDLE);
  /* USER CODE BEGIN OTG_HS_IRQn 1 */

  /* USER CODE END OTG_HS_IRQn 1 */
}
#endif

/* EOF */
