/*
 * can_TSR.h
 *
 *  Created on: 24 мар. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_BUS_CAN_TSR_H_
#define CAN_BUS_CAN_TSR_H_

#include <stdbool.h>
#include "lib/stm32f4xx/stm32f4xx.h"

//Address offset: 0x08
extern uint32_t can_TSR_read(CAN_TypeDef* can);
extern int can_TSR_LOW_get(uint32_t TSR);
extern int can_TSR_TME_get(uint32_t TSR);
extern int can_TSR_CODE_get(uint32_t TSR);
extern void can_TSR_ABRQ_set(CAN_TypeDef* can, int mailbox);
extern bool can_TSR_TERR_get(uint32_t TSR, int mailbox);
extern void can_TSR_TERR_set(CAN_TypeDef* can, int mailbox);
extern bool can_TSR_ALST_get(uint32_t TSR, int mailbox);
extern void can_TSR_ALST_set(CAN_TypeDef* can, int mailbox);
extern bool can_TSR_TXOK_get(uint32_t TSR, int mailbox);
extern void can_TSR_TXOK_clear(CAN_TypeDef* can, int mailbox);
extern bool can_TSR_RQCP_get(uint32_t TSR, int mailbox);
extern void can_TSR_RQCP_clear(CAN_TypeDef* can, int mailbox);

#endif /* CAN_BUS_CAN_TSR_H_ */
