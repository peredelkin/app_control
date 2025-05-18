
#ifndef DMA_H
#define DMA_H

#include "lib/stm32f4xx/stm32f4xx.h"
#include "lib/errors/errors.h"
#include <stdbool.h>

//NEW DMA BEGIN
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
typedef enum {
	DMA_SCR_EN_DIS,
	DMA_SCR_EN_ENA
} dma_scr_en_t;

typedef struct {
	dma_controller_n_t controller;
	dma_stream_n_t stream;
	dma_isr_ifcr_n_t status;
	bool initialized;
} dma_t;
//NEW DMA END

#define DMA_ISR_FEIF DMA_LISR_FEIF0
#define DMA_IFCR_CFEIF DMA_LIFCR_CFEIF0

#define DMA_ISR_DMEIF DMA_LISR_DMEIF0
#define DMA_IFCR_CDMEIF DMA_LIFCR_CDMEIF0

#define DMA_ISR_TEIF DMA_LISR_TEIF0
#define DMA_IFCR_CTEIF DMA_LIFCR_CTEIF0

#define DMA_ISR_HTIF DMA_LISR_HTIF0
#define DMA_IFCR_CHTIF DMA_LIFCR_CHTIF0

#define DMA_ISR_TCIF DMA_LISR_TCIF0
#define DMA_IFCR_CTCIF DMA_LIFCR_CTCIF0

#define DMA_SxCR_CHSEL_SHIFT 25
#define DMA_SxCR_MBURST_SHIFT 23
#define DMA_SxCR_PBURST_SHIFT 21
#define DMA_SxCR_PL_SHIFT 16
#define DMA_SxCR_MSIZE_SHIFT 13
#define DMA_SxCR_PSIZE_SHIFT 11
#define DMA_SxCR_DIR_SHIFT 6

#define DMA_DATA_COUNT_MAX 65535

typedef struct {
    DMA_Stream_TypeDef* stream;
    DMA_TypeDef* dma;
    uint8_t dma_isr_ifcr_n; //номер ISR/IFCR
    uint8_t dma_isr_ifcr_mask_shift; //сдвиг маски ISR/IFCR
} old_dma_t;

extern uint32_t dma_stream_fifo_error_interrupt_status_read(old_dma_t* dma_stream);

extern void dma_stream_fifo_error_interrupt_status_clear(old_dma_t* dma_stream);

extern uint32_t dma_stream_direct_mode_error_interrupt_read(old_dma_t* dma_stream);

extern void dma_stream_direct_mode_error_interrupt_clear(old_dma_t* dma_stream);

extern uint32_t dma_stream_transfer_error_interrupt_read(old_dma_t* dma_stream);

extern void dma_stream_transfer_error_interrupt_clear(old_dma_t* dma_stream);

extern uint32_t dma_stream_half_transfer_interrupt_read(old_dma_t* dma_stream);

extern void dma_stream_half_transfer_interrupt_clear(old_dma_t* dma_stream);

extern uint32_t dma_stream_transfer_complete_interrupt_read(old_dma_t* dma_stream);

extern void dma_stream_transfer_complete_interrupt_clear(old_dma_t* dma_stream);

extern void dma_stream_channel_selection(old_dma_t* dma_stream, uint8_t chsel);

extern void dma_stream_memory_burst_transfer_configuration(old_dma_t* dma_stream, uint8_t mburst);

extern void dma_stream_peripheral_burst_transfer_configuration(old_dma_t* dma_stream, uint8_t pburst);

extern void dma_stream_current_target(old_dma_t* dma_stream, bool ct);

extern bool dma_stream_current_target_read(old_dma_t* dma_stream);

extern void dma_stream_double_buffer_mode(old_dma_t* dma_stream, bool dbm);

extern void dma_stream_priority_level(old_dma_t* dma_stream, uint8_t pl);

extern void dma_stream_peripheral_increment_offset_size(old_dma_t* dma_stream, bool pincos);

extern void dma_stream_memory_data_size(old_dma_t* dma_stream, uint8_t msize);

extern void dma_stream_peripheral_data_size(old_dma_t* dma_stream, uint8_t psize);

extern void dma_stream_memory_increment_mode(old_dma_t* dma_stream, bool minc);

extern void dma_stream_peripheral_increment_mode(old_dma_t* dma_stream, bool pinc);

extern void dma_stream_circular_mode(old_dma_t* dma_stream, bool circ);

extern void dma_stream_data_transfer_direction(old_dma_t* dma_stream, uint8_t dir);

extern void dma_stream_peripheral_flow_controller(old_dma_t* dma_stream, bool pfctrl);

extern void dma_stream_transfer_complete_interrupt_enable(old_dma_t* dma_stream, bool tcie);

extern void dma_stream_half_transfer_interrupt_enable(old_dma_t* dma_stream, bool htie);

extern void dma_stream_transfer_error_interrupt_enable(old_dma_t* dma_stream, bool teie);

extern void dma_stream_direct_mode_error_interrupt_enable(old_dma_t* dma_stream, bool dmeie);

extern void dma_stream_enable(old_dma_t* dma_stream, bool en);

extern bool dma_stream_ready(old_dma_t* dma_stream);

extern void dma_stream_number_of_data(old_dma_t* dma_stream,uint16_t ndtr);

extern uint16_t dma_stream_number_of_data_read(old_dma_t* dma_stream);

extern void dma_stream_peripheral_address(old_dma_t* dma_stream,uint32_t par);

extern void dma_stream_memory_address(old_dma_t* dma_stream,uint8_t mar_n,uint32_t mar);

extern void dma_stream_struct_init(old_dma_t* dma_stream, DMA_TypeDef* dma, DMA_Stream_TypeDef* stream, uint8_t stream_n);

extern void dma_stream_struct_deinit(old_dma_t* dma_stream);

extern void dma_stream_deinit(old_dma_t* dma_stream);

#endif /* DMA_H */

