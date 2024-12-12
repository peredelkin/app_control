#ifndef RELAY_OUTPUT_H
#define RELAY_OUTPUT_H

#include "module/base.h"
#include "gpio/gpio.h"

typedef struct _relay_output_bit {
	unsigned relay_1 :1;
	unsigned relay_2 :1;
	unsigned relay_3 :1;
	unsigned relay_4 :1;
	unsigned reserved :28;
} _relay_output_bit_t;
static_assert(sizeof(_relay_output_bit_t) == 4, "Invalid size of _digital_input_bit_t!");

typedef union _relay_output_reg {
	uint32_t all;
	struct _relay_output_bit bit;
} _relay_output_reg_t;

//! Перечисление возможных бит управления.
enum _E_Relay_Output_Control {
    RELAY_OUTPUT_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Relay_Output_Status {
    RELAY_OUTPUT_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Relay_Output M_relay_output;

//! Структура модуля.
struct _S_Relay_Output {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    _relay_output_reg_t in;
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_relay_output);
    METHOD_DEINIT(M_relay_output);
    METHOD_CALC(M_relay_output);
    // Коллбэки.
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_relay_output);
EXTERN METHOD_DEINIT_PROTO(M_relay_output);
EXTERN METHOD_CALC_PROTO(M_relay_output);

#define RELAY_OUTPUT_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
		{0},\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_relay_output),\
        METHOD_DEINIT_PTR(M_relay_output),\
        METHOD_CALC_PTR(M_relay_output),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* RELAY_OUTPUT_H */
