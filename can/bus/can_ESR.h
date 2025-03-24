/*
 * can_ESR.h
 *
 *  Created on: 24 мар. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_BUS_CAN_ESR_H_
#define CAN_BUS_CAN_ESR_H_

#include "lib/stm32f4xx/stm32f4xx.h"

//Address offset: 0x18
extern uint32_t can_ESR_read(CAN_TypeDef *can);
extern uint32_t can_ESR_REC_read(uint32_t ESR);
extern uint32_t can_ESR_TEC_read(uint32_t ESR);
extern uint32_t can_ESR_LEC_read(uint32_t ESR);
extern uint32_t can_ESR_BOFF_read(uint32_t ESR);
extern uint32_t can_ESR_EPVF_read(uint32_t ESR);
extern uint32_t can_ESR_EWGF_read(uint32_t ESR);

#endif /* CAN_BUS_CAN_ESR_H_ */
