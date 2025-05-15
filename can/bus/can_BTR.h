/*
 * can_BTR.h
 *
 *  Created on: 24 мар. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_BUS_CAN_BTR_H_
#define CAN_BUS_CAN_BTR_H_

#include "lib/stm32f4xx/stm32f4xx.h"

//Address offset: 0x1C
extern void can_BTR_set(CAN_TypeDef* can, uint32_t btr);

#endif /* CAN_BUS_CAN_BTR_H_ */
