/*
 * init.c
 *
 *  Created on: 31 авг. 2022 г.
 *      Author: Ruslan
 */

#include "init.h"

void gpio_rcc_init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
}

void rcc_init(void) {
	//TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	//TIM3 SYS_TIM
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //TODO: перенести в init SYS_TIM

	//TIM4 MS_TIM
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //TODO: перенести в инит MS_TIM

	//TIM5
	//RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	gpio_rcc_init();

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
	NVIC_SetPriority(TIM2_IRQn, 6);
	NVIC_EnableIRQ(TIM2_IRQn);

	NVIC_SetPriority(SYS_TIM_IRQN, SYS_TIM_IRQ_PRIO);
	NVIC_EnableIRQ(SYS_TIM_IRQN);

	NVIC_SetPriority(MS_TIM_IRQN, MS_TIM_IRQ_PRIO);
	NVIC_EnableIRQ(MS_TIM_IRQN);

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
	sys_counter_init(TIM2);
	sys_counter_irq_enable();
	sys_counter_start();
}
