#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include <assert.h>
#include "module/base.h"

#define DIGITAL_INPUT_COUNT 32

typedef struct _digital_input_in_bit {
	unsigned const_0 :1;
	unsigned const_1 :1;
	unsigned msdi :10;
	unsigned em_stop :1;
	unsigned ac_lost :1;
	unsigned dc_lost :1;
	unsigned panel :1;
	unsigned reserved :16;
} _digital_input_in_bit_t;
static_assert(sizeof(_digital_input_in_bit_t) == 4, "Invalid size of _digital_input_in_bit_t!");

typedef union _digital_input_in_reg {
	uint32_t all;
	struct _digital_input_in_bit bit;
} _digital_input_in_reg_t;

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
    // Выходные данные.
    volatile reg_u32_t out_data;
    // Параметры.
    reg_u32_t p_invert;
    reg_u8_t p_select[DIGITAL_INPUT_COUNT];
    // Регистры.
    // Методы.
    METHOD_INIT(M_digital_input);
    METHOD_DEINIT(M_digital_input);
    METHOD_CALC(M_digital_input);
    // Коллбэки.
    // Внутренние данные.
    _digital_input_in_reg_t m_in_data;
};

EXTERN METHOD_INIT_PROTO(M_digital_input);
EXTERN METHOD_DEINIT_PROTO(M_digital_input);
EXTERN METHOD_CALC_PROTO(M_digital_input);

#define DIGITAL_INPUT_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
		0,\
        /* Параметры */\
		0,\
		{0},\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_digital_input),\
        METHOD_DEINIT_PTR(M_digital_input),\
        METHOD_CALC_PTR(M_digital_input),\
        /* Коллбэки */\
        /* Внутренние данные */\
		{0},\
    }

#endif /* DIGITAL_INPUT_H */
