/**
 ********************************************************************************
 * @file    	mxt_drivers.c
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
#include "mXT336T.c"
#include "mxt_drivers.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define OBJECT_COUNT			(0x27)
#define MSG_COUNT				(0x3E)

#define MXT_T5_MSG_LEN			(10)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static uint8_t objectTable[OBJECT_COUNT][MXT_OBJECT_SIZE];
static uint8_t msgBuffer[MSG_COUNT][MXT_T5_MSG_LEN];
static uint16_t xPress, yPress;
uint8_t counter = 0;
/********************************************************************************
 * Global Variables
 *******************************************************************************/
mxt_data_t mxtData = { .shiftedAddress = (0x4A << 1U) };
/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/*!
 * @brief get a pointer to the object in object table
 * @param *data- sensor memory buffered
 * @param type- type of object required
 */
static mxt_object_t *mxt_get_object(mxt_data_t *data, uint8_t type)
{
	mxt_object_t *object;
	int i;

	for (i = 0; i < data->info.object_num; i++) {
		object = data->object_table + i;
		if (object->type == type)
			return object;
	}

	return NULL;
}
/*!
 * @brief read the object from touch display
 * @param *data- sensor memory buffered
 * @param type- type of object required
 * @param offset- offset from start address
 * @param *val- pointer to buffer
 */
static int mxt_read_object(mxt_data_t *data, uint8_t type, uint8_t offset, uint8_t *val)
{
	mxt_object_t *object;
	uint16_t reg;

	object = mxt_get_object(data, type);
	if (object == NULL)
		return 0;

	reg = object->start_address;
	return ts_bsp_drv.ReadMultiple(data->shiftedAddress, reg + offset, val, 1);
	//mxt_read_reg(data->client, reg + offset, 1, val);
}
/*!
 * @brief write the object to touch display
 * @param *data- sensor memory buffered
 * @param type- type of object required
 * @param offset- offset from start address
 * @param *val- pointer to buffer
 */
static int mxt_write_object(mxt_data_t *data, uint8_t type, uint8_t offset, uint8_t val)
{
	mxt_object_t *object;
	uint16_t reg;
	int error = HAL_OK;

	object = mxt_get_object(data, type);
	if (!object)
		return HAL_ERROR;

	if (offset >= object->size * object->instances)
		return HAL_ERROR;

	reg = object->start_address;
	ts_bsp_drv.Write(data->shiftedAddress, reg + offset, val);

	return error;
}

#if 1			// Message Processing
/*!
 * @brief Process T100 message
 * @param
 * @param
 */
