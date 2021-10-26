/**
 ********************************************************************************
 * @file 		Transformations.h
 * @author 		Waqas Ehsan Butt
 * @date 		Oct 5, 2021
 * @copyright 	TarazTechnologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */

#ifndef INC_TRANSFORMATIONS_H_
#define INC_TRANSFORMATIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "Coordinates.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define SIN_120				(0.866f)
#define COS_120A			(-0.5f)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @brief Defines the transformation sources */
typedef enum
{
	/** @brief Select the ABC coordinates as source */
	SRC_ABC,
	/** @brief Select the Alpha Beta 0 coordinates as source */
	SRC_ALBE0,
	/** @brief Select the DQ0 coordinates as source */
	SRC_DQ0
} transformation_source_t;

/**
 * @brief types of park transformation
 *
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/alphabetazerotodq0dq0toalphabetazero.html?s_tid=doc_ta
 */
typedef enum
{
	/** @brief Rotating frame aligned with A axis at t = 0.
		 This type of Park transformation is also known as the cosine-based Park transformation. */
	PARK_COSINE,/**< PARK_COSINE */
	/** @brief Rotating frame aligned 90 degrees behind A axis.
		 This type of Park transformation is also known as the cosine-based Park transformation. */
	PARK_SINE,  /**< PARK_SINE */
} park_transform_type_t;
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
/**
 * @brief Performs the Clarke and inverse Clarke transformations
 *
 * @param *abc Pointer to the abc coordinates structure
 * @param *alphaBeta0 Pointer to the alpha Beta zero coordinates structure
 * @param src conversion source. If src = SRC_ABC converts from ABC to ALBE0 else converts from ALBE0 to ABC coordinates
 *
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/abctoalphabetazeroalphabetazerotoabc.html
 */
static inline void Transform_abc_alBe0(LIB_3COOR_ABC_t* abc, LIB_3COOR_ALBE0_t* alBe0, transformation_source_t src)
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
 *
 * @param *alphaBeta0 Pointer to the alpha Beta zero coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param *angle Pointer to the angle structure, Make sure that the Sines and Cosines are precomputed before calling this method
 * @param src conversion source. If src = SRC_ALBE0 converts from ALBE0 to DQ0 else converts from DQ0 to ALBE0 coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 *
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/alphabetazerotodq0dq0toalphabetazero.html?s_tid=doc_ta
 */
static inline void Transform_alphaBeta0_dq0(LIB_3COOR_ALBE0_t* alBe0, LIB_3COOR_DQ0_t* dq0, LIB_3COOR_SINCOS_t* angle,
		transformation_source_t src, park_transform_type_t parkType)
{
	// ALBE0 to DQO transform
	if (src == SRC_ALBE0)
	{
		dq0->zero = alBe0->zero;
		if (parkType == PARK_COSINE)
		{
			dq0->d = alBe0->alpha * cosf(angle->wt) + alBe0->beta * sinf(angle->wt);
			dq0->q = -alBe0->alpha *  sinf(angle->wt) + alBe0->beta * cosf(angle->wt);
		}
		else
		{
			dq0->d = alBe0->alpha * cosf(angle->wt - (PI / 2)) + alBe0->beta * sinf(angle->wt - (PI / 2));
			dq0->q = -alBe0->alpha * sinf(angle->wt - (PI / 2)) + alBe0->beta * cosf(angle->wt - (PI / 2));
		}
	}
	// DQ0 to ALBE0 transform
	else
	{
		alBe0->zero = dq0->zero;
		if (parkType == PARK_COSINE)
		{
			alBe0->alpha = dq0->d * cosf(angle->wt) - dq0->q *  sinf(angle->wt);
			alBe0->beta = dq0->d *  sinf(angle->wt) + dq0->q * cosf(angle->wt);
		}
		else
		{
			alBe0->alpha = dq0->d * cosf(angle->wt - (PI / 2)) - dq0->q * sinf(angle->wt - (PI / 2));
			alBe0->beta = dq0->d * sinf(angle->wt - (PI / 2)) + dq0->q * cosf(angle->wt - (PI / 2));
		}
	}
}

