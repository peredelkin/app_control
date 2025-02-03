/*
 * spi_dma.c
 *
 *  Created on: 6 мая 2022 г.
 *      Author: Ruslan
 */

#include "spi.h"
#include "lib/utils/barrier.h"

void spi_bus_enable(SPI_BUS_TypeDef *bus) {
	bus->spi->CR1.bit.SPE = 1;
}

void spi_bus_disable(SPI_BUS_TypeDef *bus) {
	while (bus->spi->SR.bit.BSY);
	bus->spi->CR1.bit.SPE = 0;
}

//NSS off
void spi_bus_nss_off(SPI_BUS_TypeDef *bus) {
	//опустить NSS
	gpio_output_bit_setup(bus->nss.pin, GPIO_STATE_OFF);
	//подождать после опускания NSS
	sys_counter_delay(0, bus->nss.leading_delay_usec);
}

//NSS on
void spi_bus_nss_on(SPI_BUS_TypeDef *bus) {
	//подождать перед поднятием NSS
	sys_counter_delay(0, bus->nss.trailing_delay_usec);
	//поднять NSS
	gpio_output_bit_setup(bus->nss.pin, GPIO_STATE_ON);
	//подождать после поднятия NSS
	sys_counter_delay(0, bus->nss.next_frame_delay_usec);
}

//Ожидает освобождения шины
void spi_bus_wait(SPI_BUS_TypeDef *bus) {
	while (bus->done == false);
}

//Освобождение шины
void spi_bus_free(SPI_BUS_TypeDef *bus) {
	bus->done = true;
}

//включение прерываний
void spi_bus_interrupt_enable(SPI_BUS_TypeDef *bus) {
	//настройки SPI, содержащие биты разрешения прерываний
	SPI_CR2_REG CR2;
	//прочесть настройки
	CR2.all = bus->spi->CR2.all;
	//прерывание "буфер приема не пуст"
	CR2.bit.RXNEIE = 1;
	//прерывание "буфер передачи пуст"
	CR2.bit.TXEIE = 1;
	//записать настройки
	bus->spi->CR2.all = CR2.all;
}

void spi_bus_transfer_start(SPI_BUS_TypeDef *bus) {
	bus->data_tx_n = 0;
	bus->data_rx_n = 0;

	bus->tx_done = false;
	bus->rx_done = false;

	spi_bus_nss_off(bus);
	spi_bus_interrupt_enable(bus);
}

static inline void spi_bus_rx_done(SPI_BUS_TypeDef *bus) {
	//настройки SPI, содержащие биты разрешения прерываний
	SPI_CR2_REG CR2;
	//прочесть настройки
	CR2.all = bus->spi->CR2.all;
	//запретить прерывание "буфер передачи пуст"
	CR2.bit.RXNEIE = 0;
	//записать настройки
	bus->spi->CR2.all = CR2.all;
	//выставим флаг
	bus->rx_done = true;
}

static inline void spi_bus_tx_done(SPI_BUS_TypeDef *bus) {
	//настройки SPI, содержащие биты разрешения прерываний
	SPI_CR2_REG CR2;
	//прочесть настройки
	CR2.all = bus->spi->CR2.all;
	//запретить прерывание "буфер передачи пуст"
	CR2.bit.TXEIE = 0;
	//записать настройки
	bus->spi->CR2.all = CR2.all;
	//выставим флаг
	bus->tx_done = true;
}

//Инициализация структуры SPI
void spi_bus_struct_init(SPI_BUS_TypeDef *bus, SPI_TypeDef *spi) {
	bus->spi = (BITS_SPI_TypeDef*) spi;

	bus->nss.pin = NULL;
	bus->nss.leading_delay_usec = 0;
	bus->nss.trailing_delay_usec = 0;
	bus->nss.next_frame_delay_usec = 0;

	bus->frame = NULL;

	bus->frame_count = 0;
	bus->frame_n = 0;
	bus->data_tx_n = 0;
	bus->data_rx_n = 0;

	bus->callback = NULL;
	bus->callback_argument = NULL;

	bus->done = true;
}

void spi_bus_open(SPI_BUS_TypeDef *bus, const CFG_REG_SPI_TypeDef *cfg) {
	spi_bus_wait(bus);

	//для отладки, если SPI не был выключен
	while (bus->spi->CR1.bit.SPE == 1);

	//настройка SPI
	bus->spi->CR1.all = cfg->CR1.all;
	bus->spi->CR2.all = cfg->CR2.all;
	spi_bus_enable(bus);

	//настройка NSS и таймингов
	bus->nss.pin = cfg->NSS;
	bus->nss.leading_delay_usec = cfg->LD_USEC;
	bus->nss.trailing_delay_usec = cfg->TD_USEC;
	bus->nss.next_frame_delay_usec = cfg->NFD_USEC;
}

void spi_bus_close(SPI_BUS_TypeDef *bus) {
	spi_bus_wait(bus);

	//для отладки, если NSS не был поднят
	if (bus->nss.pin != NULL) {
		while (gpio_output_bit_read(bus->nss.pin) == GPIO_STATE_OFF);
	}

	spi_bus_disable(bus);
	bus->spi->CR1.all = 0;
	bus->spi->CR2.all = 0;

	//Сброс настроек NSS и таймингов
	bus->nss.pin = NULL;
	bus->nss.leading_delay_usec = 0;
	bus->nss.trailing_delay_usec = 0;
	bus->nss.next_frame_delay_usec = 0;

	//Сброс указателя на данные приема/передачи
	bus->frame = NULL;

	//Сброс служебных данных
	bus->frame_count = 0;
	bus->frame_n = 0;
	bus->data_tx_n = 0;
	bus->data_rx_n = 0;

	//Сброс колбека
	bus->callback = NULL;
	bus->callback_argument = NULL;
}

