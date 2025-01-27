/*
 * modbus_panel_init.c
 *
 *  Created on: 9 авг. 2024 г.
 *      Author: Ruslan
 */

#include "modbus_init.h"
#include "modbus/rtu/modbus_rtu.h"
#include "uart/int/usart_int.h"
#include "uart/usart.h"
#include "gpio/init/gpio_init.h"
#include "uart/init/uart_init.h"
#include "modules/modules.h"
#include <string.h>

modbus_rtu_t modbus_panel;

modbus_rtu_message_t modbus_panel_rx_msg, modbus_panel_tx_msg;

bool modbus_panel_rx_callback(void)
{
    return modbus_rtu_usart_rx_callback(&modbus_panel);
}

bool modbus_panel_tx_callback(void)
{
    return modbus_rtu_usart_tx_callback(&modbus_panel);
}

bool modbus_panel_rx_byte_callback(uint8_t byte)
{
    return modbus_rtu_usart_rx_byte_callback(&modbus_panel, byte);
}

static void modbus_panel_on_msg_recv_callback(void)
{
	rs485_panel_set_out();
    if(modbus_rtu_dispatch(&modbus_panel) != E_NO_ERROR) {
    	rs485_panel_set_in();
    }
}

static modbus_rtu_error_t modbus_panel_on_report_slave_id(modbus_rtu_slave_id_t* slave_id)
{
    // Состояние - работаем.
    slave_id->status = MODBUS_RTU_RUN_STATUS_ON;
    // Идентификатор - для пример возьмём 0xaa.
    slave_id->id = 32;
    // В дополнительных данных передадим наше имя.
    slave_id->data = "STM32 MCU Modbus v1.0";
    // Длина имени.
    slave_id->data_size = strlen(slave_id->data);

    return MODBUS_RTU_ERROR_NONE;
}

static modbus_rtu_error_t modbus_on_read_hold_reg(uint16_t address, uint16_t* value)
{
    switch(address) {
    case 0: *value = ntc_temp.out_temp[0] >> 15;
    	break;
    case 1: *value = ntc_temp.out_temp[1] >> 15;
    	break;
    case 2: *value = ntc_temp.out_temp[2] >> 15;
    	break;
    case 3: *value = ntc_temp.out_temp[3] >> 15;
    	break;
    case 4: *value = ntc_temp.out_temp[4] >> 15;
    	break;
    case 5: *value = ntc_temp.out_temp[5] >> 15;
    	break;
    default: return MODBUS_RTU_ERROR_INVALID_ADDRESS;
    }

    return MODBUS_RTU_ERROR_NONE;
}

static modbus_rtu_error_t modbus_on_write_hold_reg(uint16_t address, uint16_t value) {
	switch(address) {
	default: return MODBUS_RTU_ERROR_INVALID_ADDRESS;
	}

	return MODBUS_RTU_ERROR_NONE;
}

static modbus_io_t modbus_panel_io = {
		.usart = &uart_7,
		.send = (void*)&usart_int_send,
		.recv = (void*)&usart_int_recv,
		.bytes_received = (void*)&usart_int_bytes_received,
		.rx_busy = (void*)&usart_int_rx_busy,
		.receiver_disable = (void*)&usart_int_receiver_disable,
		.receiver_enable = (void*)&usart_int_receiver_enable,
		.sleep = (void*)&usart_int_sleep,
		.tx_errors = (void*)&usart_int_tx_errors,
		.rx_errors = (void*)&usart_int_rx_errors
};

void modbus_panel_init(void)
{
    // Структура инициализации Modbus.
    modbus_rtu_init_t modbus_is;

    modbus_is.io = &modbus_panel_io;
    modbus_is.mode = MODBUS_RTU_MODE_SLAVE; // Режим - ведомый.
    modbus_is.address = 1; // Адрес.
    modbus_is.rx_message = &modbus_panel_rx_msg; // Сообщение для приёма.
    modbus_is.tx_message = &modbus_panel_tx_msg; // Сообщение для передачи.

    // Инициализируем Modbus.
    modbus_rtu_init(&modbus_panel, &modbus_is);
    // Установка каллбэка получения сообщения.
    modbus_rtu_set_msg_recv_callback(&modbus_panel, modbus_panel_on_msg_recv_callback);
    // Установка каллбэков доступа к данным.
    //modbus_rtu_set_read_coil_callback(&modbus_1, modbus_on_read_coil);
    //modbus_rtu_set_write_coil_callback(&modbus_1, modbus_on_write_coil);
    modbus_rtu_set_report_slave_id_callback(&modbus_panel, modbus_panel_on_report_slave_id);
    modbus_rtu_set_read_holding_reg_callback(&modbus_panel, modbus_on_read_hold_reg);
    modbus_rtu_set_write_holding_reg_callback(&modbus_panel, modbus_on_write_hold_reg);
}
