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

const uint8_t tic12400_addr_array[7] = {
		TIC12400_INT_STAT,
		TIC12400_IN_STAT_COMP,
		TIC12400_ANA_STAT1,
		TIC12400_ANA_STAT2,
		TIC12400_ANA_STAT3,
		TIC12400_ANA_STAT9,
		TIC12400_ANA_STAT12
};

uint8_t M_msdi_rx_frame_status_handler(M_msdi *msdi) {
	TIC12400_STATUS status = {
		.all = msdi->tic12400.status.all,
	};

	//Error RX frame parity
	if(msdi->tic12400.par_fail) {
		msdi->status |= MSDI_STATUS_ERROR;
	} else {
		//"SPI Error" or "Parity Fail"
		if (status.bit.spi_fail || status.bit.par_fail) {
			msdi->status |= MSDI_STATUS_ERROR;
		}

		//"Power-on Reset" or "Temperature Event"
		if(status.bit.por || status.bit.temp) {
			msdi->status |= MSDI_STATUS_WARNING;
		}

		//"VS Threshold Crossing" or "Switch State Change" or "Other Interrupt"
		if(status.bit.vs_th || status.bit.ssc || status.bit.oi) {
			msdi->status |= MSDI_STATUS_INT;
		}
	}

	//сброс статусов RX фрейма
	msdi->tic12400.status.all = 0;

	return status.all;
}

METHOD_INIT_IMPL(M_msdi, msdi)
{
	//настройка пинов
	gpio_tic12400_cfg_setup();
	//инициализация структуры tic12400
	tic12400_init(&(msdi->tic12400), &SPI4_Bus, &spi_tic12400_cfg);

	//Инит SPI
	spi_bus_open(msdi->tic12400.spi_bus, msdi->tic12400.spi_cfg);

	//предварительная инициализация
	tic12400_reg_write(&(msdi->tic12400), (uint32_t*) &tic124_settings_const, tic124_settings_addr, 0, TIC12400_SETTINGS_COUNT);

	//ожидание конца записи
	tic12400_wait(&msdi->tic12400);

	//проверка статусов RX фрейма
	M_msdi_rx_frame_status_handler(msdi);

	//Деинициализация SPI
	spi_bus_close(msdi->tic12400.spi_bus);
}

METHOD_DEINIT_IMPL(M_msdi, msdi)
{
}

//TODO: сделать счетчик ошибок обращения к микросхеме
METHOD_CALC_IMPL(M_msdi, msdi)
{
	//Инит SPI
	spi_bus_open(msdi->tic12400.spi_bus, msdi->tic12400.spi_cfg);

	//статус RX фрейма
	TIC12400_STATUS status = {
			.all = 0
	};

	/*
	 * Error:
	 * 		RX frame parity error
	 * 		SPI Error
	 * 		Parity Fail
	 *
	 * Warning:
	 * 		Power-on Reset
	 * 		Temperature Event
	 *
	 * Interrupt:
	 * 		VS Threshold Crossing
	 * 		Switch State Change
	 * 		Other Interrupt
	 */
	if(msdi->status & (MSDI_STATUS_ERROR | MSDI_STATUS_WARNING | MSDI_STATUS_INT)) {
		//очистка флагов
		msdi->status &= ~MSDI_STATUS_ERROR;
		//чтение "Interrupt Status Register"
		tic12400_reg_read(&(msdi->tic12400), ((uint32_t*) &msdi->data), tic12400_addr_array, 0, 1);
		//ожидание конца обмена
		tic12400_wait(&msdi->tic12400);
		//проверка статусов RX фрейма
		status.all = M_msdi_rx_frame_status_handler(msdi);
		//Деинициализация SPI и выход, если есть ошибки
		if(msdi->status & MSDI_STATUS_ERROR) {
			msdi->int_stat.all = 0;
			spi_bus_close(msdi->tic12400.spi_bus);
			return;
		}
		//очистка остальных флагов
		msdi->status &= ~(MSDI_STATUS_WARNING | MSDI_STATUS_INT);
		//Сохраним статусы
		msdi->int_stat.all |= msdi->data.INT_STAT.all;
	}

	/*
	 * "Power-On-Reset" or/and "An error is detected when loading factory settings
	 * into the device upon device initialization"
	 */
	if (msdi->int_stat.bit.por || msdi->int_stat.bit.chk_fail) {
		msdi->int_stat.bit.por = 0;
		msdi->int_stat.bit.chk_fail = 0;
		//повторная инициализация
		tic12400_reg_write(&(msdi->tic12400), (uint32_t*) &tic124_settings_const, tic124_settings_addr, 0, TIC12400_SETTINGS_COUNT);
		//ожидание конца обмена
		tic12400_wait(&msdi->tic12400);
		//проверка статусов RX фрейма
		status.all = M_msdi_rx_frame_status_handler(msdi);
		//Деинициализация SPI и выход, если есть ошибки
		if (msdi->status & (MSDI_STATUS_ERROR)) {
			spi_bus_close(msdi->tic12400.spi_bus);
			return;
		}
	}

	//Temperature Shutdown
	if(msdi->int_stat.bit.tsd) {
		msdi->int_stat.bit.tsd = 0;
	}

	//Temperature warning
	if(msdi->int_stat.bit.tw) {
		msdi->int_stat.bit.tw = 0;
	}

	//Over-voltage
	if(msdi->int_stat.bit.ov) {
		msdi->int_stat.bit.ov = 0;
	}

	//Under-voltage
	if(msdi->int_stat.bit.uv) {
		msdi->int_stat.bit.uv = 0;
	}

	//чтение входов
	tic12400_reg_read(&(msdi->tic12400), ((uint32_t*) &msdi->data), tic12400_addr_array, 1, 6);
	//ожидание конца обмена
	tic12400_wait(&msdi->tic12400);
	//проверка статусов RX фрейма
	status.all = M_msdi_rx_frame_status_handler(msdi);
	//TODO: определить условия сброса флага
	msdi->status &= ~MSDI_STATUS_VALID;
	//если нет ошибок и предупреждений
	if(!(msdi->status & (MSDI_STATUS_ERROR | MSDI_STATUS_WARNING))) {
		//заполяем данные, согласно настройкам
		msdi_data_fill(msdi);
		//данные валидны
		msdi->status |= MSDI_STATUS_VALID;
	}
	//Деинициализация SPI
	spi_bus_close(msdi->tic12400.spi_bus);
}





