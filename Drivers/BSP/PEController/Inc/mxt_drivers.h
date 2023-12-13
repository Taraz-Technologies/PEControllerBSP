/**
 ********************************************************************************
 * @file 		mxt_drivers.h
 * @author 		Waqas Ehsan Butt
 * @date 		Mar 15, 2023
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

#ifndef MXT_DRIVERS_H_
#define MXT_DRIVERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************
 * Includes
 *******************************************************************************/
#include "user_config.h"
#include "general_header.h"
#include "ts.h"
#include "mXT336T.h"
/********************************************************************************
 * Defines
 *******************************************************************************/

/* Version */
/* Version */
#define MXT_VER_20		20
#define MXT_VER_21		21
#define MXT_VER_22		22

#define REV_D	0x32

/* Registers */
#define MXT_FAMILY_ID		0x00
#define MXT_VARIANT_ID		0x01
#define MXT_VERSION		0x02
#define MXT_BUILD		0x03
#define MXT_MATRIX_X_SIZE	0x04
#define MXT_MATRIX_Y_SIZE	0x05
#define MXT_OBJECT_NUM		0x06
#define MXT_OBJECT_START	0x07

#define MXT_OBJECT_SIZE		6

#define MXT_MAX_BLOCK_WRITE	255

/* Channels */
#define MXT_336T_CHANNELS_X	24
#define MXT_336T_CHANNELS_Y	14
#define MXT_336T_MAX_NODES	(MXT_336T_CHANNELS_X * MXT_336T_CHANNELS_Y)
#define MXT_336T_X_SELFREF_RESERVED	2
#define MXT_640T_CHANNELS_X	32
#define MXT_640T_CHANNELS_Y	20
#define MXT_640T_MAX_NODES	(MXT_640T_CHANNELS_X * MXT_640T_CHANNELS_Y)
#define MXT_640T_X_SELFREF_RESERVED	4

/* Object types */
#define MXT_DEBUG_DIAGNOSTIC_T37	37
#define MXT_SPT_USERDATA_T38		38
#define MXT_GEN_MESSAGE_T5		5
#define MXT_GEN_COMMAND_T6		6
#define MXT_GEN_POWER_T7		7
#define MXT_GEN_ACQUIRE_T8		8
#define MXT_GEN_DATASOURCE_T53		53
#define MXT_TOUCH_MULTI_T9		9
#define MXT_TOUCH_KEYARRAY_T15		15
#define MXT_TOUCH_PROXIMITY_T23		23
#define MXT_TOUCH_PROXKEY_T52		52
#define MXT_PROCI_GRIPFACE_T20		20
#define MXT_PROCG_NOISE_T22		22
#define MXT_PROCI_ACTIVE_STYLUS_T63	63
#define MXT_PROCI_ONETOUCH_T24		24
#define MXT_PROCI_TWOTOUCH_T27		27
#define MXT_PROCI_GRIP_T40		40
#define MXT_PROCI_PALM_T41		41
#define MXT_PROCI_TOUCHSUPPRESSION_T42	42
#define MXT_PROCI_STYLUS_T47		47
#define MXT_PROCG_NOISESUPPRESSION_T48	48
#define MXT_SPT_COMMSCONFIG_T18		18
#define MXT_SPT_GPIOPWM_T19		19
#define MXT_SPT_SELFTEST_T25		25
#define MXT_SPT_CTECONFIG_T28		28
#define MXT_SPT_DIGITIZER_T43		43
#define MXT_SPT_MESSAGECOUNT_T44	44
#define MXT_SPT_CTECONFIG_T46		46
#define MXT_SPT_NOISESUPPRESSION_T48    48
#define MXT_SPT_TIMER_T61		61
#define MXT_PROCI_LENSBENDING_T65	65
#define MXT_SPT_GOLDENREF_T66		66
#define MXT_PDS_INFO_T68		68
#define MXT_SPT_DYMCFG_T70		70
#define MXT_SPT_DYMDATA_T71		71
#define MXT_PROCG_NOISESUPPRESSION_T72	72
#define MXT_PROCI_GLOVEDETECTION_T78		78
#define MXT_PROCI_RETRANSMISSIONCOMPENSATION_T80	80
#define MXT_TOUCH_MORE_GESTURE_T81	81
#define MXT_TOUCH_GESTURE_T92		92
#define MXT_TOUCH_SEQUENCE_LOGGER_T93	93
#define MXT_TOUCH_MULTI_T100		100
#define MXT_SPT_TOUCHSCREENHOVER_T101	101
#define MXT_PROCG_NOISESUPSELFCAP_T108	108
#define MXT_SPT_SELFCAPGLOBALCONFIG_T109	109
#define MXT_SPT_AUXTOUCHCONFIG_T104	104
#define MXT_TOUCH_KEYARRAY_T97		97

/* MXT_GEN_MESSAGE_T5 object */
#define MXT_RPTID_NOMSG		0xff

/* MXT_GEN_COMMAND_T6 field */
#define MXT_COMMAND_RESET	0
#define MXT_COMMAND_BACKUPNV	1
#define MXT_COMMAND_CALIBRATE	2
#define MXT_COMMAND_REPORTALL	3
#define MXT_COMMAND_DIAGNOSTIC	5

/* MXT_GEN_POWER_T7 field */
#define MXT_POWER_IDLEACQINT	0
#define MXT_POWER_ACTVACQINT	1
#define MXT_POWER_ACTV2IDLETO	2
#define MXT_POWER_CFG 3
#define MXT_POWER_CFG2 4

