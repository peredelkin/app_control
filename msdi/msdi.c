#include "msdi.h"
#include "gpio/gpio.h"
#include "spi/init/spi_init.h"
#include "spi/settings/spi_settings.h"
#include "gpio/init/gpio_init.h"
#include "tic12400/tic12400.h"
#include "./tic12400/settings/tic12400_settings.h"


void msdi_data_fill(M_msdi *msdi) {
	msdi->out_di = msdi->data.IN_STAT_COMP.all;
	msdi->out_ai[4] = msdi->data.ANA_STAT1.bit.in0_ana;
	msdi->out_ai[5] = msdi->data.ANA_STAT1.bit.in1_ana;
	msdi->out_ai[3] = msdi->data.ANA_STAT2.bit.in0_ana;
	msdi->out_ai[0] = msdi->data.ANA_STAT2.bit.in1_ana;
	msdi->out_ai[2] = msdi->data.ANA_STAT3.bit.in0_ana;
	msdi->out_ai[1] = msdi->data.ANA_STAT3.bit.in1_ana;
	msdi->out_ai[6] = msdi->data.ANA_STAT9.bit.in0_ana;
	msdi->out_ai[7] = msdi->data.ANA_STAT12.bit.in0_ana;
}

void msdi_data_reset(M_msdi *msdi) {
	msdi->out_di = 0;
	for (int n = 0; n < MSDI_AI_COUNT; n++) {
		msdi->out_ai[n] = 0;
	}
}

uint8_t tic12400_addr_array[7] = {
		TIC12400_INT_STAT,
		TIC12400_IN_STAT_COMP,
		TIC12400_ANA_STAT1,
		TIC12400_ANA_STAT2,
		TIC12400_ANA_STAT3,
		TIC12400_ANA_STAT9,
		TIC12400_ANA_STAT12
};

METHOD_INIT_IMPL(M_msdi, msdi)
{
	gpio_tic12400_cfg_setup();
	tic12400_init(&(msdi->tic12400), &SPI4_Bus, &spi_tic12400_cfg);

	//Инит SPI
	spi_bus_open(msdi->tic12400.spi_bus, msdi->tic12400.spi_cfg);

	//предварительная инициализация
	tic12400_reg_write(&(msdi->tic12400), (uint32_t*) &tic124_settings_const, tic124_settings_addr, 0, TIC12400_SETTINGS_COUNT);

	//Деинициализация SPI
	spi_bus_close(msdi->tic12400.spi_bus);
}

METHOD_DEINIT_IMPL(M_msdi, msdi)
{
}

//tic12400_reg_read(&(msdi->tic12400), &((uint32_t*) &tic12400_data_array)[0], &tic12400_addr_array[0], 7);
//tic12400_wait(&msdi->tic12400);
//tic12400_reg_write(&(msdi->tic12400), (uint32_t*)&tic124_settings_const, &tic124_settings_addr[0], TIC12400_SETTINGS_COUNT);
METHOD_CALC_IMPL(M_msdi, msdi)
{
	//Инит SPI
	spi_bus_open(msdi->tic12400.spi_bus, msdi->tic12400.spi_cfg);

	//чтение статуса
	tic12400_reg_read(&(msdi->tic12400), ((uint32_t*) &msdi->data), tic12400_addr_array, 0, 1);
	//обработка статуса
	if(!(tic12400_wait(&msdi->tic12400))) {
		msdi->int_stat.all |= msdi->data.INT_STAT.all;
		if(msdi->int_stat.bit.por) {
			msdi->int_stat.bit.por = 0;
			tic12400_reg_write(&(msdi->tic12400), (uint32_t*) &tic124_settings_const, tic124_settings_addr, 0, TIC12400_SETTINGS_COUNT);
		}
	}
	//чтение входов
	if(!(tic12400_wait(&msdi->tic12400))) {
		tic12400_reg_read(&(msdi->tic12400), ((uint32_t*) &msdi->data), tic12400_addr_array, 1, 6);
	}
	//заполнение данных
	if(!(tic12400_wait(&msdi->tic12400))) {
		msdi_data_fill(msdi);
	}

	//Деинициализация SPI
	spi_bus_close(msdi->tic12400.spi_bus);
}





