/*
 * gpio_tic12400_init.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define GPIO_TIC12400_COUNT 3

const gpio_pin_t GPO_CS_DI_App = GPIO_PIN(GPIOI, GPIO_PIN_8); /*7, PI8, GPIO_Output, CS_DI_App*/
const gpio_pin_t GPO_Reset_DI_App = GPIO_PIN(GPIOI, GPIO_PIN_9); /*11, PI9, GPIO_Output, Reset_DI_App*/
const gpio_pin_t GPI_Int_DI_App = GPIO_PIN(GPIOI, GPIO_PIN_10); /*12, PI10, GPIO_Input, Int_DI_App*/

const gpio_pin_cfg_t gpio_tic12400_cfg [GPIO_TIC12400_COUNT] = {
		GPIO_PIN_CFG(GPIOI, GPIO_PIN_8, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_ON),		/*7, PI8,		GPIO_Output,	ON,		CS_DI_App*/
		GPIO_PIN_CFG(GPIOI, GPIO_PIN_9, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*11, PI9,		GPIO_Output,	ON,		Reset_DI_App*/
		GPIO_PIN_CFG(GPIOI, GPIO_PIN_10, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*12, PI10,		GPIO_Input,		OFF,		Int_DI_App*/
};

void gpio_tic12400_cfg_setup() {
	gpio_pins_cfg_setup(gpio_tic12400_cfg, GPIO_TIC12400_COUNT);
}
