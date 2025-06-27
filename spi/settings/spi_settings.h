/*
 * spi_settings.h
 *
 *  Created on: 26 дек. 2022 г.
 *      Author: Ruslan
 */

#ifndef INC_SPI_SETTINGS_H_
#define INC_SPI_SETTINGS_H_

#include "spi/spi.h"
#include "gpio/init/gpio_init.h"

//SPI_BR_FPCLK_2 - dac7562		SCLK max 50MHz	SPI2 APB1 42MHz
//SPI_BR_FPCLK_32 - tic12400	SCLK max 4MHz	SPI4 APB2 84MHz
//SPI_BR_FPCLK_64 - ncv7608		SCLK max 2MHz	SPI4 APB2 84MHz
//SPI_BR_FPCLK_16 - ili9341		SCLK max 10MHz	SPI5 APB2 84MHz

#define DAC7562_SPI_BR_FPCLK SPI_BR_FPCLK_2
#define TIC12400_SPI_BR_FPCLK SPI_BR_FPCLK_32
#define NCV7608_SPI_BR_FPCLK SPI_BR_FPCLK_64
#define ILI9341_SPI_BR_FPCLK SPI_BR_FPCLK_16

extern const CFG_REG_SPI_TypeDef spi_dac7562_cfg;
extern const CFG_REG_SPI_TypeDef spi_tic12400_cfg;
extern const CFG_REG_SPI_TypeDef spi_ncv7608_cfg;
extern const CFG_REG_SPI_TypeDef spi_ili9341_cfg;

//extern const CFG_REG_SPI_TypeDef spi_spi5_cfg;

#endif /* INC_SPI_SETTINGS_H_ */
