/*
 * can_filter.h
 *
 *  Created on: 21 мар. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_BUS_CAN_MASTER_FILTER_H_
#define CAN_BUS_CAN_MASTER_FILTER_H_

#include <stdbool.h>
#include "lib/errors/errors.h"
#include "lib/stm32f4xx/stm32f4xx.h"

extern void can_master_filter_init_mode(CAN_TypeDef *CAN);
extern void can_master_filter_active_mode(CAN_TypeDef *CAN);
extern err_t can_master_can2_filter_start_bank_set(CAN_TypeDef *CAN, int CAN2SB);
extern int can_master_can2_filter_start_bank_get(CAN_TypeDef *CAN);
extern err_t can_master_filter_set_mask_mode(CAN_TypeDef *CAN, int FBM);
extern err_t can_master_filter_set_list_mode(CAN_TypeDef *CAN, int FBM);
extern err_t can_master_filter_is_list_mode(CAN_TypeDef *CAN, int FBM, bool *list_mode);
extern err_t can_master_filter_set_dual_scale(CAN_TypeDef *CAN, int FSC);
extern err_t can_master_filter_set_single_scale(CAN_TypeDef *CAN, int FSC);
extern err_t can_master_filter_is_single_scale(CAN_TypeDef *CAN, int FSC, bool *single_scale);
extern err_t can_master_filter_assigned_to_fifo_0(CAN_TypeDef *CAN, int FFA);
extern err_t can_master_filter_assigned_to_fifo_1(CAN_TypeDef *CAN, int FFA);
extern err_t can_master_filter_is_active(CAN_TypeDef *CAN, int FACT, bool *is_active);
extern err_t can_master_filter_set_inactive(CAN_TypeDef *CAN, int FACT);
extern err_t can_master_filter_set_active(CAN_TypeDef *CAN, int FACT);
extern void can_master_filter_reset_all(CAN_TypeDef *CAN);

#endif /* CAN_BUS_CAN_MASTER_FILTER_H_ */
