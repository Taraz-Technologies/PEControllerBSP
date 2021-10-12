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
#include "Transformations.h"
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

/*
static void GetSVPWM(LIB_2COOR_ALBE_t *alphaBeta, float* duties)
{
	float alpha = atan2f(alphaBeta->beta, alphaBeta->alpha);
	if(alpha < 0)
		alpha += TWO_PI;
	float m = sqrtf((alphaBeta->beta * alphaBeta->beta) + (alphaBeta->alpha * alphaBeta->alpha));
	int sector = (int)(alpha / PI_3) + 1;
	if(sector > 6)
		sector = 6;
	float T1 = sqrtf(3) * m * sinf((sector * PI_3) - alpha);
	float T2 = sqrtf(3) * m * sinf(alpha - ((sector - 1) * PI_3));
	float T0 = 1 - T1 - T2;

	switch(sector)
	{
	case 1:
		duties[0] = T1 + T2 + T0/2;
		duties[1] = T2 + T0/2;
		duties[2] = T0/2;
		break;
	case 2:
		duties[0] = 1 - (T2 + T0/2);
		duties[1] = T1 + T2 + T0/2;
		duties[2] = T0/2;
		break;
	case 3:
		duties[0] = T0/2;
		duties[1] = T1 + T2 + T0/2;
		duties[2] = T2 + T0/2;
		break;
	case 4:
		duties[0] = T0/2;
		duties[2] = T1 + T2 + T0/2;
		duties[1] = 1-(T2 + T0/2);
		break;
	case 5:
		duties[0] = T2 + T0/2;
		duties[1] = T0/2;
		duties[2] = T1 + T2 + T0/2;
		break;
	deault:
		duties[0] = T1 + T2 + T0/2;
		duties[1] = T0/2;
		duties[2] = 1-(T2 + T0/2);
		break;
	}
}
*/

/**
 * @brief Generate the SVPWM based on the DC-Link
 *
 * @param *alBe0 Pointer to the alpha beta zero coordinates structure
 * @param dc value of the dc-link
 * @param *duties duty cycle values to be exported to the PWM channels (Range 0 - 1)
 */
static void GetSVPWM_FromVref(LIB_3COOR_ALBE0_t* alBe0, float dc, float* duties)
{
	// Ur1, Ur2, Ur3 calculation
	float max = dc / sqrtf(3);
	float amp = sqrtf(alBe0->alpha * alBe0->alpha + alBe0->beta * alBe0->beta);

	alBe0->alpha =  alBe0->alpha / max;
	alBe0->beta =  alBe0->beta / max;

	LIB_3COOR_ABC_t abc;
	abc.a = alBe0->beta;
	abc.b = -alBe0->beta/2 + alBe0->alpha * sqrtf(3)/2;
	abc.c = -alBe0->beta/2 - alBe0->alpha * sqrtf(3)/2;

	// Voltage vector section determination
	// Ur1 = Ubeta_pr;
	// Ur2 = -Ubeta_pr/2 + Ualpha_pr* sqrt(3)/2;
	// Ur3 = -Ubeta_pr/2 - Ualpha_pr* sqrt(3)/2;
	int Section = abc.a >= 0 ? (abc.b >= 0 ? 1 : (abc.c >= 0 ? 3 : 2)) : (abc.b >= 0 ? (abc.c >= 0 ? 5 : 6) : 4);

	float T1, T2;
	float DCA, DCB, DCC;

	// set timings
	switch (Section)
	{
	case 1:
		T1 = abc.b;
		T2 = abc.a;
		DCA = (1-T1-T2)/2;
		DCB = DCA + T1;
		DCC = DCB + T2;
		break;
	case 2:
		T1 = -abc.b;
		T2 = -abc.c;
		DCB = (1-T1-T2)/2;
		DCA = DCB + T1;
		DCC = DCA + T2;
		break;
	case 3:
		T1 = abc.a;
		T2 = abc.c;
		DCB = (1-T1-T2)/2;
		DCC = DCB + T1;
		DCA = DCC + T2;
		break;
	case 4:
		T1 = -abc.a;
		T2 = -abc.b;
		DCC = (1-T1-T2)/2;
		DCB = DCC + T1;
		DCA = DCB + T2;
		break;
	case 5:
		T1 = abc.c;
		T2 = abc.b;
		DCC = (1-T1-T2)/2;
		DCA = DCC + T1;
		DCB = DCA + T2;
		break;
	case 6:
		T1 = -abc.c;
		T2 = -abc.a;
		DCA = (1-T1-T2)/2;
		DCC = DCA + T1;
		DCB = DCC + T2;
		break;
	deault:
		DCA = 0.5f;
		DCB = 0.5f;
		DCC = 0.5f;
	}

	duties[0] = 1 - DCA;
	duties[1] = 1 - DCB;
	duties[2] = 1 - DCC;
}

#endif
/* EOF */
