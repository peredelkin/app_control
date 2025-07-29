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

static modbus_rtu_error_t modbus_panel_on_read_hold_reg(uint16_t address, uint16_t* value)
{
	uint16_t addr_h = (address >> 1);
	uint16_t addr_l = (address & 0x1);

    switch(addr_h) {
    case 0: *value = ((uint16_t*)&(ntc_temp.out_temp[0]))[addr_l];
    	break;
    case 1: *value = ((uint16_t*)&(ntc_temp.out_temp[1]))[addr_l];
    	break;
    case 2: *value = ((uint16_t*)&(ntc_temp.out_temp[2]))[addr_l];
    	break;
    case 3: *value = ((uint16_t*)&(ntc_temp.out_temp[3]))[addr_l];
    	break;
    case 4: *value = ((uint16_t*)&(ntc_temp.out_temp[4]))[addr_l];
    	break;
    case 5: *value = 0;
    	break;
    case 6: *value = panel_led.out_data;
    	break;
    default: return MODBUS_RTU_ERROR_INVALID_ADDRESS;
    }

    return MODBUS_RTU_ERROR_NONE;
}

static modbus_rtu_error_t modbus_panel_on_write_hold_reg(uint16_t address, uint16_t value) {
	switch(address) {
	default: return MODBUS_RTU_ERROR_INVALID_ADDRESS;
	}

	return MODBUS_RTU_ERROR_NONE;
}

enum {
	MODBUS_RTU_CUSTOM_FUNC_REG_READ = 0x64,
	MODBUS_RTU_CUSTOM_FUNC_REG_WRITE,
};

#pragma pack(push, 1)
struct {
		reg_id_t id;
		uint8_t count;
	} panel_request;
#pragma pack(pop)

#pragma pack(push, 1)
	struct {
		reg_id_t id;
		uint8_t count;
	} panel_response;
#pragma pack(pop)

//! Окончание списка.
//! Идентификатор.
#define SETTINGS_END_ID ((uint32_t)-1)
//! Тип данных.
#define SETTINGS_END_TYPE ((uint8_t)-1)
//! Размер данных.
#define SETTINGS_END_DATA_SIZE ((uint8_t)-1)

size_t buf_put_reg(void* data_to, size_t index, size_t index_max, const void *data_from, size_t size) {
	uint8_t *data_to_bytes = (uint8_t*) data_to;
	uint8_t *data_from_bytes = (uint8_t*) data_from;

	size_t i = 0;
	for (; i < size; i++) {
		if (index >= index_max) break;
		if (data_from_bytes) data_to_bytes[index] = data_from_bytes[i];
		index++;
	}
	return i;
}

size_t buf_get_reg(void* data_from, size_t index, size_t index_max, void *data_to, size_t size) {
	uint8_t *data_from_bytes = (uint8_t*) data_from;
	uint8_t *data_to_bytes = (uint8_t*) data_to;

	size_t i = 0;
	for (; i < size; i++) {
		if (index >= index_max) break;
		if (data_to_bytes) data_to_bytes[i] = data_from_bytes[index];
		index++;
	}
	return i;
}

int buf_put_reg_atomic(void* data_to, size_t* m_index, size_t index_max, reg_t* reg)
{
    size_t id_putted = 0;
    size_t type_putted = 0;
    size_t size_putted = 0;
    size_t data_putted = 0;

    void* data = reg->data;
    if(data == NULL) return 0;

    size_t data_size = reg_data_size(reg);
    if(data_size == 0) return 0;

    uint8_t type = (uint8_t)reg->type;
    uint8_t size = (uint8_t)data_size;
    uint32_t id = (uint32_t)reg->id;

    size_t index = *m_index;

    if(index + sizeof(id) > index_max) return -1;
    id_putted = buf_put_reg(data_to, index, index_max, &id, sizeof(id));
    if(id_putted != sizeof(id)) return -1;

    index += id_putted;

    if(index + sizeof(type) > index_max) return -1;
    type_putted = buf_put_reg(data_to, index, index_max, &type, sizeof(type));
    if(type_putted != sizeof(type)) return -1;

    index += type_putted;

    if(index + sizeof(size) > index_max) return -1;
    size_putted = buf_put_reg(data_to, index, index_max, &size, sizeof(size));
    if(size_putted != sizeof(size)) return -1;

    index += size_putted;

    if(index + data_size > index_max) return -1;
    data_putted = buf_put_reg(data_to, index, index_max, data, data_size);
    if(data_putted != data_size) return -1;

    index += data_putted;

    *m_index = index;

    return (int)(sizeof(id) + sizeof(type) + sizeof(size) + data_size);
}

