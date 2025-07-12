/*
 * CO_driver.c
 *
 *  Created on: 10 сент. 2024 г.
 *      Author: Ruslan
 */

#include <string.h>
#include <stdbool.h>
#include "301/CO_driver.h"
#include "can/bus/can_bus.h"

//CAN interrupts
//TRANSMIT INTERRUPT
#define CAN_TMEIE_CONF		true

//FIFO 0 INTERRUPT
#define CAN_FMPIE0_CONF		true
#define CAN_FFIE0_CONF		false
#define CAN_FOVIE0_CONF		false

//FIFO 1 INTERRUPT
#define CAN_FMPIE1_CONF		true
#define CAN_FFIE1_CONF		false
#define CAN_FOVIE1_CONF		false

//STATUS CHANGE ERROR INTERRUPT
#define CAN_ERRIE_CONF		true
#define CAN_EWGIE_CONF		true
#define CAN_EPVIE_CONF		true
#define CAN_BOFIE_CONF		true
#define CAN_LECIE_CONF		true

#define CAN_WKUIE_CONF		false
#define CAN_SLKIE_CONF		false

void can_interrupts_enable(CAN_TypeDef *can) {
	//TRANSMIT INTERRUPT
	can_IER_TMEIE_set(can, CAN_TMEIE_CONF);

	//FIFO 0 INTERRUPT
	can_IER_FMPIE_set(can, 0, CAN_FMPIE0_CONF);
	can_IER_FFIE_set(can, 0, CAN_FFIE0_CONF);
	can_IER_FOVIE_set(can, 0, CAN_FOVIE0_CONF);

	//FIFO 1 INTERRUPT
	can_IER_FMPIE_set(can, 1, CAN_FMPIE1_CONF);
	can_IER_FFIE_set(can, 1, CAN_FFIE1_CONF);
	can_IER_FOVIE_set(can, 1, CAN_FOVIE1_CONF);

	//STATUS CHANGE ERROR INTERRUPT
	can_IER_ERRIE_set(can, CAN_ERRIE_CONF);
	can_IER_EWGIE_set(can, CAN_EWGIE_CONF);
	can_IER_EPVIE_set(can, CAN_EPVIE_CONF);
	can_IER_BOFIE_set(can, CAN_BOFIE_CONF);
	can_IER_LECIE_set(can, CAN_LECIE_CONF);
	can_IER_WKUIE_set(can, CAN_WKUIE_CONF);
	can_IER_SLKIE_set(can, CAN_SLKIE_CONF);
}

void can_interrupts_disable(CAN_TypeDef *can) {
	//TRANSMIT INTERRUPT
	can_IER_TMEIE_set(can, false);

	//FIFO 0 INTERRUPT
	can_IER_FMPIE_set(can, 0, false);
	can_IER_FFIE_set(can, 0, false);
	can_IER_FOVIE_set(can, 0, false);

	//FIFO 1 INTERRUPT
	can_IER_FMPIE_set(can, 1, false);
	can_IER_FFIE_set(can, 1, false);
	can_IER_FOVIE_set(can, 1, false);

	//STATUS CHANGE ERROR INTERRUPT
	can_IER_ERRIE_set(can, false);
	can_IER_EWGIE_set(can, false);
	can_IER_EPVIE_set(can, false);
	can_IER_BOFIE_set(can, false);
	can_IER_LECIE_set(can, false);
	can_IER_WKUIE_set(can, false);
	can_IER_SLKIE_set(can, false);
}

void CO_CANsetConfigurationMode(void *CANptr) {
	//проверка указателя CANptr
	if (CANptr == NULL) return;

	//приведение указателя CANptr
	can_bus_t *can_bus = ((can_bus_t*) CANptr);

	//получение указателя CAN
	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	//проверка укащателя CAN
	if (can == NULL) return;

	can_bus_initialization_request(can);
	can_interrupts_disable(can);
}

void CO_CANsetNormalMode(CO_CANmodule_t *CANmodule) {
	//проверка указателя CANmodule
	if (CANmodule == NULL) return;

	//приведение указателя CANptr
	can_bus_t *can_bus = ((can_bus_t*) CANmodule->CANptr);

	//проверка указателя can_bus
	if (can_bus == NULL) return;

	//получение указателя CAN
	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	//проверка указателя CAN
	if (can == NULL) return;

	can_bus_initialization_exit(can);
	can_bus_sleep_exit(can);

	CANmodule->CANnormal = true;

	can_interrupts_enable(can);
}

