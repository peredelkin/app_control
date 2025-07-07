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
	case 0:
		can_master_filter_assigned_to_fifo_0(can_master, filter_bank);
		bus->fifo_0_filter[bus->can_n][fifo_index] = filter_bank;
		break;
	case 1:
		can_master_filter_assigned_to_fifo_1(can_master, filter_bank);
		bus->fifo_1_filter[bus->can_n][fifo_index] = filter_bank;
		break;
	default:
		return E_INVALID_VALUE;
	}

	can_master->sFilterRegister[filter_bank].FR1 = id;
	can_master->sFilterRegister[filter_bank].FR2 = mask;

	can_master_filter_set_active(can_master, filter_bank);

	can_master_filter_active_mode(can_master);

	return E_NO_ERROR;
}

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
	case 0:
		can_master_filter_assigned_to_fifo_0(can_master, filter_bank);
		bus->fifo_0_filter[bus->can_n][fifo_index] = filter;
		break;
	case 1:
		can_master_filter_assigned_to_fifo_1(can_master, filter_bank);
		bus->fifo_1_filter[bus->can_n][fifo_index] = filter;
		break;
	default:
		return E_INVALID_VALUE;
	}

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
