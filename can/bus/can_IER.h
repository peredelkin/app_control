/*
 * can_IER.h
 *
 *  Created on: 24 мар. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_BUS_CAN_IER_H_
#define CAN_BUS_CAN_IER_H_

#include <stdbool.h>
#include "lib/stm32f4xx/stm32f4xx.h"

//Address offset: 0x14
extern void can_IER_SLKIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_SLKIE_read(CAN_TypeDef* can);
extern void can_IER_WKUIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_WKUIE_read(CAN_TypeDef* can);
extern void can_IER_ERRIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_ERRIE_read(CAN_TypeDef* can);
extern void can_IER_LECIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_LECIE_read(CAN_TypeDef* can);
extern void can_IER_BOFIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_BOFIE_read(CAN_TypeDef* can);
extern void can_IER_EPVIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_EPVIE_read(CAN_TypeDef* can);
extern void can_IER_EWGIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_EWGIE_read(CAN_TypeDef* can);
extern void can_IER_FOVIE_set(CAN_TypeDef* can, int fifo, bool state);
extern uint32_t can_IER_FOVIE_read(CAN_TypeDef* can, int fifo);
extern void can_IER_FFIE_set(CAN_TypeDef* can, int fifo, bool state);
extern uint32_t can_IER_FFIE_read(CAN_TypeDef* can, int fifo);
extern void can_IER_FMPIE_set(CAN_TypeDef* can, int fifo, bool state);
extern uint32_t can_IER_FMPIE_read(CAN_TypeDef* can, int fifo);
extern void can_IER_TMEIE_set(CAN_TypeDef* can, bool state);
extern uint32_t can_IER_TMEIE_read(CAN_TypeDef* can);

#endif /* CAN_BUS_CAN_IER_H_ */
