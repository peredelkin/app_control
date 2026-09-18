#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "lib/utils/utils.h"

#include "mso.h"


METHOD_INIT_IMPL(M_mso, mso)
{
	mso->m_ptr = malloc(MSO_DATA_SIZE);

	mso->r_channel[0].id = REG_ID_NTC_TEMP_OUT_TEMP0;
	mso->r_channel[0].enabled = true;

	mso->r_channel[1].id = REG_ID_NTC_TEMP_OUT_TEMP1;
	mso->r_channel[1].enabled = true;

	mso->r_channel[2].id = REG_ID_NTC_TEMP_OUT_TEMP2;
	mso->r_channel[2].enabled = false;

	mso->r_channel[3].id = REG_ID_MSDI_OUT_REF;
	mso->r_channel[3].enabled = true;

	mso->r_channel[4].id = REG_ID_MSDI_OUT_VCC;
	mso->r_channel[4].enabled = true;
}

METHOD_DEINIT_IMPL(M_mso, mso)
{
	free(mso->m_ptr);
}

static int ch_data_index;

METHOD_CALC_IMPL(M_mso, mso) {
	if(mso->status & MSO_STATUS_RUN) {
		if (mso->status & MSO_STATUS_READY) {
			for (int ch = 0; ch < MSO_MAX_CHANNEL_COUNT; ch++) {
				if (mso->r_channel[ch].enabled && mso->r_channel[ch].reg
						&& mso->r_channel[ch].ptr) {
					if (mso->r_channel[ch].reg->data) {
						mso->r_channel[ch].ptr[ch_data_index] = reg_valuel(mso->r_channel[ch].reg);
					}
				}
			}

			if (ch_data_index >= mso->m_ch_data_count) {
				ch_data_index = 0;
				mso->status |= MSO_STATUS_VALID;
				mso->status &= ~MSO_STATUS_RUN;
			} else {
				ch_data_index++;
			}
		} else {
			ch_data_index = 0;
		}
	} else {
		if(mso->control & MSO_CONTROL_START) {
			mso->status = MSO_STATUS_NONE;
			mso->status |= MSO_STATUS_RUN;
			mso->control &= ~MSO_CONTROL_START;
		}
	}
}

void mso_channel_reg_set(M_mso* mso, int ch, reg_t* reg) {
	__M_LOCK(mso);
	mso->status &= ~MSO_STATUS_VALID;
	mso->status &= ~MSO_STATUS_READY;
	mso->r_channel[ch].reg = reg;
	__M_UNLOCK(mso);
}

METHOD_IDLE_IMPL(M_mso, mso)
{
	if(mso->status & MSO_STATUS_RUN) {
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

		__M_LOCK(mso);
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
		mso->status |= MSO_STATUS_READY;
		__M_UNLOCK(mso);
	}
}
