/*
 * gpio_ili9341_init.c
 *
 *  Created on: 26 июн. 2025 г.
 *      Author: Ruslan
 */

#include "gpio_init.h"

#define GPIO_ILI9341_COUNT 3

const gpio_pin_t GPO_CS_SPI5_App = GPIO_PIN(GPIOH, GPIO_PIN_5); /*46, PH5, GPIO_Output, ON, CS_SPI5_App*/
const gpio_pin_t GPO_Board_Out3_App = GPIO_PIN(GPIOB, GPIO_PIN_0); /*56, PB0, GPIO_Output, Board_Out3_App*/
const gpio_pin_t GPO_Board_Out4_App = GPIO_PIN(GPIOB, GPIO_PIN_1); /*57, PB1, GPIO_Output, Board_Out4_App*/

const gpio_pin_cfg_t gpio_ili9341_cfg[GPIO_ILI9341_COUNT] = {
		GPIO_PIN_CFG(GPIOH, GPIO_PIN_5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_ON),		/*46, PH5,		GPIO_Output,	ON,		CS_SPI5_App*/
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_0, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),		/*56, PB0,		GPIO_Output,	OFF,	Board_Out3_App*/
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_1, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF)		/*57, PB1,		GPIO_Output,	OFF,	Board_Out4_App*/
};

void gpio_ili9341_cfg_setup() {
	gpio_pins_cfg_setup(gpio_ili9341_cfg, GPIO_ILI9341_COUNT);
}
