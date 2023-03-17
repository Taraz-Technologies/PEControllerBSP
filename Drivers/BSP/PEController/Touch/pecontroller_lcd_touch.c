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
#include "mxt_drivers.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define TS_I2C_ADDRESS				(0x4A << 1)
#define TS_INT_EN					(0)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static TS_DrvTypeDef *ts_driver;
static uint8_t  ts_orientation;
static uint8_t  I2C_Address = TS_I2C_ADDRESS;
static I2C_HandleTypeDef hi2c2;
TS_BSPTypeDef ts_bsp_drv;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
/* Table for touchscreen event information display on LCD : table indexed on enum @ref TS_TouchEventTypeDef information */
char * ts_event_string_tab[TOUCH_EVENT_NB_MAX] = { "None",
		"Press down",
		"Lift up",
		"Contact"
};

/* Table for touchscreen gesture Id information display on LCD : table indexed on enum @ref TS_GestureIdTypeDef information */
char * ts_gesture_id_string_tab[GEST_ID_NB_MAX] = { "None",
		"Move Up",
		"Move Right",
		"Move Down",
		"Move Left",
		"Zoom In",
		"Zoom Out"
};
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
extern void MXT336T_GetStateState(TS_StateTypeDef *TS_State);
/********************************************************************************
 * Code
 *******************************************************************************/
static inline uint16_t GetReg(uint16_t reg)
{
	return ((reg & 0xff) << 8) | ((reg & 0xff00) >> 8);
}

static void TS_Write(uint8_t addr, uint16_t reg, uint8_t value)
{
	HAL_I2C_Mem_Write(&hi2c2, addr, GetReg(reg), I2C_MEMADD_SIZE_16BIT, &value, 1, 100);
}

static uint8_t  TS_Read(uint8_t addr, uint16_t reg)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t  Value = 0;
	uint16_t index = 0;
	do {
		status = HAL_I2C_Mem_Read(&hi2c2, addr, GetReg(reg), I2C_MEMADD_SIZE_16BIT, &Value, 1, HAL_MAX_DELAY);
	} while(status != HAL_OK && ++index < 256);

	if(status == HAL_OK)
		return Value;
	else
		return 0;
}

