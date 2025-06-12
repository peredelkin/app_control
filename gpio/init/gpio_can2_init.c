/*
 * gpio_can2_init.c
 *
 *  Created on: 5 июн. 2025 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define CAN2_GPIO_COUNT 2

const gpio_pin_cfg_t gpio_can2_cfg[CAN2_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_9, GPIO_STATE_OFF),				/*92, PB12,		CAN2_RX,		OFF,		-*/
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_13, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_9, GPIO_STATE_OFF)					/*93, PB13,		CAN2_TX,		OFF,		-*/
};

void gpio_can2_cfg_setup() {
	gpio_pins_cfg_setup(gpio_can2_cfg, CAN2_GPIO_COUNT);
}
