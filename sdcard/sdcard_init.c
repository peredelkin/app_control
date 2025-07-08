/*
 * sdcard_init.c
 *
 *  Created on: 30 мая 2025 г.
 *      Author: ruslan
 */

#include <string.h>
#include "sdcard.h"
#include "sys/counter/sys_counter.h"

extern err_t sdcard_card_reset(sdcard_t* sdcard);
extern err_t sdcard_card_set_bl_len(sdcard_t* sdcard);

extern err_t sdcard_cmd(sdcard_t* sdcard, const sdcard_cmd_t* cmd, uint32_t argument);
extern err_t sdcard_acmd(sdcard_t* sdcard, const sdcard_acmd_t* cmd, uint32_t cmd_arg, uint32_t acmd_arg);
extern err_t sdcard_change_current_state(sdcard_t* sdcard);
extern err_t sdcard_status_read(sdcard_t* sdcard, uint32_t timeout);

//SDIOCLK 48 MHz
//SDIO_CK frequency = SDIOCLK / [CLKDIV + 2]
//Frequenc( PCLK2 ) ≥ 3 ⁄ 8 × Frequency ( SDIO_CK )
void sdcard_sdio_set_clock_div(uint8_t clkdiv, sdio_clken_t clken) {
	sdio_power_control(SDIO_POWER_PWRCTRL_OFF);
	sdio_clock_control(clkdiv, clken, SDIO_CLKCR_PWRSAV_ENA, SDIO_CLKCR_BYP_DIS);
}

void sdcard_sdio_power_on() {
	sdcard_sdio_set_clock_div(118, SDIO_CLKCR_CLK_EN);  //400k
	sdio_power_control(SDIO_POWER_PWRCTRL_ON);
}

void sdcard_sdio_power_off() {
	sdcard_sdio_set_clock_div(118, SDIO_CLKCR_CLK_DIS);  //400k
}

void sdcard_sdio_switch_speed() {
	sdcard_sdio_set_clock_div(0 , SDIO_CLKCR_CLK_EN); //24M
	sdio_power_control(SDIO_POWER_PWRCTRL_ON);
}

err_t sdcard_dma_init(sdcard_t* sdcard) {
	if(sdcard == NULL) return E_NULL_POINTER;

	sdcard->dma_err = dma_struct_init(&(sdcard->dma), sdcard->dma_stream);

	return sdcard->dma_err;
}

err_t sdcard_reset(sdcard_t* sdcard) {
	if(sdcard == NULL) return E_NULL_POINTER;

	sdcard->initialized = false;

	sdcard->cmd = NULL;
	sdcard->current_state = SDCARD_STATE_IDLE;
	sdcard->CCC = (SDCARD_CCC_0 | SDCARD_CCC_2 | SDCARD_CCC_4 | SDCARD_CCC_5 | SDCARD_CCC_8);
	sdcard->type = SDCARD_TYPE_UNKNOWN;

	sdcard->CSD.tran_speed = 0.0f;
	sdcard->CSD.bl_len = 0;
	sdcard->CSD.bl_count = 0;
	sdcard->CSD.capacity = 0;

	return E_NO_ERROR;
}

//установка типа карты по ответу ACMD41
void sdcard_type_define(sdcard_t* sdcard) {
	if(sdcard->response.r3.bit.CARD_CAPACITY_STATUS) {
		//10b
		sdcard->type = SDCARD_TYPE_HC_XC;
		if (sdcard->response.r3.bit.OVER_2TB_SUPPORT) {
			//11b
			sdcard->type = SDCARD_TYPE_UC;
		}
	} else {
		//00b
		sdcard->type = SDCARD_TYPE_SC;
	}
}

