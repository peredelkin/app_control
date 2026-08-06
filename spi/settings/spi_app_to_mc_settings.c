/*
 * spi_app_to_mc_settings.c
 *
 *  Created on: 1 июл. 2026 г.
 *      Author: ruslan
 */

#include "spi_settings.h"

const CFG_REG_SPI_TypeDef spi_app_to_mc_cfg = SPI_CFG(
		SPI_CPHA_FIRST,
		SPI_CPOL_IDLE_LOW,
		SPI_MSTR_SLAVE,
		APP_TO_MC_SPI_BR_FPCLK,
		SPI_LSBFIRST_LSB_FIRST,
		SPI_SSI_NSEL,
		SPI_SSM_DIS,
		SPI_RXONLY_ENA,
		SPI_DFF_8,
		SPI_CRCEN_DIS,
		SPI_BIDIOE_RX,
		SPI_BIDIMODE_UNIDIR,
		SPI_RXDMAEN_ENA,
		SPI_TXDMAEN_DIS,
		SPI_SSOE_DIS,
		SPI_FRF_MOTOROLA,
		SPI_ERRIE_DIS,
		GPI_CS_App_MC,
		1,
		1,
		3,
		0);
