/**
 ********************************************************************************
 * @file    	MainControl.c
 * @author  	Waqas Ehsan Butt
 * @date    	October 5, 2021
 * @copyright 	Taraz Technologies Pvt. Ltd.
 *
 * @brief  Main Controller for this Application
 ********************************************************************************
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "general_header.h"
#include "control_library.h"
#include "adc_config.h"
#include "grid_tie_controller.h"
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
static void Inverter3Ph_ResetSignal(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern adc_measures_t adcVals;
#if CONTROL_TYPE == OPEN_LOOP_VF_CONTROL
static inverter3Ph_config_t inverterConfig2 =
{
		.s1PinNos = {7, 9, 11},
		.dsblPinNo = 0,
		.dsblPinCount = 0,
		.legType = LEG_DEFAULT,
		.pwmConfig = {
				.lim = {
						.min = 0,
						.max = 1,
						.minMaxDutyCycleBalancing = true
				},
				.dutyMode = OUTPUT_DUTY_AT_PWMH,
				.module = &inverterPWMModuleConfig,
		},
};
#endif
static volatile bool recompute = false;
extern HRTIM_HandleTypeDef hhrtim;
#if CONTROL_TYPE == OPEN_LOOP_VF_CONTROL
static openloopvf_config_t vfConfig = {
		.pwmFreq = 25000, .acceleration = 1.00001f, .nominalFreq = 50, .nominalModulationIndex = 0.7f, .freq = 1.0f, .wt = 0, .reqFreq = 25,
};
#elif CONTROL_TYPE == GRID_TIE_CONTROL
/**
 * @brief Grid Tie Control Parameters
 */
static grid_tie_t gridTie = {0};
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Initiates PWM for both inverters, and enable Disable signals and configure the relays
 */
void MainControl_Init(void)
{
	BSP_DigitalPins_Init();
#if CONTROL_TYPE == OPEN_LOOP_VF_CONTROL
	inverterPWMModuleConfig.interruptEnabled = false;
	Inverter3Ph_Init(&inverterConfig2);
#else
	GridTieControl_Init(&gridTie, Inverter3Ph_ResetSignal);
#endif
	// turn off relays and disable pins
	BSP_Dout_SetAsIOPin(13, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(14, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(15, GPIO_PIN_RESET);
	BSP_Dout_SetAsIOPin(16, GPIO_PIN_RESET);
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

static void Inverter3Ph_ResetSignal(void)
{
	recompute = true;
}

/**
 * @brief Call this function to process the control loop.
 * If the new computation request is available new duty cycle values are computed and applied to all inverter legs
 */
float duties[3];
void MainControl_Loop(void)
{
	if(recompute)
	{

#if CONTROL_TYPE == OPEN_LOOP_VF_CONTROL
		OpenLoopVfControl_GetDuties(&vfConfig, duties);
		Inverter3Ph_UpdateDuty(&inverterConfig, duties);
		Inverter3Ph_UpdateDuty(&inverterConfig2, duties);
#elif CONTROL_TYPE == GRID_TIE_CONTROL
		/* add the required measurements and current reference point */
		gridTie.vCoor.abc.a = adcVals.V1;
		gridTie.vCoor.abc.b = adcVals.V2;
		gridTie.vCoor.abc.c = adcVals.V3;
		gridTie.vdc = adcVals.Vdc1;
		gridTie.iCoor.abc.a = adcVals.Ih1;
		gridTie.iCoor.abc.b = adcVals.Ih2;
		gridTie.iCoor.abc.c = adcVals.Ih3;

		if (gridTie.state == GRID_TIE_INACTIVE)
			gridTie.iRef = .1f;
		else if (gridTie.iRef < 5.f)
			gridTie.iRef += .0001f;

		GridTieControl_Loop(&gridTie);
#endif
		recompute = false;
	}
}

/* EOF */
