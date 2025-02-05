#include "panel_led.h"


METHOD_INIT_IMPL(M_panel_led, panel_led)
{
	panel_led->m_out_data.bit.SCR_LED = 1;
}

METHOD_DEINIT_IMPL(M_panel_led, panel_led)
{
}

//дебаг: счетчик для теста светодиодов панельки
uint8_t panel_led_blink;

METHOD_CALC_IMPL(M_panel_led, panel_led)
{
	if(panel_led_blink == 0) {
		panel_led->m_out_data.bit.T_mn = panel_led->m_out_data.bit.T_pl;
		panel_led->m_out_data.bit.T_pl = panel_led->m_out_data.bit.Alr;
		panel_led->m_out_data.bit.Alr = panel_led->m_out_data.bit.Wrn;
		panel_led->m_out_data.bit.Wrn = panel_led->m_out_data.bit.En;
		panel_led->m_out_data.bit.En = panel_led->m_out_data.bit.ZS;
		panel_led->m_out_data.bit.ZS = panel_led->m_out_data.bit.Lim;
		panel_led->m_out_data.bit.Lim = ~panel_led->m_out_data.bit.T_mn;
	}
	panel_led_blink = 0b00001111 & (panel_led_blink + 1);

	panel_led->out_data = panel_led->m_out_data.all;
}
