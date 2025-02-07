/*
 * CO_CLI_driver.c
 *
 *  Created on: 7 февр. 2025 г.
 *      Author: ruslan
 */

#include "CO_CLI_driver.h"
#include "can/CANopenNode/CANopen.h"
#include "lib/utils/utils.h"

CO_SDO_CLI_Type CO_SDO_CLI_type(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_type;
}

uint8_t CO_SDO_CLI_nodeId(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_nodeId;
}

uint16_t CO_SDO_CLI_index(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_index;
}

uint8_t CO_SDO_CLI_subIndex(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_subIndex;
}

int CO_SDO_CLI_timeout(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_timeout;
}

CO_SDO_CLI_State CO_SDO_CLI_state(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_state;
}

void CO_SDO_CLI_setState(CO_SDO_CLI_Queue* ptr, CO_SDO_CLI_State newState) {
	ptr->m_state = newState;
}

void CO_SDO_CLI_setError(CO_SDO_CLI_Queue* ptr, CO_SDO_CLI_Error newError) {
	ptr->m_error = newError;
}

bool CO_SDO_CLI_cancelled(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_cancel;
}

size_t CO_SDO_CLI_transferSize(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_transferSize;
}

size_t CO_SDO_CLI_transferedDataSize(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_dataTransfered;
}

void CO_SDO_CLI_resetTransferedSize(CO_SDO_CLI_Queue* ptr) {
	ptr->m_dataTransfered = 0;
}

void CO_SDO_CLI_setDataTransfered(CO_SDO_CLI_Queue* ptr, size_t size) {
	ptr->m_dataTransfered = size;
}

void CO_SDO_CLI_dataTransfered(CO_SDO_CLI_Queue* ptr, size_t size) {
	ptr->m_dataTransfered += size;
}

bool CO_SDO_CLI_dataTransferDone(CO_SDO_CLI_Queue* ptr) {
	return ptr->m_dataTransfered >= ptr->m_transferSize;
}

size_t CO_SDO_CLI_dataSizeToTransfer(CO_SDO_CLI_Queue *ptr) {
	if (ptr->m_dataTransfered >= ptr->m_transferSize) return 0;

	return ptr->m_transferSize - ptr->m_dataTransfered;
}

void* CO_SDO_CLI_dataToTransfer(CO_SDO_CLI_Queue *ptr) {
	   if(ptr->m_data == NULL) return NULL;

	    uint8_t* ptr_data = ptr->m_data;

	    return &ptr_data[ptr->m_dataTransfered];
}

size_t CO_SDO_CLI_bufferedDataSize(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_dataBuffered;
}

void CO_SDO_CLI_resetBufferedSize(CO_SDO_CLI_Queue *ptr) {
	ptr->m_dataBuffered = 0;
}

void CO_SDO_CLI_setDataBuffered(CO_SDO_CLI_Queue *ptr, size_t size) {
	ptr->m_dataBuffered = size;
}

void CO_SDO_CLI_dataBuffered(CO_SDO_CLI_Queue *ptr, size_t size) {
	ptr->m_dataBuffered += size;
}

bool CO_SDO_CLI_dataBufferingDone(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_dataBuffered >= ptr->m_transferSize;
}

size_t CO_SDO_CLI_dataSizeToBuffering(CO_SDO_CLI_Queue *ptr) {
	if (ptr->m_dataBuffered >= ptr->m_transferSize)
		return 0;

	return ptr->m_transferSize - ptr->m_dataBuffered;
}

void* CO_SDO_CLI_dataToBuffering(CO_SDO_CLI_Queue *ptr) {
	if (ptr->m_data == NULL)
		return NULL;

	uint8_t *ptr_data = ptr->m_data;

	return &ptr_data[ptr->m_dataBuffered];
}