#define MXT_POWER_CFG_RUN		0
#define MXT_POWER_CFG_DEEPSLEEP		1
#define MXT_POWER_CFG_WAKEUP_GESTURE	2

/* MXT_GEN_ACQUIRE_T8 field */
#define MXT_ACQUIRE_CHRGTIME	0
#define MXT_ACQUIRE_TCHDRIFT	2
#define MXT_ACQUIRE_DRIFTST	3
#define MXT_ACQUIRE_TCHAUTOCAL	4
#define MXT_ACQUIRE_SYNC	5
#define MXT_ACQUIRE_ATCHCALST	6
#define MXT_ACQUIRE_ATCHCALSTHR	7
#define MXT_ACQUIRE_ATCHFRCCALTHR 8
#define MXT_ACQUIRE_ATCHFRCCALRATIO 9
#define MXT_ACQUIRE_MEASALLOW	10
#define MXT_ACQUIRE_MEASIDLEDEF 11
#define MXT_ACQUIRE_MEASACTVDEF 12
#define MXT_ACQUIRE_REFMODE 13
#define MXT_ACQUIRE_CFG 14

/* MXT_TOUCH_MULTI_T9 field */
#define MXT_TOUCH_CTRL		0
#define MXT_TOUCH_XORIGIN	1
#define MXT_TOUCH_YORIGIN	2
#define MXT_TOUCH_XSIZE		3
#define MXT_TOUCH_YSIZE		4
#define MXT_TOUCH_BLEN		6
#define MXT_TOUCH_TCHTHR	7
#define MXT_TOUCH_TCHDI		8
#define MXT_TOUCH_ORIENT	9
#define MXT_TOUCH_MOVHYSTI	11
#define MXT_TOUCH_MOVHYSTN	12
#define MXT_TOUCH_NUMTOUCH	14
#define MXT_TOUCH_MRGHYST	15
#define MXT_TOUCH_MRGTHR	16
#define MXT_TOUCH_AMPHYST	17
#define MXT_TOUCH_XRANGE_LSB	18
#define MXT_TOUCH_XRANGE_MSB	19
#define MXT_TOUCH_YRANGE_LSB	20
#define MXT_TOUCH_YRANGE_MSB	21
#define MXT_TOUCH_XLOCLIP	22
#define MXT_TOUCH_XHICLIP	23
#define MXT_TOUCH_YLOCLIP	24
#define MXT_TOUCH_YHICLIP	25
#define MXT_TOUCH_XEDGECTRL	26
#define MXT_TOUCH_XEDGEDIST	27
#define MXT_TOUCH_YEDGECTRL	28
#define MXT_TOUCH_YEDGEDIST	29
#define MXT_TOUCH_JUMPLIMIT	30

/* MXT_TOUCH_MULTI_T100 field */
#define MXT_MULTITOUCH_CTRL		0
#define MXT_MULTITOUCH_CFG1		1
#define MXT_MULTITOUCH_SCRAUX			2
#define MXT_MULTITOUCH_TCHAUX		3
#define MXT_MULTITOUCH_TCHEVENTCFG		4
#define MXT_MULTITOUCH_AKSCFG			5
#define MXT_MULTITOUCH_NUMTCH		6
#define MXT_MULTITOUCH_XYCFG		7
#define MXT_MULTITOUCH_XORIGIN		8
#define MXT_MULTITOUCH_XSIZE		9
#define MXT_MULTITOUCH_XPITCH			10
#define MXT_MULTITOUCH_XLOCLIP		11
#define MXT_MULTITOUCH_XHICLIP		12
#define MXT_MULTITOUCH_XRANGE_LSB		13
#define MXT_MULTITOUCH_XRANGE_MSB		14
#define MXT_MULTITOUCH_XEDGECFG		15
#define MXT_MULTITOUCH_XEDGEDIST		16
#define MXT_MULTITOUCH_DXEDGECFG		17
#define MXT_MULTITOUCH_DXEDGEDIST		18
#define MXT_MULTITOUCH_YORIGIN		19
#define MXT_MULTITOUCH_YSIZE		20
#define MXT_MULTITOUCH_YPITCH			21
#define MXT_MULTITOUCH_YLOCLIP		22
#define MXT_MULTITOUCH_YHICLIP		23
#define MXT_MULTITOUCH_YRANGE_LSB		24
#define MXT_MULTITOUCH_YRANGE_MSB		25
#define MXT_MULTITOUCH_YEDGECFG		26
#define MXT_MULTITOUCH_YEDGEDIST		27
#define MXT_MULTITOUCH_GAIN		28
#define MXT_MULTITOUCH_DXGAIN			29
#define MXT_MULTITOUCH_TCHTHR			30
#define MXT_MULTITOUCH_TCHHYST		31
#define MXT_MULTITOUCH_INTTHR			32
#define MXT_MULTITOUCH_NOISESF		33
#define MXT_MULTITOUCH_MGRTHR		35
#define MXT_MULTITOUCH_MRGTHRADJSTR		36
#define MXT_MULTITOUCH_MRGHYST		37
#define MXT_MULTITOUCH_DXTHRSF		38
#define MXT_MULTITOUCH_TCHDIDOWN		39
#define MXT_MULTITOUCH_TCHDIUP		40
#define MXT_MULTITOUCH_NEXTTCHDI		41
#define MXT_MULTITOUCH_JUMPLIMIT		43
#define MXT_MULTITOUCH_MOVFILTER		44
#define MXT_MULTITOUCH_MOVSMOOTH		45
#define MXT_MULTITOUCH_MOVPRED		46
#define MXT_MULTITOUCH_MOVHYSTILSB		47
#define MXT_MULTITOUCH_MOVHYSTIMSB		48
#define MXT_MULTITOUCH_MOVHYSTNLSB		49
#define MXT_MULTITOUCH_MOVHYSTNMSB		50
#define MXT_MULTITOUCH_AMPLHYST		51
#define MXT_MULTITOUCH_SCRAREAHYST		52
#define MXT_MULTITOUCH_CFG2		60


