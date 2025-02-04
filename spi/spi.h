/*
 * spi_dma.h
 *
 *  Created on: 6 мая 2022 г.
 *      Author: Ruslan
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <assert.h>
#include <stdbool.h>
#include "sys/counter/sys_counter.h"
#include "lib/defs/defs.h"
#include "lib/errors/errors.h"
#include "lib/stm32f4xx/stm32f4xx.h"
#include "gpio/gpio.h"

#define SPI_CR1_T_SIZE 2
#define SPI_CR2_T_SIZE 2
#define SPI_SR_T_SIZE 2
#define SPI_I2SCFGR_T_SIZE 2
#define SPI_I2SPR_T_SIZE 2

//Фаза
enum _SPI_CR1_CPHA {
	SPI_CPHA_FIRST = 0, SPI_CPHA_SECOND = 1
};

typedef enum _SPI_CR1_CPHA spi_cr1_cpha_t;

//Полярность
enum _SPI_CR1_CPOL {
	SPI_CPOL_IDLE_LOW = 0, SPI_CPOL_IDLE_HIGH = 1
};

typedef enum _SPI_CR1_CPOL spi_cr1_cpol_t;

//Мастер/слейв
enum _SPI_CR1_MSTR {
	SPI_MSTR_SLAVE = 0, SPI_MSTR_MASTER = 1
};

typedef enum _SPI_CR1_MSTR spi_cr1_mstr_t;

//Скорость
enum _SPI_CR1_BR {
	SPI_BR_FPCLK_2 = 0, SPI_BR_FPCLK_4 = 1, SPI_BR_FPCLK_8 = 2, SPI_BR_FPCLK_16 = 3, SPI_BR_FPCLK_32 = 4,
	SPI_BR_FPCLK_64 = 5, SPI_BR_FPCLK_128 = 6, SPI_BR_FPCLK_256 = 7
};

typedef enum _SPI_CR1_BR spi_cr1_br_t;

//Вкл/Выкл
enum _SPI_CR1_SPE {
	SPI_SPE_DIS = 0, SPI_SPE_ENA = 1
};

typedef enum _SPI_CR1_SPE spi_cr1_spe_t;

//Порядок передачи
enum _SPI_CR1_LSBFIRST {
	SPI_LSBFIRST_MSB_FIRST = 0, SPI_LSBFIRST_LSB_FIRST = 1
};

typedef enum _SPI_CR1_LSBFIRST spi_cr1_lsbfirst_t;

//Внутренний выбор слейва
enum _SPI_CR1_SSI {
	SPI_SSI_SEL = 0, SPI_SSI_NSEL = 1
};

typedef enum _SPI_CR1_SSI spi_cr1_ssi_t;

//Программное управление слейвом
enum _SPI_CR1_SSM {
	SPI_SSM_DIS = 0, SPI_SSM_ENA = 1
};

typedef enum _SPI_CR1_SSM spi_cr1_ssm_t;

//Только прием
enum _SPI_CR1_RXONLY {
	SPI_RXONLY_DIS = 0, SPI_RXONLY_ENA = 1
};

typedef enum _SPI_CR1_RXONLY spi_cr1_rxonly_t;

//8bit/16bit фрейм
enum _SPI_CR1_DFF {
	SPI_DFF_8 = 0, SPI_DFF_16 = 1
};

typedef enum _SPI_CR1_DFF spi_cr1_dff_t;

//CRC следующий
enum _SPI_CR1_CRCNEXT {
	SPI_CRCNEXT_DATA_PHASE = 0, SPI_CRCNEXT_CRC_PHASE = 1
};

typedef enum _SPI_CR1_CRCNEXT spi_cr1_crcnext_t;

//Вычисление CRC
enum _SPI_CR1_CRCEN {
	SPI_CRCEN_DIS = 0, SPI_CRCEN_ENA = 1
};

typedef enum _SPI_CR1_CRCEN spi_cr1_crcen_t;

//Включение выхода в дрвунаправленном режиме
enum _SPI_CR1_BIDIOE {
	SPI_BIDIOE_RX = 0, SPI_BIDIOE_TX = 1
};

typedef enum _SPI_CR1_BIDIOE spi_cr1_bidioe_t;

//Включение двунаправленного режима
enum _SPI_CR1_BIDIMODE {
	SPI_BIDIMODE_UNIDIR = 0, SPI_BIDIMODE_BIDIR = 1
};

typedef enum _SPI_CR1_BIDIMODE spi_cr1_bidimode_t;

//SPI control bits 1
typedef struct PACKED SPI_CR1_BITS {
	unsigned CPHA :1; //Bit 0		Clock phase
	unsigned CPOL :1; //Bit 1		Clock polarity
	unsigned MSTR :1; //Bit 2		Master selection
	unsigned BR :3; //Bits 5:3	Baud rate control
	unsigned SPE :1; //Bit 6		SPI enable
	unsigned LSBFIRST :1; //Bit 7		Frame format
	unsigned SSI :1; //Bit 8		Internal slave select
	unsigned SSM :1; //Bit 9		Software slave management
	unsigned RXONLY :1; //Bit 10	Receive only
	unsigned DFF :1; //Bit 11	Data frame format
	unsigned CRCNEXT :1; //Bit 12	CRC transfer next
	unsigned CRCEN :1; //Bit 13	Hardware CRC calculation enable
	unsigned BIDIOE :1; //Bit 14	Output enable in bidirectional mode
	unsigned BIDIMODE :1; //Bit 15	Bidirectional data mode enable
} spi_cr1_t;
static_assert(sizeof(spi_cr1_t) == SPI_CR1_T_SIZE, "Invalid size of spi_cr1_t!");

//SPI control register 1
typedef union _SPI_CR1_REG {
	uint16_t all;
	struct SPI_CR1_BITS bit;
} SPI_CR1_REG;

//DMA приема
enum _SPI_CR2_RXDMAEN {
	SPI_RXDMAEN_DIS = 0, SPI_RXDMAEN_ENA = 1
};

typedef enum _SPI_CR2_RXDMAEN spi_cr2_rxdmaen_t;

//DMA передачи
enum _SPI_CR2_TXDMAEN {
	SPI_TXDMAEN_DIS = 0, SPI_TXDMAEN_ENA = 1
};

typedef enum _SPI_CR2_TXDMAEN spi_cr2_txdmaen_t;

//Включение пина выбора слейва
enum _SPI_CR2_SSOE {
	SPI_SSOE_DIS = 0, SPI_SSOE_ENA = 1
};

typedef enum _SPI_CR2_SSOE spi_cr2_ssoe_t;

//Формат фрейма
enum _SPI_CR2_FRF {
	SPI_FRF_MOTOROLA = 0, SPI_FRF_TI = 1,
};

typedef enum _SPI_CR2_FRF spi_cr2_frf_t;

//Включение прерывания ошибки
enum _SPI_CR2_ERRIE {
	SPI_ERRIE_DIS = 0, SPI_ERRIE_ENA = 1
};

typedef enum _SPI_CR2_ERRIE spi_cr2_errie_t;

//Включение прерывания не пустого буфера приема
enum _SPI_CR2_RXNEIE {
	SPI_RXNEIE_DIS = 0, SPI_RXNEIE_ENA = 1
};

typedef enum _SPI_CR2_RXNEIE spi_cr2_rxneie_t;

//Включение прерывание пустого буфера передачи
enum _SPI_CR2_TXEIE {
	SPI_TXEIE_DIS = 0, SPI_TXEIE_ENA = 1
};

typedef enum _SPI_CR2_TXEIE spi_cr2_txeie_t;

//SPI control bits 2
typedef struct PACKED SPI_CR2_BITS {
	unsigned RXDMAEN :1; //Bit 0		Rx buffer DMA enable
	unsigned TXDMAEN :1; //Bit 1		Tx buffer DMA enable
	unsigned SSOE :1; //Bit 2		SS output enable
	unsigned RSVD_3 :1; //Bit 3		Forced to 0 by hardware.
	unsigned FRF :1; //Bit 4		Frame format
	unsigned ERRIE :1; //Bit 5		Error interrupt enable
	unsigned RXNEIE :1; //Bit 6		RX buffer not empty interrupt enable
	unsigned TXEIE :1; //Bit 7		Tx buffer empty interrupt enable
	unsigned RSVD_15_8 :8; //Bits 15:8	Reserved, must be kept at reset value.
} spi_cr2_t;
static_assert(sizeof(spi_cr2_t) == SPI_CR2_T_SIZE, "Invalid size of spi_cr2_t!");

//SPI control register 2
typedef union _SPI_CR2_REG {
	uint16_t all;
	struct SPI_CR2_BITS bit;
} SPI_CR2_REG;

//SPI status bits
typedef struct PACKED SPI_SR_BITS {
	unsigned RXNE :1; //Bit 0		Receive buffer not empty
	unsigned TXE :1; //Bit 1		Transmit buffer empty
	unsigned CHSIDE :1; //Bit 2		Channel side
	unsigned UDR :1; //Bit 3		Underrun flag
	unsigned CRCERR :1; //Bit 4		CRC error flag
	unsigned MODF :1; //Bit 5		Mode fault
	unsigned OVR :1; //Bit 6		Overrun flag
	unsigned BSY :1; //Bit 7		Busy flag
	unsigned FRE :1; //Bit 8		Frame format error
	unsigned RSVD_15_9 :7; //Bits 15:9	Forced to 0 by hardware.
} spi_sr_t;
static_assert(sizeof(spi_sr_t) == SPI_SR_T_SIZE, "Invalid size of spi_sr_t!");

//SPI status register
typedef union _SPI_SR_REG {
	uint16_t all;
	struct SPI_SR_BITS bit;
} SPI_SR_REG;

//SPI data register (SPI_DR)
typedef union _SPI_DR_REG {
	uint16_t all;
} SPI_DR_REG;

//SPI CRC polynomial register
typedef union _SPI_CRCPR_REG {
	uint16_t all;
} SPI_CRCPR_REG;

//SPI RX CRC register
typedef union _SPI_RXCRCR_REG {
	uint16_t all;
} SPI_RXCRCR_REG;

//SPI TX CRC register
typedef union _SPI_TXCRCR_REG {
	uint16_t all;
} SPI_TXCRCR_REG;

//SPI_I2S configuration bits
typedef struct PACKED SPI_I2SCFGR_BITS {
	unsigned CHLEN :1; //Bit 0		Channel length (number of bits per audio channel)
	unsigned DATLEN :2; //Bits 2:1	Data length to be transferred
	unsigned CKPOL :1; //Bit 3		Steady state clock polarity
	unsigned I2SSTD :2; //Bits 5:4	I2S standard selection
	unsigned RSVD_6 :1; //Bit 6		Forced at 0 by hardware
	unsigned PCMSYNC :1; //Bit 7		PCM frame synchronization
	unsigned I2SCFG :2; //Bits 9:8	I2S configuration mode
	unsigned I2SE :1; //Bit 10	I2S Enable
	unsigned I2SMOD :1; //Bit 11	I2S mode selection
	unsigned RSVD_15_12 :4; //Bits 15:12 must be kept at reset value.
} spi_i2scfgr_t;
static_assert(sizeof(spi_i2scfgr_t) == SPI_I2SCFGR_T_SIZE, "Invalid size of spi_i2scfgr_t!");

//SPI_I2S configuration register
typedef union _SPI_I2S_REG {
	uint16_t all;
	struct SPI_I2SCFGR_BITS bit;
} SPI_I2SCFGR_REG;

//SPI_I2S prescaler bits
typedef struct PACKED SPI_I2SPR_BITS {
	unsigned I2SDIV :8; //Bits 7:0 I2S Linear prescaler
	unsigned ODD :1; //Bit 8 Odd factor for the prescaler
	unsigned MCKOE :1; //Bit 9 Master clock output enable
	unsigned RSVD_10_15 :6; //Bits 15:10 Reserved
} spi_i2spr_t;
static_assert(sizeof(spi_i2spr_t) == SPI_I2SPR_T_SIZE, "Invalid size of spi_i2spr_t!");

//SPI_I2S prescaler register
typedef union _SPI_I2SPR_REG {
	uint16_t all;
	struct SPI_I2SPR_BITS bit;
} SPI_I2SPR_REG;

//структура регистров SPI с битовыми полями
typedef struct {
	__IO SPI_CR1_REG CR1; /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
	uint16_t RESERVED0; /*!< Reserved, 0x02                                                           */
	__IO SPI_CR2_REG CR2; /*!< SPI control register 2,                             Address offset: 0x04 */
	uint16_t RESERVED1; /*!< Reserved, 0x06                                                           */
	__IO SPI_SR_REG SR; /*!< SPI status register,                                Address offset: 0x08 */
	uint16_t RESERVED2; /*!< Reserved, 0x0A                                                           */
	__IO SPI_DR_REG DR; /*!< SPI data register,                                  Address offset: 0x0C */
	uint16_t RESERVED3; /*!< Reserved, 0x0E                                                           */
	__IO SPI_CRCPR_REG CRCPR; /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
	uint16_t RESERVED4; /*!< Reserved, 0x12                                                           */
	__IO SPI_RXCRCR_REG RXCRCR; /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
	uint16_t RESERVED5; /*!< Reserved, 0x16                                                           */
	__IO SPI_TXCRCR_REG TXCRCR; /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
	uint16_t RESERVED6; /*!< Reserved, 0x1A                                                           */
	__IO SPI_I2SCFGR_REG I2SCFGR; /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
	uint16_t RESERVED7; /*!< Reserved, 0x1E                                                           */
	__IO SPI_I2SPR_REG I2SPR; /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
	uint16_t RESERVED8; /*!< Reserved, 0x22                                                           */
} BITS_SPI_TypeDef;

