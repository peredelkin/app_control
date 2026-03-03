/*
 * can_bus.c
 *
 *  Created on: 29 окт. 2024 г.
 *      Author: Ruslan
 */

#include <string.h>
#include <stdio.h>

#include "can_bus.h"
#include "can_master_filter.h"

//Bitrate
err_t can_bus_bitrate_set(can_bus_t* can_bus, uint16_t bitrate) {
	//проверка указателя CANptr
	if (can_bus == NULL) return E_NULL_POINTER;
	//получение указателя CAN
	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];
	//проверка указателя CAN
	if (can == NULL) return E_NULL_POINTER;
	//ошибка настройки битрейта
	err_t error = E_NO_ERROR;
	//CAN bit timing
	uint32_t btr = 0xFFFFFFFF;
	//for 168MHz
	switch (bitrate) {
	case 10:
		btr = 0x001b0117;
		break;
	case 20:
		btr = 0x001b008b;
		break;
	case 50:
		btr = 0x001b0037;
		break;
	case 100:
		btr = 0x001b001b;
		break;
	default:
		error = E_INVALID_VALUE;
		//no break
	case 125:
		btr = 0x001c0014;
		break;
	case 250:
		btr = 0x001a000b;
		break;
	case 500:
		btr = 0x001a0005;
		break;
	case 1000:
		btr = 0x001a0002;
		break;
	}

	can_BTR_set(can, btr);

	return error;
}

//Очередь RX и TX

//сброс сброс данных в очереди по указателю
void can_bus_rx_queue_reset(can_rx_frame_queue_t* ptr) {
	ptr->id = 0;
	ptr->dlc = 0;
	ptr->index = 0;
	memset(ptr->data, 0, sizeof(ptr->data));
}
void can_bus_tx_queue_reset(can_tx_frame_queue_t* ptr) {
	ptr->id = 0;
	ptr->dlc = 0;
	memset(ptr->data, 0, sizeof(ptr->data));
}

//очередь пуста, если голова и хвост совпадают
bool can_bus_rx_queue_empty(can_bus_t *bus) {
	return (bus->queue_rx.head == bus->queue_rx.tail);
}
bool can_bus_tx_queue_empty(can_bus_t *bus) {
	return (bus->queue_tx.head == bus->queue_tx.tail);
}

//очередь не пуста, если голова и хвост не совпадают
bool can_bus_rx_queue_not_empty(can_bus_t *bus) {
	return (bus->queue_rx.head != bus->queue_rx.tail);
}
bool can_bus_tx_queue_not_empty(can_bus_t *bus) {
	return (bus->queue_tx.head != bus->queue_tx.tail);
}

//получить указатель на элемент в голове очереди
can_rx_frame_queue_t* can_bus_rx_queue_head(can_bus_t *bus) {
	return &(bus->queue_rx.queue[bus->queue_rx.head]);
}
can_tx_frame_queue_t* can_bus_tx_queue_head(can_bus_t *bus) {
	return &(bus->queue_tx.queue[bus->queue_tx.head]);
}

//удалить элемент в голове очереди, если очередь не пуста
bool can_bus_rx_queue_dequeue(can_bus_t *bus) {
	if (can_bus_rx_queue_empty(bus)) return false;
	size_t new_head = bus->queue_rx.head + 1;
	if (new_head >= bus->queue_rx.size) new_head = 0;
	bus->queue_rx.head = new_head;
	return true;
}
bool can_bus_tx_queue_dequeue(can_bus_t *bus) {
	if (can_bus_tx_queue_empty(bus)) return false;
	size_t new_head = bus->queue_tx.head + 1;
	if (new_head >= bus->queue_tx.size) new_head = 0;
	bus->queue_tx.head = new_head;
	return true;
}

//удалить элемент в голове очереди и очистить его, если очередь не пуста
bool can_bus_rx_queue_reset_dequeue(can_rx_frame_queue_t* head, can_bus_t* bus) {
	can_bus_rx_queue_reset(head);
	return can_bus_rx_queue_dequeue(bus);
}
bool can_bus_tx_queue_reset_dequeue(can_tx_frame_queue_t* head, can_bus_t *bus) {
	can_bus_tx_queue_reset(head);
	return can_bus_tx_queue_dequeue(bus);
}

