/*
 * uart3_init.c
 *
 *  Created on: 23 июл. 2024 г.
 *      Author: Ruslan
 */

#include "uart_init.h"
#include "gpio/init/gpio_init.h"
#include "modbus/init/modbus_init.h"

#include "../bus/usart_bus.h"
#include "uart/usart.h"

/* UART3 BEGIN */
//RS485_1

usart_bus_t usart_3;

void USART3_IRQHandler() {
	usart_bus_irq_handler(&usart_3);
}

void DMA1_Stream1_IRQHandler() {
	usart_bus_dma_rx_channel_irq_handler(&usart_3);
}

void DMA1_Stream3_IRQHandler() {
	usart_bus_dma_tx_channel_irq_handler(&usart_3);
}

void usart3_nvic_init(uint32_t priority) {
	NVIC_SetPriority(USART3_IRQn, priority);
	NVIC_EnableIRQ(USART3_IRQn);
	//RX
	NVIC_SetPriority(DMA1_Stream1_IRQn, priority);
	NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	//TX
	NVIC_SetPriority(DMA1_Stream3_IRQn, priority);
	NVIC_EnableIRQ(DMA1_Stream3_IRQn);
}

void usart3_rcc_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
}

bool usart3_tc_callback(void)
{
	rs485_1_set_in();
    return true;
}

void usart3_bus_init() {
    usart_3.usart_device = USART3;

    usart_baud_rate_set(usart_3.usart_device,/*45000000*/SystemCoreClock/4, 9600);

    usart_bus_transmitter_enable(&usart_3);

    usart_bus_receiver_enable(&usart_3);

    dma_stream_struct_init(&usart_3.dma_rx_channel,DMA1,DMA1_Stream1,1);
    dma_stream_struct_init(&usart_3.dma_tx_channel,DMA1,DMA1_Stream3,3);

    usart_3.dma_rx_channel_n = 4;
    usart_3.dma_tx_channel_n = 4;

    usart_bus_init(&usart_3);

    // Установка каллбэков.
    usart_bus_set_rx_byte_callback(&usart_3, modbus1_rx_byte_callback);
    usart_bus_set_rx_callback(&usart_3, modbus1_rx_callback);
    usart_bus_set_tx_callback(&usart_3, modbus1_tx_callback);
    usart_bus_set_tc_callback(&usart_3, usart3_tc_callback);

    // При обнаружении свободной линии - прекратить принимать данные.
    usart_bus_set_idle_mode(&usart_3, USART_IDLE_MODE_END_RX);
}

void usart3_init(void) {
	gpio_rs485_1_cfg_setup();
	gpio_uart3_cfg_setup();
	usart3_rcc_init();
	usart3_bus_init();
}

/* UART3 END */
