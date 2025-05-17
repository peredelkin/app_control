#include "dma.h"
#include "stddef.h"

//NEW DMA BEGIN
static dma_t _dma;

enum {
	DMA_ISR_LOW,
	DMA_ISR_HIGH
};

enum {
	DMA_IFCR_LOW,
	DMA_IFCR_HIGH
};

void dma_controller_init() {
	//DMA
	_dma.dma[DMA_Controller_1] = DMA1;
	_dma.dma[DMA_Controller_2] = DMA2;

	//STREAM
	_dma.stream[DMA_Controller_1][DMA_Stream_0] = DMA1_Stream0;
	_dma.stream[DMA_Controller_1][DMA_Stream_1] = DMA1_Stream1;
	_dma.stream[DMA_Controller_1][DMA_Stream_2] = DMA1_Stream2;
	_dma.stream[DMA_Controller_1][DMA_Stream_3] = DMA1_Stream3;
	_dma.stream[DMA_Controller_1][DMA_Stream_4] = DMA1_Stream4;
	_dma.stream[DMA_Controller_1][DMA_Stream_5] = DMA1_Stream5;
	_dma.stream[DMA_Controller_1][DMA_Stream_6] = DMA1_Stream6;
	_dma.stream[DMA_Controller_1][DMA_Stream_7] = DMA1_Stream7;

	_dma.stream[DMA_Controller_2][DMA_Stream_0] = DMA2_Stream0;
	_dma.stream[DMA_Controller_2][DMA_Stream_1] = DMA2_Stream1;
	_dma.stream[DMA_Controller_2][DMA_Stream_2] = DMA2_Stream2;
	_dma.stream[DMA_Controller_2][DMA_Stream_3] = DMA2_Stream3;
	_dma.stream[DMA_Controller_2][DMA_Stream_4] = DMA2_Stream4;
	_dma.stream[DMA_Controller_2][DMA_Stream_5] = DMA2_Stream5;
	_dma.stream[DMA_Controller_2][DMA_Stream_6] = DMA2_Stream6;
	_dma.stream[DMA_Controller_2][DMA_Stream_7] = DMA2_Stream7;

	//ISR
	_dma.interrupt_mask[DMA_Stream_0].FEIF = DMA_LISR_FEIF0;
	_dma.interrupt_mask[DMA_Stream_0].DMEIF = DMA_LISR_DMEIF0;
	_dma.interrupt_mask[DMA_Stream_0].TEIF = DMA_LISR_TEIF0;
	_dma.interrupt_mask[DMA_Stream_0].HTIF = DMA_LISR_HTIF0;
	_dma.interrupt_mask[DMA_Stream_0].TCIF = DMA_LISR_TCIF0;

	_dma.interrupt_mask[DMA_Stream_1].FEIF = DMA_LISR_FEIF1;
	_dma.interrupt_mask[DMA_Stream_1].DMEIF = DMA_LISR_DMEIF1;
	_dma.interrupt_mask[DMA_Stream_1].TEIF = DMA_LISR_TEIF1;
	_dma.interrupt_mask[DMA_Stream_1].HTIF = DMA_LISR_HTIF1;
	_dma.interrupt_mask[DMA_Stream_1].TCIF = DMA_LISR_TCIF1;

	_dma.interrupt_mask[DMA_Stream_2].FEIF = DMA_LISR_FEIF2;
	_dma.interrupt_mask[DMA_Stream_2].DMEIF = DMA_LISR_DMEIF2;
	_dma.interrupt_mask[DMA_Stream_2].TEIF = DMA_LISR_TEIF2;
	_dma.interrupt_mask[DMA_Stream_2].HTIF = DMA_LISR_HTIF2;
	_dma.interrupt_mask[DMA_Stream_2].TCIF = DMA_LISR_TCIF2;

	_dma.interrupt_mask[DMA_Stream_3].FEIF = DMA_LISR_FEIF3;
	_dma.interrupt_mask[DMA_Stream_3].DMEIF = DMA_LISR_DMEIF3;
	_dma.interrupt_mask[DMA_Stream_3].TEIF = DMA_LISR_TEIF3;
	_dma.interrupt_mask[DMA_Stream_3].HTIF = DMA_LISR_HTIF3;
	_dma.interrupt_mask[DMA_Stream_3].TCIF = DMA_LISR_TCIF3;

	_dma.interrupt_mask[DMA_Stream_4].FEIF = DMA_HISR_FEIF4;
	_dma.interrupt_mask[DMA_Stream_4].DMEIF = DMA_HISR_DMEIF4;
	_dma.interrupt_mask[DMA_Stream_4].TEIF = DMA_HISR_TEIF4;
	_dma.interrupt_mask[DMA_Stream_4].HTIF = DMA_HISR_HTIF4;
	_dma.interrupt_mask[DMA_Stream_4].TCIF = DMA_HISR_TCIF4;

	_dma.interrupt_mask[DMA_Stream_5].FEIF = DMA_HISR_FEIF5;
	_dma.interrupt_mask[DMA_Stream_5].DMEIF = DMA_HISR_DMEIF5;
	_dma.interrupt_mask[DMA_Stream_5].TEIF = DMA_HISR_TEIF5;
	_dma.interrupt_mask[DMA_Stream_5].HTIF = DMA_HISR_HTIF5;
	_dma.interrupt_mask[DMA_Stream_5].TCIF = DMA_HISR_TCIF5;

	_dma.interrupt_mask[DMA_Stream_6].FEIF = DMA_HISR_FEIF6;
	_dma.interrupt_mask[DMA_Stream_6].DMEIF = DMA_HISR_DMEIF6;
	_dma.interrupt_mask[DMA_Stream_6].TEIF = DMA_HISR_TEIF6;
	_dma.interrupt_mask[DMA_Stream_6].HTIF = DMA_HISR_HTIF6;
	_dma.interrupt_mask[DMA_Stream_6].TCIF = DMA_HISR_TCIF6;

	_dma.interrupt_mask[DMA_Stream_7].FEIF = DMA_HISR_FEIF7;
	_dma.interrupt_mask[DMA_Stream_7].DMEIF = DMA_HISR_DMEIF7;
	_dma.interrupt_mask[DMA_Stream_7].TEIF = DMA_HISR_TEIF7;
	_dma.interrupt_mask[DMA_Stream_7].HTIF = DMA_HISR_HTIF7;
	_dma.interrupt_mask[DMA_Stream_7].TCIF = DMA_HISR_TCIF7;

	//init DONE
	_dma.initialized = true;
}

