#include "gpio/init/gpio_init.h"
#include "digital_output.h"
#include "modules/modules.h"


METHOD_INIT_IMPL(M_digital_output, output)
{
}

METHOD_DEINIT_IMPL(M_digital_output, output)
{
}

METHOD_CALC_IMPL(M_digital_output, output)
{
	uint32_t raw_mask;
	uint32_t in_mask;
	uint32_t invert_res;
	for(int i = 0; i < DIGITAL_INPUT_COUNT; i++) {
		raw_mask = (1 << output->select[i]);
		in_mask = (1 << i);
		invert_res = (in_mask & output->invert);
		if(output->in & raw_mask) {
			if(invert_res) {
				output->raw.all &= ~in_mask;
			} else {
				output->raw.all |= in_mask;
			}
		} else {
			if(invert_res) {
				output->raw.all |= in_mask;
			} else {
				output->raw.all &= ~in_mask;
			}
		}
	}

	do_ncv7608.in = output->raw.bit.ncv;

	do_relay.in = output->raw.bit.relay;
}
