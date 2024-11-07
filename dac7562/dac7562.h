/*
 * dac7562.h
 *
 *  Created on: 21 дек. 2022 г.
 *      Author: Ruslan
 */

#ifndef INC_DAC7562_H_
#define INC_DAC7562_H_

#include <stdint.h>
#include <assert.h>
#include "defs/defs.h"

#define DAC7562_DATA_SIZE 2

typedef struct PACKED _DAC7562_DATA_BITS {
	unsigned db0 : 1;
	unsigned db1 : 1;
	unsigned db2 : 1;
	unsigned db3 : 1;
	unsigned db4 : 1;
	unsigned db5 : 1;
	unsigned db6 : 1;
	unsigned db7 : 1;
	unsigned db8 : 1;
	unsigned db9 : 1;
	unsigned db10 : 1;
	unsigned db11 : 1;
	unsigned db12 : 1;
	unsigned db13 : 1;
	unsigned db14 : 1;
	unsigned db15 : 1;
} DAC7562_DATA_BITS;
static_assert(sizeof(DAC7562_DATA_BITS) == DAC7562_DATA_SIZE, "Invalid size of DAC7562_DATA_BITS!");

typedef union _DAC7562_DATA_BITS_REG {
	uint16_t all;
	struct _DAC7562_DATA_BITS bit;
} DAC7562_DATA_BITS_REG;

#define DAC7562_CMD_ADDR_SIZE 1

typedef struct PACKED _DAC7562_CMD_ADDR_BITS {
	unsigned addr : 3;
	unsigned cmd : 3;
	unsigned reserved : 2;
} DAC7562_CMD_ADDR_BITS;
static_assert(sizeof(DAC7562_CMD_ADDR_BITS) == DAC7562_CMD_ADDR_SIZE, "Invalid size of DAC7562_CMD_ADDR_BITS!");

typedef union _DAC7562_CMD_ADDR_BITS_REG {
	uint8_t all;
	struct _DAC7562_CMD_ADDR_BITS bit;
} DAC7562_CMD_ADDR_BITS_REG;

#define DAC7562_FRAME_SIZE (DAC7562_DATA_SIZE + DAC7562_CMD_ADDR_SIZE)

typedef struct PACKED _DAC7562_TX_FRAME_REG {
	DAC7562_DATA_BITS_REG data;
	DAC7562_CMD_ADDR_BITS_REG cmd_addr;
} DAC7562_TX_FRAME_REG;

extern void dac7562_tx_frame_write_data(DAC7562_TX_FRAME_REG* tx_frame, uint16_t data);

#endif /* INC_DAC7562_H_ */
