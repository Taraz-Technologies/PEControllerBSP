/**
 ********************************************************************************
 * @file 		Transformations.h
 * @author 		Waqas Ehsan Butt
 * @date 		Oct 5, 2021
 *
 * @brief	This file contains the functionality and definitions for different transformation
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

#ifndef INC_TRANSFORMATIONS_H_
#define INC_TRANSFORMATIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Control_Library
 * @{
 */

/** @defgroup Transformation Coordinate Transformations
 * @brief This module contains the functionality and definitions for different transformations
 * @details Following is the list of available transformations
 * 	-# <b>Clarke Transformation:</b> @ref Transform_abc_alBe0() with source = @ref SRC_ABC
 * 	-# <b>Inverse Clarke Transformation:</b> @ref Transform_abc_alBe0() with source = @ref SRC_ALBE0
 * 	-# <b>Park Transformation:</b> @ref Transform_alphaBeta0_dq0() with source = @ref SRC_ALBE0
 * 	-# <b>Inverse Park Transformation:</b> @ref Transform_alphaBeta0_dq0() with source = @ref SRC_DQ0
 * 	-# <b>Clarke + Park Transformation:</b> @ref Transform_abc_dq0() with source = @ref SRC_ABC
 * 	-# <b>Inverse Clarke + Inverse Park Transformation:</b> @ref Transform_abc_dq0() with source = @ref SRC_DQ0
 * 	-# <b>Theta to Trigonometric Values:</b> @ref Transform_wt_sincos()
 * 	-# <b>Theta to 0 - 2pi Range:</b> @ref Transform_Theta_0to2pi()
 * 	-# <b>Theta Shift to 0 - 2pi Range:</b> @ref ShiftTheta_0to2pi()
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "Coordinates.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/**
 * @brief Value of sin of 120 degrees
 */
#define SIN_120				(0.866f)
/**
 * @brief Value of cosine of 120 degrees
 */
#define COS_120A			(-0.5f)
/**
 * @brief Set to 1 if pre-computed trigonometric values should be used.
 * Use the function Transform_wt_sincos() to pre-compute all trigonometric values.
 * Set to 0 if trigonometric expressions should be solved in runtime.
 * This setting will slow down the conversions considerably
 */
#define USE_PRECOMPUTED_TRIG		(1)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @brief Transformation source definitions */
typedef enum
{
	SRC_ABC,	/**< @brief Select the ABC coordinates as source */
	SRC_ALBE0,	/**< @brief Select the Alpha Beta Zero coordinates as source */
	SRC_DQ0		/**< @brief Select the DQ0 coordinates as source */
} transformation_source_t;

/**
 * @brief Types of park transformation definitions
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/alphabetazerotodq0dq0toalphabetazero.html?s_tid=doc_ta
 */
typedef enum
{
	PARK_COSINE,	/**< Rotating frame aligned with A axis at t = 0.
		 	 	 	 	 This type of Park transformation is also known as the cosine-based Park transformation. */
	PARK_SINE,  	/**< Rotating frame aligned 90 degrees behind A axis.
		 	 	 	 	 This type of Park transformation is also known as the cosine-based Park transformation. */
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
/**
 * @brief Performs the Clarke and inverse Clarke transformations
 * @param *abc Pointer to the abc coordinates structure
 * @param *alBe0 Pointer to the alpha Beta zero coordinates structure
 * @param src conversion source. If src = SRC_ABC converts from ABC to ALBE0 else converts from ALBE0 to ABC coordinates
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/abctoalphabetazeroalphabetazerotoabc.html
 */
extern void Transform_abc_alBe0(LIB_3COOR_ABC_t* abc, LIB_3COOR_ALBE0_t* alBe0, transformation_source_t src);
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
extern void Transform_alphaBeta0_dq0(LIB_3COOR_ALBE0_t* alBe0, LIB_3COOR_DQ0_t* dq0, LIB_3COOR_TRIGNO_t* trigno,
		transformation_source_t src, park_transform_type_t parkType);
/**
 * @brief Performs the Park and inverse Park transformations with wt = 0
 * @param *alBe0 Pointer to the alpha Beta zero coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param src conversion source. If src = SRC_ALBE0 converts from ALBE0 to DQ0 else converts from DQ0 to ALBE0 coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/alphabetazerotodq0dq0toalphabetazero.html?s_tid=doc_ta
 */
extern void Transform_alphaBeta0_dq0_wt0(LIB_3COOR_ALBE0_t* alBe0, LIB_3COOR_DQ0_t* dq0,
		transformation_source_t src, park_transform_type_t parkType);

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
extern void Transform_abc_dq0(LIB_3COOR_ABC_t* abc, LIB_3COOR_DQ0_t* dq0, LIB_3COOR_TRIGNO_t* trigno,
		transformation_source_t src, park_transform_type_t parkType);

/**
 * @brief Performs the transforms between ABC and DQ0 coordinates  with wt = 0
 * @param *abc Pointer to the abc coordinates structure
 * @param *dq0 Pointer to the dq0 coordinates structures
 * @param src conversion source. If src = SRC_ABC converts from ABC to DQ0 else converts from DQ0 to ABC coordinates
 * @param parkType Select the park transformation types. For three phase systems set to PARK_SINE
 * @note https://www.mathworks.com/help/physmod/sps/powersys/ref/abctodq0dq0toabc.html?s_tid=doc_ta
 */
extern void Transform_abc_dq0_wt0(LIB_3COOR_ABC_t* abc, LIB_3COOR_DQ0_t* dq0,
		transformation_source_t src, park_transform_type_t parkType);

/**
 * @brief Transform wt to the trigonometric values required in DQ transforms to pre-compute before use
 * @param *trigno Pointer to the trigonometric information
 */
extern void Transform_wt_sincos(LIB_3COOR_TRIGNO_t *trigno);
/**
 * @brief Transform theta from value to range 0-2pi
 * @param theta current value of theta
 * @return float transformed value of theta
 */
extern float Transform_Theta_0to2pi(float theta);
/********************************************************************************
 * Code
 *******************************************************************************/
/**
 * @brief Gets the specified shift from current theta in radians adjusted to range 0 - 2pi
 * @param theta Current theta
 * @param shift Offset required
 * @return float Shifted and adjusted theta in radians
 */
static inline float ShiftTheta_0to2pi(float theta, float shift)
{
	theta += shift;
	return Transform_Theta_0to2pi(theta);
}

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