//получить указатель на элемент в хвосте очереди
can_rx_frame_queue_t* can_bus_rx_queue_tail(can_bus_t *bus) {
	return &(bus->queue_rx.queue[bus->queue_rx.tail]);
}
can_tx_frame_queue_t* can_bus_tx_queue_tail(can_bus_t *bus) {
	return &(bus->queue_tx.queue[bus->queue_tx.tail]);
}

//получить указатель на элемент в хвосте очереди и очистить его
can_rx_frame_queue_t* can_bus_rx_queue_tail_reset(can_bus_t *bus) {
	can_rx_frame_queue_t* tail = can_bus_rx_queue_tail(bus);
	can_bus_rx_queue_reset(tail);
	return tail;
}
can_tx_frame_queue_t* can_bus_tx_queue_tail_reset(can_bus_t *bus) {
	can_tx_frame_queue_t* tail = can_bus_tx_queue_tail(bus);
	can_bus_tx_queue_reset(tail);
	return tail;
}

//можно ли добавить элемент в очередь, если новый хвост не совпадает с головой
bool can_bus_rx_queue_can_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_rx.tail + 1;
	if (new_tail >= bus->queue_rx.size) new_tail = 0;
	if (new_tail == bus->queue_rx.head) return false; //ovf
	return true;
}
bool can_bus_tx_queue_can_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_tx.tail + 1;
	if (new_tail >= bus->queue_tx.size) new_tail = 0;
	if (new_tail == bus->queue_tx.head) return false; //ovf
	return true;
}

//добавить элемент в очередь, если новый хвост не совпадает с головой
bool can_bus_rx_queue_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_rx.tail + 1;
	if (new_tail >= bus->queue_rx.size) new_tail = 0;
	if (new_tail == bus->queue_rx.head) return false; //ovf
	bus->queue_rx.tail = new_tail;
	return true;
}
bool can_bus_tx_queue_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_tx.tail + 1;
	if (new_tail >= bus->queue_tx.size) new_tail = 0;
	if (new_tail == bus->queue_tx.head) return false; //ovf
	bus->queue_tx.tail = new_tail;
	return true;
}

//инициализирует очередь
void can_bus_rx_queue_init(can_bus_t *bus, can_rx_frame_queue_t* queue, size_t queue_size) {
	bus->queue_rx.queue = queue;
	bus->queue_rx.size = queue_size;
	bus->queue_rx.head = 0;
	bus->queue_rx.tail = 0;
}
void can_bus_tx_queue_init(can_bus_t *bus, can_tx_frame_queue_t* queue, size_t queue_size) {
	bus->queue_tx.queue = queue;
	bus->queue_tx.size = queue_size;
	bus->queue_tx.head = 0;
	bus->queue_tx.tail = 0;
}

/*
 * can_bus_write - добавить фрейм в очередь на передачу
 * @bus: указатель на структуру шины
 * @id: идентификатор фрейма
 * @dlc: длина данных (0-8)
 * @data: указатель на данные
 *
 * Возвращает true, если фрейм успешно добавлен в очередь, иначе false (очередь полна).
 */
bool can_bus_write(can_bus_t* bus, uint32_t id, uint8_t dlc, uint8_t* data) {
	//можно ли добавить в очередь
	if (can_bus_tx_queue_can_enqueue(bus) == false) {
		bus->error |= CAN_ERROR_TX_QUEUE_FULL;
		return false;
	} else {
		bus->error &= ~CAN_ERROR_TX_QUEUE_FULL;
	}
	//получим указатель на элемент в хвосте очереди и очистим его
	can_tx_frame_queue_t* tail = can_bus_tx_queue_tail_reset(bus);
	//заполним данные
	tail->id = id;
	tail->dlc = dlc;
	memcpy(tail->data, data, dlc); //copy DATA
	//добавим в очередь
	can_bus_tx_queue_enqueue(bus);

	return true;
}