/* T109 self Capacitance MaXcharger */
#define MXT_SELF_CAP_MAXCHARGER_CTRL 0
#define MXT_SELF_CAP_MAXCHARGER_CALCFG1 1
#define MXT_SELF_CAP_MAXCHARGER_CFG1 2
#define MXT_SELF_CAP_MAXCHARGER_CFG2 3
#define MXT_SELF_CAP_MAXCHARGER_CFG3 4
#define MXT_SELF_CAP_MAXCHARGER_NLGAIN 5
#define MXT_SELF_CAP_MAXCHARGER_IIRCOEFF 7
#define MXT_SELF_CAP_MAXCHARGER_MINNLTDDIFF 8
#define MXT_SELF_CAP_MAXCHARGER_MINNLTDHOP 9
#define MXT_SELF_CAP_MAXCHARGER_HOPST 12
#define MXT_SELF_CAP_MAXCHARGER_STABCTRL 13
#define MXT_SELF_CAP_MAXCHARGER_STABFREQ0 14
#define MXT_SELF_CAP_MAXCHARGER_STABFREQ1 15
#define MXT_SELF_CAP_MAXCHARGER_STABFREQ2 16
#define MXT_SELF_CAP_MAXCHARGER_STABFREQ3 17
#define MXT_SELF_CAP_MAXCHARGER_STABFREQ4 18
#define MXT_SELF_CAP_MAXCHARGER_STABTCHAPX0 19
#define MXT_SELF_CAP_MAXCHARGER_STABTCHAPX1 20
#define MXT_SELF_CAP_MAXCHARGER_STABTCHAPX2 21
#define MXT_SELF_CAP_MAXCHARGER_STABTCHAPX3 22
#define MXT_SELF_CAP_MAXCHARGER_STABTCHAPX4 23
#define MXT_SELF_CAP_MAXCHARGER_STABNOTCHAPX0 24
#define MXT_SELF_CAP_MAXCHARGER_STABNOTCHAPX1 25
#define MXT_SELF_CAP_MAXCHARGER_STABNOTCHAPX2 26
#define MXT_SELF_CAP_MAXCHARGER_STABNOTCHAPX3 27
#define MXT_SELF_CAP_MAXCHARGER_STABNOTCHAPX4 28
#define MXT_SELF_CAP_MAXCHARGER_STABHINLTDTHR 31
#define MXT_SELF_CAP_MAXCHARGER_NOISCTRL 33
#define MXT_SELF_CAP_MAXCHARGER_NOISFREQ0 34
#define MXT_SELF_CAP_MAXCHARGER_NOISFREQ1 35
#define MXT_SELF_CAP_MAXCHARGER_NOISFREQ2 36
#define MXT_SELF_CAP_MAXCHARGER_NOISFREQ3 37
#define MXT_SELF_CAP_MAXCHARGER_NOISFREQ4 38
#define MXT_SELF_CAP_MAXCHARGER_NOISTCHAPX0 39
#define MXT_SELF_CAP_MAXCHARGER_NOISTCHAPX1 40
#define MXT_SELF_CAP_MAXCHARGER_NOISTCHAPX2 41
#define MXT_SELF_CAP_MAXCHARGER_NOISTCHAPX3 42
#define MXT_SELF_CAP_MAXCHARGER_NOISTCHAPX4 43
#define MXT_SELF_CAP_MAXCHARGER_NOISNOTCHAPX0 44
#define MXT_SELF_CAP_MAXCHARGER_NOISNOTCHAPX1 45
#define MXT_SELF_CAP_MAXCHARGER_NOISNOTCHAPX2 46
#define MXT_SELF_CAP_MAXCHARGER_NOISNOTCHAPX3 47
#define MXT_SELF_CAP_MAXCHARGER_NOISNOTCHAPX4 48
#define MXT_SELF_CAP_MAXCHARGER_NOISLONLTDTHR 50
#define MXT_SELF_CAP_MAXCHARGER_NOISHINLTDTHR 51
#define MXT_SELF_CAP_MAXCHARGER_NOISCNT 52
#define MXT_SELF_CAP_MAXCHARGER_VNOICTRL 53
#define MXT_SELF_CAP_MAXCHARGER_VNOIFREQ0 54
#define MXT_SELF_CAP_MAXCHARGER_VNOIFREQ1 55
#define MXT_SELF_CAP_MAXCHARGER_VNOIFREQ2 56
#define MXT_SELF_CAP_MAXCHARGER_VNOIFREQ3 57
#define MXT_SELF_CAP_MAXCHARGER_VNOIFREQ4 58
#define MXT_SELF_CAP_MAXCHARGER_VNOITCHAPX0 59
#define MXT_SELF_CAP_MAXCHARGER_VNOITCHAPX1 60
#define MXT_SELF_CAP_MAXCHARGER_VNOITCHAPX2 61
#define MXT_SELF_CAP_MAXCHARGER_VNOITCHAPX3 62
#define MXT_SELF_CAP_MAXCHARGER_VNOITCHAPX4 63
#define MXT_SELF_CAP_MAXCHARGER_VNOINOTCHAPX0 64
#define MXT_SELF_CAP_MAXCHARGER_VNOINOTCHAPX1 65
#define MXT_SELF_CAP_MAXCHARGER_VNOINOTCHAPX2 66
#define MXT_SELF_CAP_MAXCHARGER_VNOINOTCHAPX3 67
#define MXT_SELF_CAP_MAXCHARGER_VNOINOTCHAPX4 68
#define MXT_SELF_CAP_MAXCHARGER_VNOILONLTDTHR 70
#define MXT_SELF_CAP_MAXCHARGER_VNOICNT 72
#define MXT_SELF_CAP_MAXCHARGER_BLKNLTDTHR 73

