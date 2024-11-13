/*
 * can_filter.c
 *
 *  Created on: 2 нояб. 2024 г.
 *      Author: Ruslan
 */

#include "can_bus.h"


void can_filter_init_mode(CAN_TypeDef* CAN) {
	CAN->FMR |= CAN_FMR_FINIT;
}

void can_filter_active_mode(CAN_TypeDef* CAN) {
	CAN->FMR &= ~CAN_FMR_FINIT;
}


err_t can2_filter_start_bank_set(CAN_TypeDef* CAN, int CAN2SB) {
	if(CAN2SB < 0) return E_INVALID_VALUE;
	if(CAN2SB > 27) return E_OUT_OF_RANGE;

	CAN->FMR &= ~CAN_FMR_CAN2SB;
	CAN->FMR |= (CAN_FMR_CAN2SB & (CAN2SB << CAN_FMR_CAN2SB_SHIFT));

	return E_NO_ERROR;
}


err_t can_filter_set_mask_mode(CAN_TypeDef* CAN, int FBM) {
	if(FBM < 0) return E_INVALID_VALUE;
	if(FBM > 27) return E_OUT_OF_RANGE;

	CAN->FM1R &= ~(CAN_FM1R_FBMx & (1 << FBM));

	return E_NO_ERROR;
}

err_t can_filter_set_list_mode(CAN_TypeDef* CAN, int FBM) {
	if(FBM < 0) return E_INVALID_VALUE;
	if(FBM > 27) return E_OUT_OF_RANGE;

	CAN->FM1R |= (CAN_FM1R_FBMx & (1 << FBM));

	return E_NO_ERROR;
}


err_t can_filter_read_mode_mask_list(CAN_TypeDef* CAN, int FBM, bool* list_mode) {
	if(FBM < 0) return E_INVALID_VALUE;
	if(FBM > 27) return E_OUT_OF_RANGE;

	if (CAN->FM1R & (CAN_FM1R_FBMx & (1 << FBM))) {
		*list_mode = true;
	} else {
		*list_mode = false;
	}

	return E_NO_ERROR;
}


err_t can_filter_set_dual_scale(CAN_TypeDef* CAN, int FSC) {
	if(FSC < 0) return E_INVALID_VALUE;
	if(FSC > 27) return E_OUT_OF_RANGE;

	CAN->FS1R &= ~(CAN_FS1R_FSCx & (1 << FSC));

	return E_NO_ERROR;
}

err_t can_filter_set_single_scale(CAN_TypeDef* CAN, int FSC) {
	if(FSC < 0) return E_INVALID_VALUE;
	if(FSC > 27) return E_OUT_OF_RANGE;

	CAN->FS1R |= (CAN_FS1R_FSCx & (1 << FSC));

	return E_NO_ERROR;
}

err_t can_filter_read_scale_dual_single(CAN_TypeDef* CAN, int FSC, bool* single_scale) {
	if(FSC < 0) return E_INVALID_VALUE;
	if(FSC > 27) return E_OUT_OF_RANGE;

	if(CAN->FS1R & (CAN_FS1R_FSCx & (1 << FSC))) {
		*single_scale = true;
	} else {
		*single_scale = false;
	}

	return E_NO_ERROR;
}


err_t can_filter_assigned_to_fifo_0(CAN_TypeDef* CAN, int FFA) {
	if(FFA < 0) return E_INVALID_VALUE;
	if(FFA > 27) return E_OUT_OF_RANGE;

	CAN->FFA1R &= ~(CAN_FFA1R_FFAx & (1 << FFA));

	return E_NO_ERROR;
}

err_t can_filter_assigned_to_fifo_1(CAN_TypeDef* CAN, int FFA) {
	if(FFA < 0) return E_INVALID_VALUE;
	if(FFA > 27) return E_OUT_OF_RANGE;

	CAN->FFA1R |= (CAN_FFA1R_FFAx & (1 << FFA));

	return E_NO_ERROR;
}


err_t can_filter_set_inactive(CAN_TypeDef* CAN, int FACT) {
	if(FACT < 0) return E_INVALID_VALUE;
	if(FACT > 27) return E_OUT_OF_RANGE;

	CAN->FA1R &= ~(CAN_FA1R_FACTx & (1 << FACT));

	return E_NO_ERROR;
}

err_t can_filter_set_active(CAN_TypeDef* CAN, int FACT) {
	if(FACT < 0) return E_INVALID_VALUE;
	if(FACT > 27) return E_OUT_OF_RANGE;

	CAN->FA1R |= (CAN_FA1R_FACTx & (1 << FACT));

	return E_NO_ERROR;
}

err_t can_filter_bank_set(CAN_TypeDef* CAN, int filter, uint32_t id, uint32_t mask) {
	if(filter < 0) return E_INVALID_VALUE;
	if(filter > 27) return E_OUT_OF_RANGE;

	//err_t err = E_NO_ERROR;

	/*err = */can_filter_set_inactive(CAN, filter);
	//if(err) return err;

	/*err = */can_filter_set_mask_mode(CAN, filter);
	//if(err) return err;

	/*err = */can_filter_set_single_scale(CAN, filter);
	//if(err) return err;

	CAN->sFilterRegister[filter].FR1 = id;
	CAN->sFilterRegister[filter].FR1 = mask;

	/*err = */can_filter_set_active(CAN, filter);
	//if(err) return err;

	return E_NO_ERROR;
}










