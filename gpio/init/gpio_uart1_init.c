/*
 * gpio_uart1_init.c
 *
 *  Created on: 25 мар. 2025 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define USART1_GPIO_COUNT 2

const gpio_pin_cfg_t gpio_uart1_cfg[USART1_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOA, GPIO_PIN_9, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_7, GPIO_STATE_ON),		/*120, PA9,		USART1_TX,		OFF,		UART_Tx_App_Jmp*/
		GPIO_PIN_CFG(GPIOA, GPIO_PIN_10, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_7, GPIO_STATE_OFF)		/*121, PA10,	USART1_RX,		OFF,		UART_Rx_App_Jmp*/
};

void gpio_uart1_cfg_setup() {
	gpio_pins_cfg_setup(gpio_uart1_cfg, USART1_GPIO_COUNT);
}
