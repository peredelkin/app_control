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


//#define CAN_BUS_FILTER_DEBUG

#ifdef CAN_BUS_FILTER_DEBUG
#include <stdio.h>
#include "sys/counter/sys_counter.h"
#endif

err_t can_bus_filter_16b_bank_set(can_bus_t* bus, int filter, uint32_t id, uint32_t mask) {
	if (filter < 0) return E_INVALID_VALUE;
	if (filter > (CAN_FILTER_MAX_COUNT - 1)) return E_OUT_OF_RANGE;

#ifdef CAN_BUS_FILTER_DEBUG
	printf("ALLOC CAN%d FILTER:%d ID:%#08x MASK:%#08x\n", bus->can_n, filter, (unsigned int)id, (unsigned int)mask);
	sys_counter_delay(0, 10000); //10ms
#endif

	CAN_TypeDef* can_master = bus->can_ptr[0];

	int filter_bank = (filter >> 1);

	if(bus->can_n == CAN_BUS_SLAVE) {
		filter_bank += can_master_can2_filter_start_bank_get(can_master);
	}

	if (filter_bank > 27) return E_OUT_OF_RANGE;

	int filter_bank_index = (filter >> 1);
	int filter_bank_subindex = (filter & 0b1);

	int fifo_n = (filter_bank_index & 0x1);
	int fifo_index = (filter_bank_index + filter_bank_subindex - fifo_n);

	bool filter_was_active = false;
	bool filter_was_single = false;

	can_filter_32b_t prev_32b_id;
	can_filter_32b_t prev_32b_mask;

	can_filter_32b_t next_32b_id;
	can_filter_32b_t next_32b_mask;

	can_filter_16b_t new_16b[2];

	//находятся ли фильтры в режиме инициализации
	bool init_mode = can_master_filter_is_init(can_master);

	//если фильтры активны
	if(init_mode == false) {
		//переведем в режим инициализации
		can_master_filter_init_mode(can_master);
	}

	can_master_filter_is_active(can_master, filter_bank, &filter_was_active);

	can_master_filter_is_single_scale(can_master, filter_bank, &filter_was_single);

	can_master_filter_set_inactive(can_master, filter_bank);

	can_master_filter_set_mask_mode(can_master, filter_bank);

	switch (fifo_n) {
	case CAN_RX_MAILBOX_0:
		can_master_filter_assigned_to_fifo_0(can_master, filter_bank);
		break;
	case CAN_RX_MAILBOX_1:
		can_master_filter_assigned_to_fifo_1(can_master, filter_bank);
		break;
	default:
		return E_INVALID_VALUE;
	}

	bus->index_array[fifo_n][fifo_index] = filter;

	if (filter_bank_subindex) {
		//фильтры должны быть настроены последовательно!
		if(filter_was_active == false || filter_was_single == false) return E_INVALID_OPERATION;

		prev_32b_id.all = can_master->sFilterRegister[filter_bank].FR1;
		prev_32b_mask.all = can_master->sFilterRegister[filter_bank].FR2;

		next_32b_id.all = id;
		next_32b_mask.all = mask;

		//id 0
		new_16b[0].bit.id_exid_15_17 = prev_32b_id.bit.exid_15_17;
		new_16b[0].bit.id_ide = prev_32b_id.bit.ide;
		new_16b[0].bit.id_rtr = prev_32b_id.bit.rtr;
		new_16b[0].bit.id_stid_0_10 = prev_32b_id.bit.stid_0_10;
		//mask 0
		new_16b[0].bit.mask_exid_15_17 = prev_32b_mask.bit.exid_15_17;
		new_16b[0].bit.mask_ide = prev_32b_mask.bit.ide;
		new_16b[0].bit.mask_rtr = prev_32b_mask.bit.rtr;
		new_16b[0].bit.mask_stid_0_10 = prev_32b_mask.bit.stid_0_10;

		//id 1
		new_16b[1].bit.id_exid_15_17 = next_32b_id.bit.exid_15_17;
		new_16b[1].bit.id_ide = next_32b_id.bit.ide;
		new_16b[1].bit.id_rtr = next_32b_id.bit.rtr;
		new_16b[1].bit.id_stid_0_10 = next_32b_id.bit.stid_0_10;
		//mask 1
		new_16b[1].bit.mask_exid_15_17 = next_32b_mask.bit.exid_15_17;
		new_16b[1].bit.mask_ide = next_32b_mask.bit.ide;
		new_16b[1].bit.mask_rtr = next_32b_mask.bit.rtr;
		new_16b[1].bit.mask_stid_0_10 = next_32b_mask.bit.stid_0_10;

		can_master_filter_set_dual_scale(can_master, filter_bank);

		can_master->sFilterRegister[filter_bank].FR1 = new_16b[0].all;
		can_master->sFilterRegister[filter_bank].FR2 = new_16b[1].all;
	} else {
		can_master_filter_set_single_scale(can_master, filter_bank);

		can_master->sFilterRegister[filter_bank].FR1 = id;
		can_master->sFilterRegister[filter_bank].FR2 = mask;
	}

	can_master_filter_set_active(can_master, filter_bank);

	//если фильтры были активны
	if(init_mode == false) {
		//вернем в активный режим
		can_master_filter_active_mode(can_master);
	}

	/* Last CANopen Index for CAN bridge */
	bus->last_index = filter;

	return E_NO_ERROR;
}

