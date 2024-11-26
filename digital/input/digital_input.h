#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Digital_Input_Control {
    DIGITAL_INPUT_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Digital_Input_Status {
    DIGITAL_INPUT_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Digital_Input M_digital_input;

//! Структура модуля.
struct _S_Digital_Input {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    reg_u32_t msdi_in;
    // Выходные данные.
    volatile reg_u16_t out;
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_digital_input);
    METHOD_DEINIT(M_digital_input);
    METHOD_CALC(M_digital_input);
    // Коллбэки.
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_digital_input);
EXTERN METHOD_DEINIT_PROTO(M_digital_input);
EXTERN METHOD_CALC_PROTO(M_digital_input);

#define DIGITAL_INPUT_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
		0,\
        /* Выходные данные */\
		0,\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_digital_input),\
        METHOD_DEINIT_PTR(M_digital_input),\
        METHOD_CALC_PTR(M_digital_input),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* DIGITAL_INPUT_H */