err_t sdcard_card_initialization(sdcard_t* sdcard) {
	//CMD8 with argument: 2.7-3.6v
	sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD8, (0b1 << 8));
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	//ACMD41 without argument
	sdcard->cmd_err = sdcard_acmd(sdcard, &sdcard_ACMD41, 0, 0);
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	//ACMD41 with argument: SDHC or SDXC supported and 3.2-3.3v
	uint8_t acmd41_timeout = 11; //11 * 100ms
	do {

		sdcard->cmd_err = sdcard_acmd(sdcard, &sdcard_ACMD41, 0, ((0b1 << 30) | (0b11 << 20)));
		if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

		sys_counter_delay(0, 100000); // 100ms

		acmd41_timeout--;

	} while ((sdcard->response.r3.bit.CARD_POWER_UP_STATUS == 0) && (acmd41_timeout > 0));

	if(acmd41_timeout == 0) {
		return E_TIME_OUT;
	}

	sdcard->cmd_err = sdcard_change_current_state(sdcard);
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	//определние типа карты по ответу ACMD41
	sdcard_type_define(sdcard);

	return E_NO_ERROR;
}

err_t sdcard_card_CID_read_any(sdcard_t* sdcard) {
	sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD2, 0);
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	sdcard->CID.all[0] = sdcard->response.r2.all[0];
	sdcard->CID.all[1] = sdcard->response.r2.all[1];
	sdcard->CID.all[2] = sdcard->response.r2.all[2];
	sdcard->CID.all[3] = sdcard->response.r2.all[3];

	return E_NO_ERROR;
}

err_t sdcard_card_RCA_read(sdcard_t* sdcard) {
	sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD3, 0);
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	sdcard->RCA = sdcard->response.r6.all & 0xFFFF0000;

	return E_NO_ERROR;
}

enum {
	SDCARD_CSD_VERSION_1 = 0,
	SDCARD_CSD_VERSION_2,
	SDCARD_CSD_VERSION_3
};

static const float taac_unit[8] = {
		1e-9,	/* 1ns		*/
		10e-9,	/* 10ns		*/
		100e-9,	/* 100ns	*/
		1e-6,	/* 1us		*/
		10e-6,	/* 10us		*/
		100e-6,	/* 100us	*/
		1e-3,	/* 1ms		*/
		10e-3	/* 10ms		*/
};

static const float taac_value[16] = {
		0.0f, 1.0f, 1.2f, 1.3f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f, 5.0f, 5.5f, 6.0f, 7.0f, 8.0f
};

err_t sdcard_CSD_TAAC_calc(sdcard_t* sdcard, uint8_t csd_version, float* taac) {

	switch (csd_version) {

	case SDCARD_CSD_VERSION_1:
		*taac = taac_value[sdcard->CSD.v1.bit.TAAC_VALUE] * taac_unit[sdcard->CSD.v1.bit.TAAC_UNIT];
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_2:
		/*
		 * This field is fixed to 0Eh, which indicates 1 ms.
		 * The host should not use TAAC, NSAC, and R2W_FACTOR
		 * to calculate timeout and should uses fixed timeout values
		 * for read and write operations (See 4.6.2).
		 */
		*taac = taac_value[sdcard->CSD.v2.bit.TAAC_VALUE] * taac_unit[sdcard->CSD.v2.bit.TAAC_UNIT];
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_3:
		/*
		 * Definition of this field is same as in CSD Version2.0.
		 */
		*taac = taac_value[sdcard->CSD.v3.bit.TAAC_VALUE] * taac_unit[sdcard->CSD.v3.bit.TAAC_UNIT];
		return E_NO_ERROR;

	default:
		break;
	}

	return E_NOT_IMPLEMENTED;
}

err_t sdcard_CSD_NSAC_calc(sdcard_t* sdcard, uint8_t csd_version, uint32_t* nsac) {

	switch (csd_version) {

	case SDCARD_CSD_VERSION_1:
		*nsac = 100 * sdcard->CSD.v1.bit.NSAC;
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_2:
		/*
		 * This field is fixed to 00h. NSAC should not be used to calculate time-out values.
		 */
		*nsac = 100 * sdcard->CSD.v2.bit.NSAC;
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_3:
		/*
		 * Definition of this field is same as in CSD Version2.0.
		 */
		*nsac = 100 * sdcard->CSD.v3.bit.NSAC;
		return E_NO_ERROR;

	default:
		break;
	}

	return E_NOT_IMPLEMENTED;
}

