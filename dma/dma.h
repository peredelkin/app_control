
#ifndef DMA_H
#define DMA_H

#include <stdbool.h>
#include <assert.h>

#include "lib/stm32f4xx/stm32f4xx.h"
#include "lib/errors/errors.h"

#define DMA_STREAM_MAKE_CFG(dbm,\
							ct,\
							chsel,\
							dmdis,\
							fth,\
							msize,\
							mburst,\
							minc,\
							psize,\
							pburst,\
							pinc,\
							pincos,\
							dir,\
							pfctrl,\
							circ,\
							tcie,\
							htie,\
							teie,\
							dmeie,\
							feie,\
							pl,\
							en)\
{\
.scr.bit.EN = en,\
.scr.bit.DMEIE = dmeie,\
.scr.bit.TEIE = teie,\
.scr.bit.HTIE = htie,\
.scr.bit.TCIE = tcie,\
.scr.bit.PFCTRL = pfctrl,\
.scr.bit.DIR = dir,\
.scr.bit.CIRC = circ,\
.scr.bit.PINC = pinc,\
.scr.bit.MINC = minc,\
.scr.bit.PSIZE = psize,\
.scr.bit.MSIZE = msize,\
.scr.bit.PINCOS = pincos,\
.scr.bit.PL = pl,\
.scr.bit.DBM = dbm,\
.scr.bit.CT = ct,\
.scr.bit.PBURST = pburst,\
.scr.bit.MBURST = mburst,\
.scr.bit.CHSEL = chsel,\
.fscr.bit.FTH = fth,\
.fscr.bit.DMDIS = dmdis,\
.fscr.bit.FEIE = feie\
}

#define DMA_DATA_COUNT_MAX 65535

#define DMA_CONTROLLERS_COUNT 2
#define DMA_CONTROLLER_STREAMS_COUNT 8

//! Stream x transfer complete interrupt flag
#define E_DMA_STREAM_TRANSFER_COMPLETE E_NO_ERROR
//! Stream x half transfer interrupt flag
#define E_DMA_STREAM_HALF_TRANSFER (E_DMA_BASE + 1)
//! Stream x transfer error interrupt flag
#define E_DMA_STREAM_ERROR (E_DMA_BASE + 2)
//! Stream x direct mode error interrupt flag
#define E_DMA_STREAM_DIRECT_ERROR (E_DMA_BASE + 3)
//! Stream x FIFO error interrupt flag
#define E_DMA_STREAM_FIFO_ERROR (E_DMA_BASE + 4)

typedef enum {
	DMA1_Stream_0,//!< DMA1_Stream0
	DMA1_Stream_1,//!< DMA1_Stream1
	DMA1_Stream_2,//!< DMA1_Stream2
	DMA1_Stream_3,//!< DMA1_Stream3
	DMA1_Stream_4,//!< DMA1_Stream4
	DMA1_Stream_5,//!< DMA1_Stream5
	DMA1_Stream_6,//!< DMA1_Stream6
	DMA1_Stream_7,//!< DMA1_Stream7

	DMA2_Stream_0,//!< DMA2_Stream0
	DMA2_Stream_1,//!< DMA2_Stream1
	DMA2_Stream_2,//!< DMA2_Stream2
	DMA2_Stream_3,//!< DMA2_Stream3
	DMA2_Stream_4,//!< DMA2_Stream4
	DMA2_Stream_5,//!< DMA2_Stream5
	DMA2_Stream_6,//!< DMA2_Stream6
	DMA2_Stream_7 //!< DMA2_Stream7
} dma_n_stream_n_t;

//настройки структуры
typedef enum {
	DMA_Controller_1,
	DMA_Controller_2
} dma_controller_n_t;

typedef enum {
	DMA_Stream_0,
	DMA_Stream_1,
	DMA_Stream_2,
	DMA_Stream_3,
	DMA_Stream_4,
	DMA_Stream_5,
	DMA_Stream_6,
	DMA_Stream_7
} dma_stream_n_t;

typedef enum {
	DMA_ISR_IFCR_LOW,
	DMA_ISR_IFCR_HIGH
} dma_isr_ifcr_n_t;

//настройки стрима

//Stream enable / flag stream ready when read low
typedef enum {
	DMA_SCR_EN_DIS,
	DMA_SCR_EN_ENA
} dma_scr_en_t;

//Direct mode error interrupt enable
typedef enum {
	DMA_SCR_DMEIE_DIS,
	DMA_SCR_DMEIE_ENA
} dma_scr_dmeie_t;

//Transfer error interrupt enable
typedef enum {
	DMA_SCR_TEIE_DIS,
	DMA_SCR_TEIE_ENA
} dma_scr_teie_t;

//Half transfer interrupt enable
typedef enum {
	DMA_SCR_HTIE_DIS,
	DMA_SCR_HTIE_ENA
} dma_scr_htie_t;

