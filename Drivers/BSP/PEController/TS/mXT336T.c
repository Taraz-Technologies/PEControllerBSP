/**
  ******************************************************************************
  * @file    mXT336T.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    03-August-2015
  * @brief   This file provides a set of functions needed to manage the MXT336T
  *          IO Expander devices.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mXT336T.h"

/* Private typedef -----------------------------------------------------------*/

#define MXT336T_MAX_INSTANCE  1

/* Private macro -------------------------------------------------------------*/

/* Touch screen driver structure initialization */
TS_DrvTypeDef mXT336T_ts_drv =
{
  mXT336T_Init,
  mXT336T_ReadID,
  mXT336T_Reset,

  mXT336T_TS_Start,
  mXT336T_TS_DetectTouch,
  mXT336T_TS_GetXY,

  mXT336T_TS_EnableIT,
  mXT336T_TS_ClearIT,
  mXT336T_TS_ITStatus,
  mXT336T_TS_DisableIT
};

/* mXT336T instances by address */
uint8_t mXT336T[MXT336T_MAX_INSTANCE] = {0};

/* Global mXT336T handle */
static mXT336T_handle_TypeDef mXT336T_handle = { MXT336T_I2C_NOT_INITIALIZED, 0, 0};

static uint8_t mXT336T_GetInstance(uint16_t DeviceAddr);
/* Private functions prototypes-----------------------------------------------*/
#if (TS_AUTO_CALIBRATION_SUPPORTED == 1)
/**
  * @brief  Start TouchScreen calibration phase
  * @param  DeviceAddr: MXT336T Device address for communication on I2C Bus.
  * @retval Status MXT336T_STATUS_OK or MXT336T_STATUS_NOT_OK.
  */
static uint32_t mXT336T_TS_Calibration(uint16_t DeviceAddr);
#endif /* TS_AUTO_CALIBRATION_SUPPORTED == 1 */

/**
  * @brief  Basic static configuration of TouchScreen
  * @param  DeviceAddr: MXT336T Device address for communication on I2C Bus.
  * @retval Status MXT336T_STATUS_OK or MXT336T_STATUS_NOT_OK.
  */
static uint32_t mXT336T_TS_Configure(uint16_t DeviceAddr);

/**
  * @brief  Initialize the mXT336T communication bus
  *         from MCU to MXT336T : ie I2C channel initialization (if required).
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
  * @retval None
  */
void mXT336T_Init(uint16_t DeviceAddr)
{
  uint8_t instance;
  uint8_t empty;

  /* Check if device instance already exists */
  instance = mXT336T_GetInstance(DeviceAddr);

  /* To prevent double initialization */
  if(instance == 0xFF)
  {
    /* Look for empty instance */
    empty = mXT336T_GetInstance(0);

    if(empty < MXT336T_MAX_INSTANCE)
    {
      /* Register the current device instance */
      mXT336T[empty] = DeviceAddr;

      /* Initialize IO BUS layer */
      ts_bsp_drv.Init();


    }
  }
}

/**
  * @brief  Software Reset the mXT336T.
  *         @note : Not applicable to MXT336T.
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
  * @retval None
  */
void mXT336T_Reset(uint16_t DeviceAddr)
{
  /* Do nothing */
  /* No software reset sequence available in MXT336T IC */
	ts_bsp_drv.Reset();
}

/**
  * @brief  Read the mXT336T device ID, pre initialize I2C in case of need to be
  *         able to read the MXT336T device ID, and verify this is a MXT336T.
  * @param  DeviceAddr: I2C MXT336T Slave address.
  * @retval The Device ID (two bytes).
  */
uint16_t mXT336T_ReadID(uint16_t DeviceAddr)
{
  /* Initialize I2C link if needed */
	ts_bsp_drv.Init();

  /* Return the device ID value */
  return (ts_bsp_drv.Read(DeviceAddr, MXT336T_FAMILY_ID_REG));
}

/**
  * @brief  Configures the touch Screen IC device to start detecting touches
  *         It goes through an internal calibration process (Hw calibration sequence of
  *         the touch screen).
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address).
  * @retval None.
  */