static void mxt_proc_t100_messages(mxt_data_t *data, uint8_t *message)
{
	uint8_t status, touch_type, touch_event;
	int x;
	int y;
	int area = 0;
	int amplitude = 0;
	uint8_t vector = 0;
	uint8_t peak = 0;
	int id;
	int index = 0;

	id = message[0] - data->T100_reportid_min;

	// touch fault
	if (id < 0 || id > data->num_touchids) {
		return;
	}

	if (id == 0) {
		status = message[1];
		data->touch_num = message[2];

		if (status & MXT_T100_SUP)
		{
			int i;
			//			for (i = 0; i < data->num_touchids - 2; i++) {
			//				input_mt_slot(input_dev, i);
			//				input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, 0);
			//			}
			//			mxt_input_sync(data);
		}
	}
	else if (id >= 2) {
		/* deal with each point report */
		status = message[1];
		touch_type = (status & 0x70) >> 4;
		touch_event = status & 0x0F;
		x = (message[3] << 8) | (message[2] & 0xFF);
		y = (message[5] << 8) | (message[4] & 0xFF);

//		if(touch_event == 5)
//		{
//			xPress = x;
//			yPress = y;
//		}
		index = 6;

		if (data->t100_tchaux_bits &  MXT_T100_VECT)
			vector = message[index++];
		if (data->t100_tchaux_bits &  MXT_T100_AMPL) {
			amplitude = message[index++];
		}
		if (data->t100_tchaux_bits &  MXT_T100_AREA) {
			area = message[index++];
		}
		if (data->t100_tchaux_bits &  MXT_T100_PEAK)
			peak = message[index++];

		//		input_mt_slot(input_dev, id - 2);

		if (status & MXT_T100_DETECT) {
			if (touch_event == MXT_T100_EVENT_DOWN || touch_event == MXT_T100_EVENT_UNSUP
					|| touch_event == MXT_T100_EVENT_MOVE || touch_event == MXT_T100_EVENT_NONE) {
				/* Touch in detect, report X/Y position */
				if (touch_event == MXT_T100_EVENT_DOWN ||
						touch_event == MXT_T100_EVENT_UNSUP)
					{
						data->finger_down[id - 2] = true;
						xPress = x;
						yPress = y;
						counter++;
					}
				if ((touch_event == MXT_T100_EVENT_MOVE ||
						touch_event == MXT_T100_EVENT_NONE) &&
						!data->finger_down[id - 2])
					return;

				//				input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, 1);
				//				input_report_abs(input_dev, ABS_MT_POSITION_X, x);
				//				input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
				//				if (touch_type == MXT_T100_TYPE_HOVERING_FINGER)
				//					input_report_abs(input_dev, BTN_TOUCH, 0);
				//				else
				//					input_report_abs(input_dev, BTN_TOUCH, 1);

				//				if (data->t100_tchaux_bits &  MXT_T100_AMPL) {
				//					if (touch_type == MXT_T100_TYPE_HOVERING_FINGER)
				//						amplitude = 0;
				//					input_report_abs(input_dev, ABS_MT_PRESSURE, amplitude);
				//				}
				//				if (data->t100_tchaux_bits &  MXT_T100_AREA) {
				//					if (touch_type == MXT_T100_TYPE_HOVERING_FINGER)
				//						area = 0;
				//					input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, area);
				//				}
				//				if (data->t100_tchaux_bits &  MXT_T100_VECT)
				//					input_report_abs(input_dev, ABS_MT_ORIENTATION, vector);
				//				mxt_input_sync(data);
			}
		} else if (touch_event == 5){
			/* Touch no longer in detect, so close out slot */
			/*if (data->touch_num == 0 &&
					data->wakeup_gesture_mode &&
					data->is_wakeup_by_gesture) {
				//				dev_info(dev, "wakeup finger release, restore t7 and t8!\n");
				data->is_wakeup_by_gesture = false;
				//				mxt_set_power_cfg(data, MXT_POWER_CFG_RUN);
			}
			//			mxt_input_sync(data);
			//			input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, 0);
			 *
			 */
			data->finger_down[id - 2] = false;
		}
	}
	}



#endif

/*!
 * @brief Process received message
 * @param
 * @param
 */
static int mxt_proc_message(mxt_data_t *data, uint8_t *msg)
{
	uint8_t report_id = msg[0];

	if (report_id == MXT_RPTID_NOMSG)
		return -1;

	//	if (report_id >= data->T9_reportid_min
	//	    && report_id <= data->T9_reportid_max) {
	//		mxt_proc_t9_messages(data, msg);
	//	} else if (report_id >= data->T63_reportid_min
	//		   && report_id <= data->T63_reportid_max) {
	//		mxt_proc_t63_messages(data, msg);
	//	} else if (report_id >= data->T15_reportid_min
	//		   && report_id <= data->T15_reportid_max) {
	//		mxt_proc_t15_messages(data, msg);
	//	} else if (report_id >= data->T19_reportid_min
	//		   && report_id <= data->T19_reportid_max) {
	//		mxt_proc_t19_messages(data, msg);
	//	} else if (report_id >= data->T25_reportid_min
	//		   && report_id <= data->T25_reportid_max) {
	//		mxt_proc_t25_messages(data, msg);
	//	} else if (report_id == data->T6_reportid) {
	//		mxt_proc_t6_messages(data, msg);
	//	} else if (report_id == data->T48_reportid) {
	//		mxt_proc_t48_messages(data, msg);
	//	} else if (report_id >= data->T42_reportid_min
	//		   && report_id <= data->T42_reportid_max) {
	//		mxt_proc_t42_messages(data, msg);
	//	} else if (report_id == data->T66_reportid) {
	//		mxt_proc_t66_messages(data, msg);
	//	} else if (report_id >= data->T81_reportid_min
	//		   && report_id <= data->T81_reportid_max) {
	//		mxt_proc_t81_message(data, msg);
	//	} else if (report_id >= data->T92_reportid_min
	//		   && report_id <= data->T92_reportid_max) {
	//		mxt_proc_t92_message(data, msg);
	//	} else if (report_id >= data->T93_reportid_min
	//		   && report_id <= data->T93_reportid_max) {
	//		mxt_proc_t93_message(data, msg);
	//	} else if (report_id >= data->T97_reportid_min
	//		   && report_id <= data->T97_reportid_max) {
	//		mxt_proc_t97_messages(data, msg);
	//	} else
	if (report_id >= data->T100_reportid_min
			&& report_id <= data->T100_reportid_max) {
		mxt_proc_t100_messages(data, msg);}
	//	} else if (report_id == data->T109_reportid) {
	//		mxt_proc_t109_messages(data, msg);
	//	}

	return 0;
}

