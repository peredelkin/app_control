/*
 * sdio.h
 *
 *  Created on: 25 дек. 2024 г.
 *      Author: ruslan
 */

#ifndef SDIO_SDIO_H_
#define SDIO_SDIO_H_

#include <assert.h>

#include "lib/errors/errors.h"
#include "sdcard/sdcard_response.h"

/*
 * Ошибки
 */
//! Ошибка CPSM
//! Command response received (CRC check failed)
#define E_SDIO_CMD_CRCFAIL (E_SDIO_BASE + 1)
//! Command response timeout
#define E_SDIO_CMD_TIMEOUT (E_SDIO_BASE + 2)

//! Ошибки DPSM
//! Data block sent/received (CRC check passed)
#define E_SDIO_DATA_DBCKEND (E_SDIO_BASE + 3)
//! Data block sent/received (CRC check failed)
#define E_SDIO_DATA_CRCFAIL (E_SDIO_BASE + 4)
//! Data timeout
#define E_SDIO_DATA_TIMEOUT (E_SDIO_BASE + 5)
//! Transmit FIFO underrun error
#define E_SDIO_DATA_TX_UNDERRUN (E_SDIO_BASE + 6)
//! Received FIFO overrun error
#define E_SDIO_DATA_RX_OVERRUN (E_SDIO_BASE + 7)
//! Start bit not detected on all data signals in wide bus mode
#define E_SDIO_DATA_STARTBIT (E_SDIO_BASE + 8)

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

typedef enum {
	SDIO_DTEN_DIS = 0b0,
	SDIO_DTEN_ENA = 0b1
} sdio_dten_t;

typedef enum {
	SDIO_DTDIR_TO_CARD = 0b0,
	SDIO_DTDIR_FROM_CARD = 0b1
} sdio_dtdir_t;

typedef enum {
	SDIO_DTMODE_BLOCK = 0b0,
	SDIO_DTMODE_STREAM = 0b1
} sdio_dtmode_t;

typedef enum {
	SDIO_DMAEN_DIS = 0b0,
	SDIO_DMAEN_ENA = 0b1
} sdio_dmaen_t;

typedef enum {
	SDIO_DBLOCKSIZE_1_byte = 0,
	SDIO_DBLOCKSIZE_2_bytes,
	SDIO_DBLOCKSIZE_4_bytes,
	SDIO_DBLOCKSIZE_8_bytes,
	SDIO_DBLOCKSIZE_16_bytes,
	SDIO_DBLOCKSIZE_32_bytes,
	SDIO_DBLOCKSIZE_64_bytes,
	SDIO_DBLOCKSIZE_128_bytes,
	SDIO_DBLOCKSIZE_256_bytes,
	SDIO_DBLOCKSIZE_512_bytes,
	SDIO_DBLOCKSIZE_1024_bytes,
	SDIO_DBLOCKSIZE_2048_bytes,
	SDIO_DBLOCKSIZE_4096_bytes,
	SDIO_DBLOCKSIZE_8192_bytes,
	SDIO_DBLOCKSIZE_16384_bytes
} sdio_dblocksize_t;

typedef enum {
	SDIO_RWSTART_DIS = 0b0,
	SDIO_RWSTART_ENA = 0b1
} sdio_rwstart_t;

typedef enum {
	SDIO_RWSTOP_DIS = 0b0,
	SDIO_RWSTOP_ENA = 0b1
} sdio_rwstop_t;

typedef enum {
	SDIO_RWMOD_D2 = 0b0,
	SDIO_RWMOD_CK = 0b1
} sdio_rwmod_t;

typedef enum {
	SDIO_SDIOEN_DIS = 0b0,
	SDIO_SDIOEN_ENA = 0b1
} sdio_sdioen_t;

typedef struct _sdio_data_bit {
	unsigned dt_en :1;
	unsigned dt_dir :1;
	unsigned dt_mode :1;
	unsigned dma_en :1;
	unsigned block_size :4;
	unsigned rw_start :1;
	unsigned rw_stop :1;
	unsigned rw_mod :1;
	unsigned sdio_en :1;
	unsigned reserved_12_31 :20;
} _sdio_data_bit_t;

typedef union _sdio_bit_reg {
	uint32_t all;
	struct _sdio_data_bit bit;
} _sdio_data_reg_t;

typedef enum {
	SDIO_RESP_CRC_NOT_INCLUDED = 0b0,
	SDIO_RESP_CRC_INCLUDED = 0b1
} sdio_resp_crc_include_t;

extern void sdio_power_control(sdio_pwrctrl_t pwrctrl);
extern void sdio_clock_control(uint8_t clkdiv, sdio_clken_t clken, sdio_pwrsav_t pwrsav, sdio_bypass_t bypass);
extern void sdio_cpsm_set(uint32_t argument, int cmd_index, sdio_respwait_t respwait, sdio_resptype_t resptype,
		sdio_intwait_t intwait, sdio_pendwait_t pendwait, sdio_cpsmen_t cpsmen, sdio_suspend_t suspend,
		sdio_cmdcompl_t cmdcompl, sdio_nien_t nien, sdio_atacmd_t atacmd);

extern void sdio_dpsm_set(sdio_dtdir_t dtdir, sdio_dtmode_t dtmode, sdio_dmaen_t dmaen,
		sdio_dblocksize_t dblocksize, sdio_rwstart_t rwstart, sdio_rwstop_t rwstop, sdio_rwmod_t rwmod,
		sdio_sdioen_t sdioen, uint32_t block_count, uint32_t timeout);

extern void sdio_dpsm_enable();

extern void sdio_dpsm_reset();

extern err_t sdio_cmd_status();
extern void sdio_response_read(sdio_resptype_t resptype, uint32_t* cmd, uint32_t* resp);
extern err_t sdio_data_status();

#endif /* SDIO_SDIO_H_ */
