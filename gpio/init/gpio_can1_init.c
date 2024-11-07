/*
 * gpio_can1_init.c
 *
 *  Created on: 21 авг. 2024 г.
 *      Author: Ruslan
 */

#include "gpio_init.h"

#define CAN1_GPIO_COUNT 2

const gpio_pin_cfg_t gpio_can1_cfg[CAN1_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_8, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_9, GPIO_STATE_OFF),					/*167, PB8,		CAN1_RX,		OFF,		APP_CAN_RX*/
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_9, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_9, GPIO_STATE_OFF)					/*168, PB9,		CAN1_TX,		OFF,		APP_CAN_TX*/
};

void gpio_can1_cfg_setup() {
	gpio_pins_cfg_setup(gpio_can1_cfg, CAN1_GPIO_COUNT);
}


