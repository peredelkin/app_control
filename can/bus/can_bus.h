/*
 * can_bus.h
 *
 *  Created on: 3 окт. 2024 г.
 *      Author: Ruslan
 */

#ifndef CAN_BUS_CAN_BUS_H_
#define CAN_BUS_CAN_BUS_H_

#include <stdbool.h>
#include "stm32f4xx/stm32f4xx.h"
#include "errors/errors.h"
#include "can_reg.h"


typedef struct {
	uint32_t id;
	bool ide;
	bool rtr;
	uint8_t dlc;
	uint8_t data[8];
} can_bus_rx_t;

typedef struct {
	uint32_t id;
	bool ide;
	bool rtr;
	uint8_t dlc;
	uint8_t data[8];
} can_bus_tx_t;

typedef struct {
	CAN_TypeDef *bus;
	uint8_t rx_index[2];
	can_bus_rx_t rx[2];
	can_bus_tx_t tx;
	uint8_t filter_mode[28];
} can_bus_t;

//Address offset: 0x00
extern void can_bus_initialization_request(CAN_TypeDef* CAN);
extern void can_bus_initialization_exit(CAN_TypeDef* CAN);
extern void can_bus_sleep_request(CAN_TypeDef* CAN);
extern void can_bus_sleep_exit(CAN_TypeDef* CAN);

//Address offset: 0x0C
extern uint32_t can_RFR_read(CAN_TypeDef* CAN, int fifo);
extern uint32_t can_RFR_FMP_read(uint32_t RFR);
extern uint32_t can_RFR_FULL_read(uint32_t RFR);
extern uint32_t can_RFR_FOVR_read(uint32_t RFR);
extern void can_RFR_fifo_release(CAN_TypeDef* CAN, int fifo);

//Address offset: 0x14
extern uint32_t can_IER_FOVIE_read(CAN_TypeDef* can, int fifo);
extern uint32_t can_IER_FFIE_read(CAN_TypeDef* can, int fifo);
extern uint32_t can_IER_FMPIE_read(CAN_TypeDef* can, int fifo);

//Address offset: 0x1C
extern void can_BTR_set(CAN_TypeDef* can, uint32_t btr);

extern err_t can_rx_mailbox_read_and_release(CAN_TypeDef* CAN, int fifo, uint8_t* fmi, can_bus_rx_t* rx_message);
extern err_t can_tx_mailbox_write_and_request(CAN_TypeDef* CAN, can_bus_tx_t* tx_message);

#endif /* CAN_BUS_CAN_BUS_H_ */
