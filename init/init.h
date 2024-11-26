/*
 * init.h
 *
 *  Created on: 31 авг. 2022 г.
 *      Author: Ruslan
 */

#ifndef INC_INIT_H_
#define INC_INIT_H_

#include "sys/counter/sys_counter.h"
#include "lib/stm32f4xx/stm32f4xx.h"

extern void rcc_init(void);
extern void nvic_init(void);
extern void system_counter_init(void);

#endif /* INC_INIT_H_ */
