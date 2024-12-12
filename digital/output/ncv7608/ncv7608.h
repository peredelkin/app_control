#ifndef NCV7608_H
#define NCV7608_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Ncv7608_Control {
    NCV7608_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Ncv7608_Status {
    NCV7608_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Ncv7608 M_ncv7608;

//! Структура модуля.
struct _S_Ncv7608 {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    reg_u32_t in;
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_ncv7608);
    METHOD_DEINIT(M_ncv7608);
    METHOD_CALC(M_ncv7608);
    // Коллбэки.
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_ncv7608);
EXTERN METHOD_DEINIT_PROTO(M_ncv7608);
EXTERN METHOD_CALC_PROTO(M_ncv7608);

#define NCV7608_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
		0,\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_ncv7608),\
        METHOD_DEINIT_PTR(M_ncv7608),\
        METHOD_CALC_PTR(M_ncv7608),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* NCV7608_H */
