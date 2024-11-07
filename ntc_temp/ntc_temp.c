#include "ntc_table.h"
#include "ntc_temp.h"
#include "iqmath/iqmath.h"
#include <stdint.h>


METHOD_INIT_IMPL(M_ntc_temp, ntc_temp)
{
}

METHOD_DEINIT_IMPL(M_ntc_temp, ntc_temp)
{
}

METHOD_CALC_IMPL(M_ntc_temp, ntc_temp)
{
	iq15_t wheit;
	uint32_t index_1, index_2;
	iq15_t temp_1, temp_2;

	if(ntc_temp->ai) {
		for(int n = 0; n < NTC_TEMP_COUNT; n++) {
			ntc_temp->index = ntc_4901_index_mask & (ntc_temp->ai[n] >> ntc_4901_data_shift);

			index_1 = ntc_temp->index;
			index_2 = ntc_temp->index + 1;
			if(index_2 > ntc_4901_table_size - 1) index_2 = ntc_4901_table_size - 1;

			temp_1 = IQ15(ntc_4901_table[index_1]);
			temp_2 = IQ15(ntc_4901_table[index_2]);
			wheit = IQ_N_TO_M(ntc_temp->ai[n], ntc_4901_data_shift, 15);
			wheit = iq15_fract(iq15_abs(wheit));

			ntc_temp->out_temp[n] = iq15_lerp(temp_1, temp_2, wheit);
		}
	}
}
