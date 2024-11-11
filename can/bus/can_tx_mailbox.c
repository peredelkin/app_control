/*
 * can_tx_mailbox.c
 *
 *  Created on: 3 окт. 2024 г.
 *      Author: Ruslan
 */

#include <string.h>
#include "can_bus.h"

enum {
	CAN_TX_MAILBOX_0 = 0,
	CAN_TX_MAILBOX_1,
	CAN_TX_MAILBOX_2
};

void can_tx_request(CAN_TypeDef* CAN, int mailbox) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		CAN->sTxMailBox[CAN_TX_MAILBOX_0].TIR |= CAN_TIR_TXRQ;
		break;

	case CAN_TX_MAILBOX_1:
		CAN->sTxMailBox[CAN_TX_MAILBOX_1].TIR |= CAN_TIR_TXRQ;
		break;

	case CAN_TX_MAILBOX_2:
		CAN->sTxMailBox[CAN_TX_MAILBOX_2].TIR |= CAN_TIR_TXRQ;
		break;

	default:
		return;
	}
}

void can_TIR_write(CAN_TypeDef* CAN, int mailbox, uint32_t data) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		CAN->sTxMailBox[CAN_TX_MAILBOX_0].TIR = data;
		break;

	case CAN_TX_MAILBOX_1:
		CAN->sTxMailBox[CAN_TX_MAILBOX_1].TIR = data;
		break;

	case CAN_TX_MAILBOX_2:
		CAN->sTxMailBox[CAN_TX_MAILBOX_2].TIR = data;
		break;

	default:
		return;
	}
}

void can_TDTR_write(CAN_TypeDef* CAN, int mailbox, uint32_t data) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		CAN->sTxMailBox[CAN_TX_MAILBOX_0].TDTR = data;
		break;

	case CAN_TX_MAILBOX_1:
		CAN->sTxMailBox[CAN_TX_MAILBOX_1].TDTR = data;
		break;

	case CAN_TX_MAILBOX_2:
		CAN->sTxMailBox[CAN_TX_MAILBOX_2].TDTR = data;
		break;

	default:
		return;
	}
}

void can_TDLR_write(CAN_TypeDef* CAN, int mailbox, uint32_t data) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		CAN->sTxMailBox[CAN_TX_MAILBOX_0].TDLR = data;
		break;

	case CAN_TX_MAILBOX_1:
		CAN->sTxMailBox[CAN_TX_MAILBOX_1].TDLR = data;
		break;

	case CAN_TX_MAILBOX_2:
		CAN->sTxMailBox[CAN_TX_MAILBOX_2].TDLR = data;
		break;

	default:
		return;
	}
}

void can_TDHR_write(CAN_TypeDef* CAN, int mailbox, uint32_t data) {
	switch(mailbox) {
	case CAN_TX_MAILBOX_0:
		CAN->sTxMailBox[CAN_TX_MAILBOX_0].TDHR = data;
		break;

	case CAN_TX_MAILBOX_1:
		CAN->sTxMailBox[CAN_TX_MAILBOX_1].TDHR = data;
		break;

	case CAN_TX_MAILBOX_2:
		CAN->sTxMailBox[CAN_TX_MAILBOX_2].TDHR = data;
		break;

	default:
		return;
	}
}

err_t can_tx_mailbox_write_and_request(CAN_TypeDef* CAN, can_bus_tx_t* tx_message) {
	if(CAN == NULL) return E_NULL_POINTER;

	if(tx_message == NULL) return E_NULL_POINTER;

	uint32_t TSR = can_TSR_read(CAN);

	int tx_empty = can_TSR_TME_get(TSR);

	if(tx_empty < 0) return E_BUSY;

	if(tx_message->dlc > 8) return E_OUT_OF_RANGE;

	uint32_t TIR = 0;
	uint32_t TDTR = 0;
	uint32_t TDLHR[2] = {0,0};

	//DLC
	TDTR |= (CAN_TDTR_DLC & tx_message->dlc); //set DLC

	//RTR or DATA?
	if (tx_message->rtr) {
		TIR |= CAN_TIR_RTR; //set RTR
	} else {
		memcpy(TDLHR, tx_message->data, tx_message->dlc); //copy DATA
	}

	//EXTID or STDID?
	if(tx_message->ide) {
		TIR |= CAN_TIR_IDE; //set IDE
		TIR |= (CAN_TIR_EXID & (tx_message->id << CAN_TIR_EXID_SHIFT)); //set EXTID
	} else {
		TIR |= (CAN_TIR_STID & (tx_message->id << CAN_TIR_STID_SHIFT)); //set STID
	}

	can_TIR_write(CAN, tx_empty, TIR);			//TX mailbox identifier
	can_TDTR_write(CAN, tx_empty, TDTR);		//CAN mailbox data length control and time stamp
	can_TDLR_write(CAN, tx_empty, TDLHR[0]);	//CAN mailbox data low
	can_TDHR_write(CAN, tx_empty, TDLHR[1]);	//CAN mailbox data high

	can_tx_request(CAN, tx_empty);	//Transmit mailbox request

	return E_NO_ERROR;
}
