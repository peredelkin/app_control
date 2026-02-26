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

#define CAN_BUS_MAKE_ID(ID) ((uint32_t) (CAN_FIR_STID & (ID << CAN_FIR_STID_SHIFT)))
#define CAN_BUS_MAKE_MASK(MASK) ((uint32_t) (CAN_FIR_STID & (MASK << CAN_FIR_STID_SHIFT)))

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

enum {
	CAN_RX_MAILBOX_0 = 0,
	CAN_RX_MAILBOX_1,
	CAN_RX_MAILBOX_COUNT
};

typedef enum {
	CAN_BUS_MASTER = 0,
	CAN_BUS_SLAVE,
	CAN_BUS_COUNT
} can_n_t;

typedef struct {
	uint32_t id;
	uint8_t dlc;
	uint8_t index;
	uint8_t data[8];
} can_rx_frame_queue_t;

typedef struct {
	uint32_t id;
	uint8_t dlc;
	uint8_t data[8];
} can_tx_frame_queue_t;

typedef struct {
	can_rx_frame_queue_t* queue;
	size_t size;
	size_t head;
	size_t tail;
} can_rx_queue_t;

typedef struct {
	can_tx_frame_queue_t* queue;
	size_t size;
	size_t head;
	size_t tail;
} can_tx_queue_t;

typedef struct {
	uint32_t id;
	uint32_t mask;
} can_filter_t;

typedef struct _can_bus_t can_bus_t;

struct _can_bus_t {
	CAN_TypeDef *can_ptr[CAN_BUS_COUNT];
	can_n_t can_n;
	can_rx_queue_t queue_rx;
	can_tx_queue_t queue_tx;
	uint8_t index_array[CAN_RX_MAILBOX_COUNT][CAN_FILTER_MAX_COUNT];
	uint32_t error;
	uint32_t tx_error_counter;
	uint32_t rx_error_counter;
	uint32_t last_error_code;
	int last_index;
	can_bus_t* bridge_bus;
	int bridge_index;
	void* co;
	err_t (*rx_callback)(can_bus_t* bus);
};

typedef union {
	uint32_t all;
	struct {
		unsigned res_0		:1;
		unsigned rtr		:1;
		unsigned ide		:1;
		unsigned exid_0_14	:15;
		unsigned exid_15_17	:3;
		unsigned stid_0_10	:11;
	} bit;
} can_filter_32b_t;

typedef union {
	uint32_t all;
	struct {
		unsigned id_exid_15_17	:3;
		unsigned id_ide			:1;
		unsigned id_rtr			:1;
		unsigned id_stid_0_10	:11;
		unsigned mask_exid_15_17:3;
		unsigned mask_ide		:1;
		unsigned mask_rtr		:1;
		unsigned mask_stid_0_10	:11;
	} bit;
} can_filter_16b_t;

extern err_t can_bus_filter_16b_bank_set(can_bus_t* bus, int filter, uint32_t id, uint32_t mask);
err_t can_bus_filter_16b_bank_alloc(can_bus_t* bus, int count);
err_t can_bus_filter_set(can_bus_t* bus, int filter, uint32_t id, uint32_t mask);

extern err_t can_bus_bitrate_set(can_bus_t* can_bus, uint16_t bitrate);

extern can_rx_frame_queue_t* can_bus_rx_queue_head(can_bus_t *bus);

extern bool can_bus_rx_process(can_bus_t* bus);
extern void can_bus_rx_queue_init(can_bus_t *bus, can_rx_frame_queue_t* queue, size_t queue_size);

extern bool can_bus_tx_process(can_bus_t* bus);
extern void can_bus_tx_queue_init(can_bus_t *bus, can_tx_frame_queue_t* queue, size_t queue_size);
extern bool can_bus_write(can_bus_t* bus, uint32_t id, uint8_t dlc, uint8_t* data);

extern void CAN_TX_IRQHandler(can_bus_t *can_bus);
extern void CAN_RX_IRQHandler(can_bus_t *can_bus, int fifo);
extern void CAN_SCE_IRQHandler(can_bus_t *can_bus);

#endif /* CAN_BUS_CAN_BUS_H_ */
