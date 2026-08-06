/*
 * exti.h
 *
 *  Created on: 5 авг. 2026 г.
 *      Author: ruslan
 */

#ifndef EXTI_EXTI_H_
#define EXTI_EXTI_H_

#include "gpio/init/gpio_init.h"

#define EXTI15_ENABLED

extern void(*exti_callback [])();

extern void exti15_10_init(uint32_t priority);

#endif /* EXTI_EXTI_H_ */