//Transfer complete interrupt enable
typedef enum {
	DMA_SCR_TCIE_DIS,
	DMA_SCR_TCIE_ENA
} dma_scr_tcie_t;

//Peripheral flow controller
typedef enum {
	DMA_SCR_PFCTRL_DIS,
	DMA_SCR_PFCTRL_ENA
} dma_scr_pfctrl_t;

//Data transfer direction
typedef enum {
	DMA_SCR_DIR_PERI_TO_MEM,
	DMA_SCR_DIR_MEM_TO_PERI,
	DMA_SCR_DIR_MEM_TO_MEM
} dma_scr_dir_t;

//Circular mode
typedef enum {
	DMA_SCR_CIRC_DIS,
	DMA_SCR_CIRC_ENA
} dma_scr_circ_t;

//Peripheral increment mode
typedef enum {
	DMA_SCR_PINC_DIS,
	DMA_SCR_PINC_ENA
} dma_scr_pinc_t;

//Memory increment mode
typedef enum {
	DMA_SCR_MINC_DIS,
	DMA_SCR_MINC_ENA
} dma_scr_minc_t;

//Peripheral data size
typedef enum {
	DMA_SCR_PSIZE_8,
	DMA_SCR_PSIZE_16,
	DMA_SCR_PSIZE_32
} dma_scr_psize_t;

//Memory data size
typedef enum {
	DMA_SCR_MSIZE_8,
	DMA_SCR_MSIZE_16,
	DMA_SCR_MSIZE_32
} dma_scr_msize_t;

//Peripheral increment offset size
typedef enum {
	DMA_SCR_PINCOS_PSIZE,
	DMA_SCR_PINCOS_32
} dma_scr_pincos_t;

//Priority level
typedef enum {
	DMA_SCR_PL_LOW,
	DMA_SCR_PL_MEDIUM,
	DMA_SCR_PL_HIGH,
	DMA_SCR_PL_VERY_HIGH
} dma_scr_pl_t;

//Double buffer mode
typedef enum {
	DMA_SCR_DBM_DIS,
	DMA_SCR_DBM_ENA
} dma_scr_dbm_t;

//Current target (only in double buffer mode)
typedef enum {
	DMA_SCR_CT_MEM0,
	DMA_SCR_CT_MEM1
} dma_scr_ct_t;

//Peripheral burst transfer configuration
typedef enum {
	DMA_SCR_PBURST_DIS,
	DMA_SCR_PBURST_4,
	DMA_SCR_PBURST_8,
	DMA_SCR_PBURST_16
} dma_scr_pburst_t;

//Memory burst transfer configuration
typedef enum {
	DMA_SCR_MBURST_DIS,
	DMA_SCR_MBURST_4,
	DMA_SCR_MBURST_8,
	DMA_SCR_MBURST_16
} dma_scr_mburst_t;

//Channel selection
typedef enum {
	DMA_SCR_CHSEL_0,
	DMA_SCR_CHSEL_1,
	DMA_SCR_CHSEL_2,
	DMA_SCR_CHSEL_3,
	DMA_SCR_CHSEL_4,
	DMA_SCR_CHSEL_5,
	DMA_SCR_CHSEL_6,
	DMA_SCR_CHSEL_7,
} dma_scr_chsel_t;

//FIFO threshold selection
typedef enum {
	DMA_FCR_FTH_ONE_FOURTH, //!< 1/4 full FIFO
	DMA_FCR_FTH_ONE_SECOND, //!< 1/2 full FIFO
	DMA_FCR_FTH_THREE_FOURTHS, //!< 3/4 full FIFO
	DMA_FCR_FTH_FULL //!< full FIFO
} dma_fcr_fth_t;

//Direct mode disable
typedef enum {
	DMA_FCR_DMDIS_DIS, //!< Direct mode enabled
	DMA_FCR_DMDIS_ENA //!< Direct mode disabled
} dma_fcr_dmdis_t;

//FIFO status
typedef enum {
	DMA_FCR_FS_0, //!< 0 < fifo_level < 1/4
	DMA_FCR_FS_1, //!< 1/4 ≤ fifo_level < 1/2
	DMA_FCR_FS_2, //!< 1/2 ≤ fifo_level < 3/4
	DMA_FCR_FS_3, //!< 3/4 ≤ fifo_level < full
	DMA_FCR_FS_EMPTY, //!< FIFO is empty
	DMA_FCR_FS_FULL //! FIFO is full
} dma_fcr_fs_t;

//FIFO error interrupt enable
typedef enum {
	DMA_FCR_FEIE_DIS,
	DMA_FCR_FEIE_ENA
} dma_fcr_feie_t;

#define DMA_SCR_WIDTH 4

