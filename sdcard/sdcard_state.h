/*
 * sdcard_state.h
 *
 *  Created on: 15 апр. 2025 г.
 *      Author: Ruslan
 */

#ifndef SDCARD_SDCARD_STATE_H_
#define SDCARD_SDCARD_STATE_H_

typedef enum {
	SDCARD_STATE_ILLEGAL = -1,
	SDCARD_STATE_IDLE,
	SDCARD_STATE_READY,
	SDCARD_STATE_IDENT,
	SDCARD_STATE_STBY,
	SDCARD_STATE_TRAN,
	SDCARD_STATE_DATA,
	SDCARD_STATE_RCV,
	SDCARD_STATE_PRG,
	SDCARD_STATE_DIS,
	SDCARD_STATE_INA,
	SDCARD_STATE_COUNT
} sdcard_state_t;

typedef struct {
	sdcard_state_t state_array[SDCARD_STATE_COUNT];
} sdcard_state_cmd_t;

#define SDCARD_STATE_CMD_MAKE(idle, ready, ident, stby, train, data, rcv, prg, dis, ina)\
		{\
				.state_array[0] = idle,\
				.state_array[1] = ready,\
				.state_array[2] = ident,\
				.state_array[3] = stby,\
				.state_array[4] = train,\
				.state_array[5] = data,\
				.state_array[6] = rcv,\
				.state_array[7] = prg,\
				.state_array[8] = dis,\
				.state_array[9] = ina,\
		}

#include "sdcard.h"

#endif /* SDCARD_SDCARD_STATE_H_ */
