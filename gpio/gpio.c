/*
 * gpio.c
 *
 *  Created on: 8 авг. 2022 г.
 *      Author: ruslan
 */

#include "gpio.h"

void gpio_pin_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_mode_t pin_mode, gpio_pin_otype_t pin_otype,
		gpio_pin_ospeed_t pin_ospeed, gpio_pin_pupd_t pin_pupd,
		gpio_pin_af_t pin_af,
		gpio_pin_state_t pin_state);

void gpio_cfg_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		const gpio_cfg_t *cfg);

void gpio_pin_cfg_setup(const gpio_pin_cfg_t *pin);

void gpio_pins_cfg_setup(const gpio_pin_cfg_t *pin, size_t count);

void gpio_port_cfg_setup(gpio_port_cfg_t *port);

void gpio_mode_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_mode_t pin_mode);

void gpio_output_type_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_otype_t pin_otype);

void gpio_output_speed_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_ospeed_t pin_ospeed);

void gpio_pull_updown_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_pupd_t pin_pupd);

gpio_pin_state_t gpio_input_data_bit_read(GPIO_TypeDef *gpio,
		gpio_pin_n_t pin_n);

gpio_pin_state_t gpio_input_bit_read(const gpio_pin_t *pin);

gpio_pin_state_t gpio_output_data_bit_read(GPIO_TypeDef *gpio,
		gpio_pin_n_t pin_n);

void gpio_output_data_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_state_t pin_state);

gpio_pin_state_t gpio_output_bit_read(const gpio_pin_t *pin);

void gpio_output_bit_setup(const gpio_pin_t *pin, gpio_pin_state_t pin_state);

void gpio_alternate_function_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_af_t pin_af);

void gpio_pin_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_mode_t pin_mode, gpio_pin_otype_t pin_otype,
		gpio_pin_ospeed_t pin_ospeed, gpio_pin_pupd_t pin_pupd,
		gpio_pin_af_t pin_af,
		gpio_pin_state_t pin_state) {
	gpio_output_speed_bit_setup(gpio, pin_n, pin_ospeed); //скорость
	gpio_output_type_bit_setup(gpio, pin_n, pin_otype); //тип
	gpio_pull_updown_bit_setup(gpio, pin_n, pin_pupd); //подтяжка
	gpio_output_data_bit_setup(gpio, pin_n, pin_state); //состояние
	gpio_alternate_function_bit_setup(gpio, pin_n, pin_af); //альт.
	gpio_mode_bit_setup(gpio, pin_n, pin_mode); //режим
}

void gpio_cfg_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		const gpio_cfg_t *cfg) {
	if(gpio == NULL) return;
	gpio_pin_setup(gpio, pin_n, cfg->pin_mode, cfg->pin_otype, cfg->pin_ospeed,
			cfg->pin_pupd, cfg->pin_af, cfg->pin_state);
}

void gpio_pin_cfg_setup(const gpio_pin_cfg_t *pin) {
	gpio_cfg_setup(pin->gpio, pin->pin_n, &pin->cfg);
}

void gpio_pins_cfg_setup(const gpio_pin_cfg_t *pin, size_t count) {
	size_t pin_count;
	for (pin_count = 0; pin_count < count; pin_count++) {
		gpio_pin_cfg_setup(&pin[pin_count]);
	}
}

void gpio_port_cfg_setup(gpio_port_cfg_t *port) {
	size_t pin_n;
	for (pin_n = 0; pin_n < GPIO_PORT_PIN_COUNT; pin_n++) {
		gpio_cfg_setup(port->gpio, pin_n, &port->cfg[pin_n]);
	}
}

