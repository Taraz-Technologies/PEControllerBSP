/**
 ******************************************************************************
 * @file    mXT336T.h
 * @author  MCD Application Team
  * @version V1.0.0
  * @date    03-August-2015
 * @brief   This file contains all the functions prototypes for the
 *          mXT336T.c IO expander driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXT336T_H
#define __MXT336T_H

#ifdef __cplusplus
extern "C" {
#endif

/* Set Multi-touch as non supported */
#ifndef TS_MULTI_TOUCH_SUPPORTED
  #define TS_MULTI_TOUCH_SUPPORTED 0
#endif

/* Set Auto-calibration as non supported */
#ifndef TS_AUTO_CALIBRATION_SUPPORTED
  #define TS_AUTO_CALIBRATION_SUPPORTED 0
#endif

/* Includes ------------------------------------------------------------------*/
#include "ts.h"

/* Macros --------------------------------------------------------------------*/

/** @typedef mXT336T_handle_TypeDef
 *  mXT336T Handle definition.
 */
typedef struct
{
  uint8_t i2cInitialized;

  /* field holding the current number of simultaneous active touches */
  uint8_t currActiveTouchNb;

  /* field holding the touch index currently managed */
  uint8_t currActiveTouchIdx;

} mXT336T_handle_TypeDef;

  /** @addtogroup BSP
   * @{
   */

  /** @addtogroup Component
   * @{
   */

  /** @defgroup MXT336T
   * @{
   */

  /* Exported types ------------------------------------------------------------*/

  /** @defgroup MXT336T_Exported_Types
   * @{
   */

  /* Exported constants --------------------------------------------------------*/

  /** @defgroup MXT336T_Exported_Constants
   * @{
   */

  /* Maximum border values of the touchscreen pad */
#define  MXT336T_MAX_WIDTH              ((uint16_t)800)     /* Touchscreen pad max width   */
#define  MXT336T_MAX_HEIGHT             ((uint16_t)480)     /* Touchscreen pad max height  */

  /* Possible values of driver functions return status */
#define MXT336T_STATUS_OK                0
#define MXT336T_STATUS_NOT_OK            1

  /* Possible values of global variable 'TS_I2C_Initialized' */
#define MXT336T_I2C_NOT_INITIALIZED      0
#define MXT336T_I2C_INITIALIZED          1

  /* Max detectable simultaneous touches */
#define MXT336T_MAX_DETECTABLE_TOUCH     10

  /**
   * @brief : Definitions for MXT336T I2C register addresses on 8 bit
   **/

  /* Current mode register of the MXT336T (R/W) */
#define MXT336T_DEV_MODE_REG             0x00

  /* Possible values of MXT336T_DEV_MODE_REG */
#define MXT336T_DEV_MODE_WORKING         0x00
#define MXT336T_DEV_MODE_FACTORY         0x04

#define MXT336T_DEV_MODE_MASK            0x7
#define MXT336T_DEV_MODE_SHIFT           4

  /* Gesture ID register */
#define MXT336T_GEST_ID_REG              0x01

  /* Possible values of MXT336T_GEST_ID_REG */
#define MXT336T_GEST_ID_NO_GESTURE       0x00
#define MXT336T_GEST_ID_MOVE_UP          0x10
#define MXT336T_GEST_ID_MOVE_RIGHT       0x14
#define MXT336T_GEST_ID_MOVE_DOWN        0x18
#define MXT336T_GEST_ID_MOVE_LEFT        0x1C
#define MXT336T_GEST_ID_ZOOM_IN          0x48
#define MXT336T_GEST_ID_ZOOM_OUT         0x49

  /* Touch Data Status register : gives number of active touch points (0..2) */
#define MXT336T_TD_STAT_REG              0x02

  /* Values related to MXT336T_TD_STAT_REG */
#define MXT336T_TD_STAT_MASK             0x0F
#define MXT336T_TD_STAT_SHIFT            0x00

  /* Values Pn_XH and Pn_YH related */
#define MXT336T_TOUCH_EVT_FLAG_PRESS_DOWN 0x00
#define MXT336T_TOUCH_EVT_FLAG_LIFT_UP    0x01
#define MXT336T_TOUCH_EVT_FLAG_CONTACT    0x02
#define MXT336T_TOUCH_EVT_FLAG_NO_EVENT   0x03

#define MXT336T_TOUCH_EVT_FLAG_SHIFT     6
#define MXT336T_TOUCH_EVT_FLAG_MASK      (3 << MXT336T_TOUCH_EVT_FLAG_SHIFT)

#define MXT336T_MSB_MASK                 0x0F
#define MXT336T_MSB_SHIFT                0

  /* Values Pn_XL and Pn_YL related */
#define MXT336T_LSB_MASK                 0xFF
#define MXT336T_LSB_SHIFT                0