typedef union {
	uint32_t all;
	struct {
		unsigned EN			:1;	//!< EN: Stream enable / flag stream ready when read low
		unsigned DMEIE		:1; //!< DMEIE: Direct mode error interrupt enable
		unsigned TEIE		:1; //!< TEIE: Transfer error interrupt enable
		unsigned HTIE		:1; //!< HTIE: Half transfer interrupt enable
		unsigned TCIE		:1; //!< TCIE: Transfer complete interrupt enable
		unsigned PFCTRL 	:1; //!< PFCTRL: Peripheral flow controller
		unsigned DIR		:2; //!< [1:0] DIR: Data transfer direction
		unsigned CIRC		:1; //!< CIRC: Circular mode
		unsigned PINC		:1; //!< PINC: Peripheral increment mode
		unsigned MINC		:1; //!< MINC: Memory increment mode
		unsigned PSIZE		:2; //!< [1:0] PSIZE: Peripheral data size
		unsigned MSIZE		:2; //!< [1:0] MSIZE: Memory data size
		unsigned PINCOS		:1; //!< PINCOS: Peripheral increment offset size
		unsigned PL			:2; //!< [1:0] PL: Priority level
		unsigned DBM		:1; //!< DBM: Double buffer mode
		unsigned CT			:1; //!< CT: Current target (only in double buffer mode)
		unsigned RES_20		:1;
		unsigned PBURST		:2; //!< [1:0] PBURST: Peripheral burst transfer configuration
		unsigned MBURST		:2; //!< [1:0] MBURST: Memory burst transfer configuration
		unsigned CHSEL		:3; //!< [2:0] CHSEL: Channel selection
		unsigned RES_28_31	:4;
	} bit;
} dma_scr_t;
static_assert(sizeof(dma_scr_t) == DMA_SCR_WIDTH, "Invalid size of dma_scr_t!");

#define DMA_SFCR_WIDTH 4

typedef union {
	uint32_t all;
	struct {
		unsigned FTH		:2;
		unsigned DMDIS		:1;
		unsigned FS			:3;
		unsigned RES_6		:1;
		unsigned FEIE		:1;
		unsigned RES_8_31	:24;
	} bit;
} dma_sfcr_t;
static_assert(sizeof(dma_sfcr_t) == DMA_SFCR_WIDTH, "Invalid size of dma_sfcr_t!");

typedef struct {
	dma_scr_t scr;
	dma_sfcr_t fscr;
} dma_stream_settings_t;

typedef struct {
	dma_controller_n_t controller;
	dma_stream_n_t stream;
	dma_isr_ifcr_n_t status;
	bool initialized;
} dma_t;

/*
 * Инициализация структуры DMA контроллера.
 * Вызывается один раз, до использовая другими.
 */
extern void dma_controller_init();

extern err_t dma_struct_init(dma_t* dma, dma_n_stream_n_t stream);

extern err_t dma_stream_open(dma_t* dma);

extern uint32_t dma_stream_status_register_read(dma_t* dma);

extern bool dma_stream_status_FEIF_read(uint32_t ISR, dma_t* dma);

extern bool dma_stream_status_DMEIF_read(uint32_t ISR, dma_t* dma);

extern bool dma_stream_status_TEIF_read(uint32_t ISR, dma_t* dma);

extern bool dma_stream_status_HTIF_read(uint32_t ISR, dma_t* dma);

extern bool dma_stream_status_TCIF_read(uint32_t ISR, dma_t* dma);

extern void dma_stream_status_FEIF_clear(dma_t* dma);

extern void dma_stream_status_DMEIF_clear(dma_t* dma);

extern void dma_stream_status_TEIF_clear(dma_t* dma);

extern void dma_stream_status_HTIF_clear(dma_t* dma);

extern void dma_stream_status_TCIF_clear(dma_t* dma);

extern void dma_stream_callback_set(dma_t* dma, void(*callback));

extern bool dma_stream_enable(dma_t* dma);

extern bool dma_stream_disable(dma_t* dma);

extern void dma_stream_setup(dma_t *dma, dma_stream_settings_t* settings);

extern uint32_t dma_stream_number_of_data_register_read(dma_t* dma);

extern void dma_stream_number_of_data_register_write(dma_t* dma, uint32_t NDTR);

extern uint32_t dma_stream_peripheral_address_register_read(dma_t* dma);

extern void dma_stream_peripheral_address_register_write(dma_t* dma, uint32_t PAR);

extern uint32_t dma_stream_memory_0_address_register_read(dma_t* dma);

extern void dma_stream_memory_0_address_register_write(dma_t* dma, uint32_t M0AR);

extern uint32_t dma_stream_memory_1_address_register_read(dma_t* dma);

extern void dma_stream_memory_1_address_register_write(dma_t* dma, uint32_t M1AR);

extern void dma_stream_deinit(dma_t* dma);

extern void dma_stream_close(dma_t* dma);


#endif /* DMA_H */

