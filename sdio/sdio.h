/*
 * sdio.h
 *
 *  Created on: 25 дек. 2024 г.
 *      Author: ruslan
 */

#ifndef SDIO_SDIO_H_
#define SDIO_SDIO_H_

#include "stm32f4xx.h"

#define SDIO_ACCESS_WIDTH 4

typedef enum {
	SDIO_POWER_PWRCTRL_OFF = 0b00,
	SDIO_POWER_PWRCTRL_RES_UP = 0b10,
	SDIO_POWER_PWRCTRL_ON = 0b11
} sdio_pwrctrl_t;

typedef struct _sdio_power_bit {
	unsigned pwrctrl :2;
	unsigned reserved :30;
} _sdio_power_bit_t;
static_assert(sizeof(_sdio_power_bit_t) == SDIO_ACCESS_WIDTH, "Invalid size of _sdio_power_bit_t!");

typedef union _sdio_power_reg {
	uint32_t all;
	struct _sdio_power_bit bit;
} _sdio_power_reg_t;

typedef enum {
	SDIO_CLKCR_CLK_DIS = 0b0,
	SDIO_CLKCR_CLK_EN = 0b1
} sdio_clken_t;

typedef enum {
	SDIO_CLKCR_PWRSAV_DIS = 0b0,
	SDIO_CLKCR_PWRSAV_ENA = 0b1
} sdio_pwrsav_t;

typedef enum {
	SDIO_CLKCR_BYP_DIS = 0b0,
	SDIO_CLKCR_BYP_ENA = 0b1
} sdio_bypass_t;

typedef enum {
	SDIO_CLKCR_BUS_DEF = 0b00,
	SDIO_CLKCR_BUS_4b = 0b01,
	SDIO_CLKCR_BUS_8b = 0b10
} sdio_widbus_t;

typedef enum {
	SDIO_CLKCR_EDGE_RISING = 0b0,
	SDIO_CLKCR_EDGE_FALLING = 0b1
} sdio_negedge_t;

typedef enum {
	SDIO_CLKCR_HWFC_DIS = 0b0,
	SDIO_CLKCR_HWFC_ENA = 0b1
} sdio_hwfc_t;

typedef struct _sdio_clkcr_bit {
	unsigned clkdiv :8;
	unsigned clken :1;
	unsigned pwrsav :1;
	unsigned bypass :1;
	unsigned widbus :2;
	unsigned negedge :1;
	unsigned hwfc_en: 1;
	unsigned reserved :17;
} _sdio_clkcr_bit_t;
static_assert(sizeof(_sdio_clkcr_bit_t) == SDIO_ACCESS_WIDTH, "Invalid size of _sdio_clkcr_bit_t!");

typedef union _sdio_clkcr_reg {
	uint32_t all;
	struct _sdio_clkcr_bit bit;
} _sdio_clkcr_reg_t;

typedef enum {
	SDIO_RESP_WAIT_DIS = 0b0,
	SDIO_RESP_WAIT_ENA = 0b1
} sdio_respwait_t;

typedef enum {
	SDIO_RESP_TYPE_SHORT = 0b0,
	SDIO_RESP_TYPE_LONG = 0b1
} sdio_resptype_t;

typedef enum {
	SDIO_INT_WAIT_DIS = 0b0,
	SDIO_INT_WAIT_ENA = 0b1
} sdio_intwait_t;

typedef enum {
	SDIO_PEND_WAIT_DIS = 0b0,
	SDIO_PEND_WAIT_ENA = 0b1
} sdio_pendwait_t;

typedef enum {
	SDIO_CPSM_DIS = 0b0,
	SDIO_CPSM_EN = 0b1
} sdio_cpsmen_t;

typedef enum {
	SDIO_SUSPEND_DIS = 0b0,
	SDIO_SUSPEND_ENA = 0b1
} sdio_suspend_t;

typedef enum {
	SDIO_CMD_COMPLETION_DIS = 0b0,
	SDIO_CMD_COMPLETION_ENA = 0b1
} sdio_cmdcompl_t;

typedef enum {
	SDIO_nIEN_DIS = 0b0,
	SDIO_nIEN_ENA = 0b1
} sdio_nien_t;

typedef enum {
	SDIO_ATACMD_DIS = 0b0,
	SDIO_ATACMD_ENA = 0b1
} sdio_atacmd_t;

typedef struct _sdio_cmd_bit {
	unsigned cmd_index :6;
	unsigned resp_wait :1;
	unsigned resp_type :1;
	unsigned int_wait :1;
	unsigned pend_wait :1;
	unsigned cpsm_en :1;
	unsigned sdio_suspend :1;
	unsigned en_cmd_compl :1;
	unsigned ni_en :1;
	unsigned ata_cmd :1;
	unsigned reserved_15_31 :17;
} _sdio_cmd_bit_t;

typedef union _sdio_cmd_reg {
	uint32_t all;
	struct _sdio_cmd_bit bit;
} _sdio_cmd_reg_t;

extern void sdio_power_control(sdio_pwrctrl_t pwrctrl);
extern void sdio_clock_control(uint8_t clkdiv, sdio_clken_t clken, sdio_pwrsav_t pwrsav, sdio_bypass_t bypass);

#endif /* SDIO_SDIO_H_ */
