/*
 * sdcard_reg.h
 *
 *  Created on: 23 апр. 2025 г.
 *      Author: Ruslan
 */

#ifndef SDCARD_SDCARD_REG_H_
#define SDCARD_SDCARD_REG_H_

#include <stdint.h>
#include <assert.h>

#define SDCARD_CID_SIZE 16

#pragma pack(push, 1)
typedef union {
	struct {
	    unsigned NOT_USED	:1;
	    unsigned CRC7		:7;
	    unsigned MDT_M		:4;
	    unsigned MDT_Y		:8;
	    unsigned RESERVED	:4;
	    uint8_t PSN[4];
	    uint8_t PRV;
	    uint8_t PNM[5];
	    uint8_t OID[2];
	    uint8_t MID;
	} bit;
	uint32_t all[4];
} sdcard_reg_CID_R2_t;
#pragma pack(pop)

static_assert(sizeof(sdcard_reg_CID_R2_t) == SDCARD_CID_SIZE, "Invalid size of sdcard CID!");

#endif /* SDCARD_SDCARD_REG_H_ */
