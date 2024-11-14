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
	case CAN_RX_FIFO_0:
		return CAN->RF0R;

	case CAN_RX_FIFO_1:
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

void can_RFR_FULL_clear(CAN_TypeDef* CAN, int fifo) {
	switch(fifo) {
	case CAN_RX_FIFO_0:
		CAN->RF0R = CAN_RFR_FULL;
		break;

	case CAN_RX_FIFO_1:
		CAN->RF1R = CAN_RFR_FULL;
		break;

	default:
		return;
	}
}

void can_RFR_FOVR_clear(CAN_TypeDef* CAN, int fifo) {
	switch(fifo) {
	case CAN_RX_FIFO_0:
		CAN->RF0R = CAN_RFR_FULL;
		break;

	case CAN_RX_FIFO_1:
		CAN->RF1R = CAN_RFR_FULL;
		break;

	default:
		return;
	}
}

void can_RFR_RFOM_set(CAN_TypeDef* CAN, int fifo) {
	switch(fifo) {
	case CAN_RX_FIFO_0:
		CAN->RF0R = CAN_RFR_RFOM;
		break;

	case CAN_RX_FIFO_1:
		CAN->RF1R = CAN_RFR_RFOM;
		break;

	default:
		return;
	}
}

err_t can_rx_mailbox_read_and_release(CAN_TypeDef* CAN, int fifo, uint32_t* id, uint8_t* dlc, uint8_t* index, uint8_t* data) {
	if(CAN == NULL) return E_NULL_POINTER;

	if(id == NULL) return E_NULL_POINTER;

	if(dlc == NULL) return E_NULL_POINTER;

	if(index == NULL) return E_NULL_POINTER;

	if(data == NULL) return E_NULL_POINTER;

	uint32_t RIR = 0;
	uint32_t RDTR = 0;
	uint32_t RDLHR[2] = {0,0};

	RIR = can_RIR_read(CAN, fifo);
	RDTR = can_RDTR_read(CAN, fifo);
	RDLHR[0] = can_RDLR_read(CAN, fifo);
	RDLHR[1] = can_RDHR_read(CAN, fifo);

	can_RFR_RFOM_set(CAN, fifo);

	//ID
	*id = ((CAN_TIR_STID | CAN_TIR_EXID | CAN_TIR_IDE | CAN_TIR_RTR) & RIR);

	//DLC
	*dlc = (uint8_t)(CAN_RDTR_DLC & RDTR);

	//INDEX
	*index = (uint8_t)((CAN_RDTR_FMI & RDTR) >> CAN_RIR_FMI_SHIFT);

	//DATA
	memcpy(data, RDLHR, *dlc); //copy DATA

	return E_NO_ERROR;
}
