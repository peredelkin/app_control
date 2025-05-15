/*
 * gpio_dip_init.c
 *
 *  Created on: 25 мар. 2025 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

const gpio_pin_t GPI_Dip0_App = GPIO_PIN(GPIOF, GPIO_PIN_10); /*28, PF10, GPIO_Input, Dip0_App*/
const gpio_pin_t GPI_Dip1_App = GPIO_PIN(GPIOC, GPIO_PIN_0); /*32, PC0, GPIO_Input, Dip1_App*/

//GPIO_PIN_CFG(GPIOF, GPIO_PIN_10, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0),					/*28, PF10,		GPIO_Input,		OFF,		Dip0_App*/
//GPIO_PIN_CFG(GPIOC, GPIO_PIN_0, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0),					/*32, PC0,		GPIO_Input,		OFF,		Dip1_App*/