/* MXT_TOUCH_KEYARRAY_T15 */
#define MXT_KEYARRAY_CTRL	0

/* MXT_PROCI_GRIPFACE_T20 field */
#define MXT_GRIPFACE_CTRL	0
#define MXT_GRIPFACE_XLOGRIP	1
#define MXT_GRIPFACE_XHIGRIP	2
#define MXT_GRIPFACE_YLOGRIP	3
#define MXT_GRIPFACE_YHIGRIP	4
#define MXT_GRIPFACE_MAXTCHS	5
#define MXT_GRIPFACE_SZTHR1	7
#define MXT_GRIPFACE_SZTHR2	8
#define MXT_GRIPFACE_SHPTHR1	9
#define MXT_GRIPFACE_SHPTHR2	10
#define MXT_GRIPFACE_SUPEXTTO	11

/* MXT_PROCI_NOISE field */
#define MXT_NOISE_CTRL		0
#define MXT_NOISE_OUTFLEN	1
#define MXT_NOISE_GCAFUL_LSB	3
#define MXT_NOISE_GCAFUL_MSB	4
#define MXT_NOISE_GCAFLL_LSB	5
#define MXT_NOISE_GCAFLL_MSB	6
#define MXT_NOISE_ACTVGCAFVALID	7
#define MXT_NOISE_NOISETHR	8
#define MXT_NOISE_FREQHOPSCALE	10
#define MXT_NOISE_FREQ0		11
#define MXT_NOISE_FREQ1		12
#define MXT_NOISE_FREQ2		13
#define MXT_NOISE_FREQ3		14
#define MXT_NOISE_FREQ4		15
#define MXT_NOISE_IDLEGCAFVALID	16

/* MXT_SPT_COMMSCONFIG_T18 */
#define MXT_COMMS_CTRL		0
#define MXT_COMMS_CMD		1

/* MXT_SPT_GPIOPWM_T19 */
#define MXT_GPIOPWM_CTRL		0
#define MXT_GPIOPWM_INTPULLUP		3
#define MXT_GPIO_FORCERPT		0x7

/* MXT_SPT_CTECONFIG_T28 field */
#define MXT_CTE_CTRL		0
#define MXT_CTE_CMD		1
#define MXT_CTE_MODE		2
#define MXT_CTE_IDLEGCAFDEPTH	3
#define MXT_CTE_ACTVGCAFDEPTH	4
#define MXT_CTE_VOLTAGE		5

#define MXT_VOLTAGE_DEFAULT	2700000
#define MXT_VOLTAGE_STEP	10000

/* MXT_DEBUG_DIAGNOSTIC_T37 */
#define MXT_DIAG_PAGE_UP	0x01
#define MXT_DIAG_MUTUAL_DELTA	0x10
#define MXT_DIAG_MUTUAL_REF	0x11
#define MXT_DIAG_SELF_DELTA	0xF7
#define MXT_DIAG_SELF_REF	0xF8
#define MXT_DIAG_PAGE_SIZE	0x80
#define MXT_DIAG_TOTAL_SIZE	0x438
#define MXT_DIAG_SELF_SIZE	0x6C
#define MXT_DIAG_REV_ID		21
#define MXT_LOCKDOWN_OFFSET	4

/* MXT_SPT_USERDATA_T38 */
#define MXT_FW_UPDATE_FLAG	0
#define MXT_CONFIG_INFO_SIZE	8

/* MXT_PROCI_STYLUS_T47 */
#define MXT_PSTYLUS_CTRL	0

/* MXT_SPT_TIMER_T61 */
#define MXT_TIMER_PERIODLSB	3
#define MXT_TIMER_PERIODMSB	4

/* MXT_PROCI_LENSBENDING_T65 */
#define MXT_LENSBENDING_CTRL	0

/* MXT_PDS_INFO_T68 */
#define MXT_LOCKDOWN_SIZE	8

