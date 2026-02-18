/*
 * tic12400.c
 *
 *  Created on: 5 апр. 2022 г.
 *      Author: Ruslan
 */

#include "tic12400.h"

/*
 * Функция для проверки бита четности в фрейме приема. Она извлекает бит четности из фрейма,
 * устанавливает его в 0, а затем рассчитывает четность для всего фрейма. Если рассчитанная
 * четность совпадает с извлеченной, функция возвращает true, иначе - false.
 * @param tic - указатель на структуру tic12400_t для проверки бита четности
 * @return true при совпадении битов четности, false при несовпадении
 */
bool tic12400_rx_frame_parity_check(tic12400_t *tic) {
	int par = tic->frame_rx.bit.par;
	tic->frame_rx.bit.par = 0;
	tic->frame_rx.bit.par = calc_parity(tic->frame_rx.all, 32, PARITY_ODD);
	return ((par) == (tic->frame_rx.bit.par));
}

/*
 * Функция для заполнения статуса устройства на основе битов из фрейма приема. Она обновляет
 * поля статуса, такие как overcurrent, temperature, undervoltage, и другие, в соответствии с
 * соответствующими битами из фрейма приема.
 * @param tic - указатель на структуру tic12400_t для обновления статуса
 */
void tic12400_status_fill(tic12400_t *tic) {
	tic->status.bit.oi |= tic->frame_rx.bit.oi;
	tic->status.bit.temp |= tic->frame_rx.bit.temp;
	tic->status.bit.vs_th |= tic->frame_rx.bit.vs_th;
	tic->status.bit.ssc |= tic->frame_rx.bit.ssc;
	tic->status.bit.par_fail |= tic->frame_rx.bit.par_fail;
	tic->status.bit.spi_fail |= tic->frame_rx.bit.spi_fail;
	tic->status.bit.por |= tic->frame_rx.bit.por;
}

/*
 * Функция обработки фрейма приема, которая проверяет бит четности и обновляет статус устройства.
 * Если бит четности не совпадает, устанавливается флаг ошибки четности.
 * @param tic - указатель на структуру tic12400_t для обработки фрейма приема
 */
void tic12400_rx_frame_parity_handler(tic12400_t *tic) {
	if (tic12400_rx_frame_parity_check(tic)) {
		tic->status.bit.rx_par_fail = 0;
		tic12400_status_fill(tic);
	} else {
		tic->status.bit.rx_par_fail = 1;
		tic->par_fail = true;
	}
}

void tic12400_handler(void *tic) {
	tic12400_rx_frame_parity_handler(tic);
}

/*
 * Функция инициализации структуры tic12400_t,
 * которая настраивает SPI-контроллер для обмена данными с TIC12400,
 * а также инициализирует все поля структуры нулями и устанавливает флаг done в true.
 * @param tic - указатель на структуру tic12400_t для инициализации
 * @param spi_bus - указатель на SPI-шину для обмена данными
 * @param spi_cfg - указатель на конфигурацию SPI для настройки шины
 */
void tic12400_init(tic12400_t *tic, SPI_BUS_TypeDef *spi_bus, const CFG_REG_SPI_TypeDef *spi_cfg) {
	tic->spi_bus = spi_bus;
	tic->spi_cfg = spi_cfg;

	tic->spi_control.tx = (uint8_t*) &tic->frame_tx.all;
	tic->spi_control.rx = (uint8_t*) &tic->frame_rx.all;
	tic->spi_control.count = TIC12400_FRAME_SIZE;
	tic->spi_control.byte_order = SPI_BYTE_ORDER_REVERSE;
	tic->spi_control.callback = &tic12400_handler;
	tic->spi_control.callback_argument= tic;

	tic->frame_tx.all = 0;
	tic->frame_rx.all = 0;
	tic->status.all = 0;

	tic->sequential.end = 0;
	tic->sequential.index = 0;
	tic->sequential.addr = NULL;
	tic->sequential.data = NULL;

	tic->par_fail = false;

	tic->done = true;
}

/*
 * Функция для заполнения фрейма обмена данными для чтения или записи. Она устанавливает
 * биты rw, addr и data в соответствии с переданными параметрами, а также рассчитывает бит
 * четности для всего фрейма и сохраняет его в поле par.
 * @param tic - указатель на структуру tic12400_t
 * @param rw - флаг операции (1 для записи, 0 для чтения)
 * @param addr - адрес регистра для доступа
 * @param data - данные для записи (игнорируются при чтении)
 */
void tic124_tx_frame_fill(tic12400_t *tic, uint32_t rw, uint32_t addr, uint32_t data) {
	tic->frame_tx.all = 0;
	tic->frame_tx.bit.rw = rw;
	tic->frame_tx.bit.addr = addr;
	tic->frame_tx.bit.data = data;
	tic->frame_tx.bit.par = calc_parity(tic->frame_tx.all, 32, PARITY_ODD);
}

/*
 * Функция передачи данных для записи, которая заполняет фрейм данными и
 * инициирует SPI-транзакцию. После завершения транзакции, данные из фрейма
 * приема сохраняются в массив данных для проверки четности.
 * @param tic - указатель на структуру tic12400_t
 * @param callback - функция обратного вызова для обработки завершения SPI-транзакции
 * @param callback_argument - аргумент для функции обратного вызова
 */
