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
	MSDI_STATUS_NONE =			STATUS_NONE,			/*zero*/
	MSDI_STATUS_READY =			STATUS_READY,			/*0*/
	MSDI_STATUS_VALID =			STATUS_VALID,			/*1*/
	MSDI_STATUS_RUN =			STATUS_RUN,				/*2*/
	MSDI_STATUS_ERROR =			STATUS_ERROR,			/*3*/
	MSDI_STATUS_WARNING =		STATUS_WARNING,			/*4*/
	MSDI_STATUS_OI =			(STATUS_USER << 0),		/*5 Other Interrupt*/
	MSDI_STATUS_TEMP =			(STATUS_USER << 1),		/*6 Temperature Event*/
	MSDI_STATUS_VS_TH =			(STATUS_USER << 2),		/*7 VS Threshold Crossing*/
	MSDI_STATUS_SSC =			(STATUS_USER << 3),		/*8 Switch State Change*/
	MSDI_STATUS_PRTY_FAIL =		(STATUS_USER << 4),		/*9 Parity Fail*/
	MSDI_STATUS_SPI_FAIL =		(STATUS_USER << 5),		/*10 SPI Error*/
	MSDI_STATUS_POR =			(STATUS_USER << 6),		/*11 Power-on Reset*/
	MSDI_STATUS_INT_POR =		(STATUS_USER << 7),		/*11 Power-on Reset*/
	MSDI_STATUS_INT_SPI_FAIL =	(STATUS_USER << 8),		/*12 SPI error*/
	MSDI_STATUS_INT_PRTY_FAIL =	(STATUS_USER << 9),		/*13 Parity error*/
	MSDI_STATUS_INT_SSC =		(STATUS_USER << 10),	/*14 Switch state change*/
	MSDI_STATUS_INT_TSD =		(STATUS_USER << 11),	/*15 Temperature Shutdown*/
	MSDI_STATUS_INT_TW =		(STATUS_USER << 12),	/*16 Temperature warning*/
	MSDI_STATUS_INT_OV =		(STATUS_USER << 13),	/*17 Over-voltage*/
	MSDI_STATUS_INT_UV =		(STATUS_USER << 14),	/*18 Under-voltage*/
	MSDI_STATUS_INT_CRC_CALC =	(STATUS_USER << 15),	/*19 CRC calculation is finished*/
	MSDI_STATUS_INT_VS0 =		(STATUS_USER << 16),	/*20 VS0_THRES2A or VS0_THRES2B*/
	MSDI_STATUS_INT_VS1 =		(STATUS_USER << 17),	/*21 VS1_THRES2A or VS1_THRES2B*/
	MSDI_STATUS_INT_WET_DIAG =	(STATUS_USER << 18),	/*22 Wetting current error*/
	MSDI_STATUS_INT_ADC_DIAG =	(STATUS_USER << 19),	/*23 ADC self-diagnostic error*/
	MSDI_STATUS_INT_CHK_FAIL =	(STATUS_USER << 20)		/*24 Error is detected when loading factory settings*/
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
    reg_u32_t out_digital;
    reg_u16_t out_analog[MSDI_AI_COUNT];
    reg_iq15_t out_ref;
    reg_iq15_t out_vcc;
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_msdi);
    METHOD_DEINIT(M_msdi);
    METHOD_CALC(M_msdi);
    // Коллбэки.
    // Внутренние данные.
    tic12400_t m_tic12400;
    TIC12400_INT_STAT_REG m_int_stat;
    tic12400_data_t m_data;
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
    }

#endif /* MSDI_H */