err_t can_bus_filter_set(can_bus_t* bus, int filter, uint32_t id, uint32_t mask) {
	if (filter < 0) return E_INVALID_VALUE;
	if (filter > (CAN_FILTER_MAX_COUNT - 1)) return E_OUT_OF_RANGE;

#ifdef CAN_BUS_FILTER_DEBUG
	printf("SET CAN%d FILTER:%d ID:%#08x MASK:%#08x\n", bus->can_n, filter, (unsigned int)id, (unsigned int)mask);
	sys_counter_delay(0, 10000); //10ms
#endif

	CAN_TypeDef* can_master = bus->can_ptr[0];

	int filter_bank = (filter >> 1);

	if(bus->can_n == CAN_BUS_SLAVE) {
		filter_bank += can_master_can2_filter_start_bank_get(can_master);
	}

	if (filter_bank > 27) return E_OUT_OF_RANGE;

	int filter_bank_subindex = (filter & 0b1);

	bool filter_was_active = false;

	can_master_filter_is_active(can_master, filter_bank, &filter_was_active);

	if(filter_was_active == false) return E_INVALID_OPERATION;

	can_master_filter_set_inactive(can_master, filter_bank);

	bool filter_was_single = false;

	can_master_filter_is_single_scale(can_master, filter_bank, &filter_was_single);

	if(filter_was_single) {
		can_master->sFilterRegister[filter_bank].FR1 = id;
		can_master->sFilterRegister[filter_bank].FR2 = mask;
	} else {
		can_filter_32b_t new_32b_id = {id};
		can_filter_32b_t new_32b_mask = {mask};

		can_filter_16b_t new_16b;

		//id
		new_16b.bit.id_exid_15_17 = new_32b_id.bit.exid_15_17;
		new_16b.bit.id_ide = new_32b_id.bit.ide;
		new_16b.bit.id_rtr = new_32b_id.bit.rtr;
		new_16b.bit.id_stid_0_10 = new_32b_id.bit.stid_0_10;
		//mask
		new_16b.bit.mask_exid_15_17 = new_32b_mask.bit.exid_15_17;
		new_16b.bit.mask_ide = new_32b_mask.bit.ide;
		new_16b.bit.mask_rtr = new_32b_mask.bit.rtr;
		new_16b.bit.mask_stid_0_10 = new_32b_mask.bit.stid_0_10;

		if (filter_bank_subindex) {
			can_master->sFilterRegister[filter_bank].FR2 = new_16b.all;
		} else {
			can_master->sFilterRegister[filter_bank].FR1 = new_16b.all;
		}
	}

	can_master_filter_set_active(can_master, filter_bank);

	return E_NO_ERROR;
}

