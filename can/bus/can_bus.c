/*
 * can_bus.c
 *
 *  Created on: 29 окт. 2024 г.
 *      Author: Ruslan
 */

#include "can_bus.h"
#include "can_master_filter.h"

err_t can_bus_filter_32b_bank_set(can_bus_t* bus, int filter_bank, uint32_t id, uint32_t mask) {
	if (filter_bank < 0) return E_INVALID_VALUE;
	if (filter_bank > 27) return E_OUT_OF_RANGE;

	CAN_TypeDef* can_master = bus->can_ptr[0];

	int fifo_n = (filter_bank & 0x1);
	int fifo_index = (filter_bank >> 1);

	can_master_filter_init_mode(can_master);

	can_master_filter_set_inactive(can_master, filter_bank);

	can_master_filter_set_mask_mode(can_master, filter_bank);

	can_master_filter_set_single_scale(can_master, filter_bank);

	switch (fifo_n) {
	case CAN_RX_MAILBOX_0:
		can_master_filter_assigned_to_fifo_0(can_master, filter_bank);
		break;
	case CAN_RX_MAILBOX_1:
		can_master_filter_assigned_to_fifo_1(can_master, filter_bank);
		break;
	default:
		return E_INVALID_VALUE;
	}

	bus->index_array[fifo_n][fifo_index] = filter_bank;

	can_master->sFilterRegister[filter_bank].FR1 = id;
	can_master->sFilterRegister[filter_bank].FR2 = mask;

	can_master_filter_set_active(can_master, filter_bank);

	can_master_filter_active_mode(can_master);

	return E_NO_ERROR;
}

err_t can_bus_filter_16b_bank_set(can_bus_t* bus, int filter, uint32_t id, uint32_t mask) {
	if (filter < 0) return E_INVALID_VALUE;
	if (filter > 55) return E_OUT_OF_RANGE;

	CAN_TypeDef* can_master = bus->can_ptr[0];

	int filter_bank = (filter >> 1);

	if(bus->can_n == CAN_BUS_SLAVE) {
		filter_bank += can_master_can2_filter_start_bank_get(can_master);
	}

	if (filter_bank > 27) return E_OUT_OF_RANGE;

	int filter_bank_index = (filter >> 1);
	int filter_bank_subindex = (filter & 0b1);

	int fifo_n = (filter_bank_index & 0x1);
	int fifo_index = (filter_bank_index + filter_bank_subindex - fifo_n);

	bool filter_was_active = false;
	bool filter_was_single = false;

	can_filter_32b_t prev_32b_id;
	can_filter_32b_t prev_32b_mask;

	can_filter_32b_t next_32b_id;
	can_filter_32b_t next_32b_mask;

	can_filter_16b_t new_16b[2];

	can_master_filter_is_active(can_master, filter_bank, &filter_was_active);

	can_master_filter_is_single_scale(can_master, filter_bank, &filter_was_single);

	can_master_filter_init_mode(can_master);

	can_master_filter_set_inactive(can_master, filter_bank);

	can_master_filter_set_mask_mode(can_master, filter_bank);

	switch (fifo_n) {
	case CAN_RX_MAILBOX_0:
		can_master_filter_assigned_to_fifo_0(can_master, filter_bank);
		break;
	case CAN_RX_MAILBOX_1:
		can_master_filter_assigned_to_fifo_1(can_master, filter_bank);
		break;
	default:
		return E_INVALID_VALUE;
	}

	bus->index_array[fifo_n][fifo_index] = filter;

	if (filter_bank_subindex) {
		//фильтры должны быть настроены последовательно!
		if(filter_was_active == false || filter_was_single == false) return E_INVALID_OPERATION;

		prev_32b_id.all = can_master->sFilterRegister[filter_bank].FR1;
		prev_32b_mask.all = can_master->sFilterRegister[filter_bank].FR2;

		next_32b_id.all = id;
		next_32b_mask.all = mask;

		//id 0
		new_16b[0].bit.id_exid_15_17 = prev_32b_id.bit.exid_15_17;
		new_16b[0].bit.id_ide = prev_32b_id.bit.ide;
		new_16b[0].bit.id_rtr = prev_32b_id.bit.rtr;
		new_16b[0].bit.id_stid_0_10 = prev_32b_id.bit.stid_0_10;
		//mask 0
		new_16b[0].bit.mask_exid_15_17 = prev_32b_mask.bit.exid_15_17;
		new_16b[0].bit.mask_ide = prev_32b_mask.bit.ide;
		new_16b[0].bit.mask_rtr = prev_32b_mask.bit.rtr;
		new_16b[0].bit.mask_stid_0_10 = prev_32b_mask.bit.stid_0_10;

		//id 1
		new_16b[1].bit.id_exid_15_17 = next_32b_id.bit.exid_15_17;
		new_16b[1].bit.id_ide = next_32b_id.bit.ide;
		new_16b[1].bit.id_rtr = next_32b_id.bit.rtr;
		new_16b[1].bit.id_stid_0_10 = next_32b_id.bit.stid_0_10;
		//mask 1
		new_16b[1].bit.mask_exid_15_17 = next_32b_mask.bit.exid_15_17;
		new_16b[1].bit.mask_ide = next_32b_mask.bit.ide;
		new_16b[1].bit.mask_rtr = next_32b_mask.bit.rtr;
		new_16b[1].bit.mask_stid_0_10 = next_32b_mask.bit.stid_0_10;

		can_master_filter_set_dual_scale(can_master, filter_bank);

		can_master->sFilterRegister[filter_bank].FR1 = new_16b[0].all;
		can_master->sFilterRegister[filter_bank].FR2 = new_16b[1].all;
	} else {
		can_master_filter_set_single_scale(can_master, filter_bank);

		can_master->sFilterRegister[filter_bank].FR1 = id;
		can_master->sFilterRegister[filter_bank].FR2 = mask;
	}

	can_master_filter_set_active(can_master, filter_bank);

	can_master_filter_active_mode(can_master);

	return E_NO_ERROR;
}

