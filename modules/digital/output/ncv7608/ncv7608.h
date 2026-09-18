#ifndef NCV7608_H
#define NCV7608_H

#include <assert.h>
#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Ncv7608_Control {
    NCV7608_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Ncv7608_Status {
    NCV7608_STATUS_NONE = STATUS_NONE,
};

typedef struct _ncv7608_bit {
	unsigned out_1 :1;
	unsigned out_2 :1;
	unsigned out_3 :1;
	unsigned out_4 :1;
	unsigned out_5 :1;
	unsigned out_6 :1;
	unsigned out_7 :1;
	unsigned out_8 :1;
	unsigned reserved : 24;
} _ncv7608_bit_t;
static_assert(sizeof(_ncv7608_bit_t) == 4, "Invalid size of _ncv7608_bit_t!");

typedef union _ncv7608_reg {
	uint32_t all;
	struct _ncv7608_bit bit;
} _ncv7608_reg_t;

//! Предварительная декларация типа модуля.
typedef struct _S_Ncv7608 M_ncv7608;

//! Структура модуля.
struct _S_Ncv7608 {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    reg_u32_t in_data;
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_ncv7608);
    METHOD_DEINIT(M_ncv7608);
    METHOD_CALC(M_ncv7608);
    // Коллбэки.
    // Внутренние данные.
    _ncv7608_reg_t m_out_data;
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
		{0},\
    }

#endif /* NCV7608_H */
