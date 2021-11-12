/**
 ********************************************************************************
 * @file 		Coordinates.h
 * @author 		Waqas Ehsan Butt
 * @date 		Oct 5, 2021
 * @copyright 	TarazTechnologies Pvt. Ltd.
 *
 * @brief   
 ********************************************************************************
 */

#ifndef INC_COORDINATES_H_
#define INC_COORDINATES_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "stdint.h"
#include "math.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @brief Mathematical constant Pi */
#define PI    										(3.141593f)
/** @brief Mathematical constant Pi multiplied by 2 */
#define TWO_PI 										(2 * PI)
/** @brief Mathematical constant Pi divided by 3 */
#define PI_3										(PI / 3)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/** @brief Structure containing the ABC coordinates of the 3 phase system */
typedef struct
{
	/** @brief First Phase Value */
	float a;
	/** @brief Second Phase Value */
	float b;
	/** @brief Third Phase Value */
	float c;
} LIB_3COOR_ABC_t;

/** @brief Structure containing the Alpha Beta And Zero Coordinates of 3 phase system */
typedef struct
{
	/** @brief Value of Alpha Axis */
	float alpha;
	/** @brief Value of Beta Axis */
	float beta;
	/** @brief Value of Zero Axis */
	float zero;
} LIB_3COOR_ALBE0_t;

/** @brief Structure containing the Angle information of the system */
typedef struct
{
	/** @brief Value of Current Angle in Radians */
	float wt;
	/** @brief Sin value of the angle given by \p wt */
	float sin;
	/** @brief Cosine value of the angle given by \p wt */
	float cos;
	/** @brief Sin value of the angle given by \p wt+2pi/3 */
	float sin_p2pB3;
	/** @brief Cosine value of the angle given by \p wt+2pi/3 */
	float cos_p2pB3;
	/** @brief Sin value of the angle given by \p wt-2pi/3 */
	float sin_m2pB3;
	/** @brief Cosine value of the angle given by \p wt-2pi/3 */
	float cos_m2pB3;
} LIB_3COOR_SINCOS_t;

/** @brief Structure containing the DQ0 coordinates of the 3 phase system */
typedef struct
{
	/** @brief Value of D Axis */
	float d;
	/** @brief Value of Q Axis */
	float q;
	/** @brief Value of Zero Axis */
	float zero;
} LIB_3COOR_DQ0_t;

/** @brief Structure containing all coordinates of the 3 phase system in different coordinate systems */
typedef struct
{
	/** @brief Structure for the ABC coordinates */
	LIB_3COOR_ABC_t abc;
	/** @brief Structure for the Alpha Beta Zero coordinates */
	LIB_3COOR_ALBE0_t alBe0;
	/** @brief Structure for the DQ0 coordinates */
	LIB_3COOR_DQ0_t dq0;
	/** @brief Structure for the angle coordinates */
	LIB_3COOR_SINCOS_t sinCosAngle;
} LIB_COOR_ALL_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/

/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
