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
/**
 * @brief Clarke Transform (abc to alpha beta transform)
 *
 * @param *abc Pointer to the abc coordinates
 * @param *alphaBeta Pointer to the alpha Beta coordinates
 */
static inline void Transform_abc_alphaBeta(LIB_3COOR_ABC_t* abc, LIB_2COOR_ALBE_t* alphaBeta)
{
	alphaBeta->alpha = (2 * abc->a - abc->b - abc->c) / (3.f);
	alphaBeta->beta = (abc->b - abc->c ) / sqrtf(3.f);
}

/**
 * @brief Inverse Clarke Transform (alpha beta to abc transform)	--todo--
 *
 * @param *alphaBeta Pointer to the alpha Beta coordinates
 * @param *abc Pointer to the abc coordinates
 */
static inline void Transform_alphaBeta_abc(LIB_2COOR_ALBE_t* alphaBeta, LIB_3COOR_ABC_t* abc)
{
	abc->a = alphaBeta->alpha;
	abc->b = ((sqrtf(3.f) * alphaBeta->beta)- alphaBeta->alpha) / 2.f;
	abc->c = -(abc->a + abc->b );
}

/**
 * @brief Park Transform (alpha beta to DQ transform)
 *
 * @param *alphaBeta Pointer to the alpha Beta coordinates
 * @param *dq Pointer to the DQ coordinates
 * @param *angle Pointer to the angles
 */
static inline void Transform_alphaBeta_dq(LIB_2COOR_ALBE_t* alphaBeta, LIB_2COOR_DQ_t* dq, LIB_3COOR_SINCOS_t* angle)
{
	dq->d = alphaBeta->alpha * angle->cos + alphaBeta->beta * angle->sin;
	dq->q = alphaBeta->beta * angle->cos - alphaBeta->alpha * angle->sin;
}

/**
 * @brief Inverse Park Transform (alpha beta to dq transform)
 *
 * @param *dq Pointer to the DQ coordinates
 * @param *alphaBeta Pointer to the alpha Beta coordinates
 * @param *angle Pointer to the angles
 */
static inline void Transform_dq_alphaBeta(LIB_2COOR_DQ_t* dq, LIB_2COOR_ALBE_t* alphaBeta, LIB_3COOR_SINCOS_t* angle)
{
	alphaBeta->alpha = dq->d * angle->cos - dq->q * angle->sin;
	alphaBeta->beta = dq->d * angle->sin + dq->q * angle->cos;
}

/**
 * @brief Transform theta to the trigonometric values
 *
 * @param *angle Pointer to the angle information
 */
static inline void Transform_theta_sincos(LIB_3COOR_SINCOS_t *angle)
{
	angle->sin = sinf(angle->theta);
	angle->cos = cosf(angle->theta);
}

/**
 * @brief Transform abc to dq0 coordinates (90 degrees behind axis)
 *
 * @param *abc Pointer to the abc coordinates
 * @param *dq0 Pointer to the dq0 coordinates
 * @param theta current angle in radians
 */
static inline void Transform_abc_dq0_90DegreeBehind(LIB_3COOR_ABC_t *abc, LIB_3COOR_DQ0_t* dq0, float theta)
{
	dq0->d = 2/3.f * (abc->a * sinf(theta) + abc->b * sinf((theta - TWO_PI/3)) + abc->c * sinf((theta + TWO_PI/3)));
	dq0->q = 2/3.f * (abc->a * cosf(theta) + abc->b * cosf((theta - TWO_PI/3)) + abc->c * cosf((theta + TWO_PI/3)));
	dq0->zero = 1/3.f * (abc->a + abc->b + abc->c );
}

/**
 * @brief Transform abc to dq0 coordinates (aligned axis)
 *
 * @param *abc Pointer to the abc coordinates
 * @param *dq0 Pointer to the dq0 coordinates
 * @param theta current angle in radians
 */
static inline void Transform_abc_dq0_aligned(LIB_3COOR_ABC_t *abc, LIB_3COOR_DQ0_t* dq0, float theta)
{
	dq0->d = 2/3.f * (abc->a * cosf(theta) + abc->b * cosf((theta - TWO_PI/3)) + abc->c * cosf((theta + TWO_PI/3)));
	dq0->q = -2/3.f * (abc->a * sinf(theta) + abc->b * sinf((theta - TWO_PI/3)) + abc->c * sinf((theta + TWO_PI/3)));
	dq0->zero = 1/3.f * (abc->a + abc->b + abc->c );
}

/*
static inline void Transform_dq0_alphaBeta_90DegreeBehind(LIB_3COOR_DQ0_t *dq0,
		LIB_2COOR_ALBE_t *alphaBeta
		LIB_3COOR_SINCOS_t *angle,)
{

}
 */

/**
 * @brief Transform dq0 to alpha beta coordinates (aligned axis)
 *
 * @param *dq0 Pointer to the dq0 coordinates
 * @param *alphaBeta Pointer to the alpha beta coordinates
 * @param *angle Pointer to the angle information
 */
static inline void Transform_dq0_alphaBeta_aligned(LIB_3COOR_DQ0_t *dq0, LIB_2COOR_ALBE_t *alphaBeta, LIB_3COOR_SINCOS_t *angle)
{
	alphaBeta->alpha = dq0->d * angle->cos - dq0->q * angle->sin;
	alphaBeta->beta = dq0->d * angle->sin + dq0->q * angle->cos;
}

/**
 * @brief Transform dq0 to abc coordinates (90 degrees behind axis)
 *
 * @param *dq0 Pointer to the dq0 coordinates
 * @param *abc Pointer to the abc coordinates
 * @param *theta current angle in radians
 */
static inline void Transform_dq0_abc_90DegreeBehind(LIB_3COOR_DQ0_t* dq0, LIB_3COOR_ABC_t *abc, float theta)
{
	abc->a = dq0->d * sinf(theta) + dq0->q * cosf(theta) + dq0->zero;
	abc->b = dq0->d * sinf(theta - (2 * PI_3)) + dq0->q * cosf(theta - (2 * PI_3)) + dq0->zero;
	abc->c = dq0->d * sinf(theta + (2 * PI_3)) + dq0->q * cosf(theta + (2 * PI_3)) + dq0->zero;
}

/**
 * @brief Transform dq0 to abc coordinates (aligned axis)
 *
 * @param *dq0 Pointer to the dq0 coordinates
 * @param *abc Pointer to the abc coordinates
 * @param *theta current angle in radians
 */
static inline void Transform_dq0_abc_aligned(LIB_3COOR_DQ0_t* dq0, LIB_3COOR_ABC_t *abc, float theta)
{
	abc->a = dq0->d * cosf(theta) - dq0->q * sin(theta) + dq0->zero;
	abc->b = dq0->d * cosf(theta - (2 * PI_3)) - dq0->q * sinf(theta - (2 * PI_3)) + dq0->zero;
	abc->c = dq0->d * cosf(theta + (2 * PI_3)) - dq0->q * sinf(theta + (2 * PI_3)) + dq0->zero;
}

#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
