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

//! Перечисление цветов
enum _Rgb_Led_Сolor {
	RGB_LED_COLOR_BLACK,
	RGB_LED_COLOR_VIOLET,
	RGB_LED_COLOR_BLUE_DARK,
	RGB_LED_COLOR_BLUE,
	RGB_LED_COLOR_GREEN,
	RGB_LED_COLOR_YELLOW,
	RGB_LED_COLOR_RED,
	RGB_LED_COLOR_WHITE
};

#define LED_RGB_GPIO ((BITS_GPIO_TypeDef*)GPIOH)
#define LED_RGB_PIN_COUNT 3
#define LED_RGB_R_PIN GPIO_PIN_12
#define LED_RGB_G_PIN GPIO_PIN_11
#define LED_RGB_B_PIN GPIO_PIN_10
#define LEG_RGB_MASK ((1 << LED_RGB_R_PIN) | (1 << LED_RGB_G_PIN) | (1 << LED_RGB_B_PIN))
#define LED_RGB_STATE_COUNT 8
#define LED_RGB_STATE_MASK 7

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
