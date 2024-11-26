#include "ncv7608.h"
#include "ncv7608_frame.h"
#include "spi/init/spi_init.h"
#include "gpio/init/gpio_init.h"
#include "spi/settings/spi_settings.h"

#define NCV7608_OUTPUT_COUNT 8
#define NCV7608_OUTPUT_1 5
#define NCV7608_OUTPUT_2 4
#define NCV7608_OUTPUT_3 6
#define NCV7608_OUTPUT_4 7
#define NCV7608_OUTPUT_5 3
#define NCV7608_OUTPUT_6 2
#define NCV7608_OUTPUT_7 0
#define NCV7608_OUTPUT_8 1

static const uint16_t ncv7608_mask_out [NCV7608_OUTPUT_COUNT] = {
		(1 << NCV7608_OUTPUT_1),
		(1 << NCV7608_OUTPUT_2),
		(1 << NCV7608_OUTPUT_3),
		(1 << NCV7608_OUTPUT_4),
		(1 << NCV7608_OUTPUT_5),
		(1 << NCV7608_OUTPUT_6),
		(1 << NCV7608_OUTPUT_7),
		(1 << NCV7608_OUTPUT_8),
};

NCV7608_TX_FRAME_REG ncv7608_tx;
NCV7608_RX_FRAME_REG ncv7608_rx;
SPI_BUS_DATA_TypeDef ncv7608_spi_control = {
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
	for (int n = 0; n < NCV7608_OUTPUT_COUNT; n++) {
		if(ncv7608->in & (1 << n)) {
			ncv7608_tx.all |= ncv7608_mask_out[n];
		} else {
			ncv7608_tx.all &= ~ncv7608_mask_out[n];
		}
	}

	//Инит SPI
	spi_bus_open(&SPI4_Bus, &spi_ncv7608_cfg);

	//занимает шину и начинает прием/передачу
	spi_bus_transfer(&SPI4_Bus, &ncv7608_spi_control, 1, SPI_BYTE_ORDER_NORMAL, NULL, NULL);

	//Деинициализация SPI
	spi_bus_close(&SPI4_Bus);
}
