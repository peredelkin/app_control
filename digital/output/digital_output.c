#include "gpio/init/gpio_init.h"
#include "digital_output.h"
#include "modules/modules.h"

static status_t settings_status_mask;
static status_t settings_status_masked;

METHOD_INIT_IMPL(M_digital_output, output)
{
	output->status = DIGITAL_OUTPUT_STATUS_NONE;
	output->control = DIGITAL_OUTPUT_CONTROL_NONE;

	INIT(panel_led);
    INIT(do_ncv7608);
    INIT(do_relay);

    output->status |= DIGITAL_OUTPUT_STATUS_READY;
    //TODO: нужно будет проверять в обработчике статус MC. до тех пор STATUS_VALID
    output->status |= DIGITAL_OUTPUT_STATUS_VALID;
}

METHOD_DEINIT_IMPL(M_digital_output, output)
{
	DEINIT(panel_led);
    DEINIT(do_ncv7608);
    DEINIT(do_relay);
}

/*
 * Логика работы дискретных выходов:
 * 1.Если вход, выбранный в настройках, активен, то запускается счетчик установки бита выхода.
 * Если счетчик не равен нулю, то он декрементируется.
 * Если счетчик равен нулю, то бит выхода устанавливается в соответствии с настройкой инверсии.
 * 2.Если вход, выбранный в настройках, неактивен, то запускается счетчик сброса бита выхода.
 * Если счетчик не равен нулю, то он декрементируется.
 * Если счетчик равен нулю, то бит выхода сбрасывается в соответствии с настройкой инверсии.
 *
 * Счетчики установки и сброса нужны для реализации задержки установки/сброса бита выхода
 * после изменения состояния входа.
 *
 * Дискретные выходы разбиты на 2 группы: NCV7608 и реле.
 * Это сделано для удобства настройки и управления ими.
 *
 * Данные для NCV7608 и реле заполняются в структуре m_out_data
 * и передаются в соответствующие модули для управления выходами.
 */
METHOD_CALC_IMPL(M_digital_output, output)
{
	uint32_t in_mask;
	uint32_t out_mask;
	for(int i = 0; i < (DIGITAL_INPUT_COUNT - 1); i++) {
		in_mask = (1 << output->p_select[i]);
		out_mask = (1 << i);
		if(output->in_data & in_mask) {
			//сбросим счетчик сброса О_о
			output->m_cnt_reset[i] = output->p_t_reset[i];
			//проверим счетчик установки
			if (output->m_cnt_set[i]) {
				output->m_cnt_set[i]--;
			} else {
				if(output->p_invert[i] == 0x1) {
					output->m_out_data.all &= ~out_mask;
				} else {
					output->m_out_data.all |= out_mask;
				}
			}
		} else {
			//сбросим счетчик установки
			output->m_cnt_set[i] = output->p_t_set[i];
			//проверим счетчик сброса
			if(output->m_cnt_reset[i]) {
				output->m_cnt_reset[i]--;
			} else {
				if(output->p_invert[i] == 0x1) {
					output->m_out_data.all |= out_mask;
				} else {
					output->m_out_data.all &= ~out_mask;
				}
			}
		}
	}

	settings_status_masked = settings.status & ~settings_status_mask;

	//настройки прочитаны
	if(settings_status_masked & SETTINGS_STATUS_READ_DONE) {
		if(settings_status_masked & SETTINGS_STATUS_VALID) {
			settings_status_mask |= SETTINGS_STATUS_VALID;
			settings_status_mask &= ~SETTINGS_STATUS_ERROR;
			output->status |= DIGITAL_OUTPUT_STATUS_RUN;
		}

		if(settings_status_masked & SETTINGS_STATUS_ERROR) {
			settings_status_mask |= SETTINGS_STATUS_ERROR;
			settings_status_mask &= ~SETTINGS_STATUS_VALID;
			output->status &= ~DIGITAL_OUTPUT_STATUS_RUN;
		}
	}

	if((output->status & (DIGITAL_OUTPUT_STATUS_READY | DIGITAL_OUTPUT_STATUS_RUN | DIGITAL_OUTPUT_STATUS_VALID)) ==
			(DIGITAL_OUTPUT_STATUS_READY | DIGITAL_OUTPUT_STATUS_RUN | DIGITAL_OUTPUT_STATUS_VALID)) {
		do_ncv7608.in_data = output->m_out_data.bit.ncv;
		do_relay.in_data = output->m_out_data.bit.relay;
	} else {
		do_ncv7608.in_data = 0;
		do_relay.in_data = 0;
	}

	CALC(panel_led);
	CALC(do_ncv7608);
	CALC(do_relay);
}
