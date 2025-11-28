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
#include "reg/buf_reg.h"

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
	MODBUS_RTU_CUSTOM_FUNC_REGS_READ = 0x64,
	MODBUS_RTU_CUSTOM_FUNC_REGS_WRITE,
	MODBUS_RTU_CUSTOM_FUNC_REG_READ,
	MODBUS_RTU_CUSTOM_FUNC_REG_WRITE
};

//modbus to local regs
#pragma pack(push, 1)
struct {
	reg_id_t id;
	uint8_t count;
} modbus_regs_request;
#pragma pack(pop)

#pragma pack(push, 1)
struct {
	reg_id_t id;
	uint8_t count;
} modbus_regs_response;
#pragma pack(pop)

//TODO: причесать этот колхоз
modbus_rtu_error_t modbus_panel_regs_read(const void* rx_data, size_t rx_size, void* tx_data, size_t* tx_size) {

	//прочитаем заголовок
	memcpy(&modbus_regs_request, rx_data, sizeof(modbus_regs_request));

	//заполним заголовок
	modbus_regs_response.id = modbus_regs_request.id;
	modbus_regs_response.count = modbus_regs_request.count;

	//скопируем заголовок
	memcpy(tx_data, &modbus_regs_response, sizeof(modbus_regs_response));

	//получим смещение заголовка
	size_t index = sizeof(modbus_regs_response);
	reg_t* reg_ptr = regs_find(modbus_regs_request.id);
	int reg_putted = 0;

	size_t response_data_size = index;
	for(int reg_count = 0; reg_count < modbus_regs_request.count; reg_count++) {
		if(reg_ptr == NULL) return MODBUS_RTU_ERROR_FUNC;
		reg_putted = buf_put_reg_atomic(tx_data, &index, MODBUS_RTU_DATA_SIZE_MAX, reg_ptr);
		if(reg_putted < 0) return MODBUS_RTU_ERROR_FUNC;
		response_data_size += reg_putted;
		reg_ptr = regs_next(reg_ptr);
	}

	*tx_size = response_data_size;

	return MODBUS_RTU_ERROR_NONE;
}

//TODO: причесать этот колхоз
modbus_rtu_error_t modbus_panel_regs_write(const void* rx_data, size_t rx_size, void* tx_data, size_t* tx_size) {

	//прочтем заголовок
	memcpy(&modbus_regs_request, rx_data, sizeof(modbus_regs_request));

	//заполним заголовок
	modbus_regs_response.id = modbus_regs_request.id;
	modbus_regs_response.count = modbus_regs_request.count;

	//скопируем заголовок
	memcpy(tx_data, &modbus_regs_response, sizeof(modbus_regs_response));

	//получим смещение заголовка
	size_t index =  sizeof(modbus_regs_request);
	reg_t* reg_ptr = NULL;
	int reg_getted = 0;

	//буфер принятых данных
	reg_id_t p_id = 0;
	reg_type_t p_type = 0;
	size_t p_size = 0;
	uint8_t p_data[4];

	for(int reg_count = 0; reg_count < modbus_regs_request.count; reg_count++) {
		//прочитаем данные
		reg_getted = buf_get_reg_atomic(rx_data, &index, MODBUS_RTU_DATA_SIZE_MAX, &p_id, &p_type, &p_size, p_data, 4);
		if(reg_getted < 0) return MODBUS_RTU_ERROR_FUNC;

		reg_ptr = regs_find(p_id);
		if(reg_ptr == NULL) return MODBUS_RTU_ERROR_FUNC;

		//сравним на соответствие запросу
		if ((reg_ptr->type == p_type) && (reg_data_size(reg_ptr) == p_size)) {
			memcpy(reg_ptr->data, p_data, p_size);
		} else {
			return MODBUS_RTU_ERROR_FUNC;
		}
	}

	//укажем размер ответа
	*tx_size = sizeof(modbus_regs_response);

	return MODBUS_RTU_ERROR_NONE;
}

//modbus to local regs or remote via canopen
typedef struct {
    // Базовые поля.
    status_t status;
    // Регистры.
    uint8_t dev_id;
    reg_id_t reg_id;
    size_t reg_size;
    iql_t reg_data;
} modbus_to_can_t;

static modbus_to_can_t modbus_to_can;

static CO_SDO_CLI_Queue* modbus_to_can_read_queue = NULL;
static CO_SDO_CLI_Queue* modbus_to_can_write_queue = NULL;

#pragma pack(push, 1)
struct {
	uint8_t dev_id;
	reg_id_t reg_id;
	size_t reg_size;
} modbus_reg_request;
#pragma pack(pop)

#pragma pack(push, 1)
struct {
	uint8_t dev_id;
	reg_id_t reg_id;
	iql_t reg_data;
	status_t status;
} modbus_reg_response;
#pragma pack(pop)

