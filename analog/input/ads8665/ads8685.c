#include "gpio/init/gpio_init.h"
#include "spi/init/spi_init.h"
#include "spi/settings/spi_settings.h"
#include "ads8685.h"
#include "modules/modules.h"

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

	ads8665->status |= ADS8685_STATUS_READY;
}

METHOD_DEINIT_IMPL(M_ads8685, ads8665)
{
}

METHOD_CALC_IMPL(M_ads8685, ads8665)
{
}
