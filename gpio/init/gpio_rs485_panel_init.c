/*
 * gpio_rs485_panel_init.c
 *
 *  Created on: 9 авг. 2024 г.
 *      Author: Ruslan
 */

#include "gpio_init.h"

#define RS485_PANEL_GPIO_COUNT 2

const gpio_pin_t gpio_rs485_panel_detect = GPIO_PIN(GPIOH, GPIO_PIN_9); /*86, PH9, GPIO_Input, Panel_Detect_App*/
const gpio_pin_t gpio_rs485_panel_dir = GPIO_PIN(GPIOB, GPIO_PIN_6); /*164, PB6, GPIO_Output, Dir_Panel_App*/


void rs485_panel_set_out() {
	gpio_output_bit_setup(&gpio_rs485_panel_dir, GPIO_STATE_ON);
}

void rs485_panel_set_in() {
	gpio_output_bit_setup(&gpio_rs485_panel_dir, GPIO_STATE_OFF);
}

const gpio_pin_cfg_t gpio_rs485_panel_cfg [RS485_PANEL_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOH, GPIO_PIN_9, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),					/*86, PH9,		GPIO_Input,		OFF,		Panel_Detect_App*/
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF)					/*164, PB6,		GPIO_Output,	OFF,	Dir_Panel_App*/
};

void gpio_rs485_panel_cfg_setup() {
	gpio_pins_cfg_setup(gpio_rs485_panel_cfg, RS485_PANEL_GPIO_COUNT);
}

