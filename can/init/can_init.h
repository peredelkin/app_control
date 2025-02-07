/*
 * can_init.h
 *
 *  Created on: 6 нояб. 2024 г.
 *      Author: Ruslan
 */

#ifndef CAN_INIT_CAN_INIT_H_
#define CAN_INIT_CAN_INIT_H_

#include "can/CANopenNode/CANopen.h"
#include "can/bus/can_bus.h"

extern void can1_nvic_init(uint32_t priority);
extern void can1_init(void);

#endif /* CAN_INIT_CAN_INIT_H_ */
