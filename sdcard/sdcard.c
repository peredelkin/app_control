#include "sdcard.h"
#include "sdcard_cmd.h"
#include "utils/utils.h"
#include "crc/crc16_ccitt.h"
#include "sys/counter/sys_counter.h"
#include <string.h>

//! Получение границы блока из адреса.
#define SDCARD_ADDRESS_BLOCK(addr) ((addr) & ~((SDCARD_BLOCK_SIZE) - 1))

typedef enum {
	SDCARD_INIT_POWER_ON,
	SDCARD_INIT_RE_INIT_MEMORY,
	SDCARD_INIT_RE_INIT_IO,
	SDCARD_INIT_MEMORY_INIT,
	SDCARD_INIT_SKIP_MEMORY_INIT,
	SDCARD_INIT_POWER_CYCLE,
	SDCARD_INIT_UNUSABLE_CARD
} sdcard_init_state_t;

static void sdcard_resp_setup(sdcard_t* sdcard) {
	sdcard->resp_wait = SDIO_RESP_WAIT_ENA;
	sdcard->resp_long = SDIO_RESP_TYPE_SHORT;
	sdcard->resp_crc = SDIO_RESP_CRC_INCLUDED;

	switch (sdcard->cmd->response_type) {
	case SDCARD_RESPONSE_NO:
		sdcard->resp_wait = SDIO_RESP_WAIT_DIS;
		break;

	case SDCARD_RESPONSE_R2:
		sdcard->resp_long = SDIO_RESP_TYPE_LONG;
		//no break

	case SDCARD_RESPONSE_R3:
		//no break

	case SDCARD_RESPONSE_R4b:
		sdcard->resp_crc = SDIO_RESP_CRC_NOT_INCLUDED;
		break;

	default:
		break;
	}
}

static err_t sdcard_cmd_status(sdcard_t* sdcard) {
	if(sdcard == NULL) return E_NULL_POINTER;

	sdcard->cmd_err = sdio_cmd_status();
	//есть ошибка
	if (sdcard->cmd_err != E_NO_ERROR) {
		//ошибка CRC
		if (sdcard->cmd_err == E_SDIO_CMD_CRCFAIL) {
			//CRC должен быть в ответе
			if (sdcard->resp_crc == SDIO_RESP_CRC_INCLUDED)
				return sdcard->cmd_err;
		} else {
			//другая ошибка
			return sdcard->cmd_err;
		}
	}

	return E_NO_ERROR;
}


