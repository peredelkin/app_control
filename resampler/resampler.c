/*
 * resampler.c
 *
 *  Created on: 2 июл. 2026 г.
 *      Author: ruslan
 */

#include "resampler.h"

/*
 * Функция вызывается при каждом получении нового семпла
 */
void resampler_calc(resampler_t* res) {
	for(int l = 0; l < RESAMPLER_L; l++) {
		for(int ch = 0; ch < RESAMPLER_COUNT; ch++) {
			res->accum[ch] -= res->buf[res->sample_in][ch];
			res->buf[res->sample_in][ch] = res->in[ch];
			res->accum[ch] += res->buf[res->sample_in][ch];
		}

		if(res->sample_in >= RESAMPLER_DEPTH) {
			res->sample_in = 0;
		} else {
			res->sample_in++;
		}

		if(res->sample_out >= RESAMPLER_M) {
			for(int ch = 0; ch < RESAMPLER_COUNT; ch++) {
				res->out[ch] = res->accum[ch]/RESAMPLER_DEPTH;
			}
			if(res->callback) res->callback(res);
			res->sample_out = 0;
		} else {
			res->sample_out++;
		}

	}
}