err_t can_bus_filter_16b_bank_alloc(can_bus_t* bus, int count) {
	err_t err = E_NO_ERROR;

	CAN_TypeDef* can_master = bus->can_ptr[0];

	can_master_filter_init_mode(can_master);

	for(int index = 0; index < count; index++) {
		err = can_bus_filter_16b_bank_set(bus, index, 0, 0);
		if(err != E_NO_ERROR) break;
	}

	can_master_filter_active_mode(can_master);

	return err;
}

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

//RX
bool can_bus_rx_queue_empty(can_bus_t *bus) {
	return (bus->queue_rx.head == bus->queue_rx.tail);
}

bool can_bus_rx_queue_notEmpty(can_bus_t *bus) {
	return (bus->queue_rx.head != bus->queue_rx.tail);
}

can_rx_frame_queue_t* can_bus_rx_queue_head(can_bus_t *bus) {
	return &(bus->queue_rx.queue[bus->queue_rx.head]);
}

bool can_bus_rx_queue_dequeue(can_bus_t *bus) {
	if (bus->queue_rx.head == bus->queue_rx.tail) return false; //empty
	size_t new_head = bus->queue_rx.head + 1;
	if (new_head >= bus->queue_rx.size) new_head = 0;
	bus->queue_rx.head = new_head;
	return true;
}

can_rx_frame_queue_t* can_bus_rx_queue_tail(can_bus_t *bus) {
	return &(bus->queue_rx.queue[bus->queue_rx.tail]);
}

bool can_bus_rx_queue_can_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_rx.tail + 1;
	if (new_tail >= bus->queue_rx.size) new_tail = 0;
	if (new_tail == bus->queue_rx.head) return false; //ovf
	return true;
}

bool can_bus_rx_queue_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_rx.tail + 1;
	if (new_tail >= bus->queue_rx.size) new_tail = 0;
	if (new_tail == bus->queue_rx.head) return false; //ovf
	bus->queue_rx.tail = new_tail;
	return true;
}

bool can_bus_rx_process(can_bus_t* bus) {
	//если нечего принимать
	if (can_bus_rx_queue_empty(bus)) return false;

	err_t rx_err = E_NO_ERROR;

	do {
		//если колбек задан
		if(bus->rx_callback != NULL) {
			rx_err = bus->rx_callback(bus);
		} else {
			rx_err = E_NULL_POINTER;
		}

		if(rx_err == E_NO_ERROR) {
			can_bus_rx_queue_dequeue(bus);
		}
	} while ((rx_err == E_NO_ERROR) && can_bus_rx_queue_notEmpty(bus));

	if((rx_err == E_NO_ERROR) || (rx_err == E_BUSY)) return true;

	return false;
}

void can_bus_rx_queue_init(can_bus_t *bus, can_rx_frame_queue_t* queue, size_t queue_size) {
	bus->queue_rx.queue = queue;
	bus->queue_rx.size = queue_size;
	bus->queue_rx.head = 0;
	bus->queue_rx.tail = 0;
}

//TX
bool can_bus_tx_queue_empty(can_bus_t *bus) {
	return (bus->queue_tx.head == bus->queue_tx.tail);
}

bool can_bus_tx_queue_notEmpty(can_bus_t *bus) {
	return (bus->queue_tx.head != bus->queue_tx.tail);
}

can_tx_frame_queue_t* can_bus_tx_queue_head(can_bus_t *bus) {
	return &(bus->queue_tx.queue[bus->queue_tx.head]);
}

bool can_bus_tx_queue_dequeue(can_bus_t *bus) {
	if (bus->queue_tx.head == bus->queue_tx.tail) return false; //empty
	size_t new_head = bus->queue_tx.head + 1;
	if (new_head >= bus->queue_tx.size) new_head = 0;
	bus->queue_tx.head = new_head;
	return true;
}

can_tx_frame_queue_t* can_bus_tx_queue_tail(can_bus_t *bus) {
	return &(bus->queue_tx.queue[bus->queue_tx.tail]);
}

bool can_bus_tx_queue_can_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_tx.tail + 1;
	if (new_tail >= bus->queue_tx.size) new_tail = 0;
	if (new_tail == bus->queue_tx.head) return false; //ovf
	return true;
}

