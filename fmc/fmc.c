/*
 * fmc.c
 *
 *  Created on: 27 дек. 2023 г.
 *      Author: Ruslan
 */

#include "fmc.h"

__attribute__((noinline))
void fmc_eccen_set(FMC_BANK23_TypeDef *bank, bool state) {
	FMC_PCR_REG_TypeDef PCR;
	PCR.all = bank->PCR.all;
	if(state) {
		PCR.bit.ECCEN = 1;
	} else {
		PCR.bit.ECCEN = 0;
	}
	bank->PCR.all = PCR.all;
}

static int _FMC_ECCPS[6] = {
		256,  //000: 256 byte
		512,  //001: 512 byte
		1024, //010: 1024 byte
		2048, //011: 2048 byte
		4096, //100: 4096 byte
		8192  //101: 8192 byte
};

int fmc_ecc_page_size(FMC_BANK23_TypeDef* bank) {
	int eccps = bank->PCR.bit.ECCPS;
	if(eccps > 0b101) return 0;
	return _FMC_ECCPS[eccps];
}

void fmc_fifo_empty_wait(FMC_BANK23_TypeDef *control) {
	while(control->SR.bit.FEMPT == 0) __NOP();
}

__attribute__((noinline))
err_t fmc_data_8_read(__IO uint8_t *fmc_data, uint8_t *data, int data_len) {
	if(fmc_data == NULL) return E_NULL_POINTER;
	if(data == NULL) return E_NULL_POINTER;
	for(int i = 0; i < data_len; i++) {
		data[i] = *fmc_data;
	}
	return E_NO_ERROR;
}

__attribute__((noinline))
err_t fmc_data_32_read(__IO uint32_t *fmc_data, uint32_t *data, int data_len) {
	if(fmc_data == NULL) return E_NULL_POINTER;
	if(data == NULL) return E_NULL_POINTER;
	for(int i = 0; i < data_len; i++) {
		data[i] = *fmc_data;
	}
	return E_NO_ERROR;
}
