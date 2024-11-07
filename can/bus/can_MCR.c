/*
 * can_MCR.c
 *
 *  Created on: 24 сент. 2024 г.
 *      Author: Ruslan
 */

#include "stm32f4xx/stm32f4xx.h"
#include "can_reg.h"

/**
 * Sets INRQ to 1 and waits for INAK 1
 * @param CAN
 */
void can_bus_initialization_request(CAN_TypeDef* CAN) {
	BIT_ON_MASK(CAN->MCR, CAN_MCR_INRQ);			//Initialization request
	while(!BIT_TEST_MASK(CAN->MSR, CAN_MSR_INAK));	//Waits for Initialization acknowledge
}

/**
 * Sets INRQ to 0 and waits for INAK 0
 * @param CAN
 */
void can_bus_initialization_exit(CAN_TypeDef* CAN) {
	BIT_OFF_MASK(CAN->MCR, CAN_MCR_INRQ);			//Normal mode request
	while(BIT_TEST_MASK(CAN->MSR, CAN_MSR_INAK));	//Waits for Normal mode
}

/**
 * Sets SLEEP to 1 and waits for SLAK 1
 * @param CAN
 */
void can_bus_sleep_request(CAN_TypeDef* CAN) {
	BIT_ON_MASK(CAN->MCR, CAN_MCR_SLEEP);			//Sleep mode request
	while(!BIT_TEST_MASK(CAN->MSR, CAN_MSR_SLAK));	//Waits for Sleep acknowledge
}

/**
 * Sets SLEEP to 0 and waits for SLAK 0
 * @param CAN
 */
void can_bus_sleep_exit(CAN_TypeDef* CAN) {
	BIT_OFF_MASK(CAN->MCR, CAN_MCR_SLEEP);			//Left Sleep mode request
	while(BIT_TEST_MASK(CAN->MSR, CAN_MSR_SLAK));	//Waits for Left Sleep mode
}
