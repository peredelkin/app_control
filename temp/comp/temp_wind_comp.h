#ifndef TEMP_WIND_COMP_H
#define TEMP_WIND_COMP_H

#include "module/base.h"
#include "temp/ntc/ntc_temp.h"

//! Перечисление возможных бит управления.
enum _E_Temp_Wind_Comp_Control {
    TEMP_WIND_COMP_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Temp_Wind_Comp_Status {
    TEMP_WIND_COMP_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Temp_Wind_Comp M_temp_wind_comp;

//! Структура модуля.
struct _S_Temp_Wind_Comp {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    reg_u32_t out_data; //!< Выход компараторов
    // Параметры.
    reg_iq15_t p_high_resist[NTC_TEMP_COUNT]; //!< Порог верхнего компаратора
    reg_iq15_t p_low_resist[NTC_TEMP_COUNT]; //!< Порог нижнего компаратора
    reg_u8_t p_high_time[NTC_TEMP_COUNT]; //!< Фильтр верхнего компаратора
    reg_u8_t p_low_time[NTC_TEMP_COUNT]; //!< Фильтр нижнего компаратора
    reg_u8_t p_invert[NTC_TEMP_COUNT]; //!< Параметр инверсии бита выхода
    // Регистры.
    // Методы.
    METHOD_INIT(M_temp_wind_comp);
    METHOD_DEINIT(M_temp_wind_comp);
    METHOD_CALC(M_temp_wind_comp);
    // Коллбэки.
    // Внутренние данные.
	reg_u8_t m_high_cnt[NTC_TEMP_COUNT];
	reg_u8_t m_low_cnt[NTC_TEMP_COUNT];
};

EXTERN METHOD_INIT_PROTO(M_temp_wind_comp);
EXTERN METHOD_DEINIT_PROTO(M_temp_wind_comp);
EXTERN METHOD_CALC_PROTO(M_temp_wind_comp);

#define TEMP_WIND_COMP_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
		0, /*out_data*/\
        /* Параметры */\
		{0},/*p_high_temp*/\
		{0},/*p_low_temp*/\
		{0},/*p_high_time*/\
		{0},/*p_low_time*/\
		{0},/*p_invert*/\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_temp_wind_comp),\
        METHOD_DEINIT_PTR(M_temp_wind_comp),\
        METHOD_CALC_PTR(M_temp_wind_comp),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* TEMP_WIND_COMP_H */
