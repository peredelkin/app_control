#include "settings.h"
#include "reg/reg.h"
#include "lib/errors/errors.h"
#include "fmc/yaffs2/yaffsfs.h"
#include "sys/counter/sys_counter.h"

char* settings_filename = "/nand/settings";
int settings_fd;

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
void settings_on_section (M_settings* settings, const char* section) {
	char* end_val = NULL;

	settings->m_buf.id = strtoul(section, &end_val, 0);
	if(end_val == NULL ||  *end_val != '\0') settings->m_buf.id = 0;
}
//! Функция пары "ключ-значение".
void settings_on_keyvalue(M_settings* settings, const char *key, const char *value) {
	char *end_val = NULL;

	if (strcmp(key, "data") == 0) {
		settings->m_buf.data = strtol(value, &end_val, 0);
		if (end_val == NULL || *end_val != '\0') settings->m_buf.data = 0;
	}

	if (strcmp(key, "type") == 0) {
		settings->m_buf.type = strtoul(value, &end_val, 0);
		if (end_val == NULL || *end_val != '\0') settings->m_buf.type = 0;
	}

	if (strcmp(key, "size") == 0) {
		settings->m_buf.size = strtoul(value, &end_val, 0);
		if (end_val == NULL || *end_val != '\0') settings->m_buf.size = 0;
	}
}
//! Функция ошибки.
void settings_on_error (ini_error_t error, size_t line, size_t pos, const char* line_str) {
	printf("Parse error %d at line %d: %s\n", (int)error, (int)line, line_str);
	sys_counter_delay(0, 10000); //10ms
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
							SETTINGS_STATUS_READ_DONE |
							SETTINGS_STATUS_WRITE_DONE);
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
							SETTINGS_STATUS_WRITE_DONE |
							SETTINGS_STATUS_READ_DONE);
}

//! Функция перехода к следующему регистру
void settings_regs_next(M_settings* settings) {
	settings->m_reg_current = regs_next(settings->m_reg_current);
}

//! Функция чтения блока параметров
void settings_read_conf(M_settings* settings) {

    ini_error_t ini_err = INI_ERROR_NONE;
    ini_expr_type_t type = INI_EXPR_EMPTY;
    char* section = NULL;
    char* key = NULL;
    char* value = NULL;
    char* err_pos = NULL;

    int keyvalue_count = 3;

    while(settings_get_line(settings->m_ini.line, settings->m_ini.line_size, settings->m_ini.stream)){
        ini_err = ini_parse_line(settings->m_ini.line, &type, &section, &key, &value, &err_pos);
        if(ini_err == INI_ERROR_NONE){
            switch(type){
            default:
                break;
            case INI_EXPR_SECTION:
            	settings_on_section(settings, section);
                break;
            case INI_EXPR_KEYVALUE:
            	settings_on_keyvalue(settings, key, value);
            	keyvalue_count--;
            	//если последний keyvalue
            	if (keyvalue_count == 0) {
					unsigned int cur_reg_size = reg_data_size(settings->m_reg_current);

					if ((settings->m_buf.id == reg_id(settings->m_reg_current)) &&
						(settings->m_buf.type == reg_type(settings->m_reg_current)) &&
						(settings->m_buf.size == cur_reg_size)) {
						memcpy(settings->m_reg_current->data, &settings->m_buf.data, cur_reg_size);
					} else {
						settings_set_read_error(settings);
					}

					return;
				}

                break;
            }
        } else {
        	settings_set_read_error(settings);
        	return;
        }
    }
}

//! Функция записи блока параметров
void settings_write_conf(M_settings* settings) {
	err_t err = E_NO_ERROR;

	memset(settings->m_str.id, 0, SETTINGS_STR_VAL_SIZE);
	memset(settings->m_str.data, 0, SETTINGS_STR_VAL_SIZE);
	memset(settings->m_str.type, 0, SETTINGS_STR_VAL_SIZE);
	memset(settings->m_str.size, 0, SETTINGS_STR_VAL_SIZE);

	snprintf(settings->m_str.id, SETTINGS_STR_VAL_SIZE, "%#08x", (unsigned int)(reg_id(settings->m_reg_current)));
	snprintf(settings->m_str.data, SETTINGS_STR_VAL_SIZE, "%i", (int)(reg_valuel(settings->m_reg_current)));
	snprintf(settings->m_str.type, SETTINGS_STR_VAL_SIZE, "%u", reg_type(settings->m_reg_current));
	snprintf(settings->m_str.size, SETTINGS_STR_VAL_SIZE, "%u", reg_data_size(settings->m_reg_current));

	err = ini_write_section(&settings->m_ini, settings->m_str.id);
	if(err != E_NO_ERROR) {
		settings_set_write_error(settings);
		return;
	}

	err = ini_write_keyvalue(&settings->m_ini, "data", settings->m_str.data);
	if(err != E_NO_ERROR) {
		settings_set_write_error(settings);
		return;
	}

	err = ini_write_keyvalue(&settings->m_ini, "type", settings->m_str.type);
	if(err != E_NO_ERROR) {
		settings_set_write_error(settings);
		return;
	}

	err = ini_write_keyvalue(&settings->m_ini, "size", settings->m_str.size);
	if(err != E_NO_ERROR) {
		settings_set_write_error(settings);
		return;
	}
}