/*
 * MULT = 2^(C_SIZE_MULT+2)
 */
err_t sdcard_CSD_MULT_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* mult) {

	switch (csd_version) {

	case SDCARD_CSD_VERSION_1:
		*mult = (1 << (sdcard->CSD.v1.bit.C_SIZE_MULT + 2));
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_2:
		*mult = 1024;
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_3:
		*mult = 1024;
		return E_NO_ERROR;

	default:
		break;
	}

	return E_NOT_IMPLEMENTED;
}

static const float tran_speed_unit[8] = {
		100e3,	/* 100kbit/s	*/
		1e6,	/* 1Mbit/s		*/
		10e6,	/* 10Mbit/s		*/
		100e6,	/* 100Mbit/s	*/
		0,		/*4*/
		0,		/*5*/
		0,		/*6*/
		0,		/*7*/
};

static const float tran_speed_value[16] = {
		0.0f, 1.0f, 1.2f, 1.3f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f, 4.5f, 5.0f, 5.5f, 6.0f, 7.0f, 8.0f
};

err_t sdcard_CSD_TRAN_SPEED_calc(sdcard_t *sdcard, uint8_t csd_version, float *tran_speed) {

	switch (csd_version) {

	case SDCARD_CSD_VERSION_1:
		*tran_speed = tran_speed_value[sdcard->CSD.v1.bit.TRAN_SPEED_VALUE]
									   * tran_speed_unit[sdcard->CSD.v1.bit.TRAN_SPEED_UNIT];
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_2:
		/*
		 *  This field shall be set to 0Bh (100Mbit/sec) in both SDR50 and DDR50 mode,
		 *  and shall be set to 2Bh (200Mbit/sec) in SDR104 mode.
		 */
		*tran_speed = tran_speed_value[sdcard->CSD.v2.bit.TRAN_SPEED_VALUE]
									   * tran_speed_unit[sdcard->CSD.v2.bit.TRAN_SPEED_UNIT];
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_3:
		/*
		 * Definition of this field is same as in CSD Version2.0.
		 */
		*tran_speed = tran_speed_value[sdcard->CSD.v3.bit.TRAN_SPEED_VALUE]
									   * tran_speed_unit[sdcard->CSD.v3.bit.TRAN_SPEED_UNIT];
		return E_NO_ERROR;

	default:
		break;
	}

	return E_NOT_IMPLEMENTED;
}

/*
 * BLOCK_LEN = 2^READ_BL_LEN
 */
err_t sdcard_CSD_BLOCK_LEN_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* len, uint32_t* len_power) {

	switch (csd_version) {

	case SDCARD_CSD_VERSION_1:
		*len = (1 << sdcard->CSD.v1.bit.READ_BL_LEN);
		*len_power = sdcard->CSD.v1.bit.READ_BL_LEN;
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_2:
		*len = (1 << sdcard->CSD.v2.bit.READ_BL_LEN);
		*len_power = sdcard->CSD.v2.bit.READ_BL_LEN;
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_3:
		*len = (1 << sdcard->CSD.v3.bit.READ_BL_LEN);
		*len_power = sdcard->CSD.v3.bit.READ_BL_LEN;
		return E_NO_ERROR;

	default:
		break;
	}

	return E_NOT_IMPLEMENTED;
}

/*
 * BLOCKNR = (C_SIZE+1) * MULT
 */
err_t sdcard_CSD_BLOCKNR_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* count) {

	uint64_t mult = 0;
	err_t err = sdcard_CSD_MULT_calc(sdcard, csd_version, &mult);
	if(err != E_NO_ERROR) return err;

	switch (csd_version) {

	case SDCARD_CSD_VERSION_1:
		*count = (sdcard->CSD.v1.bit.C_SIZE + 1) * mult;
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_2:
		*count = (sdcard->CSD.v2.bit.C_SIZE + 1) * mult;
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_3:
		*count = (sdcard->CSD.v3.bit.C_SIZE + 1) * mult;
		return E_NO_ERROR;

	default:
		break;
	}

	return E_NOT_IMPLEMENTED;
}

