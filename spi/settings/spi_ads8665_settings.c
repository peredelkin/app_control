/*
 * spi_ads8665_settings.c
 *
 *  Created on: 10 июл. 2025 г.
 *      Author: Ruslan
 */

#include "spi_settings.h"

/*
 * On power-up or after coming out of any asynchronous reset,
 * the device supports the SPI-00-S protocol for dataread and data write operations.
 */

const CFG_REG_SPI_TypeDef spi_ads8665_1_cfg = SPI_CFG(
		SPI_CPHA_FIRST,
		SPI_CPOL_IDLE_LOW,
		SPI_MSTR_MASTER,
		ADS8665_SPI_BR_FPCLK,
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
		GPO_CS1AI_App,
		1,
		1,
		3,
		86651);

const CFG_REG_SPI_TypeDef spi_ads8665_2_cfg = SPI_CFG(
		SPI_CPHA_FIRST,
		SPI_CPOL_IDLE_LOW,
		SPI_MSTR_MASTER,
		ADS8665_SPI_BR_FPCLK,
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
		GPO_CS2AI_App,
		1,
		1,
		3,
		86652);

