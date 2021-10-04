/*! 
@file Converter.h
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
 */
#ifndef CONVERTER_H
#define CONVERTER_H
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdint.h"
#include "math.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define PI    										(3.141593f)
#define TWO_PI 										(2 * PI)
#define PI_3											(PI / 3)

typedef struct
{
	float fA;
	float fB;
	float fC;
} LIB_3COOR_ABC_t;

typedef struct
{
	float fAlfa;
	float fBeta;
} LIB_2COOR_ALBE_t;

typedef struct
{
	float fd;
	float fq;
} LIB_2COOR_DQ_t;

typedef struct
{
	float fTheta;
	float fsin;
	float fcos;
} LIB_3COOR_SINCOS_t;

typedef struct
{
	float fd;
	float fq;
	float zero;
} LIB_3COOR_DQ0_t;
/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Clark conversion
 * @details abc to alfa beta conversion
 * @param *psIn fA, fB, fC
 * @param *psOut fAlfa, fBeta
 */
static inline void convert_abc2alfabeta(LIB_3COOR_ABC_t *psIn,
		LIB_2COOR_ALBE_t *psOut)
{

	psOut->fAlfa = psIn->fA;
	psOut->fBeta = (psIn->fB - psIn->fC ) / sqrtf(3.f); 

	//	psOut->fAlfa = (psIn->fA / 1.5f) - (psIn->fB + psIn->fC) / 3;
	//	psOut->fBeta = (1 / sqrtf(3.f)) * (psIn->fB - psIn->fC );

	//	psOut->fBeta = psIn->fA;
	//	psOut->fAlfa = (psIn->fB - psIn->fC ) / sqrtf(3.f);

	return;
}

/*!
 * @brief InvClark conversion
 * @details alfa, beta to A, B, C
 * @param *psIn fAlfa, fBeta
 * @param *psOut fA, fB, fC
 */
static inline void convert_alfabeta2abc(LIB_2COOR_ALBE_t *psIn,
		LIB_3COOR_ABC_t *psOut)
{
	psOut->fA = psIn->fAlfa;
	psOut->fB = ((sqrtf(3.f)/2.f)*psIn->fBeta)-(psIn->fAlfa/2.f); 
	psOut->fC = -(psOut->fA + psOut->fB );
}

/*!
 * @brief Park conversion
 * @details alfa, beta to A, B, C
 * @param *psIn fAlfa, fBeta and fsin, fcos
 * @param *psOut fd, fq
 */
static inline void convert_alfabeta2dq(LIB_2COOR_ALBE_t *psIn,
		LIB_3COOR_SINCOS_t *psInAngle,
		LIB_2COOR_DQ_t *psOut)
{
	psOut->fd = psIn->fAlfa * psInAngle->fcos + psIn->fBeta * psInAngle->fsin;
	psOut->fq = psIn->fBeta * psInAngle->fcos - psIn->fAlfa * psInAngle->fsin;
}

/*!
 * @brief InvPark conversion
 * @details dq to alfabeta
 * @param *psIn fd, fq and fsin, fcos
 * @param *psOut fAlfa, fBeta
 */
static inline void convert_dq2alfabeta(LIB_2COOR_DQ_t *psIn,
		LIB_3COOR_SINCOS_t *psInAngle,
		LIB_2COOR_ALBE_t *psOut)
{
	psOut->fAlfa = psIn->fd * psInAngle->fcos - psIn->fq * psInAngle->fsin;
	psOut->fBeta = psIn->fd * psInAngle->fsin + psIn->fq * psInAngle->fcos;
}



/*!
 * @brief Theta to sincos conversion
 * @details 
 * @param *psIn Theta
 * @param *psOut fsin, fcos
 */
static inline void convert_wt2sincos(LIB_3COOR_SINCOS_t *psOut)
{
	psOut->fsin = sinf(psOut->fTheta);
	psOut->fcos = cosf(psOut->fTheta);
}

static inline void abc2dq0_90DegreeBehind(LIB_3COOR_ABC_t *abc, LIB_3COOR_DQ0_t* dq0, float theta)
{
	dq0->fd = 2/3.f * (abc->fA * sinf(theta) + abc->fB * sinf((theta - TWO_PI/3)) + abc->fC * sinf((theta + TWO_PI/3)));
	dq0->fq = 2/3.f * (abc->fA * cosf(theta) + abc->fB * cosf((theta - TWO_PI/3)) + abc->fC * cosf((theta + TWO_PI/3)));
	dq0->zero = 1/3.f * (abc->fA + abc->fB + abc->fC );
}

static inline void abc2dq0_aligned(LIB_3COOR_ABC_t *abc, LIB_3COOR_DQ0_t* dq0, float theta)
{
	dq0->fd = 2/3.f * (abc->fA * cosf(theta) + abc->fB * cosf((theta - TWO_PI/3)) + abc->fC * cosf((theta + TWO_PI/3)));
	dq0->fq = -2/3.f * (abc->fA * sinf(theta) + abc->fB * sinf((theta - TWO_PI/3)) + abc->fC * sinf((theta + TWO_PI/3)));
	dq0->zero = 1/3.f * (abc->fA + abc->fB + abc->fC );
}