bool can_bus_tx_queue_enqueue(can_bus_t *bus) {
	size_t new_tail = bus->queue_tx.tail + 1;
	if (new_tail >= bus->queue_tx.size) new_tail = 0;
	if (new_tail == bus->queue_tx.head) return false; //ovf
	bus->queue_tx.tail = new_tail;
	return true;
}

bool can_bus_tx_process(can_bus_t* bus) {
	//если нечего передавать
	if (can_bus_tx_queue_empty(bus)) return false;

	CAN_TypeDef *can = bus->can_ptr[bus->can_n];

	can_tx_frame_queue_t* head = NULL;
	err_t tx_err = E_NO_ERROR;

	do {
		//Получим указатель
		head = can_bus_tx_queue_head(bus);
		//заполним регистры
		tx_err = can_tx_mailbox_write_and_request(can, head->id, head->dlc, head->data);
		if(tx_err == E_NO_ERROR) {
			can_bus_tx_queue_dequeue(bus);
		}
	} while ((tx_err == E_NO_ERROR) && can_bus_tx_queue_notEmpty(bus));

	if((tx_err == E_NO_ERROR) || (tx_err == E_BUSY)) return true;

	return false;
}

void can_bus_tx_queue_init(can_bus_t *bus, can_tx_frame_queue_t* queue, size_t queue_size) {
	bus->queue_tx.queue = queue;
	bus->queue_tx.size = queue_size;
	bus->queue_tx.head = 0;
	bus->queue_tx.tail = 0;
}

bool can_bus_write(can_bus_t* bus, uint32_t id, uint8_t dlc, uint8_t* data) {
	//можно ли добавить в очередь
	if (can_bus_tx_queue_can_enqueue(bus) == false) return false;

	//получим указатель
	can_tx_frame_queue_t* tail = can_bus_tx_queue_tail(bus);

	//заполним данные
	tail->id = id;
	tail->dlc = dlc;
	memcpy(tail->data, data, dlc); //copy DATA

	//добавим в очередь
	can_bus_tx_queue_enqueue(bus);

	return true;
}

//IRQ Handlers
void CAN_TSR_RQCP_Handler(can_bus_t *can_bus, uint32_t TSR) {

	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	can_tx_frame_queue_t *head = NULL;
	err_t tx_err = E_NO_ERROR;

	//проверим все 3 мейлбокса
	for (int mailbox = 0; mailbox < 3; mailbox++) {
		//запрос выполнен?
		if (can_TSR_RQCP_get(TSR, mailbox)) {
			//очистим запрос
			can_TSR_RQCP_clear(can, mailbox);
			//если есть что передать
			if (can_bus_tx_queue_notEmpty(can_bus)) {
				//Получим указатель
				head = can_bus_tx_queue_head(can_bus);
				//заполним регистры
				tx_err = can_tx_mailbox_write_and_request(can, head->id, head->dlc, head->data);
				if (tx_err == E_NO_ERROR) {
					can_bus_tx_queue_dequeue(can_bus);
				}
			}
		}
	}
}

void CAN_TX_IRQHandler(can_bus_t *can_bus) {

	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	uint32_t TSR = can_TSR_read(can);

	if (can_IER_TMEIE_read(can)) {
		CAN_TSR_RQCP_Handler(can_bus, TSR);
	}
}

