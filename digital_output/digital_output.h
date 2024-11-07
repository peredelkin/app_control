#ifndef DIGITAL_OUTPUT_H
#define DIGITAL_OUTPUT_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Digital_Output_Control {
    DIGITAL_OUTPUT_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Digital_Output_Status {
    DIGITAL_OUTPUT_STATUS_NONE = STATUS_NONE,
};

#define DIGITAL_OUTPUT_BITS_COUNT 16

//! Предварительная декларация типа модуля.
typedef struct _S_Digital_Output M_digital_output;

//! Структура модуля.
struct _S_Digital_Output {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    reg_u16_t out;
    // Параметры.
    reg_id_t id_in[DIGITAL_OUTPUT_BITS_COUNT];
    reg_u16_t bit_in[DIGITAL_OUTPUT_BITS_COUNT];
    reg_u16_t inv;
    // Регистры.
    // Методы.
    METHOD_INIT(M_digital_output);
    METHOD_DEINIT(M_digital_output);
    METHOD_CALC(M_digital_output);
    // Коллбэки.
    // Внутренние данные.
    reg_t* reg_in[DIGITAL_OUTPUT_BITS_COUNT];
    reg_u16_t tmp_in;
    reg_u16_t tmp_in_mask;
    reg_u16_t tmp_out_mask;
    reg_u16_t tmp_out;
};

EXTERN METHOD_INIT_PROTO(M_digital_output);
EXTERN METHOD_DEINIT_PROTO(M_digital_output);
EXTERN METHOD_CALC_PROTO(M_digital_output);

#define DIGITAL_OUTPUT_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
		0,\
        /* Параметры */\
		{0},\
		{0},\
		0,\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_digital_output),\
        METHOD_DEINIT_PTR(M_digital_output),\
        METHOD_CALC_PTR(M_digital_output),\
        /* Коллбэки */\
        /* Внутренние данные */\
		{0},\
		0,\
		0,\
		0,\
		0,\
    }

#endif /* DIGITAL_OUTPUT_H */
