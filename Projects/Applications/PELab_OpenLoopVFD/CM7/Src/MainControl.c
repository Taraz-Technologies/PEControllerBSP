/**
 ********************************************************************************
 * @file    	MainControl.c
 * @author  	Waqas Ehsan Butt
 * @date    	Oct 5, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief  Main Controller for this Application
 ********************************************************************************
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
#include "BasicGridTieControl.h"
#include "GridTieControl.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define OPEN_LOOP_VF_CONTROL			(1)
#define GRID_TIE_CONTROL				(2)

#define CONTROL_TYPE					(GRID_TIE_CONTROL)

#define PWM_PERIOD_Us					(40)
#define PWM_PERIOD_s					(PWM_PERIOD_Us/1000000.f)
#define PWM_FREQ_KHz					(1000.f/PWM_PERIOD_Us)
#define PWM_FREQ_Hz						(1.f/PWM_PERIOD_s)
/*******************************************************************************
 * Enums
 ******************************************************************************/

/*******************************************************************************
 * Structures
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
		.periodInUs = PWM_PERIOD_Us,
		.interruptEnabled = true,			/* not catered for right now */
		.alignment = CENTER_ALIGNED,
		.deadtimeInNanosec = 500,
		.deadtimeEnable = true,
		.resetCallback = Inverter3Ph_ResetSignal,
		.minMaxDutyCycleBalancing = false,
};
static inverter3Ph_init_config_t inverterInitConfig2 =
{
		.pins = { 7, 9, 11 },  /* should be odd as these are pairs */
		.dsblPin = 14,
		.periodInUs = PWM_PERIOD_Us,
		.interruptEnabled = false,			/* not catered for right now */
		.alignment = CENTER_ALIGNED,
		.deadtimeInNanosec = 500,
		.deadtimeEnable = true,			// --todo-- doesn't work when disabled
		.resetCallback = NULL,
		.minMaxDutyCycleBalancing = true,
};
static volatile bool recompute = false;
extern HRTIM_HandleTypeDef hhrtim;
#if CONTROL_TYPE == OPEN_LOOP_VF_CONTROL
static openloopvf_config_t vfConfig = {
		.pwmFreq = 25000, .acceleration = 1.00001f, .nominalFreq = 50, .nominalModulationIndex = 0.7f, .freq = 1.0f, .theta = 0, .reqFreq = 25,
};
#elif CONTROL_TYPE == GRID_TIE_CONTROL
/**
 * @brief Grid Tie Control Parameters
 */
static grid_tie_t gridTie = { .vCoor = {0}, .iCoor = {0}, .pll = {0},
		.qCompensator = { .Integral = 0, .Kp = 7.f, .Ki = 70.f, .dt = PWM_PERIOD_s },
		.dCompensator = { .Integral = 0, .Kp = 7.f, .Ki = 70.f, .dt = PWM_PERIOD_s },
};
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Initiates PWM for both inverters, and enable Disable signals and configure the relays
 */
void MainControl_Init(void)
{
	DigitalPins_Init();

	inverterConfig1 = Inverter3Ph_Init(&inverterInitConfig1);
	//inverterConfig2 = Inverter3Ph_Init(&inverterInitConfig2);

	if(inverterConfig1 == NULL)// || inverterConfig2 == NULL)
		Error_Handler();

	Dout_SetAsIOPin(15, GPIO_PIN_RESET);
	Dout_SetAsIOPin(16, GPIO_PIN_RESET);
}

/**
 * @brief Call this function to run both inverter PWMs
 */
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

/**
 * @brief Call this function to stop the inverters
 */
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

/**
 * @brief Call this function to process the control loop.
 * If the new computation request is available new duty cycle values are computed and applied to all inverter legs
 */
void MainControl_Loop(void)
{
	if(recompute)
	{
		float duties[3];
#if CONTROL_TYPE == OPEN_LOOP_VF_CONTROL
		OpenLoopVfControl_GetDuties(&vfConfig, duties);
		Inverter3Ph_UpdateDuty(inverterConfig1, duties);
		Inverter3Ph_UpdateDuty(inverterConfig2, duties);
#elif CONTROL_TYPE == GRID_TIE_CONTROL
		/* add the required measurements and current reference point */
		gridTie.vCoor.abc.a = adcVals.V1;
		gridTie.vCoor.abc.b = adcVals.V2;
		gridTie.vCoor.abc.c = adcVals.V3;
		gridTie.vdc = adcVals.Vdc1;
		gridTie.iCoor.abc.a = adcVals.Ih1;
		gridTie.iCoor.abc.b = adcVals.Ih2;
		gridTie.iCoor.abc.c = adcVals.Ih3;
		gridTie.iRef = 5.f;

		/* compute the duty cycles for grid tie control */
		GridTieControl_GetDuties(&gridTie, duties);

		/* Update the duty cycles for all inverter legs */
		Inverter3Ph_UpdateDuty(inverterConfig1, duties);
#endif
		recompute = false;
	}
}

/* EOF */
