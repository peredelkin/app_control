#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "mso.h"


METHOD_INIT_IMPL(M_mso, mso)
{
	mso->m_ptr = malloc(MSO_DATA_SIZE);

	mso->r_channel[0].id = REG_ID_TEMP_NTC_0;
	mso->r_channel[0].enabled = true;

	mso->r_channel[1].id = REG_ID_TEMP_NTC_1;
	mso->r_channel[1].enabled = true;

	mso->r_channel[2].id = REG_ID_TEMP_NTC_2;
	mso->r_channel[2].enabled = true;

	mso->r_channel[3].id = REG_ID_VOLTAGE_NTC_VREF;
	mso->r_channel[3].enabled = true;

	mso->r_channel[4].id = REG_ID_VOLTAGE_MSDI_VCC;
	mso->r_channel[4].enabled = true;
}

METHOD_DEINIT_IMPL(M_mso, mso)
{
	free(mso->m_ptr);
}

static int ch_data_index;

METHOD_CALC_IMPL(M_mso, mso) {
	for(int ch = 0; ch < MSO_MAX_CHANNEL_COUNT; ch++) {
		if(mso->r_channel[ch].enabled && mso->r_channel[ch].reg && mso->r_channel[ch].ptr) {
			if(mso->r_channel[ch].reg->data) {
				mso->r_channel[ch].ptr[ch_data_index] = reg_valuel(mso->r_channel[ch].reg);
			}
		}
	}

	if(ch_data_index >= mso->m_ch_data_count) {
		ch_data_index = 0;
	} else {
		ch_data_index++;
	}
}

void mso_channel_reg_set(M_mso* mso, int ch, reg_t* reg) {
	__disable_irq();
	mso->r_channel[ch].reg = reg;
	__enable_irq();
}

METHOD_IDLE_IMPL(M_mso, mso)
{
	reg_t* reg = NULL;
	int ch_count = 0;
	int ch_data_count = 0;
	mso_data_t *ptr = mso->m_ptr;

	for(int ch = 0; ch < MSO_MAX_CHANNEL_COUNT; ch++) {
		if(mso->r_channel[ch].enabled) {
			ch_count++;
			if(mso->r_channel[ch].id_old != mso->r_channel[ch].id) {
				mso->r_channel[ch].id_old = mso->r_channel[ch].id;
				reg = regs_find(mso->r_channel[ch].id);
				mso_channel_reg_set(mso, ch, reg);
			}
		}
	}

	ch_data_count = MSO_DATA_COUNT/ch_count;

	__disable_irq();
	for(int ch = 0; ch < MSO_MAX_CHANNEL_COUNT; ch++) {
		if(mso->r_channel[ch].enabled) {
			mso->r_channel[ch].ptr = ptr;
			ptr += ch_data_count;
		} else {
			mso->r_channel[ch].reg = NULL;
			mso->r_channel[ch].ptr = NULL;
		}
	}

	mso->m_ch_count = ch_count;
	mso->m_ch_data_count = ch_data_count;
	__enable_irq();
}
