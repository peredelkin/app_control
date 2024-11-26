#include "relay_output.h"

#define RELAY_OUTPUT_GPIO ((BITS_GPIO_TypeDef*) GPIOG)
#define RELAY_OUTPUT_COUNT 4
#define RELAY_OUTPUT_1_PIN GPIO_PIN_12
#define RELAY_OUTPUT_2_PIN GPIO_PIN_13
#define RELAY_OUTPUT_3_PIN GPIO_PIN_14
#define RELAY_OUTPUT_4_PIN GPIO_PIN_15
#define RELAY_OUTPUT_1_DO 0
#define RELAY_OUTPUT_2_DO 1
#define RELAY_OUTPUT_3_DO 2
#define RELAY_OUTPUT_4_DO 3

static const gpio_pin_cfg_t relay_output_cfg [RELAY_OUTPUT_COUNT] = {
		GPIO_PIN_CFG(GPIOG, RELAY_OUTPUT_1_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*155, PG12,	GPIO_Output,	OFF,	Relay1_DO_App*/
		GPIO_PIN_CFG(GPIOG, RELAY_OUTPUT_2_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*156, PG13,	GPIO_Output,	OFF,	Relay2_DO_App*/
		GPIO_PIN_CFG(GPIOG, RELAY_OUTPUT_3_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*157, PG14,	GPIO_Output,	OFF,	Relay3_DO_App*/
		GPIO_PIN_CFG(GPIOG, RELAY_OUTPUT_4_PIN, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*160, PG15,	GPIO_Output,	OFF,	Relay4_DO_App*/
};

static const uint16_t relay_output_bsrr [RELAY_OUTPUT_COUNT] = {
		(1 << RELAY_OUTPUT_1_PIN),
		(1 << RELAY_OUTPUT_2_PIN),
		(1 << RELAY_OUTPUT_3_PIN),
		(1 << RELAY_OUTPUT_4_PIN)
};

static const uint16_t relay_output_mask_in [RELAY_OUTPUT_COUNT] = {
		(1 << RELAY_OUTPUT_1_DO),
		(1 << RELAY_OUTPUT_2_DO),
		(1 << RELAY_OUTPUT_3_DO),
		(1 << RELAY_OUTPUT_4_DO)
};

METHOD_INIT_IMPL(M_relay_output, relay)
{
	//инициализация пинов
	gpio_pins_cfg_setup(relay_output_cfg, RELAY_OUTPUT_COUNT);
}

METHOD_DEINIT_IMPL(M_relay_output, relay)
{

}

METHOD_CALC_IMPL(M_relay_output, relay)
{
	for (int n = 0; n < RELAY_OUTPUT_COUNT; n++) {
		if ((relay->in & relay_output_mask_in[n]) == relay_output_mask_in[n]) {
			RELAY_OUTPUT_GPIO->BSRRL = relay_output_bsrr[n];
		} else {
			RELAY_OUTPUT_GPIO->BSRRH = relay_output_bsrr[n];
		}
	}
}
