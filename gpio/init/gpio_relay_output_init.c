/*
 * gpio_relay_output_init.c
 *
 *  Created on: 12 дек. 2024 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

#define RELAY_OUTPUT_INIT_PINS_COUNT 4

const gpio_pin_t GPO_Relay_DO_App[RELAY_OUTPUT_INIT_PINS_COUNT] ={
		GPIO_PIN(GPIOG, GPIO_PIN_12), /*155, PG12, GPIO_Output, Relay1_DO_App*/
		GPIO_PIN(GPIOG, GPIO_PIN_13), /*156, PG13, GPIO_Output, Relay2_DO_App*/
		GPIO_PIN(GPIOG, GPIO_PIN_14), /*157, PG14, GPIO_Output, Relay3_DO_App*/
		GPIO_PIN(GPIOG, GPIO_PIN_15) /*160, PG15, GPIO_Output, Relay4_DO_App*/
};

const gpio_pin_cfg_t relay_output_gpio_cfg[RELAY_OUTPUT_INIT_PINS_COUNT] = {
		GPIO_PIN_CFG(GPIOG, GPIO_PIN_12, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*155, PG12,	GPIO_Output,	OFF,	Relay1_DO_App*/
		GPIO_PIN_CFG(GPIOG, GPIO_PIN_13, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*156, PG13,	GPIO_Output,	OFF,	Relay2_DO_App*/
		GPIO_PIN_CFG(GPIOG, GPIO_PIN_14, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*157, PG14,	GPIO_Output,	OFF,	Relay3_DO_App*/
		GPIO_PIN_CFG(GPIOG, GPIO_PIN_15, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*160, PG15,	GPIO_Output,	OFF,	Relay4_DO_App*/
};

void gpio_relay_output_cfg_setup() {
	gpio_pins_cfg_setup(relay_output_gpio_cfg, RELAY_OUTPUT_INIT_PINS_COUNT);
}
