/*
 * tic12400_reg.h
 *
 *  Created on: 6 июн. 2023 г.
 *      Author: Ruslan
 */

#ifndef TIC12400_TIC12400_REG_H_
#define TIC12400_TIC12400_REG_H_

#include <stdint.h>
#include <assert.h>
#include "lib/defs/defs.h"

#define TIC12400_STATUS_SIZE 1
#define TIC12400_DATA_SIZE 3
#define TIC12400_FRAME_SIZE 4


// tic12400_
// TIC12400_

//0x1h DEVICE_ID
//0x2h INT_STAT
//0x3h CRC
//0x4h IN_STAT_MISC
//0x5h IN_STAT_COMP
//0x6h-7h IN_STAT_ADC0, IN_STAT_ADC1
//0x8h-9h IN_STAT_MATRIX0, IN_STAT_MATRIX1
//0xAh-16h ANA_STAT0- ANA_STAT12
//0x1Ah CONFIG
//0x1Bh IN_EN
//0x1Ch CS_SELECT
//0x1Dh-1Eh WC_CFG0, WC_CFG1
//0x1Fh-20h CCP_CFG0, CCP_CFG1
//0x21h THRES_COMP
//0x22h-23h INT_EN_COMP1, INT_EN_COMP2
//0x24h INT_EN_CFG0
//0x25h-28h INT_EN_CFG1- INT_EN_CFG4
//0x29h-2Dh THRES_CFG0- THRES_CFG4
//0x2Eh-30h THRESMAP_CFG0- THRESMAP_CFG2
//0x31h MATRIX
//0x32h MODE

enum TIC12400_ADDR {
	TIC12400_RESERVED0,

	TIC12400_DEVICE_ID,
	TIC12400_INT_STAT,
	TIC12400_CRC,
	TIC12400_IN_STAT_MISC,
	TIC12400_IN_STAT_COMP,
	TIC12400_IN_STAT_ADC0,
	TIC12400_IN_STAT_ADC1,
	TIC12400_IN_STAT_MATRIX0,
	TIC12400_IN_STAT_MATRIX1,
	TIC12400_ANA_STAT0,
	TIC12400_ANA_STAT1,
	TIC12400_ANA_STAT2,
	TIC12400_ANA_STAT3,
	TIC12400_ANA_STAT4,
	TIC12400_ANA_STAT5,
	TIC12400_ANA_STAT6,
	TIC12400_ANA_STAT7,
	TIC12400_ANA_STAT8,
	TIC12400_ANA_STAT9,
	TIC12400_ANA_STAT10,
	TIC12400_ANA_STAT11,
	TIC12400_ANA_STAT12,

	TIC12400_RESERVED23,
	TIC12400_RESERVED24,
	TIC12400_RESERVED25,

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
	TIC12400_MODE
};

