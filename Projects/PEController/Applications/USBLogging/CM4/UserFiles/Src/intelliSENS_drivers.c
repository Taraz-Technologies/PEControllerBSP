/**
 ********************************************************************************
 * @file    	intelliSENS_drivers.c
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
#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"
#include "ring_buffer.h"
#include "intelliSENS_drivers.h"
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
#define ADC_COUNT						(16)
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
static uint16_t intelliSENSDataBuffers[REPORT_COUNT_MAX][512];
static ring_buffer_t ringBuff = {.modulo = REPORT_COUNT_MAX - 1};
static float adcMults[ADC_COUNT] = {0};
static float adcOffsets[ADC_COUNT] = {0};
intelliSENS_data_t intelliSENSData = {0};
uint16_t conf2;
uint16_t conf3;
uint16_t adcTicks;
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/
extern void MX_USB_DEVICE_Init(void);
/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Reset the data system for the intelliSENS buffers
 */
void intelliSENS_ResetDataSystem(void)
{
	RingBuffer_Reset(&ringBuff);
	intelliSENSData.prepCounter = 0;
	intelliSENSData.sentCounter = 0;
	intelliSENSData.dataIndex = REPORT_UINT16_FIRST_SAMPLE_INDEX;
	intelliSENSData.dataBufferPtr = intelliSENSDataBuffers[0];
}

/**
 * @brief Set the relative sensitives for all ADC channels
 * @param mults ADC sensitivities
 */
static void intelliSENS_SetSensitivities(const float* mults)
{
	for (int i = 0; i < ADC_COUNT; i++)
		adcMults[i] = mults[i];
}

/**
 * @brief Set the relative offsets for all ADC channels
 * @param offsets ADC offsets
 */
static void intelliSENS_SetOffsets(const float* offsets)
{
	for (int i = 0; i < ADC_COUNT; i++)
		adcOffsets[i] = offsets[i];
}

/**
 * @brief Initialize the intelliSENS library
 * @param periodInUs ADC conversion interval in micro-seconds
 * @param mults ADC sensitivities
 * @param offsets ADC offsets
 */
void intelliSENS_Init(int periodInUs, const float* mults, const float* offsets)
{
	intelliSENS_ResetDataSystem();
	intelliSENS_SetSensitivities(mults);
	intelliSENS_SetOffsets(offsets);
	MX_USB_DEVICE_Init();
}

/**
 * @brief De-initialize the intelliSENS library
 */
static void intelliSENS_DeInit(void)
{
	//--todo-- while removing static remove usb also
	intelliSENS_ResetDataSystem();
}
#pragma GCC push_options
#pragma GCC optimize ("-Ofast")
/**
 * @brief Add the ADC data to USB buffer for logging
 * @param data pointer to the measurement data
 * @return bool <c>true</c> if added successfully else <c>false</c>. This function will not add data if not requested by the intelliSENS Application
 */
bool intelliSENS_SetADCData(uint64_t* data)
{
	if (intelliSENSData.state != STATE_START)
		return false;

	uint64_t* dataPtr = (uint64_t*)&intelliSENSDataBuffers[ringBuff.wrIndex][intelliSENSData.dataIndex];
	*dataPtr++ = *data++;
	*dataPtr++ = *data++;
	*dataPtr++ = *data++;
	*dataPtr = *data;

	intelliSENSData.dataIndex += ADC_COUNT;
	if (intelliSENSData.dataIndex > REPORT_UINT16_COUNT - ADC_COUNT)
	{
		intelliSENSData.dataIndex = REPORT_UINT16_FIRST_SAMPLE_INDEX;
		intelliSENSData.prepCounter++;
		RingBuffer_Write(&ringBuff);
	}
	return true;
}
#pragma GCC pop_options
static user_errors_t ProcesSetCmd(app_cmds_t reg)
{
	user_errors_t err = UERR_FAIL;
	uint8_t* rcvBuff = intelliSENSData.rcvBuff;
	switch (reg)
	{
	/* refresh requirement for channels */
	case CMD_CONF2:
		if(intelliSENSData.state == STATE_STOP)				/* can only process in stop state */
		{
			//if((input_mode_t)rcvBuff[RegisterValueIndex] >= INPUT_CH_INVALID)
			//	err = UERR_ILLEGAL_VAL;
			//else
			{
				//deviceRegs.regU16[REGI_INPUT] = (rcvBuff[RegisterValueIndex]);
				conf2 = (rcvBuff[RegisterValueIndex]);
				/* reconfigure dmas for usb registers */
				err = UERR_OK;
			}
		}
		break;
		/* refresh acquisition speed / division factor */
	case CMD_CONF3:
		if(intelliSENSData.state == STATE_STOP)				/* can only process in stop state */
		{
			uint16_t val = (rcvBuff[RegisterValueIndex]) | ((rcvBuff[RegisterValueIndex + 1] << 8) & 0xFF00);
			conf3 = val;
			//deviceRegs.regU16[REGI_PWM] = val;
			err = UERR_OK;
		}
		break;
	case CMD_INNER_CALIB:
		break;
	case CMD_COMPUTER_CALIB:
		break;
	default:
		break;
	}
	return err;
}
static user_errors_t ProcesGetCmd(app_cmds_t reg)
{
	user_errors_t err = UERR_FAIL;
	uint8_t* sendBuff = intelliSENSData.sendBuff;
	if(intelliSENSData.state != STATE_STOP)				/* can only process in stop state */
		return err;
	switch (reg)
	{
	/* get device specific settings, no of channels */
	case CMD_CONF2:
		//sendBuff[RegisterValueIndex] = deviceRegs.regU16[REGI_INPUT];
		sendBuff = conf2;
		err = UERR_OK;
		break;
		/* get data collection speed register value */
	case CMD_CONF3:
		//sendBuff[RegisterValueIndex] = deviceRegs.regU16[REGI_PWM];
		sendBuff = conf3;
		err = UERR_OK;
		break;
	case CMD_SAMPLE_RATE:
		sendBuff[RegisterValueIndex] = (uint8_t)(adcTicks & 0xff);
		sendBuff[RegisterValueIndex + 1] = (uint8_t)((adcTicks & 0xff00) >> 8);
		err = UERR_OK;
		break;
	default:
		break;
	}
	return err;
}

