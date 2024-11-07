#include "digital_input.h"

#define DIGITAL_INPUT_MSDI_IN_COUNT 10
#define DIGITAL_INPUT_1 8
#define DIGITAL_INPUT_2 9
#define DIGITAL_INPUT_3 10
#define DIGITAL_INPUT_4 11
#define DIGITAL_INPUT_5 12
#define DIGITAL_INPUT_6 13
#define DIGITAL_INPUT_7 14
#define DIGITAL_INPUT_8 15
#define DIGITAL_INPUT_9 19
#define DIGITAL_INPUT_10 20

static const uint32_t digital_input_msdi_in_mask [DIGITAL_INPUT_MSDI_IN_COUNT] = {
		(1 << DIGITAL_INPUT_1),
		(1 << DIGITAL_INPUT_2),
		(1 << DIGITAL_INPUT_3),
		(1 << DIGITAL_INPUT_4),
		(1 << DIGITAL_INPUT_5),
		(1 << DIGITAL_INPUT_6),
		(1 << DIGITAL_INPUT_7),
		(1 << DIGITAL_INPUT_8),
		(1 << DIGITAL_INPUT_9),
		(1 << DIGITAL_INPUT_10)
};

METHOD_INIT_IMPL(M_digital_input, input)
{
}

METHOD_DEINIT_IMPL(M_digital_input, input)
{
}

METHOD_CALC_IMPL(M_digital_input, input)
{
	for (int n = 0; n < DIGITAL_INPUT_MSDI_IN_COUNT; n++) {
		if((input->msdi_in & digital_input_msdi_in_mask[n]) == digital_input_msdi_in_mask[n]) {
			input->out |= (1 << n);
		} else {
			input->out &= ~(1 << n);
		}
	}
}