/*!
 * @brief
 * @param
 * @param
 */
static int mxt_read_count_messages(mxt_data_t *data, uint8_t count)
{
	int error;
	int i;
	uint8_t num_valid = 0;

	/* Safety check for msg_buf */
	if (count > data->max_reportid)
		return HAL_ERROR;

	/* Process remaining messages if necessary */
	error = ts_bsp_drv.ReadMultiple(data->shiftedAddress, data->T5_address, data->msg_buf, data->T5_msg_size * count);
	if (error != HAL_OK)
		return error;

	for (i = 0;  i < count; i++) {
		error = mxt_proc_message(data, data->msg_buf + data->T5_msg_size * i);

		if (error == HAL_OK)
			num_valid++;
		else
			break;
	}

	/* return number of messages read */
	return num_valid;
}

static void mxt_read_messages_t44(mxt_data_t *data)
{
	int error;
	uint8_t count, num_left;

	/* Read T44 and T5 together */
	error = ts_bsp_drv.ReadMultiple(data->shiftedAddress, data->T44_address, data->msg_buf, data->T5_msg_size + 1);
	if (error != HAL_OK)
		return;

	count = data->msg_buf[0];

	if (count == 0) {
		return;
	} else if (count > data->max_reportid) {
		count = data->max_reportid;
	}

	/* Process first message */
	error = mxt_proc_message(data, data->msg_buf + 1);
	if (error < 0) {
		return;
	}

	num_left = count - 1;

	/* Process remaining messages if necessary */
	if (num_left) {
		error = mxt_read_count_messages(data, num_left);
		if (error < 0) {
			return;
		}
	}
}

