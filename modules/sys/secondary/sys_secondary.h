#ifndef SYS_SECONDARY_H
#define SYS_SECONDARY_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Sys_Secondary_Control {
    SYS_SECONDARY_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Sys_Secondary_Status {
    SYS_SECONDARY_STATUS_NONE = STATUS_NONE,
	SYS_SECONDARY_STATUS_ERROR = STATUS_ERROR,
};

//! Перечисление возможных бит ошибок.
enum _E_Sys_Secondary_Errors {
	SYS_SECONDARY_ERROR_NONE = ERROR_NONE, //!< Нет ошибок.
	SYS_SECONDARY_ERROR_INTERNAL = 0x01, //!< Внутренняя ошибка.
	SYS_SECONDARY_ERROR_HARDWARE = 0x02, //!< Ошибка аппаратной части.
	SYS_SECONDARY_ERROR_SOFTWARE = 0x04, //!< Ошибка программной части.
	SYS_SECONDARY_ERROR_PROT = 0x08, //!< Ошибка защит.
};

//! Перечисление возможных бит предупреждений.
enum _E_Sys_Secondary_Warnings {
	SYS_SECONDARY_WARNING_NONE = WARNING_NONE, //!< Нет предупреждений.
};

//! Перечисление возможных бит состояния.
enum _E_Sys_Secondary_State {
	SYS_SECONDARY_STATE_NONE = STATE_NONE,
	SYS_SECONDARY_STATE_INIT = STATE_INIT,
	SYS_SECONDARY_STATE_IDLE = STATE_IDLE,
	SYS_SECONDARY_STATE_READY = STATE_READY,
	SYS_SECONDARY_STATE_RUN = STATE_RUN,
	SYS_SECONDARY_STATE_ERROR = STATE_ERROR,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Sys_Secondary M_sys_secondary;

//! Структура модуля.
struct _S_Sys_Secondary {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    error_t errors; //!< Ошибки.
    warning_t warnings; //!< Предупреждения.
    state_t state; //!< Состояние.
    // Входные данные.
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_sys_secondary);
    METHOD_DEINIT(M_sys_secondary);
    METHOD_CALC(M_sys_secondary);
    METHOD_IDLE(M_sys_secondary);
    // Коллбэки.
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_sys_secondary);
EXTERN METHOD_DEINIT_PROTO(M_sys_secondary);
EXTERN METHOD_CALC_PROTO(M_sys_secondary);
EXTERN METHOD_IDLE_PROTO(M_sys_secondary);

#define SYS_SECONDARY_DEFAULTS {\
		0, /* control */\
		0, /* status */\
		0, /* errors */\
		0, /* warnings */\
		0, /* state */\
        METHOD_INIT_PTR(M_sys_secondary),\
        METHOD_DEINIT_PTR(M_sys_secondary),\
        METHOD_CALC_PTR(M_sys_secondary),\
		METHOD_IDLE_PTR(M_sys_secondary),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* SYS_SECONDARY_H */
