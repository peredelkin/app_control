/*
 * modbus_init.h
 *
 *  Created on: 23 июл. 2024 г.
 *      Author: Ruslan
 */

#ifndef MODBUS_INIT_MODBUS_INIT_H_
#define MODBUS_INIT_MODBUS_INIT_H_

#include <stdbool.h>
#include <stdint.h>

extern bool modbus1_rx_callback(void);
extern bool modbus1_tx_callback(void);
extern bool modbus1_rx_byte_callback(uint8_t byte);
extern void modbus1_init(void);

extern bool modbus_panel_rx_callback(void);
extern bool modbus_panel_tx_callback(void);
extern bool modbus_panel_rx_byte_callback(uint8_t byte);
extern void modbus_panel_init(void);

#endif /* MODBUS_INIT_MODBUS_INIT_H_ */
