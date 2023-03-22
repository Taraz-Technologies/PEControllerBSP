/**
 ********************************************************************************
 * @file    	pecontroller_ts.c
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
#include "mxt_drivers.c"
#include "pecontroller_ts.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define TS_I2C_ADDRESS				(0x4A << 1)
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
static bool initComplete = false;
static TS_StateTypeDef tsState;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
TS_BSPTypeDef ts_bsp_drv;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Prepare register for communication
 * @param reg Required register
 * @return Value to be sent for register
 */
static inline uint16_t GetReg(uint16_t reg)
{
	return ((reg & 0xff) << 8) | ((reg & 0xff00) >> 8);
}
/**
 * @brief Write register in the touch screen controller
 * @param addr Device address
 * @param reg Register to be written
 * @param value Value to be written
 * @return <c>HAL_OK</c> if successful else error
 */
static uint16_t TS_Write(uint8_t addr, uint16_t reg, uint8_t value)
{
	return HAL_I2C_Mem_Write(&hi2c2, addr, GetReg(reg), I2C_MEMADD_SIZE_16BIT, &value, 1, 100);
}

/**
 * @brief Read register from the touch screen controller
 * @param addr Device address
 * @param reg Register to be read
 * @return Register value if successful else 0.
 */
static uint8_t TS_Read(uint8_t addr, uint16_t reg)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t  Value = 0;
	int retry_count = 2;
	do {
		status = HAL_I2C_Mem_Read(&hi2c2, addr, GetReg(reg), I2C_MEMADD_SIZE_16BIT, &Value, 1, 1000);
	} while(status != HAL_OK && retry_count-- >= 0);

	if(status == HAL_OK)
		return Value;
	else
		return 0;
}
/**
 * @brief Read multiple registers from the touch screen controller
 * @param addr Device address
 * @param reg Register to be read
 * @param buffer Pointer to data buffer to be filled
 * @param len Length of data to be read
 * @return <c>HAL_OK</c> if successful else error
 */
static uint16_t TS_ReadMultiple(uint8_t addr, uint16_t reg, uint8_t *buffer, uint16_t len)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint16_t wait_ms = len;// / 4;
	if (wait_ms == 0)
		wait_ms = 1;
	int retry_count = 2;
	do
	{
		status = HAL_I2C_Mem_Read(&hi2c2, addr, GetReg(reg), I2C_MEMADD_SIZE_16BIT, buffer, len, 1000);
	} while(status != HAL_OK && retry_count-- >= 0);
	return status;
}
/**
 * @brief Insert delay
 * @param delay Delay to be inserted in msecs
 */
static void TS_Delay(uint32_t delay)
{
	HAL_Delay(delay);
}
/**
 * @brief Configure relevant clocks
 */
static void ConfigClock(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
	PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_I2C2_CLK_ENABLE();
}
/**
 * @brief Configure relevant IOs
 */
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

	HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET);
}
/**
 * @brief Configure the I2C interface
 */
static void ConfigI2C(void)
{
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x10707DBC;
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
}
/**
 * @brief Perform board related initializations of the touch screen controller
 */
static void TS_Init(void)
{
	static bool isActivated = false;
	if (!isActivated)
	{
		ConfigClock();
		ConfigI2C();
		ConfigIO();
		isActivated = true;
	}
}
/**
 * @brief Reset the touch screen controller
 */
static void TS_Reset(void)
{
	HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
}

/**
 * @brief Initialize the touch screen controller
 * @param ts_SizeX Horizontal Size of the touch screen
 * @param ts_SizeY Vertical Size of the touch screen
 * @return <c>TS_OK</c> if initialization is successful else <c>TS_DEVICE_NOT_FOUND</c>
 */
uint8_t BSP_TS_Init(uint16_t ts_SizeX, uint16_t ts_SizeY)
{
	ts_bsp_drv.Init = TS_Init;
	ts_bsp_drv.Reset = TS_Reset;
	ts_bsp_drv.Read = TS_Read;
	ts_bsp_drv.ReadMultiple = TS_ReadMultiple;
	ts_bsp_drv.Write = TS_Write;
	ts_bsp_drv.Delay = TS_Delay;

	TS_Init();
	TS_Reset();
	if(MXTDrivers_Init(800, 480) == HAL_OK)
	{
		initComplete = true;
		return TS_OK;
	}
	return TS_DEVICE_NOT_FOUND;
}

/**
 * @brief  Returns status and positions of the touch screen.
 * @return  TS_State: Pointer to touch screen state structure
 */
TS_StateTypeDef* BSP_TS_GetState(void)
{
	return &tsState;
}

/**
 * @brief Poll the touch screen drivers to get touch events
 */
void BSP_TS_Poll(void)
{
	if (initComplete == false)
	{
		tsState.touchDetected = false;
		tsState.touchX = 0;
		tsState.touchY = 0;
		return;
	}
	volatile uint16_t x;
	volatile uint16_t y;
	tsState.touchDetected = MXTDrivers_GetState(&x, &y);
	tsState.touchX = x;
	tsState.touchY = y;
	return;
}

#endif
/* EOF */
