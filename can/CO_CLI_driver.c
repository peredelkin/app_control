/*
 * CO_CLI_driver.c
 *
 *  Created on: 7 февр. 2025 г.
 *      Author: ruslan
 */

#include "CO_CLI_driver.h"
#include "can/CANopenNode/CANopen.h"
#include "lib/utils/utils.h"

//Type
CO_SDO_CLI_Type CO_SDO_CLI_type(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_type;
}

void CO_SDO_CLI_setType(CO_SDO_CLI_Queue *ptr, CO_SDO_CLI_Type newType) {
	ptr->m_type = newType;
}

//NodeId
uint8_t CO_SDO_CLI_nodeId(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_nodeId;
}

void CO_SDO_CLI_setNodeId(CO_SDO_CLI_Queue *ptr, uint8_t newNodeId) {
	ptr->m_nodeId = newNodeId;
}

//Index
uint16_t CO_SDO_CLI_index(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_index;
}

void CO_SDO_CLI_setIndex(CO_SDO_CLI_Queue *ptr, uint16_t newIndex) {
	ptr->m_index = newIndex;
}

//SubIndex
uint8_t CO_SDO_CLI_subIndex(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_subIndex;
}

void CO_SDO_CLI_setSubIndex(CO_SDO_CLI_Queue *ptr, uint8_t newSubIndex) {
	ptr->m_subIndex = newSubIndex;
}

//Data
void* CO_SDO_CLI_data(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_data;
}

void CO_SDO_CLI_setData(CO_SDO_CLI_Queue *ptr, void *newData) {
	ptr->m_data = newData;
}

//DataSize
size_t CO_SDO_CLI_dataSize(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_dataSize;
}

void CO_SDO_CLI_setDataSize(CO_SDO_CLI_Queue *ptr, size_t newSize) {
	ptr->m_dataSize = newSize;
}

//Timeout
int CO_SDO_CLI_timeout(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_timeout;
}

void CO_SDO_CLI_setTimeout(CO_SDO_CLI_Queue *ptr, int newTimeout) {
	ptr->m_timeout = newTimeout;
}

//State
CO_SDO_CLI_State CO_SDO_CLI_state(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_state;
}

void CO_SDO_CLI_setState(CO_SDO_CLI_Queue *ptr, CO_SDO_CLI_State newState) {
	ptr->m_state = newState;
}

//Error
CO_SDO_CLI_Error CO_SDO_CLI_error(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_error;
}

void CO_SDO_CLI_setError(CO_SDO_CLI_Queue *ptr, CO_SDO_CLI_Error newError) {
	ptr->m_error = newError;
}

//Cancel
bool CO_SDO_CLI_cancelled(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_cancel;
}

void CO_SDO_CLI_setCancel(CO_SDO_CLI_Queue *ptr, bool newCancel) {
	ptr->m_cancel = newCancel;
}

//TransferSize
size_t CO_SDO_CLI_transferSize(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_transferSize;
}

void CO_SDO_CLI_setTransferSize(CO_SDO_CLI_Queue *ptr, size_t newTransferSize) {
	ptr->m_transferSize = newTransferSize;
}

//DataTransfered
size_t CO_SDO_CLI_transferedDataSize(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_dataTransfered;
}

void CO_SDO_CLI_resetTransferedSize(CO_SDO_CLI_Queue *ptr) {
	ptr->m_dataTransfered = 0;
}

void CO_SDO_CLI_setDataTransfered(CO_SDO_CLI_Queue *ptr, size_t size) {
	ptr->m_dataTransfered = size;
}

void CO_SDO_CLI_dataTransfered(CO_SDO_CLI_Queue *ptr, size_t size) {
	ptr->m_dataTransfered += size;
}

bool CO_SDO_CLI_dataTransferDone(CO_SDO_CLI_Queue *ptr) {
	return ptr->m_dataTransfered >= ptr->m_transferSize;
}

size_t CO_SDO_CLI_dataSizeToTransfer(CO_SDO_CLI_Queue *ptr) {
	if (ptr->m_dataTransfered >= ptr->m_transferSize) return 0;

	return ptr->m_transferSize - ptr->m_dataTransfered;
}

