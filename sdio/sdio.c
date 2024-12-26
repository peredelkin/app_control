/*
 * sdio.c
 *
 *  Created on: 25 дек. 2024 г.
 *      Author: ruslan
 */

#include <stdbool.h>
#include <assert.h>
#include "lib/errors/errors.h"

#include "sdio.h"

//TODO: проверить структуры sdio перед использованием!

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

void sdio_power_control(sdio_pwrctrl_t pwrctrl) {
	_sdio_power_reg_t power;
	power.all = SDIO->POWER;
	power.bit.pwrctrl = pwrctrl;
	while(SDIO->POWER != power.all) {
		SDIO->POWER = power.all;
	}
}

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













