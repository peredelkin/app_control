#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include <assert.h>
#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Digital_Input_Control {
	DIGITAL_INPUT_CONTROL_NONE = CONTROL_NONE,
	DIGITAL_INPUT_CONTROL_RESET = CONTROL_RESET,
	DIGITAL_INPUT_CONTROL_ENABLE = CONTROL_ENABLE,
	DIGITAL_INPUT_CONTROL_START = CONTROL_START,
	DIGITAL_INPUT_CONTROL_STOP = CONTROL_STOP
};

//! Перечисление возможных бит статуса.
enum _E_Digital_Input_Status {
	DIGITAL_INPUT_STATUS_NONE = STATUS_NONE,
	DIGITAL_INPUT_STATUS_READY = STATUS_READY,
	DIGITAL_INPUT_STATUS_VALID = STATUS_VALID,
	DIGITAL_INPUT_STATUS_RUN = STATUS_RUN,
	DIGITAL_INPUT_STATUS_ERROR = STATUS_ERROR,
	DIGITAL_INPUT_STATUS_WARNING = STATUS_WARNING,
};

#define DIGITAL_INPUT_COUNT 32

typedef struct _digital_input_out_bit {
	unsigned sw_no :1;		//0
	unsigned sw_nc :1;		//1
	unsigned sw_oc_ol :1;	//2
	unsigned test :1;		//3
	unsigned enable_run :1;	//4
	unsigned increase :1;	//5
	unsigned decrease :1;	//6
	unsigned man_auto :1;	//7
	unsigned loc_rem :1;	//8
	unsigned fwd_bwd :1;	//9
	unsigned err_reset :1;	//10
	unsigned em_off :1;		//11
	unsigned contactor :1;	//12
	unsigned release :1;	//13
	unsigned ext_exciter :1;//14
	unsigned reserved :17;	//15-31
} _digital_input_out_bit_t;
static_assert(sizeof(_digital_input_out_bit_t) == 4, "Invalid size of _digital_input_out_bit_t!");

typedef union _digital_input_out_reg {
	uint32_t all;
	struct _digital_input_out_bit bit;
} _digital_input_out_reg_t;

typedef struct _digital_input_in_bit {
	unsigned const_0 :1;	//0
	unsigned const_1 :1;	//1
	unsigned msdi :10;		//2-11
	unsigned em_stop :1;	//12
	unsigned ac_lost :1;	//13
	unsigned dc_lost :1;	//14
	unsigned panel :1;		//15
	unsigned reserved :16;	//16-31 free
} _digital_input_in_bit_t;
static_assert(sizeof(_digital_input_in_bit_t) == 4, "Invalid size of _digital_input_in_bit_t!");

typedef union _digital_input_in_reg {
	uint32_t all;
	struct _digital_input_in_bit bit;
} _digital_input_in_reg_t;

//! Предварительная декларация типа модуля.
typedef struct _S_Digital_Input M_digital_input;

//! Структура модуля.
struct _S_Digital_Input {
	// Базовые поля.
	control_t control; //!< Слово управления.
	status_t status; //!< Слово состояния.
	// Входные данные.
	// Выходные данные.
	reg_u32_t out_data; //!< Выход дискретных входов @{"id": 3, "tpdo": true}
	// Параметры.
	reg_u8_t p_invert[DIGITAL_INPUT_COUNT]; //!< Параметр инверсии бита выхода
	reg_u8_t p_select[DIGITAL_INPUT_COUNT]; //!< Параметр выбора дискретного входа
	reg_u8_t p_t_set[DIGITAL_INPUT_COUNT]; //!< Параметр задержки установки бита выхода
	reg_u8_t p_t_reset[DIGITAL_INPUT_COUNT]; //!< Параметр задержки сброса бита выхода
	reg_u8_t p_t_msdi_invalid; //!< Параметр задержки установки флага невалидности данных от MSDI
	// Регистры.
	// Методы.
	METHOD_INIT(M_digital_input);
	METHOD_DEINIT(M_digital_input);
	METHOD_CALC(M_digital_input);
	// Коллбэки.
	// Внутренние данные.
	_digital_input_in_reg_t m_in_data; //TODO: перенести из m в r для отображения сырых значений входов
	reg_u32_t m_out_data;
	reg_u8_t m_cnt_set[DIGITAL_INPUT_COUNT];
	reg_u8_t m_cnt_reset[DIGITAL_INPUT_COUNT];
	reg_u8_t m_cnt_msdi_invalid;
};

EXTERN METHOD_INIT_PROTO(M_digital_input);
EXTERN METHOD_DEINIT_PROTO(M_digital_input);
EXTERN METHOD_CALC_PROTO(M_digital_input);

#define DIGITAL_INPUT_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
		0,\
        /* Параметры */\
		{0},\
		{0},\
		{0},\
		{0},\
		0,\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_digital_input),\
        METHOD_DEINIT_PTR(M_digital_input),\
        METHOD_CALC_PTR(M_digital_input),\
        /* Коллбэки */\
        /* Внутренние данные */\
		{0},\
		0,\
		{0},\
		{0},\
		0,\
    }

#endif /* DIGITAL_INPUT_H */
