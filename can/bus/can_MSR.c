/*
 * can_MSR.c
 *
 *  Created on: 11 нояб. 2024 г.
 *      Author: Ruslan
 */

#include "can_bus.h"

void can_MSR_SLAKI_clear(CAN_TypeDef* can) {
	can->MSR = CAN_MSR_SLAKI;
}

uint32_t can_MSR_SLAKI_read(CAN_TypeDef* can) {
	return can->MSR & CAN_MSR_SLAKI;
}

void can_MSR_WKUI_clear(CAN_TypeDef* can) {
	can->MSR = CAN_MSR_WKUI;
}

uint32_t can_MSR_WKUI_read(CAN_TypeDef* can) {
	return can->MSR & CAN_MSR_WKUI;
}

void can_MSR_ERRI_clear(CAN_TypeDef* can) {
	can->MSR = CAN_MSR_ERRI;
}

uint32_t can_MSR_ERRI_read(CAN_TypeDef* can) {
	return can->MSR & CAN_MSR_ERRI;
}

uint32_t can_MSR_SLAK_read(CAN_TypeDef* can) {
	return can->MSR & CAN_MSR_SLAK;
}

uint32_t can_MSR_INAK_read(CAN_TypeDef* can) {
	return can->MSR & CAN_MSR_INAK;
}
