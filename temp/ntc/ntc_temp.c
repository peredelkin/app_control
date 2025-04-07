#include <stdint.h>
#include <stdlib.h>
#include "ntc_table.h"
#include "ntc_temp.h"
#include "lib/iqmath/iqmath.h"
#include "modules/modules.h"

METHOD_INIT_IMPL(M_ntc_temp, ntc_temp)
{
	ntc_temp->m_R_in_max = (iq15_t)(ntc_4901_table[0].kohm * IQ15_BASE);
	ntc_temp->m_R_in_min = (iq15_t)(ntc_4901_table[NTC_4901_TABLE_SIZE - 2].kohm * IQ15_BASE);
	ntc_temp->m_R_ref = (iq15_t)(3.3f * IQ15_BASE);
}

METHOD_DEINIT_IMPL(M_ntc_temp, ntc_temp)
{

}

int bsearch_ohm_comparator(const void* ptr1, const void* ptr2) {
	float R_in = *((float*)ptr1);
	//a > b
	float R_a = ((ntc_point_t*)ptr2)[0].kohm;
	float R_b = ((ntc_point_t*)ptr2)[1].kohm;
	if(R_in > R_a) return -1;
	if(R_in < R_b) return 1;
	return 0;
}

uint32_t ntc_counter;

METHOD_CALC_IMPL(M_ntc_temp, ntc_temp) {
	iq15_t vref = (iq15_t)(msdi.out_analog[6] * IQN_BASE(4, int32_t));
	iq15_t vin = (iq15_t)(msdi.out_analog[ntc_counter] * IQN_BASE(4, int32_t));
	iq15_t R_ref_voltage = vref - vin;

	if(R_ref_voltage == 0) R_ref_voltage = 1;

	ntc_temp->out_ohm[ntc_counter] = (ntc_temp->m_R_ref * vin)/R_ref_voltage;

	iq15_t R_in = ntc_temp->out_ohm[ntc_counter];

	if(R_in > ntc_temp->m_R_in_max) {
		R_in = ntc_temp->m_R_in_max;
	}

	if(R_in < ntc_temp->m_R_in_min) {
		R_in = ntc_temp->m_R_in_min;
	}

	float float_R_in = (float)(R_in)/(float)(IQ15_BASE);

	ntc_point_t* ntc_ptr = bsearch(
			&float_R_in,
			ntc_4901_table,
			NTC_4901_TABLE_SIZE,
			sizeof(ntc_point_t),
			&bsearch_ohm_comparator);

	if (ntc_ptr != NULL) {
		float R_a = ntc_ptr[0].kohm;
		float Temp_a = ntc_ptr[0].temp;

		float R_b = ntc_ptr[1].kohm;
		float Temp_b = ntc_ptr[1].temp;

		float Temp = Temp_a + ((Temp_b - Temp_a) * (R_a - float_R_in)) / (R_a - R_b);

		ntc_temp->out_temp[ntc_counter] = (iq15_t) (Temp * IQ15_BASE);
	} else {
		ntc_temp->out_temp[ntc_counter] = IQ15_MAX;
	}

	if (ntc_counter >= (NTC_TEMP_COUNT - 1)) {
		ntc_counter = 0;
	} else {
		ntc_counter++;
	}
}
