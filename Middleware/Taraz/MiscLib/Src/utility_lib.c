/**
 ********************************************************************************
 * @file    	utility_lib.c
 * @author 		Waqas Ehsan Butt
 * @date    	May 16, 2023
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
#include "utility_lib.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define ON_TEXTS_COUNT				(3)
#define OFF_TEXTS_COUNT				(3)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/

/********************************************************************************
 * Static Variables
 *******************************************************************************/
static const char* onTxts[ON_TEXTS_COUNT] = { "TRUE", "ON", "1" };
static const char* offTxts[OFF_TEXTS_COUNT] = { "FALSE", "OFF", "0" };
/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
uint8_t String_FindnCharsIndices(const char *txt, char item, uint8_t* indices, uint8_t count)
{
	uint8_t index = 0;
	int i = 0;
	// go through the length of the string
	for (; txt[i] != 0; i++)
	{
		// compare and return if equal
		if (txt[i] == item)
		{
			if(index < count)
				indices[index] = i;
			index++;
		}
	}
	// return length
	if(index == 0)
		indices[0] = i;
	// if not found return null pointer
	return index;
}
static void ReverseString(char* txt, int len)
{
	int start = 0;
	int end = len - 1;
	while (start < end)
	{
		char temp = txt[start];
		txt[start] = txt[end];
		txt[end] = temp;
		start++;
		end--;
	}
}
static bool ceil_custom(char* firstNumLoc, char* lastNumLoc)
{
	if (*firstNumLoc != '9')			// --TODO-- include for '9' later also
	{
		do
		{
			if (*lastNumLoc == '9')
				*lastNumLoc = '0';
			else if (*lastNumLoc < '9' && *lastNumLoc >= '0')
			{
				*lastNumLoc = *lastNumLoc + 1;
				break;
			}
		} while (lastNumLoc-- != firstNumLoc);
	}
	return false;
}
/**
 * @brief Concatenates two strings
 * @param dest Destination string containing the first part where the src part will be appended
 * @param src String to be appened in destination
 * @param destLen Length of the first string
 * @param insertSpace Selects if a space characters needs to be inserted in between the strings
 * @return Length of the resulting string
 */
int strcat_custom(char* dest, const char* src, int destLen, bool insertSpace)
{
	if (src != NULL)
	{
		if (insertSpace)
			dest[destLen++] = ' ';
		int unitIndex = 0;
		while (src[unitIndex] != 0)
			dest[destLen++] = src[unitIndex++];
		dest[destLen++] = 0;
	}
	return destLen;
}
/**
 * @brief This function converts the floating point number to character string
 * @param f Single precision floating point number to be converted
 * @param txt Pointer to the string
 * @param maxDigits Max number of digits to be displayed
 * @param precision Precision of reading to be displayed to avoid showing negligible numbers
 * @return Number of characters in the string
 */
int ftoa_custom(float f, char* txt, int maxDigits, int precision)
{
	if (maxDigits < 1)
		return 0;

	char* txt0 = txt;

	int val;
	if (f < 0)
	{
		*txt++ = '-';
		f *= -1;
	}

	char* firstNumLoc = txt;

	val = (int)f;
	int digits = 0;
	int div = 1;
	while (val > 0)
	{
		val /= 10;
		div *= 10;
		digits++;
	}

	if (digits > maxDigits)
	{
		val = (int)f;
		if (div > 100)
			val /= (div / 100);
		*txt++ = (val / 10) + '0';
		*txt++ = '.';
		*txt++ = (val % 10) + '0';
		*txt++ = 'e';
		digits -= 1;
		if(digits >= 10)
			*txt++ = (digits / 10) + '0';
		*txt++ = (digits % 10) + '0';
	}
	else
	{
		val = (int)f;
		itoa_custom(val, txt);
		txt += digits;
		if (val == 0)
			*txt++ = '0';
		int no1 = maxDigits - digits;
		int no2 = precision;
		int precisionCompute = no1 > no2 ? no2 : no1;
		if (precisionCompute > 0)
		{
			*txt++ = '.';
			float frac = f - val;
			for (int i = 0; i < precisionCompute ; i++)
			{
				frac *= 10;
				*txt++ = ((int)frac) % 10 + '0';
			}

			frac *= 10;
			if (((int)frac) % 10  >= 5)
			{
				if (ceil_custom(firstNumLoc, txt-1))
					txt++;
			}
		}
	}
	*txt = 0;

	return txt - txt0;
}
/**
 * @brief Custom implementation to convert string to single precision number if possible.
 * @param txt Pointer to the text field.
 * @param val Pointer to the single precision value to be updated
 * @return <c>true</c> if successful else <c>false</c>
 */
