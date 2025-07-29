/*
 * buf_reg.c
 *
 *  Created on: 29 июл. 2025 г.
 *      Author: ruslan
 */

#include <stddef.h>
#include "reg.h"
#include "reg_types.h"

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
