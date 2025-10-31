#include "settings.h"
#include "reg/reg.h"
#include "lib/errors/errors.h"

void settings_reset(M_settings* settings) {
	settings->m_reg_fisrt = regs_first();
	settings->m_reg_current = NULL;
	settings->m_reg_end = regs_end();
}

void settings_read(M_settings* settings) {
	//указатель текущего регистра не установлен
	if(settings->m_reg_current == NULL) {
		//установим статус RUN
		settings->status |= SETTINGS_STATUS_RUN;
		//сбросим статусы VALID, ERROR, WARNING, READ_DONE
		settings->status &= ~(SETTINGS_STATUS_VALID |
				SETTINGS_STATUS_ERROR |
				SETTINGS_STATUS_WARNING |
				SETTINGS_STATUS_READ_DONE);
		//установим указатель текущего регистра
		settings->m_reg_current = settings->m_reg_fisrt;
	} else {

	}
}

void settings_write(M_settings* settings) {
	//указатель текущего регистра не установлен
	if(settings->m_reg_current == NULL) {
		//установим статус RUN
		settings->status |= SETTINGS_STATUS_RUN;
		//сбросим статусы VALID, ERROR, WARNING, READ_DONE
		settings->status &= ~(SETTINGS_STATUS_VALID |
				SETTINGS_STATUS_ERROR |
				SETTINGS_STATUS_WARNING |
				SETTINGS_STATUS_WRITE_DONE);
		//установим указатель текущего регистра
		settings->m_reg_current = settings->m_reg_fisrt;
	} else {

	}
}

METHOD_INIT_IMPL(M_settings, settings)
{
	settings->status = SETTINGS_STATUS_NONE;
	settings->control = SETTINGS_CONTROL_RESET;
}

METHOD_DEINIT_IMPL(M_settings, settings)
{

}

METHOD_CALC_IMPL(M_settings, settings)
{
	if (settings->control & SETTINGS_CONTROL_RESET) {
		settings_reset(settings);
	} else if ((settings->control
			& (SETTINGS_CONTROL_ENABLE | SETTINGS_CONTROL_START))
			== (SETTINGS_CONTROL_ENABLE | SETTINGS_CONTROL_START)) {

		if((settings->control & SETTINGS_CONTROL_READ) &&
				!(settings->control & SETTINGS_CONTROL_WRITE)) {
			settings_read(settings);
		}

		if ((settings->control & SETTINGS_CONTROL_WRITE) &&
				!(settings->control & SETTINGS_CONTROL_READ)) {
			settings_write(settings);
		}
	}
}
