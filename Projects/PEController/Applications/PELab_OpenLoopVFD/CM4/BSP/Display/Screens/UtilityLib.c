/**
 ********************************************************************************
 * @file    	UtilityLib.c
 * @author 		Waqas Ehsan Butt
 * @date    	Mar 21, 2023
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
#include "UtilityLib.h"
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
 * Static Variables
 *******************************************************************************/

/********************************************************************************
 * Global Variables
 *******************************************************************************/

/********************************************************************************
 * Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief This function converts the integer number to character string
 * @param val Value of the integer
 * @param txt Pointer to the string
 * @param digits Integer digits to be printed
 */
void itoa_custom(int val, char* txt, int digits)
{
	txt[digits] = 0;
	while (digits-- > 0)
	{
		txt[digits] = val % 10 + '0';
		val /= 10;
	}
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
	bool isNeg = false;
	if (f < 0)
	{
		*txt++ = '-';
		f *= -1;
	}

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
		itoa_custom(val, txt, digits);
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
		}
	}
	*txt = 0;

	return txt - txt0;
}


/* EOF */