bool atof_custom(const char* txt, float* val)
{
	// if nothing found
	if(*txt == 0)
		return false;
	*val = 0;
	uint32_t intVal = 0, floatVal = 0, div = 1;
	int i = 0;
	// handle negative numbers
	bool isNeg = false;
	if(txt[i] == '-')
	{
		isNeg = true;
		txt++;
	}
	// handle decimal
	uint8_t dotIndex = 0, dotCount = 0;
	dotCount = String_FindnCharsIndices(txt, '.', &dotIndex, 1);
	// more than possible dots
	if(dotCount > 1)
		return false;
	// if integer part is impossible to convert due to long length
	if(dotIndex > 8)
		return false;
	// get integer value
	for(int i = 0; i < dotIndex; i++)
	{
		if(txt[i] <= '9' && txt[i] >= '0')
			intVal = (intVal * 10) + (txt[i] - '0');
		// error if any other character present
		else
			return false;
	}
	if(dotCount == 1)
	{
		for(int i = dotIndex + 1 ; txt[i] != 0; i++)
		{
			if(txt[i] <= '9' && txt[i] >= '0')
			{
				floatVal = (floatVal * 10) + (txt[i] - '0');
				div *= 10;
			}
			// error if any other character present
			else
				return false;
		}
		// if floating part is impossible to convert due to long length
		if((i - dotIndex) > 9)
			return false;
		*val = ((float)(floatVal) / div);
		*val += (intVal);
	}
	else
	{
		*val = intVal;
	}
	if(isNeg)
		*val *= (-1.0f);
	return true;
}
/**
 * @brief Custom implementation to convert boolean value to string.
 * @param b Boolean value.
 * @param txt Pointer to the text field to be filled.
 * @return Number of characters in the string.
 */
int btoa_custom(bool b, char* txt)
{
	char* txt0 = txt;
	*txt++ = 'O';
	if (b == false)
	{
		*txt++ = 'F';
		*txt++ = 'F';
	}
	else
		*txt++ = 'N';
	*txt = 0;
	return txt - txt0;
}
/**
 * @brief Custom implementation to convert string to boolean if possible.
 * @param txt Pointer to the text field.
 * @param val Pointer to the boolean value to be updated
 * @return <c>true</c> if successful else <c>false</c>
 */
bool atob_custom(const char* txt, bool* val)
{
	// if nothing found
	if(*txt == 0)
		return false;

	*val = false;

	char txtCap[6] = {0};
	int count = 0;
	for (; count < 5 && txt[count] != 0; count++)
		txtCap[count] = (txt[count] >= 'a' && txt[count] <= 'z') ? txt[count] - ('a' - 'A') : txt[count];

	for (int i = 0; i < ON_TEXTS_COUNT; i++)
	{
		const char* refTxt = onTxts[i];
		if (refTxt[count] != 0)
			continue;
		int j = 0;
		for (; j < count && refTxt[j] != 0; j++)
		{
			if (txtCap[j] != refTxt[j])
				continue;
		}
		if (j < count)
			continue;
		*val = true;
		return true;
	}

	for (int i = 0; i < OFF_TEXTS_COUNT; i++)
	{
		const char* refTxt = offTxts[i];
		if (refTxt[count] != 0)
			continue;
		int j = 0;
		for (; j < count && refTxt[j] != 0; j++)
		{
			if (txtCap[j] != refTxt[j])
				continue;
		}
		if (j < count)
			continue;
		*val = false;
		return true;
	}
	return false;
}
/**
 * @brief This function converts the integer number to character string
 * @param val Value of the integer
 * @param txt Pointer to the string
 * @return Number of characters in the string.
 */
int itoa_custom(int32_t val, char* txt)
{
	bool isNegative = false;
	int i = 0;

	if (val == 0)
	{
		txt[i++] = '0';
		txt[i] = 0;
		return 1;
	}

	if (val < 0)
	{
		isNegative = true;
		val = -val;
	}

	while (val != 0)
	{
		int digit = val % 10;
		txt[i++] = digit + '0';
		val = val / 10;
	}

	if (isNegative)
		txt[i++] = '-';

	txt[i] = 0;

	ReverseString(txt, i);

	return i;
}
/**
 * @brief This function converts the unsigned integer number to character string
 * @param val Value of the unsigned integer
 * @param txt Pointer to the string
 * @return Number of characters in the string.
 */
int utoa_custom(uint32_t val, char* txt)
{
	int i = 0;

	if (val == 0)
	{
		txt[i++] = '0';
		txt[i] = 0;
		return 1;
	}

	while (val != 0)
	{
		int digit = val % 10;
		txt[i++] = digit + '0';
		val = val / 10;
	}

	txt[i] = 0;

	ReverseString(txt, i);

	return i;
}
/**
 * @brief Custom implementation to convert string to integer if possible.
 * @param txt Text representation
 * @param result Result of the conversion
 * @return <c>true</c> if successful else <c>false</c>
 */
bool atoi_custom(const char *txt, int32_t *result)
{
	if (txt == NULL)
		return false;

	int32_t num = 0;
	bool isNegative = false;
	int i = 0;

	// Check for optional sign
	if (txt[i] == '-' || txt[i] == '+') {
		isNegative = (txt[i] == '-');
		i++;
	}

	// Convert remaining digits
	for (; txt[i] != '\0'; i++) {
		if (txt[i] >= '0' && txt[i] <= '9')
			num = num * 10 + (txt[i] - '0');
		else
			// Invalid character encountered
			return false;
	}

	// Apply sign
	if (isNegative)
		num = -num;

	*result = num;
	return true;
}
/**
 * @brief Custom implementation to convert string to unsigned integer if possible.
 * @param txt Text representation
 * @param result Result of the conversion
 * @return <c>true</c> if successful else <c>false</c>
 */
bool atou_custom(const char *txt, uint32_t *result)
{
	if (txt == NULL)
		return false;

	uint32_t num = 0;
	int i = 0;

	// Check for optional sign
	if (txt[i] == '+')
		i++;

	// Convert remaining digits
	for (; txt[i] != '\0'; i++) {
		if (txt[i] >= '0' && txt[i] <= '9')
			num = num * 10 + (txt[i] - '0');
		else
			// Invalid character encountered
			return false;
	}

	*result = num;
	return true;
}



/* EOF */
