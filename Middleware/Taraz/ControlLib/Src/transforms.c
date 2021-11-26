/**
 ********************************************************************************
 * @file    	Transformations.c
 * @author 		Waqas Ehsan Butt
 * @date    	Nov 25, 2021
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
#include "transforms.h"
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
 * @brief Performs the Clarke and inverse Clarke transformations
 * @param *abc Pointer to the abc coordinates structure
 * @param *alBe0 Pointer to the alpha Beta zero coordinates structure
 * @param src conversion source. If src = SRC_ABC converts from ABC to ALBE0 else converts from ALBE0 to ABC coordinates
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/abctoalphabetazeroalphabetazerotoabc.html
 */
void Transform_abc_alBe0(LIB_3COOR_ABC_t* abc, LIB_3COOR_ALBE0_t* alBe0, transformation_source_t src)
{
	if(src == SRC_ABC)
	{
		alBe0->alpha = (2 * abc->a - abc->b - abc->c) / (3.f);
		alBe0->beta = (abc->b - abc->c ) / sqrtf(3.f);
		alBe0->zero = (abc->a + abc->b + abc->c) / (3.f);
	}
	else
	{
		abc->a = alBe0->alpha + alBe0->zero;
		abc->b = ((sqrtf(3.f) * alBe0->beta)- alBe0->alpha) / 2.f + alBe0->zero;
		abc->c = (-(sqrtf(3.f) * alBe0->beta)- alBe0->alpha) / 2.f + alBe0->zero;
	}
}

/**
 * @brief Performs the Park and inverse Park transformations
 * @param *alBe0 Pointer to the alpha Beta zero coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param *trigno Pointer to the trigonometric structure, Make sure that the Sines and Cosines are pre-computed before calling this method
 * @param src conversion source. If src = SRC_ALBE0 converts from ALBE0 to DQ0 else converts from DQ0 to ALBE0 coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 *
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/alphabetazerotodq0dq0toalphabetazero.html?s_tid=doc_ta
 */
void Transform_alphaBeta0_dq0(LIB_3COOR_ALBE0_t* alBe0, LIB_3COOR_DQ0_t* dq0, LIB_3COOR_TRIGNO_t* trigno,
		transformation_source_t src, park_transform_type_t parkType)
{
#if	USE_PRECOMPUTED_TRIG
	// ALBE0 to DQO transform
	if (src == SRC_ALBE0)
	{
		dq0->zero = alBe0->zero;
		if (parkType == PARK_COSINE)
		{
			dq0->d = alBe0->alpha * trigno->cos + alBe0->beta * trigno->sin;
			dq0->q = -alBe0->alpha *  trigno->sin + alBe0->beta * trigno->cos;
		}
		else
		{
			dq0->d = alBe0->alpha * trigno->sin + alBe0->beta * -trigno->cos;
			dq0->q = -alBe0->alpha * -trigno->cos + alBe0->beta * trigno->sin;
		}
	}
	// DQ0 to ALBE0 transform
	else
	{
		alBe0->zero = dq0->zero;
		if (parkType == PARK_COSINE)
		{
			alBe0->alpha = dq0->d * trigno->cos - dq0->q *  trigno->sin;
			alBe0->beta = dq0->d *  trigno->sin + dq0->q * trigno->cos;
		}
		else
		{
			alBe0->alpha = dq0->d * trigno->sin - dq0->q * -trigno->cos;
			alBe0->beta = dq0->d * -trigno->cos + dq0->q * trigno->sin;
		}
	}
#else
	// ALBE0 to DQO transform
	if (src == SRC_ALBE0)
	{
		dq0->zero = alBe0->zero;
		if (parkType == PARK_COSINE)
		{
			dq0->d = alBe0->alpha * cosf(trigno->wt) + alBe0->beta * sinf(trigno->wt);
			dq0->q = -alBe0->alpha *  sinf(trigno->wt) + alBe0->beta * cosf(trigno->wt);
		}
		else
		{
			dq0->d = alBe0->alpha * cosf(trigno->wt - (PI / 2)) + alBe0->beta * sinf(trigno->wt - (PI / 2));
			dq0->q = -alBe0->alpha * sinf(trigno->wt - (PI / 2)) + alBe0->beta * cosf(trigno->wt - (PI / 2));
		}
	}
	// DQ0 to ALBE0 transform
	else
	{
		alBe0->zero = dq0->zero;
		if (parkType == PARK_COSINE)
		{
			alBe0->alpha = dq0->d * cosf(trigno->wt) - dq0->q *  sinf(trigno->wt);
			alBe0->beta = dq0->d *  sinf(trigno->wt) + dq0->q * cosf(trigno->wt);
		}
		else
		{
			alBe0->alpha = dq0->d * cosf(trigno->wt - (PI / 2)) - dq0->q * sinf(trigno->wt - (PI / 2));
			alBe0->beta = dq0->d * sinf(trigno->wt - (PI / 2)) + dq0->q * cosf(trigno->wt - (PI / 2));
		}
	}
#endif
}