bool dma_initialized() {
	return _dma.initialized;
}

dma_controller_n_t dma_define_controller(dma_n_stream_n_t stream) {
	return ((stream & DMA2_Stream_0) ? DMA_Controller_2 : DMA_Controller_1);
}

dma_stream_n_t dma_define_stream(dma_n_stream_n_t stream) {
	return (stream & DMA_Stream_7);
}

err_t dma_stream_busy_read(dma_controller_n_t controller_n, dma_stream_n_t stream_n) {
	if(dma_initialized() == false) return E_CANCELED;

	if (_dma.busy[controller_n][stream_n]) return E_BUSY;

	return E_NO_ERROR;
}

err_t dma_stream_status_read(dma_controller_n_t controller_n, dma_stream_n_t stream_n) {
	if(dma_initialized() == false) return E_CANCELED;

	uint32_t ISR = (stream_n & DMA_Stream_4) ? _dma.dma[controller_n]->ISR[DMA_ISR_HIGH] : _dma.dma[controller_n]->ISR[DMA_ISR_LOW];


	if (ISR & _dma.interrupt_mask[stream_n].FEIF) {
		return E_DMA_STREAM_FIFO_ERROR;
	}

	if (ISR & _dma.interrupt_mask[stream_n].DMEIF) {
		return E_DMA_STREAM_DIRECT_ERROR;
	}

	if (ISR & _dma.interrupt_mask[stream_n].TEIF) {
		return E_DMA_STREAM_ERROR;
	}

	if (ISR & _dma.interrupt_mask[stream_n].HTIF) {
		return E_DMA_STREAM_HALF_TRANSFER;
	}

	if (ISR & _dma.interrupt_mask[stream_n].TCIF) {
		return E_DMA_STREAM_TRANSFER_COMPLETE;
	}

	return E_NOT_IMPLEMENTED;
}

void dma_stream_status_FEIF_clear(dma_controller_n_t controller_n, dma_stream_n_t stream_n) {
	if(stream_n & DMA_Stream_4) {
		_dma.dma[controller_n]->ISR[DMA_IFCR_HIGH]  = _dma.interrupt_mask[stream_n].FEIF;
	} else {
		_dma.dma[controller_n]->ISR[DMA_IFCR_LOW]  = _dma.interrupt_mask[stream_n].FEIF;
	}
}

