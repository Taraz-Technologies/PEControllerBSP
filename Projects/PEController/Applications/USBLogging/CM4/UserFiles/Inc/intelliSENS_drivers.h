/**
 ********************************************************************************
 * @file 		intelliSENS_drivers.h
 * @author 		Waqas Ehsan Butt
 * @date 		Dec 28, 2021
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

#ifndef INTELLISENS_DRIVERS_H_
#define INTELLISENS_DRIVERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "usbd_def.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define REPORT_BYTE_COUNT					(1024)
#define REPORT_UINT16_COUNT					(REPORT_BYTE_COUNT / 2)
#define REPORT_UINT16_FIRST_SAMPLE_INDEX	(8)

#define DataCmdIdentifierIndex				((unsigned char)0x00)
#define AckIndex							((unsigned char)0x01)
#define RegisterAddressIndex				((unsigned char)0x02)
#define RegisterValueIndex					((unsigned char)0x03)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef enum
{
	STATE_START = 0x00,
	STATE_STOP = 0x01,
	STATE_MASTER = 0x02
} app_state_t;
typedef enum
{
	REQ_DATA = 0x00,
	REQ_CMD = 0x01,
} req_type_t;
typedef enum
{
	UERR_OK = 0x00,			 /* --todo-- update usb errors */
	UERR_FAIL = 0x01,
	UERR_ILLEGAL_VAL = 0x02,
} user_errors_t;
typedef enum
{
	ROLE_SLAVE = 0x00,
	ROLE_MASTER = 0x01
} app_role_t;
typedef enum
{
	CMD_START_TRANSMISSION = 0x01,
	CMD_STOP_TRANSMISSION = 0x02,
	CMD_CONF2 = 0x04,
	CMD_CONF3 = 0x5,
	CMD_INNER_CALIB = 0x06,
	CMD_COMPUTER_CALIB = 0x09,
	CMD_DEVICE_ROLE	= 0x0B,
	/* bootloader commands */
	CMD_PROGRAM = 0x11,
	CMD_ERASE_ALL = 0x12,
	CMD_ERASE_ADDR = 0x13,
	CMD_RESET = 0x14,
	CMD_FIRMWARE_WRITE_START = 0x15,
	CMD_FIRMWARE_WRITE_END = 0x16,
	CMD_READ_IDENTITY = 0x17,
	CMD_RESET_TO_BOOTLOADER = 0x18,
	CMD_RESET_TO_FIRMWARE = 0x19,
	/* bootloader commands */
} app_cmds_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	bool isCmdPending;
	uint64_t prepCounter;
	uint64_t sentCounter;
	int dataIndex;
	/*! @brief application current data processing state */
	volatile app_state_t state;
	uint8_t rcvBuff[200];
	uint8_t sendBuff[1024];
	uint16_t* dataBufferPtr;
} intelliSENS_data_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
extern USBD_HandleTypeDef hUsbDeviceHS;
extern intelliSENS_data_t intelliSENSData;
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief Add the ADC data to USB buffer for logging
 * @param data pointer to the measurement data
 * @return bool <c>true</c> if added successfully else <c>false</c>. This function will not add data if not requested by the intelliSENS Application
 */
bool intelliSENS_SetADCData(uint64_t* data);
/**
 * @brief Initialize the intelliSENS library
 * @param periodInUs ADC conversion interval in micro-seconds
 * @param mults ADC sensitivities
 * @param offsets ADC offsets
 */
void intelliSENS_Init(int periodInUs, const float* mults, const float* offsets);
/**
 * @brief Reset the data system for the intelliSENS buffers
 */
void intelliSENS_ResetDataSystem(void);
/**
 * @brief Poll the intelliSENS library to send any pending data
 */
void intelliSENS_Poll(void);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
