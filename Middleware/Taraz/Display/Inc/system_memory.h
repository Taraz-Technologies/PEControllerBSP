/**
 ********************************************************************************
 * @file 		system_memory.h
 * @author 		Waqas Ehsan Butt
 * @date 		Oct 18, 2023
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

#ifndef SYSTEM_MEMORY_H_
#define SYSTEM_MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define TARAZ_LOGO_WIDTH				(265)
#define TARAZ_LOGO_HEIGHT				(108)
#define TARAZ_LOGO_FORMAT				(LTDC_PIXEL_FORMAT_ARGB8888)
#define TARAZ_LOGO_SIZE					(4*TARAZ_LOGO_WIDTH*TARAZ_LOGO_HEIGHT)

#define BSP_QR_WIDTH					(230)
#define BSP_QR_HEIGHT					(230)
#define BSP_QR_FORMAT					(LTDC_PIXEL_FORMAT_ARGB1555)
#define BSP_QR_SIZE						(2*BSP_QR_WIDTH*BSP_QR_HEIGHT)

#define intelliSENS_QR_WIDTH			(230)
#define intelliSENS_QR_HEIGHT			(230)
#define intelliSENS_QR_FORMAT			(LTDC_PIXEL_FORMAT_ARGB1555)
#define intelliSENS_QR_SIZE				(2*intelliSENS_QR_WIDTH*intelliSENS_QR_HEIGHT) 

#define intelliSENS_LOGO_WIDTH			(328)
#define intelliSENS_LOGO_HEIGHT			(32)
#define intelliSENS_LOGO_FORMAT			(LTDC_PIXEL_FORMAT_ARGB8888)
#define intelliSENS_LOGO_SIZE			(4*intelliSENS_LOGO_WIDTH*intelliSENS_LOGO_HEIGHT)


#define SYSTEM_IMAGES_SIZE				(TARAZ_LOGO_SIZE + BSP_QR_SIZE)
#define intelliSENS_IMAGES_SIZE			(intelliSENS_QR_SIZE + intelliSENS_LOGO_SIZE)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
