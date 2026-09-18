#ifndef SYS_MAIN_H
#define SYS_MAIN_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Sys_Main_Control {
    SYS_MAIN_CONTROL_NONE = CONTROL_NONE
};

//! Перечисление возможных бит статуса.
enum _E_Sys_Main_Status {
    SYS_MAIN_STATUS_NONE = STATUS_NONE,
    SYS_MAIN_STATUS_ERROR = STATUS_ERROR,
};

//! Перечисление возможных бит ошибок.
enum _E_Sys_Main_Errors {
    SYS_MAIN_ERROR_NONE = ERROR_NONE, //!< Нет ошибок.
    SYS_MAIN_ERROR_INTERNAL = 0x01, //!< Внутренняя ошибка.
    SYS_MAIN_ERROR_HARDWARE = 0x02, //!< Ошибка аппаратной части.
    SYS_MAIN_ERROR_SOFTWARE = 0x04, //!< Ошибка программной части.
    SYS_MAIN_ERROR_PROT = 0x08, //!< Ошибка защит.
};

//! Перечисление возможных бит предупреждений.
enum _E_Sys_Main_Warnings {
    SYS_MAIN_WARNING_NONE = WARNING_NONE, //!< Нет предупреждений.
};

//! Перечисление возможных бит состояния.
enum _E_Sys_Main_State {
    SYS_MAIN_STATE_NONE = STATE_NONE,
    SYS_MAIN_STATE_INIT = STATE_INIT,
    SYS_MAIN_STATE_IDLE = STATE_IDLE,
    SYS_MAIN_STATE_READY = STATE_READY,
    SYS_MAIN_STATE_RUN = STATE_RUN,
    SYS_MAIN_STATE_ERROR = STATE_ERROR,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Sys_Main M_sys_main;

//! Структура модуля.
struct _S_Sys_Main {
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
    METHOD_INIT(M_sys_main);
    METHOD_DEINIT(M_sys_main);
    METHOD_CALC(M_sys_main);
    METHOD_IDLE(M_sys_main);
    // Коллбэки.
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_sys_main);
EXTERN METHOD_DEINIT_PROTO(M_sys_main);
EXTERN METHOD_CALC_PROTO(M_sys_main);
EXTERN METHOD_IDLE_PROTO(M_sys_main);

#define SYS_MAIN_DEFAULTS {\
		0, /* control */\
		0, /* status */\
		0, /* errors */\
		0, /* warnings */\
		0, /* state */\
		METHOD_INIT_PTR(M_sys_main), \
		METHOD_DEINIT_PTR(M_sys_main),\
		METHOD_CALC_PTR(M_sys_main), \
		METHOD_IDLE_PTR(M_sys_main),\
	}

#endif /* SYS_MAIN_H */
