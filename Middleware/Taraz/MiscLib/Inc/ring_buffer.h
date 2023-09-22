/**
 ********************************************************************************
 * @file 		ring_buffer.h
 * @author 		Waqas Ehsan Butt
 * @date 		Dec 27, 2021
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

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup Misc_Library Miscellaneous
 * @brief Contains the miscellaneous libraries.
 * @{
 */

/** @defgroup RingBuffer_Library Ring Buffer
 * @brief Contains helper functions and variables for ring buffer usage.
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
// save diagnostic state
#pragma GCC diagnostic push
// turn off the specific warning. Can also use "-Wall"
#pragma GCC diagnostic ignored "-Wunused-function"
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup RINGBUFFER_Exported_Structures Structures
  * @{
  */
/**
 * @brief Defines the parameters required by a ring buffer
 */
typedef struct
{
	int wrIndex;			/**< @brief Array index at which the next data needs to be placed */
	int rdIndex;			/**< @brief Array index from which the next data read should be done */
	int modulo;				/**< @brief Array Size - 1 */
} ring_buffer_t;
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup RINGBUFFER_Exported_Functions Functions
  * @{
  */
/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Update the @ref ring_buffer_t.rdIndex after single successful read.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 */
static void RingBuffer_Read(ring_buffer_t* buff)
{
	buff->rdIndex = (buff->rdIndex + 1) & buff->modulo;
}
/**
 * @brief Update the @ref ring_buffer_t.wrIndex after single successful write.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 */
static void RingBuffer_Write(ring_buffer_t* buff)
{
	buff->wrIndex = (buff->wrIndex + 1) & buff->modulo;
}
/**
 * @brief Update the @ref ring_buffer_t.rdIndex after multiple successful reads.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @param count No of reads done.
 */
static void RingBuffer_Read_Count(ring_buffer_t* buff, int count)
{
	buff->rdIndex = (buff->rdIndex + count) & buff->modulo;
}
/**
 * @brief Update the @ref ring_buffer_t.wrIndex after multiple successful writes.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @param count No of writes done.
 */
static void RingBuffer_Write_Count(ring_buffer_t* buff, int count)
{
	buff->wrIndex = (buff->wrIndex + count) & buff->modulo;
}
/**
 * @brief Checks if the ring buffer is empty.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @returns bool <c>true</c> if empty else <c>false</c>.
 */
static bool RingBuffer_IsEmpty(ring_buffer_t* buff)
{
	return buff->rdIndex == buff->wrIndex;
}
/**
 * @brief Checks if the ring buffer is full.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @returns bool <c>true</c> if full else <c>false</c>.
 */
static bool RingBuffer_IsFull(ring_buffer_t* buff)
{
	return buff->rdIndex == ((buff->wrIndex + 1) & buff->modulo);
}
/*!
 * @brief Get the next write location in the ring buffer.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @retval int- Next write location index in relevant buffer.
 */
static int RingBuffer_NextWriteLoc(ring_buffer_t* buff)
{
	return ((buff->wrIndex + 1) & buff->modulo);
}
/*!
 * @brief Get the next read location in the ring buffer.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @retval int- next read location index in relevant buffer.
 */
static int RingBuffer_NextReadLoc(ring_buffer_t* buff)
{
	return ((buff->rdIndex + 1) & buff->modulo);
}
/*!
 * @brief Get the next index for given index in the ring buffer.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @param index Initial index.
 * @retval int- next index in relevant buffer.
 */
static int RingBuffer_NextLoc(ring_buffer_t* buff, int index)
{
	return ((index + 1) & buff->modulo);
}

/**
 * @brief Reset the indexes for the ring buffer.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 */
static void RingBuffer_Reset(ring_buffer_t* buff)
{
	buff->rdIndex = 0;
	buff->wrIndex = 0;
}
/**
 * @brief Get the number of entries left till the end of the data buffer, if we consider it as a linear buffer.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @return Number of entries left till end of linearized buffer.
 */
static int RingBuffer_GetCountTillSize(ring_buffer_t* buff)
{
	return (buff->modulo + 1 - buff->rdIndex);
}
/**
 * @brief Get the number of available slots/entries in the @ref ring_buffer_t.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @return Number of available slots/entries in the ring_buffer_t.
 */
static int RingBuffer_GetPendingReadCount(ring_buffer_t* buff)
{
	if (buff->rdIndex > buff->wrIndex)
		return RingBuffer_GetCountTillSize(buff) + buff->wrIndex;
	else
		return buff->wrIndex - buff->rdIndex;
}
/**
 * @brief Get the number of available slots/entries in the @ref ring_buffer_t without looping back to the start of the buffer.
 * @param buff Pointer to the relevant @ref ring_buffer_t.
 * @return Number of available slots/entries in the ring_buffer_t in linear access mode.
 */
static int RingBuffer_GetPendingReadCountsTillEnd(ring_buffer_t* buff)
{
	if (buff->rdIndex > buff->wrIndex)
		return RingBuffer_GetCountTillSize(buff);
	else
		return buff->wrIndex - buff->rdIndex;
}

#pragma GCC diagnostic pop
/**
 * @}
 */
#ifdef __cplusplus
}
#endif
/**
 * @}
 */
/**
 * @}
 */
#endif 
/* EOF */