//структура настроек
typedef struct {
	SPI_CR1_REG CR1;
	SPI_CR2_REG CR2;
	const gpio_pin_t *NSS;
	uint32_t LD_USEC;
	uint32_t TD_USEC;
	uint32_t NFD_USEC;
} CFG_REG_SPI_TypeDef;

//макрос заполнения структуры настроек
#define SPI_CFG(SPI_CPHA, SPI_CPOL, SPI_MSTR, SPI_BR, SPI_LSBFIRST, SPI_SSI, SPI_SSM, SPI_RXONLY, SPI_DFF, SPI_CRCEN, SPI_BIDIOE, SPI_BIDIMODE, SPI_RXDMAEN, SPI_TXDMAEN, SPI_SSOE, SPI_FRF, SPI_ERRIE, SPI_NSS, LD, TD, NFD) {\
		.CR1.bit.CPHA = SPI_CPHA,/*CR1 Bit 0*/\
		.CR1.bit.CPOL = SPI_CPOL,/*CR1 Bit 1*/\
		.CR1.bit.MSTR = SPI_MSTR,/*CR1 Bit 2*/\
		.CR1.bit.BR = SPI_BR,/*CR1 Bits 5:3*/\
		.CR1.bit.SPE = SPI_SPE_DIS,/*CR1 Bit 6*/\
		.CR1.bit.LSBFIRST = SPI_LSBFIRST,/*CR1 Bit 7*/\
		.CR1.bit.SSI = SPI_SSI,/*CR1 Bit 8*/\
		.CR1.bit.SSM = SPI_SSM,/*CR1 Bit 9*/\
		.CR1.bit.RXONLY = SPI_RXONLY,/*CR1 Bit 10*/\
		.CR1.bit.DFF = SPI_DFF,/*CR1 Bit 11*/\
		.CR1.bit.CRCNEXT = SPI_CRCNEXT_DATA_PHASE,/*CR1 Bit 12*/\
		.CR1.bit.CRCEN = SPI_CRCEN,/*CR1 Bit 13*/\
		.CR1.bit.BIDIOE = SPI_BIDIOE,/*CR1 Bit 14*/\
		.CR1.bit.BIDIMODE = SPI_BIDIMODE,/*CR1 Bit 15*/\
		\
		.CR2.bit.RXDMAEN = SPI_RXDMAEN,/*CR2 Bit 0*/\
		.CR2.bit.TXDMAEN = SPI_TXDMAEN,/*CR2 Bit 1*/\
		.CR2.bit.SSOE = SPI_SSOE,/*Bit 2*/\
		.CR2.bit.FRF = SPI_FRF,/*CR2 Bit 4*/\
		.CR2.bit.ERRIE = SPI_ERRIE,/*CR2 Bit 5*/\
		.CR2.bit.RXNEIE = SPI_RXNEIE_DIS,/*CR2 Bit 6*/\
		.CR2.bit.TXEIE = SPI_TXEIE_DIS,/*CR2 Bit 7*/\
		.NSS = &SPI_NSS,/*NSS pin pointer*/\
		.LD_USEC = LD,\
		.TD_USEC = TD,\
		.NFD_USEC = NFD\
}

