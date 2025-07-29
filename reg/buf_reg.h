/*
 * buf_reg.h
 *
 *  Created on: 29 июл. 2025 г.
 *      Author: ruslan
 */

#ifndef REG_BUF_REG_H_
#define REG_BUF_REG_H_

extern int buf_put_reg_atomic(void* data_to, size_t* m_index, size_t index_max, reg_t* reg);
extern int buf_get_reg_atomic(void* data_from, size_t* m_index, size_t index_max, reg_id_t* p_id, reg_type_t* p_type, size_t* p_size, void* p_data, size_t data_size_max);

#endif /* REG_BUF_REG_H_ */
