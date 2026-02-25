/*
 * can_filter.c
 *
 *  Created on: 2 нояб. 2024 г.
 *      Author: Ruslan
 */

#include "can_master_filter.h"
#include "can_reg.h"

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