//вызывается в цикле или по таймеру для получения сообщений из очереди
err_t can_bus_rx_process(can_bus_t* bus) {

	err_t rx_err = E_NO_ERROR;

	//если нечего принимать
	if (can_bus_rx_queue_empty(bus)) return rx_err;

	can_rx_frame_queue_t* head = NULL;

	do {
		//получим указатель на голову очереди
		head = can_bus_rx_queue_head(bus);
		//если колбек задан
		if(bus->rx_callback != NULL) {
			rx_err = bus->rx_callback(bus, head);
		} else {
			return E_NULL_POINTER;
		}

		if(rx_err == E_NO_ERROR) {
			//очистим данные в очереди и удалим элемент
			can_bus_rx_queue_reset_dequeue(head, bus);
		}
	} while ((rx_err == E_NO_ERROR) && can_bus_rx_queue_not_empty(bus));

	return rx_err;
}

//вызывается в цикле или по таймеру для передачи сообщений из очереди
err_t can_bus_tx_process(can_bus_t* bus) {

	err_t tx_err = E_NO_ERROR;

	//если нечего передавать
	if (can_bus_tx_queue_empty(bus)) return tx_err;

	//получим указатель на устройство
	CAN_TypeDef *can = bus->can_ptr[bus->can_n];

	can_tx_frame_queue_t* head = NULL;

	//запрертим прерывание
	CAN_BUS_LOCK_TX(bus);
	do {
		//Получим указатель на голову очереди
		head = can_bus_tx_queue_head(bus);
		//Запросим передачу
		tx_err = can_tx_mailbox_write_and_request(can, head->id, head->dlc, head->data);

		if(tx_err == E_NO_ERROR) {
			//очистим данные в очереди и удалим элемент
			can_bus_tx_queue_reset_dequeue(head, bus);
		}
	} while ((tx_err == E_NO_ERROR) && can_bus_tx_queue_not_empty(bus));
	//разрешим прерывание
	CAN_BUS_UNLOCK_TX(bus);

	return tx_err;
}

//IRQ Handlers
void CAN_TSR_RQCP_Handler(can_bus_t *bus, uint32_t TSR) {

	CAN_TypeDef *can = bus->can_ptr[bus->can_n];

	err_t tx_err = E_NO_ERROR;

	//проверим все 3 мейлбокса
	for (int mailbox = 0; mailbox < 3; mailbox++) {
		//запрос выполнен?
		if (can_TSR_RQCP_get(TSR, mailbox)) {
			//очистим запрос
			can_TSR_RQCP_clear(can, mailbox);
			//запретим прерывание
			CAN_BUS_LOCK_TX(bus);
			//если есть что передать
			if (can_bus_tx_queue_not_empty(bus)) {
				//Получим указатель
				can_tx_frame_queue_t *head = can_bus_tx_queue_head(bus);
				//заполним регистры
				tx_err = can_tx_mailbox_write_and_request(can, head->id, head->dlc, head->data);
				if (tx_err == E_NO_ERROR) {
					//очистим данные в очереди и удалим элемент
					can_bus_tx_queue_reset_dequeue(head, bus);
				}
			}
			//разрешим прерывание
			CAN_BUS_UNLOCK_TX(bus);
		}
	}
}

void CAN_TX_IRQHandler(can_bus_t *bus) {

	CAN_TypeDef *can = bus->can_ptr[bus->can_n];

	uint32_t TSR = can_TSR_read(can);

	CAN_TSR_RQCP_Handler(bus, TSR);
}

void CAN_RX_FIFO_Full_Error_Set(can_bus_t *bus, int fifo) {
	switch (fifo) {
	case CAN_RX_MAILBOX_0:
		bus->error |= CAN_ERROR_RX0_FULL;
		break;

	case CAN_RX_MAILBOX_1:
		bus->error |= CAN_ERROR_RX1_FULL;
		break;

	default:
		break;
	}
}

