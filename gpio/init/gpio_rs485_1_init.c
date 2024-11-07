/*
 * gpio_rs485_1_init.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define RS485_1_GPIO_COUNT 1

const gpio_pin_t gpio_rs485_1_dir = GPIO_PIN(GPIOH, GPIO_PIN_8); /*85, PH8, GPIO_Output, Dir1_485_App*/

void rs485_1_set_out() {
	gpio_output_bit_setup(&gpio_rs485_1_dir, GPIO_STATE_ON);
}

void rs485_1_set_in() {
	gpio_output_bit_setup(&gpio_rs485_1_dir, GPIO_STATE_OFF);
}

const gpio_pin_cfg_t gpio_rs485_1_cfg [RS485_1_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOH, GPIO_PIN_8, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),					/*85, PH8,		GPIO_Output,	OFF,	Dir1_485_App*/
};

void gpio_rs485_1_cfg_setup() {
	gpio_pins_cfg_setup(gpio_rs485_1_cfg, RS485_1_GPIO_COUNT);
}