#define MXT336T_P1_XH_REG                0x03
#define MXT336T_P1_XL_REG                0x04
#define MXT336T_P1_YH_REG                0x05
#define MXT336T_P1_YL_REG                0x06

  /* Touch Pressure register value (R) */
#define MXT336T_P1_WEIGHT_REG            0x07

  /* Values Pn_WEIGHT related  */
#define MXT336T_TOUCH_WEIGHT_MASK        0xFF
#define MXT336T_TOUCH_WEIGHT_SHIFT       0

  /* Touch area register */
#define MXT336T_P1_MISC_REG              0x08

  /* Values related to MXT336T_Pn_MISC_REG */
#define MXT336T_TOUCH_AREA_MASK         (0x04 << 4)
#define MXT336T_TOUCH_AREA_SHIFT        0x04

#define MXT336T_P2_XH_REG               0x09
#define MXT336T_P2_XL_REG               0x0A
#define MXT336T_P2_YH_REG               0x0B
#define MXT336T_P2_YL_REG               0x0C
#define MXT336T_P2_WEIGHT_REG           0x0D
#define MXT336T_P2_MISC_REG             0x0E

  /* Threshold for touch detection */
#define MXT336T_TH_GROUP_REG            0x80

  /* Values MXT336T_TH_GROUP_REG : threshold related  */
#define MXT336T_THRESHOLD_MASK          0xFF
#define MXT336T_THRESHOLD_SHIFT         0

  /* Filter function coefficients */
#define MXT336T_TH_DIFF_REG             0x85

  /* Control register */
#define MXT336T_CTRL_REG                0x86

  /* Values related to MXT336T_CTRL_REG */

  /* Will keep the Active mode when there is no touching */
#define MXT336T_CTRL_KEEP_ACTIVE_MODE    0x00

  /* Switching from Active mode to Monitor mode automatically when there is no touching */
#define MXT336T_CTRL_KEEP_AUTO_SWITCH_MONITOR_MODE  0x01

  /* The time period of switching from Active mode to Monitor mode when there is no touching */
#define MXT336T_TIMEENTERMONITOR_REG     0x87

  /* Report rate in Active mode */
#define MXT336T_PERIODACTIVE_REG         0x88

  /* Report rate in Monitor mode */
#define MXT336T_PERIODMONITOR_REG        0x89

  /* The value of the minimum allowed angle while Rotating gesture mode */
#define MXT336T_RADIAN_VALUE_REG         0x91

  /* Maximum offset while Moving Left and Moving Right gesture */
#define MXT336T_OFFSET_LEFT_RIGHT_REG    0x92

  /* Maximum offset while Moving Up and Moving Down gesture */
#define MXT336T_OFFSET_UP_DOWN_REG       0x93

  /* Minimum distance while Moving Left and Moving Right gesture */
#define MXT336T_DISTANCE_LEFT_RIGHT_REG  0x94

  /* Minimum distance while Moving Up and Moving Down gesture */
#define MXT336T_DISTANCE_UP_DOWN_REG     0x95

  /* Maximum distance while Zoom In and Zoom Out gesture */
#define MXT336T_DISTANCE_ZOOM_REG        0x96

  /* High 8-bit of LIB Version info */
#define MXT336T_LIB_VER_H_REG            0xA1

  /* Low 8-bit of LIB Version info */
#define MXT336T_LIB_VER_L_REG            0xA2

  /* Chip Selecting */
#define MXT336T_CIPHER_REG               0xA3

  /* Interrupt mode register (used when in interrupt mode) */
#define MXT336T_GMODE_REG                0xA4

#define MXT336T_G_MODE_INTERRUPT_MASK    0x03
#define MXT336T_G_MODE_INTERRUPT_SHIFT   0x00

  /* Possible values of MXT336T_GMODE_REG */
#define MXT336T_G_MODE_INTERRUPT_POLLING 0x00
#define MXT336T_G_MODE_INTERRUPT_TRIGGER 0x01

  /* Current power mode the MXT336T system is in (R) */
#define MXT336T_PWR_MODE_REG             0xA5

  /* MXT336T firmware version */
#define MXT336T_FIRMID_REG               0xA6

  /* MXT336T Chip identification register */
#define MXT336T_FAMILY_ID_REG             0x00
#define MXT336T_VARIANT_ID_REG            0x01
#define MXT336T_VERSION_REG             	0x02
#define MXT336T_BUILD_REG            			0x03

  /* MXT336T Chip identification register values */
#define MXT336T_FAMILY_ID_VALUE           0xA4
#define MXT336T_VARIANT_ID_VALUE          0x15

  /*  Possible values of MXT336T_CHIP_ID_REG */
#define MXT336T_ID_VALUE                 0x11

  /* Release code version */
