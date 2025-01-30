#ifndef MSDI_H
#define MSDI_H

#include "module/base.h"
#include "tic12400/tic12400.h"
#include "lib/fsm/fsm.h"

typedef struct {
	TIC12400_INT_STAT_REG INT_STAT;
	TIC12400_IN_STAT_COMP_REG IN_STAT_COMP;
	TIC12400_ANA_STAT_REG ANA_STAT1;
	TIC12400_ANA_STAT_REG ANA_STAT2;
	TIC12400_ANA_STAT_REG ANA_STAT3;
	TIC12400_ANA_STAT_REG ANA_STAT9;
	TIC12400_ANA_STAT_REG ANA_STAT12;
} tic12400_data_t;

//! Перечисление возможных бит управления.
enum _E_Msdi_Control {
    MSDI_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Msdi_Status {
	MSDI_STATUS_NONE = STATUS_NONE,
	MSDI_STATUS_READY = STATUS_READY,
	MSDI_STATUS_VALID = STATUS_VALID,
	MSDI_STATUS_RUN = STATUS_RUN,
	MSDI_STATUS_ERROR = STATUS_ERROR,
	MSDI_STATUS_WARNING = STATUS_WARNING,
	MSDI_STATUS_INT = (STATUS_USER << 0),
};

#define MSDI_AI_COUNT 8

//! Предварительная декларация типа модуля.
typedef struct _S_Msdi M_msdi;

//! Структура модуля.
struct _S_Msdi {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    reg_u32_t out_di;
    reg_u16_t out_ai[MSDI_AI_COUNT];
    reg_iq15_t ref;
    reg_iq15_t vcc;
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_msdi);
    METHOD_DEINIT(M_msdi);
    METHOD_CALC(M_msdi);
    // Коллбэки.
    // Внутренние данные.
    tic12400_t tic12400;
    TIC12400_INT_STAT_REG int_stat;
    tic12400_data_t data;
    volatile bool done;
};

EXTERN METHOD_INIT_PROTO(M_msdi);
EXTERN METHOD_DEINIT_PROTO(M_msdi);
EXTERN METHOD_CALC_PROTO(M_msdi);

#define MSDI_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
		0,\
		{0},\
		0,\
		0,\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_msdi),\
        METHOD_DEINIT_PTR(M_msdi),\
        METHOD_CALC_PTR(M_msdi),\
        /* Коллбэки */\
        /* Внутренние данные */\
		{0},\
		{0},\
		{{0}},\
		0,\
    }

#endif /* MSDI_H */