//структура данных Device ID
typedef struct PACKED _TIC12400_DEVICE_ID_BIT {
	unsigned minor :4;
	unsigned major :7;
	unsigned reserved :13;
} tic12400_dev_id_data_t;
static_assert(sizeof(tic12400_dev_id_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_dev_id_data_t!");

typedef union _TIC12400_DEVICE_ID_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_DEVICE_ID_BIT bit;
} TIC12400_DEVICE_ID_REG;

//структура данных Interrupt Status
typedef struct PACKED _TIC12400_INT_STAT_BIT {
	unsigned por :1;
	unsigned spi_fail :1;
	unsigned par_fail :1;
	unsigned ssc :1;
	unsigned tsd :1;
	unsigned tw :1;
	unsigned ov :1;
	unsigned uv :1;
	unsigned crc_calc :1;
	unsigned vs0 :1;
	unsigned vs1 :1;
	unsigned wet_diag :1;
	unsigned adc_diag :1;
	unsigned chk_fail :1;
	unsigned reserved :10;
} tic12400_int_stat_data_t;
static_assert(sizeof(tic12400_int_stat_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_int_stat_data_t!");

typedef union _TIC12400_INT_STAT_BITS_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_INT_STAT_BIT bit;
} TIC12400_INT_STAT_REG;

//структура данных CRC
typedef struct PACKED _TIC12400_CRC_BIT {
	unsigned crc :16;
	unsigned reserved :8;
} tic12400_crc_data_t;
static_assert(sizeof(tic12400_crc_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_crc_t!");

typedef union _TIC12400_CRC_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_CRC_BIT bit;
} TIC12400_CRC_REG;

//структура данных Miscellaneous Status
typedef struct PACKED _TIC12400_IN_STAT_MISC_BIT {
	unsigned tsd_stat :1;
	unsigned tw_stat :1;
	unsigned ov_stat :1;
	unsigned uv_stat :1;
	unsigned vs0_stat :2;
	unsigned vs1_stat :2;
	unsigned in0_d :1;
	unsigned in1_d :1;
	unsigned in2_d :1;
	unsigned in3_d :1;
	unsigned adc_d :1;
	unsigned reserved :11;
} tic12400_in_stat_misc_data_t;
static_assert(sizeof(tic12400_in_stat_misc_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_in_stat_misc_data_t!");

typedef union _TIC12400_IN_STAT_MISC_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_IN_STAT_MISC_BIT bit;
} TIC12400_IN_STAT_MISC_REG;

//структура данных Comparator Status
typedef struct PACKED _TIC12400_IN_STAT_COMP_BIT {
	unsigned inc_0 :1;
	unsigned inc_1 :1;
	unsigned inc_2 :1;
	unsigned inc_3 :1;
	unsigned inc_4 :1;
	unsigned inc_5 :1;
	unsigned inc_6 :1;
	unsigned inc_7 :1;
	unsigned inc_8 :1;
	unsigned inc_9 :1;
	unsigned inc_10 :1;
	unsigned inc_11 :1;
	unsigned inc_12 :1;
	unsigned inc_13 :1;
	unsigned inc_14 :1;
	unsigned inc_15 :1;
	unsigned inc_16 :1;
	unsigned inc_17 :1;
	unsigned inc_18 :1;
	unsigned inc_19 :1;
	unsigned inc_20 :1;
	unsigned inc_21 :1;
	unsigned inc_22 :1;
	unsigned inc_23 :1;
} tic12400_in_stat_comp_data_t;
static_assert(sizeof(tic12400_in_stat_comp_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_in_stat_comp_data_t!");

typedef union _TIC12400_IN_STAT_COMP_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_IN_STAT_COMP_BIT bit;
} TIC12400_IN_STAT_COMP_REG;

//структура данных ADC0 Status
typedef struct PACKED _TIC12400_IN_STAT_ADC0_BIT {
	unsigned ina_0 :1;
	unsigned ina_1 :1;
	unsigned ina_2 :1;
	unsigned ina_3 :1;
	unsigned ina_4 :1;
	unsigned ina_5 :1;
	unsigned ina_6 :1;
	unsigned ina_7 :1;
	unsigned ina_8 :1;
	unsigned ina_9 :1;
	unsigned ina_10 :1;
	unsigned ina_11 :1;
	unsigned ina_12 :2;
	unsigned ina_13 :2;
	unsigned ina_14 :2;
	unsigned ina_15 :2;
	unsigned ina_16 :2;
	unsigned ina_17 :2;
} tic12400_in_stat_adc0_data_t;
static_assert(sizeof(tic12400_in_stat_adc0_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_in_stat_adc0_data_t!");

typedef union _TIC12400_IN_STAT_ADC0_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_IN_STAT_ADC0_BIT bit;
} TIC12400_IN_STAT_ADC0_REG;

//структура данных ADC1 Status
typedef struct PACKED _TIC12400_IN_STAT_ADC1_BIT {
	unsigned ina_18 :2;
	unsigned ina_19 :2;
	unsigned ina_20 :2;
	unsigned ina_21 :2;
	unsigned ina_22 :2;
	unsigned ina_23 :3;
	unsigned reserved :11;
} tic12400_in_stat_adc1_data_t;
static_assert(sizeof(tic12400_in_stat_adc1_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_in_stat_adc1_data_t!");

typedef union _TIC12400_IN_STAT_ADC1_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_IN_STAT_ADC1_BIT bit;
} TIC12400_IN_STAT_ADC1_REG;

//структура данных Matrix0 Status
typedef struct PACKED _TIC12400_IN_STAT_MATRIX0_BIT {
	unsigned inmat_10_in4 :1;
	unsigned inmat_10_in5 :1;
	unsigned inmat_10_in6 :1;
	unsigned inmat_10_in7 :1;
	unsigned inmat_10_in8 :1;
	unsigned inmat_10_in9 :1;
	unsigned inmat_11_in4 :1;
	unsigned inmat_11_in5 :1;
	unsigned inmat_11_in6 :1;
	unsigned inmat_11_in7 :1;
	unsigned inmat_11_in8 :1;
	unsigned inmat_11_in9 :1;
	unsigned inmat_12_in4 :1;
	unsigned inmat_12_in5 :1;
	unsigned inmat_12_in6 :1;
	unsigned inmat_12_in7 :1;
	unsigned inmat_12_in8 :1;
	unsigned inmat_12_in9 :1;
	unsigned inmat_13_in4 :1;
	unsigned inmat_13_in5 :1;
	unsigned inmat_13_in6 :1;
	unsigned inmat_13_in7 :1;
	unsigned inmat_13_in8 :1;
	unsigned inmat_13_in9 :1;
} tic12400_in_stat_matrix0_data_t;
static_assert(sizeof(tic12400_in_stat_matrix0_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_in_stat_matrix0_data_t!");

typedef union _TIC12400_IN_STAT_MATRIX0_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_IN_STAT_MATRIX0_BIT bit;
} TIC12400_IN_STAT_MATRIX0_REG;

//структура данных Matrix1 Status
typedef struct PACKED _TIC12400_IN_STAT_MATRIX1_BIT {
	unsigned inmat_14_in4 :1;
	unsigned inmat_14_in5 :1;
	unsigned inmat_14_in6 :1;
	unsigned inmat_14_in7 :1;
	unsigned inmat_14_in8 :1;
	unsigned inmat_14_in9 :1;
	unsigned inmat_15_in4 :1;
	unsigned inmat_15_in5 :1;
	unsigned inmat_15_in6 :1;
	unsigned inmat_15_in7 :1;
	unsigned inmat_15_in8 :1;
	unsigned inmat_15_in9 :1;
	unsigned in0_com :1;
	unsigned in1_com :1;
	unsigned in2_com :1;
	unsigned in3_com :1;
	unsigned in4_com :1;
	unsigned in5_com :1;
	unsigned in6_com :1;
	unsigned in7_com :1;
	unsigned in8_com :1;
	unsigned in9_com :1;
	unsigned in10_com :1;
	unsigned in11_com :1;
} tic12400_in_stat_matrix1_data_t;
static_assert(sizeof(tic12400_in_stat_matrix1_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_in_stat_matrix1_data_t!");

typedef union _TIC12400_IN_STAT_MATRIX1_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_IN_STAT_MATRIX1_BIT bit;
} TIC12400_IN_STAT_MATRIX1_REG;

//структура данных ADC Raw Code
typedef struct PACKED _TIC12400_ANA_STAT_BIT {
	unsigned in0_ana :10;
	unsigned in1_ana :10;
	unsigned reserved :4;
} tic12400_ana_stat_data_t;
static_assert(sizeof(tic12400_ana_stat_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_ana_stat_data_t!");

typedef union _TIC12400_ANA_STAT_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_ANA_STAT_BIT bit;
} TIC12400_ANA_STAT_REG;

//структура данных Device Global Configuration
typedef struct PACKED _TIC12400_CONFIG_BIT {
	unsigned reset :1;
	unsigned poll_time :4;
	unsigned poll_act_time :4;
	unsigned crc_t :1;
	unsigned poll_en :1;
	unsigned trigger :1;
	unsigned int_config :1;
	unsigned tw_cur_dis_cso :1;
	unsigned det_filter :2;
	unsigned tw_cur_dis_csi :1;
	unsigned vs_meas_en :1;
	unsigned wet_d_in0_en :1;
	unsigned wet_d_in1_en :1;
	unsigned wet_d_in2_en :1;
	unsigned wet_d_in3_en :1;
	unsigned adc_diag_t :1;
	unsigned vs_ratio :1;
} tic12400_config_data_t;
static_assert(sizeof(tic12400_config_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_config_data_t!");

typedef union _TIC12400_CONFIG_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_CONFIG_BIT bit;
} TIC12400_CONFIG_REG;

//структура данных Input Enable
typedef struct PACKED _TIC12400_IN_EN_BIT {
	unsigned in_en_0 :1;
	unsigned in_en_1 :1;
	unsigned in_en_2 :1;
	unsigned in_en_3 :1;
	unsigned in_en_4 :1;
	unsigned in_en_5 :1;
	unsigned in_en_6 :1;
	unsigned in_en_7 :1;
	unsigned in_en_8 :1;
	unsigned in_en_9 :1;
	unsigned in_en_10 :1;
	unsigned in_en_11 :1;
	unsigned in_en_12 :1;
	unsigned in_en_13 :1;
	unsigned in_en_14 :1;
	unsigned in_en_15 :1;
	unsigned in_en_16 :1;
	unsigned in_en_17 :1;
	unsigned in_en_18 :1;
	unsigned in_en_19 :1;
	unsigned in_en_20 :1;
	unsigned in_en_21 :1;
	unsigned in_en_22 :1;
	unsigned in_en_23 :1;
} tic12400_in_en_data_t;
static_assert(sizeof(tic12400_in_en_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_in_en_data_t!");

typedef union _TIC12400_IN_EN_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_IN_EN_BIT bit;
} TIC12400_IN_EN_REG;

//структура данных Current Source/Sink Selection
typedef struct PACKED _TIC12400_CS_SELECT_BIT {
	unsigned cs_in0 :1;
	unsigned cs_in1 :1;
	unsigned cs_in2 :1;
	unsigned cs_in3 :1;
	unsigned cs_in4 :1;
	unsigned cs_in5 :1;
	unsigned cs_in6 :1;
	unsigned cs_in7 :1;
	unsigned cs_in8 :1;
	unsigned cs_in9 :1;
	unsigned reserved :14;
} tic12400_cs_select_data_t;
static_assert(sizeof(tic12400_cs_select_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_cs_select_data_t!");

typedef union _TIC12400_CS_SELECT_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_CS_SELECT_BIT bit;
} TIC12400_CS_SELECT_REG;

//структура данных Wetting Current Configuration 0
typedef struct PACKED _TIC12400_WC_CFG0_BIT {
	unsigned wc_in0_in1 :3;
	unsigned wc_in2_in3 :3;
	unsigned wc_in4 :3;
	unsigned wc_in5 :3;
	unsigned wc_in6_in7 :3;
	unsigned wc_in8_in9 :3;
	unsigned wc_in10 :3;
	unsigned wc_in11 :3;
} tic12400_wc_cfg0_data_t;
static_assert(sizeof(tic12400_wc_cfg0_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_wc_cfg0_data_t!");

typedef union _TIC12400_WC_CFG0_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_WC_CFG0_BIT bit;
} TIC12400_WC_CFG0_REG;

//структура данных Wetting Current Configuration 1
typedef struct PACKED _TIC12400_WC_CFG1_BIT {
	unsigned wc_in12_in13 :3;
	unsigned wc_in14_in15 :3;
	unsigned wc_in16_in17 :3;
	unsigned wc_in18_in19 :3;
	unsigned wc_in20_in21 :3;
	unsigned wc_in22 :3;
	unsigned wc_in23 :3;
	unsigned auto_scale_dis_cso :1;
	unsigned auto_scale_dis_csi :1;
	unsigned reserved :1;
} tic12400_wc_cfg1_data_t;
static_assert(sizeof(tic12400_wc_cfg1_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_wc_cfg1_data_t!");

typedef union _TIC12400_WC_CFG1_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_WC_CFG1_BIT bit;
} TIC12400_WC_CFG1_REG;

//структура данных Clean Current Polling 0
typedef struct PACKED _TIC12400_CCP_CFG0_BIT {
	unsigned wc_ccp0 :1;
	unsigned wc_ccp1 :1;
	unsigned wc_ccp2 :1;
	unsigned wc_ccp3 :1;
	unsigned ccp_time :3;
	unsigned reserved :17;
} tic12400_ccp_cfg0_data_t;
static_assert(sizeof(tic12400_ccp_cfg0_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_ccp_cfg0_data_t!");

typedef union _TIC12400_CCP_CFG0_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_CCP_CFG0_BIT bit;
} TIC12400_CCP_CFG0_REG;

//структура данных Clean Current Polling 1
typedef struct PACKED _TIC12400_CCP_CFG1_BIT {
	unsigned ccp_in0 :1;
	unsigned ccp_in1 :1;
	unsigned ccp_in2 :1;
	unsigned ccp_in3 :1;
	unsigned ccp_in4 :1;
	unsigned ccp_in5 :1;
	unsigned ccp_in6 :1;
	unsigned ccp_in7 :1;
	unsigned ccp_in8 :1;
	unsigned ccp_in9 :1;
	unsigned ccp_in10 :1;
	unsigned ccp_in11 :1;
	unsigned ccp_in12 :1;
	unsigned ccp_in13 :1;
	unsigned ccp_in14 :1;
	unsigned ccp_in15 :1;
	unsigned ccp_in16 :1;
	unsigned ccp_in17 :1;
	unsigned ccp_in18 :1;
	unsigned ccp_in19 :1;
	unsigned ccp_in20 :1;
	unsigned ccp_in21 :1;
	unsigned ccp_in22 :1;
	unsigned ccp_in23 :1;
} tic12400_ccp_cfg1_data_t;
static_assert(sizeof(tic12400_ccp_cfg1_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_ccp_cfg1_data_t!");

typedef union _TIC12400_CCP_CFG1_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_CCP_CFG1_BIT bit;
} TIC12400_CCP_CFG1_REG;

//структура данных Comparator Threshold Control
typedef struct PACKED _TIC12400_THRES_COMP_BIT {
	unsigned thres_comp_in0_in3 :2;
	unsigned thres_comp_in4_in7 :2;
	unsigned thres_comp_in8_in11 :2;
	unsigned thres_comp_in12_in15 :2;
	unsigned thres_comp_in16_in19 :2;
	unsigned thres_comp_in20_in23 :2;
	unsigned reserved :12;
} tic12400_thres_comp_data_t;
static_assert(sizeof(tic12400_thres_comp_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_thres_comp_data_t!");

typedef union _TIC12400_THRES_COMP_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_THRES_COMP_BIT bit;
} TIC12400_THRES_COMP_REG;

//структура данных Comparator Input Interrupt Generation 1
typedef struct PACKED _TIC12400_INT_EN_COMP1_BIT {
	unsigned inc_en_0 :2;
	unsigned inc_en_1 :2;
	unsigned inc_en_2 :2;
	unsigned inc_en_3 :2;
	unsigned inc_en_4 :2;
	unsigned inc_en_5 :2;
	unsigned inc_en_6 :2;
	unsigned inc_en_7 :2;
	unsigned inc_en_8 :2;
	unsigned inc_en_9 :2;
	unsigned inc_en_10 :2;
	unsigned inc_en_11 :2;
} tic12400_ent_en_comp1_data_t;
static_assert(sizeof(tic12400_ent_en_comp1_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_ent_en_comp1_data_t!");

typedef union _TIC12400_INT_EN_COMP1_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_INT_EN_COMP1_BIT bit;
} TIC12400_INT_EN_COMP1_REG;

//структура данных Comparator Input Interrupt Generation 2
typedef struct PACKED _TIC12400_INT_EN_COMP2_BIT {
	unsigned inc_en_12 :2;
	unsigned inc_en_13 :2;
	unsigned inc_en_14 :2;
	unsigned inc_en_15 :2;
	unsigned inc_en_16 :2;
	unsigned inc_en_17 :2;
	unsigned inc_en_18 :2;
	unsigned inc_en_19 :2;
	unsigned inc_en_20 :2;
	unsigned inc_en_21 :2;
	unsigned inc_en_22 :2;
	unsigned inc_en_23 :2;
} tic12400_ent_en_comp2_data_t;
static_assert(sizeof(tic12400_ent_en_comp2_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_ent_en_comp2_data_t!");

typedef union _TIC12400_INT_EN_COMP2_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_INT_EN_COMP2_BIT bit;
} TIC12400_INT_EN_COMP2_REG;

//структура данных Global Interrupt Generation Control 0
typedef struct PACKED _TIC12400_INT_EN_CFG0_BIT {
	unsigned spi_fail_en :1;
	unsigned prty_fail_en :1;
	unsigned ssc_en :1;
	unsigned tsd_en :1;
	unsigned tw_en :1;
	unsigned ov_en :1;
	unsigned uv_en :1;
	unsigned crc_calc_en :1;
	unsigned vs0_en :1;
	unsigned vs1_en :1;
	unsigned wet_diag_en :1;
	unsigned adc_diag_en :1;
	unsigned reserved :12;
} tic12400_int_en_cfg0_data_t;
static_assert(sizeof(tic12400_int_en_cfg0_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_int_en_cfg0_data_t!");

typedef union _TIC12400_INT_EN_CFG0_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_INT_EN_CFG0_BIT bit;
} TIC12400_INT_EN_CFG0_REG;

//структура данных ADC Input Interrupt Generation Control 1
typedef struct PACKED _TIC12400_INT_EN_CFG1_BIT {
	unsigned in0_en :2;
	unsigned in1_en :2;
	unsigned in2_en :2;
	unsigned in3_en :2;
	unsigned in4_en :2;
	unsigned in5_en :2;
	unsigned in6_en :2;
	unsigned in7_en :2;
	unsigned in8_en :2;
	unsigned in9_en :2;
	unsigned in10_en :2;
	unsigned in11_en :2;
} tic12400_int_en_cfg1_data_t;
static_assert(sizeof(tic12400_int_en_cfg1_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_int_en_cfg1_data_t!");

typedef union _TIC12400_INT_EN_CFG1_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_INT_EN_CFG1_BIT bit;
} TIC12400_INT_EN_CFG1_REG;

//структура данных ADC Input Interrupt Generation Control 2
typedef struct PACKED _TIC12400_INT_EN_CFG2_BIT {
	unsigned in12_en :4;
	unsigned in13_en :4;
	unsigned in14_en :4;
	unsigned in15_en :4;
	unsigned in16_en :4;
	unsigned in17_en :4;
} tic12400_int_en_cfg2_data_t;
static_assert(sizeof(tic12400_int_en_cfg2_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_int_en_cfg2_data_t!");

typedef union _TIC12400_INT_EN_CFG2_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_INT_EN_CFG2_BIT bit;
} TIC12400_INT_EN_CFG2_REG;

//структура данных ADC Input Interrupt Generation Control 3
typedef struct PACKED _TIC12400_INT_EN_CFG3_BIT {
	unsigned in18_en :6;
	unsigned in19_en :6;
	unsigned in20_en :6;
	unsigned in21_en :6;
} tic12400_int_en_cfg3_data_t;
static_assert(sizeof(tic12400_int_en_cfg3_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_int_en_cfg3_data_t!");

typedef union _TIC12400_INT_EN_CFG3_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_INT_EN_CFG3_BIT bit;
} TIC12400_INT_EN_CFG3_REG;

//структура данных ADC Input Interrupt Generation Control 4
typedef struct PACKED _TIC12400_INT_EN_CFG4_BIT {
	unsigned in22_en :6;
	unsigned in23_en :10;
	unsigned vs_th0_en :4;
	unsigned vs_th1_en :4;
} tic12400_int_en_cfg4_data_t;
static_assert(sizeof(tic12400_int_en_cfg4_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_int_en_cfg4_data_t!");

typedef union _TIC12400_INT_EN_CFG4_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_INT_EN_CFG4_BIT bit;
} TIC12400_INT_EN_CFG4_REG;

//структура данных ADC Threshold Control
typedef struct PACKED _TIC12400_THRES_CFG0_BIT {
	unsigned thres0 :10;
	unsigned thres1 :10;
	unsigned reserved :4;
} tic12400_thres_cfg_data_t;
static_assert(sizeof(tic12400_thres_cfg_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_thres_cfg_data_t!");

typedef union _TIC12400_THRES_CFG0_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_THRES_CFG0_BIT bit;
} TIC12400_THRES_CFG_REG;

//структура данных ADC Threshold Mapping 0
typedef struct PACKED _TIC12400_THRESMAP_CFG0_BIT {
	unsigned thresmap_in0 :3;
	unsigned thresmap_in1 :3;
	unsigned thresmap_in2 :3;
	unsigned thresmap_in3 :3;
	unsigned thresmap_in4 :3;
	unsigned thresmap_in5 :3;
	unsigned thresmap_in6 :3;
	unsigned thresmap_in7 :3;
} tic12400_thresmap_cfg0_data_t;
static_assert(sizeof(tic12400_thresmap_cfg0_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_thresmap_cfg0_data_t!");

typedef union _TIC12400_THRESMAP_CFG0_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_THRESMAP_CFG0_BIT bit;
} TIC12400_THRESMAP_CFG0_REG;

//структура данных ADC Threshold Mapping 1
typedef struct PACKED _TIC12400_THRESMAP_CFG1_BIT {
	unsigned thresmap_in8 :3;
	unsigned thresmap_in9 :3;
	unsigned thresmap_in10 :3;
	unsigned thresmap_in11 :3;
	unsigned thresmap_in12_in17_thres2a :3;
	unsigned thresmap_in12_in17_thres2b :3;
	unsigned reservred :6;
} tic12400_thresmap_cfg1_data_t;
static_assert(sizeof(tic12400_thresmap_cfg1_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_thresmap_cfg1_data_t!");

typedef union _TIC12400_THRESMAP_CFG1_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_THRESMAP_CFG1_BIT bit;
} TIC12400_THRESMAP_CFG1_REG;

//структура данных ADC Threshold Mapping 2
typedef struct PACKED _TIC12400_THRESMAP_CFG2_BIT {
	unsigned thresmap_in18_in23_thres3a :3;
	unsigned thresmap_in18_in23_thres3b :3;
	unsigned thresmap_in18_in23_thres3c :3;
	unsigned thresmap_vs0_thres2a :3;
	unsigned thresmap_vs0_thres2b :3;
	unsigned thresmap_vs1_thres2a :3;
	unsigned thresmap_vs1_thres2b :3;
	unsigned reserved :3;
} tic12400_thresmap_cfg2_data_t;
static_assert(sizeof(tic12400_thresmap_cfg2_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_thresmap_cfg2_data_t!");

typedef union _TIC12400_THRESMAP_CFG2_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_THRESMAP_CFG2_BIT bit;
} TIC12400_THRESMAP_CFG2_REG;

//структура данных Matrix Setting
typedef struct PACKED _TIC12400_MATRIX_BIT {
	unsigned poll_act_time_m :3;
	unsigned matrix :2;
	unsigned thres_com :10;
	unsigned in_com_en :2;
	unsigned reserved :7;
} tic12400_matrix_data_t;
static_assert(sizeof(tic12400_matrix_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_matrix_data_t!");

typedef union _TIC12400_MATRIX_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_MATRIX_BIT bit;
} TIC12400_MATRIX_REG;

//структура данных Mode Setting
typedef struct PACKED _TIC12400_MODE_BIT {
	unsigned m_in0 :1;
	unsigned m_in1 :1;
	unsigned m_in2 :1;
	unsigned m_in3 :1;
	unsigned m_in4 :1;
	unsigned m_in5 :1;
	unsigned m_in6 :1;
	unsigned m_in7 :1;
	unsigned m_in8 :1;
	unsigned m_in9 :1;
	unsigned m_in10 :1;
	unsigned m_in11 :1;
	unsigned m_in12 :1;
	unsigned m_in13 :1;
	unsigned m_in14 :1;
	unsigned m_in15 :1;
	unsigned m_in16 :1;
	unsigned m_in17 :1;
	unsigned m_in18 :1;
	unsigned m_in19 :1;
	unsigned m_in20 :1;
	unsigned m_in21 :1;
	unsigned m_in22 :1;
	unsigned m_in23 :1;
} tic12400_mode_data_t;
static_assert(sizeof(tic12400_mode_data_t) == TIC12400_DATA_SIZE, "Invalid size of tic12400_mode_data_t!");

typedef union _TIC12400_MODE_REG {
	uint32_t all;
	unsigned all24 :24;
	struct _TIC12400_MODE_BIT bit;
} TIC12400_MODE_REG;

#endif /* TIC12400_TIC12400_REG_H_ */