/* MXT_PROCG_NOISESUPPRESSION_T72 */
#define MXT_NOISESUP_CTRL	0
#define MXT_NOISESUP_CALCFG	1
#define MXT_NOISESUP_CFG1	2
#define MXT_NOISESUP_CFG2 3
#define MXT_NOISESUP_HOPCNT 6
#define MXT_NOISESUP_HOPCNTPER 6
#define MXT_NOISESUP_HOPEVALTO 7
#define MXT_NOISESUP_HOPST 8
#define MXT_NOISESUP_NLGAINDUALX 9
#define MXT_NOISESUP_MINNLTHR 10
#define MXT_NOISESUP_INCNLTHR 11
#define MXT_NOISESUP_FALLNLTHR 12
#define MXT_NOISESUP_NLTHRMARGIN 13
#define MXT_NOISESUP_MINTHRADJ 14
#define MXT_NOISESUP_NLTHRLIMIT 15
#define MXT_NOISESUP_BGSCAN 16
#define MXT_NOISESUP_NLGAINSINGX 17
#define MXT_NOISESUP_BLKNLTHR 18
#define MXT_NOISESUP_CFG3 19
#define MXT_NOISESUP_STABCTRL 20
#define MXT_NOISESUP_STABFREQ0 21
#define MXT_NOISESUP_STABFREQ1 22
#define MXT_NOISESUP_STABFREQ2 23
#define MXT_NOISESUP_STABFREQ3 24
#define MXT_NOISESUP_STABFREQ4 25
#define MXT_NOISESUP_STABTCHAPX0 26
#define MXT_NOISESUP_STABTCHAPX1 27
#define MXT_NOISESUP_STABTCHAPX2 28
#define MXT_NOISESUP_STABTCHAPX3 29
#define MXT_NOISESUP_STABTCHAPX4 30
#define MXT_NOISESUP_STABNOTCHAPX0 31
#define MXT_NOISESUP_STABNOTCHAPX1 32
#define MXT_NOISESUP_STABNOTCHAPX2 33
#define MXT_NOISESUP_STABNOTCHAPX3 34
#define MXT_NOISESUP_STABNOTCHAPX4 35
#define MXT_NOISESUP_STABHIGHNLTHR 38
#define MXT_NOISESUP_NOISCTRL 40
#define MXT_NOISESUP_NOISFREQ0 41
#define MXT_NOISESUP_NOISFREQ1 42
#define MXT_NOISESUP_NOISFREQ2 43
#define MXT_NOISESUP_NOISFREQ3 44
#define MXT_NOISESUP_NOISFREQ4 45
#define MXT_NOISESUP_NOISTCHAPX0 46
#define MXT_NOISESUP_NOISTCHAPX1 47
#define MXT_NOISESUP_NOISTCHAPX2 48
#define MXT_NOISESUP_NOISTCHAPX3 49
#define MXT_NOISESUP_NOISTCHAPX4 50
#define MXT_NOISESUP_NOISNOTCHAPX0 51
#define MXT_NOISESUP_NOISNOTCHAPX1 52
#define MXT_NOISESUP_NOISNOTCHAPX2 53
#define MXT_NOISESUP_NOISNOTCHAPX3 54
#define MXT_NOISESUP_NOISNOTCHAPX4 55
#define MXT_NOISESUP_NOISLOWNLTHR 57
#define MXT_NOISESUP_NOISHIGHNLTHR 58
#define MXT_NOISESUP_NOISCNT 59
#define MXT_NOISESUP_VNOICTRL 60
#define MXT_NOISESUP_VNOIFREQ0 61
#define MXT_NOISESUP_VNOIFREQ1 62
#define MXT_NOISESUP_VNOIFREQ2 63
#define MXT_NOISESUP_VNOIFREQ3 64
#define MXT_NOISESUP_VNOIFREQ4 65
#define MXT_NOISESUP_VNOITCHAPX0 66
#define MXT_NOISESUP_VNOITCHAPX1 67
#define MXT_NOISESUP_VNOITCHAPX2 68
#define MXT_NOISESUP_VNOITCHAPX3 69
#define MXT_NOISESUP_VNOITCHAPX4 70
#define MXT_NOISESUP_VNOINOTCHAPX0 71
#define MXT_NOISESUP_VNOINOTCHAPX1 72
#define MXT_NOISESUP_VNOINOTCHAPX2 73
#define MXT_NOISESUP_VNOINOTCHAPX3 74
#define MXT_NOISESUP_VNOINOTCHAPX4 75
#define MXT_NOISESUP_VNOILOWNLTHR 77
#define MXT_NOISESUP_VNOICNT 79
#define MXT_NOISESUP_NOTCHMINDIFF 81
#define MXT_NOISESUP_TCHMINDIFF 82
#define MXT_NOISESUP_NOTCHMINHOP 83
#define MXT_NOISESUP_TCHMINHOP 84
#define MXT_NOISESUP_BGNLRATIOSTAB 85
#define MXT_NOISESUP_BGNLRATIONOIS 86
#define MXT_NOISESUP_BGNLRATIOVNOI 87
#define MXT_NOISESUP_NLRATIO 88
//#define MXT_NOISESUP_
/* MXT_PROCI_GLOVEDETECTION_T78 */
#define MXT_GLOVE_CTRL		0x00

/* MXT_TOUCH_KEYARRAY_T97 */
#define MXT_TOUCH_KEYARRAY_INST0_CTRL	0
#define MXT_TOUCH_KEYARRAY_INST1_CTRL	10
#define MXT_TOUCH_KEYARRAY_INST2_CTRL	20

/* MXT_SPT_TOUCHSCREENHOVER_T101 */
#define MXT_HOVER_CTRL		0x00

/* MXT_SPT_AUXTOUCHCONFIG_T104 */
#define MXT_AUXTCHCFG_XTCHTHR	2
#define MXT_AUXTCHCFG_INTTHRX	4
#define MXT_AUXTCHCFG_YTCHTHR	7
#define MXT_AUXTCHCFG_INTTHRY	9

/* MXT_SPT_SELFCAPGLOBALCONFIG_T109 */
#define MXT_SELFCAPCFG_CTRL	0
#define MXT_SELFCAPCFG_CMD	3


/* Defines for Suspend/Resume */
#define MXT_SUSPEND_STATIC      0
#define MXT_SUSPEND_DYNAMIC     1
#define MXT_T7_IDLEACQ_DISABLE  0
#define MXT_T7_ACTVACQ_DISABLE  0
#define MXT_T7_ACTV2IDLE_DISABLE 0
#define MXT_T9_DISABLE          0
#define MXT_T9_ENABLE           0x83
#define MXT_T22_DISABLE         0
#define MXT_T100_DISABLE	0

/* Define for MXT_GEN_COMMAND_T6 */
#define MXT_RESET_VALUE		0x01
#define MXT_BACKUP_VALUE	0x55

