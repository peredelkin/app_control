/*
 * can_bus.h
 *
 *  Created on: 3 окт. 2024 г.
 *      Author: Ruslan
 */

#ifndef CAN_BUS_CAN_BUS_H_
#define CAN_BUS_CAN_BUS_H_

#include <stdbool.h>

#include "lib/errors/errors.h"

#include "can_BTR.h"
#include "can_ESR.h"
#include "can_IER.h"
#include "can_MCR.h"
#include "can_MSR.h"
#include "can_TSR.h"
#include "can_rx_mailbox.h"
#include "can_tx_mailbox.h"
#include "can_master_filter.h"

#include "can_reg.h"


enum {
	CAN_RX_MAILBOX_0 = 0,
	CAN_RX_MAILBOX_1
};

#define CAN_CELL_COUNT			2
#define CAN_FILTER_MAX_COUNT	56	//16b mask + 16b id

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

typedef enum {
	CAN_BUS_MASTER = 0,
	CAN_BUS_SLAVE,
	CAN_BUS_COUNT
} can_n_t;

typedef struct {
	CAN_TypeDef *can_ptr[CAN_BUS_COUNT];
	can_n_t can_n;
	uint8_t fifo_0_filter[CAN_BUS_COUNT][CAN_FILTER_MAX_COUNT];
	uint8_t fifo_1_filter[CAN_BUS_COUNT][CAN_FILTER_MAX_COUNT];
	uint32_t error;
	uint32_t tx_error_counter;
	uint32_t rx_error_counter;
	uint32_t last_error_code;
} can_bus_t;

extern err_t can_bus_filter_32b_bank_set(can_bus_t* bus, int filter_bank, uint32_t id, uint32_t mask);
extern err_t can_bus_filter_16b_bank_set(can_bus_t* bus, int filter, uint32_t id, uint32_t mask);

#endif /* CAN_BUS_CAN_BUS_H_ */
