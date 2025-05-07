#include "sdcard.h"
#include "sdcard_cmd.h"
#include "utils/utils.h"
#include "crc/crc16_ccitt.h"
#include <string.h>


// Константы.

//! Число пустых байт в начале инициализации SD-карты.
#define SDCARD_INIT_DUMMY_BYTES 10
//! Число байт в ожидании ответа карты.
#define SDCARD_WAIT_REPLY_BYTES 8

//! Число байт до выбора SD-карты.
#define SDCARD_SELECT_BEFORE_BYTES 1
//! Число байт после выбора SD-карты.
#define SDCARD_SELECT_AFTER_BYTES 1
//! Число байт до сниятия выбора SD-карты.
#define SDCARD_DESELECT_BEFORE_BYTES 1
//! Число байт после сниятия выбора SD-карты.
#define SDCARD_DESELECT_AFTER_BYTES 1

//! Значение передаваемых данных по-умолчанию 16 бит.
#define SDCARD_DEFAULT_DATA16 0xffffU
//! Значение передаваемых данных по-умолчанию 8 бит.
#define SDCARD_DEFAULT_DATA8 0xffU
//! Значение получаемых данных в случае занятой SD-карты.
#define SDCARD_BUSY_DATA 0x0
//! Значение получаемых данных в случае отсутствия ответа SD-карты.
#define SDCARD_NOREPLY_DATA 0xff

//! Полином контрольной суммы CRC16.
#define SDCARD_CRC16_POLYNOMIAL 0x1021
#define SDCARD_CRC16_POLYNOMIAL_REVERSED 0x8408

//! Аргумент команды проверки напряжения (SEND_IF_COND).
#define SDCARD_SEND_IF_COND_CHECK 0x1aa


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


