/*
 * can_init.h
 *
 *  Created on: 6 нояб. 2024 г.
 *      Author: Ruslan
 */

#ifndef CAN_INIT_CAN_INIT_H_
#define CAN_INIT_CAN_INIT_H_

#include <stdint.h>

/*
 * |----|    |---------------------------|
 * | PC |    |            APP            |
 * |----|    |---------------------------|
 *   |       |    CAN2     |    CAN1     |
 *   |       |---------------------------|
 *   |       |             | SDO CLIENT  |-
 *   |       |             |      600    | \      |-------------|
 *   |       |             |      580    |  \     |      MC     |
 *   |       |-------------|-------------|   \    |-------------|
 *   |       | SDO SERVER  | SDO SERVER  |    --> | SDO SERVER  |
 *   |       |      600    |      600    |        |      600    |
 *   |       |      580    |      580    |        |      580    |
 *   |       |-------------|-------------|        |-------------|
 *   |-----> | SDO SERVER  | SDO SERVER  |    --> | SDO SERVER  |
 *    \      |      640+2  |      640+2  |   /    |      640+2  |
 *     \     |      620+2  |      620+2  |  /     |      620+2  |
 *      \    |-------------|-------------| /      |-------------|
 *       \-> | BRIDGE      | BRIDGE      |-
 *           | IN:  641    | OUT: 641    |
 *           | OUT: 621    | in:  621    |
 *           |---------------------------|
 */

extern void can1_nvic_init(uint32_t priority);
extern void can2_nvic_init(uint32_t priority);
extern void can_canopen_init(void);

#endif /* CAN_INIT_CAN_INIT_H_ */
