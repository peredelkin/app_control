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

err_t sdio_data_wait() {
	while (SDIO->STA & (SDIO_STA_RXACT | SDIO_STA_TXACT));
	if (SDIO->STA & SDIO_STA_DTIMEOUT) {
		SDIO->ICR = SDIO_ICR_DTIMEOUTC;
		return E_TIME_OUT;
	}
	return E_NO_ERROR;
}

err_t sdio_cmd_wait(sdcard_reply_type_t reply_type) {
	while (SDIO->STA & SDIO_STA_CMDACT);

	if (SDIO->STA & SDIO_STA_CMDREND) {
		SDIO->ICR = SDIO_ICR_CMDRENDC;
		return E_NO_ERROR;
	}

	/*
	 * If the response does not contain a CRC, the device driver must ignore the CRC failed status.
	 */
	if (SDIO->STA & SDIO_STA_CCRCFAIL) {
		SDIO->ICR = SDIO_ICR_CCRCFAILC;
		switch(reply_type) {
		case SDCARD_RESPONSE_R2:
			return E_NO_ERROR;
		case SDCARD_RESPONSE_R3:
			return E_NO_ERROR;
		default:
			return E_CRC;
		}
	}

	if (SDIO->STA & SDIO_STA_CMDSENT) {
		SDIO->ICR = SDIO_ICR_CMDSENTC;
		return E_NO_ERROR;
	}

	if (SDIO->STA & SDIO_STA_CTIMEOUT) {
		SDIO->ICR = SDIO_ICR_CTIMEOUTC;
		return E_TIME_OUT;
	}

	return E_NOT_IMPLEMENTED;
}