//тип порядка байт
typedef enum {
	SPI_BYTE_ORDER_NORMAL, SPI_BYTE_ORDER_REVERSE
} spi_byte_order_t;

//декларация структуры SPI BUS
struct _SPI_BUS_TypeDef;

//тип функции обратного вызова
typedef void (*SPI_BUS_Callback_TypeDef)(struct _SPI_BUS_TypeDef *bus);

//структура управления NSS
typedef struct {
	const gpio_pin_t *pin; //NSS pin
	uint32_t leading_delay_usec; //delay after low NSS
	uint32_t trailing_delay_usec; //delay before high NSS
	uint32_t next_frame_delay_usec; //delay after high NSS
} SPI_BUS_NSS_TypeDef;

typedef struct {
	uint8_t *tx;
	uint8_t *rx;
	size_t count;
} SPI_BUS_FRAME_TypeDef;

typedef void (*spi_bus_callback)(void*);

//структура SPI BUS
typedef struct _SPI_BUS_TypeDef {
	BITS_SPI_TypeDef *spi;
	SPI_SR_REG SR;
	SPI_BUS_NSS_TypeDef nss;
	spi_byte_order_t byte_order;
	SPI_BUS_FRAME_TypeDef *frame;
	volatile size_t frame_count;
	volatile size_t frame_n;
	volatile size_t data_n;
	spi_bus_callback callback;
	void *callback_argument;
	volatile bool done;
} SPI_BUS_TypeDef;

