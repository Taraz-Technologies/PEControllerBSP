/**
 ********************************************************************************
 * @file 		Coordinates.h
 * @author 		Waqas Ehsan Butt
 * @date 		Oct 5, 2021
 *
 * @brief	This file contains the basic definitions for different coordinate systems
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

#ifndef INC_COORDINATES_H_
#define INC_COORDINATES_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Control_Library
 * @{
 */

/** @defgroup Coordinates Coordinate System
 * @brief Contains the basic definitions for different coordinate systems
 * @{
 */
/********************************************************************************
 * Includes
 *******************************************************************************/
#include "stdint.h"
#include "math.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
/** @defgroup Coordinates_Exported_Macros Macros
  * @{
  */
/** @brief Mathematical constant Pi */
#define PI    										(3.141593f)
/** @brief Mathematical constant Pi multiplied by 2 */
#define TWO_PI 										(2 * PI)
/** @brief Mathematical constant Pi divided by 3 */
#define PI_3										(PI / 3)
/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup Coordinates_Exported_Structures Structures
  * @{
  */
/** @brief Defines the ABC coordinates of the 3 phase system */
typedef struct
{
	float a;	/**< @brief First Phase Value */
	float b;	/**< @brief Second Phase Value */
	float c;	/**< @brief Third Phase Value */
} LIB_3COOR_ABC_t;
/** @brief Defines the Alpha Beta And Zero Coordinates of 3 phase system */
typedef struct
{
	float alpha;	/**< @brief Value of Alpha Axis */
	float beta;		/**< @brief Value of Beta Axis */
	float zero;		/**< @brief Value of Zero Axis */
} LIB_3COOR_ALBE0_t;
/** @brief Defines the trigonometric information of the system */
typedef struct
{
	float wt;			/**< @brief Value of Current Angle in radians */
	float sin;			/**< @brief Sin value of the angle given by \p wt */
	float cos;			/**< @brief Cosine value of the angle given by \p wt */
	float sin_p2pB3;	/**< @brief Sin value of the angle given by \p wt+2pi/3 */
	float cos_p2pB3;	/**< @brief Cosine value of the angle given by \p wt+2pi/3 */
	float sin_m2pB3;	/**< @brief Sin value of the angle given by \p wt-2pi/3 */
	float cos_m2pB3;	/**< @brief Cosine value of the angle given by \p wt-2pi/3 */
} LIB_3COOR_TRIGNO_t;
/** @brief Defines the DQ0 coordinates of the 3 phase system */
typedef struct
{
	float d;		/**< @brief Value of D Axis */
	float q;		/**< @brief Value of Q Axis */
	float zero;		/**< @brief Value of Zero Axis */
} LIB_3COOR_DQ0_t;
/** @brief Defines all coordinates of the 3 phase system in different coordinate systems */
typedef struct
{
	LIB_3COOR_ABC_t abc;		/**< @brief Structure for the ABC coordinates */
	LIB_3COOR_ALBE0_t alBe0;	/**< @brief Structure for the Alpha Beta Zero coordinates */
	LIB_3COOR_DQ0_t dq0;		/**< @brief Structure for the DQ0 coordinates */
	LIB_3COOR_TRIGNO_t trigno;	/**< @brief Structure for the trigonometric information */
} LIB_COOR_ALL_t;
/**
 * @}
 */
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

/**
 * @}
 */

/**
 * @}
 */

#endif 
/* EOF */
