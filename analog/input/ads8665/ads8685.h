#ifndef ADS8685_H
#define ADS8685_H

#include <assert.h>
#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Ads8685_Control {
    ADS8685_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Ads8685_Status {
    ADS8685_STATUS_NONE = STATUS_NONE,
	ADS8685_STATUS_READY = STATUS_READY,
	ADS8685_STATUS_VALID = STATUS_VALID,
    SADS8685_STATUS_RUN = STATUS_RUN,
	ADS8685_STATUS_ERROR = STATUS_ERROR,
	ADS8685_STATUS_WARNING = STATUS_WARNING,
};

//DEVICE_ID_REG Register (address = 00h)
typedef struct _ads8685_DEVICE_ID_REG_bit {
	unsigned res_0_15 :16;
	unsigned dev_addr :4;
	unsigned res_20_31 :12;
} _ads8685_DEVICE_ID_REG_bit_t;
static_assert(sizeof(_ads8685_DEVICE_ID_REG_bit_t) == 4, "Invalid size of _ads8685_DEVICE_ID_REG_bit_t!");

typedef union _ads8685_DEVICE_ID_REG {
	struct _ads8685_DEVICE_ID_REG_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ads8685_DEVICE_ID_REG_t;

//RST_PWRCTL_REG Register (address = 04h)
typedef struct _ads8685_RST_PWRCTL_REG_bit {

} _ads8685_RST_PWRCTL_REG_bit_t;
static_assert(sizeof(_ads8685_RST_PWRCTL_REG_bit_t) == 4, "Invalid size of _ads8685_RST_PWRCTL_REG_bit_t!");

typedef union _ads8685_RST_PWRCTL_REG {
	struct _ads8685_RST_PWRCTL_REG_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ads8685_RST_PWRCTL_REG_t;

//SDI_CTL_REG Register (address = 08h)
typedef struct _ads8665_SDI_CTL_REG_bit {

} _ads8665_SDI_CTL_REG_bit_t;
static_assert(sizeof(_ads8665_SDI_CTL_REG_bit_t) == 4, "Invalid size of _ads8665_SDI_CTL_REG_bit_t!");

typedef union _ads8665_SDI_CTL_REG {
	struct _ads8665_SDI_CTL_REG_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ads8665_SDI_CTL_REG_t;

//SDO_CTL_REG Register (address = 0Ch)
typedef struct _ads8665_SDO_CTL_REG_bit {

} _ads8665_SDO_CTL_REG_bit_t;
static_assert(sizeof(_ads8665_SDO_CTL_REG_bit_t) == 4, "Invalid size of _ads8665_SDO_CTL_REG_bit_t!");

typedef union _ads8665_SDO_CTL_REG {
	struct _ads8665_SDO_CTL_REG_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ads8665_SDO_CTL_REG_t;

//DATAOUT_CTL_REG Register (address = 10h)
typedef struct _ads8665_DATAOUT_CTL_REG_bit {

} _ads8665_DATAOUT_CTL_REG_bit_t;
static_assert(sizeof(_ads8665_DATAOUT_CTL_REG_bit_t) == 4, "Invalid size of _ads8665_DATAOUT_CTL_REG_bit_t!");

typedef struct _ads8665_DATAOUT_CTL_REG {
	struct _ads8665_DATAOUT_CTL_REG_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ads8665_DATAOUT_CTL_REG_t;

//RANGE_SEL_REG Register (address = 14h)
typedef struct _ads8665_RANGE_SEL_REG_bit {

} _ads8665_RANGE_SEL_REG_bit_t;
static_assert(sizeof(_ads8665_RANGE_SEL_REG_bit_t) == 4, "Invalid size of _ads8665_RANGE_SEL_REG_bit_t!");

typedef union _ads8665_RANGE_SEL_REG {
	struct _ads8665_RANGE_SEL_REG_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ads8665_RANGE_SEL_REG_t;

//ALARM_REG Register (address = 20h)
typedef struct _ads8665_ALARM_REG_bit {

} _ads8665_ALARM_REG_bit_t;
static_assert(sizeof(_ads8665_ALARM_REG_bit_t) == 4, "Invalid size of _ads8665_ALARM_REG_bit_t!");

typedef union _ads8665_ALARM_REG {
	struct _ads8665_ALARM_REG_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ads8665_ALARM_REG_t;

//ALARM_H_TH_REG Register (address = 24h)
typedef union _ads8665_ALARM_H_TH_REG_bit {

} _ads8665_ALARM_H_TH_REG_bit_t;
static_assert(sizeof(_ads8665_ALARM_H_TH_REG_bit_t) == 4, "Invalid size of _ads8665_ALARM_H_TH_REG_bit_t!");

typedef union _ads8665_ALARM_H_TH_REG {
	struct _ads8665_ALARM_H_TH_REG_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ads8665_ALARM_H_TH_REG_t;

//ALARM_L_TH_REG Register (address = 28h)
typedef union _ads8665_ALARM_L_TH_REG_bit {

} _ads8665_ALARM_L_TH_REG_bit_t;
static_assert(sizeof(_ads8665_ALARM_L_TH_REG_bit_t) == 4, "Invalid size of _ads8665_ALARM_L_TH_REG_bit_t!");

typedef union _ads8665_ALARM_L_TH_REG {
	struct _ads8665_ALARM_L_TH_REG_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ads8665_ALARM_L_TH_REG_t;

//TX
typedef struct _ads8685_tx_frame_bit {
	unsigned data :16;
	unsigned addr :9;
	unsigned cmd :7;
} _ads8685_tx_frame_bit_t;
static_assert(sizeof(_ads8685_tx_frame_bit_t) == 4, "Invalid size of _ads8685_tx_frame_bit_t!");

typedef union _ads8685_tx_frame {
	struct _ads8685_tx_frame_bit bit;
	uint8_t byte[4];
	uint32_t all;
} _ssads8685_tx_frame_t;

//! Предварительная декларация типа модуля.
typedef struct _S_Ads8685 M_ads8685;

//! Структура модуля.
struct _S_Ads8685 {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    reg_u16_t out_ch_a;
    reg_u16_t out_ch_b;
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_ads8685);
    METHOD_DEINIT(M_ads8685);
    METHOD_CALC(M_ads8685);
    // Коллбэки.
    // Внутренние данные.
    SPI_BUS_TypeDef *m_spi_bus;
    _ssads8685_tx_frame_t m_tx_frame;
};

EXTERN METHOD_INIT_PROTO(M_ads8685);
EXTERN METHOD_DEINIT_PROTO(M_ads8685);
EXTERN METHOD_CALC_PROTO(M_ads8685);

#define ADS8685_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
		0,\
		0,\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_ads8685),\
        METHOD_DEINIT_PTR(M_ads8685),\
        METHOD_CALC_PTR(M_ads8685),\
        /* Коллбэки */\
        /* Внутренние данные */\
		0,\
		{{0}},\
    }

#endif /* ADS8685_H */
