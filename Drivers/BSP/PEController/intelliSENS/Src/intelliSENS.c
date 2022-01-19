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
#include "intelliSENS.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static void* hpcd_USB_OTG_HS = 0x0000000010044c08;
static void (*hal_pcd_irqhandler)(void*) = 0x0000000008100f9f;
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
static void Init(int periodInUs, const float* mults, const float* offsets)
{

}

static void Poll()
{

}

static bool SetADCData(uint64_t* data)
{
	return true;
}

static void SetADCTicks(uint16_t ticks)
{

}

void intelliSENS_Configure(void)
{
	intelliSENS.Init = 0x0000000008106f21;//Init;//0x00000000081f3f80;//Init;//0x00000000081f6c4c;//0x00000000081f6c50;
	intelliSENS.Poll = 0x000000000810738d;//Poll;//0x00000000081f70b4;//0x00000000081f70bc;
	intelliSENS.SetADCData = 0x0000000008106f49;//0x0000000008106f47;//(bool (*)(uint64_t*))0x0000000008106fa7;//SetADCData;//0x00000000081f3fa4;//SetADCData;//0x00000000081f6c70;//0x00000000081f6c78;
	intelliSENS.SetADCTicks = 0x00000000081073e1;//0x000000000810743f;//SetADCTicks;//0x00000000081f7108;//0x00000000081f7110;
}


void HAL_IncTick(void)
{
  uwTick += (uint32_t)uwTickFreq;
  ((void (*)(void))0x000000000810069d)();
}


/**
  * @brief This function handles USB On The Go HS global interrupt.
  */
void OTG_HS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_HS_IRQn 0 */

  /* USER CODE END OTG_HS_IRQn 0 */
  hal_pcd_irqhandler(hpcd_USB_OTG_HS);
  /* USER CODE BEGIN OTG_HS_IRQn 1 */

  /* USER CODE END OTG_HS_IRQn 1 */
}


/* EOF */
