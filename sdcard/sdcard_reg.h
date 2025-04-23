/*
 * sdcard_reg.h
 *
 *  Created on: 23 апр. 2025 г.
 *      Author: Ruslan
 */

#ifndef SDCARD_SDCARD_REG_H_
#define SDCARD_SDCARD_REG_H_

#include <stdint.h>

typedef union {
	struct {
		unsigned TRAN_SPEED		:8;
		unsigned NSAC			:8;
		unsigned TAAC			:8;
		unsigned RESERVED		:6;
		unsigned CSD_STRUCTURE	:2;
	} bit;
	uint32_t all;
} sdcard_reg_CSD_R2_t;

#endif /* SDCARD_SDCARD_REG_H_ */
