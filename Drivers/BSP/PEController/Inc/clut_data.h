/**
 ********************************************************************************
 * @file 		clut_data.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 12, 2023
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

#ifndef DISPLAY_CLUT_DATA_H_
#define DISPLAY_CLUT_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
 * @{
 */

/** @addtogroup Display
 * @{
 */

/** @addtogroup CLUT Color look up table
 * @brief Contains the mapping scheme for the color look up table of the PEController.
 * @{
 */
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

/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup CLUTDATA_Exported_Typedefs Type Definitions
  * @{
  */
/**
 * @brief Array containing the look-up table data for the CLUT module of PEController.
 */
extern const unsigned clut_data[256];
/**
 * @brief Color mapping array to map the colors to 256 available colors
 */
extern const unsigned char color_map[65536];
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
