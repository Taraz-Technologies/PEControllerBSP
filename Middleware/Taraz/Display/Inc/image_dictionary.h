/**
 ********************************************************************************
 * @file 		image_dictionary.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 18, 2023
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

#ifndef IMAGE_DICTIONARY_H_
#define IMAGE_DICTIONARY_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup Display
 * @{
 */

/** @defgroup Images Images
 * @brief Contains the declaration and procedures for available images.
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define LOAD_IMAGES								(0)
#define SYS_IMGS_BASE_ADDRESS					(0x08020000)
// Make sure that the images are in the correct order
//#define BASE_ADDRESS							(const uint8_t*)(0x08020000)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup IMGS_Exported_Structures Structures
 * @{
 */
/**
 * @brief Contains the relevant information for an image
 */
typedef struct
{
	uint16_t width;	 		/**< @brief Image width */
	uint16_t height;	 	/**< @brief Image height */
	uint32_t pixelFormat;	/**< @brief Image pixel format LTDC_Pixelformat */
	const uint8_t* data;	/**< @brief Image pixel color data in specified format */
} image_info_t;
/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup IMGS_Exported_Variables Variables
 * @{
 */
/**
 * @brief Taraz logo information
 */
extern image_info_t taraz_logo_info;
/**
 * @brief intelliSENS QR Code information
 */
extern image_info_t intelliSENS_QR_info;
/**
 * @brief intelliSENS logo information
 */
extern image_info_t intelliSENS_logo_info;
/**
 * @brief intelliSENS icon information
 */
extern image_info_t intelliSENS_icon_info;
/**
 * @brief BSP documentation QR Code information
 */
extern image_info_t bsp_docs_QR_info;
/**
 * @}
 */
/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif
/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
#endif 
/* EOF */