void dma_stream_status_DMEIF_clear(dma_controller_n_t controller_n, dma_stream_n_t stream_n) {
	if(stream_n & DMA_Stream_4) {
		_dma.dma[controller_n]->ISR[DMA_IFCR_HIGH]  = _dma.interrupt_mask[stream_n].DMEIF;
	} else {
		_dma.dma[controller_n]->ISR[DMA_IFCR_LOW]  = _dma.interrupt_mask[stream_n].DMEIF;
	}
}

void dma_stream_status_TEIF_clear(dma_controller_n_t controller_n, dma_stream_n_t stream_n) {
	if(stream_n & DMA_Stream_4) {
		_dma.dma[controller_n]->ISR[DMA_IFCR_HIGH]  = _dma.interrupt_mask[stream_n].TEIF;
	} else {
		_dma.dma[controller_n]->ISR[DMA_IFCR_LOW]  = _dma.interrupt_mask[stream_n].TEIF;
	}
}

void dma_stream_status_HTIF_clear(dma_controller_n_t controller_n, dma_stream_n_t stream_n) {
	if(stream_n & DMA_Stream_4) {
		_dma.dma[controller_n]->ISR[DMA_IFCR_HIGH]  = _dma.interrupt_mask[stream_n].HTIF;
	} else {
		_dma.dma[controller_n]->ISR[DMA_IFCR_LOW]  = _dma.interrupt_mask[stream_n].HTIF;
	}
}

void dma_stream_status_TCIF_clear(dma_controller_n_t controller_n, dma_stream_n_t stream_n) {
	if(stream_n & DMA_Stream_4) {
		_dma.dma[controller_n]->ISR[DMA_IFCR_HIGH]  = _dma.interrupt_mask[stream_n].TCIF;
	} else {
		_dma.dma[controller_n]->ISR[DMA_IFCR_LOW]  = _dma.interrupt_mask[stream_n].TCIF;
	}
}
//NEW DMA END

uint32_t dma_stream_fifo_error_interrupt_status_read(old_dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint32_t) (DMA_ISR_FEIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_fifo_error_interrupt_status_clear(old_dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint32_t) (DMA_IFCR_CFEIF << dma_stream->dma_isr_ifcr_mask_shift);
}

uint32_t dma_stream_direct_mode_error_interrupt_read(old_dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint32_t) (DMA_ISR_DMEIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_direct_mode_error_interrupt_clear(old_dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint32_t) (DMA_IFCR_CDMEIF << dma_stream->dma_isr_ifcr_mask_shift);
}

uint32_t dma_stream_transfer_error_interrupt_read(old_dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint32_t) (DMA_ISR_TEIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_transfer_error_interrupt_clear(old_dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint32_t) (DMA_IFCR_CTEIF << dma_stream->dma_isr_ifcr_mask_shift);
}

uint32_t dma_stream_half_transfer_interrupt_read(old_dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint32_t) (DMA_ISR_HTIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_half_transfer_interrupt_clear(old_dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint32_t) (DMA_IFCR_CHTIF << dma_stream->dma_isr_ifcr_mask_shift);
}

uint32_t dma_stream_transfer_complete_interrupt_read(old_dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint32_t) (DMA_ISR_TCIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_transfer_complete_interrupt_clear(old_dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint32_t) (DMA_IFCR_CTCIF << dma_stream->dma_isr_ifcr_mask_shift);
}

void dma_stream_channel_selection(old_dma_t* dma_stream, uint8_t chsel) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_CHSEL);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_CHSEL & (uint32_t) (chsel << DMA_SxCR_CHSEL_SHIFT)));
}

void dma_stream_memory_burst_transfer_configuration(old_dma_t* dma_stream, uint8_t mburst) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_MBURST);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_MBURST & (uint32_t) (mburst << DMA_SxCR_MBURST_SHIFT)));
}

void dma_stream_peripheral_burst_transfer_configuration(old_dma_t* dma_stream, uint8_t pburst) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PBURST);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_PBURST & (uint32_t) (pburst << DMA_SxCR_PBURST_SHIFT)));
}

void dma_stream_current_target(old_dma_t* dma_stream, bool ct) {
    if (ct) SET_BIT(dma_stream->stream->CR, DMA_SxCR_CT);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_CT);
}

bool dma_stream_current_target_read(old_dma_t* dma_stream) {
    if (READ_BIT(dma_stream->stream->CR, DMA_SxCR_CT)) return 1;
    else return 0;
}

void dma_stream_double_buffer_mode(old_dma_t* dma_stream, bool dbm) {
    if (dbm) SET_BIT(dma_stream->stream->CR,DMA_SxCR_DBM);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_DBM);
}

