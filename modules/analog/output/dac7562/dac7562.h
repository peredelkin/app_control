#ifndef DAC7562_H
#define DAC7562_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Dac7562_Control {
    DAC7562_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Dac7562_Status {
    DAC7562_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Dac7562 M_dac7562;

//! Структура модуля.
struct _S_Dac7562 {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    reg_u16_t in_ch_a;
    reg_u16_t in_ch_b;
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_dac7562);
    METHOD_DEINIT(M_dac7562);
    METHOD_CALC(M_dac7562);
    // Коллбэки.
    // Внутренние данные.
    SPI_BUS_TypeDef *m_spi_bus;
    reg_u8_t m_frame_ch_a[3];
    reg_u8_t m_frame_ch_b[3];
    reg_u8_t m_frame_ctrl[3];
};

EXTERN METHOD_INIT_PROTO(M_dac7562);
EXTERN METHOD_DEINIT_PROTO(M_dac7562);
EXTERN METHOD_CALC_PROTO(M_dac7562);

#define DAC7562_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
		0,\
		0,\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_dac7562),\
        METHOD_DEINIT_PTR(M_dac7562),\
        METHOD_CALC_PTR(M_dac7562),\
        /* Коллбэки */\
        /* Внутренние данные */\
		0,\
		{0},\
		{0},\
		{0},\
    }

#endif /* DAC7562_H */
