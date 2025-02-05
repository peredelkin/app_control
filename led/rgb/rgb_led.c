#include "rgb_led.h"
#include "gpio/init/gpio_init.h"

void rgb_led_set(bool R, bool G, bool B) {
	gpio_output_bit_setup(&GPO_RGB_LED_App[0], !B);
	gpio_output_bit_setup(&GPO_RGB_LED_App[1], !G);
	gpio_output_bit_setup(&GPO_RGB_LED_App[2], !R);
}

//! Перечисление цветов
enum _Rgb_Led_Сolor {
	RGB_LED_COLOR_BLACK,
	RGB_LED_COLOR_VIOLET,
	RGB_LED_COLOR_BLUE_DARK,
	RGB_LED_COLOR_BLUE,
	RGB_LED_COLOR_GREEN,
	RGB_LED_COLOR_YELLOW,
	RGB_LED_COLOR_RED,
	RGB_LED_COLOR_WHITE
};

METHOD_INIT_IMPL(M_rgb_led, rgb_led)
{
	gpio_rgb_led_cfg_setup(); //RGB Led
}

METHOD_DEINIT_IMPL(M_rgb_led, rgb_led)
{
}

METHOD_CALC_IMPL(M_rgb_led, rgb_led)
{
	switch(rgb_led->in_data) {
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
	default:
		break;
	}
}
