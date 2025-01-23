/*
 * interrupt_priorities.h
 *
 *  Created on: 23 янв. 2025 г.
 *      Author: ruslan
 */

#ifndef INTERRUPTS_INTERRUPT_PRIORITIES_H_
#define INTERRUPTS_INTERRUPT_PRIORITIES_H_

#define TIM2_IRQ_PRIO 1		//SYS_CNT

#define SPI2_IRQ_PRIO 4		//SPI2
#define SPI4_IRQ_PRIO 4		//SPI4

#define UART3_IRQ_PRIO 5	//UART3 + DMA Stream
#define UART6_IRQ_PRIO 5	//UART6
#define UART7_IRQ_PRIO 5	//UART7
#define CAN1_IRQ_PRIO 5		//CAN1

#define TIM4_IRQ_PRIO 7		//MS_TIM
#define TIM3_IRQ_PRIO 8		//SYS_TIM


#endif /* INTERRUPTS_INTERRUPT_PRIORITIES_H_ */
