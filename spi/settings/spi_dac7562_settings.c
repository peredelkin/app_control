/*
 * spi_dac7562_settings.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "gpio/init/gpio_init.h"
#include "spi_settings.h"

const CFG_REG_SPI_TypeDef spi_dac7562_cfg = SPI_CFG(
		SPI_CPHA_SECOND,
		SPI_CPOL_IDLE_LOW,
		SPI_MSTR_MASTER,
		SPI_BR_FPCLK_32,
		SPI_LSBFIRST_MSB_FIRST,
		SPI_SSI_NSEL,
		SPI_SSM_ENA,
		SPI_RXONLY_DIS,
		SPI_DFF_8,
		SPI_CRCEN_DIS,
		SPI_BIDIOE_RX,
		SPI_BIDIMODE_UNIDIR,
		SPI_RXDMAEN_DIS,
		SPI_TXDMAEN_DIS,
		SPI_SSOE_DIS,
		SPI_FRF_MOTOROLA,
		SPI_ERRIE_DIS,
		GPO_SYNCAO_App,
		1,
		1,
		3);
