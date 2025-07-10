#include "dma.h"
#include "stddef.h"

typedef struct {
	uint32_t FEIF;
	uint32_t DMEIF;
	uint32_t TEIF;
	uint32_t HTIF;
	uint32_t TCIF;
} _dma_interrupt_mask_t;

typedef struct {
	DMA_TypeDef* dma [DMA_CONTROLLERS_COUNT];
	DMA_Stream_TypeDef* stream [DMA_CONTROLLERS_COUNT][DMA_CONTROLLER_STREAMS_COUNT];
	bool busy [DMA_CONTROLLERS_COUNT][DMA_CONTROLLER_STREAMS_COUNT];
	_dma_interrupt_mask_t interrupt_mask [DMA_CONTROLLER_STREAMS_COUNT];
	void(*callback [DMA_CONTROLLERS_COUNT][DMA_CONTROLLER_STREAMS_COUNT])();
	bool initialized;
} _dma_controller_t;

static _dma_controller_t _dma;

static void _dma_stream_interrups_disable_all(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n);

static inline void DMA_Stream_IRQHandler(dma_controller_n_t controller_n, dma_stream_n_t stream_n) {
	if(_dma.callback[controller_n][stream_n] != NULL) {
		_dma.callback[controller_n][stream_n]();
	} else {
		_dma_stream_interrups_disable_all(controller_n, stream_n);
	}
}

void DMA1_Stream0_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_1, DMA_Stream_0);
}

void DMA1_Stream1_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_1, DMA_Stream_1);
}

void DMA1_Stream2_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_1, DMA_Stream_2);
}

void DMA1_Stream3_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_1, DMA_Stream_3);
}

void DMA1_Stream4_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_1, DMA_Stream_4);
}

void DMA1_Stream5_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_1, DMA_Stream_5);
}

void DMA1_Stream6_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_1, DMA_Stream_6);
}

void DMA1_Stream7_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_1, DMA_Stream_7);
}

void DMA2_Stream0_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_2, DMA_Stream_0);
}

void DMA2_Stream1_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_2, DMA_Stream_1);
}

void DMA2_Stream2_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_2, DMA_Stream_2);
}

void DMA2_Stream3_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_2, DMA_Stream_3);
}

void DMA2_Stream4_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_2, DMA_Stream_4);
}

void DMA2_Stream5_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_2, DMA_Stream_5);
}

void DMA2_Stream6_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_2, DMA_Stream_6);
}

void DMA2_Stream7_IRQHandler() {
	DMA_Stream_IRQHandler(DMA_Controller_2, DMA_Stream_7);
}

