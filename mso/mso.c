#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "mso.h"


METHOD_INIT_IMPL(M_mso, mso)
{
	mso->ptr = malloc(MSO_DATA_SIZE);

	printf("MSO ptr: %u\n", (unsigned int)(mso->ptr));

	mso->channel[0].id = REG_ID_TEMP_NTC_0;
	mso->channel[0].enabled = true;

	mso->channel[1].id = REG_ID_TEMP_NTC_1;
	mso->channel[1].enabled = true;

	mso->channel[2].id = REG_ID_TEMP_NTC_2;
	mso->channel[2].enabled = true;

	mso->channel[3].id = REG_ID_VOLTAGE_NTC_REF;
	mso->channel[3].enabled = true;

	mso->channel[4].id = REG_ID_VOLTAGE_MSDI_VCC;
	mso->channel[4].enabled = true;
}

METHOD_DEINIT_IMPL(M_mso, mso)
{
	free(mso->ptr);
}

static int ch_data_index;

METHOD_CALC_IMPL(M_mso, mso) {
	for(int i = 0; i < MSO_MAX_CHANNEL_COUNT; i++) {
		if(mso->channel[i].enabled && mso->channel[i].reg && mso->channel[i].ptr) {
			if(mso->channel[i].reg->data) {
				mso->channel[i].ptr[ch_data_index] = reg_valuel(mso->channel[i].reg);
			}
		}
	}

	if(ch_data_index >= mso->ch_data_count) {
		ch_data_index = 0;
	} else {
		ch_data_index++;
	}
}

void mso_channel_reg_set(M_mso* mso, int ch, reg_t* reg) {
	__disable_irq();
	mso->channel[ch].reg = reg;
	__enable_irq();
}

METHOD_IDLE_IMPL(M_mso, mso)
{
	reg_t* reg = NULL;
	int ch_count = 0;
	int ch_data_count = 0;
	mso_data_t *ptr = mso->ptr;

	for(int i = 0; i < MSO_MAX_CHANNEL_COUNT; i++) {
		if(mso->channel[i].enabled) {
			ch_count++;
			if(mso->channel[i].id_old != mso->channel[i].id) {
				mso->channel[i].id_old = mso->channel[i].id;
				reg = regs_find(mso->channel[i].id);
				mso_channel_reg_set(mso, i, reg);
			}
		}
	}

	ch_data_count = MSO_DATA_COUNT/ch_count;

	__disable_irq();
	for(int i = 0; i < MSO_MAX_CHANNEL_COUNT; i++) {
		if(mso->channel[i].enabled) {
			mso->channel[i].ptr = ptr;
			ptr += ch_data_count;
		} else {
			mso->channel[i].reg = NULL;
			mso->channel[i].ptr = NULL;
		}
	}

	mso->ch_count = ch_count;
	mso->ch_data_count = ch_data_count;
	__enable_irq();
}
