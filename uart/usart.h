/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   usart.h
 * Author: ruslan
 *
 * Created on 29 июля 2018 г., 15:44
 */

#ifndef USART_H
#define USART_H

#include <stm32f4xx.h>
#include <stdbool.h>

extern void usart_sr_clr(USART_TypeDef* usart, uint16_t mask);
extern bool usart_sr_state(USART_TypeDef* usart, uint16_t mask);

extern void usart_cr1_set(USART_TypeDef* usart, uint16_t mask, bool state);
extern bool usart_cr1_state(USART_TypeDef* usart, uint16_t mask);

extern void usart_cr2_set(USART_TypeDef* usart, uint16_t mask, bool state);
extern bool usart_cr2_state(USART_TypeDef* usart, uint16_t mask);

extern void usart_cr3_set(USART_TypeDef* usart, uint16_t mask, bool state);
extern bool usart_bus_cr3_state(USART_TypeDef* usart, uint16_t mask);

extern bool usart_transmitter_state(USART_TypeDef* usart);
extern bool usart_receiver_state(USART_TypeDef* usart);
extern bool usart_halfduplex_state(USART_TypeDef* usart);

extern void usart_baud_rate_set(USART_TypeDef* usart, uint32_t fpclk, uint32_t baud);

#endif /* USART_H */

