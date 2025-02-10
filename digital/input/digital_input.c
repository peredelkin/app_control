#include "gpio/init/gpio_init.h"
#include "digital_input.h"
#include "modules/modules.h"

METHOD_INIT_IMPL(M_digital_input, input)
{
	gpio_em_stop_cfg_setup(); //EmStop_App
	gpio_ac_dc_lost_cfg_setup(); //AC DC Lost

	//TODO: тестовые настройки!
	input->p_select[0] = 2;
	input->p_select[1] = 3;
	input->p_select[2] = 4;
	input->p_select[3] = 5;
	input->p_select[4] = 6;
	input->p_select[5] = 7;
	input->p_select[6] = 8;
	input->p_select[7] = 9;
	input->p_select[8] = 10;
	input->p_select[9] = 11;
}

METHOD_DEINIT_IMPL(M_digital_input, input)
{
}

METHOD_CALC_IMPL(M_digital_input, input)
{
	input->m_in_data.bit.const_0 = 0;

	input->m_in_data.bit.const_1 = 1;

	input->m_in_data.bit.msdi = msdi.out_digital;

	input->m_in_data.bit.em_stop = gpio_input_bit_read(&GPI_EmStop_App);

	input->m_in_data.bit.ac_lost = gpio_input_bit_read(&GPI_AC_lost_App);

	input->m_in_data.bit.dc_lost = gpio_input_bit_read(&GPI_Lost_5V_App);

	input->m_in_data.bit.panel = gpio_input_bit_read(&gpio_rs485_panel_detect);

	uint32_t raw_mask;
	uint32_t out_mask;
	uint32_t invert_res;
	for(int i = 0; i < DIGITAL_INPUT_COUNT; i++) {
		raw_mask = (1 << input->p_select[i]);
		out_mask = (1 << i);
		invert_res = (out_mask & input->p_invert);
		if(input->m_in_data.all & raw_mask) {
			if(invert_res) {
				input->out_data &= ~out_mask;
			} else {
				input->out_data |= out_mask;
			}
		} else {
			if(invert_res) {
				input->out_data |= out_mask;
			} else {
				input->out_data &= ~out_mask;
			}
		}
	}
}
