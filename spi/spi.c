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
	if(bus->nss.pin != NULL) {
		//опустить NSS
		gpio_output_bit_setup(bus->nss.pin, GPIO_STATE_OFF);
		//подождать после опускания NSS
		sys_counter_delay(0, bus->nss.leading_delay_usec);
	}
}

//NSS on
void spi_bus_nss_on(SPI_BUS_TypeDef *bus) {
	if(bus->nss.pin != NULL) {
		//подождать перед поднятием NSS
		sys_counter_delay(0, bus->nss.trailing_delay_usec);
		//поднять NSS
		gpio_output_bit_setup(bus->nss.pin, GPIO_STATE_ON);
	}
}

gpio_pin_state_t spi_bus_nss_read(SPI_BUS_TypeDef *bus) {
	if (bus->nss.pin != NULL) {
		return gpio_output_bit_read(bus->nss.pin);
	}

	return GPIO_STATE_ON;
}

//Ожидает освобождения шины
void spi_bus_wait(SPI_BUS_TypeDef *bus) {
	while (bus->done == false);
}

//Освобождение шины
void spi_bus_free(SPI_BUS_TypeDef *bus) {
	bus->done = true;
}

//Занимает шину
void spi_bus_busy(SPI_BUS_TypeDef *bus) {
	bus->done = false;
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
	//CR2.bit.TXEIE = 1;
	//записать настройки
	bus->spi->CR2.all = CR2.all;
}

void spi_bus_interrupt_distable(SPI_BUS_TypeDef *bus) {
	//настройки SPI, содержащие биты разрешения прерываний
	SPI_CR2_REG CR2;
	//прочесть настройки
	CR2.all = bus->spi->CR2.all;
	//запретить прерывание "буфер передачи пуст"
	CR2.bit.RXNEIE = 0;
	//записать настройки
	bus->spi->CR2.all = CR2.all;
}

//чтение в заглушку
void spi_bus_read_to_stub(SPI_BUS_TypeDef *bus) {
	__used static uint8_t data;
	data = (bus->spi->DR.all);
}

//чтение во фрейм
void spi_bus_read_to_frame_data(SPI_BUS_TypeDef *bus, size_t data_n) {
	if (bus->frame[bus->frame_counter].byte_order == SPI_BYTE_ORDER_REVERSE) {
		size_t _data_n = bus->frame[bus->frame_counter].count - (data_n + 1);
		bus->frame[bus->frame_counter].rx[_data_n] = bus->spi->DR.all;
	} else {
		bus->frame[bus->frame_counter].rx[data_n] = bus->spi->DR.all;
	}
}

//чтение
void spi_bus_read(SPI_BUS_TypeDef *bus) {
	if (bus->frame[bus->frame_counter].rx == NULL) {
		//прочитаем в заглушку
		spi_bus_read_to_stub(bus);
	} else {
		//прочитаем по указателю
		spi_bus_read_to_frame_data(bus, bus->byte_counter);
	}
}

//запись из заглушки
void spi_bus_write_from_stub(SPI_BUS_TypeDef *bus) {
	__used static uint8_t data = 0;
	(bus->spi->DR.all) = data;
}

//запись из фрейма
void spi_bus_write_from_frame_data(SPI_BUS_TypeDef *bus, size_t data_n) {
	if (bus->frame[bus->frame_counter].byte_order == SPI_BYTE_ORDER_REVERSE) {
		size_t _data_n = bus->frame[bus->frame_counter].count - (data_n + 1);
		(bus->spi->DR.all) = bus->frame[bus->frame_counter].tx[_data_n];
	} else {
		(bus->spi->DR.all) = bus->frame[bus->frame_counter].tx[data_n];
	}
}

//запись
void spi_bus_write(SPI_BUS_TypeDef *bus) {
	if (bus->frame[bus->frame_counter].tx == NULL) {
		//запишем из заглушки
		spi_bus_write_from_stub(bus);
	} else {
		//запишем по указателю
		spi_bus_write_from_frame_data(bus, bus->byte_counter);
	}
}

//Инициализация структуры SPI
void spi_bus_struct_init(SPI_BUS_TypeDef *bus, SPI_TypeDef *spi) {
	bus->spi = (BITS_SPI_TypeDef*) spi;

	bus->nss.pin = NULL;
	bus->nss.leading_delay_usec = 0;
	bus->nss.trailing_delay_usec = 0;

	bus->frame = NULL;

	bus->frame_count = 0;
	bus->frame_counter = 0;

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
}

void spi_bus_close(SPI_BUS_TypeDef *bus) {
	spi_bus_wait(bus);

	//для отладки, если NSS не был поднят
	while (spi_bus_nss_read(bus) == GPIO_STATE_OFF);

	spi_bus_disable(bus);
	bus->spi->CR1.all = 0;
	bus->spi->CR2.all = 0;

	//Сброс настроек NSS и таймингов
	bus->nss.pin = NULL;
	bus->nss.leading_delay_usec = 0;
	bus->nss.trailing_delay_usec = 0;

	//Сброс указателя на данные приема/передачи
	bus->frame = NULL;

	//Сброс служебных данных
	bus->frame_count = 0;
	bus->frame_counter = 0;
	bus->byte_counter = 0;
}

void spi_bus_transfer_start(SPI_BUS_TypeDef *bus) {

}

//Настройка и запуск приема/передачи
void spi_bus_transfer(SPI_BUS_TypeDef *bus, SPI_BUS_FRAME_TypeDef *frame_control_array_pointer, size_t frame_count) {

	bus->frame = frame_control_array_pointer;
	bus->frame_count = frame_count;
	bus->frame_counter = 0;
	bus->byte_counter = 0;

	spi_bus_nss_off(bus);
	spi_bus_write(bus);
	spi_bus_interrupt_enable(bus);
}

void spi_bus_frame_done_handler(SPI_BUS_TypeDef *bus) {
	//если колбек задан
	if (bus->frame[bus->frame_counter].callback != NULL) {
		//вызовем функцию колбека
		bus->frame[bus->frame_counter].callback(bus->frame[bus->frame_counter].callback_argument);
	}
	//сброс счетчика байт
	bus->byte_counter = 0;
	//следующий фрейм
	bus->frame_counter++;
	//если все фреймы переданы
	if (bus->frame_counter >= bus->frame_count) {
		//выключим прерывание
		spi_bus_interrupt_distable(bus);
		//поднимем NSS
		spi_bus_nss_on(bus);
		//освободим шину
		spi_bus_free(bus);
	} else {
		//запись
		spi_bus_write(bus);
	}
}

void spi_bus_RXNE_handler(SPI_BUS_TypeDef *bus) {
	if (bus->status.bit.RXNE /*&& bus->SR.bit.TXE*/ && bus->spi->CR2.bit.RXNEIE) {
		//чтение
		spi_bus_read(bus);
		//следующий байт
		bus->byte_counter++;
		//все байты получены
		if (bus->byte_counter >= bus->frame[bus->frame_counter].count) {
			spi_bus_frame_done_handler(bus);
		} else {
			//запись
			spi_bus_write(bus);
		}
	}
}

//Обработчик прерывания SPI
void SPI_BUS_IRQHandler(SPI_BUS_TypeDef *bus) {
	bus->status.all = bus->spi->SR.all;
	spi_bus_RXNE_handler(bus);
}

