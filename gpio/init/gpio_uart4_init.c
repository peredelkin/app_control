/*
 * gpio_uart4_init.c
 *
 *  Created on: 25 мар. 2025 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define USART4_GPIO_COUNT 2

const gpio_pin_cfg_t gpio_uart4_cfg[USART4_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOC, GPIO_PIN_10, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_8, GPIO_STATE_ON),		/*139, PC10,	UART4_TX,		OFF,		Tx2_485_App_Jmp*/
		GPIO_PIN_CFG(GPIOC, GPIO_PIN_11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_8, GPIO_STATE_OFF)		/*140, PC11,	UART4_RX,		OFF,		Rx2_485_App_Jmp*/
};

void gpio_uart4_cfg_setup() {
	gpio_pins_cfg_setup(gpio_uart4_cfg, USART4_GPIO_COUNT);
}
