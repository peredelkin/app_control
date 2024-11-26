/*
 * spi5_init.c
 *
 *  Created on: 19 июл. 2024 г.
 *      Author: Ruslan
 */

#include "spi_init.h"
#include "lib/stm32f4xx/stm32f4xx.h"
#include "gpio/init/gpio_init.h"

/* SPI5 BEGIN */


SPI_BUS_TypeDef SPI5_Bus;

void SPI5_IRQHandler() {
	SPI_BUS_IRQHandler(&SPI5_Bus);
}

void spi5_rcc_init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
}

void spi5_bus_init(void) {
	gpio_spi5_cfg_setup();
	spi5_rcc_init();
	spi_bus_struct_init(&SPI5_Bus, SPI5);
}
/* SPI5 END */
