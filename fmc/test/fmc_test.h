/*
 * fmc_test.h
 *
 *  Created on: 25 июл. 2024 г.
 *      Author: Ruslan
 */

#ifndef FMC_TEST_FMC_TEST_H_
#define FMC_TEST_FMC_TEST_H_

#include "lib/errors/errors.h"

typedef struct {
	err_t test16;
	err_t test32;
	err_t test8;
} fmc_sram_test_t;

typedef struct {
	err_t id;
	err_t bad;
} fmc_nand_test_t;

#define FMC_SRAM_TEST
#define FMC_NAND_BLOCK_TEST

extern err_t fmc_sram_test();
extern err_t fmc_nand_test();

#endif /* FMC_TEST_FMC_TEST_H_ */
