/*
 * fmc_test.c
 *
 *  Created on: 25 июл. 2024 г.
 *      Author: Ruslan
 */

#include <stdio.h>
#include "fmc_test.h"
#include "../sram/fmc_sram.h"
#include "../nand/fmc_nand.h"
#include "sys/counter/sys_counter.h"

err_t sram_32_test() {
	volatile uint32_t sram_w_data;
	volatile uint32_t sram_r_data;

	//запись
	sram_w_data = 0xFFFFFFFF;
	for(int i = 0; i < SRAM_WORDS_32; i++) {
		SRAM_DATA_32_SECTION[i] = sram_w_data;
		sram_w_data--;
	}

	//чтение
	sram_w_data = 0xFFFFFFFF;
	for(int i = 0; i < SRAM_WORDS_32; i++) {
		sram_r_data = SRAM_DATA_32_SECTION[i];
		if(sram_r_data != sram_w_data) return E_IO_ERROR;
		sram_w_data--;
	}

	//сброс
	for(int i = 0; i < SRAM_WORDS_32; i++) {
		SRAM_DATA_32_SECTION[i] = 0;
	}

	//чтение
	for(int i = 0; i < SRAM_WORDS_32; i++) {
		if(SRAM_DATA_32_SECTION[i] != 0) return E_IO_ERROR;
	}

	return E_NO_ERROR;
}

err_t sram_16_test() {
	volatile uint16_t sram_w_data;
	volatile uint16_t sram_r_data;

	//запись
	sram_w_data = 0xFFFF;
	for(int i = 0; i < SRAM_WORDS_16; i++) {
		SRAM_DATA_16_SECTION[i] = sram_w_data;
		sram_w_data--;
	}

	//чтение
	sram_w_data = 0xFFFF;
	for(int i = 0; i < SRAM_WORDS_16; i++) {
		sram_r_data = SRAM_DATA_16_SECTION[i];
		if(sram_r_data != sram_w_data) return E_IO_ERROR;
		sram_w_data--;
	}

	//сброс
	for(int i = 0; i < SRAM_WORDS_16; i++) {
		SRAM_DATA_16_SECTION[i] = 0;
	}

	//чтение
	for(int i = 0; i < SRAM_WORDS_16; i++) {
		if(SRAM_DATA_16_SECTION[i] != 0) return E_IO_ERROR;
	}

	return E_NO_ERROR;
}

err_t sram_8_test() {
	volatile uint8_t sram_w_data;
	volatile uint8_t sram_r_data;

	//запись
	sram_w_data = 0xFF;
	for(int i = 0; i < SRAM_WORDS_8; i++) {
		SRAM_DATA_8_SECTION[i] = sram_w_data;
		sram_w_data--;
	}

	//чтение
	sram_w_data = 0xFF;
	for(int i = 0; i < SRAM_WORDS_8; i++) {
		sram_r_data = SRAM_DATA_8_SECTION[i];
		if(sram_r_data != sram_w_data) return E_IO_ERROR;
		sram_w_data--;
	}

	//сброс
	for(int i = 0; i < SRAM_WORDS_8; i++) {
		SRAM_DATA_8_SECTION[i] = 0;
	}

	//чтение
	for(int i = 0; i < SRAM_WORDS_8; i++) {
		if(SRAM_DATA_8_SECTION[i] != 0) return E_IO_ERROR;
	}

	return E_NO_ERROR;
}

typedef struct {
	err_t test16;
	err_t test32;
	err_t test8;
} fmc_sram_test_t;

typedef struct {
	err_t id;
	err_t bad;
} fmc_nand_test_t;

fmc_sram_test_t fmc_sram_err;
fmc_nand_test_t fmc_nand_err;

err_t fmc_sram_test() {
	fmc_sram_err.test16 = sram_16_test();
	sys_counter_tv_print();
	printf("SRAM Test 16 %s\n", (fmc_sram_err.test16 == 0) ? "success" : "fail");

	fmc_sram_err.test32 = sram_32_test();
	sys_counter_tv_print();
	printf("SRAM Test 32 %s\n", (fmc_sram_err.test32 == 0) ? "success" : "fail");

	fmc_sram_err.test8 = sram_8_test();
	sys_counter_tv_print();
	printf("SRAM Test 8 %s\n", (fmc_sram_err.test8 == 0) ? "success" : "fail");

	if(fmc_sram_err.test16 || fmc_sram_err.test32 || fmc_sram_err.test8) {
		return E_IO_ERROR;
	} else {
		return E_NO_ERROR;
	}
}

err_t fmc_nand_test() {
	size_t bad_block_count = 0;
	fmc_nand_err.id = nand_K9F1G08U0E_id_check(&nand_K9F1G08U0E_drv);
	fmc_nand_err.bad = E_NO_ERROR;

	for (int i = 0; i < K9F1G08U0E_BLOCK_COUNT; i++) {
		if (nand_K9F1G08U0E_block_check_bad(i) != E_NO_ERROR) {
			printf("NAND bad block: %d\n", i);
			bad_block_count++;
		}
	}

	if(bad_block_count > K9F1G08U0E_BLOCK_BAD_MAX) {
		printf("NAND bad block: too much\n");
		fmc_nand_err.bad = E_OUT_OF_RANGE;
	}

	if(fmc_nand_err.id || fmc_nand_err.bad) {
		return E_STATE;
	} else {
		return E_NO_ERROR;
	}
}

