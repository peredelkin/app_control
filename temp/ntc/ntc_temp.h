#ifndef NTC_TEMP_H
#define NTC_TEMP_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Ntc_Temp_Control {
    NTC_TEMP_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Ntc_Temp_Status {
    NTC_TEMP_STATUS_NONE = STATUS_NONE,
};

#define NTC_TEMP_COUNT 6

//! Предварительная декларация типа модуля.
typedef struct _S_Ntc_Temp M_ntc_temp;

//! Структура модуля.
struct _S_Ntc_Temp {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    reg_iq15_t out_temp[NTC_TEMP_COUNT];
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_ntc_temp);
    METHOD_DEINIT(M_ntc_temp);
    METHOD_CALC(M_ntc_temp);
    // Коллбэки.
    // Внутренние данные.
    reg_u16_t *ai;
    reg_u16_t index;
};

EXTERN METHOD_INIT_PROTO(M_ntc_temp);
EXTERN METHOD_DEINIT_PROTO(M_ntc_temp);
EXTERN METHOD_CALC_PROTO(M_ntc_temp);

#define NTC_TEMP_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
		{0},\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_ntc_temp),\
        METHOD_DEINIT_PTR(M_ntc_temp),\
        METHOD_CALC_PTR(M_ntc_temp),\
        /* Коллбэки */\
        /* Внутренние данные */\
		0,\
		0,\
    }

#endif /* NTC_TEMP_H */
