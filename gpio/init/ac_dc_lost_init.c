/*
 * ac_dc_lost_init.c
 *
 *  Created on: 11 дек. 2024 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define AC_DC_LOST_INIT_PINS_COUNT 2

const gpio_pin_t GPI_AC_lost_App = GPIO_PIN(GPIOB, GPIO_PIN_14); /*94, PB14, GPIO_Input, AC_lost_App*/
const gpio_pin_t GPI_Lost_5V_App = GPIO_PIN(GPIOH, GPIO_PIN_7); /*84, PH7, GPIO_Input, Lost_5V_App*/

const gpio_pin_cfg_t ac_dc_lost_gpio_cfg[AC_DC_LOST_INIT_PINS_COUNT] = {
		GPIO_PIN_CFG(GPIOH, GPIO_PIN_7, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),					/*84, PH7,		GPIO_Input,		OFF,		Lost_5V_App*/
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_14, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),					/*94, PB14,		GPIO_Input,		OFF,		AC_lost_App*/
};

void gpio_ac_dc_lost_cfg_setup() {
	gpio_pins_cfg_setup(ac_dc_lost_gpio_cfg, AC_DC_LOST_INIT_PINS_COUNT);
}
