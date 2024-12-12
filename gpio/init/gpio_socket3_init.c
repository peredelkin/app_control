/*
 * gpio_socket3_init.c
 *
 *  Created on: 24 июл. 2024 г.
 *      Author: Ruslan
 */

#include "gpio_init.h"

#define SOCKET3_GPIO_COUNT 1

const gpio_pin_t GPO_OE_App = GPIO_PIN(GPIOI, GPIO_PIN_0); /*131, PI0, GPIO_Output, OE_App*/

const gpio_pin_cfg_t gpio_socket3_cfg[SOCKET3_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOI, GPIO_PIN_0, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_ON),		/*131, PI0,		GPIO_Output,	ON,		OE_App*/
};

void gpio_socket3_cfg_setup() {
	gpio_pins_cfg_setup(gpio_socket3_cfg, SOCKET3_GPIO_COUNT);
}
