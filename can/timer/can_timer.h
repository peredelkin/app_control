/*
 * can_timer.h
 *
 *  Created on: 23 янв. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_TIMER_CAN_TIMER_H_
#define CAN_TIMER_CAN_TIMER_H_

#include "module/base.h"
#include "stm32f4xx.h"
#include "interrupts/interrupt_priorities.h"

#define CAN_TIM_TIM TIM5
#define CAN_TIM_IRQN TIM5_IRQn
#define CAN_TIM_IRQ_PRIO TIM5_IRQ_PRIO
#define CAN_TIM_IRQHANDLER TIM5_IRQHandler

//! Перечисление возможных бит управления.
enum _E_Can_Timer_Control {
    CAN_TIMER_CONTROL_NONE = CONTROL_NONE,
    CAN_TIMER_CONTROL_ENABLE = CONTROL_ENABLE,
};

//! Перечисление возможных бит статуса.
enum _E_Can_Timer_Status {
    CAN_TIMER_STATUS_NONE = STATUS_NONE,
    CAN_TIMER_STATUS_READY = STATUS_READY,
    CAN_TIMER_STATUS_RUN = STATUS_RUN,
    CAN_TIMER_STATUS_ERROR = STATUS_ERROR,
};


#define CAN_TIMER_TICKS_FREQ 1000
//! Период таймера в тиках.
#define CAN_TIMER_TICKS_PERIOD_US (1000000 / CAN_TIMER_TICKS_FREQ)
//! Предделитель таймера.
#define CAN_TIMER_PRESCALER 84 //90


typedef struct _S_can_timer M_can_timer;

struct _S_can_timer {
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
    METHOD_INIT(M_can_timer);
    METHOD_DEINIT(M_can_timer);
    METHOD_CALC(M_can_timer);
    METHOD_CONTROL(M_can_timer);
    // Коллбэки.
    CALLBACK(on_timeout);
    // Внутренние данные.
};

EXTERN METHOD_INIT_PROTO(M_can_timer);
EXTERN METHOD_DEINIT_PROTO(M_can_timer);
EXTERN METHOD_CALC_PROTO(M_can_timer);
EXTERN METHOD_CONTROL_PROTO(M_can_timer);

#define CAN_TIMER_DEFAULTS {\
	    0, /*control*/\
		0, /*status */\
		CAN_TIM_TIM, /* TIM */\
        0, /* out_counter */\
        METHOD_INIT_PTR(M_can_timer),\
		METHOD_DEINIT_PTR(M_can_timer),\
		METHOD_CALC_PTR(M_can_timer),\
		METHOD_CONTROL_PTR(M_can_timer),\
        CALLBACK_DEFAULTS /* on_timeout */,\
    }

#endif /* CAN_TIMER_CAN_TIMER_H_ */
