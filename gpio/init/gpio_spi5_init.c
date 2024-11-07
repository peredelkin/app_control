/*
 * gpio_spi5_init.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define SPI5_GPIO_COUNT 3

const gpio_pin_cfg_t gpio_spi5_cfg[SPI5_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOF, GPIO_PIN_8, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_5, GPIO_STATE_OFF),		/*26, PF8,		SPI5_MISO,		OFF,		MISO_SPI5_App*/
		GPIO_PIN_CFG(GPIOF, GPIO_PIN_9, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_5, GPIO_STATE_OFF),		/*27, PF9,		SPI5_MOSI,		OFF,		MOSI_SPI5_App*/
		GPIO_PIN_CFG(GPIOH, GPIO_PIN_6, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_5, GPIO_STATE_OFF),		/*83, PH6,		SPI5_SCK,		OFF,		CLK_SPI5_App*/
};

void gpio_spi5_cfg_setup() {
	gpio_pins_cfg_setup(gpio_spi5_cfg, SPI5_GPIO_COUNT);
}
