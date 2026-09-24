#include "rgb_led.h"
#include "gpio/init/gpio_init.h"

void rgb_led_set(bool R, bool G, bool B) {
	gpio_output_bit_setup(&GPO_RGB_LED_App[0], !B);
	gpio_output_bit_setup(&GPO_RGB_LED_App[1], !G);
	gpio_output_bit_setup(&GPO_RGB_LED_App[2], !R);
}

void rgb_led_switch_color(uint32_t color) {
	switch(color) {
	case RGB_LED_COLOR_BLACK: rgb_led_set(0,0,0); //0R 0G 0B
		break;
	case RGB_LED_COLOR_VIOLET: rgb_led_set(1,0,1); //1R 0G 1B
		break;
	case RGB_LED_COLOR_BLUE_DARK: rgb_led_set(0,0,1); //0R 0G 1B
		break;
	case RGB_LED_COLOR_BLUE: rgb_led_set(0,1,1); //0R 1G 1B
		break;
	case RGB_LED_COLOR_GREEN: rgb_led_set(0,1,0); //0R 1G 0B
		break;
	case RGB_LED_COLOR_YELLOW: rgb_led_set(1,1,0); //1R 1G 0B
		break;
	case RGB_LED_COLOR_RED: rgb_led_set(1,0,0); //1R 0G 0B
		break;
	case RGB_LED_COLOR_WHITE: rgb_led_set(1,1,1); //1R 1G 1B
		break;
	default: rgb_led_set(0,0,0); //0R 0G 0B
		break;
	}
}

static uint32_t rgb_input = 0;

void rgb_led_calc(M_rgb_led* rgb_led) {
	if (rgb_input) {
		rgb_input--;
	} else {
		rgb_input = 4;
	}
	switch (rgb_input) {
	case 1:
		rgb_led_switch_color(rgb_led->in_data_4);
		break;
	case 2:
		rgb_led_switch_color(rgb_led->in_data_3);
		break;
	case 3:
		rgb_led_switch_color(rgb_led->in_data_2);
		break;
	case 4:
		rgb_led_switch_color(rgb_led->in_data_1);
		break;
	default:
		rgb_led_switch_color(RGB_LED_COLOR_WHITE);
		break;
	}
}

METHOD_INIT_IMPL(M_rgb_led, rgb_led)
{
	rgb_led->status = RGB_LED_STATUS_NONE;
	rgb_led->control = RGB_LED_CONTROL_NONE;

	gpio_rgb_led_cfg_setup(); //RGB Led

	rgb_led->in_data_1 = RGB_LED_COLOR_BLACK;
	rgb_led->in_data_2 = RGB_LED_COLOR_BLACK;
	rgb_led->in_data_3 = RGB_LED_COLOR_BLACK;
	rgb_led->in_data_4 = RGB_LED_COLOR_BLACK;

	rgb_led->status |= RGB_LED_STATUS_READY;
}

METHOD_DEINIT_IMPL(M_rgb_led, rgb_led)
{
	rgb_led->control |= RGB_LED_CONTROL_STOP;
}

static void rgb_led_control_handler(M_rgb_led* rgb_led) {
	if(rgb_led->control & RGB_LED_CONTROL_START) {
		rgb_led->control &= ~RGB_LED_CONTROL_START;
		rgb_led->status |= RGB_LED_STATUS_RUN;
	}

	if(rgb_led->control & RGB_LED_CONTROL_STOP) {
		rgb_led->control &= ~RGB_LED_CONTROL_STOP;
		rgb_led->status &= ~RGB_LED_STATUS_RUN;
	}
}

METHOD_CALC_IMPL(M_rgb_led, rgb_led)
{
	rgb_led_control_handler(rgb_led);

	if((rgb_led->status & (RGB_LED_STATUS_READY | RGB_LED_STATUS_RUN)) ==
			(RGB_LED_STATUS_READY | RGB_LED_STATUS_RUN)) {
		rgb_led_calc(rgb_led);
	} else {
		rgb_led_switch_color(RGB_LED_COLOR_WHITE);
	}
}


