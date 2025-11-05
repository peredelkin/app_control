#include "settings.h"
#include "reg/reg.h"
#include "lib/errors/errors.h"
#include "yaffs2/yaffsfs.h"
#include "sys/counter/sys_counter.h"

char* settings_filename = "/nand/settings.ini";
int settings_file;

#define SETTINGS_O_RFLAG	O_RDONLY
#define SETTINGS_S_RMODE	S_IRUSR

#define SETTINGS_O_WFLAG	(O_CREAT | O_TRUNC | O_WRONLY)
#define SETTINGS_S_WMODE	S_IWUSR

void settings_reset(M_settings* settings) {
	settings->status = SETTINGS_STATUS_NONE; //Reset All Status
	settings->control = SETTINGS_CONTROL_NONE; //Reset All Control

	settings->m_reg_fisrt = regs_first();
	settings->m_reg_current = NULL;
	settings->m_reg_end = regs_end();

	settings->status = SETTINGS_STATUS_READY; //Set Ready Status
}

void settings_read(M_settings *settings) {
	//если задание выполняется
	if (settings->status & SETTINGS_STATUS_RUN) {
		//если задание выполнено
		if (settings->status & SETTINGS_STATUS_READ_DONE) {
			//сбросим управляющие биты
			settings->control &= ~(SETTINGS_CONTROL_START | SETTINGS_CONTROL_READ);
			//сбросим указатель на регистр
			settings->m_reg_current = NULL;
			//закроем файл
			//yaffs_close(settings_file);
			//сбросим статус RUN
			settings->status &= ~SETTINGS_STATUS_RUN;
		} else {
			//если указатель на регистр NULL
			if (settings->m_reg_current == NULL) {
				//установим статусы ERROR, READ_DONE
				settings->status |= (SETTINGS_STATUS_ERROR | SETTINGS_STATUS_READ_DONE);
			} else {
				//если регистр имеет флаг настройки
				if (settings->m_reg_current->flags & REG_FLAG_CONF) {
					//парсим ini файл
				}
				//установим следующий регистр
				settings->m_reg_current = regs_next(settings->m_reg_current);
				//если регистр был последним
				if(settings->m_reg_current == NULL) {
					settings->status |= (SETTINGS_STATUS_VALID | SETTINGS_STATUS_READ_DONE);
				}
			}
		}
	} else {
		//установим статус RUN
		settings->status |= SETTINGS_STATUS_RUN;
		//сбросим статусы VALID, ERROR, WARNING, READ_DONE
		settings->status &= ~(SETTINGS_STATUS_VALID | SETTINGS_STATUS_ERROR | SETTINGS_STATUS_WARNING
				| SETTINGS_STATUS_READ_DONE);
		//установим указатель текущего регистра
		settings->m_reg_current = settings->m_reg_fisrt;
		//откроем файл для чтения
		//settings_file = yaffs_open(settings_filename, SETTINGS_O_RFLAG, SETTINGS_S_RMODE);
		//если произошла ошибка, завершим работу с файлом
		if (settings_file == -1) {
			//установим статусы ERROR, READ_DONE
			settings->status |= (SETTINGS_STATUS_ERROR | SETTINGS_STATUS_READ_DONE);
		}
	}
}

void settings_write(M_settings *settings) {
	//если задание выполняется
	if (settings->status & SETTINGS_STATUS_RUN) {
		//если задание выполнено
		if (settings->status & SETTINGS_STATUS_WRITE_DONE) {
			//сбросим управляющие биты
			settings->control &= ~(SETTINGS_CONTROL_START | SETTINGS_CONTROL_WRITE);
			//сбросим указатель на регистр
			settings->m_reg_current = NULL;
			//закроем файл
			//yaffs_close(settings_file);
			//сбросим статус RUN
			settings->status &= ~SETTINGS_STATUS_RUN;
		} else {
			//если указатель на регистр NULL
			if (settings->m_reg_current == NULL) {
				//установим статусы ERROR, WRITE_DONE
				settings->status |= (SETTINGS_STATUS_ERROR | SETTINGS_STATUS_WRITE_DONE);
			} else {
				//если регистр имеет флаг настройки
				if (settings->m_reg_current->flags & REG_FLAG_CONF) {
					//парсим ini файл
				}
				//установим следующий регистр
				settings->m_reg_current = regs_next(settings->m_reg_current);
				//если регистр был последним
				if(settings->m_reg_current == NULL) {
					settings->status |= (SETTINGS_STATUS_VALID | SETTINGS_STATUS_WRITE_DONE);
				}
			}
		}
	} else {
		//установим статус RUN
		settings->status |= SETTINGS_STATUS_RUN;
		//сбросим статусы VALID, ERROR, WARNING, WRITE_DONE
		settings->status &= ~(SETTINGS_STATUS_VALID | SETTINGS_STATUS_ERROR | SETTINGS_STATUS_WARNING
				| SETTINGS_STATUS_WRITE_DONE);
		//установим указатель текущего регистра
		settings->m_reg_current = settings->m_reg_fisrt;
		//откроем файл для чтения
		//settings_file = yaffs_open(settings_filename, SETTINGS_O_RFLAG, SETTINGS_S_RMODE);
		//если произошла ошибка, завершим работу с файлом
		if (settings_file == -1) {
			//установим статусы ERROR, WRITE_DONE
			settings->status |= (SETTINGS_STATUS_ERROR | SETTINGS_STATUS_WRITE_DONE);
		}
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
