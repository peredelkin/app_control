#ifndef MODBUS_TO_CAN_H
#define MODBUS_TO_CAN_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Modbus_To_Can_Control {
    MODBUS_TO_CAN_CONTROL_NONE = CONTROL_NONE,
	MODBUS_TO_CAN_CONTROL_RESET = CONTROL_RESET,
	MODBUS_TO_CAN_CONTROL_ENABLE = CONTROL_ENABLE,
	MODBUS_TO_CAN_CONTROL_START = CONTROL_START,
	MODBUS_TO_CAN_CONTROL_STOP =  CONTROL_STOP,
	MODBUS_TO_CAN_CONTROL_READ = (CONTROL_USER << 0),
	MODBUS_TO_CAN_CONTROL_WRITE = (CONTROL_USER << 1)
};

//! Перечисление возможных бит статуса.
enum _E_Modbus_To_Can_Status {
    MODBUS_TO_CAN_STATUS_NONE = STATUS_NONE,
	MODBUS_TO_CAN_STATUS_READY = STATUS_READY,
	MODBUS_TO_CAN_STATUS_VALID = STATUS_VALID,
	MODBUS_TO_CAN_STATUS_RUN = STATUS_RUN,
	MODBUS_TO_CAN_STATUS_ERROR = STATUS_ERROR,
	MODBUS_TO_CAN_STATUS_WARNING = STATUS_WARNING,
	MODBUS_TO_CAN_STATUS_READ_DONE = (STATUS_USER << 0),
	MODBUS_TO_CAN_STATUS_WRITE_DONE = (STATUS_USER << 1)
};

//! Предварительная декларация типа модуля.
typedef struct _S_Modbus_To_Can M_modbus_to_can;

//! Структура модуля.
struct _S_Modbus_To_Can {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_modbus_to_can);
    METHOD_DEINIT(M_modbus_to_can);
    METHOD_CALC(M_modbus_to_can);
    // Коллбэки.
    // Внутренние данные.
    uint8_t m_id;
    uint16_t m_index;
    uint8_t m_subindex;
    size_t m_size;
    iql_t m_data;
    uint8_t m_timeout_cnt;
};

EXTERN METHOD_INIT_PROTO(M_modbus_to_can);
EXTERN METHOD_DEINIT_PROTO(M_modbus_to_can);
EXTERN METHOD_CALC_PROTO(M_modbus_to_can);

#define MODBUS_TO_CAN_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_modbus_to_can),\
        METHOD_DEINIT_PTR(M_modbus_to_can),\
        METHOD_CALC_PTR(M_modbus_to_can),\
        /* Коллбэки */\
        /* Внутренние данные */\
	    0, /*m_id*/\
	    0, /*m_index*/\
	    0, /*m_subindex*/\
	    0, /*m_size*/\
	    0, /*m_data*/\
		0, /*m_timeout_cnt*/\
    }

#endif /* MODBUS_TO_CAN_H */