void mXT336T_TS_Start(uint16_t DeviceAddr)
{
#if (TS_AUTO_CALIBRATION_SUPPORTED == 1)
  /* Hw Calibration sequence start : should be done once after each power up */
  /* This is called internal calibration of the touch screen                 */
  mXT336T_TS_Calibration(DeviceAddr);
#endif
  /* Minimum static configuration of MXT336T */
  mXT336T_TS_Configure(DeviceAddr);

  /* By default set MXT336T IC in Polling mode : no INT generation on MXT336T for new touch available */
  /* Note TS_INT is active low                                                                      */
  mXT336T_TS_DisableIT(DeviceAddr);
}

/**
  * @brief  Return if there is touches detected or not.
  *         Try to detect new touches and forget the old ones (reset internal global
  *         variables).
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval : Number of active touches detected (can be 0, 1 or 2).
  */
uint8_t mXT336T_TS_DetectTouch(uint16_t DeviceAddr)
{
  volatile uint8_t nbTouch = 0;

  /* Read register MXT336T_TD_STAT_REG to check number of touches detection */
  nbTouch = ts_bsp_drv.Read(DeviceAddr, MXT336T_TD_STAT_REG);
  nbTouch &= MXT336T_TD_STAT_MASK;

  if(nbTouch > MXT336T_MAX_DETECTABLE_TOUCH)
  {
    /* If invalid number of touch detected, set it to zero */
    nbTouch = 0;
  }

  /* Update mXT336T driver internal global : current number of active touches */
  mXT336T_handle.currActiveTouchNb = nbTouch;

  /* Reset current active touch index on which to work on */
  mXT336T_handle.currActiveTouchIdx = 0;

  return(nbTouch);
}

/**
  * @brief  Get the touch screen X and Y positions values
  *         Manage multi touch thanks to touch Index global
  *         variable 'mXT336T_handle.currActiveTouchIdx'.
  * @param  DeviceAddr: Device address on communication Bus.
  * @param  X: Pointer to X position value
  * @param  Y: Pointer to Y position value
  * @retval None.
  */
void mXT336T_TS_GetXY(uint16_t DeviceAddr, uint16_t *X, uint16_t *Y)
{
  uint8_t regAddress = 0;
  uint8_t  dataxy[4];

  if(mXT336T_handle.currActiveTouchIdx < mXT336T_handle.currActiveTouchNb)
  {
    switch(mXT336T_handle.currActiveTouchIdx)
    {
    case 0 :
      regAddress = MXT336T_P1_XH_REG;
      break;
    case 1 :
      regAddress = MXT336T_P2_XH_REG;
      break;

    default :
      break;
    }

    /* Read X and Y positions */
    ts_bsp_drv.ReadMultiple(DeviceAddr, regAddress, dataxy, sizeof(dataxy));

    /* Send back ready X position to caller */
    *X = ((dataxy[0] & MXT336T_MSB_MASK) << 8) | (dataxy[1] & MXT336T_LSB_MASK);

    /* Send back ready Y position to caller */
    *Y = ((dataxy[2] & MXT336T_MSB_MASK) << 8) | (dataxy[3] & MXT336T_LSB_MASK);

    mXT336T_handle.currActiveTouchIdx++;
  }
}

/**
  * @brief  Configure the MXT336T device to generate IT on given INT pin
  *         connected to MCU as EXTI.
  * @param  DeviceAddr: Device address on communication Bus (Slave I2C address of MXT336T).
  * @retval None
  */
void mXT336T_TS_EnableIT(uint16_t DeviceAddr)
{
  uint8_t regValue = 0;
  regValue = (MXT336T_G_MODE_INTERRUPT_TRIGGER & (MXT336T_G_MODE_INTERRUPT_MASK >> MXT336T_G_MODE_INTERRUPT_SHIFT)) << MXT336T_G_MODE_INTERRUPT_SHIFT;

  /* Set interrupt trigger mode in MXT336T_GMODE_REG */
  ts_bsp_drv.Write(DeviceAddr, MXT336T_GMODE_REG, regValue);
}