void CAN_RX_FIFO_Overrun_Error_Set(can_bus_t *bus, int fifo) {
	switch (fifo) {
	case CAN_RX_MAILBOX_0:
		bus->error |= CAN_ERROR_RX0_OVERRUN;
		break;

	case CAN_RX_MAILBOX_1:
		bus->error |= CAN_ERROR_RX1_OVERRUN;
		break;

	default:
		break;
	}
}

void CAN_RX_FIFO_Full_Error_Clear(can_bus_t *bus, int fifo) {
	switch (fifo) {
	case CAN_RX_MAILBOX_0:
		bus->error &= ~CAN_ERROR_RX0_FULL;
		break;

	case CAN_RX_MAILBOX_1:
		bus->error &= ~CAN_ERROR_RX1_FULL;
		break;

	default:
		break;
	}
}

void CAN_RX_FIFO_Overrun_Error_Clear(can_bus_t *bus, int fifo) {
	switch (fifo) {
	case CAN_RX_MAILBOX_0:
		bus->error &= ~CAN_ERROR_RX0_OVERRUN;
		break;

	case CAN_RX_MAILBOX_1:
		bus->error &= ~CAN_ERROR_RX1_OVERRUN;
		break;

	default:
		break;
	}
}

void CAN_RX_Queue_Full_Error_Set(can_bus_t *bus) {
	bus->error |= CAN_ERROR_RX_QUEUE_FULL;
}

void CAN_RX_Queue_Full_Error_Clear(can_bus_t *bus) {
	bus->error &= ~CAN_ERROR_RX_QUEUE_FULL;
}

/*
 * TODO: разобраться с тем, что делать, если пихать больше некуда
 * CAN_IER_FMPIE_Disabled_Error_Set(bus, fifo);
 * can_IER_FMPIE_set(can_ptr, fifo, 0);
 */

void CAN_RX_IRQHandler(can_bus_t *bus, int fifo) {

	CAN_TypeDef *can_ptr = bus->can_ptr[bus->can_n];

	//получим статус FIFO на входе в прерывание
	uint32_t RFR = can_RFR_read(can_ptr, fifo);

	uint8_t index = 0;

	//FULL: FIFO full
	if (can_RFR_FULL_read(RFR)) {
		//Установим флаг ошибки, так как мы не смогли принять сообщение из-за переполнения очереди
		CAN_RX_FIFO_Full_Error_Set(bus, fifo);
	}

	//FOVR: FIFO overrun
	if (can_RFR_FOVR_read(RFR)) {
		//Установим флаг ошибки, так как мы не смогли принять сообщение из-за переполнения очереди
		CAN_RX_FIFO_Overrun_Error_Set(bus, fifo);
	}

	//если есть сообщения в мейлбоксе
	while(can_RFR_FMP_read(RFR)) {
		//Если можно добавить в очередь
		if(can_bus_rx_queue_can_enqueue(bus)) {
			//Получим указатель на элемент в хвосте очереди и очистим его
			can_rx_frame_queue_t*  tail = can_bus_rx_queue_tail_reset(bus);
			//заполним поля очереди
			if(E_NO_ERROR == can_rx_mailbox_read(can_ptr, fifo,
					&tail->id,
					&tail->dlc,
					&index,
					tail->data)) {
				//вычислим реальный индекс и запишем
				tail->index = bus->index_array[fifo][index];
				//Если успешно добавили в очередь
				if(can_bus_rx_queue_enqueue(bus)) {
					//Освободим FIFO периферии
					can_rx_mailbox_release(can_ptr, fifo);
					//Если был установлен флаг FIFO переполнен, то очистим, так как мы освободили место в FIFO
					if(can_RFR_FOVR_read(RFR)) {
						//Очистим статус переполнения FIFO периферии
						can_RFR_FOVR_clear(can_ptr, fifo);
					} else {
						//Сбросим ошибку переполнения FIFO драйвера
						CAN_RX_FIFO_Overrun_Error_Clear(bus, fifo);
						//Если был установлен флаг FIFO полон, то очистим, так как мы освободили место в FIFO
						if(can_RFR_FULL_read(RFR)) {
							//Очистим статус периферии FIFO полон
							can_RFR_FULL_clear(can_ptr, fifo);
						} else {
							//Сбросим ошибку драйвера FIFO полон
							CAN_RX_FIFO_Full_Error_Clear(bus, fifo);
						}
					}
					//сбросим ошибку очередь полная
					CAN_RX_Queue_Full_Error_Clear(bus);
				}
			}
		} else {
			//установим ошибку очередь полная
			CAN_RX_Queue_Full_Error_Set(bus);
			break;
		}

		//обновим статус FIFO
		RFR = can_RFR_read(can_ptr, fifo);
	}


	//FULL: FIFO full
	if (can_RFR_FULL_read(RFR)) {

		while(can_RFR_FMP_read(RFR)) {
			//Освободим фифо контроллера
			can_rx_mailbox_release(can_ptr, fifo);
			//обновим статус FIFO
			RFR = can_RFR_read(can_ptr, fifo);
		}

		can_RFR_FULL_clear(can_ptr, fifo);
	}

	//FOVR: FIFO overrun
	if (can_RFR_FOVR_read(RFR)) {

		while(can_RFR_FMP_read(RFR)) {
			//Освободим фифо контроллера
			can_rx_mailbox_release(can_ptr, fifo);
			//обновим статус FIFO
			RFR = can_RFR_read(can_ptr, fifo);
		}

		can_RFR_FOVR_clear(can_ptr, fifo);
	}
}


