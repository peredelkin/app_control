#include "digital_output.h"


METHOD_INIT_IMPL(M_digital_output, dout)
{
}

METHOD_DEINIT_IMPL(M_digital_output, dout)
{
}

METHOD_CALC_IMPL(M_digital_output, dout)
{
	for(int n = 0; n < DIGITAL_OUTPUT_BITS_COUNT; n++) {
		dout->tmp_out_mask = (1 << n);
		if(dout->reg_in[n]) {
			if(dout->reg_in[n]->data) {
				dout->tmp_in = reg_value_u16(dout->reg_in[n]);
				dout->tmp_in_mask = (1 << dout->bit_in[n]);
				if((dout->tmp_in & dout->tmp_in_mask) == dout->tmp_in_mask) {
					//Установить
					dout->tmp_out |= (dout->tmp_out_mask);
				} else {
					//Сбросить
					dout->tmp_out &= ~(dout->tmp_out_mask);
				}
			} else {
				//Сбросить
				dout->tmp_out &= ~(dout->tmp_out_mask);
			}
		} else {
			//Сбросить
			dout->tmp_out &= ~(dout->tmp_out_mask);
		}
	}
	//Инверсия по маске
	dout->out = dout->tmp_out ^ dout->inv;
}