bool CO_SDO_CLI_process(uint32_t dt) {
	//if(m_sdoComms.isEmpty()) return false;

	//TODO: не забыть убрать заглушки
	CO_SDO_CLI_Queue *head = NULL; //m_sdoComms.head();
	CO_t *co = NULL;
	bool m_SDOclientBlockTransfer = true;
	uint32_t m_cobidClientToServer = 0;
	uint32_t m_cobidServerToClient = 0;
	int m_defaultTimeout = 0;

	int timeout = (CO_SDO_CLI_timeout(head) == 0 ? m_defaultTimeout : CO_SDO_CLI_timeout(head));
	int timeout_min = MIN(timeout, UINT16_MAX);

	size_t size_ret = 0;
	size_t size_to_ret = 0;
	CO_SDOclient_t *sdo_cli = co->SDOclient;
	CO_SDO_return_t sdo_ret = CO_SDO_RT_ok_communicationEnd;
	CO_SDO_abortCode_t sdo_abort_ret = CO_SDO_AB_NONE;

	if (CO_SDO_CLI_type(head) == CO_SDO_CLI_Type_DOWNLOAD) {

		switch (CO_SDO_CLI_state(head)) {
		case CO_SDO_CLI_State_QUEUED: {
			uint32_t cobidCliToSrv = m_cobidClientToServer + CO_SDO_CLI_nodeId(head);
			uint32_t cobidSrvToCli = m_cobidServerToClient + CO_SDO_CLI_nodeId(head);
			sdo_ret = CO_SDOclient_setup(sdo_cli, cobidCliToSrv, cobidSrvToCli, CO_SDO_CLI_nodeId(head));
			if (sdo_ret != CO_SDO_RT_ok_communicationEnd) {
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head,CO_SDO_CLI_Error_IO);
				return true;
			}
			CO_SDO_CLI_setState(head, CO_SDO_CLI_State_INIT);
		}

			//no break
		case CO_SDO_CLI_State_INIT:
			sdo_ret = CO_SDOclientDownloadInitiate(sdo_cli, CO_SDO_CLI_index(head), CO_SDO_CLI_subIndex(head),
					CO_SDO_CLI_transferSize(head), timeout_min, m_SDOclientBlockTransfer);
			if (sdo_ret < CO_SDO_RT_ok_communicationEnd) {
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head,CO_SDO_CLI_Error_IO);
				return true;
			} else if (sdo_ret > CO_SDO_RT_ok_communicationEnd) {
				break;
			}
			CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DATA);
			dt = 0;

			//no break
		case CO_SDO_CLI_State_DATA:
			size_ret = CO_SDOclientDownloadBufWrite(sdo_cli, CO_SDO_CLI_dataToBuffering(head),
					CO_SDO_CLI_dataSizeToBuffering(head));
			CO_SDO_CLI_dataBuffered(head, size_ret);
			if (CO_SDO_CLI_dataBufferingDone(head)) {
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_RUN);
			}

			//no break
		case CO_SDO_CLI_State_RUN:
			sdo_ret = CO_SDOclientDownload(sdo_cli, dt, CO_SDO_CLI_cancelled(head), !CO_SDO_CLI_dataBufferingDone(head),
					&sdo_abort_ret, &size_ret, NULL);
			CO_SDO_CLI_setDataTransfered(head, size_ret);
			if (sdo_ret == 0) {
				if (CO_SDO_CLI_cancelled(head)) {
					CO_SDO_CLI_setError(head,CO_SDO_CLI_Error_CANCEL);
				} else {
					CO_SDO_CLI_setError(head,CO_SDO_CLI_Error_NONE);
				}
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
			} else if (sdo_ret < 0) {
				//qDebug() << size_ret << Qt::hex << sdo_abort_ret;
				CO_SDO_CLI_Error finish_err = 0; //sdoCommError(sdo_abort_ret); //TODO: заглушка!
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head,finish_err);
			} else {
				break;
			}

			//no break
		case CO_SDO_CLI_State_DONE:
			CO_SDOclientClose(sdo_cli);

			//no break
		case CO_SDO_CLI_State_IDLE:
			//m_sdoComms.dequeue();
			//head->finish();
			return true;
		}
	} else { // sdoc->type() == SDOCommunication::UPLOAD

		switch (CO_SDO_CLI_state(head)) {
		case CO_SDO_CLI_State_QUEUED: {
			uint32_t cobidCliToSrv = m_cobidClientToServer + CO_SDO_CLI_nodeId(head);
			uint32_t cobidSrvToCli = m_cobidServerToClient + CO_SDO_CLI_nodeId(head);
			sdo_ret = CO_SDOclient_setup(sdo_cli, cobidCliToSrv, cobidSrvToCli, CO_SDO_CLI_nodeId(head));
			if (sdo_ret != CO_SDO_RT_ok_communicationEnd) {
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head,CO_SDO_CLI_Error_IO);
				return true;
			}
			CO_SDO_CLI_setState(head,CO_SDO_CLI_State_INIT);
		}

			//no break
		case CO_SDO_CLI_State_INIT:
			sdo_ret = CO_SDOclientUploadInitiate(sdo_cli, CO_SDO_CLI_index(head), CO_SDO_CLI_subIndex(head),
					timeout_min, m_SDOclientBlockTransfer);
			if (sdo_ret < CO_SDO_RT_ok_communicationEnd) {
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head,CO_SDO_CLI_Error_IO);
				return true;
			} else if (sdo_ret > CO_SDO_RT_ok_communicationEnd) {
				break;
			}
			CO_SDO_CLI_setState(head,CO_SDO_CLI_State_RUN);
			dt = 0;

			//no break
		case CO_SDO_CLI_State_RUN:
			sdo_ret = CO_SDOclientUpload(sdo_cli, dt, CO_SDO_CLI_cancelled(head), &sdo_abort_ret, &size_to_ret,
					&size_ret,
					NULL);
			CO_SDO_CLI_setDataBuffered(head, size_ret);
			if (sdo_ret == 0) {
				if (CO_SDO_CLI_cancelled(head)) {
					CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
					CO_SDO_CLI_setError(head,CO_SDO_CLI_Error_CANCEL);
					return true;
				}
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DATA);
			} else if (sdo_ret < 0) {
				//qDebug() << size_ret << Qt::hex << sdo_abort_ret;
				CO_SDO_CLI_Error finish_err = 0; //sdoCommError(sdo_abort_ret); TODO: заглушка!
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head,finish_err);
				return true;
			} else {
				if (sdo_ret == CO_SDO_RT_blockUploadInProgress) {
					break;
				}
#if defined(SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH) && SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH == 1
                if(size_to_ret > 0){
                    if(size_to_ret != CO_SDO_CLI_transferSize(head)){
                        CO_SDO_CLI_setState(CO_SDO_CLI_State_DONE);
                        CO_SDO_CLI_setError(CO_SDO_CLI_Error_INVALID_SIZE);
                        return true;
                    }
                }
#endif
				if (size_ret == 0) {
					break;
				}
			}

			//no break
		case CO_SDO_CLI_State_DATA:
			size_ret = CO_SDOclientUploadBufRead(sdo_cli, (CO_SDO_CLI_dataToTransfer(head)),
					CO_SDO_CLI_dataSizeToTransfer(head));
			CO_SDO_CLI_dataTransfered(head, size_ret);

			if (CO_SDO_CLI_dataTransferDone(head)) {
				CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head,CO_SDO_CLI_Error_NONE);
			} else {
				if (CO_SDO_CLI_state(head) == CO_SDO_CLI_State_DATA && size_ret == 0) {
#if defined(SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH) && SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH == 1
                    CO_SDO_CLI_setState(CO_SDO_CLI_State_DONE);
                    CO_SDO_CLI_setError(CO_SDO_CLI_Error_INVALID_SIZE);
#else
					CO_SDO_CLI_setState(head,CO_SDO_CLI_State_DONE);
					CO_SDO_CLI_setError(head, CO_SDO_CLI_Error_NONE);
#endif
				} else {
					break;
				}
			}

			//no break
		case CO_SDO_CLI_State_DONE:
			CO_SDOclientClose(sdo_cli);

			//no break
		case CO_SDO_CLI_State_IDLE:
			//m_sdoComms.dequeue();
			//head->finish();
			return true;
		}
	}

	return false;
}