void dma_stream_priority_level(old_dma_t* dma_stream, uint8_t pl) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PL);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_PL & (uint32_t) (pl << DMA_SxCR_PL_SHIFT)));
}

void dma_stream_peripheral_increment_offset_size(old_dma_t* dma_stream, bool pincos) {
    if (pincos) SET_BIT(dma_stream->stream->CR, DMA_SxCR_PINCOS);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PINCOS);
}

void dma_stream_memory_data_size(old_dma_t* dma_stream, uint8_t msize) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_MSIZE);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_MSIZE & (uint32_t) (msize << DMA_SxCR_MSIZE_SHIFT)));
}

void dma_stream_peripheral_data_size(old_dma_t* dma_stream, uint8_t psize) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PSIZE);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_PSIZE & (uint32_t) (psize << DMA_SxCR_PSIZE_SHIFT)));
}

void dma_stream_memory_increment_mode(old_dma_t* dma_stream, bool minc) {
    if (minc) SET_BIT(dma_stream->stream->CR, DMA_SxCR_MINC);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_MINC);
}

void dma_stream_peripheral_increment_mode(old_dma_t* dma_stream, bool pinc) {
    if (pinc) SET_BIT(dma_stream->stream->CR, DMA_SxCR_PINC);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PINC);
}

void dma_stream_circular_mode(old_dma_t* dma_stream, bool circ) {
    if (circ) SET_BIT(dma_stream->stream->CR, DMA_SxCR_CIRC);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_CIRC);
}

void dma_stream_data_transfer_direction(old_dma_t* dma_stream, uint8_t dir) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_DIR);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_DIR & (uint32_t) (dir << DMA_SxCR_DIR_SHIFT)));
}

void dma_stream_peripheral_flow_controller(old_dma_t* dma_stream, bool pfctrl) {
    if (pfctrl) SET_BIT(dma_stream->stream->CR, DMA_SxCR_PFCTRL);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PFCTRL);
}

void dma_stream_transfer_complete_interrupt_enable(old_dma_t* dma_stream, bool tcie) {
    if (tcie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_TCIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_TCIE);
}

void dma_stream_half_transfer_interrupt_enable(old_dma_t* dma_stream, bool htie) {
    if (htie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_HTIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_HTIE);
}

void dma_stream_transfer_error_interrupt_enable(old_dma_t* dma_stream, bool teie) {
    if (teie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_TEIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_TEIE);
}

void dma_stream_direct_mode_error_interrupt_enable(old_dma_t* dma_stream, bool dmeie) {
    if (dmeie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_DMEIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_DMEIE);
}

void dma_stream_enable(old_dma_t* dma_stream, bool en) {
    if (en) SET_BIT(dma_stream->stream->CR, DMA_SxCR_EN);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_EN);
}

bool dma_stream_ready(old_dma_t* dma_stream) {
    if (READ_BIT(dma_stream->stream->CR, DMA_SxCR_EN)) return 0; //not ready
    else return 1; //ready
}

void dma_stream_number_of_data(old_dma_t* dma_stream,uint16_t ndtr) {
    dma_stream->stream->NDTR = ndtr;
}

uint16_t dma_stream_number_of_data_read(old_dma_t* dma_stream) {
    return dma_stream->stream->NDTR;
}

void dma_stream_peripheral_address(old_dma_t* dma_stream,uint32_t par) {
    dma_stream->stream->PAR = par;
}

void dma_stream_memory_address(old_dma_t* dma_stream,uint8_t mar_n,uint32_t mar) {
    dma_stream->stream->MAR[mar_n] = mar;
}

void dma_stream_struct_init(old_dma_t* dma_stream, DMA_TypeDef* dma, DMA_Stream_TypeDef* stream, uint8_t stream_n) {
    dma_stream->dma = dma;
    dma_stream->stream = stream;
    dma_stream->dma_isr_ifcr_n = (uint8_t) (stream_n/4);
    dma_stream->dma_isr_ifcr_mask_shift = (uint8_t)((stream_n%4)*8)-((stream_n%2)*2);
}

void dma_stream_struct_deinit(old_dma_t* dma_stream) {
    dma_stream->dma = NULL;
    dma_stream->stream = NULL;
    dma_stream->dma_isr_ifcr_n = 0;
    dma_stream->dma_isr_ifcr_mask_shift = 0;
}

void dma_stream_deinit(old_dma_t* dma_stream) {
    dma_stream_enable(dma_stream,false);
    dma_stream->stream->CR = 0;
    dma_stream->stream->NDTR = 0;
}