int buf_get_reg_atomic(void* data_from, size_t* m_index, size_t index_max, reg_id_t* p_id, reg_type_t* p_type, size_t* p_size, void* p_data, size_t data_size_max)
{
    size_t id_getted = 0;
    size_t type_getted = 0;
    size_t size_getted = 0;
    size_t data_getted = 0;
    size_t data_size = 0;

    uint32_t id;
    uint8_t type;
    uint8_t size;

    size_t index = *m_index;

    if(index + sizeof(id) > index_max) return -1;
    id_getted = buf_get_reg(data_from, index, index_max, &id, sizeof(id));
    if(id_getted != sizeof(id)) return -1;

    index += id_getted;

    if(index + sizeof(type) > index_max) return -1;
    type_getted = buf_get_reg(data_from, index, index_max, &type, sizeof(type));
    if(type_getted != sizeof(type)) return -1;

    index += type_getted;

    if(index + sizeof(size) > index_max) return -1;
    size_getted = buf_get_reg(data_from, index, index_max, &size, sizeof(size));
    if(size_getted != sizeof(size)) return -1;

    index += size_getted;

    if(id != SETTINGS_END_ID || type != SETTINGS_END_TYPE || size != SETTINGS_END_DATA_SIZE){

        data_size = (size_t)size;

        if(index + data_size > index_max) return -1;

        if(data_size <= data_size_max){
            data_getted = buf_get_reg(data_from, index, index_max, p_data, data_size);
            if(data_getted != data_size) return -1;
        }

        index += data_size;

        if(p_size) *p_size = data_getted;
    }else{
        if(p_size) *p_size = (size_t)size;
    }

    if(p_type) *p_type = (reg_type_t)type;
    if(p_id) *p_id = id;

    *m_index = index;

    return (int)(sizeof(id) + sizeof(type) + sizeof(size) + data_size);
}

modbus_rtu_error_t modbus_panel_reg_read(const void* rx_data, size_t rx_size, void* tx_data, size_t* tx_size) {

	memcpy(&panel_request, rx_data, sizeof(panel_request));

	size_t index = 0;

	reg_t* reg = regs_find(panel_request.id);

	if(reg == NULL) return MODBUS_RTU_ERROR_FUNC;

	int size = buf_put_reg_atomic(tx_data, &index, MODBUS_RTU_DATA_SIZE_MAX, reg);

	if(size <= 0) return MODBUS_RTU_ERROR_FUNC;

	*tx_size = size;

	return MODBUS_RTU_ERROR_NONE;
}

modbus_rtu_error_t modbus_panel_reg_write(const void* rx_data, size_t rx_size, void* tx_data, size_t* tx_size) {
	return MODBUS_RTU_ERROR_NONE;
}

static modbus_rtu_error_t modbus_panel_custom_function_callback(modbus_rtu_func_t func, const void* rx_data, size_t rx_size, void* tx_data, size_t* tx_size) {
	switch(func) {
	case MODBUS_RTU_CUSTOM_FUNC_REG_READ:
		return modbus_panel_reg_read(rx_data, rx_size, tx_data, tx_size);
	case MODBUS_RTU_CUSTOM_FUNC_REG_WRITE:
		return modbus_panel_reg_write(rx_data, rx_size, tx_data, tx_size);
	default:
		return MODBUS_RTU_ERROR_FUNC;
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
    modbus_rtu_set_read_holding_reg_callback(&modbus_panel, modbus_panel_on_read_hold_reg);
    modbus_rtu_set_write_holding_reg_callback(&modbus_panel, modbus_panel_on_write_hold_reg);
    modbus_rtu_set_custom_function_callback(&modbus_panel, modbus_panel_custom_function_callback);
}
