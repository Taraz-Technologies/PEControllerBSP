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

/********************************************************************************
 * Includes
 *******************************************************************************/

/********************************************************************************
 * Defines
 *******************************************************************************/

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
	const int modulo;		/**< @brief Array Size - 1 */
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
 * @brief Process ring buffer read index after read is done
 * @param buff Pointer to the relevant @ref ring_buffer_t
 */
static void RingBuffer_Read(ring_buffer_t* buff)
{
	buff->rdIndex = (buff->rdIndex + 1) & buff->modulo;
}
/**
 * @brief Process ring buffer write index after write is done
 * @param buff Pointer to the relevant @ref ring_buffer_t
 */
static void RingBuffer_Write(ring_buffer_t* buff)
{
	buff->wrIndex = (buff->wrIndex + 1) & buff->modulo;
}
/**
 * @brief Checks if the ring buffer is empty
 * @returns bool <c>true</c> if empty else <c>false</c>
 */
static bool RingBuffer_IsEmpty(ring_buffer_t* buff)
{
	return buff->rdIndex == buff->wrIndex;
}
/**
 * @brief Checks if the ring buffer is full
 * @returns bool <c>true</c> if full else <c>false</c>
 */
static bool RingBuffer_IsFull(ring_buffer_t* buff)
{
	return buff->rdIndex == ((buff->wrIndex + 1) & buff->modulo);
}
/*!
 * @brief Get the next write location in the ring buffer
 * @param buff Pointer to the relevant @ref ring_buffer_t
 * @retval int- Next write location
 */
static int RingBuffer_NextWriteLoc(ring_buffer_t* buff)
{
	return ((buff->wrIndex + 1) & buff->modulo);
}
/*!
 * @brief Get the next read location in the ring buffer
 * @param buff Pointer to the relevant @ref ring_buffer_t
 * @retval int- next read location
 */
static int RingBuffer_NextReadLoc(ring_buffer_t* buff)
{
	return ((buff->rdIndex + 1) & buff->modulo);
}
/*!
 * @brief Get the next index for given index
 * @param buff Pointer to the relevant @ref ring_buffer_t
 * @retval int- next index
 */
static int RingBuffer_NextLoc(ring_buffer_t* buff, int index)
{
	return ((index + 1) & buff->modulo);
}

/**
 * @brief Reset the indexes for the ring buffer
 * @param buff Pointer to the relevant @ref ring_buffer_t
 */
static void RingBuffer_Reset(ring_buffer_t* buff)
{
	buff->rdIndex = 0;
	buff->wrIndex = 0;
}


/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
