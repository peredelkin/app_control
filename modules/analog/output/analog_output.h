#ifndef ANALOG_OUTPUT_H
#define ANALOG_OUTPUT_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Analog_Output_Control {
    ANALOG_OUTPUT_CONTROL_NONE = CONTROL_NONE,
	ANALOG_OUTPUT_CONTROL_RESET = CONTROL_RESET,
	ANALOG_OUTPUT_CONTROL_ENABLE = CONTROL_ENABLE,
	ANALOG_OUTPUT_CONTROL_START = CONTROL_START,
	ANALOG_OUTPUT_CONTROL_STOP = CONTROL_STOP
};

//! Перечисление возможных бит статуса.
enum _E_Analog_Output_Status {
    ANALOG_OUTPUT_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Analog_Output M_analog_output;

//! Структура модуля.
struct _S_Analog_Output {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_analog_output);
    METHOD_DEINIT(M_analog_output);
    METHOD_CALC(M_analog_output);
    // Коллбэки.
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_analog_output);
EXTERN METHOD_DEINIT_PROTO(M_analog_output);
EXTERN METHOD_CALC_PROTO(M_analog_output);

#define ANALOG_OUTPUT_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_analog_output),\
        METHOD_DEINIT_PTR(M_analog_output),\
        METHOD_CALC_PTR(M_analog_output),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* ANALOG_OUTPUT_H */