void tic12400_write(tic12400_t *tic, spi_bus_callback_t callback, void *callback_argument) {
	tic124_tx_frame_fill(tic, 1, tic->sequential.addr[tic->sequential.index],
			tic->sequential.data[tic->sequential.index]);
	spi_bus_transfer(tic->spi_bus, &tic->spi_control, 1, callback, callback_argument);
}

/*
 * Функция для чтения данных из регистра TIC12400. Она заполняет фрейм для чтения,
 * инициирует SPI-транзакцию и сохраняет полученные данные в массиве sequential.data.
 * @param tic - указатель на структуру tic12400_t
 * @param callback - функция обратного вызова для обработки завершения SPI-транзакции
 * @param callback_argument - аргумент для функции обратного вызова
 */
void tic12400_read(tic12400_t *tic, spi_bus_callback_t callback, void *callback_argument) {
	tic124_tx_frame_fill(tic, 0, tic->sequential.addr[tic->sequential.index], 0);
	spi_bus_transfer(tic->spi_bus, &tic->spi_control, 1, callback, callback_argument);
	tic->sequential.data[tic->sequential.index] = tic->frame_rx.bit.data;
}

/*
 * Общая функция передачи данных для чтения и записи, которая последовательно
 * передает/принимает данные,
 * проверяя четность в ответе и устанавливая флаг ошибки четности при необходимости.
 * Функция возвращает статус четности после завершения обмена.
 * @param tic - указатель на структуру tic12400_t
 * @param tx - флаг, указывающий на операцию (true для записи, false для чтения)
 * @param data - указатель на массив данных для передачи или приема
 * @param addr - указатель на массив адресов регистров
 * @param start - начальный индекс в массивах данных и адресов
 * @param count - количество регистров для передачи/приема
 * @param callback - функция обратного вызова для обработки завершения SPI-транзакции
 * @param callback_argument - аргумент для функции обратного вызова
 * @return статус четности (true при ошибке четности, false при успешной передаче/приеме)
 */
bool tic12400_transfer(tic12400_t *tic, bool tx, uint32_t *data, const uint8_t *addr, uint8_t start, uint8_t count,
		spi_bus_callback_t callback, void *callback_argument) {
	//установка флагов
	tic->done = false;
	tic->par_fail = false;
	//настройка индексов приема/передачи
	tic->sequential.end = start + count;
	tic->sequential.index = start;
	tic->sequential.addr = addr;
	tic->sequential.data = data;
	//ожидание окончания обмена или ошибки четности
	while (tic->done == false) {
		//есть откуда/куда передавать
		if (tic->sequential.data != NULL) {
			//сравнение индекса и ошибки четности в ответе
			if ((tic->sequential.index < tic->sequential.end) && (tic->par_fail == false)) {
				//передача или прием
				if(tx == true) {
					tic12400_write(tic, callback, callback_argument);
				} else {
					tic12400_read(tic, callback, callback_argument);
				}
				//следующий фрейм
				tic->sequential.index++;
			} else {
				//все данные переданы/приняты или ошибка четности в ответе
				tic->done = true;
			}
		} else {
			//указатель на данные == NULL
			tic->done = true;
		}
	}
	//статус четности в конце обмена
	return tic->par_fail;
}

/*
 * Функция для записи данных в регистры TIC12400, которая вызывает общую функцию передачи
 * с флагом tx, установленным в true. Она возвращает статус четности после завершения обмена.
 * @param tic - указатель на структуру tic12400_t
 * @param data - указатель на массив данных для записи
 * @param addr - указатель на массив адресов регистров для записи
 * @param start - начальный индекс в массивах данных и адресов
 * @param count - количество регистров для записи
 * @param callback - функция обратного вызова для обработки завершения SPI-транзакции
 * @param callback_argument - аргумент для функции обратного вызова
 * @return статус четности (true при ошибке четности, false при успешной передаче)
 */
bool tic12400_reg_write(tic12400_t *tic, uint32_t *data, const uint8_t *addr, uint8_t start, uint8_t count,
		spi_bus_callback_t callback, void *callback_argument) {
	return tic12400_transfer(tic, true, data, addr, start, count, callback, callback_argument);
}

/*
 * Функция для чтения данных из регистров TIC12400, которая вызывает общую функцию передачи
 * с флагом tx, установленным в false. Она возвращает статус четности после завершения обмена.
 * @param tic - указатель на структуру tic12400_t
 * @param data - указатель на массив для сохранения прочитанных данных
 * @param addr - указатель на массив адресов регистров для чтения
 * @param start - начальный индекс в массивах данных и адресов
 * @param count - количество регистров для чтения
 * @param callback - функция обратного вызова для обработки завершения SPI-транзакции
 * @param callback_argument - аргумент для функции обратного вызова
 * @return статус четности (true при ошибке четности, false при успешной передаче)
 */
bool tic12400_reg_read(tic12400_t *tic, uint32_t *data, const uint8_t *addr, uint8_t start, uint8_t count,
		spi_bus_callback_t callback, void *callback_argument) {
	return tic12400_transfer(tic, false, data, addr, start, count, callback, callback_argument);
}

