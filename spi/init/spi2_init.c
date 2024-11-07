/*
 * spi_init.c
 *
 *  Created on: 24 окт. 2023 г.
 *      Author: Ruslan
 */

#include "spi_init.h"
#include "stm32f4xx/stm32f4xx.h"
#include "gpio/init/gpio_init.h"

/* SPI2 BEGIN */
//dac7562

SPI_BUS_TypeDef SPI2_Bus;

void SPI2_IRQHandler() {
	SPI_BUS_IRQHandler(&SPI2_Bus);
}

void spi2_nvic_init(uint32_t priority) {
	NVIC_SetPriority(SPI2_IRQn, priority);
	NVIC_EnableIRQ(SPI2_IRQn);
}

void spi2_rcc_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
}

void spi2_bus_init(void) {
	gpio_spi2_cfg_setup();
	spi2_rcc_init();
	spi_bus_struct_init(&SPI2_Bus, SPI2);
}
/* SPI2 END */
