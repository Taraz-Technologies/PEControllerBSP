/*! 
@file Control.h
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
#ifndef CONTROL_H
#define CONTROL_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "GeneralHeader.h"
#include "Converter.h"
#include "Filters.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/
typedef struct
{
	float Kp; float Ki;
	float dt;
	float Integral;
} pi_data_t;
 /*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief lock the volatge Phase to the grid
 * @param grid voltages
 * @param voltage peak measurement
 */
LIB_3COOR_SINCOS_t* LockVoltagePhase(LIB_3COOR_ABC_t* sVabc, LIB_3COOR_DQ0_t* sVdq0);
LIB_3COOR_SINCOS_t* SetCurrent(LIB_3COOR_ABC_t* sIabc, LIB_3COOR_SINCOS_t* sSincos, float iRef, float* duties);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief adjust theta so that it remains in the range 0-2pi */
static float AdjustTheta(float theta)
{
	// adjust theta for the overflow
	if(theta > TWO_PI)
		theta -= (TWO_PI);
	
	if(theta < 0)
		theta += (TWO_PI);
	
	return theta;
}

/*! @brief add a manual theta shift and readjust theta to bounded region (0-2pi) */
static float ShiftTheta(float theta, float shift)
{
	theta += shift;
	return AdjustTheta(theta);
}

/*! @brief Generate the SPWM signal */
static void GenerateSPWM(float theta, float modulationIndex, float* duties)
{
	// get the equivalent duty cycle
	float resThetas[3] = { theta , theta + (TWO_PI/3), theta - (TWO_PI/3)};
	for (int i = 0; i < 3; i++)
	{
		theta = AdjustTheta(resThetas[i]);
		duties[i] = ((sinf(theta) * modulationIndex) * 0.5f) + 0.5f;
	}
}

/*! @brief evaluate the pi for the error */
static float EvaluatePI(pi_data_t* data, float err)
{
	data->Integral += (err * data->dt) ;
	return (data->Kp * err) + (data->Ki * data->Integral);
}

#endif
/* EOF */
