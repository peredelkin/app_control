/*
 * gpio_rs485_2_dir_init.c
 *
 *  Created on: 25 мар. 2025 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define RS485_2_GPIO_COUNT 1

const gpio_pin_t gpio_rs485_2_dir = GPIO_PIN(GPIOD, GPIO_PIN_3); /*145, PD3, GPIO_Output, OFF, Dir2_485_App_Jmp*/

void rs485_2_set_out() {
	gpio_output_bit_setup(&gpio_rs485_2_dir, GPIO_STATE_ON);
}

void rs485_2_set_in() {
	gpio_output_bit_setup(&gpio_rs485_2_dir, GPIO_STATE_OFF);
}

const gpio_pin_cfg_t gpio_rs485_2_cfg [RS485_2_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF) /*145, PD3,		GPIO_Output,	OFF,	Dir2_485_App_Jmp*/
};

void gpio_rs485_2_cfg_setup() {
	gpio_pins_cfg_setup(gpio_rs485_2_cfg, RS485_2_GPIO_COUNT);
}
