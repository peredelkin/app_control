/*
 * sdio.c
 *
 *  Created on: 25 дек. 2024 г.
 *      Author: ruslan
 */

#include "sdio.h"
#include "stm32f4xx.h"

void sdio_power_control(sdio_pwrctrl_t pwrctrl) {
	_sdio_power_reg_t power;
	power.all = SDIO->POWER;

	power.bit.pwrctrl = pwrctrl;

	SDIO->POWER = power.all;
}

void sdio_clock_control(
		uint8_t clkdiv,
		sdio_clken_t clken,
		sdio_pwrsav_t pwrsav,
		sdio_bypass_t bypass) {
	_sdio_clkcr_reg_t clkcr;
	clkcr.all = SDIO->CLKCR;

	clkcr.bit.clkdiv = clkdiv;
	clkcr.bit.clken = clken;
	clkcr.bit.pwrsav = pwrsav;
	clkcr.bit.bypass = bypass;
	clkcr.bit.widbus = SDIO_CLKCR_BUS_DEF;
	clkcr.bit.negedge = SDIO_CLKCR_EDGE_RISING;
	clkcr.bit.hwfc_en = SDIO_CLKCR_HWFC_DIS;

	SDIO->CLKCR = clkcr.all;
}

void sdio_command(
		uint32_t argument,
		int cmd_index,
		sdio_respwait_t respwait,
		sdio_resptype_t resptype,
		sdio_intwait_t intwait,
		sdio_pendwait_t pendwait,
		sdio_cpsmen_t cpsmen,
		sdio_suspend_t suspend,
		sdio_cmdcompl_t cmdcompl,
		sdio_nien_t nien,
		sdio_atacmd_t atacmd) {

	_sdio_cmd_reg_t cmd;
	cmd.all = SDIO->CMD;

	cmd.bit.cmd_index = cmd_index;
	cmd.bit.resp_wait = respwait;
	cmd.bit.resp_type = resptype;
	cmd.bit.int_wait = intwait;
	cmd.bit.pend_wait = pendwait;
	cmd.bit.cpsm_en = cpsmen;
	cmd.bit.sdio_suspend = suspend;
	cmd.bit.en_cmd_compl = cmdcompl;
	cmd.bit.ni_en = nien;
	cmd.bit.ata_cmd = atacmd;

	SDIO->ARG = argument;

	SDIO->CMD = cmd.all;
}

#define SDIO_BLOCK_COUNT_MAX ((1 << 16) - 1)
#define SDIO_DATA_LENGTH_MAX ((1 << 25) - 1)

err_t sdio_data(
		sdio_dten_t dten,
		sdio_dtdir_t dtdir,
		sdio_dtmode_t dtmode,
		sdio_dmaen_t dmaen,
		sdio_dblocksize_t dblocksize,
		sdio_rwstart_t rwstart,
		sdio_rwstop_t rwstop,
		sdio_rwmod_t rwmod,
		sdio_sdioen_t sdioen,
		uint32_t block_count,
		uint32_t timeout) {

	if (dblocksize > SDIO_DBLOCKSIZE_16384_bytes) return E_OUT_OF_RANGE;

	if (block_count > SDIO_BLOCK_COUNT_MAX) return E_OUT_OF_RANGE;

	uint32_t data_length =  (1 << dblocksize) * block_count;

	if (data_length > SDIO_DATA_LENGTH_MAX) return E_OUT_OF_RANGE;

	_sdio_data_reg_t dctrl;
	dctrl.all = SDIO->DCTRL;

	SDIO->DTIMER = timeout;

	SDIO->DLEN = data_length;

	SDIO->DCTRL = dctrl.all;

	return E_NO_ERROR;
}

uint32_t sdio_CMD_ACT() {
	return (SDIO->STA & SDIO_STA_CMDACT);
}

uint32_t sdio_DATA_ACT() {
	return (SDIO->STA & (SDIO_STA_TXACT | SDIO_STA_RXACT));
}

err_t sdio_cmd_status() {
	while(sdio_CMD_ACT());

	/*Command response timeout*/
	if (SDIO->STA & SDIO_STA_CTIMEOUT) {
		SDIO->ICR = SDIO_ICR_CTIMEOUTC;
		return E_TIME_OUT;
	}

	/*Command response received (CRC check failed)*/
	if (SDIO->STA & SDIO_STA_CCRCFAIL) {
		SDIO->ICR = SDIO_ICR_CCRCFAILC;
		return E_CRC;
	}

	/*Command response received (CRC check passed)*/
	if (SDIO->STA & SDIO_STA_CMDREND) {
		SDIO->ICR = SDIO_ICR_CMDRENDC;
		return E_NO_ERROR;
	}

	/*Command sent (no response required)*/
	if (SDIO->STA & SDIO_STA_CMDSENT) {
		SDIO->ICR = SDIO_ICR_CMDSENTC;
		return E_NO_ERROR;
	}

	return E_NOT_IMPLEMENTED;
}

err_t sdio_data_status() {
	uint32_t STA;
	while(sdio_DATA_ACT()) {
		STA = SDIO->STA;
		/*Data NOT end (data counter, SDIDCOUNT, is NOT zero)*/
		/*Data block sent/received (CRC check passed)*/
		if((!(STA & SDIO_STA_DATAEND)) && (STA & SDIO_STA_DBCKEND)) {
			SDIO->ICR = SDIO_ICR_DBCKENDC;
		}
	}

	/*Data timeout*/
	if (SDIO->STA & SDIO_STA_DTIMEOUT) {
		SDIO->ICR = SDIO_ICR_DTIMEOUTC;
		return E_TIME_OUT;
	}

	/*Data block sent/received (CRC check failed)*/
	if (SDIO->STA & SDIO_STA_DCRCFAIL) {
		SDIO->ICR = SDIO_ICR_DCRCFAILC;
		return E_CRC;
	}

	/*Data end (data counter, SDIDCOUNT, is zero)*/
	/*Data block sent/received (CRC check passed)*/
	if ((SDIO->STA & SDIO_STA_DATAEND) && (SDIO->STA & SDIO_STA_DBCKEND)) {
		SDIO->ICR = SDIO_ICR_DATAENDC;
		SDIO->ICR = SDIO_ICR_DBCKENDC;
		return E_NO_ERROR;
	}

	return E_NOT_IMPLEMENTED;
}

void sdio_response_read(sdio_resptype_t resptype, uint32_t* cmd, uint32_t* resp) {
	*cmd = SDIO->RESPCMD;

	resp[0] = SDIO->RESP1;

	if(resptype == SDIO_RESP_TYPE_SHORT) return;

	resp[1] = SDIO->RESP2;
	resp[2] = SDIO->RESP3;
	resp[3] = SDIO->RESP4;
}









