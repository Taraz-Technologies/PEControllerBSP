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
#include "general_header.h"
#include "control_library.h"
#include "adc_config.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/
#define OPEN_LOOP_VF_CONTROL			(1)
#define GRID_TIE_CONTROL				(2)

#define CONTROL_TYPE					(OPEN_LOOP_VF_CONTROL)

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

static pwm_module_config_t inverterPWMModuleConfig =
{
		.interruptEnabled = true,
		.alignment = CENTER_ALIGNED,
		.periodInUsec = PWM_PERIOD_Us,
		.deadtime = {
				.on = true,
				.nanoSec = 500,
		},
		.callback = Inverter3Ph_ResetSignal,
};
static inverter3Ph_config_t inverterConfig =
{
		.s1PinNos = {1, 3, 5},
		.dsblPinNo = 14,
		.dsblPinCount = 1,
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
static pwm_module_config_t boostPWMConfig = {
		.interruptEnabled = false,
		.alignment = EDGE_ALIGNED,
		.periodInUsec = PWM_PERIOD_Us,
		.deadtime = NULL,
		.callback = NULL,
};
static independent_pwm_config_t boostConfig =
{
		.pinNo = 8,
		.pwmConfig = {
				.lim = {
						.min = 0,
						.max = 0.5f,
				},
				.module = &boostPWMConfig,
		}
};
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
static grid_tie_t gridTie = { .vCoor = {0}, .iCoor = {0}, .pll = {0},
		.qCompensator = { .Integral = 0, .Kp = 7.f, .Ki = 40.f, .dt = PWM_PERIOD_s },
		.dCompensator = { .Integral = 0, .Kp = 7.f, .Ki = 40.f, .dt = PWM_PERIOD_s },
		.iRef = 0.1f,
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
	BSP_DigitalPins_Init();

	// Initialize the inverter
	Inverter3Ph_Init(&inverterConfig);
#if CONTROL_TYPE == OPEN_LOOP_VF_CONTROL
	inverterPWMModuleConfig.interruptEnabled = false;
	Inverter3Ph_Init(&inverterConfig2);
#else
	boostConfig.dutyUpdateFnc = BSP_PWM_ConfigChannel(boostConfig.pinNo, &boostConfig.pwmConfig);
	boostConfig.dutyUpdateFnc(boostConfig.pinNo, 0.f, &boostConfig.pwmConfig);
	BSP_Dout_SetAsIOPin(7, GPIO_PIN_RESET);
	BSP_Dout_SetAsPWMPin(boostConfig.pinNo);
#endif
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
		/* compensator */
		static bool vdcCorrect = false;
		static float data_avg[8];
		static low_pass_filter_t filt = {.data = data_avg, .size=8, .filt = {.avg = 0, .count = 8, .dataPtr = data_avg}};
		static pi_compensator_t boostPI = {.has_lmt = true, .Kp = .01f, .Ki = 1.1f, .dt = PWM_PERIOD_s, .Integral = 0, .max = 0.45f, .min = 0.f };
		float errAvg = MovingAverage_Compute(&filt.filt, 700.f - gridTie.vdc);
		float duty = PI_Compensate(&boostPI, errAvg);

		if (vdcCorrect == false && fabsf(700 - gridTie.vdc) < 20)
			vdcCorrect = true;

		/* add the required measurements and current reference point */
		gridTie.vCoor.abc.a = adcVals.V1;
		gridTie.vCoor.abc.b = adcVals.V2;
		gridTie.vCoor.abc.c = adcVals.V3;
		gridTie.vdc = adcVals.Vdc1;
		gridTie.iCoor.abc.a = adcVals.Ih1;
		gridTie.iCoor.abc.b = adcVals.Ih2;
		gridTie.iCoor.abc.c = adcVals.Ih3;

		if (vdcCorrect && gridTie.iRef < 10.f)
			gridTie.iRef += .0001f;
		/* compute the duty cycles for grid tie control */
		GridTieControl_GetDuties(&gridTie, duties);
		/* Update the duty cycles for all inverter legs */
		Inverter3Ph_UpdateDuty(&inverterConfig, duties);

		/* Update duty cycle for the boost */
		boostConfig.dutyUpdateFnc(boostConfig.pinNo, duty, &boostConfig.pwmConfig);
#endif
		recompute = false;
	}
}

/* EOF */
