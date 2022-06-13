/**
 ********************************************************************************
 * @file 		pll.h
 * @author 		Waqas Ehsan Butt
 * @date 		Dec 6, 2021
 *
 * @brief
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

#ifndef INC_PLL_H_
#define INC_PLL_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Control_Library
 * @{
 */

/** @defgroup PLL	Phase Locked Loop
 * @brief Contains the declarations and procedures to implement Phase Locked Loop
 * @details
 * The first step for grid phase detection is to assign suitable filters and
 * compensator values and phase detection error settings for the PLL structure.
 * Once configured the PLL should be run in every cycle to detect the voltage phase.
 * Below programming example further describes the module usage
 *
 * ==============================================================================
 *                Grid Voltage Phase Detection
 * ==============================================================================
 * @code
	pll_lock_t pll = {0};

	// Initializes the PLL module
	void Init (void)
	{
		static float vDFiltData[PLL_FILT_SIZE] = {0};
		static float vQFiltData[PLL_FILT_SIZE] = {0};
		pll.coords = &gridTie.vCoor;
		pll.dFilt.dataPtr = vDFiltData;
		pll.qFilt.dataPtr = vQFiltData;
		pll.dFilt.count = PLL_FILT_SIZE;
		pll.qFilt.count = PLL_FILT_SIZE;
		pll.compensator.Kp = .001f;
		pll.compensator.Ki = .8f;
		pll.compensator.dt = PWM_PERIOD_s;
		pll.qLockMax = 10;
		pll.dLockMin = gridTie.pll.qLockMax * 10;
		pll.cycleCount = (int)((1 / PWM_PERIOD_s) * 10);
	}

	// Poll the PLL module on every cycle and connect to the output if PLL is locked else disconnect
	void Poll(void)
	{
		if (Pll_LockGrid(&pll) == PLL_LOCKED)
			GenerateOutput();
		else
			DisconnectOutput();
	}
 @endcode
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
/** @defgroup PLL_Exported_Macros Macros
  * @{
  */
/**
 * @brief Set this to 1 if PLL monitoring is required
 */
#define MONITOR_PLL						(0)
/**
 * @}
 */
/********************************************************************************
 * Typedefs
 *******************************************************************************/
/** @defgroup PLL_Exported_Typedefs Type Definitions
  * @{
  */
/**
 * @brief PLL state definitions
 */
typedef enum
{
	PLL_INVALID,/**< Grid phase not detected */
	PLL_PENDING,/**< Grid phase detected but the conditions for lock not yet achieved */
	PLL_LOCKED  /**< Grid phase detection completed */
} pll_states_t;
/**
 * @}
 */
/********************************************************************************
 * Structures
 *******************************************************************************/
/** @defgroup PLL_Exported_Structures Structures
  * @{
  */
/**
 * @brief Defines the internal parameters used buy the PLL module
 */
typedef struct
{
#if	MONITOR_PLL
	float dMin;						/**< @brief Minimum D in the previous cycle */
	float qMax;						/**< @brief Maximum q in the previous cycle */
#endif
	float tempQMax;					/**< @brief Temporary variable for evaluating cycle maximum for Q */
	float tempDMin;					/**< @brief Temporary variable for evaluating cycle minimum for D */
	float tempDMax;					/**< @brief Temporary variable for evaluating cycle maximum for D */
	int index;						/**< @brief Current index of the cycle */
} pll_info_t;

/**
 * @brief Defines the parameters required by the PLL
 */
typedef struct
{
	LIB_COOR_ALL_t* coords;			/**< @brief Grid voltage coordinates in different coordinate systems */
	pi_compensator_t compensator;	/**< @brief PI compensator for Q adjustment */
	pll_info_t info;				/**< @brief PLL info internaly used by the system */
	pll_states_t status;			/**< @brief Current status of PLL */
	pll_states_t prevStatus;		/**< @brief Previous cycle status of PLL */
	float qLockMax;					/**< @brief Maximum value of Q. If in a cycle defined by @ref cycleCount the value
									remains less than this value the PLL will be considered locked */
	float dLockMin;					/**< @brief Minimum value of D. If in a cycle defined by @ref cycleCount the value
									remains greater only than the PLL locking will be enabled */
	float dLockMax;					/**< @brief Maximum value of D. If in a cycle defined by @ref cycleCount the value
									remains smaller only than the PLL locking will be enabled */
	int cycleCount;					/**< @brief If the PLL remains lock for this many control loops than it will be considered locked */
	float expectedGridFreq;			/**< @brief Expected grid frequency  */
} pll_lock_t;
/**
 * @}
 */
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/** @defgroup PLL_Exported_Functions Functions
  * @{
  */
/**
 * @brief Initialize the PLL structure.
 * @param *pll Structure to be initialized.
 */
extern void PLL_Init(pll_lock_t* pll);
/**
 * @brief Lock the grid voltages using PLL
 *
 * @param pll Pointer to the data structure
 * @return pll_states_t PLL_LOCKED if grid phase successfully locked
 */
extern pll_states_t Pll_LockGrid(pll_lock_t* pll);
/********************************************************************************
 * Code
 *******************************************************************************/

/**
 * @}
 */
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
