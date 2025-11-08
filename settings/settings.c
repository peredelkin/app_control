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

//! Функция чтения очередной линии файла.
static char* ini_get_line(char* line, int num, void* fd)
{
	int nc = 0;
	int len = num;
	char *p = line;
	char s;
	int rc;

	/* Byte-by-byte read without any conversion (ANSI/OEM API) */
	len -= 1;	/* Make a room for the terminator */
	while (nc < len) {
		rc = yaffs_read(*((int*)fd), &s, 1); /* Get a byte */
		if (rc != 1) break;		/* EOF? */
		//if (s == '\r') continue; //Enable with LF - CRLF conversion.
		*p++ = s; nc++;
		if (s == '\n') break;
	}

	*p = 0;		/* Terminate the string */
	return nc ? line : 0;	/* When no data read due to EOF or error, return with error. */
}

//! Функция записи очередной линии файла.
static int ini_put_line(char* line, void* fd)
{
	size_t len = strlen(line);
	line[len] = '\n';
	return yaffs_write(*((int*)fd), line, len);
}

//! Функция установки на начало файла.
static void ini_rewind(void* fd)
{
	yaffs_lseek(*((int*)fd), 0, SEEK_SET);
}

void settings_set_read_error(M_settings* settings) {
	settings->status |= (SETTINGS_STATUS_ERROR | SETTINGS_STATUS_READ_DONE);
}

void settings_set_read_done(M_settings* settings) {
	settings->status |= (SETTINGS_STATUS_VALID | SETTINGS_STATUS_READ_DONE);
}

void settings_set_write_error(M_settings* settings) {
	settings->status |= (SETTINGS_STATUS_ERROR | SETTINGS_STATUS_WRITE_DONE);
}

void settings_set_write_done(M_settings* settings) {
	settings->status |= (SETTINGS_STATUS_VALID | SETTINGS_STATUS_WRITE_DONE);
}

void settings_negs_next(M_settings* settings) {
	settings->m_reg_current = regs_next(settings->m_reg_current);
}

void settings_read_conf(M_settings* settings) {

}

void settings_write_conf(M_settings* settings) {

}

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
					//читаем ini файл
					settings_read_conf(settings);
				} else {
					//установим следующий регистр
					settings_negs_next(settings);
				}
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
		settings->status &= ~(SETTINGS_STATUS_VALID | SETTINGS_STATUS_ERROR | SETTINGS_STATUS_WARNING
				| SETTINGS_STATUS_READ_DONE);
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
					//пишем ini файл
					settings_write_conf(settings);
				} else {
					//установим следующий регистр
					settings_negs_next(settings);
				}
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
		settings->status &= ~(SETTINGS_STATUS_VALID | SETTINGS_STATUS_ERROR | SETTINGS_STATUS_WARNING
				| SETTINGS_STATUS_WRITE_DONE);
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

	ini_init_t init;

	//buf
	init.line = NULL;
	init.line_size = 0;

	//i/o
	init.get_line = ini_get_line;
	init.put_line = ini_put_line;
	init.rewind = ini_rewind;

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
