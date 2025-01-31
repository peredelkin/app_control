#include "panel_led.h"


METHOD_INIT_IMPL(M_panel_led, panel_led)
{
	panel_led->raw.bit.SCR_LED = 1;
}

METHOD_DEINIT_IMPL(M_panel_led, panel_led)
{
}

//дебаг: счетчик для теста светодиодов панельки
uint8_t panel_led_blink;

METHOD_CALC_IMPL(M_panel_led, panel_led)
{
	if(panel_led_blink == 0) panel_led->raw.bit.ZS ^= 1;
	panel_led_blink++;

	panel_led->out = panel_led->raw.all;
}
