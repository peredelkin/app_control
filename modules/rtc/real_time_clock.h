#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H

#include <assert.h>
#include "module/base.h"

typedef struct _rtc_tr_bit {
	unsigned SU :4;
	unsigned ST :3;
	unsigned res_7 :1;
	unsigned MNU :4;
	unsigned MNT :3;
	unsigned res_15 :1;
	unsigned HU :4;
	unsigned HT :2;
	unsigned PM :1;
	unsigned res_23_31 :9;
} rtc_tr_bit_t;
static_assert(sizeof(rtc_tr_bit_t) == 4, "Invalid size of rtc_tr_bit_t!");

typedef struct _rtc_dr_bit {
	unsigned DU :4;
	unsigned DT :2;
	unsigned res_6_7 :2;
	unsigned MU :4;
	unsigned MT :1;
	unsigned WDU :3;
	unsigned YU :4;
	unsigned YT :4;
	unsigned res_24_31 :8;
} rtc_dr_bit_t;
static_assert(sizeof(rtc_dr_bit_t) == 4, "Invalid size of rtc_dr_bit_t!");

//! Перечисление возможных бит управления.
enum _E_Real_Time_Clock_Control {
    REAL_TIME_CLOCK_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Real_Time_Clock_Status {
    REAL_TIME_CLOCK_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Real_Time_Clock M_real_time_clock;

//! Структура модуля.
struct _S_Real_Time_Clock {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_real_time_clock);
    METHOD_DEINIT(M_real_time_clock);
    METHOD_CALC(M_real_time_clock);
    // Коллбэки.
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_real_time_clock);
EXTERN METHOD_DEINIT_PROTO(M_real_time_clock);
EXTERN METHOD_CALC_PROTO(M_real_time_clock);

#define REAL_TIME_CLOCK_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_real_time_clock),\
        METHOD_DEINIT_PTR(M_real_time_clock),\
        METHOD_CALC_PTR(M_real_time_clock),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* REAL_TIME_CLOCK_H */
