/*
 * can_IER.c
 *
 *  Created on: 29 окт. 2024 г.
 *      Author: Ruslan
 */

#include "can_bus.h"


//SLKIE: Sleep interrupt enable
void can_IER_SLKIE_set(CAN_TypeDef* can, bool state) {
	if(state) {
		can->IER |= CAN_IER_SLKIE;
	} else {
		can->IER &= ~CAN_IER_SLKIE;
	}
}

uint32_t can_IER_SLKIE_read(CAN_TypeDef* can) {
	return can->IER & CAN_IER_SLKIE;
}

//WKUIE: Wakeup interrupt enable
void can_IER_WKUIE_set(CAN_TypeDef* can, bool state) {
	if(state) {
		can->IER |= CAN_IER_WKUIE;
	} else {
		can->IER &= ~CAN_IER_WKUIE;
	}
}

uint32_t can_IER_WKUIE_read(CAN_TypeDef* can) {
	return can->IER & CAN_IER_WKUIE;
}

//ERRIE: Error interrupt enable
void can_IER_ERRIE_set(CAN_TypeDef* can, bool state) {
	if(state) {
		can->IER |= CAN_IER_ERRIE;
	} else {
		can->IER &= ~CAN_IER_ERRIE;
	}
}

uint32_t can_IER_ERRIE_read(CAN_TypeDef* can) {
	return can->IER & CAN_IER_ERRIE;
}

//LECIE: Last error code interrupt enable
void can_IER_LECIE_set(CAN_TypeDef* can, bool state) {
	if(state) {
		can->IER |= CAN_IER_LECIE;
	} else {
		can->IER &= ~CAN_IER_LECIE;
	}
}

uint32_t can_IER_LECIE_read(CAN_TypeDef* can) {
	return can->IER & CAN_IER_LECIE;
}

//BOFIE: Bus-off interrupt enable
void can_IER_BOFIE_set(CAN_TypeDef* can, bool state) {
	if(state) {
		can->IER |= CAN_IER_BOFIE;
	} else {
		can->IER &= ~CAN_IER_BOFIE;
	}
}

uint32_t can_IER_BOFIE_read(CAN_TypeDef* can) {
	return can->IER & CAN_IER_BOFIE;
}

//EPVIE: Error passive interrupt enable
void can_IER_EPVIE_set(CAN_TypeDef* can, bool state) {
	if(state) {
		can->IER |= CAN_IER_EPVIE;
	} else {
		can->IER &= ~CAN_IER_EPVIE;
	}
}

uint32_t can_IER_EPVIE_read(CAN_TypeDef* can) {
	return can->IER & CAN_IER_EPVIE;
}

//EWGIE: Error warning interrupt enable
void can_IER_EWGIE_set(CAN_TypeDef* can, bool state) {
	if(state) {
		can->IER |= CAN_IER_EWGIE;
	} else {
		can->IER &= ~CAN_IER_EWGIE;
	}
}

uint32_t can_IER_EWGIE_read(CAN_TypeDef* can) {
	return can->IER & CAN_IER_EWGIE;
}

//FOVIE: FIFO overrun interrupt enable
void can_IER_FOVIE_set(CAN_TypeDef* can, int fifo, bool state) {
	switch(fifo) {
	case CAN_RX_MAILBOX_0:
		if(state) {
			can->IER |= CAN_IER_FOVIE0;
		} else {
			can->IER &= ~CAN_IER_FOVIE0;
		}
		break;

	case CAN_RX_MAILBOX_1:
		if(state) {
			can->IER |= CAN_IER_FOVIE1;
		} else {
			can->IER &= ~CAN_IER_FOVIE1;
		}
		break;

	default: return;
	}
}

uint32_t can_IER_FOVIE_read(CAN_TypeDef* can, int fifo) {
	switch(fifo) {
	case CAN_RX_MAILBOX_0: return can->IER & CAN_IER_FOVIE0;
	case CAN_RX_MAILBOX_1: return can->IER & CAN_IER_FOVIE1;
	default: return 0;
	}
}

//FFIE: FIFO full interrupt enable
void can_IER_FFIE_set(CAN_TypeDef* can, int fifo, bool state) {
	switch(fifo) {
	case CAN_RX_MAILBOX_0:
		if(state) {
			can->IER |= CAN_IER_FFIE0;
		} else {
			can->IER &= ~CAN_IER_FFIE0;
		}
		break;

	case CAN_RX_MAILBOX_1:
		if(state) {
			can->IER |= CAN_IER_FFIE1;
		} else {
			can->IER &= ~CAN_IER_FFIE1;
		}
		break;

	default: return;
	}
}

uint32_t can_IER_FFIE_read(CAN_TypeDef* can, int fifo) {
	switch(fifo) {
	case CAN_RX_MAILBOX_0: return can->IER & CAN_IER_FFIE0;
	case CAN_RX_MAILBOX_1: return can->IER & CAN_IER_FFIE1;
	default: return 0;
	}
}

//FMPIE: FIFO message pending interrupt enable
void can_IER_FMPIE_set(CAN_TypeDef* can, int fifo, bool state) {
	switch(fifo) {

	case CAN_RX_MAILBOX_0:
		if(state)
		{
			can->IER |= CAN_IER_FMPIE0;
		} else {
			can->IER &= ~CAN_IER_FMPIE0;
		}
		break;

	case CAN_RX_MAILBOX_1:
		if(state) {
			can->IER |= CAN_IER_FMPIE1;
		} else {
			can->IER &= ~CAN_IER_FMPIE1;
		}
		break;

	default: return;
	}
}

uint32_t can_IER_FMPIE_read(CAN_TypeDef* can, int fifo) {
	switch(fifo) {
	case CAN_RX_MAILBOX_0: return can->IER & CAN_IER_FMPIE0;
	case CAN_RX_MAILBOX_1: return can->IER & CAN_IER_FMPIE1;
	default: return 0;
	}
}

//TMEIE: Transmit mailbox empty interrupt enable
void can_IER_TMEIE_set(CAN_TypeDef* can, bool state) {
	if(state) {
		can->IER |= CAN_IER_TMEIE;
	} else {
		can->IER &= ~CAN_IER_TMEIE;
	}
}

uint32_t can_IER_TMEIE_read(CAN_TypeDef* can) {
	return can->IER & CAN_IER_TMEIE;
}
