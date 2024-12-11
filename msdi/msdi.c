#include "msdi.h"
#include "gpio/gpio.h"
#include "spi/init/spi_init.h"
#include "spi/settings/spi_settings.h"
#include "gpio/init/gpio_init.h"
#include "tic12400/tic12400.h"
#include "./tic12400/settings/tic12400_settings.h"

#define DI_MSDI_COUNT 10

#define DI_0_MSDI_INPUT  8
#define DI_1_MSDI_INPUT  9
#define DI_2_MSDI_INPUT  10
#define DI_3_MSDI_INPUT  11
#define DI_4_MSDI_INPUT  12
#define DI_5_MSDI_INPUT  13
#define DI_6_MSDI_INPUT  14
#define DI_7_MSDI_INPUT  15
#define DI_8_MSDI_INPUT  19
#define DI_9_MSDI_INPUT  20

#define AI_0_MSDI_INPUT  4
#define AI_1_MSDI_INPUT  5
#define AI_2_MSDI_INPUT  3
#define AI_3_MSDI_INPUT  0
#define AI_4_MSDI_INPUT  2
#define AI_5_MSDI_INPUT  1
#define AI_6_MSDI_INPUT  6
#define AI_7_MSDI_INPUT  7

static const uint32_t DI_MSDI_MASK [DI_MSDI_COUNT] = {
		(1 << DI_0_MSDI_INPUT),
		(1 << DI_1_MSDI_INPUT),
		(1 << DI_2_MSDI_INPUT),
		(1 << DI_3_MSDI_INPUT),
		(1 << DI_4_MSDI_INPUT),
		(1 << DI_5_MSDI_INPUT),
		(1 << DI_6_MSDI_INPUT),
		(1 << DI_7_MSDI_INPUT),
		(1 << DI_8_MSDI_INPUT),
		(1 << DI_9_MSDI_INPUT)
};

void msdi_data_fill(M_msdi *msdi) {
	for (int n = 0; n < DI_MSDI_COUNT; n++) {
		if((msdi->data.IN_STAT_COMP.all & DI_MSDI_MASK[n]) == DI_MSDI_MASK[n]) {
			msdi->out_di |= (1 << n);
		} else {
			msdi->out_di &= ~(1 << n);
		}
	}
	msdi->out_ai[AI_0_MSDI_INPUT] = msdi->data.ANA_STAT1.bit.in0_ana;
	msdi->out_ai[AI_1_MSDI_INPUT] = msdi->data.ANA_STAT1.bit.in1_ana;
	msdi->out_ai[AI_2_MSDI_INPUT] = msdi->data.ANA_STAT2.bit.in0_ana;
	msdi->out_ai[AI_3_MSDI_INPUT] = msdi->data.ANA_STAT2.bit.in1_ana;
	msdi->out_ai[AI_4_MSDI_INPUT] = msdi->data.ANA_STAT3.bit.in0_ana;
	msdi->out_ai[AI_5_MSDI_INPUT] = msdi->data.ANA_STAT3.bit.in1_ana;
	msdi->out_ai[AI_6_MSDI_INPUT] = msdi->data.ANA_STAT9.bit.in0_ana;
	msdi->out_ai[AI_7_MSDI_INPUT] = msdi->data.ANA_STAT12.bit.in0_ana;

	msdi->ref = ((IQ15(6) * (msdi->out_ai[6])) >> 10); //full scale 6v

	msdi->vcc = ((IQ15(30) * (msdi->out_ai[7])) >> 10); //full scale 30v
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

//TODO: сделать счетчик ошибок обращения к микросхеме
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
		//TODO: переименовать аргумент count в end либо переписать, чтобы работало как количество
		tic12400_reg_read(&(msdi->tic12400), ((uint32_t*) &msdi->data), tic12400_addr_array, 1, 7);
	}
	//заполнение данных
	if(!(tic12400_wait(&msdi->tic12400))) {
		msdi_data_fill(msdi);
	}

	//Деинициализация SPI
	spi_bus_close(msdi->tic12400.spi_bus);
}





