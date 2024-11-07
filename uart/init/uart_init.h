/*
 * uart_init.h
 *
 *  Created on: 26 окт. 2023 г.
 *      Author: Ruslan
 */

#ifndef USART_INIT_H_
#define USART_INIT_H_

#include <stdint.h>
//#include "uart/usart_bus.h"
//#include "uart/usart_buf.h"

typedef struct _UsartBus usart_bus_t;
typedef struct _UsartBuf usart_buf_t;
typedef struct _UsartInt usart_int_t;

extern usart_bus_t usart_3;
extern void usart3_nvic_init(uint32_t priority);
extern void usart3_init(void);

extern usart_buf_t usart_6;
extern void usart6_nvic_init(uint32_t priority);
extern void usart6_init();

extern usart_int_t uart_7;
extern void uart7_nvic_init(uint32_t priority);
extern void uart7_init();

#endif /* UART_BUS_INIT_H_ */
