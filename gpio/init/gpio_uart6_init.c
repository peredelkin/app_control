/*
 * gpio_uart6_init.c
 *
 *  Created on: 23 июл. 2024 г.
 *      Author: Ruslan
 */

#include "gpio_init.h"

#define USART6_GPIO_COUNT 2

const gpio_pin_cfg_t gpio_uart6_cfg[USART6_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOC, GPIO_PIN_6, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_UP, GPIO_AF_8, GPIO_STATE_ON),		/*115, PC6,		USART6_TX,		OFF,		Tx_UART6_App*/
		GPIO_PIN_CFG(GPIOC, GPIO_PIN_7, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_UP, GPIO_AF_8, GPIO_STATE_OFF)		/*116, PC7,		USART6_RX,		OFF,		Rx_UART6_App*/
};

void gpio_uart6_cfg_setup() {
	gpio_pins_cfg_setup(gpio_uart6_cfg, USART6_GPIO_COUNT);
}
