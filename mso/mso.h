#ifndef MSO_H
#define MSO_H

#include <stdbool.h>

#include "module/base.h"

typedef iql_t mso_data_t;

#define MSO_MAX_CHANNEL_COUNT 32
#define MSO_DATA_BLOCK_COUNT 128
#define MSO_DATA_COUNT (MSO_MAX_CHANNEL_COUNT * MSO_DATA_BLOCK_COUNT)
#define MSO_DATA_SIZE (MSO_DATA_COUNT * sizeof(mso_data_t))


//! Перечисление возможных бит управления.
enum _E_Mso_Control {
    MSO_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Mso_Status {
    MSO_STATUS_NONE = STATUS_NONE,
};

typedef struct {
	bool enabled;
	reg_id_t id;
	reg_id_t id_old;
	reg_t* reg;
	mso_data_t *ptr;
} mso_channel_t;

//! Предварительная декларация типа модуля.
typedef struct _S_Mso M_mso;

//! Структура модуля.
struct _S_Mso {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    // Параметры.
    // Регистры.
    mso_channel_t r_channel[MSO_MAX_CHANNEL_COUNT];
    // Методы.
    METHOD_INIT(M_mso);
    METHOD_DEINIT(M_mso);
    METHOD_CALC(M_mso);
    METHOD_IDLE(M_mso);
    // Коллбэки.
    // Внутренние данные.
    int m_ch_count;
    int m_ch_data_count;
    mso_data_t *m_ptr;
};

EXTERN METHOD_INIT_PROTO(M_mso);
EXTERN METHOD_DEINIT_PROTO(M_mso);
EXTERN METHOD_CALC_PROTO(M_mso);
EXTERN METHOD_IDLE_PROTO(M_mso);

#define MSO_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
		{{0}},\
        /* Методы */\
        METHOD_INIT_PTR(M_mso),\
        METHOD_DEINIT_PTR(M_mso),\
        METHOD_CALC_PTR(M_mso),\
		METHOD_IDLE_PTR(M_mso),\
        /* Коллбэки */\
        /* Внутренние данные */\
		0,\
		0,\
		0,\
    }

#endif /* MSO_H */