//volatile uint8_t touchTH;
/*static void SetResolution(mxt_data_t *data, uint16_t x, uint16_t y)
{
	if (mxt_get_object(data, MXT_TOUCH_MULTI_T100) != NULL) {

		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_XRANGE_MSB, (x >> 8) & 0xff);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_XRANGE_LSB, (uint8_t)(x & 0xff));

		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_YRANGE_MSB, (y >> 8) & 0xff);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_YRANGE_LSB, (uint8_t)(y & 0xff));

		uint8_t val;
		mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_CFG1, &val);
		val = (val & ~(0xE0)) | (0x80);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_CFG1, val);
		//		mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_CFG1, &val);
		//		orient = (val & 0xE0) >> 5;
	}
	else {
		mxt_write_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_XRANGE_MSB, (x >> 8) & 0xff);
		mxt_write_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_XRANGE_LSB, (uint8_t)(x & 0xff));

		mxt_write_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_YRANGE_MSB, (y >> 8) & 0xff);
		mxt_write_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_YRANGE_LSB, (uint8_t)(y & 0xff));

		//		mxt_read_object(data, MXT_TOUCH_MULTI_T9, MXT_TOUCH_CFG1, &val);
		//		orient = (val & 0xE0) >> 5;
	}

	uint8_t val = 0;
	mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_TCHTHR, &touchTH);
	mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_TCHTHR, 0x10);
}
*/
// gets the object table
static uint16_t mxt_get_object_table(mxt_data_t *data)
{
	uint16_t error;
	int i;
	uint16_t end_address;
	uint8_t reportid = 0;
	int add_num = 0;
	//	data->mem_size = 0;

	error = ts_bsp_drv.ReadMultiple(data->shiftedAddress, MXT_OBJECT_START, (uint8_t*)objectTable, sizeof(objectTable));
	if (error != HAL_OK)
		return error;

	data->info.object_num = OBJECT_COUNT;
	if (data->object_table == NULL)
		data->object_table = malloc(sizeof(mxt_object_t) * data->info.object_num);

	for (i = 0; i < data->info.object_num; i++) {
		// set destination object
		mxt_object_t *object = data->object_table + i;


		object->type = objectTable[i][0];
		if (object->type == 35) {
			object->type = 150 + add_num;
			add_num ++;
		}
		object->start_address = (objectTable[i][2] << 8) | objectTable[i][1];
		object->size = objectTable[i][3] + 1;
		object->instances = objectTable[i][4] + 1;
		object->num_report_ids = objectTable[i][5];

		if (object->num_report_ids) {
			reportid += object->num_report_ids * object->instances;
			object->max_reportid = reportid;
			object->min_reportid = object->max_reportid - object->instances * object->num_report_ids + 1;
		}

		end_address = object->start_address + object->size * object->instances - 1;

		//		if (end_address >= data->mem_size)
		//			data->mem_size = end_address + 1;

		/* save data for objects used when processing interrupts */
		switch (object->type) {
		case MXT_TOUCH_MULTI_T9:
			data->T9_reportid_max = object->max_reportid;
			data->T9_reportid_min = object->min_reportid;
			data->num_touchids = object->num_report_ids * object->instances;
			break;
		case MXT_GEN_COMMAND_T6:
			data->T6_reportid = object->max_reportid;
			break;
		case MXT_GEN_MESSAGE_T5:
			if (data->info.family_id == 0x80) {
				/* On mXT224 must read and discard CRC byte
				 * otherwise DMA reads are misaligned */
				data->T5_msg_size = object->size;
			} else {
				/* CRC not enabled, therefore don't read last byte */
				data->T5_msg_size = object->size - 1;
			}
			data->T5_address = object->start_address;
			break;
		case MXT_GEN_POWER_T7:
			data->T7_address = object->start_address;
			break;
		case MXT_TOUCH_KEYARRAY_T15:
			data->T15_reportid_max = object->max_reportid;
			data->T15_reportid_min = object->min_reportid;
			break;
		case MXT_SPT_GPIOPWM_T19:
			data->T19_reportid_max = object->max_reportid;
			data->T19_reportid_min = object->min_reportid;
			break;
		case MXT_SPT_SELFTEST_T25:
			data->T25_reportid_max = object->max_reportid;
			data->T25_reportid_min = object->min_reportid;
			break;
		case MXT_DEBUG_DIAGNOSTIC_T37:
			data->T37_address = object->start_address;
			break;
		case MXT_PROCI_TOUCHSUPPRESSION_T42:
			data->T42_reportid_max = object->max_reportid;
			data->T42_reportid_min = object->min_reportid;
			break;
		case MXT_SPT_MESSAGECOUNT_T44:
			data->T44_address = object->start_address;
			break;
		case MXT_SPT_NOISESUPPRESSION_T48:
			data->T48_reportid = object->max_reportid;
			break;
		case MXT_PROCI_ACTIVE_STYLUS_T63:
			data->T63_reportid_max = object->max_reportid;
			data->T63_reportid_min = object->min_reportid;
			data->num_stylusids =
					object->num_report_ids * object->instances;
			break;
		case MXT_SPT_GOLDENREF_T66:
			data->T66_reportid = object->max_reportid;
			break;
		case MXT_TOUCH_MORE_GESTURE_T81:
			data->T81_reportid_min = object->min_reportid;
			data->T81_reportid_max = object->max_reportid;
			break;
		case MXT_TOUCH_GESTURE_T92:
			data->T92_reportid_min = object->min_reportid;
			data->T92_reportid_max = object->max_reportid;
			break;
		case MXT_TOUCH_SEQUENCE_LOGGER_T93:
			data->T93_reportid_min = object->min_reportid;
			data->T93_reportid_max = object->max_reportid;
			break;
		case MXT_TOUCH_MULTI_T100:
			data->T100_reportid_max = object->max_reportid;
			data->T100_reportid_min = object->min_reportid;
			data->num_touchids = object->num_report_ids * object->instances;
			break;
		case MXT_SPT_SELFCAPGLOBALCONFIG_T109:
			data->T109_reportid = object->max_reportid;
			break;
		case MXT_TOUCH_KEYARRAY_T97:
			data->T97_reportid_max = object->max_reportid;
			data->T97_reportid_min = object->min_reportid;
			break;
		default:
			break;
		}
	}

	/* Store maximum reportid */
	data->max_reportid = reportid;

	/* If T44 exists, T9 position has to be directly after */
	if (data->T44_address && (data->T5_address != data->T44_address + 1)) {
		return HAL_ERROR;
	}

	data->msg_buf = (uint8_t*)msgBuffer;
	//	/* Allocate message buffer */
	//	data->msg_buf = kcalloc(data->max_reportid, data->T5_msg_size, GFP_KERNEL);
	//	if (!data->msg_buf) {
	//		return 0;
	//	}

	return 0;
}

