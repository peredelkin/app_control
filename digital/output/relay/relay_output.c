#include "relay_output.h"
#include "gpio/init/gpio_init.h"

METHOD_INIT_IMPL(M_relay_output, relay)
{
	gpio_relay_output_cfg_setup(); //Relay output
}

METHOD_DEINIT_IMPL(M_relay_output, relay)
{

}

METHOD_CALC_IMPL(M_relay_output, relay)
{
	relay->m_out_data.all = relay->in_data;

	gpio_output_bit_setup(&GPO_Relay_DO_App[0], relay->m_out_data.bit.relay_1);
	gpio_output_bit_setup(&GPO_Relay_DO_App[1], relay->m_out_data.bit.relay_2);
	gpio_output_bit_setup(&GPO_Relay_DO_App[2], relay->m_out_data.bit.relay_3);
	gpio_output_bit_setup(&GPO_Relay_DO_App[3], relay->m_out_data.bit.relay_4);
}