/**
  * @brief  Configure the MXT336T device to stop generating IT on the given INT pin
  *         connected to MCU as EXTI.
  * @param  DeviceAddr: Device address on communication Bus (Slave I2C address of MXT336T).
  * @retval None
  */
void mXT336T_TS_DisableIT(uint16_t DeviceAddr)
{
  uint8_t regValue = 0;
  regValue = (MXT336T_G_MODE_INTERRUPT_POLLING & (MXT336T_G_MODE_INTERRUPT_MASK >> MXT336T_G_MODE_INTERRUPT_SHIFT)) << MXT336T_G_MODE_INTERRUPT_SHIFT;

  /* Set interrupt polling mode in MXT336T_GMODE_REG */
  ts_bsp_drv.Write(DeviceAddr, MXT336T_GMODE_REG, regValue);
}

/**
  * @brief  Get IT status from MXT336T interrupt status registers
  *         Should be called Following an EXTI coming to the MCU to know the detailed
  *         reason of the interrupt.
  *         @note : This feature is not applicable to MXT336T.
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
  * @retval TS interrupts status : always return 0 here
  */
uint8_t mXT336T_TS_ITStatus(uint16_t DeviceAddr)
{
  /* Always return 0 as feature not applicable to MXT336T */
  return 0;
}

/**
  * @brief  Clear IT status in MXT336T interrupt status clear registers
  *         Should be called Following an EXTI coming to the MCU.
  *         @note : This feature is not applicable to MXT336T.
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
  * @retval None
  */
void mXT336T_TS_ClearIT(uint16_t DeviceAddr)
{
  /* Nothing to be done here for MXT336T */
}

/**** NEW FEATURES enabled when Multi-touch support is enabled ****/

#if (TS_MULTI_TOUCH_SUPPORTED == 1)

/**
  * @brief  Get the last touch gesture identification (zoom, move up/down...).
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
  * @param  pGestureId : Pointer to get last touch gesture Identification.
  * @retval None.
  */
void mXT336T_TS_GetGestureID(uint16_t DeviceAddr, uint32_t * pGestureId)
{
  volatile uint8_t ucReadData = 0;

  ucReadData = ts_bsp_drv.Read(DeviceAddr, MXT336T_GEST_ID_REG);

  * pGestureId = ucReadData;
}

/**
  * @brief  Get the touch detailed informations on touch number 'touchIdx' (0..1)
  *         This touch detailed information contains :
  *         - weight that was applied to this touch
  *         - sub-area of the touch in the touch panel
  *         - event of linked to the touch (press down, lift up, ...)
  * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
  * @param  touchIdx : Passed index of the touch (0..1) on which we want to get the
  *                    detailed information.
  * @param  pWeight : Pointer to to get the weight information of 'touchIdx'.
  * @param  pArea   : Pointer to to get the sub-area information of 'touchIdx'.
  * @param  pEvent  : Pointer to to get the event information of 'touchIdx'.

  * @retval None.
  */
void mXT336T_TS_GetTouchInfo(uint16_t   DeviceAddr,
                            uint32_t   touchIdx,
                            uint32_t * pWeight,
                            uint32_t * pArea,
                            uint32_t * pEvent)
{
  uint8_t regAddress = 0;
  uint8_t dataxy[3];

  if(touchIdx < mXT336T_handle.currActiveTouchNb)
  {
    switch(touchIdx)
    {
    case 0 :
      regAddress = MXT336T_P1_WEIGHT_REG;
      break;

    case 1 :
      regAddress = MXT336T_P2_WEIGHT_REG;
      break;

    default :
      break;

    } /* end switch(touchIdx) */

    /* Read weight, area and Event Id of touch index */
    ts_bsp_drv.ReadMultiple(DeviceAddr, regAddress, dataxy, sizeof(dataxy));

    /* Return weight of touch index */
    * pWeight = (dataxy[0] & MXT336T_TOUCH_WEIGHT_MASK) >> MXT336T_TOUCH_WEIGHT_SHIFT;
    /* Return area of touch index */
    * pArea = (dataxy[1] & MXT336T_TOUCH_AREA_MASK) >> MXT336T_TOUCH_AREA_SHIFT;
    /* Return Event Id  of touch index */
    * pEvent = (dataxy[2] & MXT336T_TOUCH_EVT_FLAG_MASK) >> MXT336T_TOUCH_EVT_FLAG_SHIFT;

  } /* of if(touchIdx < mXT336T_handle.currActiveTouchNb) */
}