/*
 * v1: memory capacity = BLOCKNR * BLOCK_LEN
 * v2: memory capacity = (C_SIZE+1) * 512KByte
 * v3: memory capacity = (C_SIZE+1) * 512KByte
 */
err_t sdcard_CSD_memory_capacity_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* capacity) {

	err_t err = E_NO_ERROR;

	uint64_t count = 0;
	err = sdcard_CSD_BLOCKNR_calc(sdcard, csd_version, &count);
	if(err != E_NO_ERROR) return err;

	uint32_t len_power = 0;
	uint64_t len = 0;
	err = sdcard_CSD_BLOCK_LEN_calc(sdcard, csd_version, &len, &len_power);
	if(err != E_NO_ERROR) return err;

	*capacity = count * len;

	return E_NO_ERROR;
}

err_t sdcard_CSD_erasable_sector_calc(sdcard_t* sdcard, uint8_t csd_version, uint32_t* erase_len) {
	uint8_t ERASE_BLK_EN = 0;
	uint8_t SECTOR_SIZE = 0;

	//CSD Version
	switch (csd_version) {
	case SDCARD_CSD_VERSION_1:
		ERASE_BLK_EN = sdcard->CSD.v1.bit.ERASE_BLK_EN;
		SECTOR_SIZE = sdcard->CSD.v1.bit.SECTOR_SIZE;
		break;

	case SDCARD_CSD_VERSION_2:
		ERASE_BLK_EN = sdcard->CSD.v2.bit.ERASE_BLK_EN;
		SECTOR_SIZE = sdcard->CSD.v2.bit.SECTOR_SIZE;
		break;

	case SDCARD_CSD_VERSION_3:
		ERASE_BLK_EN = sdcard->CSD.v3.bit.ERASE_BLK_EN;
		SECTOR_SIZE = sdcard->CSD.v3.bit.SECTOR_SIZE;
		break;

	default:
		return E_NOT_IMPLEMENTED;
	}

	//SD Card Type
	switch (sdcard->type) {
	case SDCARD_TYPE_SC:
		if(ERASE_BLK_EN) {
			*erase_len = SDCARD_BLOCK_SIZE;
		} else {
			*erase_len = SDCARD_BLOCK_SIZE * SECTOR_SIZE;
		}
		break;

	case SDCARD_TYPE_HC_XC:
		*erase_len = SDCARD_BLOCK_SIZE;
		break;

	case SDCARD_TYPE_UC:
		*erase_len = SDCARD_BLOCK_SIZE;
		break;

	case SDCARD_TYPE_UNKNOWN:
		//no break

	default:
		return E_NOT_IMPLEMENTED;
	}

	return E_NO_ERROR;
}

typedef enum {
	sdcard_card_CSD_read_state_CMD,
	sdcard_card_CSD_read_state_CPY,
	sdcard_card_CSD_read_state_TRAN_SPEED_calc,
	sdcard_card_CSD_read_state_BLOCK_LEN_calc,
	sdcard_card_CSD_read_state_BLOCKNR_calc,
	sdcard_card_CSD_read_state_capacity_calc,
	sdcard_card_CSD_read_state_erasable_calc,
	sdcard_card_CSD_read_state_DONE
}sdcard_card_CSD_read_state_t;