void* CO_SDO_CLI_dataToTransfer(CO_SDO_CLI_Queue *ptr) {
	if (ptr->m_data == NULL) return NULL;

	uint8_t *ptr_data = ptr->m_data;

	return &ptr_data[ptr->m_dataTransfered];
}

//DataBuffered
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
	if (ptr->m_dataBuffered >= ptr->m_transferSize) return 0;

	return ptr->m_transferSize - ptr->m_dataBuffered;
}

void* CO_SDO_CLI_dataToBuffering(CO_SDO_CLI_Queue *ptr) {
	if (ptr->m_data == NULL) return NULL;

	uint8_t *ptr_data = ptr->m_data;

	return &ptr_data[ptr->m_dataBuffered];
}

CO_SDO_CLI_Error CO_SDO_CLI_sdoCommError(CO_SDO_abortCode_t code) {
	switch (code) {
	default:
		break;
	case CO_SDO_AB_NONE:
		return CO_SDO_CLI_Error_NONE;
		//case CO_SDO_AB_TOGGLE_BIT:
	case CO_SDO_AB_TIMEOUT:
		return CO_SDO_CLI_Error_TIMEOUT;
	case CO_SDO_AB_CMD:
	case CO_SDO_AB_BLOCK_SIZE:
	case CO_SDO_AB_SEQ_NUM:
	case CO_SDO_AB_CRC:
		return CO_SDO_CLI_Error_IO;
	case CO_SDO_AB_OUT_OF_MEM:
		return CO_SDO_CLI_Error_OUT_OF_MEM;
	case CO_SDO_AB_UNSUPPORTED_ACCESS:
	case CO_SDO_AB_WRITEONLY:
	case CO_SDO_AB_READONLY:
		return CO_SDO_CLI_Error_ACCESS;
	case CO_SDO_AB_NOT_EXIST:
		return CO_SDO_CLI_Error_NOT_FOUND;
	case CO_SDO_AB_NO_MAP:
	case CO_SDO_AB_MAP_LEN:
		return CO_SDO_CLI_Error_INVALID_VALUE;
	case CO_SDO_AB_PRAM_INCOMPAT:
	case CO_SDO_AB_DEVICE_INCOMPAT:
		return CO_SDO_CLI_Error_GENERAL;
	case CO_SDO_AB_HW:
		return CO_SDO_CLI_Error_IO;
	case CO_SDO_AB_TYPE_MISMATCH:
	case CO_SDO_AB_DATA_LONG:
	case CO_SDO_AB_DATA_SHORT:
		return CO_SDO_CLI_Error_INVALID_SIZE;
	case CO_SDO_AB_SUB_UNKNOWN:
		return CO_SDO_CLI_Error_NOT_FOUND;
	case CO_SDO_AB_INVALID_VALUE:
	case CO_SDO_AB_VALUE_HIGH:
	case CO_SDO_AB_VALUE_LOW:
	case CO_SDO_AB_MAX_LESS_MIN:
		return CO_SDO_CLI_Error_INVALID_VALUE;
	case CO_SDO_AB_NO_RESOURCE:
		return CO_SDO_CLI_Error_IO;
	case CO_SDO_AB_GENERAL:
		return CO_SDO_CLI_Error_GENERAL;
		//case CO_SDO_AB_DATA_TRANSF:
		//case CO_SDO_AB_DATA_LOC_CTRL:
		//case CO_SDO_AB_DATA_DEV_STATE:
	case CO_SDO_AB_DATA_OD:
	case CO_SDO_AB_NO_DATA:
		return CO_SDO_CLI_Error_NO_DATA;
	}

	return CO_SDO_CLI_Error_UNKNOWN;
}

bool CO_SDO_CLI_Queue_empty(CO_SDO_CLI_Driver_t *drv) {
	return (drv->queue_head == drv->queue_tail);
}

bool CO_SDO_CLI_Queue_notEmpty(CO_SDO_CLI_Driver_t *drv) {
	return (drv->queue_head != drv->queue_tail);
}

