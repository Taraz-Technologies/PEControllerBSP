/**
 ********************************************************************************
 * @file 		Pll.h
 * @author 		Waqas Ehsan Butt
 * @date 		Oct 5, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief
 ********************************************************************************
 */

#ifndef INC_PLL_H_
#define INC_PLL_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "GeneralHeader.h"
#include "Control.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
#define FILTER_DEFAULT_SIZE				(8)
#define FILTER_MAX_SIZE					(32)
#define FILTER_COUNT					(10)
#define ACCEPTABLE_Q					(20.f)
#define MAX_Q_DATA_INDEX				(25000)

#define EVALUATE_D_STATS				(1)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
typedef enum
{
	PLL_INVALID,
	PLL_PENDING,
	PLL_LOCKED
} pll_states_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
	float* data;
	mov_avg_float_t filt;
	int size;
} low_pass_filter_t;

typedef struct
{
	float tempCycleMax;
	float acceptableMax;
	float cycleMax;
	int index;
	int maxIndex;
#if EVALUATE_D_STATS
	float dMin;
	float dMax;
	float dDiff;
	float dMid;
#endif
} pll_info_t;

typedef struct
{
	LIB_COOR_ALL_t coords;
	low_pass_filter_t dFilt;
	low_pass_filter_t qFilt;
	pi_data_t compensator;
	pll_info_t info;
	pll_states_t status;
	pll_states_t prevStatus;
} pll_lock_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief Lock the grid voltages using Pll
 *
 * @param pll Pointer to the data structure
 * @return pll_states_t PLL_LOCKED if grid phase successfully locked
 */
pll_states_t Pll_LockGrid(pll_lock_t* pll);
/********************************************************************************
 * Code
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
