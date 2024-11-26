/*
 * can_bus.h
 *
 *  Created on: 3 окт. 2024 г.
 *      Author: Ruslan
 */

#ifndef CAN_BUS_CAN_BUS_H_
#define CAN_BUS_CAN_BUS_H_

#include <stdbool.h>
#include "lib/stm32f4xx/stm32f4xx.h"
#include "lib/errors/errors.h"
#include "lib/bits/bits.h"
#include "can_reg.h"


enum {
	CAN_RX_MAILBOX_0 = 0,
	CAN_RX_MAILBOX_1
};


#define CAN_ERROR_RX_WARNING	((uint32_t)BIT(0))
#define CAN_ERROR_TX_WARNING	((uint32_t)BIT(1))

#define CAN_ERROR_RX_PASSIVE	((uint32_t)BIT(2))
#define CAN_ERROR_TX_PASSIVE	((uint32_t)BIT(3))

#define CAN_ERROR_TX_BUSSOFF	((uint32_t)BIT(4))

#define CAN_ERROR_RX0_OVERRUN	((uint32_t)BIT(5))
#define CAN_ERROR_RX0_FULL		((uint32_t)BIT(6))

#define CAN_ERROR_RX1_OVERRUN	((uint32_t)BIT(7))
#define CAN_ERROR_RX1_FULL		((uint32_t)BIT(8))

enum {
	CAN_ESR_LEC_No_Error = 0,
	CAN_ESR_LEC_Stuff_Error,
	CAN_ESR_LEC_Form_Error,
	CAN_ESR_LEC_Acknowledgment_Error,
	CAN_ESR_LEC_Bit_recessive_Error,
	CAN_ESR_LEC_Bit_dominant_Error,
	CAN_ESR_LEC_CRC_Error,
	CAN_ESR_LEC_Set_by_software
};


typedef struct {
	CAN_TypeDef *bus;
	uint32_t error;
	uint32_t tx_error_counter;
	uint32_t rx_error_counter;
	uint32_t last_error_code;
} can_bus_t;

//Address offset: 0x00
extern void can_MCR_DBF_set(CAN_TypeDef* CAN, bool DBF);
extern void can_software_master_reset(CAN_TypeDef* CAN);
extern void can_MCR_TTCM_set(CAN_TypeDef* CAN, bool TTCM);
extern void can_MCR_ABOM_set(CAN_TypeDef* CAN, bool ABOM);
extern void can_MCR_AWUM_set(CAN_TypeDef* CAN, bool AWUM);
extern void can_MCR_NART_set(CAN_TypeDef* CAN, bool NART);
extern void can_MCR_RFLM_set(CAN_TypeDef* CAN, bool RFLM);
extern void can_MCR_TXFP_set(CAN_TypeDef* CAN, bool TXFP);
extern void can_bus_initialization_request(CAN_TypeDef* CAN);
extern void can_bus_initialization_exit(CAN_TypeDef* CAN);
extern void can_bus_sleep_request(CAN_TypeDef* CAN);
extern void can_bus_sleep_exit(CAN_TypeDef* CAN);

//Address offset: 0x04
extern uint32_t can_MSR_read(CAN_TypeDef* can);
extern void can_MSR_SLAKI_clear(CAN_TypeDef* can);
extern uint32_t can_MSR_SLAKI_read(uint32_t MSR);
extern void can_MSR_WKUI_clear(CAN_TypeDef* can);
extern uint32_t can_MSR_WKUI_read(uint32_t MSR);
extern void can_MSR_ERRI_clear(CAN_TypeDef* can);
extern uint32_t can_MSR_ERRI_read(uint32_t MSR);
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
extern void can_RFR_FULL_clear(CAN_TypeDef* CAN, int fifo);
extern void can_RFR_FOVR_clear(CAN_TypeDef* CAN, int fifo);
extern void can_RFR_RFOM_set(CAN_TypeDef* CAN, int fifo);

//Address offset: 0x14
extern void can_IER_SLKIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_SLKIE_read(CAN_TypeDef* can);
extern void can_IER_WKUIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_WKUIE_read(CAN_TypeDef* can);
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

//Address offset: 0x18
extern uint32_t can_ESR_read(CAN_TypeDef *can);
extern uint32_t can_ESR_REC_read(uint32_t ESR);
extern uint32_t can_ESR_TEC_read(uint32_t ESR);
extern uint32_t can_ESR_LEC_read(uint32_t ESR);
extern uint32_t can_ESR_BOFF_read(uint32_t ESR);
extern uint32_t can_ESR_EPVF_read(uint32_t ESR);
extern uint32_t can_ESR_EWGF_read(uint32_t ESR);

//Address offset: 0x1C
extern void can_BTR_set(CAN_TypeDef* can, uint32_t btr);

//Address offset: 0x200
extern void can_filter_init_mode(CAN_TypeDef* CAN);
extern void can_filter_active_mode(CAN_TypeDef* CAN);
err_t can2_filter_start_bank_set(CAN_TypeDef* CAN, int CAN2SB);
extern err_t can_filter_bank_set(CAN_TypeDef* CAN, int filter, uint32_t id, uint32_t mask);

extern err_t can_rx_mailbox_read_and_release(CAN_TypeDef* CAN, int fifo, uint32_t* id, uint8_t* dlc, uint8_t* index, uint8_t* data);
extern err_t can_tx_mailbox_write_and_request(CAN_TypeDef* CAN, uint32_t id, uint8_t dlc, uint8_t* data);

#endif /* CAN_BUS_CAN_BUS_H_ */
