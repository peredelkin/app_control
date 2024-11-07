/*
 * gpio_uart3_init.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define USART3_GPIO_COUNT 2

const gpio_pin_cfg_t gpio_uart3_cfg[USART3_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_10, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_7, GPIO_STATE_OFF),					/*79, PB10,		USART3_TX,		OFF,		Tx1_485_App*/
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_7, GPIO_STATE_OFF)					/*80, PB11,		USART3_RX,		OFF,		Rx1_485_App*/
};

void gpio_uart3_cfg_setup() {
	gpio_pins_cfg_setup(gpio_uart3_cfg, USART3_GPIO_COUNT);
}