//RX
bool can_bus_rx_queue_empty(can_bus_t *bus) {
	return (bus->queue_rx.head == bus->queue_rx.tail);
}

bool can_bus_rx_queue_notEmpty(can_bus_t *bus) {
	return (bus->queue_rx.head != bus->queue_rx.tail);
}

can_rx_frame_queue_t* can_bus_rx_queue_head(can_bus_t *bus) {
	return &(bus->queue_rx.queue[bus->queue_rx.head]);
}

bool can_bus_rx_queue_dequeue(can_bus_t *bus) {
	if (bus->queue_rx.head == bus->queue_rx.tail) return false; //empty
	size_t new_head = bus->queue_rx.head + 1;
	if (new_head >= bus->queue_rx.size) new_head = 0;
	bus->queue_rx.head = new_head;
	return true;
}

can_rx_frame_queue_t* can_bus_rx_queue_tail(can_bus_t *bus) {
	return &(bus->queue_rx.queue[bus->queue_rx.tail]);
}

bool can_bus_rx_queue_can_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_rx.tail + 1;
	if (new_tail >= bus->queue_rx.size) new_tail = 0;
	if (new_tail == bus->queue_rx.head) return false; //ovf
	return true;
}

bool can_bus_rx_queue_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_rx.tail + 1;
	if (new_tail >= bus->queue_rx.size) new_tail = 0;
	if (new_tail == bus->queue_rx.head) return false; //ovf
	bus->queue_rx.tail = new_tail;
	return true;
}

bool can_bus_rx_process(can_bus_t* bus) {
	if (can_bus_rx_queue_empty(&bus->queue_rx)) return false;

	can_rx_frame_queue_t* head = can_bus_rx_queue_head(&bus->queue_rx);

	return false;
}

//TX
bool can_bus_tx_queue_empty(can_bus_t *bus) {
	return (bus->queue_tx.head == bus->queue_tx.tail);
}

bool can_bus_tx_queue_notEmpty(can_bus_t *bus) {
	return (bus->queue_tx.head != bus->queue_tx.tail);
}

can_tx_frame_queue_t* can_bus_tx_queue_head(can_bus_t *bus) {
	return &(bus->queue_tx.queue[bus->queue_tx.head]);
}

bool can_bus_tx_queue_dequeue(can_bus_t *bus) {
	if (bus->queue_tx.head == bus->queue_tx.tail) return false; //empty
	size_t new_head = bus->queue_tx.head + 1;
	if (new_head >= bus->queue_tx.size) new_head = 0;
	bus->queue_tx.head = new_head;
	return true;
}

can_tx_frame_queue_t* can_bus_tx_queue_tail(can_bus_t *bus) {
	return &(bus->queue_tx.queue[bus->queue_tx.tail]);
}

bool can_bus_tx_queue_can_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_tx.tail + 1;
	if (new_tail >= bus->queue_tx.size) new_tail = 0;
	if (new_tail == bus->queue_tx.head) return false; //ovf
	return true;
}

bool can_bus_tx_queue_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_tx.tail + 1;
	if (new_tail >= bus->queue_tx.size) new_tail = 0;
	if (new_tail == bus->queue_tx.head) return false; //ovf
	bus->queue_tx.tail = new_tail;
	return true;
}

bool can_bus_tx_process(can_bus_t* bus) {
	if (can_bus_tx_queue_empty(bus)) return false;

	can_tx_frame_queue_t* head = NULL;
	err_t tx_err = E_NO_ERROR;

	do {
		head = can_bus_tx_queue_head(&bus->queue_tx);
		tx_err = can_tx_mailbox_write_and_request(bus->can_ptr[bus->can_n], head->id, head->dlc, head->data);
		if(tx_err == E_NO_ERROR) {
			can_bus_tx_queue_dequeue(&bus->queue_tx);
		}
	} while ((tx_err == E_NO_ERROR) && (can_bus_tx_queue_notEmpty(&bus->queue_tx) == true));

	if((tx_err == E_NO_ERROR) || (tx_err == E_BUSY)) return true;

	return false;
}