void dma_controller_init() {
	//RCC DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

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

static bool _dma_initialized() {
	return _dma.initialized;
}

static void _dma_stream_callback_set(dma_controller_n_t controller_n, dma_stream_n_t stream_n, void(*callback)) {
	_dma.callback[controller_n][stream_n] = callback;
}

static dma_controller_n_t _dma_define_controller(dma_n_stream_n_t stream) {
	return ((stream & DMA2_Stream_0) ? DMA_Controller_2 : DMA_Controller_1);
}

static dma_stream_n_t _dma_define_stream(dma_n_stream_n_t stream) {
	return (stream & DMA_Stream_7);
}

static dma_isr_ifcr_n_t _dma_define_status(dma_n_stream_n_t stream) {
	return (stream & DMA_Stream_4) ? DMA_ISR_IFCR_HIGH : DMA_ISR_IFCR_LOW;
}

static bool _dma_stream_busy_read(dma_controller_n_t controller_n, dma_stream_n_t stream_n) {
	return _dma.busy[controller_n][stream_n];
}

static void _dma_stream_busy_write(dma_controller_n_t controller_n, dma_stream_n_t stream_n, bool busy) {
	_dma.busy[controller_n][stream_n] = busy;
}

static uint32_t _dma_stream_status_register_read(dma_controller_n_t controller_n, dma_isr_ifcr_n_t status) {
	return _dma.dma[controller_n]->ISR[status];
}

static bool _dma_stream_status_FEIF_read(uint32_t ISR, dma_stream_n_t stream_n) {
	if (ISR & _dma.interrupt_mask[stream_n].FEIF) return true;
	return false;
}

static bool _dma_stream_status_DMEIF_read(uint32_t ISR, dma_stream_n_t stream_n) {
	if (ISR & _dma.interrupt_mask[stream_n].DMEIF) return true;
	return false;
}

static bool _dma_stream_status_TEIF_read(uint32_t ISR, dma_stream_n_t stream_n) {
	if (ISR & _dma.interrupt_mask[stream_n].TEIF) return true;
	return false;
}

static bool _dma_stream_status_HTIF_read(uint32_t ISR, dma_stream_n_t stream_n) {
	if (ISR & _dma.interrupt_mask[stream_n].HTIF) return true;
	return false;
}

static bool _dma_stream_status_TCIF_read(uint32_t ISR, dma_stream_n_t stream_n) {
	if (ISR & _dma.interrupt_mask[stream_n].TCIF) return true;
	return false;
}

static void _dma_stream_status_FEIF_clear(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		dma_isr_ifcr_n_t status) {
	_dma.dma[controller_n]->IFCR[status] = _dma.interrupt_mask[stream_n].FEIF;
}

static void _dma_stream_status_DMEIF_clear(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		dma_isr_ifcr_n_t status) {
	_dma.dma[controller_n]->IFCR[status] = _dma.interrupt_mask[stream_n].DMEIF;
}

static void _dma_stream_status_TEIF_clear(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		dma_isr_ifcr_n_t status) {
	_dma.dma[controller_n]->IFCR[status] = _dma.interrupt_mask[stream_n].TEIF;
}

static void _dma_stream_status_HTIF_clear(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		dma_isr_ifcr_n_t status) {
	_dma.dma[controller_n]->IFCR[status] = _dma.interrupt_mask[stream_n].HTIF;
}

static void _dma_stream_status_TCIF_clear(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		dma_isr_ifcr_n_t status) {
	_dma.dma[controller_n]->IFCR[status] = _dma.interrupt_mask[stream_n].TCIF;
}

static uint32_t _dma_stream_control_register_read(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n) {
	return _dma.stream[controller_n][stream_n]->CR;
}

static void _dma_stream_control_register_write(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		uint32_t CR) {
	_dma.stream[controller_n][stream_n]->CR = CR;
}

static uint32_t _dma_stream_fifo_control_register_read(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n) {
	return _dma.stream[controller_n][stream_n]->FCR;
}

static void _dma_stream_fifo_control_register_write(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		uint32_t FCR) {
	_dma.stream[controller_n][stream_n]->FCR = FCR;
}

static uint32_t _dma_stream_number_of_data_register_read(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n) {
	return _dma.stream[controller_n][stream_n]->NDTR;
}

static void _dma_stream_number_of_data_register_write(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		uint32_t NDTR) {
	_dma.stream[controller_n][stream_n]->NDTR = NDTR;
}

static uint32_t _dma_stream_peripheral_address_register_read(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n) {
	return _dma.stream[controller_n][stream_n]->PAR;
}

static void _dma_stream_peripheral_address_register_write(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		uint32_t PAR) {
	_dma.stream[controller_n][stream_n]->PAR = PAR;
}

static uint32_t _dma_stream_memory_0_address_register_read(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n) {
	return _dma.stream[controller_n][stream_n]->M0AR;
}

static void _dma_stream_memory_0_address_register_write(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		uint32_t M0AR) {
	_dma.stream[controller_n][stream_n]->M0AR = M0AR;
}

static uint32_t _dma_stream_memory_1_address_register_read(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n) {
	return _dma.stream[controller_n][stream_n]->M1AR;
}

static void _dma_stream_memory_1_address_register_write(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n,
		uint32_t M1AR) {
	_dma.stream[controller_n][stream_n]->M1AR = M1AR;
}

static void _dma_stream_interrups_disable_all(
		dma_controller_n_t controller_n,
		dma_stream_n_t stream_n) {

	dma_scr_t scr;
	dma_sfcr_t fscr;

	scr.all = _dma_stream_control_register_read(controller_n, stream_n);
	fscr.all = _dma_stream_fifo_control_register_read(controller_n, stream_n);

	scr.bit.DMEIE = DMA_SCR_DMEIE_DIS;
	scr.bit.TEIE = DMA_SCR_TEIE_DIS;
	scr.bit.HTIE = DMA_SCR_HTIE_DIS;
	scr.bit.TCIE = DMA_SCR_TCIE_DIS;

	fscr.bit.FEIE = DMA_FCR_FEIE_DIS;

	_dma_stream_control_register_write(controller_n, stream_n, scr.all);
	_dma_stream_fifo_control_register_write(controller_n, stream_n, fscr.all);
}

//функции пользователя
err_t dma_struct_init(dma_t* dma, dma_n_stream_n_t stream) {
	if(dma == NULL) return E_NULL_POINTER;

	dma->initialized = false;

	if(_dma_initialized() == false) return E_CANCELED;

	dma->controller = _dma_define_controller(stream);
	dma->stream = _dma_define_stream(stream);
	dma->status = _dma_define_status(stream);
	dma->initialized = true;

	return E_NO_ERROR;
}

err_t dma_stream_open(dma_t* dma) {
	if(dma == NULL) return E_NULL_POINTER;

	if(dma->initialized == false) return E_CANCELED;

	if(_dma_initialized() == false) return E_CANCELED;

	if(_dma_stream_busy_read(dma->controller, dma->stream) == true) {
		return E_BUSY;
	}

	_dma_stream_busy_write(dma->controller, dma->stream, true);

	return E_NO_ERROR;
}

uint32_t dma_stream_status_register_read(dma_t* dma) {
	if (dma->initialized == false) return 0;
	return _dma_stream_status_register_read(dma->controller, dma->status);
}

bool dma_stream_status_FEIF_read(uint32_t ISR, dma_t* dma) {
	if (dma->initialized == false) return false;
	return _dma_stream_status_FEIF_read(ISR, dma->stream);
}

bool dma_stream_status_DMEIF_read(uint32_t ISR, dma_t* dma) {
	if (dma->initialized == false) return false;
	return _dma_stream_status_DMEIF_read(ISR, dma->stream);
}

bool dma_stream_status_TEIF_read(uint32_t ISR, dma_t* dma) {
	if (dma->initialized == false) return false;
	return _dma_stream_status_TEIF_read(ISR, dma->stream);
}

bool dma_stream_status_HTIF_read(uint32_t ISR, dma_t* dma) {
	if (dma->initialized == false) return false;
	return _dma_stream_status_HTIF_read(ISR, dma->stream);
}

bool dma_stream_status_TCIF_read(uint32_t ISR, dma_t* dma) {
	if (dma->initialized == false) return false;
	return _dma_stream_status_TCIF_read(ISR, dma->stream);
}

void dma_stream_status_FEIF_clear(dma_t* dma) {
	if (dma->initialized == false) return;
	_dma_stream_status_FEIF_clear(dma->controller, dma->stream, dma->status);
}

void dma_stream_status_DMEIF_clear(dma_t* dma) {
	if (dma->initialized == false) return;
	_dma_stream_status_DMEIF_clear(dma->controller, dma->stream, dma->status);
}

void dma_stream_status_TEIF_clear(dma_t* dma) {
	if (dma->initialized == false) return;
	_dma_stream_status_TEIF_clear(dma->controller, dma->stream, dma->status);
}

void dma_stream_status_HTIF_clear(dma_t* dma) {
	if (dma->initialized == false) return;
	_dma_stream_status_HTIF_clear(dma->controller, dma->stream, dma->status);
}

void dma_stream_status_TCIF_clear(dma_t* dma) {
	if (dma->initialized == false) return;
	_dma_stream_status_TCIF_clear(dma->controller, dma->stream, dma->status);
}

void dma_stream_callback_set(dma_t* dma, void(*callback)) {
	if (dma->initialized == false) return;
	_dma_stream_callback_set(dma->controller, dma->stream, callback);
}

bool dma_stream_enable(dma_t* dma) {
	if (dma->initialized == false) return false;

	dma_scr_t scr;

	scr.all = _dma_stream_control_register_read(dma->controller, dma->stream);

	scr.bit.EN = DMA_SCR_EN_ENA;

	_dma_stream_control_register_write(dma->controller, dma->stream, scr.all);

	return true;
}

bool dma_stream_disable(dma_t* dma) {
	if (dma->initialized == false) return false;

	dma_scr_t scr;

	scr.all = _dma_stream_control_register_read(dma->controller, dma->stream);

	scr.bit.EN = DMA_SCR_EN_DIS;

	_dma_stream_control_register_write(dma->controller, dma->stream, scr.all);

	return true;
}

void dma_stream_init(
		dma_t* dma,
		dma_scr_dbm_t dbm,
		dma_scr_ct_t ct,

		dma_scr_chsel_t chsel,

		dma_fcr_dmdis_t dmdis,
		dma_fcr_fth_t fth,

		dma_scr_msize_t msize,
		dma_scr_mburst_t mburst,
		dma_scr_minc_t minc,

		dma_scr_psize_t psize,
		dma_scr_pburst_t pburst,
		dma_scr_pinc_t pinc,
		dma_scr_pincos_t pincos,

		dma_scr_dir_t dir,
		dma_scr_pfctrl_t pfctrl,
		dma_scr_circ_t circ,

		dma_scr_tcie_t tcie,
		dma_scr_htie_t htie,
		dma_scr_teie_t teie,
		dma_scr_dmeie_t dmeie,
		dma_fcr_feie_t feie,

		dma_scr_pl_t pl,
		dma_scr_en_t en) {
	if (dma->initialized == false) return;

	dma_scr_t scr;
	dma_sfcr_t fscr;

	scr.all = _dma_stream_control_register_read(dma->controller, dma->stream);

	if(scr.bit.EN == DMA_SCR_EN_ENA) return;

	fscr.all = _dma_stream_fifo_control_register_read(dma->controller, dma->stream);

	scr.bit.EN = en;
	scr.bit.DMEIE = dmeie;
	scr.bit.TEIE = teie;
	scr.bit.HTIE = htie;
	scr.bit.TCIE = tcie;
	scr.bit.PFCTRL = pfctrl;
	scr.bit.DIR = dir;
	scr.bit.CIRC = circ;
	scr.bit.PINC = pinc;
	scr.bit.MINC = minc;
	scr.bit.PSIZE = psize;
	scr.bit.MSIZE = msize;
	scr.bit.PINCOS = pincos;
	scr.bit.PL = pl;
	scr.bit.DBM = dbm;
	scr.bit.CT = ct;
	scr.bit.PBURST = pburst;
	scr.bit.MBURST = mburst;
	scr.bit.CHSEL = chsel;

	fscr.bit.FTH = fth;
	fscr.bit.DMDIS = dmdis;
	fscr.bit.FEIE = feie;

	_dma_stream_fifo_control_register_write(dma->controller, dma->stream, fscr.all);

	_dma_stream_control_register_write(dma->controller, dma->stream, scr.all);
}

uint32_t dma_stream_number_of_data_register_read(dma_t* dma) {
	if (dma->initialized == false) return 0;
	return _dma_stream_number_of_data_register_read(dma->controller, dma->stream);
}

void dma_stream_number_of_data_register_write(dma_t* dma, uint32_t NDTR) {
	if (dma->initialized == false) return;
	_dma_stream_number_of_data_register_write(dma->controller, dma->stream, NDTR);
}

uint32_t dma_stream_peripheral_address_register_read(dma_t* dma) {
	if (dma->initialized == false) return 0;
	return _dma_stream_peripheral_address_register_read(dma->controller, dma->stream);
}

void dma_stream_peripheral_address_register_write(dma_t* dma, uint32_t PAR) {
	if (dma->initialized == false) return;
	_dma_stream_peripheral_address_register_write(dma->controller, dma->stream, PAR);
}

uint32_t dma_stream_memory_0_address_register_read(dma_t* dma) {
	if (dma->initialized == false) return 0;
	return _dma_stream_memory_0_address_register_read(dma->controller, dma->stream);
}

void dma_stream_memory_0_address_register_write(dma_t* dma, uint32_t M0AR) {
	if (dma->initialized == false) return;
	_dma_stream_memory_0_address_register_write(dma->controller, dma->stream, M0AR);
}

uint32_t dma_stream_memory_1_address_register_read(dma_t* dma) {
	if (dma->initialized == false) return 0;
	return _dma_stream_memory_1_address_register_read(dma->controller, dma->stream);
}

void dma_stream_memory_1_address_register_write(dma_t* dma, uint32_t M1AR) {
	if (dma->initialized == false) return;
	_dma_stream_memory_1_address_register_write(dma->controller, dma->stream, M1AR);
}

void dma_stream_deinit(dma_t* dma) {
	if (dma->initialized == false) return;

	_dma_stream_control_register_write(dma->controller, dma->stream, 0);
	_dma_stream_fifo_control_register_write(dma->controller, dma->stream, 0);

	dma_stream_number_of_data_register_write(dma, 0);
	dma_stream_peripheral_address_register_write(dma, 0);
	dma_stream_memory_0_address_register_write(dma, 0);
	dma_stream_memory_1_address_register_write(dma, 0);

	dma_stream_callback_set(dma, NULL);
}

void dma_stream_close(dma_t* dma) {
	if (dma->initialized == false) return;

	dma_stream_deinit(dma);

	_dma_stream_busy_write(dma->controller, dma->stream, false);
}