CO_SDO_CLI_Queue* CO_SDO_CLI_Queue_head(CO_SDO_CLI_Driver_t *drv) {
	return &(drv->queue[drv->queue_head]);
}

bool CO_SDO_CLI_Queue_headNext(CO_SDO_CLI_Driver_t *drv) {
	if (drv->queue_head == drv->queue_tail) return false; //empty
	size_t new_head = drv->queue_head + 1;
	if (new_head >= drv->queue_size) new_head = 0;
	drv->queue_head = new_head;
	return true;
}

CO_SDO_CLI_Queue* CO_SDO_CLI_Queue_tail(CO_SDO_CLI_Driver_t *drv) {
	return &(drv->queue[drv->queue_tail]);
}

bool CO_SDO_CLI_Queue_tailNext(CO_SDO_CLI_Driver_t *drv) {
	size_t new_tail = drv->queue_tail + 1;
	if (new_tail >= drv->queue_size) new_tail = 0;
	if (new_tail == drv->queue_head) return false; //ovf
	drv->queue_tail = new_tail;
	return true;
}

bool CO_SDO_CLI_process(CO_SDO_CLI_Driver_t *drv, uint32_t dt) {
	if (CO_SDO_CLI_Queue_empty(drv))
		return false;

	CO_SDO_CLI_Queue *head = CO_SDO_CLI_Queue_head(drv);

	int timeout = (CO_SDO_CLI_timeout(head) == 0 ? drv->m_defaultTimeout : CO_SDO_CLI_timeout(head));
	int timeout_min = MIN(timeout, UINT16_MAX);

	size_t size_ret = 0;
	size_t size_to_ret = 0;
	CO_SDOclient_t *sdo_cli = drv->sdo_cli;
	CO_SDO_return_t sdo_ret = CO_SDO_RT_ok_communicationEnd;
	CO_SDO_abortCode_t sdo_abort_ret = CO_SDO_AB_NONE;

	if (CO_SDO_CLI_type(head) == CO_SDO_CLI_Type_DOWNLOAD) {

		switch (CO_SDO_CLI_state(head)) {
		case CO_SDO_CLI_State_QUEUED: {
			uint32_t cobidCliToSrv = drv->m_cobidClientToServer + CO_SDO_CLI_nodeId(head);
			uint32_t cobidSrvToCli = drv->m_cobidServerToClient + CO_SDO_CLI_nodeId(head);
			sdo_ret = CO_SDOclient_setup(sdo_cli, cobidCliToSrv, cobidSrvToCli, CO_SDO_CLI_nodeId(head));
			if (sdo_ret != CO_SDO_RT_ok_communicationEnd) {
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head, CO_SDO_CLI_Error_IO);
				return true;
			}
			CO_SDO_CLI_setState(head, CO_SDO_CLI_State_INIT);
		}

			//no break
		case CO_SDO_CLI_State_INIT:
			sdo_ret = CO_SDOclientDownloadInitiate(sdo_cli, CO_SDO_CLI_index(head), CO_SDO_CLI_subIndex(head),
					CO_SDO_CLI_transferSize(head), timeout_min, drv->m_SDOclientBlockTransfer);
			if (sdo_ret < CO_SDO_RT_ok_communicationEnd) {
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head, CO_SDO_CLI_Error_IO);
				return true;
			} else if (sdo_ret > CO_SDO_RT_ok_communicationEnd) {
				break;
			}
			CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DATA);
			dt = 0;

			//no break
		case CO_SDO_CLI_State_DATA:
			size_ret = CO_SDOclientDownloadBufWrite(sdo_cli, CO_SDO_CLI_dataToBuffering(head),
					CO_SDO_CLI_dataSizeToBuffering(head));
			CO_SDO_CLI_dataBuffered(head, size_ret);
			if (CO_SDO_CLI_dataBufferingDone(head)) {
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_RUN);
			}

			//no break
		case CO_SDO_CLI_State_RUN:
			sdo_ret = CO_SDOclientDownload(sdo_cli, dt, CO_SDO_CLI_cancelled(head), !CO_SDO_CLI_dataBufferingDone(head),
					&sdo_abort_ret, &size_ret, NULL);
			CO_SDO_CLI_setDataTransfered(head, size_ret);
			if (sdo_ret == 0) {
				if (CO_SDO_CLI_cancelled(head)) {
					CO_SDO_CLI_setError(head, CO_SDO_CLI_Error_CANCEL);
				} else {
					CO_SDO_CLI_setError(head, CO_SDO_CLI_Error_NONE);
				}
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
			} else if (sdo_ret < 0) {
				CO_SDO_CLI_Error finish_err = CO_SDO_CLI_sdoCommError(sdo_abort_ret);
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head, finish_err);
			} else {
				break;
			}

			//no break
		case CO_SDO_CLI_State_DONE:
			CO_SDOclientClose(sdo_cli);

			//no break
		case CO_SDO_CLI_State_IDLE:
			CO_SDO_CLI_Queue_headNext(drv);
			CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
			return true;
		}

	} else if (CO_SDO_CLI_type(head) == CO_SDO_CLI_Type_UPLOAD) {

		switch (CO_SDO_CLI_state(head)) {
		case CO_SDO_CLI_State_QUEUED: {
			uint32_t cobidCliToSrv = drv->m_cobidClientToServer + CO_SDO_CLI_nodeId(head);
			uint32_t cobidSrvToCli = drv->m_cobidServerToClient + CO_SDO_CLI_nodeId(head);
			sdo_ret = CO_SDOclient_setup(sdo_cli, cobidCliToSrv, cobidSrvToCli, CO_SDO_CLI_nodeId(head));
			if (sdo_ret != CO_SDO_RT_ok_communicationEnd) {
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head, CO_SDO_CLI_Error_IO);
				return true;
			}
			CO_SDO_CLI_setState(head, CO_SDO_CLI_State_INIT);
		}

			//no break
		case CO_SDO_CLI_State_INIT:
			sdo_ret = CO_SDOclientUploadInitiate(sdo_cli, CO_SDO_CLI_index(head), CO_SDO_CLI_subIndex(head),
					timeout_min, drv->m_SDOclientBlockTransfer);
			if (sdo_ret < CO_SDO_RT_ok_communicationEnd) {
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head, CO_SDO_CLI_Error_IO);
				return true;
			} else if (sdo_ret > CO_SDO_RT_ok_communicationEnd) {
				break;
			}
			CO_SDO_CLI_setState(head, CO_SDO_CLI_State_RUN);
			dt = 0;

			//no break
		case CO_SDO_CLI_State_RUN:
			sdo_ret = CO_SDOclientUpload(sdo_cli, dt, CO_SDO_CLI_cancelled(head), &sdo_abort_ret, &size_to_ret,
					&size_ret,
					NULL);
			CO_SDO_CLI_setDataBuffered(head, size_ret);
			if (sdo_ret == 0) {
				if (CO_SDO_CLI_cancelled(head)) {
					CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
					CO_SDO_CLI_setError(head, CO_SDO_CLI_Error_CANCEL);
					return true;
				}
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DATA);
			} else if (sdo_ret < 0) {
				CO_SDO_CLI_Error finish_err = CO_SDO_CLI_sdoCommError(sdo_abort_ret);
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head, finish_err);
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
				CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
				CO_SDO_CLI_setError(head, CO_SDO_CLI_Error_NONE);
			} else {
				if (CO_SDO_CLI_state(head) == CO_SDO_CLI_State_DATA && size_ret == 0) {
#if defined(SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH) && SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH == 1
                    CO_SDO_CLI_setState(CO_SDO_CLI_State_DONE);
                    CO_SDO_CLI_setError(CO_SDO_CLI_Error_INVALID_SIZE);
#else
					CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
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
			CO_SDO_CLI_Queue_headNext(drv);
			CO_SDO_CLI_setState(head, CO_SDO_CLI_State_DONE);
			return true;
		}
	}

	return false;
}

