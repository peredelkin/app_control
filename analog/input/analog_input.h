#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Analog_Input_Control {
    ANALOG_INPUT_CONTROL_NONE = CONTROL_NONE,
	ANALOG_INPUT_CONTROL_RESET = CONTROL_RESET,
	ANALOG_INPUT_CONTROL_ENABLE = CONTROL_ENABLE,
	ANALOG_INPUT_CONTROL_START = CONTROL_START,
	ANALOG_INPUT_CONTROL_STOP = CONTROL_STOP
};

//! Перечисление возможных бит статуса.
enum _E_Analog_Input_Status {
    ANALOG_INPUT_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Analog_Input M_analog_input;

//! Структура модуля.
struct _S_Analog_Input {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_analog_input);
    METHOD_DEINIT(M_analog_input);
    METHOD_CALC(M_analog_input);
    // Коллбэки.
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_analog_input);
EXTERN METHOD_DEINIT_PROTO(M_analog_input);
EXTERN METHOD_CALC_PROTO(M_analog_input);

#define ANALOG_INPUT_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_analog_input),\
        METHOD_DEINIT_PTR(M_analog_input),\
        METHOD_CALC_PTR(M_analog_input),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* ANALOG_INPUT_H */
