/*
 * can_rx_mailbox.c
 *
 *  Created on: 3 окт. 2024 г.
 *      Author: Ruslan
 */

#include <string.h>
#include "can_bus.h"

enum {
	CAN_RX_FIFO_0 = 0,
	CAN_RX_FIFO_1
};

uint32_t can_RIR_read(CAN_TypeDef* CAN, int fifo) {
	switch(fifo) {
	case CAN_RX_FIFO_0:
		return CAN->sFIFOMailBox[CAN_RX_FIFO_0].RIR;

	case CAN_RX_FIFO_1:
		return CAN->sFIFOMailBox[CAN_RX_FIFO_1].RIR;

	default:
		return 0;
	}
}

uint32_t can_RDTR_read(CAN_TypeDef* CAN, int fifo) {
	switch(fifo) {
	case CAN_RX_FIFO_0:
		return CAN->sFIFOMailBox[CAN_RX_FIFO_0].RDTR;

	case CAN_RX_FIFO_1:
		return CAN->sFIFOMailBox[CAN_RX_FIFO_1].RDTR;

	default:
		return 0;
	}
}

uint32_t can_RDLR_read(CAN_TypeDef* CAN, int fifo) {
	switch(fifo) {
	case CAN_RX_FIFO_0:
		return CAN->sFIFOMailBox[CAN_RX_FIFO_0].RDLR;

	case CAN_RX_FIFO_1:
		return CAN->sFIFOMailBox[CAN_RX_FIFO_1].RDLR;

	default:
		return 0;
	}
}

uint32_t can_RDHR_read(CAN_TypeDef* CAN, int fifo) {
	switch(fifo) {
	case CAN_RX_FIFO_0:
		return CAN->sFIFOMailBox[CAN_RX_FIFO_0].RDHR;

	case CAN_RX_FIFO_1:
		return CAN->sFIFOMailBox[CAN_RX_FIFO_1].RDHR;

	default:
		return 0;
	}
}

uint32_t can_RFR_read(CAN_TypeDef* CAN, int fifo) {
	switch(fifo) {
	case 0:
		return CAN->RF0R;

	case 1:
		return CAN->RF1R;

	default:
		return 0;
	}
}

uint32_t can_RFR_FMP_read(uint32_t RFR) {
	return RFR & CAN_RFR_FMP;
}

uint32_t can_RFR_FULL_read(uint32_t RFR) {
	return RFR & CAN_RFR_FULL;
}

uint32_t can_RFR_FOVR_read(uint32_t RFR) {
	return RFR & CAN_RFR_FOVR;
}

void can_RFR_fifo_release(CAN_TypeDef* CAN, int fifo) {
	switch(fifo) {
	case 0:
		CAN->RF0R |= CAN_RFR_RFOM;
		break;

	case 1:
		CAN->RF1R |= CAN_RFR_RFOM;
		break;

	default:
		return;
	}
}

err_t can_rx_mailbox_read_and_release(CAN_TypeDef* CAN, int fifo, uint8_t* fmi, can_bus_rx_t* rx_message) {
	if(CAN == NULL) return E_NULL_POINTER;

	if(fmi == NULL) return E_NULL_POINTER;

	if(rx_message == NULL) return E_NULL_POINTER;

	uint32_t RIR = 0;
	uint32_t RDTR = 0;
	uint32_t RDLHR[2] = {0,0};

	RIR = can_RIR_read(CAN, fifo);
	RDTR = can_RDTR_read(CAN, fifo);
	RDLHR[0] = can_RDLR_read(CAN, fifo);
	RDLHR[1] = can_RDHR_read(CAN, fifo);

	can_RFR_fifo_release(CAN, fifo);

	//FMI
	*fmi = (uint8_t)((RDTR & CAN_RDTR_FMI) >> CAN_RIR_FMI_SHIFT);

	//DLC
	rx_message->dlc = (uint8_t)(RDTR & CAN_RDTR_DLC);

	//RTR or DATA?
	if (RIR & CAN_RIR_RTR) {
		rx_message->rtr = true;
	} else {
		rx_message->rtr = false;
		memcpy(rx_message->data, RDLHR, rx_message->dlc); //copy DATA
	}

	//EXTID or STDID?
	if(RIR & CAN_RIR_IDE) {
		rx_message->ide = true;
		rx_message->id = (uint32_t)((RIR & CAN_RIR_EXID) >> CAN_RIR_EXID_SHIFT);
	} else {
		rx_message->ide = false;
		rx_message->id = (uint32_t)((RIR & CAN_RIR_STID) >> CAN_RIR_STID_SHIFT);
	}

	return E_NO_ERROR;
}
