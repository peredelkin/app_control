/*
 * can_filter.c
 *
 *  Created on: 2 нояб. 2024 г.
 *      Author: Ruslan
 */

#include "can_master_filter.h"
#include "can_reg.h"
#include "can_bus.h"

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

bool can_master_filter_is_init(CAN_TypeDef *CAN) {
	if(CAN->FMR & CAN_FMR_FINIT) return 1;
	return 0;
}

void can_master_filter_init_mode(CAN_TypeDef *CAN) {
	CAN->FMR |= CAN_FMR_FINIT;
}

void can_master_filter_active_mode(CAN_TypeDef *CAN) {
	CAN->FMR &= ~CAN_FMR_FINIT;
}

err_t can_master_can2_filter_start_bank_set(CAN_TypeDef *CAN, int CAN2SB) {
	if (CAN2SB < 0) return E_INVALID_VALUE;
	if (CAN2SB > 27) return E_OUT_OF_RANGE;

	CAN->FMR &= ~CAN_FMR_CAN2SB;
	CAN->FMR |= (CAN_FMR_CAN2SB & (CAN2SB << CAN_FMR_CAN2SB_SHIFT));

	return E_NO_ERROR;
}

int can_master_can2_filter_start_bank_get(CAN_TypeDef *CAN) {
	return ((CAN->FMR & CAN_FMR_CAN2SB) >> CAN_FMR_CAN2SB_SHIFT);
}

err_t can_master_filter_set_mask_mode(CAN_TypeDef *CAN, int FBM) {
	if (FBM < 0) return E_INVALID_VALUE;
	if (FBM > 27) return E_OUT_OF_RANGE;

	CAN->FM1R &= ~(CAN_FM1R_FBMx & (1 << FBM));

	return E_NO_ERROR;
}

err_t can_master_filter_set_list_mode(CAN_TypeDef *CAN, int FBM) {
	if (FBM < 0) return E_INVALID_VALUE;
	if (FBM > 27) return E_OUT_OF_RANGE;

	CAN->FM1R |= (CAN_FM1R_FBMx & (1 << FBM));

	return E_NO_ERROR;
}

err_t can_master_filter_is_list_mode(CAN_TypeDef *CAN, int FBM, bool *list_mode) {
	if (FBM < 0) return E_INVALID_VALUE;
	if (FBM > 27) return E_OUT_OF_RANGE;

	if (CAN->FM1R & (CAN_FM1R_FBMx & (1 << FBM))) {
		*list_mode = true;
	} else {
		*list_mode = false;
	}

	return E_NO_ERROR;
}

err_t can_master_filter_set_dual_scale(CAN_TypeDef *CAN, int FSC) {
	if (FSC < 0) return E_INVALID_VALUE;
	if (FSC > 27) return E_OUT_OF_RANGE;

	CAN->FS1R &= ~(CAN_FS1R_FSCx & (1 << FSC));

	return E_NO_ERROR;
}

err_t can_master_filter_set_single_scale(CAN_TypeDef *CAN, int FSC) {
	if (FSC < 0) return E_INVALID_VALUE;
	if (FSC > 27) return E_OUT_OF_RANGE;

	CAN->FS1R |= (CAN_FS1R_FSCx & (1 << FSC));

	return E_NO_ERROR;
}

err_t can_master_filter_is_single_scale(CAN_TypeDef *CAN, int FSC, bool *single_scale) {
	if (FSC < 0) return E_INVALID_VALUE;
	if (FSC > 27) return E_OUT_OF_RANGE;

	if (CAN->FS1R & (CAN_FS1R_FSCx & (1 << FSC))) {
		*single_scale = true;
	} else {
		*single_scale = false;
	}

	return E_NO_ERROR;
}

err_t can_master_filter_assigned_to_fifo_0(CAN_TypeDef *CAN, int FFA) {
	if (FFA < 0) return E_INVALID_VALUE;
	if (FFA > 27) return E_OUT_OF_RANGE;

	CAN->FFA1R &= ~(CAN_FFA1R_FFAx & (1 << FFA));

	return E_NO_ERROR;
}

err_t can_master_filter_assigned_to_fifo_1(CAN_TypeDef *CAN, int FFA) {
	if (FFA < 0) return E_INVALID_VALUE;
	if (FFA > 27) return E_OUT_OF_RANGE;

	CAN->FFA1R |= (CAN_FFA1R_FFAx & (1 << FFA));

	return E_NO_ERROR;
}

err_t can_master_filter_is_active(CAN_TypeDef *CAN, int FACT, bool *is_active) {
	if (FACT < 0) return E_INVALID_VALUE;
	if (FACT > 27) return E_OUT_OF_RANGE;

	if (CAN->FA1R & (CAN_FA1R_FACTx & (1 << FACT))) {
		*is_active = true;
	} else {
		*is_active = false;
	}

	return E_NO_ERROR;
}

