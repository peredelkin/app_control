#include "gpio/init/gpio_init.h"
#include "digital_input.h"
#include "modules/modules.h"

METHOD_INIT_IMPL(M_digital_input, input)
{
}

METHOD_DEINIT_IMPL(M_digital_input, input)
{
}

//msdi.out_di

METHOD_CALC_IMPL(M_digital_input, input)
{
	input->raw.bit.msdi = msdi.out_di;

	input->raw.bit.em_stop = gpio_input_bit_read(&GPI_EmStop_App);

	input->raw.bit.ac_lost = gpio_input_bit_read(&GPI_AC_lost_App);

	input->raw.bit.dc_lost = gpio_input_bit_read(&GPI_Lost_5V_App);

	input->raw.bit.panel = gpio_input_bit_read(&gpio_rs485_panel_detect);

	uint32_t raw_mask;
	uint32_t out_mask;
	uint32_t invert_res;
	for(int i = 0; i < DIGITAL_INPUT_COUNT; i++) {
		raw_mask = (1 << input->select[i]);
		out_mask = (1 << i);
		invert_res = (out_mask & input->invert);
		if(input->raw.all & raw_mask) {
			if(invert_res) {
				input->out &= ~out_mask;
			} else {
				input->out |= out_mask;
			}
		} else {
			if(invert_res) {
				input->out |= out_mask;
			} else {
				input->out &= ~out_mask;
			}
		}
	}
}
