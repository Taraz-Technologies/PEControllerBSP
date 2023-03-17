/**
 ********************************************************************************
 * @file 		pecontroller_ts.h
 * @author 		Waqas Ehsan Butt
 * @date 		Mar 15, 2023
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

#ifndef PECONTROLLER_TS_H_
#define PECONTROLLER_TS_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
#if LCD_DATA_MONITORING
#include "general_header.h"
#include "mxt_drivers.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @brief With FT6206 : maximum 2 touches detected simultaneously
  */
#define TS_MAX_NB_TOUCH                 ((uint32_t) MXT336T_MAX_DETECTABLE_TOUCH)

#define TS_NO_IRQ_PENDING               ((uint8_t) 0)
#define TS_IRQ_PENDING                  ((uint8_t) 1)

#define TS_SWAP_NONE                    ((uint8_t) 0x01)
#define TS_SWAP_X                       ((uint8_t) 0x02)
#define TS_SWAP_Y                       ((uint8_t) 0x04)
#define TS_SWAP_XY                      ((uint8_t) 0x08)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/**
 *  @brief TS_StatusTypeDef
 *  Define BSP_TS_xxx() functions possible return value,
 *  when status is returned by those functions.
 */
typedef enum
{
  TS_OK                = 0x00, /*!< Touch Ok */
  TS_ERROR             = 0x01, /*!< Touch Error */
  TS_TIMEOUT           = 0x02, /*!< Touch Timeout */
  TS_DEVICE_NOT_FOUND  = 0x03  /*!< Touchscreen device not found */
} TS_StatusTypeDef;

/********************************************************************************
 * Structures
 *******************************************************************************/
/**
*  @brief TS_StateTypeDef
*  Define TS State structure
*/
typedef struct
{
  bool  touchDetected;                	/*!< Contains touch detections state   */
  uint16_t touchX;      				 /*!< Contains horizontal touch location state */
  uint16_t touchY;      				/*!< Contains vertical touch location state */
} TS_StateTypeDef;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief Initialize the touch screen controller
 * @param ts_SizeX Horizontal Size of the touch screen
 * @param ts_SizeY Vertical Size of the touch screen
 * @return <c>TS_OK</c> if initialization is successful else <c>TS_DEVICE_NOT_FOUND</c>
 */
uint8_t BSP_TS_Init(uint16_t ts_SizeX, uint16_t ts_SizeY);
/**
 * @brief  Returns status and positions of the touch screen.
 * @param  TS_State: Pointer to touch screen current state structure
 * @return <c>TS_OK</c> if initialization is successful else <c>TS_DEVICE_NOT_FOUND</c>
 */
uint8_t BSP_TS_GetState(TS_StateTypeDef *TS_State);
/********************************************************************************
 * Code
 *******************************************************************************/


#endif
#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
