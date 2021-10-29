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
#define MONITOR_PI			(1)
/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/
typedef struct
{
	bool has_lmt;
	float Kp; float Ki;
	float dt;
	float Integral;
	float max;
	float min;
#if MONITOR_PI
	float err;
	float result;
#endif
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
	while (theta > TWO_PI)
		theta -= (TWO_PI);

	while (theta < 0)
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
#if MONITOR_PI
	data->err = err;
#endif
	data->Integral += (err * data->dt) ;
	if (data->has_lmt)
	{
		if(data->Integral > data->max)
			data->Integral = data->max;
		if(data->Integral < data->min)
			data->Integral = data->min;
	}
#if MONITOR_PI
	data->result = (data->Kp * err) + (data->Ki * data->Integral);
	return data->result;
#else
	return (data->Kp * err) + (data->Ki * data->Integral);
#endif
}

static float GetRef(LIB_3COOR_ALBE0_t *alBe0)
{
	float a = fabs(alBe0->alpha);
	float y = fabs(alBe0->beta);
	if (a < y)
	{
		a /= y;
		y *= sqrtf(a * a + 1.0);
	}
	else if (a > y)
	{
		y /= a;
		y = sqrtf(y * y + 1.0) * a;
	}
	else
		y = a * 1.4142135623730951;

	return y;
}

static void GetSVPWM(LIB_3COOR_ALBE0_t *alBe0, float* duties)
{
	float m = GetRef(alBe0);

	/* Saturate: '<S3>/0<m<1 ' */
	if (m > 1.0)
		m = 1.0;
	else if (m < 0.0)
		m = 0.0;

	float angle = atan2f(alBe0->beta, alBe0->alpha);
	angle = AdjustTheta(angle);
}

static void GetSVPWM_FromDQ(LIB_3COOR_DQ0_t *dq0, float* duties)
{
	// get the angle for sector computation
	float ang = atan2f(dq0->q , dq0->d);
	ang = AdjustTheta(ang);

	// get relevant sector
	int sector = (int)(ang * 6 / TWO_PI);
	if (sector > 5)
		sector = 5;

	float vref1 = dq0->q;
	float vref2 = dq0->d;

}


static void GetSVPWM_FromAlBe(LIB_3COOR_ALBE0_t *alBe0, float* duties)
{
	// hypotd (Get the value of m)
	float a = fabsf(alBe0->alpha);
	float m = fabsf(alBe0->beta);

	if (a < m)
	{
		a  = a / m;
		m = sqrtf(a * a + 1) * m;
	}
	else if (a > m)
	{
		m = m / a;
		m = sqrtf(m * m + 1) * a;
	}

	if (m < 0)
		m = 0;
	else if (m > 1)
		m = 1;

	// get the angle 0-2pi
	float wt = 0;
	if (alBe0->alpha == 0)
	{
		if (alBe0->beta > 0)
			wt = PI/2;
		else if (alBe0->beta < 0)
			wt = -PI/2;
		else
			wt = 0;
	}
	else
		wt = atan2f(alBe0->beta, alBe0->alpha);
	wt = AdjustTheta(wt);

	// get sector (1 - 6)
	int n = (int)(wt * 3/PI) + 1;

	// get wtn for the sector 0-pi/6
	float wtn = wt - (n - 1) * PI/3;

	// get SVPWM timing
	float p1 = m * cosf(wtn + PI/6);
	float p2 = m * sinf(wtn);
	float p0 = (1 - p2 - p1)/2;

	// set duty cycles
	duties[0] = p0;
	duties[1] = p0;
	duties[2] = p0;

	float tAll = p0 + p1 + p2;
	float t1 = p0 + p1;
	float t2 = p0 + p2;

	if (n == 1)
	{
		duties[0] = tAll;
		duties[1] = t2;
	}
	else if (n == 2)
	{
		duties[0] = t1;
		duties[1] = tAll;
	}
	else if (n == 3)
	{
		duties[1] = tAll;
		duties[2] = t2;
	}
	else if (n == 4)
	{
		duties[1] = t1;
		duties[2] = tAll;
	}
	else if (n == 5)
	{
		duties[2] = tAll;
		duties[0] = t2;
	}
	else if (n == 6)
	{
		duties[2] = t1;
		duties[0] = tAll;
	}
	else
	{
		duties[0] = 0;
	}
}


/**
 * @brief Generate the SVPWM based on the DC-Link
 *
 * @param *dq0 Pointer to the DQ0 coordinates structure
 * @param dc value of the dc-link
 * @param *duties duty cycle values to be exported to the PWM channels (Range 0 - 1)
 */
static void GetSVPWM_FromVrefDQ(LIB_3COOR_DQ0_t* dq0, float dc, float* duties)
{
	float m = sqrtf(3 * (dq0->q * dq0->q + dq0->d * dq0->d)) / (dc * 5.f);
	float wt = atan2f(dq0->q, dq0->d);
	wt = AdjustTheta(wt);

	int n = (wt * 6 / TWO_PI) + 1;
	if (n > 6)
		n = 6;

	//wt -= ((n - 1) * (TWO_PI / 6));   // below formula looks to be working for 0 <= wt <= 360

	float T1 = m * sinf (n * PI_3 - wt);
	float T2 = m * sinf (wt - (n - 1) * PI_3);
	float T0 = 1 - T1 - T2;
	if (T0 > 1 || T0 < 0)
		Error_Handler();

	switch(n)
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
	default:
		duties[0] = T1 + T2 + T0/2;
		duties[1] = T0/2;
		duties[2] = 1-(T2 + T0/2);
		break;
	}
}

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
	default:
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
