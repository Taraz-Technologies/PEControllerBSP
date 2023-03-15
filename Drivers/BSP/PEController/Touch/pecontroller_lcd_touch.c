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
static I2C_HandleTypeDef hi2c2;
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

static void ConfigClock(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
	PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_I2C2_CLK_ENABLE();
}

static void ConfigIO(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/**I2C2 GPIO Configuration
	    PH4     ------> I2C2_SCL
	    PH5     ------> I2C2_SDA
	 */
	GPIO_InitStruct.Pin = CTP_I2C_SCL_Pin|CTP_I2C_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	/*Configure GPIO pin : CTP_INT_Pin */
	GPIO_InitStruct.Pin = CTP_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(CTP_INT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : CTP_RST_Pin */
	GPIO_InitStruct.Pin = CTP_RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CTP_RST_GPIO_Port, &GPIO_InitStruct);
}

static void ConfigI2C(void)
{
	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x307075B1;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */
}

void BSP_Touch_Init(void)
{

}

void BSP_Touch_Poll(void)
{

}

#endif
/* EOF */