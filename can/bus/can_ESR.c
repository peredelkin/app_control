/*
 * can_ESR.c
 *
 *  Created on: 11 нояб. 2024 г.
 *      Author: Ruslan
 */

#include "can_bus.h"

uint32_t can_ESR_read(CAN_TypeDef *can) {
	return can->ESR;
}

uint32_t can_ESR_REC_read(uint32_t ESR) {
	return (ESR & CAN_ESR_REC) >> CAN_ESR_REC_SHIFT;
}

uint32_t can_ESR_TEC_read(uint32_t ESR) {
	return (ESR & CAN_ESR_TEC) >> CAN_ESR_TEC_SHIFT;
}

uint32_t can_ESR_LEC_read(uint32_t ESR) {
	return (ESR & CAN_ESR_LEC) >> CAN_ESR_LEC_SHIFT;
}

uint32_t can_ESR_BOFF_read(uint32_t ESR) {
	return ESR & CAN_ESR_BOFF;
}

uint32_t can_ESR_EPVF_read(uint32_t ESR) {
	return ESR & CAN_ESR_EPVF;
}

uint32_t can_ESR_EWGF_read(uint32_t ESR) {
	return ESR & CAN_ESR_EWGF;
}