/**
 * @brief Performs the Park and inverse Park transformations with wt = 0
 *
 * @param *alphaBeta0 Pointer to the alpha Beta zero coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param src conversion source. If src = SRC_ALBE0 converts from ALBE0 to DQ0 else converts from DQ0 to ALBE0 coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 *
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/alphabetazerotodq0dq0toalphabetazero.html?s_tid=doc_ta
 */
static inline void Transform_alphaBeta0_dq0_wt0(LIB_3COOR_ALBE0_t* alBe0, LIB_3COOR_DQ0_t* dq0,
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
 *
 * @param *abc Pointer to the abc coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param *angle Pointer to the angle structure, Make sure that the Sines and Cosines are precomputed before calling this method
 * @param src conversion source. If src = SRC_ABC converts from ABC to DQ0 else converts from DQ0 to ABC coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 *
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/abctodq0dq0toabc.html?s_tid=doc_ta
 */
static inline void Transform_abc_dq0(LIB_3COOR_ABC_t* abc, LIB_3COOR_DQ0_t* dq0, LIB_3COOR_SINCOS_t* angle,
		transformation_source_t src, park_transform_type_t parkType)
{
	// ABC to DQO transform
	if (src == SRC_ABC)
	{
		if (parkType == PARK_COSINE)
		{
			dq0->d = 2/3.f * (abc->a * cosf(angle->wt) + abc->b * cosf((angle->wt - TWO_PI/3)) + abc->c * cosf((angle->wt + TWO_PI/3)));
			dq0->q = -2/3.f * (abc->a * sinf(angle->wt) + abc->b * sinf((angle->wt - TWO_PI/3)) + abc->c * sinf((angle->wt + TWO_PI/3)));
			dq0->zero = (abc->a + abc->b + abc->c) / 3.f;
		}
		else
		{
			dq0->d = 2/3.f * (abc->a * sinf(angle->wt) + abc->b * sinf((angle->wt - TWO_PI/3)) + abc->c * sinf((angle->wt + TWO_PI/3)));
			dq0->q = 2/3.f * (abc->a * cosf(angle->wt) + abc->b * cosf((angle->wt - TWO_PI/3)) + abc->c * cosf((angle->wt + TWO_PI/3)));
			dq0->zero = (abc->a + abc->b + abc->c) / 3.f;
		}
	}
	// DQ0 to ABC transform
	else
	{
		if (parkType == PARK_COSINE)
		{
			abc->a = dq0->d * cosf(angle->wt) - dq0->q * sin(angle->wt) + dq0->zero;
			abc->b = dq0->d * cosf(angle->wt - (TWO_PI/3)) - dq0->q * sinf(angle->wt - (TWO_PI/3)) + dq0->zero;
			abc->c = dq0->d * cosf(angle->wt + (TWO_PI/3)) - dq0->q * sinf(angle->wt + (TWO_PI/3)) + dq0->zero;
		}
		else
		{
			abc->a = dq0->d * sinf(angle->wt) + dq0->q * cosf(angle->wt) + dq0->zero;
			abc->b = dq0->d * sinf(angle->wt - (TWO_PI/3)) + dq0->q * cosf(angle->wt - (TWO_PI/3)) + dq0->zero;
			abc->c = dq0->d * sinf(angle->wt + (TWO_PI/3)) + dq0->q * cosf(angle->wt + (TWO_PI/3)) + dq0->zero;
		}
	}
}

/**
 * @brief Performs the transforms between ABC and DQ0 coordinates  with wt = 0
 *
 * @param *abc Pointer to the abc coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param src conversion source. If src = SRC_ABC converts from ABC to DQ0 else converts from DQ0 to ABC coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 *
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/abctodq0dq0toabc.html?s_tid=doc_ta
 */
static inline void Transform_abc_dq0_wt0(LIB_3COOR_ABC_t* abc, LIB_3COOR_DQ0_t* dq0,
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
 * @brief Transform theta to the trigonometric values
 *
 * @param *angle Pointer to the angle information
 */
static inline void Transform_theta_sincos(LIB_3COOR_SINCOS_t *angle)
{
	//angle->sin = sinf(angle->wt);
	//angle->cos = cosf(angle->wt);
}

#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