void CAN_RX_IRQHandler(can_bus_t *can_bus, int fifo) {

	CAN_TypeDef *can_ptr = can_bus->can_ptr[can_bus->can_n];

	uint32_t RFR = can_RFR_read(can_ptr, fifo);

	uint8_t index;

	//FMPIE0: FIFO message pending interrupt enabled
	if (can_IER_FMPIE_read(can_ptr, fifo)) {
		//FIFO 0 message pending
		if (can_RFR_FMP_read(RFR)) {
			//Если можно добавить в очередь
			if(can_bus_rx_queue_can_enqueue(can_bus)) {
				//Получим указатель
				can_rx_frame_queue_t*  tail = can_bus_rx_queue_tail(can_bus);
				//заполним поля очереди
				if(E_NO_ERROR == can_rx_mailbox_read(can_ptr, fifo,
						&tail->id,
						&tail->dlc,
						&index,
						tail->data)) {
					//вычислим реальный индекс и запишем
					tail->index = can_bus->index_array[fifo][index]; //TODO: перенести вычисление индекса в функцию can_rx_mailbox_read
					//Если успешно добавили в очередь
					if(can_bus_rx_queue_enqueue(can_bus)) {
						//Освободим фифо контроллера
						can_rx_mailbox_release(can_ptr, fifo);
					}
				}
			}
		}
	}

	//FULL: FIFO full
	if (can_RFR_FULL_read(RFR)) {
		switch (fifo) {
		case CAN_RX_MAILBOX_0:
			can_bus->error |= CAN_ERROR_RX0_FULL;
			break;

		case CAN_RX_MAILBOX_1:
			can_bus->error |= CAN_ERROR_RX1_FULL;
			break;

		default:
			break;
		}

		//FFIE0: FIFO full interrupt enabled
		if (can_IER_FFIE_read(can_ptr, fifo)) {

		}

		can_RFR_FULL_clear(can_ptr, fifo);
	}

	//FOVR: FIFO overrun
	if (can_RFR_FOVR_read(RFR)) {
		switch (fifo) {
		case CAN_RX_MAILBOX_0:
			can_bus->error |= CAN_ERROR_RX0_OVERRUN;
			break;

		case CAN_RX_MAILBOX_1:
			can_bus->error |= CAN_ERROR_RX1_OVERRUN;
			break;

		default:
			break;
		}

		//FOVIE0: FIFO overrun interrupt enabled
		if (can_IER_FOVIE_read(can_ptr, fifo)) {

		}

		can_RFR_FOVR_clear(can_ptr, fifo);
	}
}


void CAN_SCE_IRQHandler(can_bus_t *can_bus) {

	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	uint32_t MSR = can_MSR_read(can);

	uint32_t ESR = can_ESR_read(can);

	can_bus->rx_error_counter = can_ESR_REC_read(ESR);

	can_bus->tx_error_counter = can_ESR_TEC_read(ESR);

	if (can_IER_ERRIE_read(can)) {
		if (can_MSR_ERRI_read(MSR)) {
			if (can_IER_EWGIE_read(can)) {
				if (can_ESR_EWGF_read(ESR)) {
					if (can_bus->rx_error_counter >= 96) {
						can_bus->error |= CAN_ERROR_RX_WARNING;
					}

					if (can_bus->tx_error_counter >= 96) {
						can_bus->error |= CAN_ERROR_TX_WARNING;
					}
				} else {
					can_bus->error &= ~(CAN_ERROR_TX_WARNING | CAN_ERROR_RX_WARNING);
				}
			}

			if (can_IER_EPVIE_read(can)) {
				if (can_ESR_EPVF_read(ESR)) {
					if (can_bus->rx_error_counter > 127) {
						can_bus->error |= CAN_ERROR_RX_PASSIVE;
					}

					if (can_bus->tx_error_counter > 127) {
						can_bus->error |= CAN_ERROR_TX_PASSIVE;
					}
				} else {
					can_bus->error &= ~(CAN_ERROR_TX_PASSIVE | CAN_ERROR_RX_PASSIVE);
				}
			}

			if (can_IER_BOFIE_read(can)) {
				if (can_ESR_BOFF_read(ESR)) {
					can_bus->error |= CAN_ERROR_TX_BUSSOFF;
				} else {
					can_bus->error &= ~CAN_ERROR_TX_BUSSOFF;
				}
			}

			if (can_IER_LECIE_read(can)) {
				can_bus->last_error_code = can_ESR_LEC_read(ESR);
			}

			can_MSR_ERRI_clear(can);
		}
	}

	if (can_MSR_WKUI_read(MSR)) {
		if (can_IER_WKUIE_read(can)) {
		}
		can_MSR_WKUI_clear(can);
	}

	if (can_IER_SLKIE_read(can)) {
		if (can_MSR_SLAKI_read(MSR)) {

			can_MSR_SLAKI_clear(can);
		}
	}
}