/*!
@brief Process cmd that needs device specific responses
@returns
 */
static user_errors_t USBAppLocal_ProcessCmd(void)
{
	uint8_t registerAddress = intelliSENSData.rcvBuff[RegisterAddressIndex];
	uint8_t reg = 0x7f & registerAddress;				/* gets the register to rd / write */
	if(registerAddress == reg)
		return ProcesSetCmd((app_cmds_t)reg);
	else
		return ProcesGetCmd((app_cmds_t)reg);
}

/*!
@brief Read the identity buffer
@param *buffer- this buffer contains the version at the end of function
*/
void ReadDeviceIdentity(uint8_t* buffer)
{
	buffer[0] = 1;
	buffer[1] = 4;
	buffer[2] = 1;
	buffer[3] = 0;
	buffer[4] = 1;
	buffer[5] = 2;
	buffer[6] = 1;
	buffer[7] = 2;
}

/**
 * @brief Processes the received commands if any
 */
static void ProcessCommand(void)
{
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
			err = USBAppLocal_ProcessCmd();
			break;
		}

		intelliSENSData.sendBuff[AckIndex] = (uint8_t)err;			/* update acknowledge status */
		intelliSENSData.isCmdPending = false;						/* indicate that command processed */
		// for slave in communication active mode no need to respond because
		if(intelliSENSData.state != STATE_START || (app_role_t)registerValue == ROLE_SLAVE)			/* --todo--  */
			USBD_HID_SendReport(&hUsbDeviceHS, intelliSENSData.sendBuff, 1024);
		// ReceiveCmd();				/* send next command fetch so it may not be ignored */
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
	if(ringBuff.rdIndex != nextWrIndex && ringBuff.rdIndex != wrIndexBuffered)
	{
		intelliSENSData.sentCounter++;
		/* get a copy of the prepared counter as it may change during run time */
		unsigned long long preparedCounterBuff = intelliSENSData.prepCounter;
		uint8_t* dataPtr = (uint8_t*)intelliSENSDataBuffers[ringBuff.rdIndex];
		RingBuffer_Read(&ringBuff);				/* move to next read location */
		// copy the counters to send to the computer
		// As first byte is needed to inform if the buffer is a cmd or data, only 7 bytes of one counter
		// should be sent, as it is little endian so LSB will be at start so offset should be at start
		// Note - 7 Bytes counter validity only
		memcpy(&dataPtr[1], &intelliSENSData.sentCounter, 8);
		memcpy(&dataPtr[8], &preparedCounterBuff, 8);
		// Send current readable buffer
		USBD_HID_SendReport(&hUsbDeviceHS, dataPtr, 1024);
	}
}
volatile int pollingCount = 0;
/**
 * @brief Poll the intelliSENS library to send any pending data
 */
void intelliSENS_Poll(void)
{
	pollingCount++;
	if (hUsbDeviceHS.dev_state == USBD_STATE_CONFIGURED && ((USBD_HID_HandleTypeDef *)(hUsbDeviceHS.pClassData))->state == HID_IDLE)
	{
		if(intelliSENSData.isCmdPending)
			ProcessCommand();
		else if (intelliSENSData.state == STATE_START)
			USB_TrySendData();
	}
}

void intelliSENS_SetADCTicks(uint16_t ticks)
{
	adcTicks = ticks;
}

/* EOF */
