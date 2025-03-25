/*
 * gpio_board_id_init.c
 *
 *  Created on: 25 мар. 2025 г.
 *      Author: ruslan
 */

#include "gpio_init.h"

const gpio_pin_t GPI_Board_ID0 = GPIO_PIN(GPIOI, GPIO_PIN_4); /*173, PI4, GPIO_Input, Board_ID0_App*/
const gpio_pin_t GPI_Board_ID1 = GPIO_PIN(GPIOI, GPIO_PIN_5); /*174, PI5, GPIO_Input, Board_ID1_App*/
const gpio_pin_t GPI_Board_ID2 = GPIO_PIN(GPIOI, GPIO_PIN_6); /*175, PI6, GPIO_Input, Board_ID2_App*/
const gpio_pin_t GPI_Board_ID3 = GPIO_PIN(GPIOI, GPIO_PIN_7); /*176, PI7, GPIO_Input, Board_ID3_App*/

//GPIO_PIN_CFG(GPIOI, GPIO_PIN_4, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0),					/*173, PI4,		GPIO_Input,		OFF,		Board_ID0_App*/
//GPIO_PIN_CFG(GPIOI, GPIO_PIN_5, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0),					/*174, PI5,		GPIO_Input,		OFF,		Board_ID1_App*/
//GPIO_PIN_CFG(GPIOI, GPIO_PIN_6, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0),					/*175, PI6,		GPIO_Input,		OFF,		Board_ID2_App*/
//GPIO_PIN_CFG(GPIOI, GPIO_PIN_7, GPIO_MODE_IN, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0),					/*176, PI7,		GPIO_Input,		OFF,		Board_ID3_App*/
