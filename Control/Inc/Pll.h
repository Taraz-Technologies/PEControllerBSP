/**
 ********************************************************************************
 * @file 		Pll.h
 * @author 		Waqas Ehsan Butt
 * @date 		Oct 5, 2021
 * @copyright 	TarazTechnologies Pvt. Ltd.
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
#include "Control.h"
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
/*!
 * @brief lock the volatge Phase to the grid
 * @param grid voltages
 * @param voltage peak measurement
 */
static LIB_3COOR_SINCOS_t* LockVoltagePhase(LIB_COOR_ALL_t* coords)
{
	Transform_abc_dq0_90DegreeBehind(&coords->abc, &coords->dq0, coords->sinCosAngle.theta);
	LowPassFilter_VoltageDQ(sVdq0);
	float thetaShift = EvaluatePI(&qVPI, -sVdq0->fq);
	sSincos.fTheta -= thetaShift;
	sSincos.fTheta = AdjustTheta(sSincos.fTheta);

	convert_wt2sincos(&sSincos);

	ThetaLogger(sSincos.fTheta);

	if(IsPLLSynched(sVdq0))
		return &sSincos;

	return NULL;
}

#ifdef __cplusplus
}
#endif

#endif 
/* EOF */
