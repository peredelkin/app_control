/*
 * tic12400.c
 *
 *  Created on: 5 апр. 2022 г.
 *      Author: Ruslan
 */

#include "tic12400.h"

void tic12400_handler(void *tic);

bool tic12400_wait(tic12400_t *tic) {
	while(tic->done == false);
	return tic->par_fail;
}

void tic12400_bus_wait(tic12400_t *tic) {
	spi_bus_wait(tic->spi_bus);
	tic->done = false;
}

void tic12400_bus_free(tic12400_t *tic) {
	tic->done = true;
	spi_bus_free(tic->spi_bus);
}

void tic12400_transfer(tic12400_t *tic) {
	tic->par_fail = false;
	spi_bus_transfer(tic->spi_bus, &tic->spi_control, 1, SPI_BYTE_ORDER_REVERSE, &tic12400_handler, tic);
}

void tic12400_transfer_from_callback(tic12400_t *tic) {
	spi_bus_transfer_from_callback(tic->spi_bus, &tic->spi_control, 1, SPI_BYTE_ORDER_REVERSE, &tic12400_handler, tic);
}

void tic124_tx_frame_fill(tic12400_t *tic, uint32_t rw, uint32_t addr, uint32_t data) {
	tic->tx_frame.all = 0;
	tic->tx_frame.bit.rw = rw;
	tic->tx_frame.bit.addr = addr;
	tic->tx_frame.bit.data = data;
	tic->tx_frame.bit.par = calc_parity(tic->tx_frame.all, 32, PARITY_ODD);
}

bool tic12400_rx_frame_parity_check(tic12400_t *tic) {
	int par = tic->rx_frame.bit.par;
	tic->rx_frame.bit.par = 0;
	tic->rx_frame.bit.par = calc_parity(tic->rx_frame.all, 32, PARITY_ODD);
	return ((par) == (tic->rx_frame.bit.par));
}

void tic12400_tx_handler(tic12400_t *tic) {
	if (tic->sequential.data) {
		tic->sequential.index++;
		if (tic->sequential.index < tic->sequential.end) {
			tic124_tx_frame_fill(tic, 1, tic->sequential.addr[tic->sequential.index],
					tic->sequential.data[tic->sequential.index]);
			tic12400_transfer_from_callback(tic);
		} else {
			tic12400_bus_free(tic);
		}
	} else {
		tic12400_bus_free(tic);
	}
}

void tic12400_rx_handler(tic12400_t *tic) {
	if (tic->sequential.data) {
		tic->sequential.data[tic->sequential.index] = tic->rx_frame.bit.data;
		tic->sequential.index++;
		if (tic->sequential.index < tic->sequential.end) {
			tic124_tx_frame_fill(tic, 0, tic->sequential.addr[tic->sequential.index], 0);
			tic12400_transfer_from_callback(tic);
		} else {
			tic12400_bus_free(tic);
		}
	} else {
		tic12400_bus_free(tic);
	}
}

void tic12400_tx_rx_handler(tic12400_t *tic) {
	if (tic->tx_frame.bit.rw) {
		tic12400_tx_handler(tic);
	} else {
		tic12400_rx_handler(tic);
	}
}

void tic12400_rx_frame_parity_handler(tic12400_t *tic) {
	if (tic12400_rx_frame_parity_check(tic)) {
		tic12400_tx_rx_handler(tic);
	} else {
		tic->par_fail = true;
		tic12400_bus_free(tic);
	}
}

void tic12400_handler(void *tic) {
	tic12400_rx_frame_parity_handler(tic);
}

void tic12400_init(tic12400_t *tic, SPI_BUS_TypeDef *spi_bus, const CFG_REG_SPI_TypeDef *spi_cfg) {
	tic->spi_bus = spi_bus;
	tic->spi_cfg = spi_cfg;

	tic->spi_control.tx = (uint8_t*) &tic->tx_frame.all;
	tic->spi_control.rx = (uint8_t*) &tic->rx_frame.all;
	tic->spi_control.count = TIC12400_FRAME_SIZE;

	tic->tx_frame.all = 0;
	tic->rx_frame.all = 0;

	tic->sequential.end = 0;
	tic->sequential.index = 0;
	tic->sequential.addr = NULL;
	tic->sequential.data = NULL;

	tic->par_fail = false;

	tic->done = true;
}

void tic12400_sequential_fill(tic12400_t *tic, uint32_t *data, const uint8_t *addr, uint8_t start, uint8_t count) {
	tic->sequential.end = start + count;
	tic->sequential.index = start;
	tic->sequential.addr = addr;
	tic->sequential.data = data;
}

void tic12400_reg_read(tic12400_t *tic, uint32_t *data, const uint8_t *addr, uint8_t start, uint8_t count) {
	tic12400_bus_wait(tic);
	tic12400_sequential_fill(tic, data, addr, start, count);
	tic124_tx_frame_fill(tic, 0, addr[tic->sequential.index], 0);
	tic12400_transfer(tic);
}

void tic12400_reg_write(tic12400_t *tic, uint32_t *data, const uint8_t *addr, uint8_t start, uint8_t count) {
	tic12400_bus_wait(tic);
	tic12400_sequential_fill(tic, data, addr, start, count);
	tic124_tx_frame_fill(tic, 1, tic->sequential.addr[tic->sequential.index], tic->sequential.data[tic->sequential.index]);
	tic12400_transfer(tic);
}

