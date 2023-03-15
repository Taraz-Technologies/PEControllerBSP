/**
 ********************************************************************************
 * @file    	pecontroller_lcd_touch.c
 * @author 		Waqas Ehsan Butt
 * @date    	Mar 15, 2023
 *
 * @brief   
 ********************************************************************************
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
#if LCD_DATA_MONITORING
#include "pecontroller_lcd_touch.h"
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

/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
static inline uint16_t GetReg(uint16_t reg)
{
	return ((reg & 0xff) << 8) | ((reg & 0xff00) >> 8);
}

void BSP_Touch_WriteReg(uint8_t addr, uint16_t reg, uint8_t value)
{
	HAL_I2C_Mem_Write(&hi2c2, addr, GetReg(reg), I2C_MEMADD_SIZE_16BIT, &value, 1, 100);
}

uint8_t  BSP_Touch_ReadReg(uint8_t addr, uint16_t reg)
{
	HAL_StatusTypeDef status = HAL_OK;
  uint8_t  Value = 0;
	uint16_t index = 0;
	do {
		status = HAL_I2C_Mem_Read(&hi2c2, addr, GetReg(reg), I2C_MEMADD_SIZE_16BIT, &Value, 1, HAL_MAX_DELAY);
	} while(status != HAL_OK && index < 256);

	if(status == HAL_OK)
		return Value;
	else
		return 0;
}

uint16_t BSP_Touch_ReadRegisters(uint8_t addr, uint16_t reg, uint8_t *buffer, uint16_t len)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint16_t index = 0;
	do
	{
		status = HAL_I2C_Mem_Read(&hi2c2, addr, GetReg(reg), I2C_MEMADD_SIZE_16BIT, buffer, len, HAL_MAX_DELAY);
	} while(status != HAL_OK && index++ < 256);
	return status;
}

void BSP_Touch_Init(void)
{

}

void BSP_Touch_Poll(void)
{

}

#endif
/* EOF */