/* Define for MXT_PROCG_NOISESUPPRESSION_T42 */
#define MXT_T42_MSG_TCHSUP	(1 << 0)

/* Delay times */
#define MXT_BACKUP_TIME		25	/* msec */
#define MXT_RESET_TIME		200	/* msec */
#define MXT_RESET_NOCHGREAD	400	/* msec */
#define MXT_FWRESET_TIME	1000	/* msec */
#define MXT_WAKEUP_TIME		25	/* msec */

/* Defines for MXT_SLOWSCAN_EXTENSIONS */
#define SLOSCAN_DISABLE         0       /* Disable slow scan */
#define SLOSCAN_ENABLE          1       /* Enable slow scan */
#define SLOSCAN_SET_ACTVACQINT  2       /* Set ACTV scan rate */
#define SLOSCAN_SET_IDLEACQINT  3       /* Set IDLE scan rate */
#define SLOSCAN_SET_ACTV2IDLETO 4       /* Set the ACTIVE to IDLE TimeOut */

/* Command to unlock bootloader */
#define MXT_UNLOCK_CMD_MSB	0xaa
#define MXT_UNLOCK_CMD_LSB	0xdc

/* Bootloader mode status */
#define MXT_WAITING_BOOTLOAD_CMD	0xc0	/* valid 7 6 bit only */
#define MXT_WAITING_FRAME_DATA	0x80	/* valid 7 6 bit only */
#define MXT_FRAME_CRC_CHECK	0x02
#define MXT_FRAME_CRC_FAIL	0x03
#define MXT_FRAME_CRC_PASS	0x04
#define MXT_APP_CRC_FAIL	0x40	/* valid 7 8 bit only */
#define MXT_BOOT_STATUS_MASK	0x3f
#define MXT_BOOT_EXTENDED_ID	(1 << 5)
#define MXT_BOOT_ID_MASK	0x1f

/* Define for T6 status byte */
#define MXT_STATUS_RESET	(1 << 7)
#define MXT_STATUS_OFL		(1 << 6)
#define MXT_STATUS_SIGERR	(1 << 5)
#define MXT_STATUS_CAL		(1 << 4)
#define MXT_STATUS_CFGERR	(1 << 3)
#define MXT_STATUS_COMSERR	(1 << 2)

/* Define for T8 measallow byte */
#define MXT_MEASALLOW_MULT	(1 << 0)
#define MXT_MEASALLOW_SELT	(1 << 1)

/* T9 Touch status */
#define MXT_T9_UNGRIP		(1 << 0)
#define MXT_T9_SUPPRESS		(1 << 1)
#define MXT_T9_AMP		(1 << 2)
#define MXT_T9_VECTOR		(1 << 3)
#define MXT_T9_MOVE		(1 << 4)
#define MXT_T9_RELEASE		(1 << 5)
#define MXT_T9_PRESS		(1 << 6)
#define MXT_T9_DETECT		(1 << 7)

/* T100 Touch status */
#define MXT_T100_CTRL_RPTEN	(1 << 1)
#define MXT_T100_CFG1_SWITCHXY	(1 << 5)

#define MXT_T100_EVENT_NONE	0
#define MXT_T100_EVENT_MOVE	1
#define MXT_T100_EVENT_UNSUP	2
#define MXT_T100_EVENT_SUP	3
#define MXT_T100_EVENT_DOWN	4
#define MXT_T100_EVENT_UP	5
#define MXT_T100_EVENT_UNSUPSUP	6
#define MXT_T100_EVENT_UNSUPUP	7
#define MXT_T100_EVENT_DOWNSUP	8
#define MXT_T100_EVENT_DOWNUP	9

#define MXT_T100_TYPE_RESERVED	0
#define MXT_T100_TYPE_FINGER	1
#define MXT_T100_TYPE_PASSIVE_STYLUS	2
#define MXT_T100_TYPE_ACTIVE_STYLUS	3
#define MXT_T100_TYPE_HOVERING_FINGER	4
#define MXT_T100_TYPE_HOVERING_GLOVE	5

#define MXT_T100_DETECT		(1 << 7)
#define MXT_T100_VECT		(1 << 0)
#define MXT_T100_AMPL		(1 << 1)
#define MXT_T100_AREA		(1 << 2)
#define MXT_T100_PEAK		(1 << 4)

#define MXT_T100_SUP		(1 << 6)

/* T15 KeyArray */
#define MXT_KEY_RPTEN		(1 << 1)
#define MXT_KEY_ADAPTTHREN	(1 << 2)

/* Touch orient bits */
#define MXT_XY_SWITCH		(1 << 0)
#define MXT_X_INVERT		(1 << 1)
#define MXT_Y_INVERT		(1 << 2)

/* T47 passive stylus */
#define MXT_PSTYLUS_ENABLE	(1 << 0)

/* T63 Stylus */
#define MXT_STYLUS_PRESS	(1 << 0)
#define MXT_STYLUS_RELEASE	(1 << 1)
#define MXT_STYLUS_MOVE		(1 << 2)
#define MXT_STYLUS_SUPPRESS	(1 << 3)

#define MXT_STYLUS_DETECT	(1 << 4)
#define MXT_STYLUS_TIP		(1 << 5)
#define MXT_STYLUS_ERASER	(1 << 6)
#define MXT_STYLUS_BARREL	(1 << 7)

#define MXT_STYLUS_PRESSURE_MASK	0x3F

/* Touchscreen absolute values */
#define MXT_MAX_AREA		0xff

