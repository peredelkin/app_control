/*
 * gpio_ncv7608_init.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define NCV7608_GPIO_COUNT 2

const gpio_pin_t GPO_CS_DO_App = GPIO_PIN(GPIOE, GPIO_PIN_4); /*3, PE4, GPIO_Output, CS_DO_App*/
const gpio_pin_t GPO_EN_DO_App = GPIO_PIN(GPIOA, GPIO_PIN_0); /*40, PA0/WKUP, GPIO_Output, EN_DO_App*/

const gpio_pin_cfg_t ncv7608_gpio_cfg [NCV7608_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOE, GPIO_PIN_4, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_ON),		/*3, PE4,		GPIO_Output,	ON,		CS_DO_App*/
		GPIO_PIN_CFG(GPIOA, GPIO_PIN_0, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_ON),		/*40, PA0/WKUP,	GPIO_Output,	OFF,	EN_DO_App*/
};

void gpio_ncv7608_cfg_setup() {
	gpio_pins_cfg_setup(ncv7608_gpio_cfg, NCV7608_GPIO_COUNT);
}
