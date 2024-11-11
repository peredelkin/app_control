/*
 * can_TSR.c
 *
 *  Created on: 7 нояб. 2024 г.
 *      Author: Ruslan
 */

#include "can_bus.h"

uint32_t can_TSR_read(CAN_TypeDef* can) {
	return can->TSR;
}

enum {
	CAN_TX_MAILBOX_ERR = -1,
	CAN_TX_MAILBOX_0,
	CAN_TX_MAILBOX_1,
	CAN_TX_MAILBOX_2
};

int can_TSR_LOW_get(uint32_t TSR) {
	if(TSR & CAN_TSR_LOW0)
		return CAN_TX_MAILBOX_0;
	if(TSR & CAN_TSR_LOW1)
		return CAN_TX_MAILBOX_1;
	if(TSR & CAN_TSR_LOW2)
		return CAN_TX_MAILBOX_2;

	return CAN_TX_MAILBOX_ERR;
}

int can_TSR_TME_get(uint32_t TSR) {
	if (TSR & CAN_TSR_TME0)
		return CAN_TX_MAILBOX_0;
	if (TSR & CAN_TSR_TME1)
		return CAN_TX_MAILBOX_1;
	if (TSR & CAN_TSR_TME2)
		return CAN_TX_MAILBOX_2;

	return CAN_TX_MAILBOX_ERR;
}

//TODO: разобраться, зачем и как работает
int can_TSR_CODE_get(uint32_t TSR) {
	switch(TSR & CAN_TSR_CODE) {
	case (0 << CAN_TIR_CODE_SHIFT):
			return CAN_TX_MAILBOX_0;
	case (1 << CAN_TIR_CODE_SHIFT):
			return CAN_TX_MAILBOX_1;
	case (2 << CAN_TIR_CODE_SHIFT):
			return CAN_TX_MAILBOX_2;
	default:
		return CAN_TX_MAILBOX_ERR;
	}
}

void can_TSR_ABRQ_set(CAN_TypeDef* can, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		can->TSR = CAN_TSR_ABRQ0;
		break;

	case CAN_TX_MAILBOX_1:
		can->TSR = CAN_TSR_ABRQ1;
		break;

	case CAN_TX_MAILBOX_2:
		can->TSR = CAN_TSR_ABRQ2;
		break;

	default: return;
	}
}

bool can_TSR_TERR_get(uint32_t TSR, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		if(TSR & CAN_TSR_TERR0) return true;
		else return false;

	case CAN_TX_MAILBOX_1:
		if(TSR & CAN_TSR_TERR1) return true;
		else return false;

	case CAN_TX_MAILBOX_2:
		if(TSR & CAN_TSR_TERR2) return true;
		else return false;

	default: return false;
	}
}

void can_TSR_TERR_set(CAN_TypeDef* can, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		can->TSR = CAN_TSR_TERR0;
		break;

	case CAN_TX_MAILBOX_1:
		can->TSR = CAN_TSR_TERR1;
		break;

	case CAN_TX_MAILBOX_2:
		can->TSR = CAN_TSR_TERR2;
		break;

	default: return;
	}
}

bool can_TSR_ALST_get(uint32_t TSR, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		if(TSR & CAN_TSR_ALST0) return true;
		else return false;

	case CAN_TX_MAILBOX_1:
		if(TSR & CAN_TSR_ALST1) return true;
		else return false;

	case CAN_TX_MAILBOX_2:
		if(TSR & CAN_TSR_ALST2) return true;
		else return false;

	default: return false;
	}
}

void can_TSR_ALST_set(CAN_TypeDef* can, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		can->TSR = CAN_TSR_ALST0;
		break;

	case CAN_TX_MAILBOX_1:
		can->TSR = CAN_TSR_ALST1;
		break;

	case CAN_TX_MAILBOX_2:
		can->TSR = CAN_TSR_ALST2;
		break;

	default: return;
	}
}

bool can_TSR_TXOK_get(uint32_t TSR, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		if(TSR & CAN_TSR_TXOK0) return true;
		else return false;

	case CAN_TX_MAILBOX_1:
		if(TSR & CAN_TSR_TXOK1) return true;
		else return false;

	case CAN_TX_MAILBOX_2:
		if(TSR & CAN_TSR_TXOK2) return true;
		else return false;

	default: return false;
	}
}

void can_TSR_TXOK_clear(CAN_TypeDef* can, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		can->TSR = CAN_TSR_TXOK0;
		break;

	case CAN_TX_MAILBOX_1:
		can->TSR = CAN_TSR_TXOK1;
		break;

	case CAN_TX_MAILBOX_2:
		can->TSR = CAN_TSR_TXOK2;
		break;

	default: return;
	}
}

bool can_TSR_RQCP_get(uint32_t TSR, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		if(TSR & CAN_TSR_RQCP0) return true;
		else return false;

	case CAN_TX_MAILBOX_1:
		if(TSR & CAN_TSR_RQCP1) return true;
		else return false;

	case CAN_TX_MAILBOX_2:
		if(TSR & CAN_TSR_RQCP2) return true;
		else return false;

	default: return false;
	}
}

void can_TSR_RQCP_clear(CAN_TypeDef* can, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		can->TSR = CAN_TSR_RQCP0;
		break;

	case CAN_TX_MAILBOX_1:
		can->TSR = CAN_TSR_RQCP1;
		break;

	case CAN_TX_MAILBOX_2:
		can->TSR = CAN_TSR_RQCP2;
		break;

	default: return;
	}
}