void gpio_mode_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_mode_t pin_mode) {

	//BITS_GPIO_TypeDef *gpio_bits = (BITS_GPIO_TypeDef*) gpio;

	GPIO_MODER_REG MODER;

	//read from gpio
	MODER.all = gpio->MODER;

	switch (pin_n) {
	case GPIO_PIN_0:
		/*gpio_bits->*/MODER.bit.MODER0 = pin_mode;
		break;
	case GPIO_PIN_1:
		/*gpio_bits->*/MODER.bit.MODER1 = pin_mode;
		break;
	case GPIO_PIN_2:
		/*gpio_bits->*/MODER.bit.MODER2 = pin_mode;
		break;
	case GPIO_PIN_3:
		/*gpio_bits->*/MODER.bit.MODER3 = pin_mode;
		break;
	case GPIO_PIN_4:
		/*gpio_bits->*/MODER.bit.MODER4 = pin_mode;
		break;
	case GPIO_PIN_5:
		/*gpio_bits->*/MODER.bit.MODER5 = pin_mode;
		break;
	case GPIO_PIN_6:
		/*gpio_bits->*/MODER.bit.MODER6 = pin_mode;
		break;
	case GPIO_PIN_7:
		/*gpio_bits->*/MODER.bit.MODER7 = pin_mode;
		break;
	case GPIO_PIN_8:
		/*gpio_bits->*/MODER.bit.MODER8 = pin_mode;
		break;
	case GPIO_PIN_9:
		/*gpio_bits->*/MODER.bit.MODER9 = pin_mode;
		break;
	case GPIO_PIN_10:
		/*gpio_bits->*/MODER.bit.MODER10 = pin_mode;
		break;
	case GPIO_PIN_11:
		/*gpio_bits->*/MODER.bit.MODER11 = pin_mode;
		break;
	case GPIO_PIN_12:
		/*gpio_bits->*/MODER.bit.MODER12 = pin_mode;
		break;
	case GPIO_PIN_13:
		/*gpio_bits->*/MODER.bit.MODER13 = pin_mode;
		break;
	case GPIO_PIN_14:
		/*gpio_bits->*/MODER.bit.MODER14 = pin_mode;
		break;
	case GPIO_PIN_15:
		/*gpio_bits->*/MODER.bit.MODER15 = pin_mode;
		break;
	default:
		break;
	}

	//write to gpio
	gpio->MODER = MODER.all;
}

void gpio_output_type_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_otype_t pin_otype) {

	//BITS_GPIO_TypeDef *gpio_bits = (BITS_GPIO_TypeDef*) gpio;

	GPIO_OTYPER_REG OTYPER;

	//read from gpio
	OTYPER.all = gpio->OTYPER;

	switch (pin_n) {
	case GPIO_PIN_0:
		/*gpio_bits->*/OTYPER.bit.OT0 = pin_otype;
		break;
	case GPIO_PIN_1:
		/*gpio_bits->*/OTYPER.bit.OT1 = pin_otype;
		break;
	case GPIO_PIN_2:
		/*gpio_bits->*/OTYPER.bit.OT2 = pin_otype;
		break;
	case GPIO_PIN_3:
		/*gpio_bits->*/OTYPER.bit.OT3 = pin_otype;
		break;
	case GPIO_PIN_4:
		/*gpio_bits->*/OTYPER.bit.OT4 = pin_otype;
		break;
	case GPIO_PIN_5:
		/*gpio_bits->*/OTYPER.bit.OT5 = pin_otype;
		break;
	case GPIO_PIN_6:
		/*gpio_bits->*/OTYPER.bit.OT6 = pin_otype;
		break;
	case GPIO_PIN_7:
		/*gpio_bits->*/OTYPER.bit.OT7 = pin_otype;
		break;
	case GPIO_PIN_8:
		/*gpio_bits->*/OTYPER.bit.OT8 = pin_otype;
		break;
	case GPIO_PIN_9:
		/*gpio_bits->*/OTYPER.bit.OT9 = pin_otype;
		break;
	case GPIO_PIN_10:
		/*gpio_bits->*/OTYPER.bit.OT10 = pin_otype;
		break;
	case GPIO_PIN_11:
		/*gpio_bits->*/OTYPER.bit.OT11 = pin_otype;
		break;
	case GPIO_PIN_12:
		/*gpio_bits->*/OTYPER.bit.OT12 = pin_otype;
		break;
	case GPIO_PIN_13:
		/*gpio_bits->*/OTYPER.bit.OT13 = pin_otype;
		break;
	case GPIO_PIN_14:
		/*gpio_bits->*/OTYPER.bit.OT14 = pin_otype;
		break;
	case GPIO_PIN_15:
		/*gpio_bits->*/OTYPER.bit.OT15 = pin_otype;
		break;
	default:
		break;
	}

	//write to gpio
	gpio->OTYPER = OTYPER.all;
}

void gpio_output_speed_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_ospeed_t pin_ospeed) {

	//BITS_GPIO_TypeDef *gpio_bits = (BITS_GPIO_TypeDef*) gpio;

	GPIO_OSPEEDR_REG OSPEEDR;

	//read from gpio
	OSPEEDR.all = gpio->OSPEEDR;

	switch (pin_n) {
	case GPIO_PIN_0:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR0 = pin_ospeed;
		break;
	case GPIO_PIN_1:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR1 = pin_ospeed;
		break;
	case GPIO_PIN_2:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR2 = pin_ospeed;
		break;
	case GPIO_PIN_3:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR3 = pin_ospeed;
		break;
	case GPIO_PIN_4:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR4 = pin_ospeed;
		break;
	case GPIO_PIN_5:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR5 = pin_ospeed;
		break;
	case GPIO_PIN_6:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR6 = pin_ospeed;
		break;
	case GPIO_PIN_7:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR7 = pin_ospeed;
		break;
	case GPIO_PIN_8:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR8 = pin_ospeed;
		break;
	case GPIO_PIN_9:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR9 = pin_ospeed;
		break;
	case GPIO_PIN_10:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR10 = pin_ospeed;
		break;
	case GPIO_PIN_11:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR11 = pin_ospeed;
		break;
	case GPIO_PIN_12:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR12 = pin_ospeed;
		break;
	case GPIO_PIN_13:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR13 = pin_ospeed;
		break;
	case GPIO_PIN_14:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR14 = pin_ospeed;
		break;
	case GPIO_PIN_15:
		/*gpio_bits->*/OSPEEDR.bit.OSPEEDR15 = pin_ospeed;
		break;
	default:
		break;
	}

	//write to gpio
	gpio->OSPEEDR = OSPEEDR.all;
}

