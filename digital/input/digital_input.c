#include "gpio/init/gpio_init.h"
#include "digital_input.h"
#include "modules/modules.h"

METHOD_INIT_IMPL(M_digital_input, input)
{
	input->status = DIGITAL_INPUT_STATUS_NONE;
	input->control = DIGITAL_INPUT_CONTROL_NONE;

	INIT(msdi);

	gpio_em_stop_cfg_setup(); //EmStop_App
	gpio_ac_dc_lost_cfg_setup(); //AC DC Lost

	input->m_in_data.bit.const_0 = 0; //бит 0 всегда 0
	input->m_in_data.bit.const_1 = 1; //бит 1 всегда 1

	input->status |= DIGITAL_INPUT_STATUS_READY;
}

METHOD_DEINIT_IMPL(M_digital_input, input)
{
	DEINIT(msdi);
}

static void digital_input_control_handler(M_digital_input* input) {
	if(digital_in.control & DIGITAL_INPUT_CONTROL_START) {
		digital_in.control &= ~DIGITAL_INPUT_CONTROL_START;
		input->status |= DIGITAL_INPUT_STATUS_RUN;
	}

	if(digital_in.control & DIGITAL_INPUT_CONTROL_STOP) {
		digital_in.control &= ~DIGITAL_INPUT_CONTROL_STOP;
		input->status &= ~DIGITAL_INPUT_STATUS_RUN;
	}
}

static void digital_input_calc(M_digital_input* input) {
	input->m_in_data.bit.em_stop = gpio_input_bit_read(&GPI_EmStop_App);

	input->m_in_data.bit.ac_lost = gpio_input_bit_read(&GPI_AC_lost_App);

	input->m_in_data.bit.dc_lost = gpio_input_bit_read(&GPI_Lost_5V_App);

	input->m_in_data.bit.panel = gpio_input_bit_read(&gpio_rs485_panel_detect);

	uint32_t in_mask;
	uint32_t out_mask;
	for(int i = 0; i < DIGITAL_INPUT_COUNT; i++) {
		in_mask = (1 << input->p_select[i]);
		out_mask = (1 << i);
		if(input->m_in_data.all & in_mask) {
			//сбросим счетчик сброса О_о
			input->m_cnt_reset[i] = input->p_t_reset[i];
			//проверим счетчик установки
			if(input->m_cnt_set[i]) {
				input->m_cnt_set[i]--;
			} else {
				if (input->p_invert[i] == 0x1) {
					input->m_out_data &= ~out_mask;
				} else {
					input->m_out_data |= out_mask;
				}
			}
		} else {
			//сбросим счетчик установки
			input->m_cnt_set[i] = input->p_t_set[i];
			//проверим счетчик сброса
			if(input->m_cnt_reset[i]) {
				input->m_cnt_reset[i]--;
			} else {
				if(input->p_invert[i] == 0x1) {
					input->m_out_data |= out_mask;
				} else {
					input->m_out_data &= ~out_mask;
				}
			}
		}
	}
}

/*
 * Алгоритм работы:
 * 1. Считаем данные от MSDI, если они валидны, то заполним входные данные от MSDI,
 * иначе, если счетчик невалидности данных от MSDI не равен нулю, то уменьшим его на единицу,
 * иначе очистим данные от MSDI.
 * 2. Считаем данные от остальных дискретных входов.
 * 3. Для каждого дискретного входа, согласно настройкам,
 * проверим его состояние и установим или сбросим соответствующий бит выхода,
 * учитывая задержки установки и сброса бита выхода.
 */
METHOD_CALC_IMPL(M_digital_input, input)
{
	digital_input_control_handler(input);

	CALC(msdi);

	//Проверим валидность данных от MSDI
	if(msdi.status & MSDI_STATUS_VALID) {
		//сбросим счетчик
		input->m_cnt_msdi_invalid = input->p_t_msdi_invalid;
		//заполним данные от MSDI
		input->m_in_data.bit.msdi = msdi.out_digital;
		//данные входов валидны
		input->status |= DIGITAL_INPUT_STATUS_VALID;
	} else {
		if(input->m_cnt_msdi_invalid) {
			//начнем отсчет
			input->m_cnt_msdi_invalid--;
		} else {
			//очистим данные от MSDI
			input->m_in_data.bit.msdi = 0;
			//данные входов не валидны
			input->status &= ~DIGITAL_INPUT_STATUS_VALID;
		}
	}

	if((input->status & (DIGITAL_INPUT_STATUS_READY | DIGITAL_INPUT_STATUS_RUN | DIGITAL_INPUT_STATUS_VALID)) ==
			(DIGITAL_INPUT_STATUS_READY | DIGITAL_INPUT_STATUS_RUN | DIGITAL_INPUT_STATUS_VALID)) {
		digital_input_calc(input);
		input->out_data = input->m_out_data;
	} else {
		input->out_data = 0;
	}
}
