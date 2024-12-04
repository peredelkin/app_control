/*
 * gpio_em_stop_init.c
 *
 *  Created on: 4 дек. 2024 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define EM_STOP_INIT_PINS_COUNT 1

const gpio_pin_t GPI_EmStop_App = GPIO_PIN(GPIOC, GPIO_PIN_3); /*35, PC3, GPIO_Input, EmStop_App*/

const gpio_pin_cfg_t em_stop_gpio_cfg[EM_STOP_INIT_PINS_COUNT] = {
		GPIO_PIN_CFG(GPIOC, GPIO_PIN_3, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF)					/*35, PC3,		GPIO_Input,		OFF,		EmStop_App*/
};

void gpio_em_stop_cfg_setup() {
	gpio_pins_cfg_setup(em_stop_gpio_cfg, EM_STOP_INIT_PINS_COUNT);
}
