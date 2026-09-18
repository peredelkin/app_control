#include "panel_led.h"
#include "modules/modules.h"


METHOD_INIT_IMPL(M_panel_led, panel_led)
{
	panel_led->m_out_data.bit.SCR_LED = 1;
}

METHOD_DEINIT_IMPL(M_panel_led, panel_led)
{
}

METHOD_CALC_IMPL(M_panel_led, panel_led)
{
	panel_led->m_out_data.bit.ZS = digital_out.in_data.bit.ready;
	panel_led->m_out_data.bit.Alr = digital_out.in_data.bit.err;
	panel_led->m_out_data.bit.Wrn = digital_out.in_data.bit.warn;
	panel_led->m_out_data.bit.Lim = digital_out.in_data.bit.limit;
	panel_led->m_out_data.bit.En = digital_out.in_data.bit.sw_on;
	panel_led->m_out_data.bit.T_mn = digital_out.in_data.bit.fwd_bwd & digital_out.in_data.bit.run;
	panel_led->m_out_data.bit.T_pl = ~digital_out.in_data.bit.fwd_bwd & digital_out.in_data.bit.run;

	panel_led->out_data = panel_led->m_out_data.all;
}
