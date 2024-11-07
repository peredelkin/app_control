/*
 * uart7_init.c
 *
 *  Created on: 9 авг. 2024 г.
 *      Author: Ruslan
 */

#include "uart_init.h"
#include "gpio/init/gpio_init.h"
#include "modbus/init/modbus_init.h"

#include "../int/usart_int.h"
#include "uart/usart.h"

/* UART3 BEGIN */
//RS485_Panel

usart_int_t uart_7;

void UART7_IRQHandler() {
	usart_int_irq_handler(&uart_7);
}

void uart7_nvic_init(uint32_t priority) {
	NVIC_SetPriority(UART7_IRQn, priority);
	NVIC_EnableIRQ(UART7_IRQn);
}

void uart7_rcc_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_UART7EN;
}

bool uart7_tc_callback(void)
{
	rs485_panel_set_in();
    return true;
}

void uart7_int_init() {
    uart_7.usart_device = UART7;

    usart_baud_rate_set(uart_7.usart_device,/*45000000*/SystemCoreClock/4, 9600);

    usart_int_transmitter_enable(&uart_7);

    usart_int_receiver_enable(&uart_7);

    usart_int_init(&uart_7);

    // Установка каллбэков.
    usart_int_set_rx_byte_callback(&uart_7, modbus_panel_rx_byte_callback);
    usart_int_set_rx_callback(&uart_7, modbus_panel_rx_callback);
    usart_int_set_tx_callback(&uart_7, modbus_panel_tx_callback);
    usart_int_set_tc_callback(&uart_7, uart7_tc_callback);

    // При обнаружении свободной линии - прекратить принимать данные.
    usart_int_set_idle_mode(&uart_7, USART_INT_IDLE_MODE_END_RX);
}

void uart7_init(void) {
	gpio_rs485_panel_cfg_setup();
	gpio_uart7_cfg_setup();
	uart7_rcc_init();
	uart7_int_init();
}
