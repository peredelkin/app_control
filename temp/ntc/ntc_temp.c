#include "ntc_table.h"
#include "ntc_temp.h"
#include "lib/iqmath/iqmath.h"
#include <stdint.h>

#define NTC_FILTER_SHIFT 2
#define NTC_FILTER_LENGTH (1 << NTC_FILTER_SHIFT)

typedef struct {
	uint16_t data[NTC_TEMP_COUNT][NTC_FILTER_LENGTH];
	uint32_t tmp[NTC_TEMP_COUNT];
	uint16_t out[NTC_TEMP_COUNT];
} ntc_filter1_t;

ntc_filter1_t ntc_filter;
uint32_t ntc_counter;

METHOD_INIT_IMPL(M_ntc_temp, ntc_temp)
{
}

METHOD_DEINIT_IMPL(M_ntc_temp, ntc_temp)
{
}

METHOD_CALC_IMPL(M_ntc_temp, ntc_temp) {
	if (ntc_temp->ai) {
		for (int n = 0; n < NTC_TEMP_COUNT; n++) {
			ntc_filter.tmp[ntc_counter] += ntc_temp->ai[ntc_counter];
			ntc_filter.tmp[ntc_counter] -= ntc_filter.data[ntc_counter][(NTC_FILTER_LENGTH - 1)];
			for (int i = (NTC_FILTER_LENGTH - 2); i >= 0; i--) {
				ntc_filter.data[ntc_counter][i + 1] = ntc_filter.data[ntc_counter][i];
			}
			ntc_filter.data[ntc_counter][0] = ntc_temp->ai[ntc_counter];
			ntc_filter.out[ntc_counter] = (ntc_filter.tmp[ntc_counter] >> NTC_FILTER_SHIFT);
		}
	}

	iq15_t wheit;
	uint32_t index_1, index_2;
	iq15_t temp_1, temp_2;

//	if(ntc_temp->ai) {
//	for (int n = 0; n < NTC_TEMP_COUNT; n++) {
//			ntc_temp->index = ntc_4901_index_mask & (ntc_temp->ai[n] >> ntc_4901_data_shift);
		ntc_temp->index = ntc_4901_index_mask & (ntc_filter.out[ntc_counter] >> ntc_4901_data_shift);

		index_1 = ntc_temp->index;
		index_2 = ntc_temp->index + 1;
		if (index_2 > ntc_4901_table_size - 1)
			index_2 = ntc_4901_table_size - 1;

		temp_1 = IQ15(ntc_4901_table[index_1]);
		temp_2 = IQ15(ntc_4901_table[index_2]);
//			wheit = IQ_N_TO_M(ntc_temp->ai[n], ntc_4901_data_shift, 15);
		wheit = IQ_N_TO_M(ntc_filter.out[ntc_counter], ntc_4901_data_shift, 15);
		wheit = iq15_fract(iq15_abs(wheit));

		ntc_temp->tmp[ntc_counter] += ((iq15_lerp(temp_1, temp_2, wheit) << 4) - (ntc_temp->out_temp[ntc_counter] << 4) >> 4);
		ntc_temp->out_temp[ntc_counter] = ntc_temp->tmp[ntc_counter] >> 4;

//	}
//	}

	if (ntc_counter >= (NTC_TEMP_COUNT-1)) {
		ntc_counter = 0;
	} else {
		ntc_counter++;
	}
}