static err_t sdcard_cmd_send(sdcard_t* sdcard, const sdcard_cmd_t* cmd, uint32_t argument) {

	if(cmd->state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_SDCARD_ILLEGAL_COMMAND;

	sdcard->cmd = (sdcard_cmd_t*) cmd;

	sdcard_resp_setup(sdcard);

	sdio_command(
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

	return E_NO_ERROR;
}

static err_t sdcard_acmd_send(sdcard_t* sdcard, const sdcard_acmd_t* cmd, uint32_t argument) {

	if(cmd->state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_SDCARD_ILLEGAL_COMMAND;

	sdcard->cmd = (sdcard_cmd_t*) cmd;

	sdcard_resp_setup(sdcard);

	sdio_command(
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

	return E_NO_ERROR;
}

#define SDIO_COMMAND_INDEX_MASK 0b111111

//TODO: проверить, все ли ответы, где используется функция, имеют корректный индекс
static err_t sdcard_response_index_compare(sdcard_t* sdcard, uint32_t cmd) {
	if(sdcard->cmd->index == cmd) return E_NO_ERROR;
	return E_INVALID_VALUE;
}

static err_t sdcard_response_rcv(sdcard_t* sdcard) {
	if(sdcard == NULL || sdcard->cmd == NULL) return E_NULL_POINTER;

	err_t err = sdio_cmd_wait();
	//есть ошибка
	if(err != E_NO_ERROR) {
		//ошибка CRC
		if(err == E_CRC) {
			//CRC должен быть в ответе
			if (sdcard->resp_crc == SDIO_RESP_CRC_INCLUDED) return err;
		} else {
			//другая ошибка
			return err;
		}
	}

	if(sdcard->resp_wait == SDIO_RESP_WAIT_DIS) return E_NO_ERROR;

	uint32_t cmd;
	uint32_t resp[4];

	sdio_response_read(sdcard->resp_long, &cmd, resp);

	cmd = cmd & SDIO_COMMAND_INDEX_MASK; //отбрасываем мусор

	if(sdcard)

	switch (sdcard->cmd->response_type) {

	case SDCARD_RESPONSE_R1b:
		//TODO: сюда нужна функция проверки состояния I/O с таймаутом. или не сюда.
		//no break

	case SDCARD_RESPONSE_R1:
		if (sdcard_response_index_compare(sdcard, cmd) != E_NO_ERROR) return E_INVALID_VALUE;
		sdcard->response.r1.all = resp[0];
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R2:
		if(cmd != SDIO_COMMAND_INDEX_MASK) return E_INVALID_VALUE;
		sdcard->response.r2.all[0] = resp[3];
		sdcard->response.r2.all[1] = resp[2];
		sdcard->response.r2.all[2] = resp[1];
		sdcard->response.r2.all[3] = resp[0];
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R3:
		if(cmd != SDIO_COMMAND_INDEX_MASK) return E_INVALID_VALUE;
		sdcard->response.r3.all = resp[0];
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R4:
		if(sdcard_response_index_compare(sdcard, cmd) != E_NO_ERROR) return E_INVALID_VALUE;
		sdcard->response.r4.all = resp[0];
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R4b:
		sdcard->response.r4b.all = resp[0];
		//TODO: сюда нужна функция проверки состояния I/O с таймаутом. или не сюда.
		return E_NOT_IMPLEMENTED;

	case SDCARD_RESPONSE_R5:
		if(sdcard_response_index_compare(sdcard, cmd) != E_NO_ERROR) return E_INVALID_VALUE;
		sdcard->response.r5.all = resp[0];
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R6:
		if(sdcard_response_index_compare(sdcard, cmd) != E_NO_ERROR) return E_INVALID_VALUE;
		sdcard->response.r6.all = resp[0];
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R7:
		if(sdcard_response_index_compare(sdcard, cmd) != E_NO_ERROR) return E_INVALID_VALUE;
		sdcard->response.r7.all = resp[0];
		return E_NO_ERROR;

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
		if(sdcard->response.r1.bit.AKE_SEQ_ERROR) return E_NOT_IMPLEMENTED;
		if(sdcard->response.r1.bit.WP_ERASE_SKIP) return E_SDCARD_WP_ERASE_SKIP;
		if(sdcard->response.r1.bit.CSD_OVERWRITE) return E_SDCARD_CSD_OVERWRITE;
		if(sdcard->response.r1.bit.ERROR) return E_SDCARD_ERROR;
		if(sdcard->response.r1.bit.CC_ERROR) return E_SDCARD_CC_ERROR;
		if(sdcard->response.r1.bit.CARD_ECC_FAILED) return E_SDCARD_CARD_ECC_FAILED;
		if(sdcard->response.r1.bit.ILLEGAL_COMMAND) return E_SDCARD_ILLEGAL_COMMAND;
		if(sdcard->response.r1.bit.COM_CRC_ERROR) return E_SDCARD_COM_CRC_ERROR;
		if(sdcard->response.r1.bit.LOCK_UNLOCK_FAILED) return E_SDCARD_LOCK_UNLOCK_CMD_FAILED;
		if(sdcard->response.r1.bit.WP_VIOLATION) return E_SDCARD_WP_VIOLATION;
		if(sdcard->response.r1.bit.ERASE_PARAM) return E_SDCARD_ERASE_PARAM;
		if(sdcard->response.r1.bit.ERASE_SEQ_ERROR) return E_SDCARD_ERASE_SEQ_ERROR;
		if(sdcard->response.r1.bit.BLOCK_LEN_ERROR) return E_NOT_IMPLEMENTED;
		if(sdcard->response.r1.bit.ADDRESS_ERROR) return E_NOT_IMPLEMENTED;
		if(sdcard->response.r1.bit.OUT_OF_RANGE) return E_SDCARD_OUT_OF_RANGE;
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R6:
		if(sdcard->response.r6.bit.AKE_SEQ_ERROR) return E_NOT_IMPLEMENTED;
		if(sdcard->response.r6.bit.ERROR) return E_SDCARD_ERROR;
		if(sdcard->response.r6.bit.ILLEGAL_COMMAND) return E_SDCARD_ILLEGAL_COMMAND;
		if(sdcard->response.r6.bit.COM_CRC_ERROR) return E_SDCARD_COM_CRC_ERROR;
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
		if (sdcard->current_state != sdcard->response.r1.bit.CURRENT_STATE) return E_INVALID_VALUE;
		break;

	case SDCARD_RESPONSE_R6:
		if (sdcard->current_state != sdcard->response.r6.bit.CURRENT_STATE) return E_INVALID_VALUE;
		break;

	default:
		break;
	}

	//такой ситуации не должно возникать, если команда была принята
	if (sdcard->cmd->state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_INVALID_VALUE;

	//изменить текущее состояние через состояние команды
	sdcard->current_state = sdcard->cmd->state[sdcard->current_state];

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
	if (state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_INVALID_VALUE;

	//изменить текущее состояние через завершение операции
	sdcard->current_state = state[sdcard->current_state];

	return E_NO_ERROR;
}

err_t sdcard_cmd(sdcard_t* sdcard, const sdcard_cmd_t* cmd, uint32_t argument) {
	if(sdcard == NULL || cmd == NULL) return E_NULL_POINTER;

	err_t err = E_NO_ERROR;

	err = sdcard_cmd_send(sdcard, cmd, argument);
	if(err != E_NO_ERROR) return err;

	err = sdcard_response_rcv(sdcard);
	if(err != E_NO_ERROR) return err;

	err = sdcard_status_error_check(sdcard);
	if(err != E_NO_ERROR) return err;

	err = sdcard_change_current_state(sdcard);
	if(err != E_NO_ERROR) return err;

	return err;
}


err_t sdcard_acmd(sdcard_t* sdcard, const sdcard_acmd_t* cmd, uint32_t argument) {
	if(sdcard == NULL || cmd == NULL) return E_NULL_POINTER;

	err_t err = E_NO_ERROR;

	err = sdcard_cmd(sdcard, &sdcard_CMD55, 0);
	if(err != E_NO_ERROR) return err;

	if (sdcard->response.r1.bit.APP_CMD == 0) return E_INVALID_OPERATION;

	err = sdcard_acmd_send(sdcard, cmd, argument);
	if(err != E_NO_ERROR) return err;

	err = sdcard_response_rcv(sdcard);
	if(err != E_NO_ERROR) return err;

	err = sdcard_status_error_check(sdcard);
	if(err != E_NO_ERROR) return err;

	/*
	 * Менять состояние после выполнения
	 * других ACMD необходимо вручную
	 * из-за условия смены состояния ACMD41
	 */

	return err;
}

//установка типа карты по ответу ACMD41
void sdcard_type_set(sdcard_t* sdcard) {
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
err_t sdcard_CSD_BLOCK_LEN_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* len) {

	switch (csd_version) {

	case SDCARD_CSD_VERSION_1:
		*len = (1 << sdcard->CSD.v1.bit.READ_BL_LEN);
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_2:
		*len = (1 << sdcard->CSD.v2.bit.READ_BL_LEN);
		return E_NO_ERROR;

	case SDCARD_CSD_VERSION_3:
		*len = (1 << sdcard->CSD.v3.bit.READ_BL_LEN);
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

	uint64_t len = 0;
	err = sdcard_CSD_BLOCK_LEN_calc(sdcard, csd_version, &len);
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
err_t sdcard_cmd_read(sdcard_t* sdcard, uint32_t count, uint32_t* addr) {
	if(sdcard == NULL || addr == NULL) return E_NULL_POINTER;

	err_t err = E_NO_ERROR;

	if(count > 1) {
		err = sdcard_cmd(sdcard, &sdcard_CMD23, count);
		if (err != E_NO_ERROR) return err;
	}

	if(sdcard->type == SDCARD_TYPE_UC) {
		err = sdcard_cmd(sdcard, &sdcard_CMD22, (0b111111 & addr[1]));	//[5:0] extended address
		if (err != E_NO_ERROR) return err;
	}

	if(count == 1) {
		//Single Block Read
		err = sdcard_cmd(sdcard, &sdcard_CMD17, addr[0]);
	} else {
		//Multi-Block Read
		err = sdcard_cmd(sdcard, &sdcard_CMD18, addr[0]);
	}

	return err;
}

err_t sdcard_cmd_write(sdcard_t* sdcard, uint32_t count, uint32_t* addr) {
	if(sdcard == NULL || addr == NULL) return E_NULL_POINTER;

	err_t err = E_NO_ERROR;

	if(count > 1) {
		err = sdcard_cmd(sdcard, &sdcard_CMD23, count);
		if (err != E_NO_ERROR) return err;
	}

	if(sdcard->type == SDCARD_TYPE_UC) {
		err = sdcard_cmd(sdcard, &sdcard_CMD22, (0b111111 & addr[1]));	//[5:0] extended address
		if (err != E_NO_ERROR) return err;
	}

	if(count == 1) {
		//Single Block Write
		err = sdcard_cmd(sdcard, &sdcard_CMD24, addr[0]);
	} else {
		//Multi-Block Write
		err = sdcard_cmd(sdcard, &sdcard_CMD25, addr[0]);
	}

	return err;
}

err_t sdcard_cmd_erase(sdcard_t* sdcard, uint32_t* first, uint32_t* last, uint32_t func) {
	if(sdcard == NULL || first == NULL || last == NULL) return E_NULL_POINTER;

	err_t err = E_NO_ERROR;

	//first
	if (sdcard->type == SDCARD_TYPE_UC) {
		err = sdcard_cmd(sdcard, &sdcard_CMD22, (0b111111 & first[1]));	//[5:0] extended address
		if (err != E_NO_ERROR) return err;
	}

	err = sdcard_cmd(sdcard, &sdcard_CMD32, first[0]);
	if(err != E_NO_ERROR) return err;

	//last
	if (sdcard->type == SDCARD_TYPE_UC) {
		err = sdcard_cmd(sdcard, &sdcard_CMD22, (0b111111 & last[1]));	//[5:0] extended address
		if(err != E_NO_ERROR) return err;
	}

	err = sdcard_cmd(sdcard, &sdcard_CMD33, last[0]);
	if(err != E_NO_ERROR) return err;

	//erase
	err = sdcard_cmd(sdcard, &sdcard_CMD38, func);
	return err;
}

//data path
err_t sdcard_dpsm_read_setup(sdcard_t* sdcard) {
	if(sdcard == NULL) return E_NULL_POINTER;

	return E_NO_ERROR;
}

err_t sdcard_dpsm_write_setup(sdcard_t* sdcard) {
	if(sdcard == NULL) return E_NULL_POINTER;

	return E_NO_ERROR;
}

//dma
err_t sdcard_dma_read_setup(sdcard_t* sdcard, uint32_t mar, uint16_t ndtr) {
	if(sdcard == NULL) return E_NULL_POINTER;
	//stream check
	if (!dma_stream_ready(&(sdcard->dma))) return E_BUSY;
	//stream deinit
	dma_stream_deinit(&(sdcard->dma));
	//stream conf
	dma_stream_number_of_data(&(sdcard->dma), ndtr);							//Count

	dma_stream_peripheral_burst_transfer_configuration(&(sdcard->dma), 0b01);	//4 beats
	dma_stream_peripheral_data_size(&(sdcard->dma), 0b10);						//32-bit
	dma_stream_peripheral_address(&(sdcard->dma), &(SDIO->FIFO));				//Source
	dma_stream_peripheral_flow_controller(&(sdcard->dma), true);				//Peripheral as flow controller

	dma_stream_memory_burst_transfer_configuration(&(sdcard->dma), 0b01);		//4 beats
	dma_stream_memory_data_size(&(sdcard->dma), 0b10);							//32-bit
	dma_stream_memory_address(&(sdcard->dma), 0, mar);							//Destination
	dma_stream_memory_increment_mode(&(sdcard->dma), true);						//Memory increment

	dma_stream_data_transfer_direction(&(sdcard->dma), 0b00);					//Peripheral-to-memory

	return E_NO_ERROR;
}

err_t sdcard_dma_write_setup(sdcard_t* sdcard, uint32_t mar, uint16_t ndtr) {
	if(sdcard == NULL) return E_NULL_POINTER;
	//stream check
	if (!dma_stream_ready(&(sdcard->dma))) return E_BUSY;
	//stream deinit
	dma_stream_deinit(&(sdcard->dma));
	//stream conf
	dma_stream_number_of_data(&(sdcard->dma), ndtr);							//Count

	dma_stream_peripheral_burst_transfer_configuration(&(sdcard->dma), 0b01);	//4 beats
	dma_stream_peripheral_data_size(&(sdcard->dma), 0b10);						//32-bit
	dma_stream_peripheral_address(&(sdcard->dma), &(SDIO->FIFO));				//Destination
	dma_stream_peripheral_flow_controller(&(sdcard->dma), true);				//Peripheral as flow controller

	dma_stream_memory_burst_transfer_configuration(&(sdcard->dma), 0b01);		//4 beats
	dma_stream_memory_data_size(&(sdcard->dma), 0b10);							//32-bit
	dma_stream_memory_address(&(sdcard->dma), 0, mar);							//Source
	dma_stream_memory_increment_mode(&(sdcard->dma), true);						//Memory increment

	dma_stream_data_transfer_direction(&(sdcard->dma), 0b01);					//Memory-to-peripheral

	return E_NO_ERROR;
}