/**
 * @brief Performs the Park and inverse Park transformations with wt = 0
 * @param *alBe0 Pointer to the alpha Beta zero coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param src conversion source. If src = SRC_ALBE0 converts from ALBE0 to DQ0 else converts from DQ0 to ALBE0 coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/alphabetazerotodq0dq0toalphabetazero.html?s_tid=doc_ta
 */
void Transform_alphaBeta0_dq0_wt0(LIB_3COOR_ALBE0_t* alBe0, LIB_3COOR_DQ0_t* dq0,
		transformation_source_t src, park_transform_type_t parkType)
{
	dq0->zero = alBe0->zero;
	// ALBE0 to DQO transform
	if (src == SRC_ALBE0)
	{
		if (parkType == PARK_COSINE)
		{
			dq0->d = alBe0->alpha;
			dq0->q = alBe0->beta;
		}
		else
		{
			dq0->d = -alBe0->beta;
			dq0->q = alBe0->alpha;
		}
	}
	// DQ0 to ALBE0 transform
	else
	{
		if (parkType == PARK_COSINE)
		{
			alBe0->alpha = dq0->d;
			alBe0->beta = dq0->q;
		}
		else
		{
			alBe0->alpha = dq0->q;
			alBe0->beta = -dq0->d;
		}
	}
}

/**
 * @brief Performs the transforms between ABC and DQ0 coordinates
 * @param *abc Pointer to the abc coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param *trigno Pointer to the trigonometric structure, Make sure that the Sines and Cosines are pre-computed before calling this method
 * @param src conversion source. If src = SRC_ABC converts from ABC to DQ0 else converts from DQ0 to ABC coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 *
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/abctodq0dq0toabc.html?s_tid=doc_ta
 */
void Transform_abc_dq0(LIB_3COOR_ABC_t* abc, LIB_3COOR_DQ0_t* dq0, LIB_3COOR_TRIGNO_t* trigno,
		transformation_source_t src, park_transform_type_t parkType)
{
#if	USE_PRECOMPUTED_TRIG
	// ABC to DQO transform
	if (src == SRC_ABC)
	{
		if (parkType == PARK_COSINE)
		{
			dq0->d = 2/3.f * (abc->a * trigno->cos + abc->b * trigno->cos_m2pB3 + abc->c * trigno->cos_p2pB3);
			dq0->q = -2/3.f * (abc->a * trigno->sin + abc->b * trigno->sin_m2pB3 + abc->c * trigno->sin_p2pB3);
			dq0->zero = (abc->a + abc->b + abc->c) / 3.f;
		}
		else
		{
			dq0->d = 2/3.f * (abc->a * trigno->sin + abc->b * trigno->sin_m2pB3 + abc->c * trigno->sin_p2pB3);
			dq0->q = 2/3.f * (abc->a * trigno->cos + abc->b * trigno->cos_m2pB3 + abc->c * trigno->cos_p2pB3);
			dq0->zero = (abc->a + abc->b + abc->c) / 3.f;
		}
	}
	// DQ0 to ABC transform
	else
	{
		if (parkType == PARK_COSINE)
		{
			abc->a = dq0->d * trigno->cos - dq0->q * trigno->sin + dq0->zero;
			abc->b = dq0->d * trigno->cos_m2pB3 - dq0->q * trigno->sin_m2pB3 + dq0->zero;
			abc->c = dq0->d * trigno->cos_p2pB3 - dq0->q * trigno->sin_p2pB3 + dq0->zero;
		}
		else
		{
			abc->a = dq0->d * trigno->sin + dq0->q * trigno->cos + dq0->zero;
			abc->b = dq0->d * trigno->sin_m2pB3 + dq0->q * trigno->cos_m2pB3 + dq0->zero;
			abc->c = dq0->d * trigno->sin_p2pB3 + dq0->q * trigno->cos_p2pB3 + dq0->zero;
		}
	}
#else
	// ABC to DQO transform
	if (src == SRC_ABC)
	{
		if (parkType == PARK_COSINE)
		{
			dq0->d = 2/3.f * (abc->a * cosf(trigno->wt) + abc->b * cosf((trigno->wt - TWO_PI/3)) + abc->c * cosf((trigno->wt + TWO_PI/3)));
			dq0->q = -2/3.f * (abc->a * sinf(trigno->wt) + abc->b * sinf((trigno->wt - TWO_PI/3)) + abc->c * sinf((trigno->wt + TWO_PI/3)));
			dq0->zero = (abc->a + abc->b + abc->c) / 3.f;
		}
		else
		{
			dq0->d = 2/3.f * (abc->a * sinf(trigno->wt) + abc->b * sinf((trigno->wt - TWO_PI/3)) + abc->c * sinf((trigno->wt + TWO_PI/3)));
			dq0->q = 2/3.f * (abc->a * cosf(trigno->wt) + abc->b * cosf((trigno->wt - TWO_PI/3)) + abc->c * cosf((trigno->wt + TWO_PI/3)));
			dq0->zero = (abc->a + abc->b + abc->c) / 3.f;
		}
	}
	// DQ0 to ABC transform
	else
	{
		if (parkType == PARK_COSINE)
		{
			abc->a = dq0->d * cosf(trigno->wt) - dq0->q * sin(trigno->wt) + dq0->zero;
			abc->b = dq0->d * cosf(trigno->wt - (TWO_PI/3)) - dq0->q * sinf(trigno->wt - (TWO_PI/3)) + dq0->zero;
			abc->c = dq0->d * cosf(trigno->wt + (TWO_PI/3)) - dq0->q * sinf(trigno->wt + (TWO_PI/3)) + dq0->zero;
		}
		else
		{
			abc->a = dq0->d * sinf(trigno->wt) + dq0->q * cosf(trigno->wt) + dq0->zero;
			abc->b = dq0->d * sinf(trigno->wt - (TWO_PI/3)) + dq0->q * cosf(trigno->wt - (TWO_PI/3)) + dq0->zero;
			abc->c = dq0->d * sinf(trigno->wt + (TWO_PI/3)) + dq0->q * cosf(trigno->wt + (TWO_PI/3)) + dq0->zero;
		}
	}
#endif
}

