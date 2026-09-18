#include "temp_wind_comp.h"
#include "modules/modules.h"

METHOD_INIT_IMPL(M_temp_wind_comp, wind_comp)
{
}

METHOD_DEINIT_IMPL(M_temp_wind_comp, wind_comp)
{
}

METHOD_CALC_IMPL(M_temp_wind_comp, wind_comp)
{
	uint32_t out_mask;
	for(int i = 0; i < NTC_TEMP_COUNT; i++) {
		out_mask = (1 << i);
		if(ntc_temp.out_ohm[i] < wind_comp->p_low_resist[i]) {
			if(wind_comp->m_low_cnt[i]) {
				wind_comp->m_low_cnt[i]--;
			} else {
				if(wind_comp->p_invert[i]) {
					wind_comp->out_data |= out_mask;
				} else {
					wind_comp->out_data &= ~out_mask;
				}
			}
		} else {
			wind_comp->m_low_cnt[i] = wind_comp->p_low_time[i];
		}

		if(ntc_temp.out_ohm[i] > wind_comp->p_high_resist[i]) {
			if(wind_comp->m_high_cnt[i]) {
				wind_comp->m_high_cnt[i]--;
			} else {
				if(wind_comp->p_invert[i]) {
					wind_comp->out_data &= ~out_mask;
				} else {
					wind_comp->out_data |= out_mask;
				}
			}
		} else {
			wind_comp->m_high_cnt[i] = wind_comp->p_high_time[i];
		}
	}
}
