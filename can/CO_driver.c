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
#define CAN1_TMEIE		true

//FIFO 0 INTERRUPT
#define CAN1_FMPIE0		true
#define CAN1_FFIE0		true
#define CAN1_FOVIE0		true

//FIFO 1 INTERRUPT
#define CAN1_FMPIE1		false
#define CAN1_FFIE1		false
#define CAN1_FOVIE1		false

//STATUS CHANGE ERROR INTERRUPT
#define CAN1_ERRIE		true
#define CAN1_EWGIE		true
#define CAN1_EPVIE		true
#define CAN1_BOFIE		true
#define CAN1_LECIE		true

#define CAN1_WKUIE		false
#define CAN1_SLKIE		false

void can_interrupts_enable(CAN_TypeDef *can) {
	//TRANSMIT INTERRUPT
	can_IER_TMEIE_set(can, CAN1_TMEIE);

	//FIFO 0 INTERRUPT
	can_IER_FMPIE_set(can, 0, CAN1_FMPIE0);
	can_IER_FFIE_set(can, 0, CAN1_FFIE0);
	can_IER_FOVIE_set(can, 0, CAN1_FOVIE0);

	//FIFO 1 INTERRUPT
	can_IER_FMPIE_set(can, 1, CAN1_FMPIE1);
	can_IER_FFIE_set(can, 1, CAN1_FFIE1);
	can_IER_FOVIE_set(can, 1, CAN1_FOVIE1);

	//STATUS CHANGE ERROR INTERRUPT
	can_IER_ERRIE_set(can, CAN1_ERRIE);
	can_IER_EWGIE_set(can, CAN1_EWGIE);
	can_IER_EPVIE_set(can, CAN1_EPVIE);
	can_IER_BOFIE_set(can, CAN1_BOFIE);
	can_IER_LECIE_set(can, CAN1_LECIE);
	can_IER_WKUIE_set(can, CAN1_WKUIE);
	can_IER_SLKIE_set(can, CAN1_SLKIE);
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
	if (CANptr == NULL)
		return;

	if (((can_bus_t*) CANptr)->bus == NULL)
		return;

	CAN_TypeDef *can_bus = (CAN_TypeDef*) (((can_bus_t*) CANptr)->bus);

	can_bus_initialization_request(can_bus);
	//can_bus_sleep_request(can_bus); TODO: //разобраться, почему зависает
	can_filter_init_mode(can_bus);
	can_interrupts_disable(can_bus);
}

void CO_CANsetNormalMode(CO_CANmodule_t *CANmodule) {
	if (CANmodule == NULL)
		return;

	if (CANmodule->CANptr == NULL)
		return;

	if (((can_bus_t*) CANmodule->CANptr)->bus == NULL)
		return;

	CAN_TypeDef *can_bus = (CAN_TypeDef*) (((can_bus_t*) CANmodule->CANptr)->bus);

	can_filter_active_mode(can_bus);
	can_bus_initialization_exit(can_bus);
	can_bus_sleep_exit(can_bus);
	can_interrupts_enable(can_bus);

	CANmodule->CANnormal = true;
}

CO_ReturnError_t CO_CANbitRate_set(void *CANptr, uint16_t CANbitRate) {
	if (CANptr == NULL)
		return CO_ERROR_ILLEGAL_ARGUMENT;

	if (((can_bus_t*) CANptr)->bus == NULL)
		return CO_ERROR_ILLEGAL_ARGUMENT;

	CO_ReturnError_t error = CO_ERROR_NO;

	uint32_t btr = 0xFFFFFFFF; //CAN bit timing

	switch (CANbitRate) {
	case 10:
		//10	0.0000	250	18	15	2	88.9	 0x001e00f9
		btr = 0x001e00f9;
		break;
	case 20:
		//20	0.0000	125	18	15	2	88.9	 0x001e007c
		btr = 0x001e007c;
		break;
	case 50:
		//50	0.0000	50	18	15	2	88.9	 0x001e0031
		btr = 0x001e0031;
		break;
	case 125:
		//125	0.0000	20	18	15	2	88.9	 0x001e0013
		btr = 0x001e0013;
		break;
	case 250:
		//250	0.0000	10	18	15	2	88.9	 0x001e0009
		btr = 0x001e0009;
		break;
	case 500:
		//500	0.0000	5	18	15	2	88.9	 0x001e0004
		btr = 0x001e0004;
		break;
	case 1000:
		//1000	0.0000	3	15	12	2	86.7	 0x001b0002
		btr = 0x001b0002;
		break;
	default:
		//125	0.0000	20	18	15	2	88.9	 0x001e0013
		btr = 0x001e0013;
		error = CO_ERROR_ILLEGAL_BAUDRATE;
		break;
	}

	can_BTR_set((CAN_TypeDef*) (((can_bus_t*) CANptr)->bus), btr);

	return error;
}