void settings_cmd_read(M_settings* settings) {
	settings->control |= (SETTINGS_CONTROL_ENABLE | SETTINGS_CONTROL_START | SETTINGS_CONTROL_READ);
}

void settings_cmd_write(M_settings* settings) {
	settings->control |= (SETTINGS_CONTROL_ENABLE | SETTINGS_CONTROL_START | SETTINGS_CONTROL_WRITE);
}

void settings_reset(M_settings *settings) {
	//сбросим статус READY
	settings->status &= ~SETTINGS_STATUS_READY;
	//сбросим control
	settings->control = SETTINGS_CONTROL_NONE;
	//установим статус READY
	settings->status |= SETTINGS_STATUS_READY;
}

void settings_read(M_settings *settings) {
	//если задание выполняется
	if (settings->status & SETTINGS_STATUS_RUN) {
		//если задание выполнено
		if (settings->status & SETTINGS_STATUS_READ_DONE) {
			//сбросим управляющие биты
			settings->control &= ~(SETTINGS_CONTROL_ENABLE | SETTINGS_CONTROL_START | SETTINGS_CONTROL_READ);
			//сбросим указатель на регистр
			settings->m_reg_current = NULL;
			//закроем файл
			yaffs_close(settings_fd);
			//сбросим статус RUN
			settings->status &= ~SETTINGS_STATUS_RUN;
			//установим статус READY
			settings->status |= SETTINGS_STATUS_READY;
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
				/**
				 * если в ходе выполнения чтения был установлен статус READ DONE,
				 * пропустим инкремент указателя регистра,
				 * для корректной установки статуса.
				 */
				if (!(settings->status & SETTINGS_STATUS_READ_DONE)) {
					//установим следующий регистр
					settings_regs_next(settings);
					//если регистр был последним
					if(settings->m_reg_current == NULL) {
						//установим статусы VALID, READ_DONE
						settings_set_read_done(settings);
					}
				}
			}
		}
	} else {
		//сбросим статус READY
		settings->status &= ~SETTINGS_STATUS_READY;
		//установим статус RUN
		settings->status |= SETTINGS_STATUS_RUN;
		//сбросим статусы VALID, ERROR, WARNING, READ_DONE
		settings_reset_read_status(settings);
		//установим указатель текущего регистра
		settings->m_reg_current = regs_first();
		//откроем файл для чтения
		settings_fd = yaffs_open(settings_filename, SETTINGS_O_RFLAG, SETTINGS_S_RMODE);
		//если произошла ошибка, завершим работу с файлом
		if (settings_fd < 0) {
			//установим статусы ERROR, READ_DONE
			settings_set_read_error(settings);
		} else {
			//установим поток ini
			ini_set_stream(&settings->m_ini, &settings_fd);
		}
	}
}

void settings_write(M_settings *settings) {
	//если задание выполняется
	if (settings->status & SETTINGS_STATUS_RUN) {
		//если задание выполнено
		if (settings->status & SETTINGS_STATUS_WRITE_DONE) {
			//сбросим управляющие биты
			settings->control &= ~(SETTINGS_CONTROL_ENABLE | SETTINGS_CONTROL_START | SETTINGS_CONTROL_WRITE);
			//сбросим указатель на регистр
			settings->m_reg_current = NULL;
			//закроем файл
			yaffs_close(settings_fd);
			//сбросим статус RUN
			settings->status &= ~SETTINGS_STATUS_RUN;
			//установим статус READY
			settings->status |= SETTINGS_STATUS_READY;
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
				/**
				 * Если в ходе выполнения записи был установлен WRITE DONE,
				 * пропустим инкремент указателя регистра,
				 * для корректной установки статуса
				 */
				if (!(settings->status & SETTINGS_STATUS_WRITE_DONE)) {
					//установим следующий регистр
					settings_regs_next(settings);
					//если регистр был последним
					if (settings->m_reg_current == NULL) {
						//установим статусы VALID, WRITE_DONE
						settings_set_write_done(settings);
					}
				}
			}
		}
	} else {
		//сбросим статус READY
		settings->status &= ~SETTINGS_STATUS_READY;
		//установим статус RUN
		settings->status |= SETTINGS_STATUS_RUN;
		//сбросим статусы VALID, ERROR, WARNING, WRITE_DONE
		settings_reset_write_status(settings);
		//установим указатель текущего регистра
		settings->m_reg_current = regs_first();
		//откроем файл для чтения
		settings_fd = yaffs_open(settings_filename, SETTINGS_O_WFLAG, SETTINGS_S_WMODE);
		//если произошла ошибка, завершим работу с файлом
		if (settings_fd < 0) {
			//установим статусы ERROR, WRITE_DONE
			settings_set_write_error(settings);
		} else {
			//установим поток ini
			ini_set_stream(&settings->m_ini, &settings_fd);
		}
	}
}

METHOD_INIT_IMPL(M_settings, settings)
{
	settings->status = SETTINGS_STATUS_NONE;
	settings->control = SETTINGS_CONTROL_NONE;

	settings->m_reg_current = NULL;

	settings_fd = -1;

	ini_init_t init;

	//buf
	init.line = settings->m_str.buf;
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

METHOD_IDLE_IMPL(M_settings, settings)
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
