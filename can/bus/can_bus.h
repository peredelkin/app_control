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

//Address offset: 0x04
extern void can_MSR_SLAKI_clear(CAN_TypeDef* can);
extern uint32_t can_MSR_SLAKI_read(CAN_TypeDef* can);
extern void can_MSR_WKUI_clear(CAN_TypeDef* can);
extern uint32_t can_MSR_WKUI_read(CAN_TypeDef* can);
extern void can_MSR_ERRI_clear(CAN_TypeDef* can);
extern uint32_t can_MSR_ERRI_read(CAN_TypeDef* can);
extern uint32_t can_MSR_SLAK_read(CAN_TypeDef* can);
extern uint32_t can_MSR_INAK_read(CAN_TypeDef* can);

//Address offset: 0x08
extern uint32_t can_TSR_read(CAN_TypeDef* can);
extern int can_TSR_LOW_get(uint32_t TSR);
extern int can_TSR_TME_get(uint32_t TSR);
extern int can_TSR_CODE_get(uint32_t TSR);
extern void can_TSR_ABRQ_set(CAN_TypeDef* can, int mailbox);
extern bool can_TSR_TERR_get(uint32_t TSR, int mailbox);
extern void can_TSR_TERR_set(CAN_TypeDef* can, int mailbox);
extern bool can_TSR_ALST_get(uint32_t TSR, int mailbox);
extern void can_TSR_ALST_set(CAN_TypeDef* can, int mailbox);
extern bool can_TSR_TXOK_get(uint32_t TSR, int mailbox);
extern void can_TSR_TXOK_clear(CAN_TypeDef* can, int mailbox);
extern bool can_TSR_RQCP_get(uint32_t TSR, int mailbox);
extern void can_TSR_RQCP_clear(CAN_TypeDef* can, int mailbox);

//Address offset: 0x0C
extern uint32_t can_RFR_read(CAN_TypeDef* CAN, int fifo);
extern uint32_t can_RFR_FMP_read(uint32_t RFR);
extern uint32_t can_RFR_FULL_read(uint32_t RFR);
extern uint32_t can_RFR_FOVR_read(uint32_t RFR);
extern void can_RFR_fifo_release(CAN_TypeDef* CAN, int fifo);

//Address offset: 0x14
extern void can_IER_SLKIE_set(CAN_TypeDef* can, bool state);
extern void can_IER_WKUIE_set(CAN_TypeDef* can, bool state);
extern void can_IER_ERRIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_ERRIE_read(CAN_TypeDef* can);
extern void can_IER_LECIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_LECIE_read(CAN_TypeDef* can);
extern void can_IER_BOFIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_BOFIE_read(CAN_TypeDef* can);
extern void can_IER_EPVIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_EPVIE_read(CAN_TypeDef* can);
extern void can_IER_EWGIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_EWGIE_read(CAN_TypeDef* can);
extern void can_IER_FOVIE_set(CAN_TypeDef* can, int fifo, bool state);
extern uint32_t can_IER_FOVIE_read(CAN_TypeDef* can, int fifo);
extern void can_IER_FFIE_set(CAN_TypeDef* can, int fifo, bool state);
extern uint32_t can_IER_FFIE_read(CAN_TypeDef* can, int fifo);
extern void can_IER_FMPIE_set(CAN_TypeDef* can, int fifo, bool state);
extern uint32_t can_IER_FMPIE_read(CAN_TypeDef* can, int fifo);
extern void can_IER_TMEIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_TMEIE_read(CAN_TypeDef* can);

//Address offset: 0x1C
extern void can_BTR_set(CAN_TypeDef* can, uint32_t btr);

//Address offset: 0x200
extern void can_filter_init_mode(CAN_TypeDef* CAN);
extern void can_filter_active_mode(CAN_TypeDef* CAN);

extern err_t can_rx_mailbox_read_and_release(CAN_TypeDef* CAN, int fifo, uint8_t* fmi, can_bus_rx_t* rx_message);
extern err_t can_tx_mailbox_write_and_request(CAN_TypeDef* CAN, can_bus_tx_t* tx_message);

#endif /* CAN_BUS_CAN_BUS_H_ */