#define MXT336T_RELEASE_CODE_ID_REG      0xAF

  /* Current operating mode the MXT336T system is in (R) */
#define MXT336T_STATE_REG                0xBC

  /**
   * @}
   */

  /* Exported macro ------------------------------------------------------------*/

  /** @defgroup mXT336T_Exported_Macros
   * @{
   */

  /* Exported functions --------------------------------------------------------*/

  /** @defgroup mXT336T_Exported_Functions
   * @{
   */

  /**
   * @brief mXT336T Control functions
   */


/**
 * @brief  Initialize the mXT336T communication bus
 *         from MCU to MXT336T : ie I2C channel initialization (if required).
 * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
 * @retval None
 */
void mXT336T_Init(uint16_t DeviceAddr);

/**
 * @brief  Software Reset the mXT336T.
 * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
 * @retval None
 */
void mXT336T_Reset(uint16_t DeviceAddr);

/**
 * @brief  Read the mXT336T device ID, pre intitalize I2C in case of need to be
 *         able to read the MXT336T device ID, and verify this is a MXT336T.
 * @param  DeviceAddr: I2C MXT336T Slave address.
 * @retval The Device ID (two bytes).
 */
uint16_t mXT336T_ReadID(uint16_t DeviceAddr);

/**
 * @brief  Configures the touch Screen IC device to start detecting touches
 * @param  DeviceAddr: Device address on communication Bus (I2C slave address).
 * @retval None.
 */
void mXT336T_TS_Start(uint16_t DeviceAddr);

/**
 * @brief  Return if there is touches detected or not.
 *         Try to detect new touches and forget the old ones (reset internal global
 *         variables).
 * @param  DeviceAddr: Device address on communication Bus.
 * @retval : Number of active touches detected (can be 0, 1 or 2).
 */
uint8_t mXT336T_TS_DetectTouch(uint16_t DeviceAddr);

/**
 * @brief  Get the touch screen X and Y positions values
 *         Manage multi touch thanks to touch Index global
 *         variable 'mXT336T_handle.currActiveTouchIdx'.
 * @param  DeviceAddr: Device address on communication Bus.
 * @param  X: Pointer to X position value
 * @param  Y: Pointer to Y position value
 * @retval None.
 */
void mXT336T_TS_GetXY(uint16_t DeviceAddr, uint16_t *X, uint16_t *Y);

/**
 * @brief  Configure the MXT336T device to generate IT on given INT pin
 *         connected to MCU as EXTI.
 * @param  DeviceAddr: Device address on communication Bus (Slave I2C address of MXT336T).
 * @retval None
 */
void mXT336T_TS_EnableIT(uint16_t DeviceAddr);

/**
 * @brief  Configure the MXT336T device to stop generating IT on the given INT pin
 *         connected to MCU as EXTI.
 * @param  DeviceAddr: Device address on communication Bus (Slave I2C address of MXT336T).
 * @retval None
 */
void mXT336T_TS_DisableIT(uint16_t DeviceAddr);

/**
 * @brief  Get IT status from MXT336T interrupt status registers
 *         Should be called Following an EXTI coming to the MCU to know the detailed
 *         reason of the interrupt.
 * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
 * @retval TS interrupts status
 */
uint8_t mXT336T_TS_ITStatus (uint16_t DeviceAddr);

/**
 * @brief  Clear IT status in MXT336T interrupt status clear registers
 *         Should be called Following an EXTI coming to the MCU.
 * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
 * @retval TS interrupts status
 */
void mXT336T_TS_ClearIT (uint16_t DeviceAddr);

/**** NEW FEATURES enabled when Multi-touch support is enabled ****/

#if (TS_MULTI_TOUCH_SUPPORTED == 1)

/**
 * @brief  Get the last touch gesture identification (zoom, move up/down...).
 * @param  DeviceAddr: Device address on communication Bus (I2C slave address of MXT336T).
 * @param  pGestureId : Pointer to get last touch gesture Identification.
 * @retval None.
 */
void mXT336T_TS_GetGestureID(uint16_t DeviceAddr, uint32_t * pGestureId);

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
                            uint32_t * pEvent);

#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */

/* Imported TS IO functions --------------------------------------------------------*/

/** @defgroup mXT336T_Imported_Functions
 * @{
 */

/* TouchScreen (TS) external IO functions */
//extern void     TS_IO_Delay(uint32_t Delay);

  /**
   * @}
   */

  /* Imported global variables --------------------------------------------------------*/

  /** @defgroup mXT336T_Imported_Globals
   * @{
   */


/* Touch screen driver structure */
extern TS_DrvTypeDef mXT336T_ts_drv;
extern TS_BSPTypeDef ts_bsp_drv;

  /**
   * @}
   */

#ifdef __cplusplus
}
#endif
#endif /* __MXT336T_H */


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
