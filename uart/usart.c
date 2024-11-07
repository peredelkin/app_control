/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "usart.h"

//UART SR
void usart_sr_clr(USART_TypeDef* usart, uint16_t mask) {
	usart->SR &= ~mask;
}

bool usart_sr_state(USART_TypeDef* usart, uint16_t mask) {
	if (usart->SR & mask) return ENABLE;
	return DISABLE;
}

//UART CR1
void usart_cr1_set(USART_TypeDef* usart, uint16_t mask, bool state) {
	if(state) {
		usart->CR1 |= mask;
	} else {
		usart->CR1 &= ~mask;
	}
}

bool usart_cr1_state(USART_TypeDef* usart, uint16_t mask) {
    if (usart->CR1 & mask) return ENABLE;
    return DISABLE;
}

//UART CR2
void usart_cr2_set(USART_TypeDef* usart, uint16_t mask, bool state) {
	if(state) {
		usart->CR2 |= mask;
	} else {
		usart->CR2 &= ~mask;
	}
}

bool usart_cr2_state(USART_TypeDef* usart, uint16_t mask) {
    if (usart->CR2 & mask) return ENABLE;
    return DISABLE;
}

//UART CR3
void usart_cr3_set(USART_TypeDef* usart, uint16_t mask, bool state) {
	if(state) {
		usart->CR3 |= mask;
	} else {
		usart->CR3 &= ~mask;
	}
}

bool usart_bus_cr3_state(USART_TypeDef* usart, uint16_t mask) {
    if (usart->CR3 & mask) return ENABLE;
    return DISABLE;
}

//UART RE/TE/HD STATE
bool usart_transmitter_state(USART_TypeDef* usart) {
	return usart_cr1_state(usart, USART_CR1_TE);
}

bool usart_receiver_state(USART_TypeDef* usart) {
	return usart_cr1_state(usart, USART_CR1_RE);
}

bool usart_halfduplex_state(USART_TypeDef* usart) {
	return usart_bus_cr3_state(usart, USART_CR3_HDSEL);
}

//UART Baud Rate
bool usart_oversampling_mode_read(USART_TypeDef* usart) {
    return usart_cr1_state(usart, USART_CR1_OVER8);
}

void usart_baud_rate_register_set(USART_TypeDef* usart, uint16_t mantissa, uint16_t fraction) {
    usart->BRR = ((uint16_t) (mantissa << 4) & (uint16_t) USART_BRR_DIV_Mantissa) |
            ((uint16_t) fraction & (uint16_t) USART_BRR_DIV_Fraction);
}

void usart_baud_rate_set(USART_TypeDef* usart, uint32_t fpclk, uint32_t baud) {
    uint32_t over8 = (uint32_t) usart_oversampling_mode_read(usart);
    uint32_t mantissa = (fpclk / ((8 * (2 - over8)) * baud));
    uint16_t divider = (uint16_t) (fpclk / baud);
    uint16_t fraction = (uint16_t) (divider - (uint16_t) (mantissa << 4));
    if (over8) {
    	usart_baud_rate_register_set(usart, mantissa, (fraction & (uint16_t) 0x07));
    } else {
        usart_baud_rate_register_set(usart, mantissa, fraction);
    }
}
