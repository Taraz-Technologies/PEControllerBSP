/**
 ********************************************************************************
 * @file    	intelliSENS_Drivers.c
 * @author 		Waqas Ehsan Butt
 * @date    	December 27, 2021
 *
 * @brief   Contains the intelliSENS drivers
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
#include "general_header.h"
#include "ring_buffer.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/**
 * @brief Defines the maximum simultaneous USB reports log
 */
#define REPORT_COUNT_MAX				(16)
/**
 * @brief No of ADCs used bu the system
 */
#define ADC_COUNT						(8)
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
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	bool isCmdPending;
	bool isPreviousSent;
	uint64_t prepCounter;
	uint64_t sentCounter;
	/*! @brief application current data processing state */
	volatile app_state_t state;
	uint8_t rcvBuff[200];
	uint8_t sendBuff[1024];
	uint16_t* dataBufferPtr;
} intelliSENS_data_t;
/********************************************************************************
 * Static Variables
 *******************************************************************************/

/********************************************************************************
 * Global Variables
 *******************************************************************************/
extern USBD_HandleTypeDef hUsbDeviceHS;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
static uint16_t intelliSENSDataBuffers[REPORT_COUNT_MAX][512];
static ring_buffer_t ringBuff = {.modulo = REPORT_COUNT_MAX - 1};
static float adcMults[ADC_COUNT] = 0;
static float adcOffsets[ADC_COUNT] = 0;
static intelliSENS_data_t intelliSENSData = {0};
/********************************************************************************
 * Code
 *******************************************************************************/
static inline void ReceiveCmd(void)
{

}

void intelliSENS_ResetDataSystem(void)
{
	RingBuffer_Reset(&ringBuff);
	intelliSENSData.prepCounter = 0;
	intelliSENSData.sentCounter = 0;
	intelliSENSData.dataBufferPtr = intelliSENSDataBuffers[0];
}

static void intelliSENS_SetSensitivities(float* mults)
{

}

static void intelliSENS_SetOffsets(float* offsets)
{

}

void intelliSENS_Init(int periodInUs, float* mults, float* offsets)
{
	intelliSENS_ResetDataSystem();
}

void intelliSENS_DeInit(void)
{
	intelliSENS_ResetDataSystem();
}

void intelliSENS_SetADCData(uint16_t* data)
{

}

/**
 * @brief Processes the received commands if any
 */
static void ProcessCommand(void)
{
	/* wait for the previous transmission completion */
	while(usbAppData.isPreviousSent == FALSE);
	/* set default reply to non compliant command */
	user_errors_t err = UERR_OK;
	// get parameters for command identification
	req_type_t reqType 	= (req_type_t)intelliSENSData.rcvBuff[DataCmdIdentifierIndex];
	uint8_t registerAddress = intelliSENSData.rcvBuff[RegisterAddressIndex];
	uint8_t registerValue 	= intelliSENSData.rcvBuff[RegisterValueIndex];
	// perform only if it is a command
	if(reqType == REQ_CMD){
		// prepare response header for current command
		intelliSENSData.sendBuff[DataCmdIdentifierIndex] = (uint8_t)REQ_CMD;
		intelliSENSData.sendBuff[RegisterAddressIndex] = registerAddress;
		intelliSENSData.sendBuff[RegisterValueIndex] = registerValue;
		// register address identifies the command
		switch(registerAddress){
			/* Process conversion start command - only available for the master device */
			case CMD_START_TRANSMISSION:
			{
				if (intelliSENSData.state == STATE_STOP)
				{
					intelliSENS_ResetDataSystem();
					intelliSENSData.state = STATE_START;
					err = UERR_OK;
				}
			}
				break;
			/* Process conversion stop command */
			case CMD_STOP_TRANSMISSION:
			{
				intelliSENS_ResetDataSystem();
				intelliSENSData.state = STATE_STOP;
				err = UERR_OK;
			}
				break;
			/* Update role of device either master / slave */
			case CMD_DEVICE_ROLE:
			{
				/* Update only possible if communication is inactive */
				if(intelliSENSData.state == STATE_STOP)
				{
					intelliSENS_ResetDataSystem();
					err = UERR_OK;
				}
			}
				break;
			/* Get the device identity */
			case CMD_READ_IDENTITY:
			{
				ReadDeviceIdentity(intelliSENSData.sendBuff + RegisterValueIndex);
				err = UERR_OK;
			}
				break;
			// Reset device and enter bootloader on reset
			case CMD_RESET_TO_BOOTLOADER:
				break;
			/* if not device independent command check for response from device commands */
			default:
				err = USBAppLocal_ProcessCmd(&usbAppData);
				break;
		}

		usbAppData.sendBuff[AckIndex] = (uint8_t)err;			/* update acknowledge status */
		usbAppData.isCmdPending = FALSE;						/* indicate that command processed */
		// for slave in communication active mode no need to respond because
		if(usbAppData.state != STATE_START || (app_role_t)registerValue == ROLE_SLAVE)			/* --todo--  */
		{
			usbAppData.isPreviousSent = FALSE;
			SendCmdResponse();
		}
		ReceiveCmd();				/* send next command fetch so it may not be ignored */
	}
}

/**
 * @brief Try and send new data if available and active
 */
static void USB_TrySendData(void)
{
	/* as the write location may change in run time get a copy */
	int wrIndexBuffered = ringBuff.wrIndex;
	int nextWrIndex = RingBuffer_NextLoc(&ringBuff, wrIndexBuffered);
	// Make sure that read location is not the same as the location being written or the location
	// which is next in line for writing to avoid overwriting (next location lock is a safety precaution
	// in case the write of current buffer being written is nearly full and may overlap during
	// copying previous data has already been sent only than try sending new data
	if(ringBuff.rdIndex != nextWrIndex && ringBuff.rdIndex != wrIndexBuffered && intelliSENSData.isPreviousSent)
	{
		intelliSENSData.sentCounter++;
		/* get a copy of the prepared counter as it may change during run time */
		unsigned long long preparedCounterBuff = intelliSENSData.prepCounter;
		uint8_t* dataPtr = intelliSENSDataBuffers[ringBuff.rdIndex];
		RingBuffer_Read(buff);				/* move to next read location */
		// copy the counters to send to the computer
		// As first byte is needed to inform if the buffer is a cmd or data, only 7 bytes of one counter
		// should be sent, as it is little endian so LSB will be at start so offset should be at start
		// Note - 7 Bytes counter validity only
		memcpy(&dataPtr[1], &intelliSENSData.sentCounter, 8);
		memcpy(&dataPtr[8], &preparedCounterBuff, 8);
		// Mark that sending is busy
		intelliSENSData.isPreviousSent = false;
		// Send current readable buffer
		USBD_HID_SendReport(&hUsbDeviceHS, dataPtr, 1024);
	}
}

void intelliSENS_Poll(void)
{
	if (hUsbDeviceHS.dev_state == USBD_STATE_CONFIGURED && (USBD_HID_HandleTypeDef *)(hUsbDeviceHS.pClassData)->state == HID_IDLE)
	{
		if(intelliSENSData.isCmdPending)
			ProcessCommand();
		else if (intelliSENSData.state == STATE_START)
			USB_TrySendData();
	}
}

/* EOF */
