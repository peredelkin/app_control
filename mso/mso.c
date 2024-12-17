#include <malloc.h>
#include <string.h>

#include "mso.h"


METHOD_INIT_IMPL(M_mso, mso)
{
	mso->ptr = malloc(MSO_DATA_SIZE);
}

METHOD_DEINIT_IMPL(M_mso, mso)
{
}

uint32_t mso_write_index;

METHOD_CALC_IMPL(M_mso, mso) {
	if (mso->ptr != NULL) {
		for (int ch = 0; ch < MSO_MAX_CHANNEL_COUNT; ch++) {
			if (mso->channel[ch].enabled) {
				if (mso->channel[ch].reg != NULL) {
					if (mso->channel[ch].reg->id == mso->channel[ch].id) {
						if (mso->channel[ch].reg->data != NULL) {
							memcpy(&(mso->ptr[mso_write_index]), mso->channel[ch].reg->data, sizeof(mso_data_t));
						}
					}
				}

				if (mso_write_index >= MSO_WORDS_COUNT - 1) {
					mso_write_index = 0;
				} else {
					mso_write_index++;
				}
			}
		}

	}
}

METHOD_IDLE_IMPL(M_mso, mso)
{
	for(int i = 0; i < MSO_MAX_CHANNEL_COUNT; i++) {
		if(mso->channel[i].enabled) { //канал включен
			if(mso->channel[i].reg != NULL) { //регистр найден
				if(mso->channel[i].reg->id != mso->channel[i].id) {//id канала не равен id регистра
					mso->channel[i].reg = regs_find(mso->channel[i].id);
				}
			} else {
				mso->channel[i].reg = regs_find(mso->channel[i].id);
			}
		}
	}
}