CO_ReturnError_t CO_CANmodule_init(CO_CANmodule_t *CANmodule, void *CANptr, CO_CANrx_t rxArray[], uint16_t rxSize,
		CO_CANtx_t txArray[], uint16_t txSize, uint16_t CANbitRate) {
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
		can_bus_t *can_device = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.
		if (can_device != NULL && can_device->bus != NULL) {
			can_bus_sleep_request(can_device->bus);
			can_bus_initialization_request(can_device->bus);
		}
	}
}

CO_ReturnError_t CO_CANrxBufferInit(CO_CANmodule_t *CANmodule, uint16_t index, uint16_t ident, uint16_t mask,
		bool_t rtr, void *object, void (*CANrx_callback)(void *object, void *message)) {

	if (CANmodule == NULL)
		return CO_ERROR_ILLEGAL_ARGUMENT;
	if (index >= CANmodule->rxSize)
		return CO_ERROR_ILLEGAL_ARGUMENT;
	if (object == NULL)
		return CO_ERROR_ILLEGAL_ARGUMENT;
	if (CANrx_callback == NULL)
		return CO_ERROR_ILLEGAL_ARGUMENT;

	CAN_TypeDef *can_bus = (CAN_TypeDef*) (((can_bus_t*) CANmodule->CANptr)->bus);

	err_t err = E_NO_ERROR;

	/* buffer, which will be configured */
	CO_CANrx_t *buffer = &CANmodule->rxArray[index];

	/* Configure object variables */
	buffer->object = object;
	buffer->pCANrx_callback = CANrx_callback;

	/* CAN identifier and CAN mask, bit aligned with CAN module. */
	uint32_t can_id = (uint32_t) (CAN_FIR_STID & (ident << CAN_FIR_STID_SHIFT));
	uint32_t can_mask = (uint32_t) (CAN_FIR_STID & (mask << CAN_FIR_STID_SHIFT));

	if (rtr) {
		can_id |= CAN_FIR_RTR;
		can_mask |= CAN_FIR_RTR;
	}

	err = can_filter_bank_set(can_bus, index, can_id, can_mask);
	if (err == E_INVALID_VALUE || err == E_OUT_OF_RANGE)
		return CO_ERROR_ILLEGAL_ARGUMENT;

	return CO_ERROR_NO;
}

CO_CANtx_t* CO_CANtxBufferInit(CO_CANmodule_t *CANmodule, uint16_t index, uint16_t ident, bool_t rtr, uint8_t noOfBytes,
		bool_t syncFlag) {

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

	CAN_TypeDef *can_bus = (CAN_TypeDef*) (((can_bus_t*) CANmodule->CANptr)->bus);

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
	bus_err = can_tx_mailbox_write_and_request(can_bus, buffer->ident, buffer->DLC, buffer->data);

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
	can_bus_t *can_device = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.

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
						bus_err = can_tx_mailbox_write_and_request(can_device->bus, buffer->ident, buffer->DLC,
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
							if(CANmodule->CANtxCount > 0) CANmodule->CANtxCount--;
							CANmodule->bufferInhibitFlag = buffer->syncFlag;
							break;

						default:
							break;
						}
					}

					buffer++;
				} /* end of for loop */

				/* Clear counter if no more messages */
//				if (message_index == 0U) {
//					CANmodule->CANtxCount = 0U;
//				}
			} else {
				can_TSR_RQCP_clear(can_device->bus, mailbox); //TODO: определить условие сброса фалага и место вызова
			}
		}
	}
}

void CO_TX_IRQHandler(CO_CANmodule_t *CANmodule) {

	can_bus_t *can_device = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.

	uint32_t TSR = can_TSR_read(can_device->bus);

	if (can_IER_TMEIE_read(can_device->bus)) {
		CO_TSR_RQCP_Handler(CANmodule, TSR);
	}
}

