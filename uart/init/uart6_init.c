/*
 * uart6_init.c
 *
 *  Created on: 23 июл. 2024 г.
 *      Author: Ruslan
 */

#include "uart_init.h"
#include "gpio/init/gpio_init.h"

#define USART_STDIO
#include "../buf/usart_buf.h"
#include "uart/usart.h"

/* USART6 BEGIN */

usart_buf_t usart_6;

#define USART_WRITE_BUFFER_SIZE 128
static uint8_t usart_write_buffer[USART_WRITE_BUFFER_SIZE];

#define USART_READ_BUFFER_SIZE 128
static uint8_t usart_read_buffer[USART_READ_BUFFER_SIZE];

void USART6_IRQHandler() {
	usart_buf_irq_handler(&usart_6);
}

void usart6_nvic_init(uint32_t priority) {
	NVIC_SetPriority(USART6_IRQn, priority);
	NVIC_EnableIRQ(USART6_IRQn);
}

void usart6_rcc_init() {
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
}

void usart6_buf_init() {
	usart_baud_rate_set(USART6,/*90000000*/SystemCoreClock/2, 115200);
	usart_buf_tx_rx_enable(USART6, true, true);
	usart_buf_enable(USART6, true);

	usart_buf_init_t usart6_is = {
			.usart = USART6,
			.write_buffer = usart_write_buffer,
			.write_buffer_size = USART_WRITE_BUFFER_SIZE,
			.read_buffer = usart_read_buffer,
			.read_buffer_size = USART_READ_BUFFER_SIZE
	};

	usart_buf_init(&usart_6, &usart6_is);
	usart_setup_stdio(&usart_6);
}

void usart6_init() {
	gpio_uart6_cfg_setup();
	usart6_rcc_init();
	usart6_buf_init();
}

/* USART6 END */
