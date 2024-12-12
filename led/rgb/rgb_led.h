#ifndef RGB_LED_H
#define RGB_LED_H

#include "module/base.h"
#include "gpio/gpio.h"

//! Перечисление возможных бит управления.
enum _E_Rgb_Led_Control {
    RGB_LED_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Rgb_Led_Status {
    RGB_LED_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Rgb_Led M_rgb_led;

//! Структура модуля.
struct _S_Rgb_Led {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    reg_u16_t in;
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_rgb_led);
    METHOD_DEINIT(M_rgb_led);
    METHOD_CALC(M_rgb_led);
    // Коллбэки.
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_rgb_led);
EXTERN METHOD_DEINIT_PROTO(M_rgb_led);
EXTERN METHOD_CALC_PROTO(M_rgb_led);

#define RGB_LED_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
		0,\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_rgb_led),\
        METHOD_DEINIT_PTR(M_rgb_led),\
        METHOD_CALC_PTR(M_rgb_led),\
        /* Коллбэки */\
        /* Внутренние данные */\
    }

#endif /* RGB_LED_H */
