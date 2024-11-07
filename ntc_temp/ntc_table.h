/*
 * b57891s0103f008.h
 *
 *  Created on: 15 дек. 2022 г.
 *      Author: Ruslan
 */

#ifndef INC_NTC_TABLE_H_
#define INC_NTC_TABLE_H_

#include "iqmath/iq_types.h"

#define ntc_4901_data_shift 2
#define ntc_4901_index_mask 0xFF
#define ntc_4901_table_size 256

extern const iqs_t ntc_4901_table[ntc_4901_table_size];

#endif /* INC_NTC_TABLE_H_ */