err_t sdcard_card_CSD_read(sdcard_t* sdcard) {
	sdcard_card_CSD_read_state_t state = sdcard_card_CSD_read_state_CMD;

	do {
		switch(state) {
		case sdcard_card_CSD_read_state_CMD:
			sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD9, sdcard->RCA);
			break;

		case sdcard_card_CSD_read_state_CPY:
			memcpy(sdcard->CSD.v1.all, sdcard->response.r2.all, sizeof(sdcard_reg_CSD_v1_t));
			memcpy(sdcard->CSD.v2.all, sdcard->response.r2.all, sizeof(sdcard_reg_CSD_v2_t));
			memcpy(sdcard->CSD.v3.all, sdcard->response.r2.all, sizeof(sdcard_reg_CSD_v3_t));
			sdcard->cmd_err = E_NO_ERROR;
			break;

		case sdcard_card_CSD_read_state_TRAN_SPEED_calc:
			sdcard->cmd_err = sdcard_CSD_TRAN_SPEED_calc(sdcard, sdcard->CSD.v1.bit.CSD_STRUCTURE, &sdcard->CSD.tran_speed);
			break;

		case sdcard_card_CSD_read_state_BLOCK_LEN_calc:
			sdcard->cmd_err = sdcard_CSD_BLOCK_LEN_calc(sdcard, sdcard->CSD.v1.bit.CSD_STRUCTURE, &sdcard->CSD.bl_len, &sdcard->CSD.bl_len_power);
			break;

		case sdcard_card_CSD_read_state_BLOCKNR_calc:
			sdcard->cmd_err = sdcard_CSD_BLOCKNR_calc(sdcard, sdcard->CSD.v1.bit.CSD_STRUCTURE, &sdcard->CSD.bl_count);
			break;

		case sdcard_card_CSD_read_state_capacity_calc:
			sdcard->cmd_err = sdcard_CSD_memory_capacity_calc(sdcard, sdcard->CSD.v1.bit.CSD_STRUCTURE, &sdcard->CSD.capacity);
			break;

		case sdcard_card_CSD_read_state_erasable_calc:
			sdcard->cmd_err = sdcard_CSD_erasable_sector_calc(sdcard, sdcard->CSD.v1.bit.CSD_STRUCTURE, &sdcard->CSD.erase_bl_len);
			break;

		default:
			return E_NOT_IMPLEMENTED;
		}

		if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

		state++;

	} while (state != sdcard_card_CSD_read_state_DONE);

	return E_NO_ERROR;
}

err_t sdcard_card_CID_read(sdcard_t* sdcard) {
	sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD10, sdcard->RCA);
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	sdcard->CID.all[0] = sdcard->response.r2.all[0];
	sdcard->CID.all[1] = sdcard->response.r2.all[1];
	sdcard->CID.all[2] = sdcard->response.r2.all[2];
	sdcard->CID.all[3] = sdcard->response.r2.all[3];

	return E_NO_ERROR;
}

err_t sdcard_card_init(sdcard_t *sdcard) {
	sdcard_sdio_power_on(); //400k

	err_t err = E_NO_ERROR;
	//инициализация структуры sdcard
	err = sdcard_dma_init(sdcard);
	if (err != E_NO_ERROR) return err;

	err = sdcard_reset(sdcard);
	if (err != E_NO_ERROR) return err;

	//CMD0
	err = sdcard_card_reset(sdcard);
	if (err != E_NO_ERROR) return err;

	//CMD8 + ACMD41
	err = sdcard_card_initialization(sdcard);
	if (err != E_NO_ERROR) return err;

	//CMD2
	err = sdcard_card_CID_read_any(sdcard);
	if (err != E_NO_ERROR) return err;

	//CMD3
	err = sdcard_card_RCA_read(sdcard);
	if (err != E_NO_ERROR) return err;

	//CMD9
	err = sdcard_card_CSD_read(sdcard);
	if (err != E_NO_ERROR) return err;

	//CMD10
	err = sdcard_card_CID_read(sdcard);
	if (err != E_NO_ERROR) return err;

	//CMD7
	err = sdcard_card_select(sdcard);
	if (err != E_NO_ERROR) return err;

	//SET_BLOCKLEN for SDSC
	if (sdcard->type == SDCARD_TYPE_SC) {
		//CMD16
		err = sdcard_card_set_bl_len(sdcard);
		if (err != E_NO_ERROR) return err;
	}

	sdcard_sdio_switch_speed();

	//ACMD13
	err =  sdcard_status_read(sdcard, 0xFFFFFF); //Timeout about 0,7s
	if (err != E_NO_ERROR) return err;

	sdcard->initialized = true;

	return E_NO_ERROR;
}