// initialize the mxt object
static uint16_t mxt_initialize(mxt_data_t *data, uint16_t ts_SizeX, uint16_t ts_SizeY)
{
	mxt_info_t *info = &data->info;
	uint16_t error = HAL_OK;
	uint8_t retry_count = 0;

	/* Read info block */
	while ((error != HAL_OK || info->family_id == 0) && retry_count++ < 100)
	{
		error = ts_bsp_drv.ReadMultiple(data->shiftedAddress, MXT_FAMILY_ID, (uint8_t*)info, MXT_OBJECT_NUM - MXT_FAMILY_ID + 1);
	}

	/* If error occurs in reading register, do not download firmware */
	if (error != HAL_OK) {
		//		error = mxt_probe_bootloader(data);
		//		if (error) {
		//			/* Chip is not in appmode or bootloader mode */
		//			return error;
		//		} else {
		//			if (++retry_count > 10) {
		//				return 0;
		//			}

		//			/* Tell bootloader to enter app mode. Ignore errors
		//			 * since we're in a retry loop */
		//			mxt_send_bootloader_cmd(data, false);
		//			msleep(MXT_FWRESET_TIME);
		//			goto retry_probe;
		//		}
		return error;
	}
	//	data->state = APPMODE;

	/* Get object table information */
	error = mxt_get_object_table(data);
	if (error != HAL_OK)
		return error;

	//	error = mxt_get_t38_flag(data);
	//	if (error) {
	//		dev_err(&client->dev, "Error %d getting update flag\n", error);
	//		return error;
	//	}

	//	/* Read information block CRC */
	//	error = mxt_read_info_block_crc(data);
	//	if (error) {
	//		dev_err(&client->dev, "Error %d reading info block CRC\n", error);
	//	}

	//	error = mxt_probe_power_cfg(data);
	//	if (error) {
	//		dev_err(&client->dev, "Failed to initialize power cfg\n");
	//		return error;
	//	}

	/* Check register init values */
	//	error = mxt_check_reg_init(data);
	//	if (error) {
	//		dev_err(&client->dev, "Failed to initialize config\n");
	//		return error;
	//	}

	if (mxt_get_object(data, MXT_TOUCH_MULTI_T100) != NULL)
	{
		error = mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_TCHAUX, &data->t100_tchaux_bits);
		if (error != HAL_OK)
			return error;
	}

	// set touch threshold
		uint8_t init_val =0;
		//Put non-zero value to calibrate
		mxt_write_object(data, MXT_GEN_COMMAND_T6, MXT_COMMAND_CALIBRATE, 11);
		//Time interval between successive idle burst cycles.  Typical: 32ms
		mxt_write_object(data, MXT_GEN_POWER_T7, MXT_POWER_IDLEACQINT, 32);
		//Time interval between successive active burst cycles.  Typical: 16ms
		mxt_write_object(data, MXT_GEN_POWER_T7, MXT_POWER_ACTVACQINT, 16);
		//Timeout period; the device remains active for this period after the first touch. Typical: 50 (10 sec)
		mxt_write_object(data, MXT_GEN_POWER_T7, MXT_POWER_ACTV2IDLETO, 50);
		//start in active mode, suppress overflow msgs, 4 MSBs, pipeline both modes
		mxt_write_object(data, MXT_GEN_POWER_T7, MXT_POWER_CFG, 0b11000011);
		//The ACTV2IDLETO is reset on subsequent touches and counts from the last touch
		mxt_read_object(data, MXT_GEN_POWER_T7, MXT_POWER_CFG2, &init_val);
		init_val = init_val & (0b11111110);
		mxt_write_object(data, MXT_GEN_POWER_T7, MXT_POWER_CFG2, init_val);
		///////////
		//Scanning time. default: 0 (2us)
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_CHRGTIME, 0);
		//Typical: 20
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_TCHDRIFT, 20);
		//Default:
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_DRIFTST, 10);
		//Recalibrate if an object remains in contact with CTP for certain time (0 = dont calibrate)
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_TCHAUTOCAL, 50);
		//dont Sync acquisition cycles with external clock
		mxt_read_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_SYNC, &init_val);
		init_val = init_val & (0b11111010);
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_SYNC, init_val);
		//Should be less than DRIFTST and 0 if only mutual capacitace is used
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_ATCHCALST, 0);
		//Typical: 0
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_ATCHCALSTHR, 0);
		//Typical: 50
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_ATCHFRCCALTHR, 50);
		//Typical: 25
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_ATCHFRCCALRATIO, 25);
		//#################################################################################
		mxt_read_object(data, MXT_SPT_AUXTOUCHCONFIG_T104, MXT_AUXTCHCFG_CTRL, &init_val);
		init_val = init_val | (0b00000001);
		mxt_write_object(data,MXT_SPT_AUXTOUCHCONFIG_T104, MXT_AUXTCHCFG_CTRL, init_val);
		//#################################################################################
		//Enable/disable mutual cap, self cap, selfprox with bits 0 1 & 3 respectively
		mxt_read_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_MEASALLOW, &init_val);
		init_val = ((init_val | 0b00000011) & 0b11110111);
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_MEASALLOW, &init_val);
		//Enable/disable mutual cap, self cap, selfprox in idle mode with bits 0 1 & 3 respectively
		mxt_read_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_MEASIDLEDEF, &init_val);
		init_val = ((init_val | 0b00000001) & 0b11110101);
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_MEASIDLEDEF, &init_val);
		//Enable/disable mutual cap, self cap in active mode with bits 0 & 1 respectively (Enable only one)
		mxt_read_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_MEASACTVDEF, &init_val);
		init_val = ((init_val | 0b00000001) & 0b11111101);
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_MEASACTVDEF, init_val);
		//#############################################################################
		//1 is recommended by microchip
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_REFMODE, 1);
		//
		mxt_read_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_CFG, &init_val);
		init_val = init_val & (0b01111001);
		mxt_write_object(data, MXT_GEN_ACQUIRE_T8, MXT_ACQUIRE_CFG, init_val);
		////////
		//Disable the T15 Key array object
		mxt_read_object(data, MXT_TOUCH_KEYARRAY_T15, MXT_KEYARRAY_CTRL, &init_val);
		init_val = init_val & (0b00110011);
		mxt_write_object(data, MXT_TOUCH_KEYARRAY_T15, MXT_KEYARRAY_CTRL, init_val);
		//###########################################################################
		mxt_read_object(data, MXT_SPT_COMMSCONFIG_T18, MXT_COMMS_CTRL, &init_val);
		init_val = (init_val & (0b00110011)) |
		mxt_write_object(data, MXT_SPT_COMMSCONFIG_T18, MXT_COMMS_CTRL, init_val);
		mxt_write_object(data, MXT_SPT_COMMSCONFIG_T18, MXT_COMMS_CMD, 0);
		//###########################################################################
		//X & Y resolution of the CTP
		mxt_write_object(data,  MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_XRANGE_MSB, (ts_SizeX >> 8) & 0xff);
		mxt_write_object(data,  MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_XRANGE_LSB, (uint8_t)(ts_SizeX & 0xff));
		mxt_write_object(data,  MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_YRANGE_MSB, (ts_SizeY >> 8) & 0xff);
		mxt_write_object(data,  MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_YRANGE_LSB, (uint8_t)(ts_SizeY & 0xff));
		//Orientation of the LCD & T100 message generation
		mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_CFG1, &init_val);
		init_val = (init_val & ~(0xE0)) | (0x80);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_CFG1, init_val);
		mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_CTRL, &init_val);
		init_val = init_val | (0b01111111);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_CTRL, init_val);
		init_val = init_val & (0b01111111);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_CTRL, init_val);
		//CTP is vertically flipped & message will be generated on events only
		mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_SCRAUX, &init_val);
		init_val = init_val | (0b01001110);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_SCRAUX, init_val);
		//Maximum number of touches to detect
		mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_NUMTCH, &init_val);
		init_val = init_val | (0b00000001);//(0b1110101);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_NUMTCH, init_val);
		init_val = init_val & (0b00000001);//(0b1110101);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_NUMTCH, init_val);
		// set touch threshold
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_TCHTHR, 25);
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_DXGAIN, 10);
		mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_TCHDIDOWN, &init_val);
		init_val =   (init_val  & (0b1100010)) | 0b00000010;
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_TCHDIDOWN, init_val);
		 //
		mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_TCHDIUP, &init_val);
		init_val = (init_val  & (0b1100010)) | 0b00000010;
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_TCHDIUP, init_val);
		//
		mxt_read_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_NEXTTCHDI, &init_val);
		init_val = (init_val  & (0b1100010)) | 0b00000010;
		mxt_write_object(data, MXT_TOUCH_MULTI_T100, MXT_MULTITOUCH_NEXTTCHDI, init_val);
		//################################################################################
	/*
		mxt_read_object(data, MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_CTRL, &init_val);
		init_val = (init_val | (0b00000001)) & 0b00000001;
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_CTRL, init_val);

		mxt_read_object(data, MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_CFG1, &init_val);
		init_val = (init_val | (0b00000001)) & 0b11111101;
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_CFG1, init_val);

		mxt_read_object(data, MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_CFG2, &init_val);
		init_val = (init_val) & 0b11111101;
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_CFG2, init_val);
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_HOPCNT, 255);
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_HOPCNTPER, 1);

		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_NLGAINDUALX, 15);
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_MINNLTHR, 20);//thr/2
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_MINNLTHR, 20);//thr/2
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_NLTHRMARGIN, 20);//same as MINNLTHR
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_MINTHRADJ, 19);
		//Tuning//mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_NLTHRLIMIT, 19);
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_NLGAINSINGX, 15);
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_CFG3, 216);
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_BGSCAN, 11);
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_BGSCAN, 9);
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_NOISCNT, 3);
		mxt_read_object(data, MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_VNOICTRL, &init_val);
		init_val = (init_val) | 0b00001001;
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_VNOICTRL, init_val);
		mxt_write_object(data,MXT_PROCG_NOISESUPPRESSION_T72, MXT_NOISESUP_VNOICNT, 3);


		mxt_read_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_CTRL, &init_val);
		init_val = init_val | (0b00000001);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108,MXT_SELF_CAP_MAXCHARGER_CTRL , init_val);

		mxt_read_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_CALCFG1, &init_val);
		init_val = init_val | (0b00000001);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_CALCFG1, init_val);

		mxt_read_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_CFG3, &init_val);
		init_val = init_val | (0b01000000);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_CFG3, init_val);

		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_IIRCOEFF, 0);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_MINNLTDDIFF, 6);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_MINNLTDHOP, 3);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_HOPST, 0);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_STABCTRL, 0);

		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_NOISCTRL, 0);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_NOISCNT, 3);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_VNOICTRL, 15);
		mxt_write_object(data, MXT_PROCG_NOISESUPSELFCAP_T108, MXT_SELF_CAP_MAXCHARGER_VNOICNT, 3);
	*/
		mxt_read_object(data, MXT_PROCI_RETRANSMISSIONCOMPENSATION_T80, MXT_RETRANS_CTRL, &init_val);
		init_val = (init_val | (0b00000001)) & (0b11000001);
		mxt_write_object(data, MXT_PROCI_RETRANSMISSIONCOMPENSATION_T80, MXT_RETRANS_CTRL, init_val);


	//	while(1)
	//		mxt_read_messages_t44(data);

	//	error = mxt_get_init_setting(data);
	//	if (error != HAL_OK)
	//		return error;

	return error;
}
/**
 * @brief Initialize the internal touch screen drivers
 * @param ts_SizeX Horizontal screen resolution
 * @param ts_SizeY Vertical screen resolution
 * @return <c>HAL_OK</c> if successful else error
 */
uint16_t MXTDrivers_Init(uint16_t ts_SizeX, uint16_t ts_SizeY)
{
	return mxt_initialize(&mxtData, ts_SizeX, ts_SizeY);
}

/**
 * @brief Detect the state and locations of the touch screen
 * @param x Location to be filled with x position
 * @param y Location to be filled with y position
 * @return <c>true</c> if touch screen pressed else <c>false</c>
 */
bool MXTDrivers_GetState(uint16_t* x, uint16_t* y)
{
	mxt_read_messages_t44(&mxtData);
	for (int i = 0; i < MXT_MAX_FINGER_NUM; i++)
	{
		if(mxtData.finger_down[i] == true)
		{
			*x = xPress;
			*y = yPress;
			return true;
		}
	}
	return false;
}

/* EOF */
