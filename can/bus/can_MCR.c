/*
 * can_MCR.c
 *
 *  Created on: 24 сент. 2024 г.
 *      Author: Ruslan
 */

#include "can_bus.h"

void can_MCR_DBF_set(CAN_TypeDef* CAN, bool DBF) {
	if(DBF) {
		CAN->MCR |= CAN_MCR_DBF;
	} else {
		CAN->MCR &= ~CAN_MCR_DBF;
	}
}

void can_software_master_reset(CAN_TypeDef* CAN) {
	//This bit is automatically reset to 0.
	CAN->MCR |= CAN_MCR_RESET;
	while(CAN->MCR & CAN_MCR_RESET);
}

void can_MCR_TTCM_set(CAN_TypeDef* CAN, bool TTCM) {
	if(TTCM) {
		CAN->MCR |= CAN_MCR_TTCM;
	} else {
		CAN->MCR &= ~CAN_MCR_TTCM;
	}
}

void can_MCR_ABOM_set(CAN_TypeDef* CAN, bool ABOM) {
	if(ABOM) {
		CAN->MCR |= CAN_MCR_ABOM;
	} else {
		CAN->MCR &= ~CAN_MCR_ABOM;
	}
}

void can_MCR_AWUM_set(CAN_TypeDef* CAN, bool AWUM) {
	if(AWUM) {
		CAN->MCR |= CAN_MCR_AWUM;
	} else {
		CAN->MCR &= ~CAN_MCR_AWUM;
	}
}

void can_MCR_NART_set(CAN_TypeDef* CAN, bool NART) {
	if(NART) {
		CAN->MCR |= CAN_MCR_NART;
	} else {
		CAN->MCR &= ~CAN_MCR_NART;
	}
}

void can_MCR_RFLM_set(CAN_TypeDef* CAN, bool RFLM) {
	if(RFLM) {
		CAN->MCR |= CAN_MCR_RFLM;
	} else {
		CAN->MCR &= ~CAN_MCR_RFLM;
	}
}

void can_MCR_TXFP_set(CAN_TypeDef* CAN, bool TXFP) {
	if(TXFP) {
		CAN->MCR |= CAN_MCR_TXFP;
	} else {
		CAN->MCR &= ~CAN_MCR_TXFP;
	}
}


/**
 * Sets SLEEP to 1 and waits for SLAK 1
 * @param CAN
 */
void can_bus_sleep_request(CAN_TypeDef* CAN) {
	BIT_ON_MASK(CAN->MCR, CAN_MCR_SLEEP);			//Sleep mode request
	while(can_MSR_SLAK_read(CAN) != CAN_MSR_SLAK);	//Waits for Sleep acknowledge
}

/**
 * Sets SLEEP to 0 and waits for SLAK 0
 * @param CAN
 */
void can_bus_sleep_exit(CAN_TypeDef* CAN) {
	BIT_OFF_MASK(CAN->MCR, CAN_MCR_SLEEP);			//Left Sleep mode request
	while (can_MSR_SLAK_read(CAN) == CAN_MSR_SLAK);
}

/**
 * Sets INRQ to 1 and waits for INAK 1
 * @param CAN
 */
void can_bus_initialization_request(CAN_TypeDef* CAN) {
	BIT_ON_MASK(CAN->MCR, CAN_MCR_INRQ);			//Initialization request
	while(can_MSR_INAK_read(CAN) != CAN_MSR_INAK);	//Waits for Initialization acknowledge
}

/**
 * Sets INRQ to 0 and waits for INAK 0
 * @param CAN
 */
void can_bus_initialization_exit(CAN_TypeDef* CAN) {
	BIT_OFF_MASK(CAN->MCR, CAN_MCR_INRQ);			//Normal mode request
	while(can_MSR_INAK_read(CAN) == CAN_MSR_INAK);	//Waits for Normal mode
}
