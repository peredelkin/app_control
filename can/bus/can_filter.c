/*
 * can_filter.c
 *
 *  Created on: 2 нояб. 2024 г.
 *      Author: Ruslan
 */

#include "can_bus.h"

void can_filter_init_mode(CAN_TypeDef *CAN) {
	CAN->FMR |= CAN_FMR_FINIT;
}

void can_filter_active_mode(CAN_TypeDef *CAN) {
	CAN->FMR &= ~CAN_FMR_FINIT;
}

err_t can2_filter_start_bank_set(CAN_TypeDef *CAN, int CAN2SB) {
	if (CAN2SB < 0) return E_INVALID_VALUE;
	if (CAN2SB > 27) return E_OUT_OF_RANGE;

	CAN->FMR &= ~CAN_FMR_CAN2SB;
	CAN->FMR |= (CAN_FMR_CAN2SB & (CAN2SB << CAN_FMR_CAN2SB_SHIFT));

	return E_NO_ERROR;
}

err_t can_filter_set_mask_mode(CAN_TypeDef *CAN, int FBM) {
	if (FBM < 0) return E_INVALID_VALUE;
	if (FBM > 27) return E_OUT_OF_RANGE;

	CAN->FM1R &= ~(CAN_FM1R_FBMx & (1 << FBM));

	return E_NO_ERROR;
}

err_t can_filter_set_list_mode(CAN_TypeDef *CAN, int FBM) {
	if (FBM < 0) return E_INVALID_VALUE;
	if (FBM > 27) return E_OUT_OF_RANGE;

	CAN->FM1R |= (CAN_FM1R_FBMx & (1 << FBM));

	return E_NO_ERROR;
}

err_t can_filter_is_list_mode(CAN_TypeDef *CAN, int FBM, bool *list_mode) {
	if (FBM < 0) return E_INVALID_VALUE;
	if (FBM > 27) return E_OUT_OF_RANGE;

	if (CAN->FM1R & (CAN_FM1R_FBMx & (1 << FBM))) {
		*list_mode = true;
	} else {
		*list_mode = false;
	}

	return E_NO_ERROR;
}

err_t can_filter_set_dual_scale(CAN_TypeDef *CAN, int FSC) {
	if (FSC < 0) return E_INVALID_VALUE;
	if (FSC > 27) return E_OUT_OF_RANGE;

	CAN->FS1R &= ~(CAN_FS1R_FSCx & (1 << FSC));

	return E_NO_ERROR;
}

err_t can_filter_set_single_scale(CAN_TypeDef *CAN, int FSC) {
	if (FSC < 0) return E_INVALID_VALUE;
	if (FSC > 27) return E_OUT_OF_RANGE;

	CAN->FS1R |= (CAN_FS1R_FSCx & (1 << FSC));

	return E_NO_ERROR;
}

err_t can_filter_is_single_scale(CAN_TypeDef *CAN, int FSC, bool *single_scale) {
	if (FSC < 0) return E_INVALID_VALUE;
	if (FSC > 27) return E_OUT_OF_RANGE;

	if (CAN->FS1R & (CAN_FS1R_FSCx & (1 << FSC))) {
		*single_scale = true;
	} else {
		*single_scale = false;
	}

	return E_NO_ERROR;
}

err_t can_filter_assigned_to_fifo_0(CAN_TypeDef *CAN, int FFA) {
	if (FFA < 0) return E_INVALID_VALUE;
	if (FFA > 27) return E_OUT_OF_RANGE;

	CAN->FFA1R &= ~(CAN_FFA1R_FFAx & (1 << FFA));

	return E_NO_ERROR;
}

err_t can_filter_assigned_to_fifo_1(CAN_TypeDef *CAN, int FFA) {
	if (FFA < 0) return E_INVALID_VALUE;
	if (FFA > 27) return E_OUT_OF_RANGE;

	CAN->FFA1R |= (CAN_FFA1R_FFAx & (1 << FFA));

	return E_NO_ERROR;
}

err_t can_filter_is_active(CAN_TypeDef *CAN, int FACT, bool *is_active) {
	if (FACT < 0) return E_INVALID_VALUE;
	if (FACT > 27) return E_OUT_OF_RANGE;

	if (CAN->FA1R & (CAN_FA1R_FACTx & (1 << FACT))) {
		*is_active = true;
	} else {
		*is_active = false;
	}

	return E_NO_ERROR;
}

err_t can_filter_set_inactive(CAN_TypeDef *CAN, int FACT) {
	if (FACT < 0) return E_INVALID_VALUE;
	if (FACT > 27) return E_OUT_OF_RANGE;

	CAN->FA1R &= ~(CAN_FA1R_FACTx & (1 << FACT));

	return E_NO_ERROR;
}

err_t can_filter_set_active(CAN_TypeDef *CAN, int FACT) {
	if (FACT < 0) return E_INVALID_VALUE;
	if (FACT > 27) return E_OUT_OF_RANGE;

	CAN->FA1R |= (CAN_FA1R_FACTx & (1 << FACT));

	return E_NO_ERROR;
}

err_t can_filter_32b_bank_set(CAN_TypeDef *CAN, int filter, uint32_t id, uint32_t mask) {
	if (filter < 0) return E_INVALID_VALUE;
	if (filter > 27) return E_OUT_OF_RANGE;

	can_filter_set_inactive(CAN, filter);

	can_filter_set_mask_mode(CAN, filter);

	can_filter_set_single_scale(CAN, filter);

	CAN->sFilterRegister[filter].FR1 = id;
	CAN->sFilterRegister[filter].FR2 = mask;

	can_filter_set_active(CAN, filter);

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

err_t can_filter_16b_bank_set(CAN_TypeDef *CAN, int filter, uint32_t id, uint32_t mask) {
	if (filter < 0) return E_INVALID_VALUE;
	if (filter > 55) return E_OUT_OF_RANGE;

	int filter_index = (filter >> 1);
	int filter_subindex = (filter & 0b1);

	if (filter_index > 27) return E_OUT_OF_RANGE;

	bool filter_was_active = false;
	bool filter_was_single = false;

	can_filter_32b_t prev_32b_id;
	can_filter_32b_t prev_32b_mask;

	can_filter_32b_t next_32b_id;
	can_filter_32b_t next_32b_mask;

	can_filter_16b_t new_16b[2];

	can_filter_is_active(CAN, filter_index, &filter_was_active);

	can_filter_is_single_scale(CAN, filter_index, &filter_was_single);

	can_filter_set_inactive(CAN, filter_index);

	can_filter_set_mask_mode(CAN, filter_index);

	if (filter_subindex) {
		//фильтры должны быть настроены последовательно!
		if(filter_was_active == false || filter_was_single == false) return E_INVALID_OPERATION;

		prev_32b_id.all = CAN->sFilterRegister[filter_index].FR1;
		prev_32b_mask.all = CAN->sFilterRegister[filter_index].FR2;

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

		can_filter_set_dual_scale(CAN, filter_index);

		CAN->sFilterRegister[filter_index].FR1 = new_16b[0].all;
		CAN->sFilterRegister[filter_index].FR2 = new_16b[1].all;
	} else {
		can_filter_set_single_scale(CAN, filter_index);

		CAN->sFilterRegister[filter_index].FR1 = id;
		CAN->sFilterRegister[filter_index].FR2 = mask;
	}

	can_filter_set_active(CAN, filter_index);

	return E_NO_ERROR;
}

