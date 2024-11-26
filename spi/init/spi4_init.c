/*
 * spi4_init.c
 *
 *  Created on: 19 июл. 2024 г.
 *      Author: Ruslan
 */

#include "spi_init.h"
#include "lib/stm32f4xx/stm32f4xx.h"
#include "gpio/init/gpio_init.h"

/* SPI4 BEGIN */
//tic12400
//ncv7608
SPI_BUS_TypeDef SPI4_Bus;

void SPI4_IRQHandler() {
	SPI_BUS_IRQHandler(&SPI4_Bus);
}

void spi4_nvic_init(uint32_t priority) {
	NVIC_SetPriority(SPI4_IRQn, priority);
	NVIC_EnableIRQ(SPI4_IRQn);
}

void spi4_rcc_init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
}

void spi4_bus_init(void) {
	gpio_spi4_cfg_setup();
	spi4_rcc_init();
	spi_bus_struct_init(&SPI4_Bus, SPI4);
}
/* SPI4 END*/
