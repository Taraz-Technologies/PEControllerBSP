/*! 
@file MainControl.c
@brief 
@details 

@author Waqas Ehsan Butt
@copyright Taraz Technologies Pvt. Ltd.
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "GeneralHeader.h"
#include "Inverter3Ph.h"
#include "MainControl.h"
#include "Control.h"
#include "MAX11046App.h"
#include "OpenLoopVfControl.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structs
 ******************************************************************************/

 /*******************************************************************************
 * Prototypes
 ******************************************************************************/
void Inverter3Ph_ResetSignal(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static inverter3Ph_config_t* inverterConfig1;
static inverter3Ph_config_t* inverterConfig2;
static inverter3Ph_init_config_t inverterInitConfig1 =
{
	.pins = { 1, 3, 5 },  /* should be odd as these are pairs */
	.dsblPin = 13,
	.periodInUs = 40,
	.interruptEnabled = true,			/* not catered for right now */
	.alignment = CENTER_ALIGNED,
	.deadtimeInNanosec = 1000,
	.deadtimeEnable = true,
	.resetCallback = Inverter3Ph_ResetSignal,
};
static inverter3Ph_init_config_t inverterInitConfig2 =
{
	.pins = { 7, 9, 11 },  /* should be odd as these are pairs */
	.dsblPin = 14,
	.periodInUs = 40,
	.interruptEnabled = false,			/* not catered for right now */
	.alignment = CENTER_ALIGNED,
	.deadtimeInNanosec = 1000,
	.deadtimeEnable = true,
	.resetCallback = Inverter3Ph_ResetSignal,
};
static volatile bool recompute = false;
extern HRTIM_HandleTypeDef hhrtim;
/*******************************************************************************
 * Code
 ******************************************************************************/
void MainControl_Init(void)
{
	DigitalPins_Init();
	
	inverterConfig1 = Inverter3Ph_Init(&inverterInitConfig1);
	inverterConfig2 = Inverter3Ph_Init(&inverterInitConfig2);
	
	if(inverterConfig1 == NULL || inverterConfig2 == NULL)
		Error_Handler();
	
	Dout_SetAsIOPin(15, GPIO_PIN_RESET);
	Dout_SetAsIOPin(16, GPIO_PIN_RESET);
}

void MainControl_Run(void)
{
	HAL_HRTIM_WaveformOutputStart(&hhrtim,HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2 | HRTIM_OUTPUT_TC1 | HRTIM_OUTPUT_TC2 | HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2 | HRTIM_OUTPUT_TE1 | HRTIM_OUTPUT_TE2);																	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);		
	HAL_HRTIM_WaveformCountStart_IT(&hhrtim,HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C | HRTIM_TIMERID_TIMER_D | HRTIM_TIMERID_TIMER_E);	
	
	hhrtim.Instance->sMasterRegs.MCR &= ~(HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C | HRTIM_TIMERID_TIMER_D | HRTIM_TIMERID_TIMER_E);
	htim1.Instance->CR1 &= ~(TIM_CR1_CEN);
	htim1.Instance->CNT = 0;
	
	htim1.Instance->CR1 |= (TIM_CR1_CEN);
	hhrtim.Instance->sMasterRegs.MCR |= (HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C | HRTIM_TIMERID_TIMER_D | HRTIM_TIMERID_TIMER_E);
}

void MainControl_Stop(void)
{
	HAL_HRTIM_WaveformOutputStop(&hhrtim,HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2 | HRTIM_OUTPUT_TC1 | HRTIM_OUTPUT_TC2 | HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2 | HRTIM_OUTPUT_TE1 | HRTIM_OUTPUT_TE2);
	HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
	HAL_HRTIM_WaveformCountStop_IT(&hhrtim,HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C | HRTIM_TIMERID_TIMER_D | HRTIM_TIMERID_TIMER_E);
}

void Inverter3Ph_ResetSignal(void)
{
	recompute = true;
}

/*! @brief callback for when one cycle is completed on the PWM driver */
void HAL_HRTIM_CounterResetCallback(HRTIM_HandleTypeDef * hhrtim,
                                              uint32_t TimerIdx)
{
	recompute = true;
}

/*! @brief Fill the Voltage ABC Units */
static void FillGridVoltages(LIB_3COOR_ABC_t* sVabc)
{
	sVabc->fA = adcVals.V1; 
	sVabc->fB = adcVals.V2;
	sVabc->fC = adcVals.V3;
}

/*! @brief fill the grid current values */
static void FillGridCurrents(LIB_3COOR_ABC_t* sIabc)
{
//	sIabc->fA = adcVals.Ih1; 
//  sIabc->fB = adcVals.Ih2;
//	sIabc->fC = adcVals.Ih3;
	
	sIabc->fA = adcVals.V1; 
	sIabc->fB = adcVals.V2;
	sIabc->fC = adcVals.V3;
}

void MainControl_Loop(void)
{
	if(recompute)
	{
#if 1
		float duties[3];
		static openloopvf_config_t vfConfig = {
				.pwmFreq = 25000, .acceleration = 1.00001f, .nominalFreq = 50, .nominalModulationIndex = 0.7f, .freq = 1.0f, .theta = 0, .reqFreq = 25,
		};
		OpenLoopVfControl_GetDuties(&vfConfig, duties);
		Inverter3Ph_UpdateDuty(inverterConfig1, duties);
		Inverter3Ph_UpdateDuty(inverterConfig2, duties);
#else
		static LIB_3COOR_ABC_t sVabc; 
		static LIB_3COOR_DQ0_t sVdq0;
		static bool lockedPhase = false;
		LIB_3COOR_SINCOS_t* sSincos;
		FillGridVoltages(&sVabc);
		sSincos = LockVoltagePhase(&sVabc, &sVdq0);
		
		if(sSincos != NULL)
		{
			sSincos->fTheta += 0;
			sSincos->fTheta = AdjustTheta(sSincos->fTheta);
			convert_wt2sincos(sSincos);
			if(lockedPhase == false)
			{
				// turn on relays once the phase is locked properly --todo--
				lockedPhase = true;
			}				
			theta = sSincos->fTheta;
		}
		
		if(lockedPhase)
		{
			static LIB_3COOR_ABC_t sIabc; 
			float duties[3];
			FillGridCurrents(&sIabc);
			SetCurrent(&sIabc, sSincos, 5, duties);
			Inverter3Ph_UpdateDuty(inverterConfig, duties);
		}
		if(sSincos != NULL)
		{
			sSincos->fTheta -= 0;
			sSincos->fTheta = AdjustTheta(sSincos->fTheta);
			convert_wt2sincos(sSincos);
		}
#endif
		recompute = false;
	}
}

/* EOF */
