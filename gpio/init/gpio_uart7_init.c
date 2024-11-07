/*
 * gpio_uart7_init.c
 *
 *  Created on: 9 авг. 2024 г.
 *      Author: Ruslan
 */

#include "gpio_init.h"

#define USART7_GPIO_COUNT 2

const gpio_pin_cfg_t gpio_uart7_cfg[USART7_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOF, GPIO_PIN_6, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_8, GPIO_STATE_OFF),					/*24, PF6,		UART7_RX,		OFF,		Rx_Panel_App*/
		GPIO_PIN_CFG(GPIOF, GPIO_PIN_7, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_8, GPIO_STATE_ON)					/*25, PF7,		UART7_TX,		OFF,		Tx_Panel_App*/
};

void gpio_uart7_cfg_setup() {
	gpio_pins_cfg_setup(gpio_uart7_cfg, USART7_GPIO_COUNT);
}