bool CO_SDO_CLI_setRead(CO_SDO_CLI_Driver_t *drv, CO_SDO_CLI_Queue *ptr) {
	if (ptr == NULL) return false;
	if (CO_SDO_CLI_dataSize(ptr) == 0) return false;
	if (CO_SDO_CLI_data(ptr) == NULL) return false;

	CO_SDO_CLI_resetTransferedSize(ptr);
	CO_SDO_CLI_resetBufferedSize(ptr);
	CO_SDO_CLI_setError(ptr, CO_SDO_CLI_Error_NONE);
	CO_SDO_CLI_setCancel(ptr, false);
	CO_SDO_CLI_setType(ptr, CO_SDO_CLI_Type_UPLOAD);
	CO_SDO_CLI_setState(ptr, CO_SDO_CLI_State_QUEUED);

	return CO_SDO_CLI_Queue_tailNext(drv);
}

bool CO_SDO_CLI_read(CO_SDO_CLI_Driver_t *drv, uint8_t devId, uint16_t dataIndex, uint8_t dataSubIndex, void *data,
		size_t dataSize, int timeout) {
	if (data == NULL || dataSize == 0) return false;
	if (devId < 1 || devId > 127) return false;

	CO_SDO_CLI_Queue *ptr = CO_SDO_CLI_Queue_tail(drv);

	CO_SDO_CLI_setNodeId(ptr, devId);
	CO_SDO_CLI_setIndex(ptr, dataIndex);
	CO_SDO_CLI_setSubIndex(ptr, dataSubIndex);
	CO_SDO_CLI_setData(ptr, data);
	if (CO_SDO_CLI_dataSize(ptr) < dataSize) {
		CO_SDO_CLI_setDataSize(ptr, dataSize);
	}
	CO_SDO_CLI_setTransferSize(ptr, dataSize);
	CO_SDO_CLI_setTimeout(ptr, (timeout == 0) ? drv->m_defaultTimeout : timeout);

	return CO_SDO_CLI_setRead(drv, ptr);
}

