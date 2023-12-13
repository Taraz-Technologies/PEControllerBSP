/**
 ********************************************************************************
 * @file 		utility_lib.h
 * @author 		Waqas Ehsan Butt
 * @date 		May 16, 2023
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

#ifndef UTILITY_LIB_H_
#define UTILITY_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif
/** @addtogroup Misc_Library
 * @{
 */

/** @defgroup Utility_Lib Utility Library
 * @brief The library contains helper functions for generic usage.
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "error_config.h"
#include "data_config.h"
#include "limits.h"
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
/** @defgroup UTILITYLIB_Exported_Functions Functions
 * @{
 */
/**
 * @brief Copy the source string to destination
 * @param dest Copy destination
 * @param src Source string
 */
extern void CopyString(char *dest, const char *src);
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
 * @brief This function converts the floating point number to character string
 * @param f Single precision floating point number to be converted
 * @param txt Pointer to the string
 * @param maxDigits Max number of digits to be displayed
 * @param precision Precision of reading to be displayed to avoid showing negligible numbers
 * @return Number of characters in the string
 */
extern int ftoa_custom(float f, char* txt, int maxDigits, int precision);
/**
 * @brief Custom implementation to convert string to single precision number if possible.
 * @param txt Pointer to the text field.
 * @param val Pointer to the single precision value to be updated
 * @return <c>true</c> if successful else <c>false</c>
 */
extern bool atof_custom(const char* txt, float* f);
/**
 * @brief Custom implementation to convert boolean value to string.
 * @param b Boolean value.
 * @param txt Pointer to the text field to be filled.
 * @return Number of characters in the string.
 */
extern int btoa_custom(bool b, char* txt);
/**
 * @brief Custom implementation to convert string to boolean if possible.
 * @param txt Pointer to the text field.
 * @param val Pointer to the boolean value to be updated
 * @return <c>true</c> if successful else <c>false</c>
 */
extern bool atob_custom(const char* txt, bool* val);
/**
 * @brief This function converts the integer number to character string
 * @param val Value of the integer
 * @param txt Pointer to the string
 * @return Number of characters in the string.
 */
extern int itoa_custom(int32_t val, char* txt);
/**
 * @brief This function converts the unsigned integer number to character string
 * @param val Value of the unsigned integer
 * @param txt Pointer to the string
 * @return Number of characters in the string.
 */
extern int utoa_custom(uint32_t val, char* txt);
/**
 * @brief Custom implementation to convert string to integer if possible.
 * @param txt Text representation
 * @param result Result of the conversion
 * @return <c>true</c> if successful else <c>false</c>
 */
extern bool atoi_custom(const char *txt, int32_t *result);
/**
 * @brief Custom implementation to convert string to unsigned integer if possible.
 * @param txt Text representation
 * @param result Result of the conversion
 * @return <c>true</c> if successful else <c>false</c>
 */
extern bool atou_custom(const char *txt, uint32_t *result);
/********************************************************************************
 * Code
 *******************************************************************************/

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