/* T66 Golden Reference */
#define MXT_GOLDENREF_CTRL		0x00
#define MXT_GOLDENREF_FCALFAILTHR	0x01
#define MXT_GOLDENREF_FCALDRIFTCNT	0x02
#define MXT_GOLDENREF_FCALDRIFTCOEF	0x03
#define MXT_GOLDENREF_FCALDRIFTTLIM	0x04

#define MXT_GOLDCTRL_ENABLE		(1 << 0)
#define MXT_GOLDCTRL_REPEN		(1 << 1)

#define MXT_GOLDSTS_BADSTOREDATA	(1 << 0)
#define MXT_GOLDSTS_FCALSEQERR	(1 << 3)
#define MXT_GOLDSTS_FCALSEQTO		(1 << 4)
#define MXT_GOLDSTS_FCALSEQDONE	(1 << 5)
#define MXT_GOLDSTS_FCALPASS		(1 << 6)
#define MXT_GOLDSTS_FCALFAIL		(1 << 7)

#define MXT_GOLDCMD_NONE	0x00
#define MXT_GOLDCMD_PRIME	0x04
#define MXT_GOLDCMD_GENERATE	0x08
#define MXT_GOLDCMD_CONFIRM	0x0C

#define MXT_GOLD_CMD_MASK	0x0C

#define MXT_GOLDSTATE_INVALID	0xFF
#define MXT_GOLDSTATE_IDLE	MXT_GOLDSTS_FCALSEQDONE
#define MXT_GOLDSTATE_PRIME	0x02
#define MXT_GOLDSTATE_GEN	0x04
#define MXT_GOLDSTATE_GEN_PASS	(0x04 | MXT_GOLDSTS_FCALPASS)
#define MXT_GOLDSTATE_GEN_FAIL	(0x04 | MXT_GOLDSTS_FCALFAIL)

#define MXT_GOLD_STATE_MASK	0x06

/* T78 glove setting */
#define MXT_GLOVECTL_ALL_ENABLE	0xB9
#define MXT_GLOVECTL_GAINEN	(1 << 4)

/* T80 retransmission */
#define MXT_RETRANS_CTRL	0x0
#define MXT_RETRANS_ATCHTHR	0x4
#define MXT_RETRANS_CTRL_MOISTCALEN	(1 << 4)

/* T81 gesture */
#define MXT_GESTURE_CTRL	0x0

/* T72 noise suppression */
#define MXT_NOICTRL_ENABLE	(1 << 0)
#define MXT_NOICFG_VNOISY	(1 << 1)
#define MXT_NOICFG_NOISY	(1 << 0)

/* MXT_TOUCH_SEQ_LOGGER_T93 field */
#define MXT_TOUCH_SEQ_LOGGER_CTRL 0
#define MXT_TOUCH_SEQ_LOGGER_XMIN_LSB 1
#define MXT_TOUCH_SEQ_LOGGER_XMIN_MSB 2
#define MXT_TOUCH_SEQ_LOGGER_XMAX_LSB 3
#define MXT_TOUCH_SEQ_LOGGER_XMAX_MSB 4
#define MXT_TOUCH_SEQ_LOGGER_YMIN_LSB 5
#define MXT_TOUCH_SEQ_LOGGER_YMIN_MSB 6
#define MXT_TOUCH_SEQ_LOGGER_YMAX_LSB 7
#define MXT_TOUCH_SEQ_LOGGER_YMAX_MSB 8
#define MXT_TOUCH_SEQ_LOGGER_DTPRETIMMIN 18
#define MXT_TOUCH_SEQ_LOGGER_DTPOSTTIMMIN 19
#define MXT_TOUCH_SEQ_LOGGER_DTTCHMOVMAX_1 20
#define MXT_TOUCH_SEQ_LOGGER_DTTCHMOVMAX_2 21
#define MXT_TOUCH_SEQ_LOGGER_DTNOTCHMOVMAX_1 22
#define MXT_TOUCH_SEQ_LOGGER_DTNOTCHMOVMAX_2 23
#define MXT_TOUCH_SEQ_LOGGER_DTTCHTIMMIN 24
#define MXT_TOUCH_SEQ_LOGGER_DTTCHTIMMAX 25
#define MXT_TOUCH_SEQ_LOGGER_DTNOTCHTIMMIN 26
#define MXT_TOUCH_SEQ_LOGGER_DTNOTCHTIMMAX 27
#define MXT_TOUCH_SEQ_LOGGER_FAILRPTEN_1 28
#define MXT_TOUCH_SEQ_LOGGER_FAILRPTEN_2 29

/* MXT_SPT_AUXTOUCHCONFIG_T104 */
#define MXT_AUXTCHCFG_CTRL 0

/* T109 self-cap */
#define MXT_SELFCTL_RPTEN	0x2
#define MXT_SELFCMD_TUNE	0x1
#define MXT_SELFCMD_STM_TUNE	0x2
#define MXT_SELFCMD_AFN_TUNE	0x3
#define MXT_SELFCMD_STCR_TUNE	0x4
#define MXT_SELFCMD_AFCR_TUNE	0x5
#define MXT_SELFCMD_AFNVMSTCR_TUNE	0x6
#define MXT_SELFCMD_RCR_TUNE	0x7


#define MXT_INPUT_EVENT_START		0
#define MXT_INPUT_EVENT_SENSITIVE_MODE_OFF		0
#define MXT_INPUT_EVENT_SENSITIVE_MODE_ON		1
#define MXT_INPUT_EVENT_STYLUS_MODE_OFF		2
#define MXT_INPUT_EVENT_STYLUS_MODE_ON		3
#define MXT_INPUT_EVENT_WAKUP_MODE_OFF		4
#define MXT_INPUT_EVENT_WAKUP_MODE_ON		5
#define MXT_INPUT_EVENT_END		5