err_t can_master_filter_set_inactive(CAN_TypeDef *CAN, int FACT) {
	if (FACT < 0) return E_INVALID_VALUE;
	if (FACT > 27) return E_OUT_OF_RANGE;

	CAN->FA1R &= ~(CAN_FA1R_FACTx & (1 << FACT));

	return E_NO_ERROR;
}

err_t can_master_filter_set_active(CAN_TypeDef *CAN, int FACT) {
	if (FACT < 0) return E_INVALID_VALUE;
	if (FACT > 27) return E_OUT_OF_RANGE;

	CAN->FA1R |= (CAN_FA1R_FACTx & (1 << FACT));

	return E_NO_ERROR;
}

static void can_master_filter_reset(CAN_TypeDef *CAN, int filter_bank) {
	if (filter_bank < 0) return;
	if (filter_bank > 27) return;

	CAN->sFilterRegister[filter_bank].FR1 = 0;
	CAN->sFilterRegister[filter_bank].FR2 = 0;
}

void can_master_filter_reset_all(CAN_TypeDef *CAN) {
	for(int filter_bank = 0; filter_bank < 28; filter_bank++) {
		can_master_filter_reset(CAN, filter_bank);
	}
}

err_t can_bus_filter_16b_bank_set(can_bus_t* bus, int filter, uint32_t id, uint32_t mask) {
	if (filter < 0) return E_INVALID_VALUE;
	if (filter > (CAN_FILTER_MAX_COUNT - 1)) return E_OUT_OF_RANGE;

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

	//находятся ли фильтры в режиме инициализации
	bool init_mode = can_master_filter_is_init(can_master);

	//если фильтры активны
	if(init_mode == false) {
		//переведем в режим инициализации
		can_master_filter_init_mode(can_master);
	}

	can_master_filter_is_active(can_master, filter_bank, &filter_was_active);

	can_master_filter_is_single_scale(can_master, filter_bank, &filter_was_single);

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

	//если фильтры были активны
	if(init_mode == false) {
		//вернем в активный режим
		can_master_filter_active_mode(can_master);
	}

	/* Last CANopen Index for CAN bridge */
	bus->last_index = filter;

	return E_NO_ERROR;
}

err_t can_bus_filter_set(can_bus_t* bus, int filter, uint32_t id, uint32_t mask) {
	if (filter < 0) return E_INVALID_VALUE;
	if (filter > (CAN_FILTER_MAX_COUNT - 1)) return E_OUT_OF_RANGE;

	CAN_TypeDef* can_master = bus->can_ptr[0];

	int filter_bank = (filter >> 1);

	if(bus->can_n == CAN_BUS_SLAVE) {
		filter_bank += can_master_can2_filter_start_bank_get(can_master);
	}

	if (filter_bank > 27) return E_OUT_OF_RANGE;

	int filter_bank_subindex = (filter & 0b1);

	bool filter_was_active = false;

	can_master_filter_is_active(can_master, filter_bank, &filter_was_active);

	if(filter_was_active == false) return E_INVALID_OPERATION;

	can_master_filter_set_inactive(can_master, filter_bank);

	bool filter_was_single = false;

	can_master_filter_is_single_scale(can_master, filter_bank, &filter_was_single);

	if(filter_was_single) {
		can_master->sFilterRegister[filter_bank].FR1 = id;
		can_master->sFilterRegister[filter_bank].FR2 = mask;
	} else {
		can_filter_32b_t new_32b_id = {id};
		can_filter_32b_t new_32b_mask = {mask};

		can_filter_16b_t new_16b;

		//id
		new_16b.bit.id_exid_15_17 = new_32b_id.bit.exid_15_17;
		new_16b.bit.id_ide = new_32b_id.bit.ide;
		new_16b.bit.id_rtr = new_32b_id.bit.rtr;
		new_16b.bit.id_stid_0_10 = new_32b_id.bit.stid_0_10;
		//mask
		new_16b.bit.mask_exid_15_17 = new_32b_mask.bit.exid_15_17;
		new_16b.bit.mask_ide = new_32b_mask.bit.ide;
		new_16b.bit.mask_rtr = new_32b_mask.bit.rtr;
		new_16b.bit.mask_stid_0_10 = new_32b_mask.bit.stid_0_10;

		if (filter_bank_subindex) {
			can_master->sFilterRegister[filter_bank].FR2 = new_16b.all;
		} else {
			can_master->sFilterRegister[filter_bank].FR1 = new_16b.all;
		}
	}

	can_master_filter_set_active(can_master, filter_bank);

	return E_NO_ERROR;
}

err_t can_bus_filter_16b_bank_alloc(can_bus_t* bus, int count) {
	err_t err = E_NO_ERROR;

	CAN_TypeDef* can_master = bus->can_ptr[0];

	can_master_filter_init_mode(can_master);

	for(int index = 0; index < count; index++) {
		err = can_bus_filter_16b_bank_set(bus, index, 0, 0);
		if(err != E_NO_ERROR) break;
	}

	can_master_filter_active_mode(can_master);

	return err;
}

