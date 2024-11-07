/*
 * gpio_spi4_init.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define SPI4_GPIO_COUNT 3

const gpio_pin_cfg_t gpio_spi4_cfg[SPI4_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOE, GPIO_PIN_2, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_UP, GPIO_AF_5, GPIO_STATE_OFF),		/*1, PE2,		SPI4_SCK,		OFF,		SCLK_DI_App*/
		GPIO_PIN_CFG(GPIOE, GPIO_PIN_5, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_5, GPIO_STATE_OFF),		/*4, PE5,		SPI4_MISO,		OFF,		MISO_DI_App*/
		GPIO_PIN_CFG(GPIOE, GPIO_PIN_6, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_5, GPIO_STATE_OFF),		/*5, PE6,		SPI4_MOSI,		OFF,		MOSI_DI_App*/
};

void gpio_spi4_cfg_setup() {
	gpio_pins_cfg_setup(gpio_spi4_cfg, SPI4_GPIO_COUNT);
}