bool CO_SDO_CLI_setWrite(CO_SDO_CLI_Driver_t *drv, CO_SDO_CLI_Queue *ptr) {
	if (ptr == NULL) return false;
	if (CO_SDO_CLI_dataSize(ptr) == 0) return false;
	if (CO_SDO_CLI_data(ptr) == NULL) return false;

	CO_SDO_CLI_resetTransferedSize(ptr);
	CO_SDO_CLI_resetBufferedSize(ptr);
	CO_SDO_CLI_setError(ptr, CO_SDO_CLI_Error_NONE);
	CO_SDO_CLI_setCancel(ptr, false);
	CO_SDO_CLI_setType(ptr, CO_SDO_CLI_Type_DOWNLOAD);
	CO_SDO_CLI_setState(ptr, CO_SDO_CLI_State_QUEUED);

	return CO_SDO_CLI_Queue_tailNext(drv);
}

bool CO_SDO_CLI_write(CO_SDO_CLI_Driver_t *drv, uint8_t devId, uint16_t dataIndex, uint8_t dataSubIndex, void *data,
		size_t dataSize, int timeout) {
	if (data == NULL || dataSize == 0) return false;
	if (devId < 1 || devId > 127) return false;

	CO_SDO_CLI_Queue *ptr = CO_SDO_CLI_Queue_tail(drv);

	CO_SDO_CLI_setNodeId(ptr, devId);
	CO_SDO_CLI_setIndex(ptr, dataIndex);
	CO_SDO_CLI_setSubIndex(ptr, dataSubIndex);
	CO_SDO_CLI_setData(ptr, data);
	if (CO_SDO_CLI_dataSize(ptr) < dataSize) {
		CO_SDO_CLI_setDataSize(ptr, dataSize);
	}
	CO_SDO_CLI_setTransferSize(ptr, dataSize);
	CO_SDO_CLI_setTimeout(ptr, (timeout == 0) ? drv->m_defaultTimeout : timeout);

	return CO_SDO_CLI_setWrite(drv, ptr);
}

void CO_SDO_CLI_cancel(CO_SDO_CLI_Queue *ptr) {
	CO_SDO_CLI_setCancel(ptr, true);
}
