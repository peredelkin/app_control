/*
 * b57891s0103f008.h
 *
 *  Created on: 15 дек. 2022 г.
 *      Author: Ruslan
 */

#ifndef INC_NTC_TABLE_H_
#define INC_NTC_TABLE_H_

#define NTC_4901_TABLE_SIZE 43

typedef struct {
	float temp;
	float kohm;
} ntc_point_t;

extern const ntc_point_t ntc_4901_table[NTC_4901_TABLE_SIZE];

#endif /* INC_NTC_TABLE_H_ */
