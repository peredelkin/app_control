/*
 * spi1_init.c
 *
 *  Created on: 30 июн. 2026 г.
 *      Author: ruslan
 */

#include "spi_init.h"
#include "spi/settings/spi_settings.h"
#include "lib/stm32f4xx/stm32f4xx.h"
#include "gpio/init/gpio_init.h"

#define SPI1_DATA_SECTION &(SPI1->DR)

#include "resampler/resampler.h"
resampler_t spi1_resampler;

//void resampler_init(resampler_t* resampler) {
//	resampler->sample_in = 0;
//	resampler->sample_out = 0;
//
//	for(int i = 0; i < RESAMPLER_COUNT; i++) {
//		resampler->in[i] = 0; //сброс входных данных
//		resampler->accum[i] = 0; //сброс аккумулятора
//		resampler->out[i] = 0; //сброс выходных данных
//	}
//
//	for(int d = 0; d < RESAMPLER_DEPTH; d++) {
//		for(int i = 0; i < RESAMPLER_COUNT; i++) {
//			resampler->buf[d][i] = 0; //сброс буфера
//		}
//	}
//
//	resampler->callback = NULL;
//}

#include "dma/dma.h"
dma_t spi1_rx_dma;

static dma_stream_settings_t dma_rx_settings = DMA_STREAM_MAKE_CFG(
		DMA_SCR_DBM_DIS,			/*Double buffer mode*/
		DMA_SCR_CT_MEM0,			/*Current target*/
		DMA_SCR_CHSEL_3,			/*Channel selection*/
		DMA_FCR_DMDIS_ENA,			/*Direct mode disable*/
		DMA_FCR_FTH_ONE_FOURTH,		/*FIFO threshold selection*/
		DMA_SCR_MSIZE_32,			/*Memory data size*/
		DMA_SCR_MBURST_DIS,			/*Memory burst transfer configuration*/
		DMA_SCR_MINC_ENA,			/*Memory increment mode*/
		DMA_SCR_PSIZE_8,			/*Peripheral data size*/
		DMA_SCR_PBURST_DIS,			/*Peripheral burst transfer configuration*/
		DMA_SCR_PINC_DIS,			/*Peripheral increment mode*/
		DMA_SCR_PINCOS_PSIZE,		/*Peripheral increment offset size*/
		DMA_SCR_DIR_PERI_TO_MEM,	/*Data transfer direction*/
		DMA_SCR_PFCTRL_DIS,			/*Peripheral flow controller*/
		DMA_SCR_CIRC_DIS,			/*Circular mode*/
		DMA_SCR_TCIE_DIS,			/*Transfer complete interrupt enable*/
		DMA_SCR_HTIE_DIS,			/*Half transfer interrupt enable*/
		DMA_SCR_TEIE_DIS,			/*Transfer error interrupt enable*/
		DMA_SCR_DMEIE_DIS,			/*Direct mode error interrupt enable*/
		DMA_FCR_FEIE_DIS,			/*FIFO error interrupt enable*/
		DMA_SCR_PL_MEDIUM,			/*Priority level: Medium*/
		DMA_SCR_EN_DIS);

err_t spi1_rx_dma_init(dma_t* dma) {
	return dma_struct_init(dma, DMA2_Stream_0);
}

err_t spi1_rx_dma_start(dma_t* dma) {
	//stream open
	err_t err = dma_stream_open(dma);
	if(err != E_NO_ERROR) return err;
	//stream deinit
	dma_stream_deinit(dma);
	//stream conf
	dma_stream_number_of_data_register_write(dma, RESAMPLER_COUNT*4);								//8->32
	dma_stream_peripheral_address_register_write(dma, (uint32_t) SPI1_DATA_SECTION);				//DR
	dma_stream_memory_0_address_register_write(dma, (uint32_t) spi1_resampler.in);					//array

	dma_stream_setup(dma, &dma_rx_settings);

	dma_stream_enable(dma);

	return E_NO_ERROR;
}

err_t spi1_rx_dma_stop(dma_t* dma) {
	err_t err = E_NOT_IMPLEMENTED;

	if(dma_stream_is_busy(dma) == E_NO_ERROR) {
		return err;
	}

	uint32_t ISR = dma_stream_status_register_read(dma);

	dma_stream_disable(dma); //disable Stream

	if (dma_stream_status_TCIF_read(ISR, dma)) {
		dma_stream_status_TCIF_clear(dma); //clear TC
		err =  E_NO_ERROR;
	}

	if (dma_stream_status_TEIF_read(ISR, dma)) { //if TE
		dma_stream_status_TEIF_clear(dma); //clear TE
		err = E_STATE;
	}

	dma_stream_close(dma); //close Stream
	return err;
}

/* SPI1 BEGIN */
//90 MHz
//APP <-> MC
void spi1_resampler_callback(void* ptr);

void spi1_rcc_init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}

void spi1_init() {
	SPI1->CR1 &= ~SPI_CR1_SPE; //Disable SPI

	SPI1->CR1 = spi_app_to_mc_cfg.CR1.all;
	SPI1->CR2 = spi_app_to_mc_cfg.CR2.all;
}

void spi1_bus_init(void) {
	gpio_spi1_cfg_setup();
	spi1_rcc_init();
	spi1_init();
	spi1_resampler.callback = spi1_resampler_callback;
}

void spi1_nss_handler() {
	//RISE
	if(gpio_input_bit_read(spi_app_to_mc_cfg.NSS)) {
		SPI1->CR1 &= ~SPI_CR1_SPE; //Disable SPI

		if(spi1_rx_dma_stop(&spi1_rx_dma) == E_NO_ERROR) {
			resampler_calc(&spi1_resampler);
		}

		if(spi1_rx_dma_start(&spi1_rx_dma) == E_NO_ERROR) {
			SPI1->CR1 |= SPI_CR1_SPE; //Enable SPI
		}
	}
}

void spi1_resampler_callback(void* ptr) {
	resampler_t* res = ptr;
}
/* SPI1 END */
