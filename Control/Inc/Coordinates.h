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
#define PI    										(3.141593f)
#define TWO_PI 										(2 * PI)
#define PI_3										(PI / 3)
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	float a;
	float b;
	float c;
} LIB_3COOR_ABC_t;

typedef struct
{
	float alpha;
	float beta;
} LIB_2COOR_ALBE_t;

typedef struct
{
	float d;
	float q;
} LIB_2COOR_DQ_t;

typedef struct
{
	float theta;
	float sin;
	float cos;
} LIB_3COOR_SINCOS_t;

typedef struct
{
	float d;
	float q;
	float zero;
} LIB_3COOR_DQ0_t;

typedef struct
{
	LIB_3COOR_ABC_t abc;
	LIB_2COOR_ALBE_t alphaBeta;
	LIB_3COOR_DQ0_t dq0;
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