void CO_RX_IRQHandler(CO_CANmodule_t *CANmodule, int fifo) {

	can_bus_t *can_device = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.

	err_t err = E_NO_ERROR;

	CO_CANrx_t *buffer = NULL;

	uint8_t index = 0;

	CO_CANrxMsg_t rcvMsg = { 0 };

	uint32_t RFR = can_RFR_read(can_device->bus, fifo);

	//FOVIE0: FIFO overrun interrupt enabled
	if (can_IER_FOVIE_read(can_device->bus, fifo)) {
		//FOVR: FIFO overrun
		if (can_RFR_FOVR_read(RFR)) {
			switch (fifo) {
			case CAN_RX_MAILBOX_0:
				can_device->error |= CAN_ERROR_RX0_OVERRUN;
				break;

			case CAN_RX_MAILBOX_1:
				can_device->error |= CAN_ERROR_RX1_OVERRUN;
				break;

			default:
				break;
			}

			can_RFR_FOVR_clear(can_device->bus, fifo);
		}
	}

	//FFIE0: FIFO full interrupt enabled
	if (can_IER_FFIE_read(can_device->bus, fifo)) {
		//FULL: FIFO full
		if (can_RFR_FULL_read(RFR)) {
			switch (fifo) {
			case CAN_RX_MAILBOX_0:
				can_device->error |= CAN_ERROR_RX0_FULL;
				break;

			case CAN_RX_MAILBOX_1:
				can_device->error |= CAN_ERROR_RX1_FULL;
				break;

			default:
				break;
			}

			can_RFR_FULL_clear(can_device->bus, fifo);
		}
	}

	//FMPIE0: FIFO message pending interrupt enabled
	if (can_IER_FMPIE_read(can_device->bus, fifo)) {
		//FIFO 0 message pending
		if (can_RFR_FMP_read(RFR)) {
			err = can_rx_mailbox_read_and_release(can_device->bus, fifo, &rcvMsg.ident, &rcvMsg.DLC, &index,
					rcvMsg.data);

			switch(err) {
			case E_NO_ERROR:
				buffer = &CANmodule->rxArray[index];
				break;
			default:
				break;
			}

			/* Call specific function, which will process the message */
			if ((buffer != NULL) && (buffer->pCANrx_callback != NULL)) {
				buffer->pCANrx_callback(buffer->object, (void*) &rcvMsg);
			}
		}
	}
}

void CO_SCE_IRQHandler(CO_CANmodule_t *CANmodule) {

	can_bus_t *can_device = (can_bus_t*) (CANmodule->CANptr); //Pointer to CAN device.

	uint32_t MSR = can_MSR_read(can_device->bus);

	uint32_t ESR = can_ESR_read(can_device->bus);

	can_device->rx_error_counter = can_ESR_REC_read(ESR);

	can_device->tx_error_counter = can_ESR_TEC_read(ESR);

	if (can_IER_ERRIE_read(can_device->bus)) {
		if (can_MSR_ERRI_read(MSR)) {
			if (can_IER_EWGIE_read(can_device->bus)) {
				if (can_ESR_EWGF_read(ESR)) {
					if (can_device->rx_error_counter >= 96) {
						can_device->error |= CAN_ERROR_RX_WARNING;
					}

					if (can_device->tx_error_counter >= 96) {
						can_device->error |= CAN_ERROR_TX_WARNING;
					}
				} else {
					can_device->error &= ~(CAN_ERROR_TX_WARNING | CAN_ERROR_RX_WARNING);
				}
			}

			if (can_IER_EPVIE_read(can_device->bus)) {
				if (can_ESR_EPVF_read(ESR)) {
					if (can_device->rx_error_counter > 127) {
						can_device->error |= CAN_ERROR_RX_PASSIVE;
					}

					if (can_device->tx_error_counter > 127) {
						can_device->error |= CAN_ERROR_TX_PASSIVE;
					}
				} else {
					can_device->error &= ~(CAN_ERROR_TX_PASSIVE | CAN_ERROR_RX_PASSIVE);
				}
			}

			if (can_IER_BOFIE_read(can_device->bus)) {
				if (can_ESR_BOFF_read(ESR)) {
					can_device->error |= CAN_ERROR_TX_BUSSOFF;
				} else {
					can_device->error &= ~CAN_ERROR_TX_BUSSOFF;
				}
			}

			if (can_IER_LECIE_read(can_device->bus)) {
				can_device->last_error_code = can_ESR_LEC_read(ESR);
			}

			can_MSR_ERRI_clear(can_device->bus);
		}
	}

	if (can_IER_WKUIE_read(can_device->bus)) {
		if (can_MSR_WKUI_read(MSR)) {

			can_MSR_WKUI_clear(can_device->bus);
		}
	}

	if (can_IER_SLKIE_read(can_device->bus)) {
		if (can_MSR_SLAKI_read(MSR)) {

			can_MSR_SLAKI_clear(can_device->bus);
		}
	}
}

