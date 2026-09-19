#include "panel_led.h"
#include "modules/modules.h"


METHOD_INIT_IMPL(M_panel_led, panel_led)
{
	panel_led->status = PANEL_LED_STATUS_NONE;
	panel_led->control = PANEL_LED_CONTROL_NONE;

	panel_led->m_out_data.bit.SCR_LED = 1;

	panel_led->status = PANEL_LED_STATUS_READY;
}

METHOD_DEINIT_IMPL(M_panel_led, panel_led)
{
}

static void panel_led_control_handler(M_panel_led* panel_led) {
	if(panel_led->control & PANEL_LED_CONTROL_START) {
		panel_led->control &= ~PANEL_LED_CONTROL_START;
		panel_led->status |= PANEL_LED_STATUS_RUN;
	}

	if(panel_led->control & PANEL_LED_CONTROL_STOP) {
		panel_led->control &= ~PANEL_LED_CONTROL_STOP;
		panel_led->status &= ~PANEL_LED_STATUS_RUN;
	}
}

static void panel_led_calc(M_panel_led* panel_led) {
	panel_led->m_out_data.bit.ZS = digital_out.in_data.bit.ready;
	panel_led->m_out_data.bit.Alr = digital_out.in_data.bit.err;
	panel_led->m_out_data.bit.Wrn = digital_out.in_data.bit.warn;
	panel_led->m_out_data.bit.Lim = digital_out.in_data.bit.limit;
	panel_led->m_out_data.bit.En = digital_out.in_data.bit.sw_on;
	panel_led->m_out_data.bit.T_mn = digital_out.in_data.bit.fwd_bwd & digital_out.in_data.bit.run;
	panel_led->m_out_data.bit.T_pl = ~digital_out.in_data.bit.fwd_bwd & digital_out.in_data.bit.run;

	panel_led->out_data = panel_led->m_out_data.all;
}

METHOD_CALC_IMPL(M_panel_led, panel_led)
{
	panel_led_control_handler(panel_led);

	if((panel_led->status & (PANEL_LED_STATUS_READY & PANEL_LED_STATUS_RUN)) ==
			(PANEL_LED_STATUS_READY & PANEL_LED_STATUS_RUN)) {
		panel_led_calc(panel_led);
	} else {
		panel_led->out_data = 0xFFFFFFFF;
	}
}
