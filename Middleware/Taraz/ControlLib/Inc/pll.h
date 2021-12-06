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

/** @addtogroup Control_Library
 * @{
 */

/** @defgroup PLL	Phase Locked Loop
 * @brief Contains the declarations and procedures to implement all Phase Locked Loop
 * @{
 */

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "general_header.h"
#include "dsp_library.h"
#include "transforms.h"
/********************************************************************************
 * Defines
 *******************************************************************************/
//#define FILTER_DEFAULT_SIZE				(8)
//#define FILTER_MAX_SIZE					(32)
//#define FILTER_COUNT					(10)
//#define ACCEPTABLE_Q					(20.f)
//#define MAX_Q_DATA_INDEX				(25000)

//#define MONITOR_PLL						(0)
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/**
 * @brief PLL state definitions
 */
typedef enum
{
	PLL_INVALID,/**< Grid phase not detected */
	PLL_PENDING,/**< Grid phase detected but the conditions for lock not yet achieved */
	PLL_LOCKED  /**< Grid phase detection completed */
} pll_states_t;
/********************************************************************************
 * Structures
 *******************************************************************************/
typedef struct
{
#if	MONITOR_PLL
	float dMin;
	float qMax;
#endif
	float tempQMax;
	int index;
	float tempDMin;
} pll_info_t;

/**
 * @brief Defines the parameters required by the PLL
 */
typedef struct
{
	LIB_COOR_ALL_t* coords;			/**< @brief Grid voltage coordinates in different coordinate systems */
	mov_avg_t dFilt;				/**< @brief Filter for the D coordinate of DQ0 coordinates */
	mov_avg_t qFilt;				/**< @brief Filter for the Q coordinate of DQ0 coordinates */
	pi_compensator_t compensator;	/**< @brief PI compensator for Q adjustment */
	pll_info_t info;				/**< @brief PLL info internaly used by the system */
	pll_states_t status;			/**< @brief Current status of PLL */
	pll_states_t prevStatus;		/**< @brief Previous cycle status of PLL */
	float qLockMax;					/**< @brief Maximum value of Q. If in a cycle defined by @ref cycleCount the value
									remains less than this value the PLL will be considered locked */
	float dLockMin;					/**< @brief Minimum value of D. If in a cycle defined by @ref cycleCount the value
									remains greater only than the PLL locking will be enabled */
	int cycleCount;					/**< @brief If the PLL remains lock for this many control loops than it will be considered locked */
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