#define MXT_MAX_FINGER_NUM	1
#define BOOTLOADER_1664_1188	1
/********************************************************************************
 * Typedefs
 *******************************************************************************/

/********************************************************************************
 * Structures
 *******************************************************************************/
/*! @brief object id information */
typedef struct {
	uint8_t family_id;
	uint8_t variant_id;
	uint8_t version;
	uint8_t build;
	uint8_t matrix_xsize;
	uint8_t matrix_ysize;
	uint8_t object_num;
} mxt_info_t;

typedef struct {
	uint8_t type;
	uint16_t start_address;
	uint16_t size;
	uint16_t instances;
	uint8_t num_report_ids;

	/* to map object and message */
	uint8_t min_reportid;
	uint8_t max_reportid;
}mxt_object_t;

/* Each client has this additional data */
typedef struct {
//	enum mxt_device_state state;
	I2C_HandleTypeDef* hi2c;
//	struct regulator *regulator_vdd;
//	struct regulator *regulator_avdd;
//	uint16_t mem_size;
	uint8_t shiftedAddress;
	mxt_info_t info;
	mxt_object_t *object_table;
	unsigned int irq;
	unsigned int max_x;
	unsigned int max_y;
	bool debug_enabled;
	bool driver_paused;
	bool irq_enabled;
	uint8_t bootloader_addr;
	uint8_t actv_cycle_time;
	uint8_t idle_cycle_time;
	uint8_t actv2idle_timeout;
	uint8_t is_stopped;
	uint8_t max_reportid;
	uint32_t config_crc;
	uint32_t info_block_crc;
	uint8_t num_touchids;
	uint8_t num_stylusids;
	uint8_t *msg_buf;
	uint8_t last_message_count;
	uint8_t t100_tchaux_bits;
	unsigned long keystatus;
	uint8_t vendor_id;
	uint8_t panel_id;
	uint8_t rev_id;
	int current_index;
	uint8_t update_flag;
	uint8_t test_result[6];
	int touch_num;
	uint8_t diag_mode;
	uint8_t atchthr;
	uint8_t sensitive_mode;
	uint8_t stylus_mode;
	uint8_t wakeup_gesture_mode;
	bool is_wakeup_by_gesture;
	int hover_tune_status;
	uint8_t adcperx_normal[10];
	uint8_t adcperx_wakeup[10];
	bool firmware_updated;
	uint8_t lockdown_info[MXT_LOCKDOWN_SIZE];
	uint8_t config_info[MXT_CONFIG_INFO_SIZE];

	/* Slowscan parameters	*/
	int slowscan_enabled;
	uint8_t slowscan_actv_cycle_time;
	uint8_t slowscan_idle_cycle_time;
	uint8_t slowscan_actv2idle_timeout;
	uint8_t slowscan_shad_actv_cycle_time;
	uint8_t slowscan_shad_idle_cycle_time;
	uint8_t slowscan_shad_actv2idle_timeout;
//	struct mxt_golden_msg golden_msg;
//	struct mxt_selfcap_status selfcap_status;
	bool finger_down[MXT_MAX_FINGER_NUM];

	/* Cached parameters from object table */
	uint16_t T5_address;
	uint8_t T5_msg_size;
	uint8_t T6_reportid;
	uint16_t T7_address;
	uint8_t T9_reportid_min;
	uint8_t T9_reportid_max;
	uint8_t T15_reportid_min;
	uint8_t T15_reportid_max;
	uint8_t T19_reportid_min;
	uint8_t T19_reportid_max;
	uint8_t T25_reportid_min;
	uint8_t T25_reportid_max;
	uint16_t T37_address;
	uint8_t T42_reportid_min;
	uint8_t T42_reportid_max;
	uint16_t T44_address;
	uint8_t T48_reportid;
	uint8_t T63_reportid_min;
	uint8_t T63_reportid_max;
	uint8_t T66_reportid;
	uint8_t T81_reportid_min;
	uint8_t T81_reportid_max;
	uint8_t T92_reportid_min;
	uint8_t T92_reportid_max;
	uint8_t T93_reportid_min;
	uint8_t T93_reportid_max;
	uint8_t T100_reportid_min;
	uint8_t T100_reportid_max;
	uint8_t T109_reportid;
	uint8_t T97_reportid_min;
	uint8_t T97_reportid_max;

//	struct pinctrl *ts_pinctrl;
//	struct pinctrl_state *gpio_state_active;
//	struct pinctrl_state *gpio_state_suspend;

//#ifdef CONFIG_FB
//	struct notifier_block fb_notif;
//#endif
}mxt_data_t;
/********************************************************************************
 * Exported Variables
 *******************************************************************************/

/********************************************************************************
 * Global Function Prototypes
 *******************************************************************************/
/**
 * @brief Initialize the internal touch screen drivers
 * @param ts_SizeX Horizontal screen resolution
 * @param ts_SizeY Vertical screen resolution
 * @return <c>HAL_OK</c> if successful else error
 */
extern uint16_t MXTDrivers_Init(uint16_t ts_SizeX, uint16_t ts_SizeY);
/**
 * @brief Detect the state and locations of the touch screen
 * @param x Location to be filled with x position
 * @param y Location to be filled with y position
 * @return <c>true</c> if touch screen pressed else <c>false</c>
 */
extern bool MXTDrivers_GetState(uint16_t* x, uint16_t* y);
/********************************************************************************
 * Code
 *******************************************************************************/

#endif
#ifdef __cplusplus
}
#endif

/* EOF */
