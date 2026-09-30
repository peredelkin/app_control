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
		if((msdi->m_data.IN_STAT_COMP.all & DI_MSDI_MASK[n]) == DI_MSDI_MASK[n]) {
			msdi->out_digital |= (1 << n);
		} else {
			msdi->out_digital &= ~(1 << n);
		}
	}
	msdi->out_analog[AI_0_MSDI_INPUT] = msdi->m_data.ANA_STAT1.bit.in0_ana;
	msdi->out_analog[AI_1_MSDI_INPUT] = msdi->m_data.ANA_STAT1.bit.in1_ana;
	msdi->out_analog[AI_2_MSDI_INPUT] = msdi->m_data.ANA_STAT2.bit.in0_ana;
	msdi->out_analog[AI_3_MSDI_INPUT] = msdi->m_data.ANA_STAT2.bit.in1_ana;
	msdi->out_analog[AI_4_MSDI_INPUT] = msdi->m_data.ANA_STAT3.bit.in0_ana;
	msdi->out_analog[AI_5_MSDI_INPUT] = msdi->m_data.ANA_STAT3.bit.in1_ana;
	msdi->out_analog[AI_6_MSDI_INPUT] = msdi->m_data.ANA_STAT9.bit.in0_ana;
	msdi->out_analog[AI_7_MSDI_INPUT] = msdi->m_data.ANA_STAT12.bit.in0_ana;

	msdi->out_ref = ((IQ15(6) * (msdi->out_analog[AI_6_MSDI_INPUT])) >> 10); //full scale 6v

	msdi->out_vcc = ((IQ15(30) * (msdi->out_analog[AI_7_MSDI_INPUT])) >> 10); //full scale 30v
}

