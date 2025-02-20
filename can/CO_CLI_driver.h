/*
 * CO_CLI_driver.h
 *
 *  Created on: 7 февр. 2025 г.
 *      Author: ruslan
 */

#ifndef CAN_CO_CLI_DRIVER_H_
#define CAN_CO_CLI_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "can/CANopenNode/CANopen.h"

typedef enum {
	CO_SDO_CLI_Type_NONE = 0,
	CO_SDO_CLI_Type_UPLOAD = 1,
	CO_SDO_CLI_Type_DOWNLOAD = 2
} CO_SDO_CLI_Type;

typedef enum {
	CO_SDO_CLI_State_IDLE = 0,
	CO_SDO_CLI_State_QUEUED = 1,
	CO_SDO_CLI_State_INIT = 2,
	CO_SDO_CLI_State_DATA = 3,
	CO_SDO_CLI_State_RUN = 4,
	CO_SDO_CLI_State_DONE = 5
} CO_SDO_CLI_State;

typedef enum {
    CO_SDO_CLI_Error_NONE          = 0,
    CO_SDO_CLI_Error_IO            = 1,
    CO_SDO_CLI_Error_TIMEOUT       = 2,
    CO_SDO_CLI_Error_CANCEL        = 3,
    CO_SDO_CLI_Error_INVALID_SIZE  = 4,
    CO_SDO_CLI_Error_INVALID_VALUE = 5,
    CO_SDO_CLI_Error_ACCESS        = 6,
    CO_SDO_CLI_Error_NOT_FOUND     = 7,
    CO_SDO_CLI_Error_NO_DATA       = 8,
    CO_SDO_CLI_Error_OUT_OF_MEM    = 9,
    CO_SDO_CLI_Error_GENERAL       = 10,
    CO_SDO_CLI_Error_UNKNOWN       = 11
} CO_SDO_CLI_Error;

typedef struct {
	CO_SDO_CLI_Type m_type;
	uint8_t m_nodeId;
	uint16_t m_index;
	uint8_t m_subIndex;
    void* m_data;
    size_t m_dataSize;
    int m_timeout;
    CO_SDO_CLI_State m_state;
    CO_SDO_CLI_Error m_error;
    bool m_cancel;
    size_t m_transferSize;
    size_t m_dataTransfered;
    size_t m_dataBuffered;
} CO_SDO_CLI_Queue;

typedef struct {
	CO_SDOclient_t *sdo_cli;
	bool m_SDOclientBlockTransfer;
	uint32_t m_cobidClientToServer;
	uint32_t m_cobidServerToClient;
	int m_defaultTimeout;
	CO_SDO_CLI_Queue *queue;
	size_t queue_size;
	size_t queue_head;
	size_t queue_tail;
} CO_SDO_CLI_Driver_t;

extern bool CO_SDO_CLI_process(CO_SDO_CLI_Driver_t *drv, uint32_t dt);

extern CO_SDO_CLI_Queue* CO_SDO_CLI_read(CO_SDO_CLI_Driver_t *drv, uint8_t devId,
		uint16_t dataIndex, uint8_t dataSubIndex, void *data, size_t dataSize, int timeout);

extern CO_SDO_CLI_Queue* CO_SDO_CLI_write(CO_SDO_CLI_Driver_t *drv, uint8_t devId,
		uint16_t dataIndex, uint8_t dataSubIndex, void *data, size_t dataSize, int timeout);

#endif /* CAN_CO_CLI_DRIVER_H_ */
