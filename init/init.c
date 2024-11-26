/*
 * init.c
 *
 *  Created on: 31 авг. 2022 г.
 *      Author: Ruslan
 */

#include "init.h"

void rcc_init(void) {
	//TIM5
	//RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	//DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
}

#include "uart/init/uart_init.h"
#include "spi/init/spi_init.h"
#include "can/init/can_init.h"

void nvic_init(void) {
	NVIC_SetPriorityGrouping(0b000);

	//TIM
	NVIC_SetPriority(TIM5_IRQn, 6);
	NVIC_EnableIRQ(TIM5_IRQn);

	//UART3
	usart3_nvic_init(5);

	//UART6
	usart6_nvic_init(5);

	//UART7
	uart7_nvic_init(5);

	//SPI2
	spi2_nvic_init(4);

	//SPI4
	spi4_nvic_init(4);

	//CAN1
	can1_nvic_init(5);
}

void system_counter_init(void) {
	sys_counter_init(SYS_CNT_TIM);
	sys_counter_irq_enable();
	NVIC_SetPriority(SYS_CNT_IRQN, SYS_CNT_IRQ_PRIO);
	NVIC_EnableIRQ(SYS_CNT_IRQN);
	sys_counter_start();
}