#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */

#if (TS_AUTO_CALIBRATION_SUPPORTED == 1)
/**
  * @brief  Start TouchScreen calibration phase
  * @param  DeviceAddr: MXT336T Device address for communication on I2C Bus.
  * @retval Status MXT336T_STATUS_OK or MXT336T_STATUS_NOT_OK.
  */
static uint32_t mXT336T_TS_Calibration(uint16_t DeviceAddr)
{
  uint32_t nbAttempt = 0;
  volatile uint8_t ucReadData;
  volatile uint8_t regValue;
  uint32_t status = MXT336T_STATUS_OK;
  uint8_t bEndCalibration = 0;

  /* >> Calibration sequence start */

  /* Switch MXT336T back to factory mode to calibrate */
  regValue = (MXT336T_DEV_MODE_FACTORY & MXT336T_DEV_MODE_MASK) << MXT336T_DEV_MODE_SHIFT;
  ts_bsp_drv.Write(DeviceAddr, MXT336T_DEV_MODE_REG, regValue); /* 0x40 */

  /* Read back the same register MXT336T_DEV_MODE_REG */
  ucReadData = ts_bsp_drv.Read(DeviceAddr, MXT336T_DEV_MODE_REG);
  TS_IO_Delay(300); /* Wait 300 ms */

  if(((ucReadData & (MXT336T_DEV_MODE_MASK << MXT336T_DEV_MODE_SHIFT)) >> MXT336T_DEV_MODE_SHIFT) != MXT336T_DEV_MODE_FACTORY )
  {
    /* Return error to caller */
    return(MXT336T_STATUS_NOT_OK);
  }

  /* Start calibration command */
  ts_bsp_drv.Write(DeviceAddr, MXT336T_TD_STAT_REG, 0x04);
  TS_IO_Delay(300); /* Wait 300 ms */

  /* 100 attempts to wait switch from factory mode (calibration) to working mode */
  for (nbAttempt=0; ((nbAttempt < 100) && (!bEndCalibration)) ; nbAttempt++)
  {
    ucReadData = ts_bsp_drv.Read(DeviceAddr, MXT336T_DEV_MODE_REG);
    ucReadData = (ucReadData & (MXT336T_DEV_MODE_MASK << MXT336T_DEV_MODE_SHIFT)) >> MXT336T_DEV_MODE_SHIFT;
    if(ucReadData == MXT336T_DEV_MODE_WORKING)
    {
      /* Auto Switch to MXT336T_DEV_MODE_WORKING : means calibration have ended */
      bEndCalibration = 1; /* exit for loop */
    }

    TS_IO_Delay(200); /* Wait 200 ms */
  }

  /* Calibration sequence end << */

  return(status);
}
#endif /* TS_AUTO_CALIBRATION_SUPPORTED == 1 */

/**
  * @brief  Basic static configuration of TouchScreen
  * @param  DeviceAddr: MXT336T Device address for communication on I2C Bus.
  * @retval Status MXT336T_STATUS_OK or MXT336T_STATUS_NOT_OK.
  */
static uint32_t mXT336T_TS_Configure(uint16_t DeviceAddr)
{
  uint32_t status = MXT336T_STATUS_OK;

  /* Nothing special to be done for MXT336T */

  return(status);
}

/**
  * @brief  Check if the device instance of the selected address is already registered
  *         and return its index
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval Index of the device instance if registered, 0xFF if not.
  */
static uint8_t mXT336T_GetInstance(uint16_t DeviceAddr)
{
  uint8_t idx = 0;

  /* Check all the registered instances */
  for(idx = 0; idx < MXT336T_MAX_INSTANCE ; idx ++)
  {
    if(mXT336T[idx] == DeviceAddr)
    {
      return idx;
    }
  }

  return 0xFF;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
