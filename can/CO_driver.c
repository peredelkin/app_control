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
#define CAN1_TMEIE		false

//FIFO 0 INTERRUPT
#define CAN1_FMPIE0		false
#define CAN1_FFIE0		false
#define CAN1_FOVIE0		false

//FIFO 1 INTERRUPT
#define CAN1_FMPIE1		false
#define CAN1_FFIE1		false
#define CAN1_FOVIE1		false

//STATUS CHANGE ERROR INTERRUPT
#define CAN1_ERRIE		false
#define CAN1_EWGIE		false
#define CAN1_EPVIE		false
#define CAN1_BOFIE		false
#define CAN1_LECIE		false

#define CAN1_WKUIE		false
#define CAN1_SLKIE		false

void can_interrupts_enable(CAN_TypeDef* can) {
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

void can_interrupts_disable(CAN_TypeDef* can) {
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

void CO_CANsetConfigurationMode(void* CANptr) {
	if(CANptr == NULL) return;

	if(((can_bus_t*)CANptr)->bus == NULL) return;

	CAN_TypeDef* can_bus = (CAN_TypeDef*)(((can_bus_t*)CANptr)->bus);

	can_bus_initialization_request(can_bus);
	can_filter_init_mode(can_bus);
	can_interrupts_disable(can_bus);
}

void CO_CANsetNormalMode(CO_CANmodule_t* CANmodule) {
	if(CANmodule == NULL) return;

	if(CANmodule->CANptr == NULL) return;

	if(((can_bus_t*)CANmodule->CANptr)->bus == NULL) return;

	CAN_TypeDef* can_bus = (CAN_TypeDef*)(((can_bus_t*)CANmodule->CANptr)->bus);

	can_filter_active_mode(can_bus);
	can_bus_initialization_exit(can_bus);
	can_interrupts_enable(can_bus);

	CANmodule->CANnormal = true;
}

CO_ReturnError_t CO_CANbitRate_set(void *CANptr, uint16_t CANbitRate) {
	if(CANptr == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;

	if(((can_bus_t*)CANptr)->bus == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;

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

CO_ReturnError_t CO_CANmodule_init(CO_CANmodule_t* CANmodule, void* CANptr, CO_CANrx_t rxArray[], uint16_t rxSize,
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

void CO_CANmodule_disable(CO_CANmodule_t* CANmodule) {
    if (CANmodule != NULL && CANmodule->CANptr != NULL) {
    	can_bus_t* can_device = (can_bus_t*)(CANmodule->CANptr); //Pointer to CAN device.
    	if(can_device != NULL && can_device->bus != NULL) {
    	   	can_bus_sleep_request(can_device->bus);
    	    can_bus_initialization_request(can_device->bus);
    	}
    }
}

CO_ReturnError_t CO_CANrxBufferInit(
		CO_CANmodule_t* CANmodule,
		uint16_t index,
		uint16_t ident,
		uint16_t mask,
		bool_t rtr,
		void* object,
		void (*CANrx_callback)(void* object, void* message)) {

	if(CANmodule == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;
	if(index < CANmodule->rxSize) return CO_ERROR_ILLEGAL_ARGUMENT;
	if(object == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;
	if(CANrx_callback == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;

	CAN_TypeDef* can_bus = (CAN_TypeDef*)(((can_bus_t*)CANmodule->CANptr)->bus);

	err_t err = E_NO_ERROR;

    /* buffer, which will be configured */
    CO_CANrx_t* buffer = &CANmodule->rxArray[index];

    /* Configure object variables */
    buffer->object = object;
    buffer->pCANrx_callback = CANrx_callback;

    /* CAN identifier and CAN mask, bit aligned with CAN module. */
    uint32_t can_id = (uint32_t)(CAN_FIR_STID & (ident << CAN_FIR_STID_SHIFT));
    uint32_t can_mask = (uint32_t)(CAN_FIR_STID & (mask << CAN_FIR_STID_SHIFT));

    if(rtr) {
    	can_id |= CAN_FIR_RTR;
    	can_mask |= CAN_FIR_RTR;
    }

    err = can_filter_bank_set(can_bus, index, can_id, can_mask);
    if(err == E_INVALID_VALUE || err == E_OUT_OF_RANGE) return CO_ERROR_ILLEGAL_ARGUMENT;

    return CO_ERROR_NO;
}

CO_CANtx_t* CO_CANtxBufferInit(
		CO_CANmodule_t* CANmodule,
		uint16_t index,
		uint16_t ident,
		bool_t rtr,
		uint8_t noOfBytes,
		bool_t syncFlag) {

}

CO_ReturnError_t CO_CANsend(
		CO_CANmodule_t* CANmodule,
		CO_CANtx_t* buffer) {

}

void CO_CANclearPendingSyncPDOs(
		CO_CANmodule_t* CANmodule) {

}

enum {
	CAN_ESR_LEC_No_Error = 0,
	CAN_ESR_LEC_Stuff_Error,
	CAN_ESR_LEC_Form_Error,
	CAN_ESR_LEC_Acknowledgment_Error,
	CAN_ESR_LEC_Bit_recessive_Error,
	CAN_ESR_LEC_Bit_dominant_Error,
	CAN_ESR_LEC_CRC_Error,
	CAN_ESR_LEC_Set_by_software
};

void CO_CANmodule_process(CO_CANmodule_t* CANmodule) {

	can_bus_t* can_device = (can_bus_t*)(CANmodule->CANptr); //Pointer to CAN device.

	uint16_t status = CANmodule->CANerrorStatus;

	uint32_t ESR = can_ESR_read(can_device->bus);

	uint32_t REC = can_ESR_REC_read(ESR);

	uint32_t TEC = can_ESR_TEC_read(ESR);

	if(can_IER_EWGIE_read(can_device->bus)) {
		if(can_ESR_EWGF_read(ESR)) {
			if(REC >= 96) {
				status |= CO_CAN_ERRRX_WARNING;
			}

			if(TEC >= 96) {
				status |= CO_CAN_ERRTX_WARNING;
			}
		} else {
			status &= ~(CO_CAN_ERRTX_WARNING | CO_CAN_ERRRX_WARNING);
		}
	}

	if(can_IER_EPVIE_read(can_device->bus)) {
		if(can_ESR_EPVF_read(ESR)) {
			if(REC > 127) {
				status |= CO_CAN_ERRRX_PASSIVE;
			}

			if(TEC > 127) {
				status |= CO_CAN_ERRTX_PASSIVE;
			}
		} else {
			status &= ~(CO_CAN_ERRTX_PASSIVE | CO_CAN_ERRRX_PASSIVE);
		}
	}

	if(can_IER_BOFIE_read(can_device->bus)) {
		if(can_ESR_BOFF_read(ESR)) {
			status |= CO_CAN_ERRTX_BUS_OFF;
		} else {
			status &= ~CO_CAN_ERRTX_BUS_OFF;
		}
	}

	if(can_IER_LECIE_read(can_device->bus)) {
		uint32_t LEC = can_ESR_LEC_read(ESR);
		switch(LEC) {
		case CAN_ESR_LEC_No_Error:
			break;
		case CAN_ESR_LEC_Stuff_Error:
			break;
		case CAN_ESR_LEC_Form_Error:
			break;
		case CAN_ESR_LEC_Acknowledgment_Error:
			break;
		case CAN_ESR_LEC_Bit_recessive_Error:
			break;
		case CAN_ESR_LEC_Bit_dominant_Error:
			break;
		case CAN_ESR_LEC_CRC_Error:
			break;
		case CAN_ESR_LEC_Set_by_software:
			break;
		default:
			break;
		}
	}

	CANmodule->CANerrorStatus = status;
}


//CAN1_TX_IRQHandler                /* CAN1 TX                      */
//CAN1_RX0_IRQHandler               /* CAN1 RX0                     */
//CAN1_RX1_IRQHandler               /* CAN1 RX1                     */
//CAN1_SCE_IRQHandler               /* CAN1 SCE                     */


void CO_TX_IRQHandler(CO_CANmodule_t* CANmodule) {

	can_bus_t* can_device = (can_bus_t*)(CANmodule->CANptr); //Pointer to CAN device.

	uint32_t TSR = can_TSR_read(can_device->bus);

	if(can_IER_TMEIE_read(can_device->bus)) {
		for(int i = 0; i < 3; i++) {
			if(can_TSR_RQCP_get(TSR, i)) {
				can_TSR_RQCP_clear(can_device->bus, i);
			}
		}
	}
}

void CO_RX_IRQHandler(CO_CANmodule_t* CANmodule, int fifo) {

	can_bus_t* can_device = (can_bus_t*)(CANmodule->CANptr); //Pointer to CAN device.

	err_t err = E_NO_ERROR;

	CO_CANrx_t* buffer = NULL;

	uint32_t RFR = can_RFR_read(can_device->bus, fifo);

	//FOVIE0: FIFO overrun interrupt enabled
	if(can_IER_FOVIE_read(can_device->bus, fifo)) {
		//FOVR: FIFO overrun
		if(can_RFR_FOVR_read(RFR)) {

		}
	}

	//FFIE0: FIFO full interrupt enabled
	if(can_IER_FFIE_read(can_device->bus, fifo)) {
		//FULL: FIFO full
		if(can_RFR_FULL_read(RFR)) {

		}
	}

	//FMPIE0: FIFO message pending interrupt enabled
	if(can_IER_FMPIE_read(can_device->bus, fifo)) {
		//FIFO 0 message pending
		if(can_RFR_FMP_read(RFR)) {
			err = can_rx_mailbox_read_and_release(can_device->bus, fifo, &(can_device->rx_index[fifo]), &(can_device->rx[fifo]));

			buffer = &CANmodule->rxArray[can_device->rx_index[fifo]];

	        /* Call specific function, which will process the message */
	        if ((buffer != NULL) && (buffer->pCANrx_callback != NULL)) {
	            buffer->pCANrx_callback(buffer->object, (void*)&(can_device->rx[fifo]));
	        }
		}
	}
}

void CO_SCE_IRQHandler(CO_CANmodule_t* CANmodule) {

	can_bus_t* can_device = (can_bus_t*)(CANmodule->CANptr); //Pointer to CAN device.

	uint32_t MSR = can_MSR_read(can_device->bus);

	if(can_IER_ERRIE_read(can_device->bus)) {
		if(can_MSR_ERRI_read(MSR)) {

			CO_CANmodule_process(CANmodule);

			can_MSR_ERRI_clear(can_device->bus);
		}
	}

	if(can_IER_WKUIE_read(can_device->bus)) {
		if(can_MSR_WKUI_read(MSR)) {

			can_MSR_WKUI_clear(can_device->bus);
		}
	}

	if(can_IER_SLKIE_read(can_device->bus)) {
		if(can_MSR_SLAKI_read(MSR)) {

			can_MSR_SLAKI_clear(can_device->bus);
		}
	}
}

