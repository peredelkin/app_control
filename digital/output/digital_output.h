#ifndef DIGITAL_OUTPUT_H
#define DIGITAL_OUTPUT_H

#include <assert.h>
#include "module/base.h"

#define DIGITAL_OUTPUT_COUNT 32

typedef struct _digital_output_bit {
	unsigned ncv :8;
	unsigned relay :4;
	unsigned reserved :20;
} _digital_output_bit_t;
static_assert(sizeof(_digital_output_bit_t) == 4, "Invalid size of _digital_input_bit_t!");

typedef union _digital_output_reg {
	uint32_t all;
	struct _digital_output_bit bit;
} _digital_output_reg_t;

//! Перечисление возможных бит управления.
enum _E_Digital_Output_Control {
	DIGITAL_OUTPUT_CONTROL_NONE = CONTROL_NONE,
	DIGITAL_OUTPUT_CONTROL_RESET = CONTROL_RESET,
	DIGITAL_OUTPUT_CONTROL_ENABLE = CONTROL_ENABLE,
	DIGITAL_OUTPUT_CONTROL_START = CONTROL_START,
	DIGITAL_OUTPUT_CONTROL_STOP = CONTROL_STOP
};

//! Перечисление возможных бит статуса.
enum _E_Digital_Output_Status {
    DIGITAL_OUTPUT_STATUS_NONE = STATUS_NONE,
	DIGITAL_OUTPUT_STATUS_READY = STATUS_READY,
	DIGITAL_OUTPUT_STATUS_VALID = STATUS_VALID,
	DIGITAL_OUTPUT_STATUS_RUN = STATUS_RUN,
	DIGITAL_OUTPUT_STATUS_ERROR = STATUS_ERROR,
	DIGITAL_OUTPUT_STATUS_WARNING = STATUS_WARNING,
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
    reg_u32_t in_data; //!< Вход дискретных выходов @{"id": 3, "rpdo": true}
    // Выходные данные.
    // Параметры.
    reg_u8_t p_invert[DIGITAL_OUTPUT_COUNT]; //!< Параметр инверсии дискретного выхода
    reg_u8_t p_select[DIGITAL_OUTPUT_COUNT]; //!< Параметр выбора входного бита
    reg_u8_t p_t_set[DIGITAL_OUTPUT_COUNT]; //!< Параметр задержки установки дискретного выхода
    reg_u8_t p_t_reset[DIGITAL_OUTPUT_COUNT]; //!< Параметр задержки сброса дискретного выхода
    // Регистры.
    // Методы.
    METHOD_INIT(M_digital_output);
    METHOD_DEINIT(M_digital_output);
    METHOD_CALC(M_digital_output);
    // Коллбэки.
    // Внутренние данные.
    _digital_output_reg_t m_out_data;
	reg_u8_t m_cnt_set[DIGITAL_OUTPUT_COUNT];
	reg_u8_t m_cnt_reset[DIGITAL_OUTPUT_COUNT];
};

EXTERN METHOD_INIT_PROTO(M_digital_output);
EXTERN METHOD_DEINIT_PROTO(M_digital_output);
EXTERN METHOD_CALC_PROTO(M_digital_output);

#define DIGITAL_OUTPUT_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
		0,\
        /* Выходные данные */\
        /* Параметры */\
		{0},\
		{0},\
		{0},\
		{0},\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_digital_output),\
        METHOD_DEINIT_PTR(M_digital_output),\
        METHOD_CALC_PTR(M_digital_output),\
        /* Коллбэки */\
        /* Внутренние данные */\
		{0},\
		{0},\
		{0},\
    }

#endif /* DIGITAL_OUTPUT_H */
