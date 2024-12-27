/*
 * rcc.c
 *
 *  Created on: 27 дек. 2024 г.
 *      Author: ruslan
 */

#include <stdio.h>
#include "stm32f4xx.h"

typedef struct _RCC_CR_BIT{
	unsigned hsi_on :1;			/*0		HSION*/
	unsigned hsi_rdy :1;		/*1		HSIRDY*/
	unsigned reserved_2 :1;		/*2		Reserved*/
	unsigned hsi_trim :5;		/*7:3	HSITRIM[4:0]*/
	unsigned hsi_cal :8;		/*15:8	HSICAL[7:0]*/
	unsigned hse_on: 1;			/*16	HSEON*/
	unsigned hse_rdy :1;		/*17	HSERDY*/
	unsigned hse_byp :1;		/*18	HSEBYP*/
	unsigned css_on :1;			/*19	CSSON*/
	unsigned reserved_20_23 :4;	/*23:20 Reserved*/
	unsigned pll_on :1;			/*24	PLLON*/
	unsigned pll_rdy :1;		/*25	PLLRDY*/
	unsigned pll_i2s_on :1;		/*26	PLLI2SON*/
	unsigned pll_i2s_rdy :1;	/*27	PLLI2SRDY*/
	unsigned pll_sai_on :1;		/*28	PLLSAION:*/
	unsigned pll_sai_rdy :1;	/*29	PLLSAIRDY*/
	unsigned reserved_30_31 :2;	/*31:30	Reserved*/
} rcc_cr_bit_t;

typedef union {
	uint32_t all;
	struct _RCC_CR_BIT bit;
} rcc_cr_reg_t;

typedef struct _RCC_PLLCFGR_BIT {
	unsigned pll_m :6;
	unsigned pll_n :9;
	unsigned reserved_15: 1;
	unsigned pll_p :2;
	unsigned reserved_18_21 :4;
	unsigned pll_src :1;
	unsigned reserved_23 :1;
	unsigned pll_q :4;
	unsigned reserved_28_31 :4;
} rcc_pllcfgr_bit_t;

typedef union {
	uint32_t all;
	struct _RCC_PLLCFGR_BIT bit;
} rcc_pllcfgr_reg_t;

typedef struct _RCC_CFGR_BIT {
	unsigned sw :2;				/*1:0 SW*/
	unsigned sws :2;			/*3:2 SWS*/
	unsigned hpre :4;			/*7:4 HPRE*/
	unsigned reserved_8_9 :2;	/*9:8 Reserved*/
	unsigned p_pre1 :3;			/*12:10 PPRE1*/
	unsigned p_pre2 :3;			/*15:13 PPRE2*/
	unsigned rtc_pre :5;		/*20:16 RTCPRE*/
	unsigned mco1 :2;			/*22:21 MCO1*/
	unsigned i2s_src :1;		/*23 I2SSRC*/
	unsigned mco1_pre :3;		/*26:24 MCO1PRE*/
	unsigned mco2_pre :3;		/*29:27 MCO2PRE*/
	unsigned mco2 :2;			/*31:30 MCO2*/
} rcc_cfgr_bit_t;

typedef union {
	uint32_t all;
	struct _RCC_CFGR_BIT bit;
} rcc_cfgr_reg_t;

void print_rcc_info() {
	rcc_cr_reg_t rcc_cr;
	rcc_cr.all = RCC->CR;

	printf("RCC_CR: ");
	printf("HSION %i, ", rcc_cr.bit.hsi_on);
	printf("HSIRDY %i, ", rcc_cr.bit.hsi_rdy);
	printf("HSITRIM %i, ", rcc_cr.bit.hsi_trim);
	printf("HSICAL %i, ", rcc_cr.bit.hsi_cal);
	printf("HSEON %i, ", rcc_cr.bit.hse_on);
	printf("HSERDY %i, ", rcc_cr.bit.hse_rdy);
	printf("HSEBYP %i, ", rcc_cr.bit.hse_byp);
	printf("CSSON %i, ", rcc_cr.bit.css_on);
	printf("PLLON %i, ", rcc_cr.bit.pll_on);
	printf("PLLRDY %i, ", rcc_cr.bit.pll_rdy);
	printf("PLLI2SON %i, ", rcc_cr.bit.pll_i2s_on);
	printf("PLLI2SRDY %i, ", rcc_cr.bit.pll_i2s_rdy);
	printf("PLLSAION %i, ", rcc_cr.bit.pll_sai_on);
	printf("PLLSAIRDY %i\n", rcc_cr.bit.pll_sai_rdy);

	rcc_pllcfgr_reg_t rcc_pllcfgr;
	rcc_pllcfgr.all = RCC->PLLCFGR;

	printf("RCC_PLLCFGR: ");
	printf("PLLM %i, ", rcc_pllcfgr.bit.pll_m);
	printf("PLLN %i, ", rcc_pllcfgr.bit.pll_n);
	printf("PLLP %i, ", rcc_pllcfgr.bit.pll_p);
	printf("PLLSRC %i, ", rcc_pllcfgr.bit.pll_src);
	printf("PLLQ %i\n", rcc_pllcfgr.bit.pll_q);

	rcc_cfgr_reg_t rcc_cfgr;
	rcc_cfgr.all = RCC->CFGR;

	printf("RCC_CFGR: ");
	printf("SW %i, ", rcc_cfgr.bit.sw);
	printf("SWS %i, ", rcc_cfgr.bit.sws);
	printf("HPRE %i, ", rcc_cfgr.bit.hpre);
	printf("PPRE1 %i, ", rcc_cfgr.bit.p_pre1);
	printf("PPRE2 %i, ", rcc_cfgr.bit.p_pre2);
	printf("RTCPRE %i, ", rcc_cfgr.bit.rtc_pre);
	printf("MCO1 %i, ", rcc_cfgr.bit.mco1);
	printf("I2SSRC %i, ", rcc_cfgr.bit.i2s_src);
	printf("MCO1PRE %i, ", rcc_cfgr.bit.mco1_pre);
	printf("MCO2PRE %i, ", rcc_cfgr.bit.mco2_pre);
	printf("MCO2 %i\n", rcc_cfgr.bit.mco2);
}