static void modbus_to_can_read(modbus_to_can_t* ptr) {
	//если очередь не задана
    if(modbus_to_can_read_queue == NULL) {
       	//установим статус RUN
        ptr->status |= MODBUS_TO_CAN_STATUS_RUN;
    	//сбросим статусы VALID, ERROR, WARNING, READ_DONE
    	ptr->status &= ~(MODBUS_TO_CAN_STATUS_VALID |
    			MODBUS_TO_CAN_STATUS_ERROR |
				MODBUS_TO_CAN_STATUS_WARNING |
				MODBUS_TO_CAN_STATUS_READ_DONE);
    	//добавим в очередь задание
    	modbus_to_can_read_queue = CO_SDO_CLI_read(
    			&can1_cli_driver,
				CAN_BUS_DATA_ID_FROM_ID(ptr->reg_id),
				CAN_BUS_DATA_INDEX_FROM_ID(ptr->reg_id),
				CAN_BUS_DATA_SUB_INDEX_FROM_ID(ptr->reg_id),
				&ptr->reg_data,
				ptr->reg_size, 200); //200ms timeout
    } else {
    	//если задание выполнено
		if (modbus_to_can_read_queue->m_state == CO_SDO_CLI_State_DONE) {
			//проверим статус задания
			if (modbus_to_can_read_queue->m_error == CO_SDO_CLI_Error_NONE) {
				//установим статусы VALID, WRITE_DONE
				ptr->status |= (MODBUS_TO_CAN_STATUS_VALID | MODBUS_TO_CAN_STATUS_READ_DONE);
			} else {
				//установим статусы ERROR, WRITE_DONE
				ptr->status |= (MODBUS_TO_CAN_STATUS_ERROR | MODBUS_TO_CAN_STATUS_READ_DONE);
			}
			//сбросим статус RUN
			ptr->status &= ~MODBUS_TO_CAN_STATUS_RUN;
			//сбросим указатель на очередь
			modbus_to_can_read_queue = NULL;
		}
    }
}

static void modbus_to_can_write(modbus_to_can_t* ptr) {
	//если очередь не задана
    if(modbus_to_can_write_queue == NULL) {
       	//установим статус RUN
        ptr->status |= MODBUS_TO_CAN_STATUS_RUN;
    	//сбросим статусы VALID, ERROR, WARNING, WRITE_DONE
    	ptr->status &= ~(MODBUS_TO_CAN_STATUS_VALID |
    			MODBUS_TO_CAN_STATUS_ERROR |
				MODBUS_TO_CAN_STATUS_WARNING |
				MODBUS_TO_CAN_STATUS_WRITE_DONE);
    	//добавим в очередь задание
    	modbus_to_can_write_queue = CO_SDO_CLI_write(
    			&can1_cli_driver,
				CAN_BUS_DATA_ID_FROM_ID(ptr->reg_id),
				CAN_BUS_DATA_INDEX_FROM_ID(ptr->reg_id),
				CAN_BUS_DATA_SUB_INDEX_FROM_ID(ptr->reg_id),
				&ptr->reg_data,
				ptr->reg_size, 200); //200ms timeout
    } else {
    	//если задание выполнено
		if (modbus_to_can_write_queue->m_state == CO_SDO_CLI_State_DONE) {
			//проверим статус задания
			if (modbus_to_can_write_queue->m_error == CO_SDO_CLI_Error_NONE) {
				//установим статусы VALID, WRITE_DONE
				ptr->status |= (MODBUS_TO_CAN_STATUS_VALID | MODBUS_TO_CAN_STATUS_WRITE_DONE);
			} else {
				//установим статусы ERROR, WRITE_DONE
				ptr->status |= (MODBUS_TO_CAN_STATUS_ERROR | MODBUS_TO_CAN_STATUS_WRITE_DONE);
			}
			//сбросим статус RUN
			ptr->status &= ~MODBUS_TO_CAN_STATUS_RUN;
			//сбросим указатель на очередь
			modbus_to_can_write_queue = NULL;
		}
    }
}

modbus_rtu_error_t modbus_panel_reg_read(const void* rx_data, size_t rx_size, void* tx_data, size_t* tx_size) {
	//прочитаем заголовок
	memcpy(&modbus_reg_request, rx_data, sizeof(modbus_reg_request));

	//обработчик чтения
	modbus_to_can_read(&modbus_to_can);

	return MODBUS_RTU_ERROR_NONE;
}

modbus_rtu_error_t modbus_panel_reg_write(const void* rx_data, size_t rx_size, void* tx_data, size_t* tx_size) {
	//прочитаем заголовок
	memcpy(&modbus_reg_request, rx_data, sizeof(modbus_reg_request));

	//обработчик записи
	modbus_to_can_write(&modbus_to_can);

	return MODBUS_RTU_ERROR_NONE;
}

static modbus_rtu_error_t modbus_panel_custom_function_callback(modbus_rtu_func_t func, const void* rx_data, size_t rx_size, void* tx_data, size_t* tx_size) {
	switch(func) {
	case MODBUS_RTU_CUSTOM_FUNC_REGS_READ:
		return modbus_panel_regs_read(rx_data, rx_size, tx_data, tx_size);
	case MODBUS_RTU_CUSTOM_FUNC_REGS_WRITE:
		return modbus_panel_regs_write(rx_data, rx_size, tx_data, tx_size);
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