void gpio_pull_updown_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_pupd_t pin_pupd) {

	//BITS_GPIO_TypeDef *gpio_bits = (BITS_GPIO_TypeDef*) gpio;

	GPIO_PUPDR_REG PUPDR;

	//read from gpio
	PUPDR.all = gpio->PUPDR;

	switch (pin_n) {
	case GPIO_PIN_0:
		/*gpio_bits->*/PUPDR.bit.PUPDR0 = pin_pupd;
		break;
	case GPIO_PIN_1:
		/*gpio_bits->*/PUPDR.bit.PUPDR1 = pin_pupd;
		break;
	case GPIO_PIN_2:
		/*gpio_bits->*/PUPDR.bit.PUPDR2 = pin_pupd;
		break;
	case GPIO_PIN_3:
		/*gpio_bits->*/PUPDR.bit.PUPDR3 = pin_pupd;
		break;
	case GPIO_PIN_4:
		/*gpio_bits->*/PUPDR.bit.PUPDR4 = pin_pupd;
		break;
	case GPIO_PIN_5:
		/*gpio_bits->*/PUPDR.bit.PUPDR5 = pin_pupd;
		break;
	case GPIO_PIN_6:
		/*gpio_bits->*/PUPDR.bit.PUPDR6 = pin_pupd;
		break;
	case GPIO_PIN_7:
		/*gpio_bits->*/PUPDR.bit.PUPDR7 = pin_pupd;
		break;
	case GPIO_PIN_8:
		/*gpio_bits->*/PUPDR.bit.PUPDR8 = pin_pupd;
		break;
	case GPIO_PIN_9:
		/*gpio_bits->*/PUPDR.bit.PUPDR9 = pin_pupd;
		break;
	case GPIO_PIN_10:
		/*gpio_bits->*/PUPDR.bit.PUPDR10 = pin_pupd;
		break;
	case GPIO_PIN_11:
		/*gpio_bits->*/PUPDR.bit.PUPDR11 = pin_pupd;
		break;
	case GPIO_PIN_12:
		/*gpio_bits->*/PUPDR.bit.PUPDR12 = pin_pupd;
		break;
	case GPIO_PIN_13:
		/*gpio_bits->*/PUPDR.bit.PUPDR13 = pin_pupd;
		break;
	case GPIO_PIN_14:
		/*gpio_bits->*/PUPDR.bit.PUPDR14 = pin_pupd;
		break;
	case GPIO_PIN_15:
		/*gpio_bits->*/PUPDR.bit.PUPDR15 = pin_pupd;
		break;
	default:
		break;
	}

	//write to gpio
	gpio->PUPDR = PUPDR.all;
}

gpio_pin_state_t gpio_input_data_bit_read(GPIO_TypeDef *gpio,
		gpio_pin_n_t pin_n) {

	//BITS_GPIO_TypeDef *gpio_bits = (BITS_GPIO_TypeDef*) gpio;

	GPIO_IDR_REG IDR;

	//read from gpio
	IDR.all = gpio->IDR;

	switch (pin_n) {
	case GPIO_PIN_0:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR0);
	case GPIO_PIN_1:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR1);
	case GPIO_PIN_2:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR2);
	case GPIO_PIN_3:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR3);
	case GPIO_PIN_4:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR4);
	case GPIO_PIN_5:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR5);
	case GPIO_PIN_6:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR6);
	case GPIO_PIN_7:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR7);
	case GPIO_PIN_8:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR8);
	case GPIO_PIN_9:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR9);
	case GPIO_PIN_10:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR10);
	case GPIO_PIN_11:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR11);
	case GPIO_PIN_12:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR12);
	case GPIO_PIN_13:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR13);
	case GPIO_PIN_14:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR14);
	case GPIO_PIN_15:
		return (gpio_pin_state_t) (/*gpio_bits->*/IDR.bit.IDR15);
	default:
		return GPIO_STATE_OFF;
	}
}

