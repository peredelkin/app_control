/*
 * can_MCR.h
 *
 *  Created on: 24 мар. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_BUS_CAN_MCR_H_
#define CAN_BUS_CAN_MCR_H_

#include <stdbool.h>
#include "lib/stm32f4xx/stm32f4xx.h"

//Address offset: 0x00
extern void can_MCR_DBF_set(CAN_TypeDef* CAN, bool DBF);
extern void can_software_master_reset(CAN_TypeDef* CAN);
extern void can_MCR_TTCM_set(CAN_TypeDef* CAN, bool TTCM);
extern void can_MCR_ABOM_set(CAN_TypeDef* CAN, bool ABOM);
extern void can_MCR_AWUM_set(CAN_TypeDef* CAN, bool AWUM);
extern void can_MCR_NART_set(CAN_TypeDef* CAN, bool NART);
extern void can_MCR_RFLM_set(CAN_TypeDef* CAN, bool RFLM);
extern void can_MCR_TXFP_set(CAN_TypeDef* CAN, bool TXFP);

/**
 * Sets SLEEP to 1 and waits for SLAK 1
 * @param CAN
 */
extern void can_bus_sleep_request(CAN_TypeDef* CAN);

/**
 * Sets SLEEP to 0 and waits for SLAK 0
 * @param CAN
 */
extern void can_bus_sleep_exit(CAN_TypeDef* CAN);

/**
 * Sets INRQ to 1 and waits for INAK 1
 * @param CAN
 */
extern void can_bus_initialization_request(CAN_TypeDef* CAN);

/**
 * Sets INRQ to 0 and waits for INAK 0
 * @param CAN
 */
extern void can_bus_initialization_exit(CAN_TypeDef* CAN);

#endif /* CAN_BUS_CAN_MCR_H_ */
