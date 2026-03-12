#include "gpio/init/gpio_init.h"
#include "spi/init/spi_init.h"
#include "spi/settings/spi_settings.h"
#include "ads8685.h"
#include "modules/modules.h"

void ads8665_read(M_ads8685* ads8665) {
	if(gpio_input_bit_read(&GPI_RVS1_App)) {
		ads8665->m_tx_frame.all = 0;
		spi_bus_open(ads8665->m_spi_bus, &spi_ads8665_1_cfg);
		spi_bus_transfer(ads8665->m_spi_bus, &(ads8665->m_spi_control), 1, NULL, NULL);
		spi_bus_close(ads8665->m_spi_bus);
		ads8665->out_ch_a = ads8665->m_rx_frame.bit.conversion_result;
	}

	if(gpio_input_bit_read(&GPI_RVS2_App)) {
		ads8665->m_tx_frame.all = 0;
		spi_bus_open(ads8665->m_spi_bus, &spi_ads8665_2_cfg);
		spi_bus_transfer(ads8665->m_spi_bus, &(ads8665->m_spi_control), 1, NULL, NULL);
		spi_bus_close(ads8665->m_spi_bus);
		ads8665->out_ch_b = ads8665->m_rx_frame.bit.conversion_result;
	}
}

METHOD_INIT_IMPL(M_ads8685, ads8665)
{
	ads8665->status = ADS8685_STATUS_NONE;
	ads8665->control = ADS8685_CONTROL_NONE;

	ads8665->m_spi_bus = &SPI2_Bus;

	gpio_ads8685_cfg_setup(); //RVS2_App ResetAI_App RVS1_App CS1AI_App CS2AI_App

	gpio_output_bit_setup(&GPO_ResetAI_App, GPIO_STATE_OFF);
	sys_counter_delay(0, 100);
	gpio_output_bit_setup(&GPO_ResetAI_App, GPIO_STATE_ON);
	sys_counter_delay(0, 100);

	ads8665->m_spi_control.count = 4;
	ads8665->m_spi_control.rx = (uint8_t*)&(ads8665->m_rx_frame.all);
	ads8665->m_spi_control.tx = (uint8_t*)&(ads8665->m_tx_frame.all);
	ads8665->m_spi_control.byte_order = SPI_BYTE_ORDER_REVERSE;
	ads8665->m_spi_control.callback = NULL;
	ads8665->m_spi_control.callback_argument = NULL;

	ads8665->status |= ADS8685_STATUS_READY;
}

METHOD_DEINIT_IMPL(M_ads8685, ads8665)
{
}

METHOD_CALC_IMPL(M_ads8685, ads8665)
{
	ads8665_read(ads8665);
}
