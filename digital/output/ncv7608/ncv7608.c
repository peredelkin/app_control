#include "ncv7608.h"
#include "ncv7608_frame.h"
#include "spi/init/spi_init.h"
#include "gpio/init/gpio_init.h"
#include "spi/settings/spi_settings.h"

NCV7608_TX_FRAME_REG ncv7608_tx;
NCV7608_RX_FRAME_REG ncv7608_rx;
SPI_BUS_FRAME_TypeDef ncv7608_spi_control = {
		.tx = (uint8_t*)&ncv7608_tx.all,
		.rx = (uint8_t*)&ncv7608_rx.all,
		.count = 2
};

METHOD_INIT_IMPL(M_ncv7608, ncv7608)
{
	gpio_ncv7608_cfg_setup();
}

METHOD_DEINIT_IMPL(M_ncv7608, ncv7608)
{
}

METHOD_CALC_IMPL(M_ncv7608, ncv7608)
{
	ncv7608->raw.all = ncv7608->in;

	ncv7608_tx.bit.driver_1_ena = ncv7608->raw.bit.out_7;
	ncv7608_tx.bit.driver_2_ena = ncv7608->raw.bit.out_8;
	ncv7608_tx.bit.driver_3_ena = ncv7608->raw.bit.out_6;
	ncv7608_tx.bit.driver_4_ena = ncv7608->raw.bit.out_5;
	ncv7608_tx.bit.driver_5_ena = ncv7608->raw.bit.out_2;
	ncv7608_tx.bit.driver_6_ena = ncv7608->raw.bit.out_1;
	ncv7608_tx.bit.driver_7_ena = ncv7608->raw.bit.out_3;
	ncv7608_tx.bit.driver_8_ena = ncv7608->raw.bit.out_4;

	//Инит SPI
	spi_bus_open(&SPI4_Bus, &spi_ncv7608_cfg);

	//занимает шину и начинает прием/передачу
	spi_bus_transfer(&SPI4_Bus, &ncv7608_spi_control, 1, SPI_BYTE_ORDER_NORMAL, NULL, NULL);

	//Деинициализация SPI
	spi_bus_close(&SPI4_Bus);
}