static err_t sdcard_cmd_send(sdcard_t* sdcard, const sdcard_cmd_t* cmd, uint32_t argument) {

	if(cmd->state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_SDCARD_CMD_STATE_MISMATCH;

	sdcard->cmd = (sdcard_cmd_t*) cmd;

	sdcard_resp_setup(sdcard);

	sdio_cpsm_set(
			argument,
			sdcard->cmd->index,
			sdcard->resp_wait,
			sdcard->resp_long,
			SDIO_INT_WAIT_DIS,
			SDIO_PEND_WAIT_DIS,
			SDIO_CPSM_EN,
			SDIO_SUSPEND_DIS,
			SDIO_CMD_COMPLETION_DIS,
			SDIO_nIEN_DIS,
			SDIO_ATACMD_DIS);

	return sdcard_cmd_status(sdcard);
}

static err_t sdcard_acmd_send(sdcard_t* sdcard, const sdcard_acmd_t* cmd, uint32_t argument) {

	if(cmd->state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_SDCARD_CMD_STATE_MISMATCH;

	sdcard->cmd = (sdcard_cmd_t*) cmd;

	sdcard_resp_setup(sdcard);

	sdio_cpsm_set(
			argument,
			sdcard->cmd->index,
			sdcard->resp_wait,
			sdcard->resp_long,
			SDIO_INT_WAIT_DIS,
			SDIO_PEND_WAIT_DIS,
			SDIO_CPSM_EN,
			SDIO_SUSPEND_DIS,
			SDIO_CMD_COMPLETION_DIS,
			SDIO_nIEN_DIS,
			SDIO_ATACMD_DIS);

	return sdcard_cmd_status(sdcard);
}

#define SDIO_COMMAND_INDEX_MASK 0b111111

//TODO: проверить, все ли ответы, где используется функция, имеют корректный индекс
static err_t sdcard_response_index_compare(sdcard_t* sdcard, uint32_t cmd) {
	if(sdcard->cmd->index == cmd) return E_NO_ERROR;
	return E_SDCARD_RESPONSE_INDEX_MISMATCH;
}

static err_t sdcard_response_rcv(sdcard_t* sdcard) {
	if(sdcard == NULL || sdcard->cmd == NULL) return E_NULL_POINTER;

	if(sdcard->resp_wait == SDIO_RESP_WAIT_DIS) return E_NO_ERROR;

	uint32_t resp_cmd;
	uint32_t resp_data[4];

	sdio_response_read(sdcard->resp_long, &resp_cmd, resp_data);

	resp_cmd = resp_cmd & SDIO_COMMAND_INDEX_MASK; //отбрасываем мусор

	switch (sdcard->cmd->response_type) {

	case SDCARD_RESPONSE_R1b:
		//TODO: сюда нужна функция проверки состояния I/O с таймаутом. или не сюда.
		//no break

	case SDCARD_RESPONSE_R1:
		sdcard->response.r1.all = resp_data[0];
		return sdcard_response_index_compare(sdcard, resp_cmd);

	case SDCARD_RESPONSE_R2:
		sdcard->response.r2.all[0] = resp_data[3];
		sdcard->response.r2.all[1] = resp_data[2];
		sdcard->response.r2.all[2] = resp_data[1];
		sdcard->response.r2.all[3] = resp_data[0];
		if(resp_cmd != SDIO_COMMAND_INDEX_MASK) return E_SDCARD_RESPONSE_INDEX_MISMATCH;
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R3:
		sdcard->response.r3.all = resp_data[0];
		if(resp_cmd != SDIO_COMMAND_INDEX_MASK) return E_SDCARD_RESPONSE_INDEX_MISMATCH;
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R4:
		sdcard->response.r4.all = resp_data[0];
		return sdcard_response_index_compare(sdcard, resp_cmd);

	case SDCARD_RESPONSE_R4b:
		sdcard->response.r4b.all = resp_data[0];
		//TODO: сюда нужна функция проверки состояния I/O с таймаутом. или не сюда.
		return E_NOT_IMPLEMENTED;

	case SDCARD_RESPONSE_R5:
		sdcard->response.r5.all = resp_data[0];
		return sdcard_response_index_compare(sdcard, resp_cmd);

	case SDCARD_RESPONSE_R6:
		sdcard->response.r6.all = resp_data[0];
		return sdcard_response_index_compare(sdcard, resp_cmd);

	case SDCARD_RESPONSE_R7:
		sdcard->response.r7.all = resp_data[0];
		return sdcard_response_index_compare(sdcard, resp_cmd);

	default:
		break;
	}

	return E_NOT_IMPLEMENTED;
}


static err_t sdcard_status_error_check(sdcard_t* sdcard) {
	if(sdcard == NULL || sdcard->cmd == NULL) return E_NULL_POINTER;

	switch (sdcard->cmd->response_type) {
	case SDCARD_RESPONSE_R1b:
		//no break
	case SDCARD_RESPONSE_R1:
		if(sdcard->response.r1.bit.AKE_SEQ_ERROR)		return 	E_SDCARD_RESPONSE_AKE_SEQ_ERROR;
		if(sdcard->response.r1.bit.WP_ERASE_SKIP)		return 	E_SDCARD_RESPONSE_WP_ERASE_SKIP;
		if(sdcard->response.r1.bit.CSD_OVERWRITE)		return 	E_SDCARD_RESPONSE_CSD_OVERWRITE;
		if(sdcard->response.r1.bit.ERROR)				return 	E_SDCARD_RESPONSE_ERROR;
		if(sdcard->response.r1.bit.CC_ERROR)			return 	E_SDCARD_RESPONSE_CC_ERROR;
		if(sdcard->response.r1.bit.CARD_ECC_FAILED)		return 	E_SDCARD_RESPONSE_CARD_ECC_FAILED;
		if(sdcard->response.r1.bit.ILLEGAL_COMMAND)		return 	E_SDCARD_RESPONSE_ILLEGAL_COMMAND;
		if(sdcard->response.r1.bit.COM_CRC_ERROR)		return 	E_SDCARD_RESPONSE_COM_CRC_ERROR;
		if(sdcard->response.r1.bit.LOCK_UNLOCK_FAILED)	return	E_SDCARD_RESPONSE_LOCK_UNLOCK_FAILED;
		if(sdcard->response.r1.bit.WP_VIOLATION)		return	E_SDCARD_RESPONSE_WP_VIOLATION;
		if(sdcard->response.r1.bit.ERASE_PARAM)			return	E_SDCARD_RESPONSE_ERASE_PARAM;
		if(sdcard->response.r1.bit.ERASE_SEQ_ERROR)		return 	E_SDCARD_RESPONSE_ERASE_SEQ_ERROR;
		if(sdcard->response.r1.bit.BLOCK_LEN_ERROR)		return 	E_SDCARD_RESPONSE_BLOCK_LEN_ERROR;
		if(sdcard->response.r1.bit.ADDRESS_ERROR)		return 	E_SDCARD_RESPONSE_ADDRESS_ERROR;
		if(sdcard->response.r1.bit.OUT_OF_RANGE)		return 	E_SDCARD_RESPONSE_OUT_OF_RANGE;
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R6:
		if(sdcard->response.r6.bit.AKE_SEQ_ERROR)		return E_SDCARD_RESPONSE_AKE_SEQ_ERROR;
		if(sdcard->response.r6.bit.ERROR)				return E_SDCARD_RESPONSE_ERROR;
		if(sdcard->response.r6.bit.ILLEGAL_COMMAND)		return E_SDCARD_RESPONSE_ILLEGAL_COMMAND;
		if(sdcard->response.r6.bit.COM_CRC_ERROR)		return E_SDCARD_RESPONSE_COM_CRC_ERROR;
		return E_NO_ERROR;

	default:
		break;
	}

	return E_NO_ERROR;
}

err_t sdcard_change_current_state(sdcard_t* sdcard) {
	if(sdcard == NULL || sdcard->cmd == NULL) return E_NULL_POINTER;

	//проверка состояния в ответе
	switch (sdcard->cmd->response_type) {
	case SDCARD_RESPONSE_R1b:
		//no break

	case SDCARD_RESPONSE_R1:
		if (sdcard->current_state != sdcard->response.r1.bit.CURRENT_STATE) return E_SDCARD_RESPONSE_STATE_MISMATCH;
		break;

	case SDCARD_RESPONSE_R6:
		if (sdcard->current_state != sdcard->response.r6.bit.CURRENT_STATE) return E_SDCARD_RESPONSE_STATE_MISMATCH;
		break;

	default:
		break;
	}

	//такой ситуации не должно возникать, если команда была принята
	if (sdcard->cmd->state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_SDCARD_RESPONSE_STATE_MISMATCH;

	//изменить текущее состояние через состояние команды
	sdcard->current_state = sdcard->cmd->state[sdcard->current_state];

	return E_NO_ERROR;
}

err_t sdcard_sync_current_state(sdcard_t* sdcard) {
	if(sdcard == NULL || sdcard->cmd == NULL) return E_NULL_POINTER;

	//проверка состояния в ответе
	switch (sdcard->cmd->response_type) {
	case SDCARD_RESPONSE_R1b:
		//no break

	case SDCARD_RESPONSE_R1:
		sdcard->current_state = sdcard->response.r1.bit.CURRENT_STATE;
		break;

	case SDCARD_RESPONSE_R6:
		sdcard->current_state = sdcard->response.r6.bit.CURRENT_STATE;
		break;

	default:
		return E_INVALID_OPERATION;
		break;
	}

	return E_NO_ERROR;
}

err_t sdcard_operation_complete_state(sdcard_t* sdcard) {
	if(sdcard == NULL) return E_NULL_POINTER;

	static const sdcard_state_t state[SDCARD_STATE_COUNT] = {
			SDCARD_STATE_ILLEGAL,	//idle
			SDCARD_STATE_ILLEGAL,	//ready
			SDCARD_STATE_ILLEGAL,	//ident
			SDCARD_STATE_ILLEGAL,	//stby
			SDCARD_STATE_ILLEGAL,	//tran
			SDCARD_STATE_TRAN,		//data
			SDCARD_STATE_ILLEGAL,	//rcv
			SDCARD_STATE_TRAN,		//prg
			SDCARD_STATE_STBY,		//dis
			SDCARD_STATE_ILLEGAL,	//ina
	};

	//проверка возможности изменения состояния
	if (state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_SDCARD_CMD_STATE_MISMATCH;

	//изменить текущее состояние через завершение операции
	sdcard->current_state = state[sdcard->current_state];

	return E_NO_ERROR;
}

err_t sdcard_cmd(sdcard_t* sdcard, const sdcard_cmd_t* cmd, uint32_t argument) {
	if(sdcard == NULL || cmd == NULL) return E_NULL_POINTER;

	sdcard->cmd_err = sdcard_cmd_send(sdcard, cmd, argument);
	if(sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	sdcard->resp_err = sdcard_response_rcv(sdcard);
	if(sdcard->resp_err != E_NO_ERROR) return sdcard->resp_err;

	sdcard->resp_err = sdcard_status_error_check(sdcard);
	if(sdcard->resp_err != E_NO_ERROR) return sdcard->resp_err;

	sdcard->cmd_err = sdcard_change_current_state(sdcard);
	if(sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	return E_NO_ERROR;
}


err_t sdcard_acmd(sdcard_t* sdcard, const sdcard_acmd_t* cmd, uint32_t argument) {
	if(sdcard == NULL || cmd == NULL) return E_NULL_POINTER;

	sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD55, 0);
	if(sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	if (sdcard->response.r1.bit.APP_CMD == 0) return E_INVALID_OPERATION;

	sdcard->cmd_err = sdcard_acmd_send(sdcard, cmd, argument);
	if(sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	sdcard->resp_err = sdcard_response_rcv(sdcard);
	if(sdcard->resp_err != E_NO_ERROR) return sdcard->resp_err;

	sdcard->resp_err = sdcard_status_error_check(sdcard);
	if(sdcard->resp_err != E_NO_ERROR) return sdcard->resp_err;

	/*
	 * Менять состояние после выполнения
	 * других ACMD необходимо вручную
	 * из-за условия смены состояния ACMD41
	 */

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


//CSD
void sdcard_CSD_fill(sdcard_t* sdcard) {
	sdcard->CSD.v1.all[0] = sdcard->response.r2.all[0];
	sdcard->CSD.v1.all[1] = sdcard->response.r2.all[1];
	sdcard->CSD.v1.all[2] = sdcard->response.r2.all[2];
	sdcard->CSD.v1.all[3] = sdcard->response.r2.all[3];

	sdcard->CSD.v2.all[0] = sdcard->response.r2.all[0];
	sdcard->CSD.v2.all[1] = sdcard->response.r2.all[1];
	sdcard->CSD.v2.all[2] = sdcard->response.r2.all[2];
	sdcard->CSD.v2.all[3] = sdcard->response.r2.all[3];

	sdcard->CSD.v3.all[0] = sdcard->response.r2.all[0];
	sdcard->CSD.v3.all[1] = sdcard->response.r2.all[1];
	sdcard->CSD.v3.all[2] = sdcard->response.r2.all[2];
	sdcard->CSD.v3.all[3] = sdcard->response.r2.all[3];
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

//CID
void sdcard_CID_fill(sdcard_t* sdcard) {
	sdcard->CID.all[0] = sdcard->response.r2.all[0];
	sdcard->CID.all[1] = sdcard->response.r2.all[1];
	sdcard->CID.all[2] = sdcard->response.r2.all[2];
	sdcard->CID.all[3] = sdcard->response.r2.all[3];
}

//read, write, erase commands
err_t sdcard_cmd_read(sdcard_t* sdcard, uint32_t count, uint64_t addr) {
	if(sdcard == NULL) return E_NULL_POINTER;

	if(count == 0) return E_INVALID_VALUE;

	union __U_ADDR_64 {
	    uint64_t _64;
	    struct _S_ADDR_64 {uint32_t lo; uint32_t hi;} _32;
	};

	union __U_ADDR_64 __u_addr_64 = {addr};

	uint32_t addr_hi = __u_addr_64._32.hi;
	uint32_t addr_lo = __u_addr_64._32.lo;

	if(sdcard->type == SDCARD_TYPE_SC) {
		addr_lo = addr_lo * 512; //TODO: сделать настройку размера блка
	} else {
		if(count > 1) {
			//Set block count
			sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD23, count);
			if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;
		}
	}

	if(sdcard->type == SDCARD_TYPE_UC) {
		//Set extended address
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD22, addr_hi);
		if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;
	}

	if(count > 1) {
		//Multi-Block Read
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD18, addr_lo);
	} else {
		//Single Block Read
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD17, addr_lo);
	}

	return sdcard->cmd_err;
}

err_t sdcard_cmd_write(sdcard_t* sdcard, uint32_t count, uint64_t addr) {
	if(sdcard == NULL) return E_NULL_POINTER;

	if(count == 0) return E_INVALID_VALUE;

	union __U_ADDR_64 {
	    uint64_t _64;
	    struct _S_ADDR_64 {uint32_t lo; uint32_t hi;} _32;
	};

	union __U_ADDR_64 __u_addr_64 = {addr};

	uint32_t addr_hi = __u_addr_64._32.hi;
	uint32_t addr_lo = __u_addr_64._32.lo;

	if(sdcard->type == SDCARD_TYPE_SC) {
		addr_lo = addr_lo * 512; //TODO: сделать настройку размера блка
	} else {
		if(count > 1) {
			//Set block count
			sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD23, count);
			if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;
		}
	}

	if(sdcard->type == SDCARD_TYPE_UC) {
		//Set extended address
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD22, addr_hi);
		if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;
	}

	if(count > 1) {
		//Multi-Block Write
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD25, addr_lo);
	} else {
		//Single Block Write
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD24, addr_lo);
	}

	return sdcard->cmd_err;
}

err_t sdcard_cmd_erase(sdcard_t* sdcard, uint64_t addr_first, uint64_t addr_last, uint32_t func) {
	if(sdcard == NULL) return E_NULL_POINTER;

	union __U_ADDR_64 {
	    uint64_t _64;
	    struct _S_ADDR_64 {uint32_t lo; uint32_t hi;} _32;
	};

	union __U_ADDR_64 __u_addr_first_64 = {addr_first};

	uint32_t addr_first_hi = __u_addr_first_64._32.hi;
	uint32_t addr_first_lo = __u_addr_first_64._32.lo;

	union __U_ADDR_64 __u_addr_last_64 = {addr_last};

	uint32_t addr_last_hi = __u_addr_last_64._32.hi;
	uint32_t addr_last_lo = __u_addr_last_64._32.lo;

	if(sdcard->type == SDCARD_TYPE_SC) {
		//TODO: сделать настройку размера блка
		addr_first_lo = addr_first_lo * 512;
		addr_last_lo = addr_last_lo * 512;
	}

	//first
	if (sdcard->type == SDCARD_TYPE_UC) {
		//Set extended address
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD22, addr_first_hi);
		if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;
	}

	sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD32, addr_first_lo);
	if(sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	//last
	if (sdcard->type == SDCARD_TYPE_UC) {
		//Set extended address
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD22, addr_last_hi);
		if(sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;
	}

	sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD33, addr_last_lo);
	if(sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	//erase
	sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD38, func);
	return sdcard->cmd_err;
}

