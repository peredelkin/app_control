/*
 * sdcard_response.c
 *
 *  Created on: 23 апр. 2025 г.
 *      Author: Ruslan
 */

#include "sdcard_response.h"

#include "sdio/sdio.h"
#include "sdcard.h"

#define SDIO_COMMAND_INDEX_MASK 0b111111

static err_t sdcard_response_index_compare(sdcard_t* sdcard, uint32_t cmd) {
	if(sdcard->cmd->index == cmd) return E_NO_ERROR;
	return E_INVALID_VALUE;
}

err_t sdcard_response_rcv(sdcard_t* sdcard) {
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
	case SDCARD_RESPONSE_R1:
		if(sdcard_response_index_compare(sdcard, cmd) != E_NO_ERROR) return E_INVALID_VALUE;
		sdcard->response.r1.all = resp[0];
		return E_NO_ERROR;

	case SDCARD_RESPONSE_R1b:
		if(sdcard_response_index_compare(sdcard, cmd) != E_NO_ERROR) return E_INVALID_VALUE;
		sdcard->response.r1b.all = resp[0];
		//TODO: сюда нужна функция проверки состояния I/O с таймаутом. или не сюда.
		return E_NO_ERROR; //E_NOT_IMPLEMENTED

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

