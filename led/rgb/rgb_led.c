#include "rgb_led.h"

static const gpio_pin_cfg_t rgb_led_cfg [LED_RGB_PIN_COUNT] =
{
	GPIO_PIN_CFG(GPIOH, LED_RGB_R_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_2, GPIO_STATE_ON),	/*89, PH12,		TIM5_CH3,		OFF,		R_LED_App*/
	GPIO_PIN_CFG(GPIOH, LED_RGB_G_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_2, GPIO_STATE_ON),	/*88, PH11,		TIM5_CH2,		OFF,		G_LED_App*/
	GPIO_PIN_CFG(GPIOH, LED_RGB_B_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_2, GPIO_STATE_ON),	/*87, PH10,		TIM5_CH1,		OFF,		B_LED_App*/
};

static const uint16_t rgb_led_bsrrh [LED_RGB_STATE_COUNT] = {
		(LEG_RGB_MASK & ((0 << LED_RGB_R_PIN) | (0 << LED_RGB_G_PIN) | (0 << LED_RGB_B_PIN))),	/*0 черный*/
		(LEG_RGB_MASK & ((1 << LED_RGB_R_PIN) | (0 << LED_RGB_G_PIN) | (1 << LED_RGB_B_PIN))),	/*1 фиолетовый*/
		(LEG_RGB_MASK & ((0 << LED_RGB_R_PIN) | (0 << LED_RGB_G_PIN) | (1 << LED_RGB_B_PIN))),	/*2 синий*/
		(LEG_RGB_MASK & ((0 << LED_RGB_R_PIN) | (1 << LED_RGB_G_PIN) | (1 << LED_RGB_B_PIN))),	/*3 голубой*/
		(LEG_RGB_MASK & ((0 << LED_RGB_R_PIN) | (1 << LED_RGB_G_PIN) | (0 << LED_RGB_B_PIN))),	/*4 зеленый*/
		(LEG_RGB_MASK & ((1 << LED_RGB_R_PIN) | (1 << LED_RGB_G_PIN) | (0 << LED_RGB_B_PIN))),	/*5 желтый*/
		(LEG_RGB_MASK & ((1 << LED_RGB_R_PIN) | (0 << LED_RGB_G_PIN) | (0 << LED_RGB_B_PIN))),	/*6 красный*/
		(LEG_RGB_MASK & ((1 << LED_RGB_R_PIN) | (1 << LED_RGB_G_PIN) | (1 << LED_RGB_B_PIN))),	/*7 белый*/
};

static const uint16_t rgb_led_bsrrl [LED_RGB_STATE_COUNT] = {
		(LEG_RGB_MASK & ~((0 << LED_RGB_R_PIN) | (0 << LED_RGB_G_PIN) | (0 << LED_RGB_B_PIN))),
		(LEG_RGB_MASK & ~((1 << LED_RGB_R_PIN) | (0 << LED_RGB_G_PIN) | (1 << LED_RGB_B_PIN))),
		(LEG_RGB_MASK & ~((0 << LED_RGB_R_PIN) | (0 << LED_RGB_G_PIN) | (1 << LED_RGB_B_PIN))),
		(LEG_RGB_MASK & ~((0 << LED_RGB_R_PIN) | (1 << LED_RGB_G_PIN) | (1 << LED_RGB_B_PIN))),
		(LEG_RGB_MASK & ~((0 << LED_RGB_R_PIN) | (1 << LED_RGB_G_PIN) | (0 << LED_RGB_B_PIN))),
		(LEG_RGB_MASK & ~((1 << LED_RGB_R_PIN) | (1 << LED_RGB_G_PIN) | (0 << LED_RGB_B_PIN))),
		(LEG_RGB_MASK & ~((1 << LED_RGB_R_PIN) | (0 << LED_RGB_G_PIN) | (0 << LED_RGB_B_PIN))),
		(LEG_RGB_MASK & ~((1 << LED_RGB_R_PIN) | (1 << LED_RGB_G_PIN) | (1 << LED_RGB_B_PIN))),
};

METHOD_INIT_IMPL(M_rgb_led, rgb_led)
{
	//инициализация пинов
	gpio_pins_cfg_setup(rgb_led_cfg, LED_RGB_PIN_COUNT);
}

METHOD_DEINIT_IMPL(M_rgb_led, rgb_led)
{
}

METHOD_CALC_IMPL(M_rgb_led, rgb_led)
{
	uint16_t index = 0;
	index = LED_RGB_STATE_MASK & (rgb_led->in);
	LED_RGB_GPIO->BSRRH = rgb_led_bsrrh[index];
	LED_RGB_GPIO->BSRRL = rgb_led_bsrrl[index];
}
