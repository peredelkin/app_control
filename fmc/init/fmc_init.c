/*
 * fmc_init.c
 *
 *  Created on: 23 нояб. 2023 г.
 *      Author: Ruslan
 */

#include <stdio.h>
#include "fmc_init.h"
#include "../settings/fmc_settings.h"
#include "gpio/init/gpio_init.h"
#include "sys_counter/sys_counter.h"
#include "../test/fmc_test.h"
#include "../nand/fmc_nand.h"

void fmc_rcc_init() {
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
}

void fmc_banks_deinit(void) {
	//reset Bank1 config
	for (int i = 0; i < 4; i++) {
		FMC_BANK1->BCTR[i].CR.all = 0;
		FMC_BANK1->BCTR[i].TR.all = 0;
		FMC_BANK1E->BWTR[i].WTR.all = 0;
	}

	//reset Bank2 config
	FMC_BANK2->PCR.all = 0;
	FMC_BANK2->PMEM.all = 0;
	FMC_BANK2->PATT.all = 0;

	//reset Bank3 config
	FMC_BANK3->PCR.all = 0;
	FMC_BANK3->PMEM.all = 0;
	FMC_BANK3->PATT.all = 0;

	//reset Bank4 config
	FMC_BANK4->PCR.all = 0;
	FMC_BANK4->PMEM.all = 0;
	FMC_BANK4->PATT.all = 0;
	FMC_BANK4->PIO.all = 0;
}

void fmc_bank1_init(FMC_BANK1_TypeDef *bank, FMC_BANK1E_TypeDef *bank_e) {
	//запись настроек SRAM
	bank->BCTR[0].CR.all = FMC_SRAM1_BCTR_CFG.CR.all;
	bank->BCTR[0].TR.all = FMC_SRAM1_BCTR_CFG.TR.all;
	bank_e->BWTR[0].WTR.all = FMC_SRAM1_BWTR_CFG.WTR.all;
}

void fmc_bank3_init(FMC_BANK23_TypeDef *bank) {
	//запись настроек NAND
	bank->PCR.all = FMC_NAND3_CFG.PCR.all;
	bank->PMEM.all = FMC_NAND3_CFG.PMEM.all;
	bank->PATT.all = FMC_NAND3_CFG.PATT.all;
}

err_t fmc_gpio_init(void) {
	//проверка подтягивающих резисторов
	gpio_pin_state_t sram_nCE = gpio_input_bit_read(&GPIO_SRAM_nCE);
	gpio_pin_state_t nand_nCE = gpio_input_bit_read(&GPIO_NAND_nCE);
	gpio_pin_state_t nand_RnB = gpio_input_bit_read(&GPI_NAND_RnB);

	if (sram_nCE && nand_nCE && nand_RnB) {
		fmc_rcc_init(); //включение тактирования
		fmc_banks_deinit();	//сброс настроек банков

		fmc_bank1_init(FMC_BANK1, FMC_BANK1E);	//инициализация SRAM
		fmc_bank3_init(FMC_BANK3);	//инициализация NAND

		gpio_fmc_cfg_setup();	//инициализация GPIO FMC SRAM + NAND

		return E_NO_ERROR;
	} else {
		return E_CANCELED;
	}
}

err_t fmc_init() {
	//проверка подтягивающих резисторов и инициализация пинов
	err_t gpio_init_err = fmc_gpio_init();

	sys_counter_tv_print();
	if(gpio_init_err) {
		printf("FMC GPIO Init fail\n");
		return gpio_init_err;
	} else {
		printf("FMC GPIO Init success\n");
	}

	//тестирование чтения/записи данными шириной 16,32,8 бит
	err_t sram_test_err = fmc_sram_test();

	sys_counter_tv_print();
	if(sram_test_err) {
		printf("SRAM Test fail\n");
		return sram_test_err;
	} else {
		printf("SRAM Test success\n");
	}

	//инициализация DMA
	nand_K9F1G08U0E_init(&nand_K9F1G08U0E_drv);

	//проверка id и количества бэд блоков
	err_t nand_test_err = fmc_nand_test();

	sys_counter_tv_print();
	if (nand_test_err) {
		printf("NAND Test fail\n");
		return nand_test_err;
	} else {
		printf("NAND Test success\n");
	}

	return E_NO_ERROR;
}


