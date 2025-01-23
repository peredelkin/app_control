/*
 * interrupt_priorities.h
 *
 *  Created on: 23 янв. 2025 г.
 *      Author: ruslan
 */

#ifndef INTERRUPTS_INTERRUPT_PRIORITIES_H_
#define INTERRUPTS_INTERRUPT_PRIORITIES_H_

#define TIM2_IRQ_PRIO 1		//SYS_CNT

#define SPI2_IRQ_PRIO 3		//SPI2
#define SPI4_IRQ_PRIO 3		//SPI4

#define CAN1_IRQ_PRIO 5		//Socket 3
#define UART6_IRQ_PRIO 5	//Socket 3
#define UART7_IRQ_PRIO 5	//RS485 Panel

#define UART3_IRQ_PRIO 7	//UART3 + DMA Stream

#define TIM4_IRQ_PRIO 8		//MS_TIM

#define TIM3_IRQ_PRIO 9		//SYS_TIM

#define TIM5_IRQ_PRIO 15	//CAN1 CO_process


#endif /* INTERRUPTS_INTERRUPT_PRIORITIES_H_ */
