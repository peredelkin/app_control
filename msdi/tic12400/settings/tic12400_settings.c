/*
 * tic12400_settings.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "tic12400_settings.h"

const TIC12400_SETTINGS_TypeDef tic124_settings_const = {
		.CONFIG.bit.reset = 0x0,			/*No reset*/
		.CONFIG.bit.poll_time = 0x2,		/*8ms(how frequently each input is sampled)*/
		.CONFIG.bit.poll_act_time = 0x7,	/*512μs(time for wetting current)*/
		.CONFIG.bit.crc_t = 0x0,			/*no CRC calculation triggered*/
		.CONFIG.bit.poll_en = 0x1,			/*Polling enabled*/
		.CONFIG.bit.trigger = 0x0,			/*Stop TIC12400-Q1 from normal operation*/
		.CONFIG.bit.int_config = 0x0,		/*INT pin assertion scheme set to static*/
		.CONFIG.bit.tw_cur_dis_cso = 0x0,	/*Enable CSO wetting current reduction*/
		.CONFIG.bit.det_filter = 0x1,		/*2 consecutive and equal samples required to be valid data*/
		.CONFIG.bit.tw_cur_dis_csi = 0x0,	/*Enable CSI wetting current reduction*/
		.CONFIG.bit.vs_meas_en = 0x1,		/*Enable VS measurement at the end of every polling cycle*/
		.CONFIG.bit.wet_d_in0_en = 0x0,		/*Disable wetting current diagnostic for input IN0*/
		.CONFIG.bit.wet_d_in1_en = 0x0,		/*Disable wetting current diagnostic for input IN1*/
		.CONFIG.bit.wet_d_in2_en = 0x0,		/*Disable wetting current diagnostic for input IN2*/
		.CONFIG.bit.wet_d_in3_en = 0x0,		/*Disable wetting current diagnostic for input IN3*/
		.CONFIG.bit.adc_diag_t = 0x0,		/*Disable ADC self-diagnostic feature*/
		.CONFIG.bit.vs_ratio = 0x1,			/*Use voltage divider factor of 10 for the VS measurement*/

		.IN_EN.bit.in_en_0 = 0x0,			/*Input disabled for ResDI_4*/
		.IN_EN.bit.in_en_1 = 0x0,			/*Input disabled for ResDI_2*/
		.IN_EN.bit.in_en_2 = 0x1,			/*Input enabled for NTC5*/
		.IN_EN.bit.in_en_3 = 0x1,			/*Input enabled for NTC6*/
		.IN_EN.bit.in_en_4 = 0x1,			/*Input enabled for NTC4*/
		.IN_EN.bit.in_en_5 = 0x1,			/*Input enabled for NTC1*/
		.IN_EN.bit.in_en_6 = 0x1,			/*Input enabled for NTC3*/
		.IN_EN.bit.in_en_7 = 0x1,			/*Input enabled for NTC2*/
		.IN_EN.bit.in_en_8 = 0x1,			/*Input enabled for DI1*/
		.IN_EN.bit.in_en_9 = 0x1,			/*Input enabled for DI2*/
		.IN_EN.bit.in_en_10 = 0x1,			/*Input enabled for DI3*/
		.IN_EN.bit.in_en_11 = 0x1,			/*Input enabled for DI4*/
		.IN_EN.bit.in_en_12 = 0x1,			/*Input enabled for DI5*/
		.IN_EN.bit.in_en_13 = 0x1,			/*Input enabled for DI6*/
		.IN_EN.bit.in_en_14 = 0x1,			/*Input enabled for DI7*/
		.IN_EN.bit.in_en_15 = 0x1,			/*Input enabled for DI8*/
		.IN_EN.bit.in_en_16 = 0x0,			/*Input disabled for NC*/
		.IN_EN.bit.in_en_17 = 0x0,			/*Input disabled for NC*/
		.IN_EN.bit.in_en_18 = 0x1,			/*Input enabled for 5VREF*/
		.IN_EN.bit.in_en_19 = 0x1,			/*Input enabled for DI9*/
		.IN_EN.bit.in_en_20 = 0x1,			/*Input enabled for DI10*/
		.IN_EN.bit.in_en_21 = 0x0,			/*Input disabled for ResDI_10*/
		.IN_EN.bit.in_en_22 = 0x0,			/*Input disabled for ResDI_8*/
		.IN_EN.bit.in_en_23 = 0x0,			/*Input disabled for ResDI_6*/

		.CS_SELECT.bit.cs_in0 = 0x1,		/*Current Sink (CSI)*/
		.CS_SELECT.bit.cs_in1 = 0x1,		/*Current Sink (CSI)*/
		.CS_SELECT.bit.cs_in2 = 0x1,		/*Current Sink (CSI)*/
		.CS_SELECT.bit.cs_in3 = 0x1,		/*Current Sink (CSI)*/
		.CS_SELECT.bit.cs_in4 = 0x1,		/*Current Sink (CSI)*/
		.CS_SELECT.bit.cs_in5 = 0x1,		/*Current Sink (CSI)*/
		.CS_SELECT.bit.cs_in6 = 0x1,		/*Current Sink (CSI)*/
		.CS_SELECT.bit.cs_in7 = 0x1,		/*Current Sink (CSI)*/
		.CS_SELECT.bit.cs_in8 = 0x1,		/*Current Sink (CSI)*/
		.CS_SELECT.bit.cs_in9 = 0x1,		/*Current Sink (CSI)*/

		.WC_CFG0.bit.wc_in0_in1 = 0x0,		/*0mA wetting current ResDI_4 and ResDI_2*/
		.WC_CFG0.bit.wc_in2_in3 = 0x0,		/*0mA wetting current NTC5 and NTC6*/
		.WC_CFG0.bit.wc_in4 = 0x0,			/*0mA wetting current NTC4*/
		.WC_CFG0.bit.wc_in5 = 0x0,			/*0mA wetting current NTC1*/
		.WC_CFG0.bit.wc_in6_in7 = 0x0,		/*0mA wetting current NTC3 and NTC2*/
		.WC_CFG0.bit.wc_in8_in9 = 0x0,		/*0mA wetting current DI1 and DI2*/
		.WC_CFG0.bit.wc_in10 = 0x0,			/*0mA wetting current DI3*/
		.WC_CFG0.bit.wc_in11 = 0x0,			/*0mA wetting current DI4*/

		.WC_CFG1.bit.wc_in12_in13 = 0x0,	/*0mA wetting current DI5 and DI6*/
		.WC_CFG1.bit.wc_in14_in15 = 0x0,	/*0mA wetting current DI7 and DI8*/
		.WC_CFG1.bit.wc_in16_in17 = 0x0,	/*0mA wetting current NC and NC*/
		.WC_CFG1.bit.wc_in18_in19 = 0x0,	/*0mA wetting current 5VREF and DI9*/
		.WC_CFG1.bit.wc_in20_in21 = 0x0,	/*0mA wetting current DI10 and ResDI_10*/
		.WC_CFG1.bit.wc_in22 = 0x0, 		/*0mA wetting current ResDI_8*/
		.WC_CFG1.bit.wc_in23 = 0x0, 		/*0mA wetting current ResDI_6*/
		.WC_CFG1.bit.auto_scale_dis_cso = 0x0, /*Enable CSO wetting current auto-scaling*/
		.WC_CFG1.bit.auto_scale_dis_csi = 0x0, /*Enable CSI wetting current auto-scaling*/

		.CCP_CFG0.bit.wc_ccp0 = 0x0,		/*10mA wetting current in CCP mode*/
		.CCP_CFG0.bit.wc_ccp1 = 0x0,		/*10mA wetting current in CCP mode*/
		.CCP_CFG0.bit.wc_ccp2 = 0x0,		/*10mA wetting current in CCP mode*/
		.CCP_CFG0.bit.wc_ccp3 = 0x0,		/*10mA wetting current in CCP mode*/
		.CCP_CFG0.bit.ccp_time = 0x0,		/*Wetting current activation time in CCP mode 64μs*/

		.CCP_CFG1.bit.ccp_in0 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in1 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in2 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in3 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in4 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in5 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in6 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in7 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in8 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in9 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in10 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in11 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in12 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in13 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in14 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in15 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in16 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in17 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in18 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in19 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in20 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in21 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in22 = 0x0,		/*No CCP wetting current*/
		.CCP_CFG1.bit.ccp_in23 = 0x0,		/*No CCP wetting current*/

		.THRES_COMP.bit.thres_comp_in0_in3 = 0x3, /*Comparator threshold set to 4 V*/
		.THRES_COMP.bit.thres_comp_in4_in7 = 0x3, /*Comparator threshold set to 4 V*/
		.THRES_COMP.bit.thres_comp_in8_in11 = 0x3, /*Comparator threshold set to 4 V*/
		.THRES_COMP.bit.thres_comp_in12_in15 = 0x3, /*Comparator threshold set to 4 V*/
		.THRES_COMP.bit.thres_comp_in16_in19 = 0x3, /*Comparator threshold set to 4 V*/
		.THRES_COMP.bit.thres_comp_in20_in23 = 0x3, /*Comparator threshold set to 4 V*/

		.INT_EN_COMP1.bit.inc_en_0 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_1 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_2 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_3 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_4 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_5 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_6 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_7 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_8 = 0x3,	/*!No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_9 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_10 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP1.bit.inc_en_11 = 0x0,	/*No interrupt generation*/

		.INT_EN_COMP2.bit.inc_en_12 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_13 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_14 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_15 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_16 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_17 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_18 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_19 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_20 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_21 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_22 = 0x0,	/*No interrupt generation*/
		.INT_EN_COMP2.bit.inc_en_23 = 0x0,	/*No interrupt generation*/

		.INT_EN_CFG0.bit.spi_fail_en = 0x0,	/*INT pin assertion due to SPI fail event disabled*/
		.INT_EN_CFG0.bit.prty_fail_en = 0x0, /*INT pin assertion due to parity fail event disabled*/
		.INT_EN_CFG0.bit.ssc_en = 0x0,		/*INT pin assertion due to SSC event disabled*/
		.INT_EN_CFG0.bit.tsd_en = 0x0,		/*INT pin assertion due to TSD event disabled*/
		.INT_EN_CFG0.bit.tw_en = 0x0,		/*INT pin assertion due to TW event disabled*/
		.INT_EN_CFG0.bit.ov_en = 0x0,		/*INT pin assertion due to OV event disabled*/
		.INT_EN_CFG0.bit.uv_en = 0x0,		/*INT pin assertion due to UV event disabled*/
		.INT_EN_CFG0.bit.crc_calc_en = 0x0, /*INT pin assertion due to CRC calculation completion disabled*/
		.INT_EN_CFG0.bit.vs0_en = 0x0,		/*INT pin assertion due to VS0 threshold crossing disabled*/
		.INT_EN_CFG0.bit.vs1_en = 0x0,		/*INT pin assertion due to VS1 threshold crossing disabled*/
		.INT_EN_CFG0.bit.wet_diag_en = 0x0, /*INT pin assertion due to wetting current error disabled*/
		.INT_EN_CFG0.bit.adc_diag_en = 0x0, /*INT pin assertion due to ADC error disabled*/

		.INT_EN_CFG1.bit.in0_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in1_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in2_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in3_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in4_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in5_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in6_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in7_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in8_en = 0x3,		/*!No interrupt generation*/
		.INT_EN_CFG1.bit.in9_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in10_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG1.bit.in11_en = 0x0,		/*No interrupt generation*/

		.INT_EN_CFG2.bit.in12_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG2.bit.in13_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG2.bit.in14_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG2.bit.in15_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG2.bit.in16_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG2.bit.in17_en = 0x0,		/*No interrupt generation*/

		.INT_EN_CFG3.bit.in18_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG3.bit.in19_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG3.bit.in20_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG3.bit.in21_en = 0x0,		/*No interrupt generation*/

		.INT_EN_CFG4.bit.in22_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG4.bit.in23_en = 0x0,		/*No interrupt generation*/
		.INT_EN_CFG4.bit.vs_th0_en = 0x0,	/*No interrupt generation*/
		.INT_EN_CFG4.bit.vs_th1_en = 0x0,	/*No interrupt generation*/

		.THRES_CFG0.bit.thres0 = 0x0,		/*Value of threshold 0*/
		.THRES_CFG0.bit.thres1 = 0x0,		/*Value of threshold 1*/

		.THRES_CFG1.bit.thres0 = 0x0,		/*Value of threshold 2*/
		.THRES_CFG1.bit.thres1 = 0x0,		/*Value of threshold 3*/

		.THRES_CFG2.bit.thres0 = 0x0,		/*Value of threshold 4*/
		.THRES_CFG2.bit.thres1 = 0x0,		/*Value of threshold 5*/

		.THRES_CFG3.bit.thres0 = 0x0,		/*Value of threshold 6*/
		.THRES_CFG3.bit.thres1 = 0x0,		/*Value of threshold 7*/

		.THRES_CFG4.bit.thres0 = 0x0,		/*Value of threshold 8*/
		.THRES_CFG4.bit.thres1 = 0x0,		/*Value of threshold 9*/

		.THRESMAP_CFG0.bit.thresmap_in0 = 0x0, /*Thres 0*/
		.THRESMAP_CFG0.bit.thresmap_in1 = 0x0, /*Thres 0*/
		.THRESMAP_CFG0.bit.thresmap_in2 = 0x0, /*Thres 0*/
		.THRESMAP_CFG0.bit.thresmap_in3 = 0x0, /*Thres 0*/
		.THRESMAP_CFG0.bit.thresmap_in4 = 0x0, /*Thres 0*/
		.THRESMAP_CFG0.bit.thresmap_in5 = 0x0, /*Thres 0*/
		.THRESMAP_CFG0.bit.thresmap_in6 = 0x0, /*Thres 0*/
		.THRESMAP_CFG0.bit.thresmap_in7 = 0x0, /*Thres 0*/

		.THRESMAP_CFG1.bit.thresmap_in8 = 0x0, /*Thres 0*/
		.THRESMAP_CFG1.bit.thresmap_in9 = 0x0, /*Thres 0*/
		.THRESMAP_CFG1.bit.thresmap_in10 = 0x0, /*Thres 0*/
		.THRESMAP_CFG1.bit.thresmap_in11 = 0x0, /*Thres 0*/
		.THRESMAP_CFG1.bit.thresmap_in12_in17_thres2a = 0x0, /*Thres 0*/
		.THRESMAP_CFG1.bit.thresmap_in12_in17_thres2b = 0x0, /*Thres 0*/

		.THRESMAP_CFG2.bit.thresmap_in18_in23_thres3a = 0x0, /*Thres 0*/
		.THRESMAP_CFG2.bit.thresmap_in18_in23_thres3b = 0x0, /*Thres 0*/
		.THRESMAP_CFG2.bit.thresmap_in18_in23_thres3c = 0x0, /*Thres 0*/
		.THRESMAP_CFG2.bit.thresmap_vs0_thres2a = 0x0, /*Thres 0*/
		.THRESMAP_CFG2.bit.thresmap_vs0_thres2b = 0x0, /*Thres 0*/
		.THRESMAP_CFG2.bit.thresmap_vs1_thres2a = 0x0, /*Thres 0*/
		.THRESMAP_CFG2.bit.thresmap_vs1_thres2b = 0x0, /*Thres 0*/

		.MATRIX.bit.poll_act_time_m = 0x0,	/*Polling active time 64μs*/
		.MATRIX.bit.matrix = 0x0,			/*No matrix, regular inputs only*/
		.MATRIX.bit.thres_com = 0x0,		/*Value of threshold*/
		.MATRIX.bit.in_com_en = 0x0,		/*No interrupt generation for w.r.t. threshold*/

		.MODE.bit.m_in0 = 0x0,				/*Comparator mode for ResDI_4, IN_STAT_COMP*/
		.MODE.bit.m_in1 = 0x0,				/*Comparator mode for ResDI_2, IN_STAT_COMP*/
		.MODE.bit.m_in2 = 0x1,				/*ADC mode for NTC5,  ANA_STAT1*/
		.MODE.bit.m_in3 = 0x1,				/*ADC mode for NTC6,  ANA_STAT1*/
		.MODE.bit.m_in4 = 0x1,				/*ADC mode for NTC4,  ANA_STAT2*/
		.MODE.bit.m_in5 = 0x1,				/*ADC mode for NTC1,  ANA_STAT2*/
		.MODE.bit.m_in6 = 0x1,				/*ADC mode for NTC3,  ANA_STAT3*/
		.MODE.bit.m_in7 = 0x1,				/*ADC mode for NTC2,  ANA_STAT3*/
		.MODE.bit.m_in8 = 0x0,				/*Comparator mode for DI1, IN_STAT_COMP*/
		.MODE.bit.m_in9 = 0x0,				/*Comparator mode for DI2, IN_STAT_COMP*/
		.MODE.bit.m_in10 = 0x0,				/*Comparator mode for DI3, IN_STAT_COMP*/
		.MODE.bit.m_in11 = 0x0,				/*Comparator mode for DI4, IN_STAT_COMP*/
		.MODE.bit.m_in12 = 0x0,				/*Comparator mode for DI5, IN_STAT_COMP*/
		.MODE.bit.m_in13 = 0x0,				/*Comparator mode for DI6, IN_STAT_COMP*/
		.MODE.bit.m_in14 = 0x0,				/*Comparator mode for DI7, IN_STAT_COMP*/
		.MODE.bit.m_in15 = 0x0,				/*Comparator mode for DI8, IN_STAT_COMP*/
		.MODE.bit.m_in16 = 0x0,				/*Comparator mode for NC, IN_STAT_COMP*/
		.MODE.bit.m_in17 = 0x0,				/*Comparator mode for NC, IN_STAT_COMP*/
		.MODE.bit.m_in18 = 0x1,				/*ADC mode for 5VREF, ANA_STAT9*/
		.MODE.bit.m_in19 = 0x0,				/*Comparator mode for DI9, IN_STAT_COMP*/
		.MODE.bit.m_in20 = 0x0,				/*Comparator mode for DI10, IN_STAT_COMP*/
		.MODE.bit.m_in21 = 0x0,				/*Comparator mode for ResDI_10, IN_STAT_COMP*/
		.MODE.bit.m_in22 = 0x0,				/*Comparator mode for ResDI_8, IN_STAT_COMP*/
		.MODE.bit.m_in23 = 0x0,				/*Comparator mode for ResDI_6, IN_STAT_COMP*/

		.CONFIG_START.bit.reset = 0x0,			/*No reset*/
		.CONFIG_START.bit.poll_time = 0x2,		/*8ms(how frequently each input is sampled)*/
		.CONFIG_START.bit.poll_act_time = 0x7,	/*512μs(time for wetting current)*/
		.CONFIG_START.bit.crc_t = 0x0,			/*no CRC calculation triggered*/
		.CONFIG_START.bit.poll_en = 0x1,			/*Polling enabled*/
		.CONFIG_START.bit.trigger = 0x1,			/*Trigger TIC12400-Q1 from normal operation.*/
		.CONFIG_START.bit.int_config = 0x0,		/*INT pin assertion scheme set to static*/
		.CONFIG_START.bit.tw_cur_dis_cso = 0x0,	/*Enable CSO wetting current reduction*/
		.CONFIG_START.bit.det_filter = 0x1,		/*2 consecutive and equal samples required to be valid data*/
		.CONFIG_START.bit.tw_cur_dis_csi = 0x0,	/*Enable CSI wetting current reduction*/
		.CONFIG_START.bit.vs_meas_en = 0x1,		/*Enable VS measurement at the end of every polling cycle*/
		.CONFIG_START.bit.wet_d_in0_en = 0x0,		/*Disable wetting current diagnostic for input IN0*/
		.CONFIG_START.bit.wet_d_in1_en = 0x0,		/*Disable wetting current diagnostic for input IN1*/
		.CONFIG_START.bit.wet_d_in2_en = 0x0,		/*Disable wetting current diagnostic for input IN2*/
		.CONFIG_START.bit.wet_d_in3_en = 0x0,		/*Disable wetting current diagnostic for input IN3*/
		.CONFIG_START.bit.adc_diag_t = 0x0,		/*Disable ADC self-diagnostic feature*/
		.CONFIG_START.bit.vs_ratio = 0x1,			/*Use voltage divider factor of 10 for the VS measurement*/
};

const uint8_t tic124_settings_addr[TIC12400_SETTINGS_COUNT] = {
		TIC12400_CONFIG,
		TIC12400_IN_EN,
		TIC12400_CS_SELECT,
		TIC12400_WC_CFG0,
		TIC12400_WC_CFG1,
		TIC12400_CCP_CFG0,
		TIC12400_CCP_CFG1,
		TIC12400_THRES_COMP,
		TIC12400_INT_EN_COMP1,
		TIC12400_INT_EN_COMP2,
		TIC12400_INT_EN_CFG0,
		TIC12400_INT_EN_CFG1,
		TIC12400_INT_EN_CFG2,
		TIC12400_INT_EN_CFG3,
		TIC12400_INT_EN_CFG4,
		TIC12400_THRES_CFG0,
		TIC12400_THRES_CFG1,
		TIC12400_THRES_CFG2,
		TIC12400_THRES_CFG3,
		TIC12400_THRES_CFG4,
		TIC12400_THRESMAP_CFG0,
		TIC12400_THRESMAP_CFG1,
		TIC12400_THRESMAP_CFG2,
		TIC12400_MATRIX,
		TIC12400_MODE,
		TIC12400_CONFIG
};
