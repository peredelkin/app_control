/*
 * init.h
 *
 *  Created on: 31 авг. 2022 г.
 *      Author: Ruslan
 */

#ifndef INC_INIT_H_
#define INC_INIT_H_

#include <sys_counter/sys_counter.h>
#include "stm32f4xx/stm32f4xx.h"

#define SYS_TIM_TIM TIM3
#define SYS_TIM_IRQN TIM3_IRQn
#define SYS_TIM_IRQ_PRIO 7
#define SYS_TIM_IRQHANDLER TIM3_IRQHandler

#define MS_TIM_TIM TIM4
#define MS_TIM_IRQN TIM4_IRQn
#define MS_TIM_IRQ_PRIO 8
#define MS_TIM_IRQHANDLER TIM4_IRQHandler

extern void rcc_init(void);
extern void nvic_init(void);
extern void system_counter_init(void);

#endif /* INC_INIT_H_ */
