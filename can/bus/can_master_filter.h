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

#define CAN_FILTER_MAX_COUNT	56	//16b mask + 16b id

extern void can_master_filter_init_mode(CAN_TypeDef *CAN);
extern err_t can_master_can2_filter_start_bank_set(CAN_TypeDef *CAN, int CAN2SB);
extern void can_master_filter_reset_all(CAN_TypeDef *CAN);

#endif /* CAN_BUS_CAN_MASTER_FILTER_H_ */