//Инициализация структуры SPI
extern void spi_bus_struct_init(SPI_BUS_TypeDef *bus, SPI_TypeDef *spi);

//Инициализация SPI
extern void spi_bus_open(SPI_BUS_TypeDef *bus, const CFG_REG_SPI_TypeDef *cfg);

//Деинициализация SPI
extern void spi_bus_close(SPI_BUS_TypeDef *bus);

extern void spi_bus_wait(SPI_BUS_TypeDef *bus);

extern void spi_bus_free(SPI_BUS_TypeDef *bus);

//Обработчик прерывания SPI
extern void SPI_BUS_IRQHandler(SPI_BUS_TypeDef *bus);

//Запуск приема/передачи
extern void spi_bus_transfer(
		SPI_BUS_TypeDef *bus,
		SPI_BUS_FRAME_TypeDef *frame_control_array_pointer,
		size_t frame_control_array_amount,
		spi_byte_order_t frame_byte_order,
		spi_bus_callback callback,
		void *callback_argument);

//Запуск приема/передачи из колбека
extern void spi_bus_transfer_from_callback(
		SPI_BUS_TypeDef *bus,
		SPI_BUS_FRAME_TypeDef *frame_control_array_pointer,
		size_t frame_control_array_amount,
		spi_byte_order_t frame_byte_order,
		spi_bus_callback callback,
		void *callback_argument);

#endif /* INC_SPI_H_ */