static inline void dq02alphaBeta_aligned(LIB_3COOR_DQ0_t *dq0,
		LIB_3COOR_SINCOS_t *sinCos,
		LIB_2COOR_ALBE_t *alphaBeta)
{
	alphaBeta->fAlfa = dq0->fd * sinCos->fcos - dq0->fq * sinCos->fsin;
	alphaBeta->fBeta = dq0->fd * sinCos->fsin + dq0->fq * sinCos->fcos;
}

static inline void dq02alphaBeta_90DegreeBehind(LIB_3COOR_DQ0_t *dq0,
		LIB_3COOR_SINCOS_t *sinCos,
		LIB_2COOR_ALBE_t *alphaBeta)
{

}

static inline void dq02abc_90DegreeBehind(LIB_3COOR_ABC_t *abc, LIB_3COOR_DQ0_t* dq0, float theta)
{
	abc->fA = dq0->fd * sinf(theta) + dq0->fq * cosf(theta) + dq0->zero;
	abc->fB = dq0->fd * sinf(theta - (2 * PI_3)) + dq0->fq * cosf(theta - (2 * PI_3)) + dq0->zero;
	abc->fC = dq0->fd * sinf(theta + (2 * PI_3)) + dq0->fq * cosf(theta + (2 * PI_3)) + dq0->zero;
}

static inline void dq02abc_aligned(LIB_3COOR_ABC_t *abc, LIB_3COOR_DQ0_t* dq0, float theta)
{
	abc->fA = dq0->fd * cosf(theta) - dq0->fq * sin(theta) + dq0->zero;
	abc->fB = dq0->fd * cosf(theta - (2 * PI_3)) - dq0->fq * sinf(theta - (2 * PI_3)) + dq0->zero;
	abc->fC = dq0->fd * cosf(theta + (2 * PI_3)) - dq0->fq * sinf(theta + (2 * PI_3)) + dq0->zero;
}

static void GetSVPWM(LIB_2COOR_ALBE_t *alphaBeta, float* duties)
{
	float alpha = atan2f(alphaBeta->fBeta, alphaBeta->fAlfa);
	if(alpha < 0)
		alpha += TWO_PI;
	float m = sqrtf((alphaBeta->fBeta * alphaBeta->fBeta) + (alphaBeta->fAlfa * alphaBeta->fAlfa));
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
	default:
		duties[0] = T1 + T2 + T0/2;
		duties[1] = T0/2;
		duties[2] = 1-(T2 + T0/2);
		break;
	}
}

/**
 * @brief Svpwm generation From Vref
 * @param alphaBeta Alpha Beta Values
 * @param dc value of the dc-link
 * @param *duties duty cycle values to be exported to the PWM channels
 */
static void GetSVPWM_FromVref(LIB_2COOR_ALBE_t* alphaBeta, float dc, float* duties)
{
	// Ur1, Ur2, Ur3 calculation
	float max = dc/sqrtf(3);
	float amp = sqrtf(alphaBeta->fAlfa * alphaBeta->fAlfa + alphaBeta->fBeta * alphaBeta->fBeta);

	alphaBeta->fAlfa =  alphaBeta->fAlfa / max;
	alphaBeta->fBeta =  alphaBeta->fBeta / max;

	LIB_3COOR_ABC_t abc;
	abc.fA = alphaBeta->fBeta;
	abc.fB = -alphaBeta->fBeta/2 + alphaBeta->fAlfa * sqrtf(3)/2;
	abc.fC = -alphaBeta->fBeta/2 - alphaBeta->fAlfa * sqrtf(3)/2;

	// Voltage vector section determination
	// Ur1 = Ubeta_pr;
	// Ur2 = -Ubeta_pr/2 + Ualpha_pr* sqrt(3)/2;
	// Ur3 = -Ubeta_pr/2 - Ualpha_pr* sqrt(3)/2;
	int Section = abc.fA >= 0 ? (abc.fB >= 0 ? 1 : (abc.fC >= 0 ? 3 : 2)) : (abc.fB >= 0 ? (abc.fC >= 0 ? 5 : 6) : 4);

	float T1, T2;
	float DCA, DCB, DCC;

	// set timings
	switch (Section)
	{
	case 1:
		T1 = abc.fB;
		T2 = abc.fA;
		DCA = (1-T1-T2)/2;
		DCB = DCA + T1;
		DCC = DCB + T2;

	case 2:
		T1 = -abc.fB;
		T2 = -abc.fC;
		DCB = (1-T1-T2)/2;
		DCA = DCB + T1;
		DCC = DCA + T2;

	case 3:
		T1 = abc.fA;
		T2 = abc.fC;
		DCB = (1-T1-T2)/2;
		DCC = DCB + T1;
		DCA = DCC + T2;

	case 4:
		T1 = -abc.fA;
		T2 = -abc.fB;
		DCC = (1-T1-T2)/2;
		DCB = DCC + T1;
		DCA = DCB + T2;

	case 5:
		T1 = abc.fC;
		T2 = abc.fB;
		DCC = (1-T1-T2)/2;
		DCA = DCC + T1;
		DCB = DCA + T2;

	case 6:
		T1 = -abc.fC;
		T2 = -abc.fA;
		DCA = (1-T1-T2)/2;
		DCC = DCA + T1;
		DCB = DCC + T2;

	default:
		DCA = 0.5f;
		DCB = 0.5f;
		DCC = 0.5f;
	}


	duties[0] = 1-DCA;
	duties[1] = 1-DCB;
	duties[2] = 1-DCC;
}

#endif
/* EOF */
