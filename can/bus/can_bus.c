/*
 * can_bus.c
 *
 *  Created on: 29 окт. 2024 г.
 *      Author: Ruslan
 */

#include "can_filter.h"

err_t can_bus_filter_32b_bank_set(can_bus_t* bus, int filter, uint32_t id, uint32_t mask) {
	if (filter < 0) return E_INVALID_VALUE;
	if (filter > 27) return E_OUT_OF_RANGE;

	int fifo_n = (filter & 0x1);
	int fifo_index = (filter >> 1);

	can_filter_set_inactive(bus->can, filter);

	can_filter_set_mask_mode(bus->can, filter);

	can_filter_set_single_scale(bus->can, filter);

	switch (fifo_n) {
	case 0:
		can_filter_assigned_to_fifo_0(bus->can, filter);
		bus->fifo_0_filter[fifo_index] = filter;
		break;
	case 1:
		can_filter_assigned_to_fifo_1(bus->can, filter);
		bus->fifo_1_filter[fifo_index] = filter;
		break;
	default:
		return E_INVALID_VALUE;
	}

	bus->can->sFilterRegister[filter].FR1 = id;
	bus->can->sFilterRegister[filter].FR2 = mask;

	can_filter_set_active(bus->can, filter);

	return E_NO_ERROR;
}
