/*
 * can_MSR.h
 *
 *  Created on: 24 мар. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_BUS_CAN_MSR_H_
#define CAN_BUS_CAN_MSR_H_

#include "lib/stm32f4xx/stm32f4xx.h"

//Address offset: 0x04
extern uint32_t can_MSR_read(CAN_TypeDef* can);
extern void can_MSR_SLAKI_clear(CAN_TypeDef* can);
extern uint32_t can_MSR_SLAKI_read(uint32_t MSR);
extern void can_MSR_WKUI_clear(CAN_TypeDef* can);
extern uint32_t can_MSR_WKUI_read(uint32_t MSR);
extern void can_MSR_ERRI_clear(CAN_TypeDef* can);
extern uint32_t can_MSR_ERRI_read(uint32_t MSR);
extern uint32_t can_MSR_SLAK_read(CAN_TypeDef* can);
extern uint32_t can_MSR_INAK_read(CAN_TypeDef* can);

#endif /* CAN_BUS_CAN_MSR_H_ */
