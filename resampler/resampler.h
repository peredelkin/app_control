/*
 * resampler.h
 *
 *  Created on: 2 июл. 2026 г.
 *      Author: ruslan
 */

#ifndef RESAMPLER_RESAMPLER_H_
#define RESAMPLER_RESAMPLER_H_

#include <stdint.h>

#define RESAMPLER_L 5
#define RESAMPLER_M 18
#define RESAMPLER_COUNT 16
#define RESAMPLER_DEPTH 16

typedef struct _resampler_t resampler_t;

typedef void (*resampler_callback_t)(void*);

struct _resampler_t{
	uint32_t sample_in;
	uint32_t sample_out;
	int32_t in[RESAMPLER_COUNT] __attribute__((aligned(4)));
	int32_t buf[RESAMPLER_DEPTH][RESAMPLER_COUNT];
	int64_t accum[RESAMPLER_COUNT];
	int32_t out[RESAMPLER_COUNT] __attribute__((aligned(4)));
	resampler_callback_t callback;
};

extern void resampler_calc(resampler_t* res);

#endif /* RESAMPLER_RESAMPLER_H_ */
