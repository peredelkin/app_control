#ifndef MS_TIMER_H
#define MS_TIMER_H

#include "module/base.h"
#include "stm32f4xx.h"

#define MS_TIM_TIM TIM4
#define MS_TIM_IRQN TIM4_IRQn
#define MS_TIM_IRQ_PRIO 8
#define MS_TIM_IRQHANDLER TIM4_IRQHandler

//! Перечисление возможных бит управления.
enum _E_Ms_Timer_Control {
    MS_TIMER_CONTROL_NONE = CONTROL_NONE,
    MS_TIMER_CONTROL_ENABLE = CONTROL_ENABLE,
};

//! Перечисление возможных бит статуса.
enum _E_Ms_Timer_Status {
    MS_TIMER_STATUS_NONE = STATUS_NONE,
    MS_TIMER_STATUS_READY = STATUS_READY,
    MS_TIMER_STATUS_RUN = STATUS_RUN,
    MS_TIMER_STATUS_ERROR = STATUS_ERROR,
};


#define MS_TIMER_TICKS_FREQ 1000
//! Период таймера в тиках.
#define MS_TIMER_TICKS_PERIOD_US (1000000 / MS_TIMER_TICKS_FREQ)
//! Предделитель таймера.
#define MS_TIMER_PRESCALER 90


typedef struct _S_ms_timer M_ms_timer;

struct _S_ms_timer {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    TIM_TypeDef* TIM; //!< Таймер.
    // Выходные данные.
    reg_u32_t out_counter; //!< Счётчик тиков.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_ms_timer);
    METHOD_DEINIT(M_ms_timer);
    METHOD_CALC(M_ms_timer);
    METHOD_CONTROL(M_ms_timer);
    // Коллбэки.
    CALLBACK(on_timeout);
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_ms_timer);
EXTERN METHOD_DEINIT_PROTO(M_ms_timer);
EXTERN METHOD_CALC_PROTO(M_ms_timer);
EXTERN METHOD_CONTROL_PROTO(M_ms_timer);

#define MS_TIMER_DEFAULTS {\
	    0, /*control*/\
		0, /*status */\
		MS_TIM_TIM, /* TIM */\
        0, /* out_counter */\
        METHOD_INIT_PTR(M_ms_timer),\
		METHOD_DEINIT_PTR(M_ms_timer),\
		METHOD_CALC_PTR(M_ms_timer),\
		METHOD_CONTROL_PTR(M_ms_timer),\
        CALLBACK_DEFAULTS /* on_timeout */,\
    }

#endif /* MS_TIMER_H */
