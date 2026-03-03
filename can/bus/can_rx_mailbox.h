/*
 * can_rx_mailbox.h
 *
 *  Created on: 24 мар. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_BUS_CAN_RX_MAILBOX_H_
#define CAN_BUS_CAN_RX_MAILBOX_H_

#include "lib/errors/errors.h"
#include "lib/stm32f4xx/stm32f4xx.h"

extern uint32_t can_RFR_read(CAN_TypeDef* CAN, int fifo);
extern uint32_t can_RFR_FMP_read(uint32_t RFR);
extern uint32_t can_RFR_FULL_read(uint32_t RFR);
extern uint32_t can_RFR_FOVR_read(uint32_t RFR);
extern void can_RFR_FULL_clear(CAN_TypeDef* CAN, int fifo);
extern void can_RFR_FOVR_clear(CAN_TypeDef* CAN, int fifo);
extern err_t can_rx_mailbox_read_and_release(CAN_TypeDef* CAN, int fifo, uint32_t* id, uint8_t* dlc, uint8_t* index, uint8_t* data);

#endif /* CAN_BUS_CAN_RX_MAILBOX_H_ */