void msdi_data_reset(M_msdi *msdi) {
	msdi->out_digital = 0;
	for (int n = 0; n < MSDI_AI_COUNT; n++) {
		msdi->out_analog[n] = 0;
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


static void msdi_rx_frame_status_handler(M_msdi *msdi) {
	//Other Interrupt: OV, UV, CRC_CALC. WET_DIAG, ADC_DIAG, CHK_FAIL.
	if(msdi->m_tic12400.status.bit.oi) {
		msdi->status |= MSDI_STATUS_OI;
	}
	//Temperature Event: TW, TSD.
	if(msdi->m_tic12400.status.bit.temp) {
		msdi->status |= MSDI_STATUS_TEMP;
	}
	//VS Threshold Crossing: VS0, VS1.
	if(msdi->m_tic12400.status.bit.vs_th) {
		msdi->status |= MSDI_STATUS_VS_TH;
	}
	//Switch State Change: SSC
	if(msdi->m_tic12400.status.bit.ssc) {
		msdi->status |= MSDI_STATUS_SSC;
	}
	//Parity Fail: PRTY_FAIL
	if(msdi->m_tic12400.status.bit.par_fail) {
		msdi->status |= MSDI_STATUS_PRTY_FAIL;
	}
	//SPI Error: SPI_FAIL
	if(msdi->m_tic12400.status.bit.spi_fail) {
		msdi->status |= MSDI_STATUS_SPI_FAIL;
	}
	//Power-on Reset: POR
	if(msdi->m_tic12400.status.bit.por) {
		msdi->status |= MSDI_STATUS_POR;
	}
	//сбросим статусы
	msdi->m_tic12400.status.all = 0;
}

static void msdi_int_status_handler(M_msdi *msdi) {
	//Power-on Reset
	if(msdi->status & MSDI_STATUS_POR) {
		if(msdi->m_data.INT_STAT.bit.por) {
			msdi->status |= MSDI_STATUS_INT_POR;
			msdi->status &= ~MSDI_STATUS_POR;
			//msdi->m_data.INT_STAT.bit.por = 0;
		}
	}

	//SPI Error
	if(msdi->status & MSDI_STATUS_SPI_FAIL) {
		if(msdi->m_data.INT_STAT.bit.spi_fail) {
			msdi->status |= MSDI_STATUS_INT_SPI_FAIL;
			msdi->status &= ~MSDI_STATUS_SPI_FAIL;
			//msdi->m_data.INT_STAT.bit.spi_fail = 0;
		}
	}

	//Parity Fail
	if(msdi->status & MSDI_STATUS_PRTY_FAIL) {
		if(msdi->m_data.INT_STAT.bit.par_fail) {
			msdi->status |= MSDI_STATUS_INT_PRTY_FAIL;
			msdi->status &= ~MSDI_STATUS_PRTY_FAIL;
			//msdi->m_data.INT_STAT.bit.par_fail = 0;
		}
	}

	//Switch state change
	if(msdi->status & MSDI_STATUS_SSC) {
		if(msdi->m_data.INT_STAT.bit.ssc) {
			msdi->status |= MSDI_STATUS_INT_SSC;
			msdi->status &= ~MSDI_STATUS_SSC;
			//msdi->m_data.INT_STAT.bit.ssc = 0;
		}
	}

	//VS Threshold Crossing
	if(msdi->status & MSDI_STATUS_VS_TH) {
		bool processed = false;
		//VS0_THRES2A or VS0_THRES2B
		if(msdi->m_data.INT_STAT.bit.vs0) {
			msdi->status |= MSDI_STATUS_INT_VS0;
			processed = true;
			//msdi->m_data.INT_STAT.bit.vs0 = 0;
		}
		//VS1_THRES2A or VS1_THRES2B
		if(msdi->m_data.INT_STAT.bit.vs1) {
			msdi->status |= MSDI_STATUS_INT_VS1;
			processed = true;
			//msdi->m_data.INT_STAT.bit.vs1 = 0;
		}

		if(processed == true) {
			msdi->status &= ~MSDI_STATUS_VS_TH;
		}
	}

	//Temperature Event
	if(msdi->status & MSDI_STATUS_TEMP) {
		bool processed = false;
		//Temperature Shutdown
		if(msdi->m_data.INT_STAT.bit.tsd) {
			msdi->status |= MSDI_STATUS_INT_TSD;
			processed = true;
			//msdi->m_data.INT_STAT.bit.tsd = 0;
		}
		//Temperature warning
		if(msdi->m_data.INT_STAT.bit.tw) {
			msdi->status |= MSDI_STATUS_INT_TW;
			processed = true;
			//msdi->m_data.INT_STAT.bit.tw = 0;
		}

		if(processed == true) {
			msdi->status &= ~MSDI_STATUS_TEMP;
		}
	}

	//Other Interrupt: OV, UV, CRC_CALC, WET_DIAG, ADC_DIAG, CHK_FAIL.
	if(msdi->status & MSDI_STATUS_OI) {
		bool processed = false;
		//Over-voltage
		if(msdi->m_data.INT_STAT.bit.ov) {
			msdi->status |= MSDI_STATUS_INT_OV;
			processed = true;
		}
		//Under-voltage
		if(msdi->m_data.INT_STAT.bit.uv) {
			msdi->status |= MSDI_STATUS_INT_UV;
			processed = true;
		}
		//CRC calculation is finished
		if(msdi->m_data.INT_STAT.bit.crc_calc) {
			msdi->status |= MSDI_STATUS_INT_CRC_CALC;
			processed = true;
		}
		//Wetting current error
		if(msdi->m_data.INT_STAT.bit.wet_diag) {
			msdi->status |= MSDI_STATUS_INT_WET_DIAG;
			processed = true;
		}
		//ADC self-diagnostic error
		if(msdi->m_data.INT_STAT.bit.adc_diag) {
			msdi->status |= MSDI_STATUS_INT_ADC_DIAG;
			processed = true;
		}
		//Error is detected when loading factory settings
		if(msdi->m_data.INT_STAT.bit.chk_fail) {
			msdi->status |= MSDI_STATUS_INT_CHK_FAIL;
			processed = true;
		}

		if(processed == true) {
			msdi->status &= ~MSDI_STATUS_OI;
		}
	}
}

static void msdi_spi_bus_close(M_msdi* msdi) {
	spi_bus_close(msdi->m_tic12400.spi_bus);
}

static bool msdi_software_reset(M_msdi* msdi) {
	TIC12400_CONFIG_REG CONFIG = {0};
	uint8_t CONFIG_ADDR = TIC12400_CONFIG;
	CONFIG.bit.reset = 1;
	bool prty = tic12400_reg_read(&(msdi->m_tic12400), ((uint32_t*) &CONFIG), &CONFIG_ADDR, 0, 1, NULL, NULL);
	return prty;
}

static bool msdi_load_settings(M_msdi* msdi) {
	return tic12400_reg_write(&(msdi->m_tic12400), ((uint32_t*) &tic124_settings_const), tic124_settings_addr, 0, TIC12400_SETTINGS_COUNT, NULL, NULL);
}

static bool msdi_read_int_stat(M_msdi* msdi) {
	return tic12400_reg_read(&(msdi->m_tic12400), ((uint32_t*) &msdi->m_data), tic12400_addr_array, 0, 1, NULL, NULL);
}

static bool msdi_read_inputs(M_msdi* msdi) {
	return tic12400_reg_read(&(msdi->m_tic12400), ((uint32_t*) &msdi->m_data), tic12400_addr_array, 1, 6, NULL, NULL);
}

METHOD_INIT_IMPL(M_msdi, msdi)
{
	msdi->status = MSDI_STATUS_NONE;
	msdi->control = MSDI_CONTROL_NONE;
	//настройка пинов
	gpio_tic12400_cfg_setup();
	//инициализация структуры tic12400
	tic12400_init(&(msdi->m_tic12400), &SPI4_Bus, &spi_tic12400_cfg);
}

METHOD_DEINIT_IMPL(M_msdi, msdi)
{
}

METHOD_CALC_IMPL(M_msdi, msdi)
{
	//Инит SPI
	spi_bus_open(msdi->m_tic12400.spi_bus, msdi->m_tic12400.spi_cfg);

	//сброс статуса валидности данных
	msdi->status &= ~MSDI_STATUS_VALID;

	//чтение "Interrupt Status Register"
	if (msdi_read_int_stat(msdi)) {
		//случилась ошибка четности
		msdi->status |= MSDI_STATUS_ERROR;
		//освободим SPI и выйдем
		msdi_spi_bus_close(msdi);
		return;
	} else {
		//обработаем статусы RX фрейма
		msdi_rx_frame_status_handler(msdi);
		//обработаем статусы INT_STAT
		msdi_int_status_handler(msdi);
	}
	//если POR все еще не был обработан после программного сброса
	if (msdi->status & MSDI_STATUS_POR) {
		//освободим SPI и выйдем
		msdi_spi_bus_close(msdi);
		return;
	}

	//Error when Loading factory settings
	if(msdi->status & MSDI_STATUS_INT_CHK_FAIL) {
		//модуль не готов
		msdi->status &= ~MSDI_STATUS_READY;
		//выполним программный сброс
		if(msdi_software_reset(msdi)) {
			//случилась ошибка четности
			msdi->status |= MSDI_STATUS_ERROR;
		} else {
			//сбросим флаги INT_POR и INT_CHK_FAIL
			msdi->status &= ~(MSDI_STATUS_INT_POR | MSDI_STATUS_INT_CHK_FAIL);
			//вручную установим флаг POR
			msdi->status |= MSDI_STATUS_POR;
			//обработаем статусы RX фрейма
			msdi_rx_frame_status_handler(msdi);
		}
		//освободим SPI и выйдем
		msdi_spi_bus_close(msdi);
		return;
	}

	//Successful power-on-reset
	if(msdi->status & MSDI_STATUS_INT_POR) {
		//модуль не готов
		msdi->status &= ~MSDI_STATUS_READY;
		//повторная инициализация
		if (msdi_load_settings(msdi)) {
			//случилась ошибка четности
			msdi->status |= MSDI_STATUS_ERROR;
			//освободим SPI и выйдем
			msdi_spi_bus_close(msdi);
			return;
		} else {
			//проверим статус RX фрейма
			msdi_rx_frame_status_handler(msdi);
			//Если в процессе записи настроек возникли ошибки или предупреждения
			if(msdi->status &
					(MSDI_STATUS_OI |
					MSDI_STATUS_TEMP |
					MSDI_STATUS_PRTY_FAIL |
					MSDI_STATUS_SPI_FAIL |
					MSDI_STATUS_POR)) {
				//освободим SPI и выйдем
				msdi_spi_bus_close(msdi);
				return;
			} else {
				//сбросим флаг INT_POR
				msdi->status &= ~MSDI_STATUS_INT_POR;
				//модуль готов
				msdi->status |= MSDI_STATUS_READY;
			}
		}
	}

	if (msdi->status & MSDI_STATUS_READY) {
		//чтение входов
		if (msdi_read_inputs(msdi)) {
			//случилась ошибка четности
			msdi->status |= MSDI_STATUS_ERROR;
		} else {
			//проверим статус RX фрейма
			msdi_rx_frame_status_handler(msdi);
			//Если в процессе чтения входов не возникли ошибки или предупреждения
			if(!(msdi->status &
					(MSDI_STATUS_OI |
					MSDI_STATUS_TEMP |
					MSDI_STATUS_PRTY_FAIL |
					MSDI_STATUS_SPI_FAIL |
					MSDI_STATUS_POR))) {
				//заполяем данные, согласно настройкам
				msdi_data_fill(msdi);
				//сбросим статус ошибки
				msdi->status &= ~MSDI_STATUS_ERROR;
				//данные валидны
				msdi->status |= MSDI_STATUS_VALID;
			}
		}
	}
	//Деинициализация SPI
	msdi_spi_bus_close(msdi);
}