/**
 * @brief Performs the transforms between ABC and DQ0 coordinates  with wt = 0
 * @param *abc Pointer to the abc coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param src conversion source. If src = SRC_ABC converts from ABC to DQ0 else converts from DQ0 to ABC coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/abctodq0dq0toabc.html?s_tid=doc_ta
 */
void Transform_abc_dq0_wt0(LIB_3COOR_ABC_t* abc, LIB_3COOR_DQ0_t* dq0,
		transformation_source_t src, park_transform_type_t parkType)
{
	// ABC to DQO transform
	if (src == SRC_ABC)
	{
		if (parkType == PARK_COSINE)
		{
			dq0->d = 2/3.f * (abc->a + abc->b * COS_120A + abc->c * COS_120A);
			dq0->q = -2/3.f * (abc->b * -SIN_120 + abc->c * SIN_120);
			dq0->zero = (abc->a + abc->b + abc->c) / 3.f;
		}
		else
		{
			dq0->d = 2/3.f * (abc->b * -SIN_120 + abc->c * SIN_120);
			dq0->q = 2/3.f * (abc->a + abc->b * COS_120A + abc->c * COS_120A);
			dq0->zero = (abc->a + abc->b + abc->c) / 3.f;
		}
	}
	// DQ0 to ABC transform
	else
	{
		if (parkType == PARK_COSINE)
		{
			abc->a = dq0->d + dq0->zero;
			abc->b = dq0->d * COS_120A - dq0->q * -SIN_120 + dq0->zero;
			abc->c = dq0->d * COS_120A - dq0->q * SIN_120 + dq0->zero;
		}
		else
		{
			abc->a = dq0->q * 1 + dq0->zero;
			abc->b = dq0->d * -SIN_120 + dq0->q * COS_120A + dq0->zero;
			abc->c = dq0->d * SIN_120 + dq0->q * COS_120A + dq0->zero;
		}
	}
}

/**
 * @brief Transform wt to the trigonometric values required in DQ transforms to pre-compute before use
 * @param *trigno Pointer to the trigonometric information
 */
void Transform_wt_sincos(LIB_3COOR_TRIGNO_t *trigno)
{
	trigno->sin = sinf(trigno->wt);
	trigno->cos = cosf(trigno->wt);

	float casb = trigno->cos * SIN_120;
	float sacb = trigno->sin * COS_120A;
	float cacb = trigno->cos * COS_120A;
	float sasb = trigno->sin * SIN_120;

	trigno->sin_p2pB3 = sacb + casb;
	trigno->sin_m2pB3 = sacb - casb;
	trigno->cos_p2pB3 = cacb - sasb;
	trigno->cos_m2pB3 = cacb + sasb;
}

/**
 * @brief Transform theta from value to range 0-2pi
 * @param theta current value of theta
 * @return float transformed value of theta
 */
float Transform_Theta_0to2pi(float theta)
{
	// adjust theta for the overflow
	while (theta > TWO_PI)
		theta -= (TWO_PI);

	while (theta < 0)
		theta += (TWO_PI);

	return theta;
}

/* EOF */
