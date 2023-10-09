/**
 ********************************************************************************
 * @file 		app_screen_data.h
 * @author 		Waqas Ehsan Butt
 * @date 		Jun 6, 2023
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

#ifndef APP_SCREEN_DATA_H_
#define APP_SCREEN_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup BSP
 * @{
 */
/** @addtogroup Display
 * @{
 */
/** @defgroup DisplayScreens Screens
 * @brief Contains the declaration and procedures for the display screens
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "image_dictionary.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup PEDISPLAYSCREEN_Exported_Structures Structures
  * @{
  */
/**
 * @brief Contains the information of the application required by the display unit.
 */
typedef struct
{
	const char* appInfo;							/**< @brief Text representing the application information.
															Set to NULL if no need to display this parameter.*/
	const char* connectionInfo;						/**< @brief Text representing the connection information for the application.
															Set to NULL if no need to display this parameter.*/
	const char* documentationLink;					/**< @brief Text representing the documentation link for the application.
															Set to NULL if no need to display this parameter.*/
	image_info_t* img;								/**< @brief Image displayed in the application screen.
															Set to NULL if no image to display.*/
} appinfo_display_t;
/**
  * @}
  */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/
/** @defgroup PEDISPLAYSCREEN_Exported_Variables Variables
  * @{
  */
/**
 * @brief This information will be displayed in the application information screen
 * @note The information is application dependent and can be edited in <b>screen_appinfo_data.c</b>.
 * The file is located in \"CM4/BSP/Display\" folder in the relevant project
 */
extern appinfo_display_t appInfoDisplay;
/**
 * @brief Assigns the image to be displayed on the splash screen
 */
extern image_info_t* splashImg;
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
