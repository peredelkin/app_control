#ifndef SETTINGS_H
#define SETTINGS_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Settings_Control {
	SETTINGS_CONTROL_NONE = CONTROL_NONE,
	SETTINGS_CONTROL_RESET = CONTROL_RESET,
	SETTINGS_CONTROL_ENABLE = CONTROL_ENABLE,
	SETTINGS_CONTROL_START = CONTROL_START,
	SETTINGS_CONTROL_STOP =  CONTROL_STOP,
	SETTINGS_CONTROL_READ = (CONTROL_USER << 0),
	SETTINGS_CONTROL_WRITE = (CONTROL_USER << 1)
};

//! Перечисление возможных бит статуса.
enum _E_Settings_Status {
    SETTINGS_STATUS_NONE = STATUS_NONE,
	SETTINGS_STATUS_READY = STATUS_READY,
	SETTINGS_STATUS_VALID = STATUS_VALID,
	SETTINGS_STATUS_RUN = STATUS_RUN,
	SETTINGS_STATUS_ERROR = STATUS_ERROR,
	SETTINGS_STATUS_WARNING = STATUS_WARNING,
	SETTINGS_STATUS_READ_DONE = (STATUS_USER << 0),
	SETTINGS_STATUS_WRITE_DONE = (STATUS_USER << 1)
};

//! Предварительная декларация типа модуля.
typedef struct _S_Settings M_settings;

//! Структура модуля.
struct _S_Settings {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_settings);
    METHOD_DEINIT(M_settings);
    METHOD_CALC(M_settings);
    // Коллбэки.
    // Внутренние данные.
    reg_t* m_reg_fisrt;
    reg_t* m_reg_current;
    reg_t* m_reg_end;
};

EXTERN METHOD_INIT_PROTO(M_settings);
EXTERN METHOD_DEINIT_PROTO(M_settings);
EXTERN METHOD_CALC_PROTO(M_settings);

#define SETTINGS_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_settings),\
        METHOD_DEINIT_PTR(M_settings),\
        METHOD_CALC_PTR(M_settings),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* SETTINGS_H */
