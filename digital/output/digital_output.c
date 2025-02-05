#include "gpio/init/gpio_init.h"
#include "digital_output.h"
#include "modules/modules.h"


METHOD_INIT_IMPL(M_digital_output, output)
{
    INIT(do_ncv7608);
    INIT(do_relay);
}

METHOD_DEINIT_IMPL(M_digital_output, output)
{
    DEINIT(do_ncv7608);
    DEINIT(do_relay);
}

METHOD_CALC_IMPL(M_digital_output, output)
{
	uint32_t raw_mask;
	uint32_t in_mask;
	uint32_t invert_res;
	for(int i = 0; i < DIGITAL_INPUT_COUNT; i++) {
		raw_mask = (1 << output->p_select[i]);
		in_mask = (1 << i);
		invert_res = (in_mask & output->p_invert);
		if(output->in_data & raw_mask) {
			if(invert_res) {
				output->m_out_data.all &= ~in_mask;
			} else {
				output->m_out_data.all |= in_mask;
			}
		} else {
			if(invert_res) {
				output->m_out_data.all |= in_mask;
			} else {
				output->m_out_data.all &= ~in_mask;
			}
		}
	}

	do_ncv7608.in_data = output->m_out_data.bit.ncv;

	do_relay.in_data = output->m_out_data.bit.relay;

	CALC(do_ncv7608);
	CALC(do_relay);
}
