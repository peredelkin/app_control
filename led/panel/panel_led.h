#ifndef PANEL_LED_H
#define PANEL_LED_H

#include <assert.h>
#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Panel_Led_Control {
    PANEL_LED_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Panel_Led_Status {
    PANEL_LED_STATUS_NONE = STATUS_NONE,
};

typedef struct PACKED _panel_led_bit {
	unsigned SCR_LED: 1;	//P10
	unsigned T_mn: 1;		//P11
	unsigned T_pl: 1;		//P12
	unsigned Alr: 1;		//P13
	unsigned Wrn: 1;		//P14
	unsigned En: 1;			//P15
	unsigned ZS: 1;			//P16
	unsigned Lim: 1;		//P17
	unsigned reserved :8;
} _panel_led_bit_t;
static_assert(sizeof(_panel_led_bit_t) == 2, "Invalid size of _panel_led_bit_t!");

typedef union _panel_led_reg {
	uint16_t all;
	struct _panel_led_bit bit;
} _panel_led_reg_t;

//! Предварительная декларация типа модуля.
typedef struct _S_Panel_Led M_panel_led;

//! Структура модуля.
struct _S_Panel_Led {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    reg_u16_t out_data;
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_panel_led);
    METHOD_DEINIT(M_panel_led);
    METHOD_CALC(M_panel_led);
    // Коллбэки.
    // Внутренние данные.
    _panel_led_reg_t m_out_data;
};

EXTERN METHOD_INIT_PROTO(M_panel_led);
EXTERN METHOD_DEINIT_PROTO(M_panel_led);
EXTERN METHOD_CALC_PROTO(M_panel_led);

#define PANEL_LED_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
		0,\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_panel_led),\
        METHOD_DEINIT_PTR(M_panel_led),\
        METHOD_CALC_PTR(M_panel_led),\
        /* Коллбэки */\
        /* Внутренние данные */\
		{0},\
    }

#endif /* PANEL_LED_H */
