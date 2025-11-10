#include "settings.h"
#include "reg/reg.h"
#include "lib/errors/errors.h"
#include "yaffs2/yaffsfs.h"
#include "sys/counter/sys_counter.h"

char* settings_filename = "/nand/settings";
int settings_file;

#define SETTINGS_STR_SIZE 256
char settings_str_buffer[SETTINGS_STR_SIZE];

#define SETTINGS_O_RFLAG	O_RDONLY
#define SETTINGS_S_RMODE	(S_IRUSR | S_IWUSR)

#define SETTINGS_O_WFLAG	(O_CREAT | O_TRUNC | O_WRONLY)
#define SETTINGS_S_WMODE	(S_IWUSR | S_IRUSR)

//! Функция чтения очередной линии файла.
static char* settings_get_line(char* line, int num, void* fd)
{
	int nc = 0;
	int len = num;
	char *p = line;
	char s;
	int rc;

	len -= 1;
	while (nc < len) {
		rc = yaffs_read(*((int*)fd), &s, 1);
		if (rc != 1) break;
		*p++ = s; nc++;
		if (s == '\n') break;
	}

	*p = 0;
	return nc ? line : 0;
}

//! Функция записи очередной линии файла.
static int settings_put_line(char* line, void* fd)
{
	size_t len = strlen(line);
	return yaffs_write(*((int*)fd), line, len);
}

//! Функция установки на начало файла.
static void settings_rewind(void* fd)
{
	yaffs_lseek(*((int*)fd), 0, SEEK_SET);
}

//! Функция начала секции.
bool settings_on_section (const char* section) {

	return false;
}
//! Функция пары "ключ-значение".
bool settings_on_keyvalue (const char* key, const char* value) {

	return false;
}
//! Функция ошибки.
bool settings_on_error (ini_error_t error, size_t line, size_t pos, const char* line_str) {

	return false;
}

//! Функция установки статуса ошибки чтения
void settings_set_read_error(M_settings* settings) {
	settings->status |= (SETTINGS_STATUS_ERROR | SETTINGS_STATUS_READ_DONE);
}

//! Функция установки статуса чтения без ошибок
void settings_set_read_done(M_settings* settings) {
	settings->status |= (SETTINGS_STATUS_VALID | SETTINGS_STATUS_READ_DONE);
}

//! Функция сброса статусов чтения
void settings_reset_read_status(M_settings* settings) {
	settings->status &= ~(	SETTINGS_STATUS_VALID |
							SETTINGS_STATUS_ERROR |
							SETTINGS_STATUS_WARNING |
							SETTINGS_STATUS_READ_DONE);
}

//! Функция установки статуса ошибки записи
void settings_set_write_error(M_settings* settings) {
	settings->status |= (SETTINGS_STATUS_ERROR | SETTINGS_STATUS_WRITE_DONE);
}

//! Функция установки статуса записи без ошибок
void settings_set_write_done(M_settings* settings) {
	settings->status |= (SETTINGS_STATUS_VALID | SETTINGS_STATUS_WRITE_DONE);
}

//! Функция сброса статусов записи
void settings_reset_write_status(M_settings* settings) {
	settings->status &= ~(	SETTINGS_STATUS_VALID |
							SETTINGS_STATUS_ERROR |
							SETTINGS_STATUS_WARNING |
							SETTINGS_STATUS_WRITE_DONE);
}

//! Функция перехода к следующему регистру
void settings_negs_next(M_settings* settings) {
	settings->m_reg_current = regs_next(settings->m_reg_current);
}

//! Функция чтения блока параметров
void settings_read_conf(M_settings* settings) {
//	if (ini_parse(&settings->m_ini) != E_NO_ERROR) {
//		settings_set_read_error(settings);
//	}
}

//! Функция записи блока параметров
void settings_write_conf(M_settings* settings) {
	err_t err = E_NO_ERROR;

	memset(settings->m_regs_id, 0, SETTINGS_STR_VAL_SIZE);
	memset(settings->m_regs_data, 0, SETTINGS_STR_VAL_SIZE);
	memset(settings->m_regs_type, 0, SETTINGS_STR_VAL_SIZE);
	memset(settings->m_regs_size, 0, SETTINGS_STR_VAL_SIZE);

	snprintf(settings->m_regs_id, SETTINGS_STR_VAL_SIZE, "%u", settings->m_reg_current->id);
	snprintf(settings->m_regs_data, SETTINGS_STR_VAL_SIZE, "%i", reg_valuel(settings->m_reg_current));
	snprintf(settings->m_regs_type, SETTINGS_STR_VAL_SIZE, "%u", settings->m_reg_current->type);
	snprintf(settings->m_regs_size, SETTINGS_STR_VAL_SIZE, "%u", reg_data_size(settings->m_reg_current));

	err = ini_write_section(&settings->m_ini, settings->m_regs_id);
	if(err != E_NO_ERROR) {
		settings_set_write_error(settings);
		return;
	}

	err = ini_write_keyvalue(&settings->m_ini, "data", settings->m_regs_data);
	if(err != E_NO_ERROR) {
		settings_set_write_error(settings);
		return;
	}

	err = ini_write_keyvalue(&settings->m_ini, "type", settings->m_regs_type);
	if(err != E_NO_ERROR) {
		settings_set_write_error(settings);
		return;
	}

	err = ini_write_keyvalue(&settings->m_ini, "size", settings->m_regs_size);
	if(err != E_NO_ERROR) {
		settings_set_write_error(settings);
		return;
	}
}