//Настройка и запуск приема/передачи
void spi_bus_transfer(SPI_BUS_TypeDef *bus, SPI_BUS_FRAME_TypeDef *frame_control_array_pointer, size_t frame_count,
		spi_byte_order_t frame_byte_order, spi_bus_callback callback, void *callback_argument) {

	spi_bus_wait(bus);
	bus->done = false;

	bus->byte_order = frame_byte_order;
	bus->frame = frame_control_array_pointer;
	bus->frame_count = frame_count;
	bus->frame_n = 0;


	bus->callback = callback;
	bus->callback_argument = callback_argument;

	spi_bus_transfer_start(bus);
}

//Настройка и запуск приема/передачи из колбека
void spi_bus_transfer_from_callback(SPI_BUS_TypeDef *bus, SPI_BUS_FRAME_TypeDef *frame_control_array_pointer,
		size_t frame_count, spi_byte_order_t frame_byte_order, spi_bus_callback callback, void *callback_argument) {

	bus->byte_order = frame_byte_order;
	bus->frame = frame_control_array_pointer;
	bus->frame_count = frame_count;
	bus->frame_n = 0;
	bus->data_tx_n = 0;
	bus->data_rx_n = 0;

	bus->callback = callback;
	bus->callback_argument = callback_argument;

	spi_bus_transfer_start(bus);
}

//чтение в заглушку
static inline void spi_bus_read_to_stub(SPI_BUS_TypeDef *bus) {
	__used static uint8_t data;
	data = (bus->spi->DR.all);
}

//чтение во фрейм
static inline void spi_bus_read_to_frame_data(SPI_BUS_TypeDef *bus) {
	if (bus->byte_order == SPI_BYTE_ORDER_REVERSE) {
		size_t _data_n = bus->frame[bus->frame_n].count - (bus->data_rx_n + 1);
		bus->frame[bus->frame_n].rx[_data_n] = bus->spi->DR.all;
	} else {
		bus->frame[bus->frame_n].rx[bus->data_rx_n] = bus->spi->DR.all;
	}
}

//запись из заглушки
static inline void spi_bus_write_from_stub(SPI_BUS_TypeDef *bus) {
	__used static uint8_t data = 0;
	(bus->spi->DR.all) = data;
}

//запись из фрейма
static inline void spi_bus_write_from_frame_data(SPI_BUS_TypeDef *bus) {
	if (bus->byte_order == SPI_BYTE_ORDER_REVERSE) {
		size_t _data_n = bus->frame[bus->frame_n].count - (bus->data_tx_n + 1);
		(bus->spi->DR.all) = bus->frame[bus->frame_n].tx[_data_n];
	} else {
		(bus->spi->DR.all) = bus->frame[bus->frame_n].tx[bus->data_tx_n];
	}
}

static inline void spi_bus_RXNE_handler(SPI_BUS_TypeDef *bus) {
	//буфер приема не пуст, прерывание включено
	if (bus->SR.bit.RXNE && bus->spi->CR2.bit.RXNEIE) {
		//указатель NULL, куда будут прочитаны данные
		if (bus->frame[bus->frame_n].rx == NULL) {
			//прочитаем в заглушку
			spi_bus_read_to_stub(bus);
		} else {
			//прочитаем по указателю
			spi_bus_read_to_frame_data(bus);
		}
		//следующий байт
		bus->data_rx_n++;
		//все байты получены
		if (bus->data_rx_n >= bus->frame[bus->frame_n].count) {
			spi_bus_rx_done(bus);
		}
	}
}

static inline void spi_bus_TXE_handler(SPI_BUS_TypeDef *bus) {
	//буфер пуст, прерывание включено
	if (bus->SR.bit.TXE && bus->spi->CR2.bit.TXEIE) {
		//указатель NULL, откуда будут записаны данные
		if (bus->frame[bus->frame_n].tx == NULL) {
			//запишем из заглушки
			spi_bus_write_from_stub(bus);
		} else {
			//запишем по указателю
			spi_bus_write_from_frame_data(bus);
		}
		//следующий байт
		bus->data_tx_n++;
		//все байты переданы
		if (bus->data_tx_n >= bus->frame[bus->frame_n].count) {
			spi_bus_tx_done(bus);
		}
	}
}

static inline void spi_bus_frame_done_handler(SPI_BUS_TypeDef *bus) {
	if (bus->tx_done && bus->rx_done) {
		//поднимем NSS
		spi_bus_nss_on(bus);
		//следующий фрейм
		bus->frame_n++;
		//если все фреймы переданы
		if (bus->frame_n >= bus->frame_count) {
			//если колбек не задан
			if (bus->callback == NULL) {
				//освободим шину
				spi_bus_free(bus);
			} else {
				//вызовем функцию колбека
				bus->callback(bus->callback_argument);
			}
		} else {
			spi_bus_transfer_start(bus);
		}
	}
}

//Обработчик прерывания SPI
void SPI_BUS_IRQHandler(SPI_BUS_TypeDef *bus) {
	bus->SR.all = bus->spi->SR.all;
	spi_bus_RXNE_handler(bus);
	spi_bus_TXE_handler(bus);
	spi_bus_frame_done_handler(bus);
}