gpio_pin_state_t gpio_input_bit_read(const gpio_pin_t *pin) {
	return gpio_input_data_bit_read(pin->gpio, pin->pin_n);
}

gpio_pin_state_t gpio_output_data_bit_read(GPIO_TypeDef *gpio,
		gpio_pin_n_t pin_n) {

	//BITS_GPIO_TypeDef *gpio_bits = (BITS_GPIO_TypeDef*) gpio;

	GPIO_ODR_REG ODR;

	//read form gpio
	ODR.all = gpio->ODR;

	switch (pin_n) {
	case GPIO_PIN_0:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR0);
	case GPIO_PIN_1:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR1);
	case GPIO_PIN_2:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR2);
	case GPIO_PIN_3:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR3);
	case GPIO_PIN_4:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR4);
	case GPIO_PIN_5:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR5);
	case GPIO_PIN_6:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR6);
	case GPIO_PIN_7:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR7);
	case GPIO_PIN_8:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR8);
	case GPIO_PIN_9:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR9);
	case GPIO_PIN_10:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR10);
	case GPIO_PIN_11:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR11);
	case GPIO_PIN_12:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR12);
	case GPIO_PIN_13:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR13);
	case GPIO_PIN_14:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR14);
	case GPIO_PIN_15:
		return (gpio_pin_state_t) (/*gpio_bits->*/ODR.bit.ODR15);
	default:
		return GPIO_STATE_OFF;
	}
}

gpio_pin_state_t gpio_output_bit_read(const gpio_pin_t *pin) {
	return gpio_output_data_bit_read(pin->gpio, pin->pin_n);
}

void gpio_output_data_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n, gpio_pin_state_t pin_state) {
	if(pin_state) {
		gpio->BSRRL = (1 << pin_n);
	} else {
		gpio->BSRRH = (1 << pin_n);
	}
}

void gpio_output_bit_setup(const gpio_pin_t *pin, gpio_pin_state_t pin_state) {
	gpio_output_data_bit_setup(pin->gpio, pin->pin_n, pin_state);
}

void gpio_alternate_function_bit_setup(GPIO_TypeDef *gpio, gpio_pin_n_t pin_n,
		gpio_pin_af_t pin_af) {

	//BITS_GPIO_TypeDef *gpio_bits = (BITS_GPIO_TypeDef*) gpio;

	GPIO_AFRL_REG AFRL;
	GPIO_AFRH_REG AFRH;

	//read from gpio
	AFRL.all = gpio->AFR[0];
	AFRH.all = gpio->AFR[1];

	switch (pin_n) {
	case GPIO_PIN_0:
		/*gpio_bits->*/AFRL.bit.AFRL0 = pin_af;
		break;
	case GPIO_PIN_1:
		/*gpio_bits->*/AFRL.bit.AFRL1 = pin_af;
		break;
	case GPIO_PIN_2:
		/*gpio_bits->*/AFRL.bit.AFRL2 = pin_af;
		break;
	case GPIO_PIN_3:
		/*gpio_bits->*/AFRL.bit.AFRL3 = pin_af;
		break;
	case GPIO_PIN_4:
		/*gpio_bits->*/AFRL.bit.AFRL4 = pin_af;
		break;
	case GPIO_PIN_5:
		/*gpio_bits->*/AFRL.bit.AFRL5 = pin_af;
		break;
	case GPIO_PIN_6:
		/*gpio_bits->*/AFRL.bit.AFRL6 = pin_af;
		break;
	case GPIO_PIN_7:
		/*gpio_bits->*/AFRL.bit.AFRL7 = pin_af;
		break;
	case GPIO_PIN_8:
		/*gpio_bits->*/AFRH.bit.AFRH8 = pin_af;
		break;
	case GPIO_PIN_9:
		/*gpio_bits->*/AFRH.bit.AFRH9 = pin_af;
		break;
	case GPIO_PIN_10:
		/*gpio_bits->*/AFRH.bit.AFRH10 = pin_af;
		break;
	case GPIO_PIN_11:
		/*gpio_bits->*/AFRH.bit.AFRH11 = pin_af;
		break;
	case GPIO_PIN_12:
		/*gpio_bits->*/AFRH.bit.AFRH12 = pin_af;
		break;
	case GPIO_PIN_13:
		/*gpio_bits->*/AFRH.bit.AFRH13 = pin_af;
		break;
	case GPIO_PIN_14:
		/*gpio_bits->*/AFRH.bit.AFRH14 = pin_af;
		break;
	case GPIO_PIN_15:
		/*gpio_bits->*/AFRH.bit.AFRH15 = pin_af;
		break;
	default:
		break;
	}

	//write to gpio
	gpio->AFR[0] = AFRL.all;
	gpio->AFR[1] = AFRH.all;
}

