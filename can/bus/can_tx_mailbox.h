/*
 * can_tx_mailbox.h
 *
 *  Created on: 24 мар. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_BUS_CAN_TX_MAILBOX_H_
#define CAN_BUS_CAN_TX_MAILBOX_H_

#include "lib/errors/errors.h"
#include "lib/stm32f4xx/stm32f4xx.h"

extern uint32_t can_TIR_read(CAN_TypeDef* CAN, int mailbox);
extern err_t can_tx_mailbox_write_and_request(CAN_TypeDef* CAN, uint32_t id, uint8_t dlc, uint8_t* data);

#endif /* CAN_BUS_CAN_TX_MAILBOX_H_ */