//TODO: пересмотреть обработчик ошибок
void CAN_SCE_IRQHandler(can_bus_t *bus) {

	CAN_TypeDef *can = bus->can_ptr[bus->can_n];

	uint32_t MSR = can_MSR_read(can);

	uint32_t ESR = can_ESR_read(can);

	bus->rx_error_counter = can_ESR_REC_read(ESR);

	bus->tx_error_counter = can_ESR_TEC_read(ESR);

	if (can_MSR_ERRI_read(MSR)) {
		if (can_IER_ERRIE_read(can)) {
			if (can_IER_EWGIE_read(can)) {
				if (can_ESR_EWGF_read(ESR)) {
					if (bus->rx_error_counter >= 96) {
						bus->error |= CAN_ERROR_RX_WARNING;
					}

					if (bus->tx_error_counter >= 96) {
						bus->error |= CAN_ERROR_TX_WARNING;
					}
				} else {
					bus->error &= ~(CAN_ERROR_TX_WARNING | CAN_ERROR_RX_WARNING);
				}
			}

			if (can_IER_EPVIE_read(can)) {
				if (can_ESR_EPVF_read(ESR)) {
					if (bus->rx_error_counter > 127) {
						bus->error |= CAN_ERROR_RX_PASSIVE;
					}

					if (bus->tx_error_counter > 127) {
						bus->error |= CAN_ERROR_TX_PASSIVE;
					}
				} else {
					bus->error &= ~(CAN_ERROR_TX_PASSIVE | CAN_ERROR_RX_PASSIVE);
				}
			}

			if (can_IER_BOFIE_read(can)) {
				if (can_ESR_BOFF_read(ESR)) {
					bus->error |= CAN_ERROR_TX_BUSSOFF;
				} else {
					bus->error &= ~CAN_ERROR_TX_BUSSOFF;
				}
			}

			if (can_IER_LECIE_read(can)) {
				//TODO: добавить возможность вести лог ошибок
				bus->last_error_code = can_ESR_LEC_read(ESR);
			}
		}
		can_MSR_ERRI_clear(can);
	}

	if (can_MSR_WKUI_read(MSR)) {
		if (can_IER_WKUIE_read(can)) {
		}
		can_MSR_WKUI_clear(can);
	}

	if (can_MSR_SLAKI_read(MSR)) {
		if (can_IER_SLKIE_read(can)) {

		}
		can_MSR_SLAKI_clear(can);
	}
}