err_t sdcard_cmd_sync_state(sdcard_t* sdcard) {
	if(sdcard == NULL) return E_NULL_POINTER;

	sdcard->cmd_err = sdcard_cmd_send(sdcard, &sdcard_CMD13, sdcard->RCA);
	if(sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	sdcard->resp_err = sdcard_response_rcv(sdcard);
	if(sdcard->resp_err != E_NO_ERROR) return sdcard->resp_err;

	sdcard->resp_err = sdcard_status_error_check(sdcard);
	if(sdcard->resp_err != E_NO_ERROR) return sdcard->resp_err;

	sdcard->cmd_err = sdcard_sync_current_state(sdcard);
	if(sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	return E_NO_ERROR;
}

//dma
err_t sdcard_dma_common_setup(sdcard_t* sdcard, uint32_t* memory_addr, dma_scr_dir_t dir) {
	if(sdcard == NULL) return E_NULL_POINTER;

	sdcard->dma_err = dma_stream_open(&sdcard->dma);
	if(sdcard->dma_err != E_NO_ERROR) return sdcard->dma_err;

	dma_stream_peripheral_address_register_write(&(sdcard->dma), (uint32_t)&(SDIO->FIFO));	//Source/Destination
	dma_stream_memory_0_address_register_write(&(sdcard->dma), (uint32_t)memory_addr);		//Destination/Source

    dma_stream_init(
    		&sdcard->dma,
    		DMA_SCR_DBM_DIS,
			DMA_SCR_CT_MEM0,
			DMA_SCR_CHSEL_4,
			DMA_FCR_DMDIS_ENA,
			DMA_FCR_FTH_FULL,
			DMA_SCR_MSIZE_32,
			DMA_SCR_MBURST_4,
			DMA_SCR_MINC_ENA,
			DMA_SCR_PSIZE_32,
			DMA_SCR_PBURST_4,
			DMA_SCR_PINC_DIS,
			DMA_SCR_PINCOS_PSIZE,
			dir,
			DMA_SCR_PFCTRL_ENA,
			DMA_SCR_CIRC_DIS,
			DMA_SCR_TCIE_DIS,
			DMA_SCR_HTIE_DIS,
			DMA_SCR_TEIE_DIS,
			DMA_SCR_DMEIE_DIS,
			DMA_FCR_FEIE_DIS,
			DMA_SCR_PL_LOW,
			DMA_SCR_EN_ENA);

	return E_NO_ERROR;
}

err_t sdcard_dma_read_setup(sdcard_t* sdcard, uint32_t* memory_addr) {
	return sdcard_dma_common_setup(sdcard, memory_addr, DMA_SCR_DIR_PERI_TO_MEM);		//Peripheral-to-memory
}

err_t sdcard_dma_write_setup(sdcard_t* sdcard, uint32_t* memory_addr) {
	return sdcard_dma_common_setup(sdcard, memory_addr, DMA_SCR_DIR_MEM_TO_PERI);		//Memory-to-peripheral
}

err_t sdcard_wait_transfer_complete(sdcard_t *sdcard) {
	do {
		sdcard->data_err = sdio_data_status();
	} while ((sdcard->data_err == E_NOT_IMPLEMENTED) || sdcard->data_err == E_SDIO_DATA_DBCKEND);

	uint32_t ISR = dma_stream_status_register_read(&sdcard->dma);

	if (dma_stream_status_TEIF_read(ISR, &sdcard->dma)) { //if TE
		dma_stream_disable(&(sdcard->dma)); //disable Stream
		dma_stream_status_TEIF_clear(&sdcard->dma); //clear TE
		return E_DMA_STREAM_ERROR;
	}

	if (dma_stream_status_TCIF_read(ISR, &sdcard->dma)) { //if TC
		dma_stream_disable(&(sdcard->dma)); //disable Stream
		dma_stream_status_TCIF_clear(&(sdcard->dma)); //clear TC
	} else {
		return E_STATE;
	}

	return sdcard->data_err;
}

//data
err_t sdcard_read(sdcard_t* sdcard, uint32_t* memory_addr, uint64_t block_addr, uint32_t block_count, uint32_t timeout) {
	if (sdcard == NULL || memory_addr == NULL) return E_NULL_POINTER;

	if (block_count == 0) return E_INVALID_VALUE;

	//stream conf
	uint32_t item_count = (block_count * 512) / 4; //TODO: сделать настройку размера блка

	if(item_count > DMA_DATA_COUNT_MAX) return E_OUT_OF_RANGE;

	sdcard->dma_err = sdcard_dma_read_setup(sdcard, memory_addr);
	if (sdcard->dma_err != E_NO_ERROR) return sdcard->dma_err;

	sdio_dpsm_set(
			SDIO_DTDIR_FROM_CARD,
			SDIO_DTMODE_BLOCK,
			SDIO_DMAEN_ENA,
			9, /*TODO: сделать настройку размера блока*/
			SDIO_RWSTART_DIS,
			SDIO_RWSTOP_DIS,
			SDIO_RWMOD_D2,
			SDIO_SDIOEN_ENA,
			block_count,
			timeout);

	sdcard->cmd_err = sdcard_cmd_read(sdcard, block_count, block_addr);
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	sdio_dpsm_enable();

	sdcard->data_err = sdcard_wait_transfer_complete(sdcard);
	dma_stream_close(&sdcard->dma);

	if(sdcard->type == SDCARD_TYPE_SC) {
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD12, 0);
	}

	sdio_dpsm_reset();

	if (sdcard->data_err != E_NO_ERROR) return sdcard->data_err;
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	uint32_t sdcard_timeout = 100; //1s

	do {
		sdcard->cmd_err = sdcard_cmd_sync_state(sdcard);
		if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;
		sys_counter_delay(0, 10000); // 10ms
		sdcard_timeout--;
	} while ((sdcard->current_state != SDCARD_STATE_TRAN) && (sdcard_timeout > 0));

	if(sdcard_timeout == 0) return E_TIME_OUT;

	return E_NO_ERROR;
}

err_t sdcard_write(sdcard_t* sdcard, uint32_t* memory_addr, uint64_t block_addr, uint32_t block_count, uint32_t timeout) {
	if (sdcard == NULL || memory_addr == NULL) return E_NULL_POINTER;

	if (block_count == 0) return E_INVALID_VALUE;

	//stream conf
	uint32_t item_count = (block_count * 512) / 4; //TODO: сделать настройку размера блка

	if(item_count > DMA_DATA_COUNT_MAX) return E_OUT_OF_RANGE;

	sdcard->dma_err = sdcard_dma_write_setup(sdcard, memory_addr);
	if (sdcard->dma_err != E_NO_ERROR) return sdcard->dma_err;

	sdio_dpsm_set(
			SDIO_DTDIR_TO_CARD,
			SDIO_DTMODE_BLOCK,
			SDIO_DMAEN_ENA,
			9, /*TODO: сделать настройку размера блока*/
			SDIO_RWSTART_DIS,
			SDIO_RWSTOP_DIS,
			SDIO_RWMOD_D2,
			SDIO_SDIOEN_ENA,
			block_count,
			timeout);

	sdcard->cmd_err = sdcard_cmd_write(sdcard, block_count, block_addr);
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	sdio_dpsm_enable();

	sdcard->data_err = sdcard_wait_transfer_complete(sdcard);
	dma_stream_close(&sdcard->dma);

	if(sdcard->type == SDCARD_TYPE_SC) {
		sdcard->cmd_err = sdcard_cmd(sdcard, &sdcard_CMD12, 0);
	}

	sdio_dpsm_reset();

	if (sdcard->data_err != E_NO_ERROR) return sdcard->data_err;
	if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;

	uint32_t sdcard_timeout = 100; //1s

	do {
		sdcard->cmd_err = sdcard_cmd_sync_state(sdcard);
		if (sdcard->cmd_err != E_NO_ERROR) return sdcard->cmd_err;
		sys_counter_delay(0, 10000); // 10ms
		sdcard_timeout--;
	} while ((sdcard->current_state != SDCARD_STATE_TRAN) && (sdcard_timeout > 0));

	if(sdcard_timeout == 0) return E_TIME_OUT;

	return E_NO_ERROR;
}