//for 168MHz
CO_ReturnError_t CO_CANbitRate_set(void *CANptr, uint16_t CANbitRate) {
	//проверка указателя CANptr
	if (CANptr == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;

	//приведение указателя CANptr
	can_bus_t *can_bus = ((can_bus_t*) CANptr);

	//получение указателя CAN
	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	//проверка указателя CAN
	if (can == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;

	CO_ReturnError_t error = CO_ERROR_NO;

	uint32_t btr = 0xFFFFFFFF; //CAN bit timing

	switch (CANbitRate) {
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
		error = CO_ERROR_ILLEGAL_BAUDRATE;
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

CO_ReturnError_t CO_CANmodule_init(CO_CANmodule_t *CANmodule, void *CANptr, CO_CANrx_t rxArray[],
		uint16_t rxSize, CO_CANtx_t txArray[], uint16_t txSize, uint16_t CANbitRate) {
	CO_ReturnError_t error = CO_ERROR_NO;

	/* verify arguments */
	if ((CANmodule == NULL) || (CANptr == NULL) || (rxArray == NULL) || (txArray == NULL)) {
		return CO_ERROR_ILLEGAL_ARGUMENT;
	}

	/* Hold CANModule variable */
	CANmodule->CANptr = CANptr;

	/* Configure object variables */
	CANmodule->rxArray = rxArray;
	CANmodule->rxSize = rxSize;
	CANmodule->txArray = txArray;
	CANmodule->txSize = txSize;
	CANmodule->CANerrorStatus = 0;
	CANmodule->CANnormal = false;
	CANmodule->useCANrxFilters = true; /* Use HW filters */
	CANmodule->bufferInhibitFlag = false;
	CANmodule->firstCANtxMessage = true;
	CANmodule->CANtxCount = 0U;
	CANmodule->errOld = 0U;

	/* Reset all variables */
	for (uint16_t i = 0U; i < rxSize; i++) {
		rxArray[i].object = NULL;
		rxArray[i].pCANrx_callback = NULL;
	}
	for (uint16_t i = 0U; i < txSize; i++) {
		txArray[i].bufferFull = false;
	}

	error = CO_CANbitRate_set(CANptr, CANbitRate);

	return error;
}

void CO_CANmodule_disable(CO_CANmodule_t *CANmodule) {
	if (CANmodule != NULL && CANmodule->CANptr != NULL) {

		can_bus_t *can_bus = ((can_bus_t*) CANmodule->CANptr);

		CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

		if (can == NULL) return;

		can_bus_sleep_request(can);
		can_bus_initialization_request(can);
	}
}

CO_ReturnError_t CO_CANrxBufferInit(CO_CANmodule_t *CANmodule, uint16_t index, uint16_t ident,
		uint16_t mask, bool_t rtr, void *object,
		void (*CANrx_callback)(void *object, void *message)) {

	if (CANmodule == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;
	if (index >= CANmodule->rxSize) return CO_ERROR_ILLEGAL_ARGUMENT;
	if (object == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;
	if (CANrx_callback == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;

	err_t err = E_NO_ERROR;

	/* CAN identifier and CAN mask, bit aligned with CAN module. */
	uint32_t can_id = CAN_BUS_MAKE_ID(ident); //(uint32_t) (CAN_FIR_STID & (ident << CAN_FIR_STID_SHIFT));
	uint32_t can_mask = CAN_BUS_MAKE_MASK(mask); //(uint32_t) (CAN_FIR_STID & (mask << CAN_FIR_STID_SHIFT));

	if (rtr) {
		can_id |= CAN_FIR_RTR;
		can_mask |= CAN_FIR_RTR;
	}

	can_bus_t *can_bus = ((can_bus_t*) CANmodule->CANptr);

	err = can_bus_filter_16b_bank_set(can_bus, index, can_id, can_mask);
	if (err == E_INVALID_VALUE || err == E_OUT_OF_RANGE) return CO_ERROR_ILLEGAL_ARGUMENT;

	/* Last CANopen Index for CAN bridge */
	can_bus->last_filter = index;

	/* buffer, which will be configured */
	CO_CANrx_t *buffer = &CANmodule->rxArray[index];

	/* Configure object variables */
	buffer->object = object;
	buffer->pCANrx_callback = CANrx_callback;

	return CO_ERROR_NO;
}

CO_CANtx_t* CO_CANtxBufferInit(CO_CANmodule_t *CANmodule, uint16_t index, uint16_t ident,
		bool_t rtr, uint8_t noOfBytes, bool_t syncFlag) {

	CO_CANtx_t *buffer = NULL;

	if ((CANmodule != NULL) && (index < CANmodule->txSize)) {
		/* get specific buffer */
		buffer = &CANmodule->txArray[index];

		/* CAN identifier, DLC and rtr, bit aligned with CAN module transmit buffer, microcontroller specific. */
		if (rtr) {
			buffer->ident = (uint32_t) ((CAN_TIR_STID & (ident << CAN_TIR_STID_SHIFT)) | CAN_TIR_RTR);
		} else {
			buffer->ident = (uint32_t) (CAN_TIR_STID & (ident << CAN_TIR_STID_SHIFT));
		}

		buffer->DLC = noOfBytes;

		buffer->bufferFull = false;
		buffer->syncFlag = syncFlag;
	}

	return buffer;
}

CO_ReturnError_t CO_CANsend(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer) {

	can_bus_t *can_bus = ((can_bus_t*) CANmodule->CANptr);

	if (can_bus == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;

	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	CO_ReturnError_t co_err = CO_ERROR_NO;

	err_t bus_err = E_NO_ERROR;

	/* Verify overflow */
	if (buffer->bufferFull) {
		if (!CANmodule->firstCANtxMessage) {
			/* don't set error, if bootup message is still on buffers */
			CANmodule->CANerrorStatus |= CO_CAN_ERRTX_OVERFLOW;
		}
		co_err = CO_ERROR_TX_OVERFLOW;
	}

	CO_LOCK_CAN_SEND(CANmodule);
	/* if CAN TX buffer is free, copy message to it */
	bus_err = can_tx_mailbox_write_and_request(can, buffer->ident, buffer->DLC, buffer->data);

	switch (bus_err) {
	case E_NULL_POINTER:
		co_err = CO_ERROR_ILLEGAL_ARGUMENT;
		break;

	case E_BUSY:
		buffer->bufferFull = true;
		CANmodule->CANtxCount++;
		break;

	case E_OUT_OF_RANGE:
		co_err = CO_ERROR_ILLEGAL_ARGUMENT;
		break;

	case E_NO_ERROR:
		if (CANmodule->CANtxCount == 0) {
			CANmodule->bufferInhibitFlag = buffer->syncFlag;
		}
		break;

	default:
		break;
	}

	CO_UNLOCK_CAN_SEND(CANmodule);

	return co_err;
}

//TODO: сделать отмену TXREQ для SyncPDOs в драйвере, не забыв определить, в каком мейлбоксе находится SyncPDO
void CO_CANclearPendingSyncPDOs(CO_CANmodule_t *CANmodule) {

	uint32_t tpdoDeleted = 0U;

	CO_LOCK_CAN_SEND(CANmodule);
	/* Abort message from CAN module, if there is synchronous TPDO.
	 * Take special care with this functionality. */
	if (/* messageIsOnCanBuffer && */CANmodule->bufferInhibitFlag) {
		/* clear TXREQ */
		CANmodule->bufferInhibitFlag = false;
		tpdoDeleted = 1U;
	}
	/* delete also pending synchronous TPDOs in TX buffers */
	if (CANmodule->CANtxCount != 0U) {
		uint16_t i;
		CO_CANtx_t *buffer = &CANmodule->txArray[0];
		for (i = CANmodule->txSize; i > 0U; i--) {
			if (buffer->bufferFull) {
				if (buffer->syncFlag) {
					buffer->bufferFull = false;
					CANmodule->CANtxCount--;
					tpdoDeleted = 2U;
				}
			}
			buffer++;
		}
	}
	CO_UNLOCK_CAN_SEND(CANmodule);

	if (tpdoDeleted != 0U) {
		CANmodule->CANerrorStatus |= CO_CAN_ERRTX_PDO_LATE;
	}

}

//CO_CAN_ERR

void CO_CANmodule_process(CO_CANmodule_t *CANmodule) {

	can_bus_t *can_device = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.

	uint32_t bus_error = can_device->error;

	uint16_t status = CANmodule->CANerrorStatus;

	if (bus_error & CAN_ERROR_RX_WARNING) {
		status |= CO_CAN_ERRRX_WARNING;
	} else {
		//status &= ~CO_CAN_ERRRX_WARNING;
	}

	if (bus_error & CAN_ERROR_TX_WARNING) {
		status |= CO_CAN_ERRTX_WARNING;
	} else {
		//status &= ~ CO_CAN_ERRTX_WARNING;
	}

	if (bus_error & CAN_ERROR_RX_PASSIVE) {
		status |= CO_CAN_ERRRX_PASSIVE;
	} else {
		//status &= ~ CO_CAN_ERRRX_PASSIVE;
	}

	if (bus_error & CAN_ERROR_TX_PASSIVE) {
		status |= CO_CAN_ERRTX_PASSIVE;
	} else {
		//status &= ~ CO_CAN_ERRTX_PASSIVE;
	}

	if (bus_error & CAN_ERROR_TX_BUSSOFF) {
		status |= CO_CAN_ERRTX_BUS_OFF;
	} else {
		//status &= ~ CO_CAN_ERRTX_BUS_OFF;
	}

	if (bus_error & (CAN_ERROR_RX0_OVERRUN | CAN_ERROR_RX1_OVERRUN)) {
		status |= CO_CAN_ERRRX_OVERFLOW;
	} else {
		//status &= ~CO_CAN_ERRRX_OVERFLOW;
	}

	if (bus_error & (CAN_ERROR_RX0_FULL | CAN_ERROR_RX1_FULL)) {

	} else {

	}

	CANmodule->CANerrorStatus = status;
}

//CAN1_TX_IRQHandler                /* CAN1 TX                      */
//CAN1_RX0_IRQHandler               /* CAN1 RX0                     */
//CAN1_RX1_IRQHandler               /* CAN1 RX1                     */
//CAN1_SCE_IRQHandler               /* CAN1 SCE                     */

void CO_TSR_RQCP_Handler(CO_CANmodule_t *CANmodule, uint32_t TSR) {
	can_bus_t *can_bus = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.

	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	err_t bus_err = E_NO_ERROR;

	int mailbox;

	for (mailbox = 0; mailbox < 3; mailbox++) {
		if (can_TSR_RQCP_get(TSR, mailbox)) {
			/* First CAN message (bootup) was sent successfully */
			CANmodule->firstCANtxMessage = false;
			/* clear flag from previous message */
			CANmodule->bufferInhibitFlag = false;
			/* Are there any new messages waiting to be send */
			if (CANmodule->CANtxCount > 0U) {
				uint16_t message_index; /* index of transmitting message */

				/* first buffer */
				CO_CANtx_t *buffer = &CANmodule->txArray[0];
				/* search through whole array of pointers to transmit message buffers. */
				for (message_index = CANmodule->txSize; message_index > 0U; message_index--) {
					/* if message buffer is full, send it. */
					if (buffer->bufferFull) {
						/* if CAN TX buffer is free, copy message to it */
						bus_err = can_tx_mailbox_write_and_request(can, buffer->ident, buffer->DLC,
								buffer->data);

						switch (bus_err) {
						case E_NULL_POINTER:
							break;

						case E_BUSY:
							break;

						case E_OUT_OF_RANGE:
							break;

						case E_NO_ERROR:
							buffer->bufferFull = false;
							if (CANmodule->CANtxCount > 0) CANmodule->CANtxCount--;
							CANmodule->bufferInhibitFlag = buffer->syncFlag;
							break;

						default:
							break;
						}
					}

					buffer++;
				} /* end of for loop */

				/* Clear counter if no more messages */
				if (message_index == 0U) {
					CANmodule->CANtxCount = 0U;
				}
			} else {
				can_TSR_RQCP_clear(can, mailbox); //TODO: определить условие сброса фалага и место вызова
			}
		}
	}
}

void CO_TX_IRQHandler(CO_CANmodule_t *CANmodule) {

	can_bus_t *can_bus = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.

	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	uint32_t TSR = can_TSR_read(can);

	if (can_IER_TMEIE_read(can)) {
		CO_TSR_RQCP_Handler(CANmodule, TSR);
	}
}

//void CO_can_rx_mailbox_read_and_release(CO_CANmodule_t *CANmodule, int fifo) {
//	can_bus_t *can_bus = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.
//
//	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];
//
//	err_t err = E_NO_ERROR;
//
//	CO_CANrx_t *buffer = NULL;
//
//	uint8_t index = 0;
//	uint8_t CO_index = 0;
//
//	CO_CANrxMsg_t rcvMsg = { 0 };
//
//	err = can_rx_mailbox_read(can, fifo, &rcvMsg.ident, &rcvMsg.DLC, &index,
//			rcvMsg.data);
//
//	CO_index = can_bus->index_array[fifo][index];
//
//	switch (err) {
//	case E_NO_ERROR:
//		buffer = &CANmodule->rxArray[CO_index];
//		break;
//	default:
//		break;
//	}
//
//	/* Call specific function, which will process the message */
//	if ((buffer != NULL) && (buffer->pCANrx_callback != NULL)) {
//		buffer->pCANrx_callback(buffer->object, (void*) &rcvMsg);
//	}
//}

void CO_RX_IRQHandler(CO_CANmodule_t *CANmodule, int fifo) {

	can_bus_t *can_bus = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.

	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	uint32_t RFR = can_RFR_read(can, fifo);

	//FMPIE0: FIFO message pending interrupt enabled
	if (can_IER_FMPIE_read(can, fifo)) {
		//FIFO 0 message pending
		if (can_RFR_FMP_read(RFR)) {
			CO_can_rx_mailbox_read_and_release(CANmodule, fifo);
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
		if (can_IER_FFIE_read(can, fifo)) {

		}

		can_RFR_FULL_clear(can, fifo);
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
		if (can_IER_FOVIE_read(can, fifo)) {

		}

		can_RFR_FOVR_clear(can, fifo);
	}
}

void CO_SCE_IRQHandler(CO_CANmodule_t *CANmodule) {

	can_bus_t *can_bus = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.

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