static uint16_t TS_ReadMultiple(uint8_t addr, uint16_t reg, uint8_t *buffer, uint16_t len)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint16_t index = 0;
	do
	{
		status = HAL_I2C_Mem_Read(&hi2c2, addr, GetReg(reg), I2C_MEMADD_SIZE_16BIT, buffer, len, HAL_MAX_DELAY);
	} while(status != HAL_OK && index++ < 256);
	return status;
}
static void TS_Delay(uint32_t delay)
{
	HAL_Delay(delay);
}
static void ConfigClock(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
	PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_HSI; //RCC_I2C123CLKSOURCE_D2PCLK1;
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

	HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET);
}
static void ConfigI2C(void)
{
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x10707DBC;//0x307075B1;
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
static void TS_Reset(void)
{
	HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(CTP_RST_GPIO_Port, CTP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
}

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
	MXTDrivers_Init(800, 480);
	//MXT336T_Init();

	uint8_t ts_status = TS_OK;
#if 0
	uint8_t ts_id1, ts_id2 = 0;
	/* Note : I2C_Address is un-initialized here, but is not used at all in init function */
	/* but the prototype of Init() is like that in template and should be respected       */

	/* Initialize the communication channel to sensor (I2C) if necessary */
	/* that is initialization is done only once after a power up         */
	mXT336T_ts_drv.Init(I2C_Address);

	ts_id1 = mXT336T_ts_drv.ReadID(TS_I2C_ADDRESS);
	I2C_Address    = TS_I2C_ADDRESS;

	/* Scan TouchScreen IC controller ID register by I2C Read       */
	if(ts_id1 == MXT336T_FAMILY_ID_VALUE)
	{
		/* Found FT6206 : Initialize the TS driver structure */
		ts_driver = &mXT336T_ts_drv;

		/* Get LCD chosen orientation */
		if(ts_SizeX < ts_SizeY)
		{
			ts_orientation = TS_SWAP_NONE;
		}
		else
		{
			ts_orientation = TS_SWAP_XY | TS_SWAP_Y;
		}

		if(ts_status == TS_OK)
		{
			/* Software reset the TouchScreen */
			ts_driver->Reset(I2C_Address);

			/* Calibrate, Configure and Start the TouchScreen driver */
			ts_driver->Start(I2C_Address);

		} /* of if(ts_status == TS_OK) */
	}
	else
	{
		ts_status = TS_DEVICE_NOT_FOUND;
	}
#endif
	return (ts_status);
}

#if TS_INT_EN
/**
 * @brief  Configures and enables the touch screen interrupts.
 * @retval TS_OK if all initializations are OK. Other value if error.
 */
uint8_t BSP_TS_ITConfig(void)
{
	uint8_t ts_status = TS_ERROR;
	uint8_t io_status = IO_ERROR;

	/* Initialize the IO */
	io_status = BSP_IO_Init();
	if(io_status != IO_OK)
	{
		return (ts_status);
	}

	/* Configure TS IT line IO : falling edge on FT6206/FT6336G (see data sheet)                     */
	/* Configure TS_INT_PIN (MFX_IO_14) in IT Falling edge to generate MFX_IRQ_OUT in EXTI on MCU    */
	io_status = BSP_IO_ConfigPin(TS_INT_PIN, IO_MODE_IT_FALLING_EDGE_PU);
	if(io_status != IO_OK)
	{
		return (ts_status);
	}

	/* Enable the TS in interrupt mode */
	/* In that case the INT output of FT6206 when new touch is available */
	/* is active low and directed on MFX IO14 */
	ts_driver->EnableIT(I2C_Address);

	/* If arrived here : set good status on exit */
	ts_status = TS_OK;

	return (ts_status);
}

/**
 * @brief  Gets the touch screen interrupt status.
 * @retval TS_IRQ_PENDING if touchscreen IRQ is pending, TS_NO_IRQ_PENDING when no IRQ TS is pending.
 */
uint8_t BSP_TS_ITGetStatus(void)
{
	uint8_t itStatus = TS_NO_IRQ_PENDING; /* By default no IRQ TS pending */
	uint32_t mfx_irq_status = 0; /* No MFX IRQ by default */

	/* Check status of MFX_IO14 in particular which is the Touch Screen INT pin active low */
	mfx_irq_status = BSP_IO_ITGetStatus(TS_INT_PIN);
	if(mfx_irq_status != 0) /* Note : returned mfx_irq_status = 0x4000 == (1<<TS_INT_PIN) == (1<<14) */
	{
		/* This is Touch Screen INT case : so this is a new touch available that produced the IRQ EXTI */
		itStatus = TS_IRQ_PENDING;
	}

	/* Return the TS IT status */
	return (itStatus);
}

/**
 * @brief  Clears all touch screen interrupts.
 */
void BSP_TS_ITClear(void)
{
	/* Clear TS_INT_PIN IRQ in MFX */
	//BSP_IO_ITClearPin(TS_INT_PIN);
}

#endif

/**
 * @brief  Returns status and positions of the touch screen.
 * @param  TS_State: Pointer to touch screen current state structure
 * @retval TS_OK if all initializations are OK. Other value if error.
 */
uint8_t BSP_TS_GetState(TS_StateTypeDef *TS_State)
{
	uint8_t ts_status = TS_OK;
#if 0
	static uint32_t _x[TS_MAX_NB_TOUCH] = {0, 0};
	static uint32_t _y[TS_MAX_NB_TOUCH] = {0, 0};

	uint16_t tmp;
	uint16_t Raw_x[TS_MAX_NB_TOUCH];
	uint16_t Raw_y[TS_MAX_NB_TOUCH];
	uint16_t xDiff;
	uint16_t yDiff;
	uint32_t index;
#if (TS_MULTI_TOUCH_SUPPORTED == 1)
	uint32_t weight = 0;
	uint32_t area = 0;
	uint32_t event = 0;
#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */

	/* Check and update the number of touches active detected */
	TS_State->touchDetected = ts_driver->DetectTouch(I2C_Address);
	if(TS_State->touchDetected)
	{
		for(index=0; index < TS_State->touchDetected; index++)
		{
			/* Get each touch coordinates */
			ts_driver->GetXY(I2C_Address, &(Raw_x[index]), &(Raw_y[index]));

			if(ts_orientation & TS_SWAP_XY)
			{
				tmp = Raw_x[index];
				Raw_x[index] = Raw_y[index];
				Raw_y[index] = tmp;
			}

			if(ts_orientation & TS_SWAP_X)
			{
				Raw_x[index] = MXT336T_MAX_WIDTH - 1 - Raw_x[index];
			}

			if(ts_orientation & TS_SWAP_Y)
			{
				Raw_y[index] = MXT336T_MAX_HEIGHT - 1 - Raw_y[index];
			}

			xDiff = Raw_x[index] > _x[index]? (Raw_x[index] - _x[index]): (_x[index] - Raw_x[index]);
			yDiff = Raw_y[index] > _y[index]? (Raw_y[index] - _y[index]): (_y[index] - Raw_y[index]);

			if ((xDiff + yDiff) > 5)
			{
				_x[index] = Raw_x[index];
				_y[index] = Raw_y[index];
			}


			TS_State->touchX[index] = _x[index];
			TS_State->touchY[index] = _y[index];

#if (TS_MULTI_TOUCH_SUPPORTED == 1)

			/* Get touch info related to the current touch */
			ft6x06_TS_GetTouchInfo(I2C_Address, index, &weight, &area, &event);

			/* Update TS_State structure */
			TS_State->touchWeight[index] = weight;
			TS_State->touchArea[index]   = area;

			/* Remap touch event */
			switch(event)
			{
			case FT6206_TOUCH_EVT_FLAG_PRESS_DOWN  :
				TS_State->touchEventId[index] = TOUCH_EVENT_PRESS_DOWN;
				break;
			case FT6206_TOUCH_EVT_FLAG_LIFT_UP :
				TS_State->touchEventId[index] = TOUCH_EVENT_LIFT_UP;
				break;
			case FT6206_TOUCH_EVT_FLAG_CONTACT :
				TS_State->touchEventId[index] = TOUCH_EVENT_CONTACT;
				break;
			case FT6206_TOUCH_EVT_FLAG_NO_EVENT :
				TS_State->touchEventId[index] = TOUCH_EVENT_NO_EVT;
				break;
			default :
				ts_status = TS_ERROR;
				break;
			} /* of switch(event) */

#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */

		} /* of for(index=0; index < TS_State->touchDetected; index++) */

#if (TS_MULTI_TOUCH_SUPPORTED == 1)
		/* Get gesture Id */
		ts_status = BSP_TS_Get_GestureId(TS_State);
#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */

	} /* end of if(TS_State->touchDetected != 0) */

	//MXT336T_GetStateState(TS_State);
#endif

	TS_State->touchDetected = MXTDrivers_GetState(TS_State->touchX, TS_State->touchY);

	return (ts_status);
}

#if (TS_MULTI_TOUCH_SUPPORTED == 1)
/**
 * @brief  Update gesture Id following a touch detected.
 * @param  TS_State: Pointer to touch screen current state structure
 * @retval TS_OK if all initializations are OK. Other value if error.
 */
uint8_t BSP_TS_Get_GestureId(TS_StateTypeDef *TS_State)
{
	uint32_t gestureId = 0;
	uint8_t  ts_status = TS_OK;

	/* Get gesture Id */
	ft6x06_TS_GetGestureID(I2C_Address, &gestureId);

	/* Remap gesture Id to a TS_GestureIdTypeDef value */
	switch(gestureId)
	{
	case FT6206_GEST_ID_NO_GESTURE :
		TS_State->gestureId = GEST_ID_NO_GESTURE;
		break;
	case FT6206_GEST_ID_MOVE_UP :
		TS_State->gestureId = GEST_ID_MOVE_UP;
		break;
	case FT6206_GEST_ID_MOVE_RIGHT :
		TS_State->gestureId = GEST_ID_MOVE_RIGHT;
		break;
	case FT6206_GEST_ID_MOVE_DOWN :
		TS_State->gestureId = GEST_ID_MOVE_DOWN;
		break;
	case FT6206_GEST_ID_MOVE_LEFT :
		TS_State->gestureId = GEST_ID_MOVE_LEFT;
		break;
	case FT6206_GEST_ID_ZOOM_IN :
		TS_State->gestureId = GEST_ID_ZOOM_IN;
		break;
	case FT6206_GEST_ID_ZOOM_OUT :
		TS_State->gestureId = GEST_ID_ZOOM_OUT;
		break;
	default :
		ts_status = TS_ERROR;
		break;
	} /* of switch(gestureId) */

	return(ts_status);
}
#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */

#if (TS_MULTI_TOUCH_SUPPORTED == 1)
/**
 * @brief  Function used to reset all touch data before a new acquisition
 *         of touch information.
 * @param  TS_State: Pointer to touch screen current state structure
 * @retval TS_OK if OK, TE_ERROR if problem found.
 */
uint8_t BSP_TS_ResetTouchData(TS_StateTypeDef *TS_State)
{
	uint8_t ts_status = TS_ERROR;
	uint32_t index;

	if (TS_State != (TS_StateTypeDef *)NULL)
	{
		TS_State->gestureId = GEST_ID_NO_GESTURE;
		TS_State->touchDetected = 0;

		for(index = 0; index < TS_MAX_NB_TOUCH; index++)
		{
			TS_State->touchX[index]       = 0;
			TS_State->touchY[index]       = 0;
			TS_State->touchArea[index]    = 0;
			TS_State->touchEventId[index] = TOUCH_EVENT_NO_EVT;
			TS_State->touchWeight[index]  = 0;
		}

		ts_status = TS_OK;

	} /* of if (TS_State != (TS_StateTypeDef *)NULL) */

	return (ts_status);
}
#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */


#endif
/* EOF */
