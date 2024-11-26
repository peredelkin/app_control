/*
 * dac7562.c
 *
 *  Created on: 23 дек. 2022 г.
 *      Author: Ruslan
 */

#include "dac7562.h"

void dac7562_tx_frame_write_data(DAC7562_TX_FRAME_REG* tx_frame, uint16_t data) {
	tx_frame->data.all = data;
}

