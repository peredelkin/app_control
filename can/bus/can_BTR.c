/*
 * can_BTR.c
 *
 *  Created on: 29 окт. 2024 г.
 *      Author: Ruslan
 */

#include "can_bus.h"

void can_BTR_SJW_set(CAN_TypeDef* can, uint32_t btr) {
	can->BTR &= ~CAN_BTR_SJW;
	can->BTR |= CAN_BTR_SJW & btr;
}

void can_BTR_TS2_set(CAN_TypeDef* can, uint32_t btr) {
	can->BTR &= ~CAN_BTR_TS2;
	can->BTR |= CAN_BTR_TS2 & btr;
}

void can_BTR_TS1_set(CAN_TypeDef* can, uint32_t btr) {
	can->BTR &= ~CAN_BTR_TS1;
	can->BTR |= CAN_BTR_TS1 & btr;
}

void can_BTR_BRP_set(CAN_TypeDef* can, uint32_t btr) {
	can->BTR &= ~CAN_BTR_BRP;
	can->BTR |= CAN_BTR_BRP & btr;
}

void can_BTR_set(CAN_TypeDef* can, uint32_t btr) {
	can_BTR_BRP_set(can, btr);
	can_BTR_TS1_set(can, btr);
	can_BTR_TS2_set(can, btr);
	can_BTR_SJW_set(can, btr);
}
