/**
 ********************************************************************************
 * @file 		UtilityLib.h
 * @author 		Waqas Ehsan Butt
 * @date 		Mar 21, 2023
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

#ifndef DISPLAY_SCREENS_UTILITYLIB_H_
#define DISPLAY_SCREENS_UTILITYLIB_H_

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
/**
 * @brief Concatenates two strings
 * @param dest Destination string containing the first part where the src part will be appended
 * @param src String to be appened in destination
 * @param destLen Length of the first string
 * @param insertSpace Selects if a space characters needs to be inserted in between the strings
 * @return Length of the resulting string
 */
extern int strcat_custom(char* dest, const char* src, int destLen, bool insertSpace);
/**
 * @brief This function converts the integer number to character string
 * @param val Value of the integer
 * @param txt Pointer to the string
 * @param digits Integer digits to be printed
 */
extern void itoa_custom(int val, char* txt, int digits);
/**
 * @brief This function converts the floating point number to character string
 * @param f Single precision floating point number to be converted
 * @param txt Pointer to the string
 * @param maxDigits Max number of digits to be displayed
 * @param precision Precision of reading to be displayed to avoid showing negligible numbers
 * @return Number of characters in the string
 */
extern int ftoa_custom(float f, char* txt, int maxDigits, int precision);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