//! Функция сброса модуля
void settings_reset(M_settings* settings) {
	settings->status = SETTINGS_STATUS_NONE; //Reset All Status
	settings->control = SETTINGS_CONTROL_NONE; //Reset All Control

	settings->m_reg_current = NULL;
	settings_file = -1;

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
			yaffs_close(settings_file);
			//сбросим статус RUN
			settings->status &= ~SETTINGS_STATUS_RUN;
		} else {
			//если указатель на регистр NULL
			if (settings->m_reg_current == NULL) {
				//установим статусы ERROR, READ_DONE
				settings_set_read_error(settings);
			} else {
				//если регистр имеет флаг настройки
				if (settings->m_reg_current->flags & REG_FLAG_CONF) {
					//парсим блок ini файла
					settings_read_conf(settings);
				}
				//установим следующий регистр
				settings_negs_next(settings);
				//если регистр был последним
				if(settings->m_reg_current == NULL) {
					//установим статусы VALID, READ_DONE
					settings_set_read_done(settings);
				}
			}
		}
	} else {
		//установим статус RUN
		settings->status |= SETTINGS_STATUS_RUN;
		//сбросим статусы VALID, ERROR, WARNING, READ_DONE
		settings_reset_read_status(settings);
		//установим указатель текущего регистра
		settings->m_reg_current = settings->m_reg_fisrt;
		//откроем файл для чтения
		settings_file = yaffs_open(settings_filename, SETTINGS_O_RFLAG, SETTINGS_S_RMODE);
		//если произошла ошибка, завершим работу с файлом
		if (settings_file == -1) {
			//установим статусы ERROR, READ_DONE
			settings_set_read_error(settings);
		} else {
			//установим поток ini
			ini_set_stream(&settings->m_ini, &settings_file);
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
			yaffs_close(settings_file);
			//сбросим статус RUN
			settings->status &= ~SETTINGS_STATUS_RUN;
		} else {
			//если указатель на регистр NULL
			if (settings->m_reg_current == NULL) {
				//установим статусы ERROR, WRITE_DONE
				settings_set_write_error(settings);
			} else {
				//если регистр имеет флаг настройки
				if (settings->m_reg_current->flags & REG_FLAG_CONF) {
					//запишем блок в ini файл
					settings_write_conf(settings);
				}
				//установим следующий регистр
				settings_negs_next(settings);
				//если регистр был последним
				if(settings->m_reg_current == NULL) {
					//установим статусы VALID, WRITE_DONE
					settings_set_write_done(settings);
				}
			}
		}
	} else {
		//установим статус RUN
		settings->status |= SETTINGS_STATUS_RUN;
		//сбросим статусы VALID, ERROR, WARNING, WRITE_DONE
		settings_reset_write_status(settings);
		//установим указатель текущего регистра
		settings->m_reg_current = settings->m_reg_fisrt;
		//откроем файл для чтения
		settings_file = yaffs_open(settings_filename, SETTINGS_O_WFLAG, SETTINGS_S_WMODE);
		//если произошла ошибка, завершим работу с файлом
		if (settings_file == -1) {
			//установим статусы ERROR, WRITE_DONE
			settings_set_write_error(settings);
		} else {
			//установим поток ini
			ini_set_stream(&settings->m_ini, &settings_file);
		}
	}
}

METHOD_INIT_IMPL(M_settings, settings)
{
	settings->status = SETTINGS_STATUS_NONE;
	settings->control = SETTINGS_CONTROL_RESET;

	settings->m_reg_fisrt = regs_first();
	settings->m_reg_end = regs_end();

	ini_init_t init;

	//buf
	init.line = settings_str_buffer;
	init.line_size = SETTINGS_STR_SIZE;

	//i/o
	init.get_line = settings_get_line;
	init.put_line = settings_put_line;
	init.rewind = settings_rewind;

	//callbacks
	init.on_section = NULL;
	init.on_keyvalue = NULL;
	init.on_error = NULL;

	ini_init(&settings->m_ini, &init);
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
