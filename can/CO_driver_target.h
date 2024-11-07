/*
 * CO_driver_target.h
 *
 *  Created on: 10 сент. 2024 г.
 *      Author: Ruslan
 */

#ifndef CAN_CANOPENNODE_301_CO_DRIVER_TARGET_H_
#define CAN_CANOPENNODE_301_CO_DRIVER_TARGET_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "bus/can_bus.h"

/*
 * User Configuration.
 */

// User static variables for allocation.
#define CO_USE_GLOBALS 1

// Disable LEDs.
#define CO_CONFIG_LEDS 0

// Disable storage.
#define CO_CONFIG_STORAGE 0

// Disable emergency.
#define CO_CONFIG_EM 0

// Disable LSS.
#define CO_CONFIG_LSS 0

// Disable HB consumer.
#define CO_CONFIG_HB_CONS 0

// HB config.
#define FIRST_HB_TIME_MS 0xffff

// NMT config.
#define CO_CONFIG_NMT (CO_NMT_STARTUP_TO_OPERATIONAL)

// Disable TIME prod/cons.
#define CO_CONFIG_TIME 0

// Disable SYNC.
#define CO_CONFIG_SYNC 0

// Disable PDO.
#define CO_CONFIG_PDO 0

// Enable SDO server.
//#define CO_CONFIG_SDO_SRV

// SDO server timeout.
#define SDO_SERVER_TIMEOUT_MS 500

// Disable SDO client.
#define CO_CONFIG_SDO_CLI 0

// SDO client timeout.
#define SDO_CLIENT_TIMEOUT_MS 500

// SDO client block transfer.
#define SDO_CLIENT_BLOCK_TRANSFER false

// Node Id.
#define NODE_ID 1

#undef CO_CONFIG_STORAGE_ENABLE // We don't need Storage option, implement based on your use case and remove this line from here

/* Basic definitions. If big endian, CO_SWAP_xx macros must swap bytes. */
#define CO_LITTLE_ENDIAN
#define CO_SWAP_16(x) x
#define CO_SWAP_32(x) x
#define CO_SWAP_64(x) x

/* NULL is defined in stddef.h */
/* true and false are defined in stdbool.h */
/* int8_t to uint64_t are defined in stdint.h */
typedef uint_fast8_t bool_t;
typedef float float32_t;
typedef double float64_t;

/**
 * CAN receive callback function which pre-processes received CAN message
 *
 * It is called by fast CAN receive thread. Each \ref CO_obj "CANopenNode Object" defines its own and registers it with
 * CO_CANrxBufferInit(), by passing function pointer.
 *
 * @param object pointer to specific \ref CO_obj "CANopenNode Object", registered with CO_CANrxBufferInit()
 * @param rxMsg pointer to received CAN message
 */
void CANrx_callback(void* object, void* rxMsg);

/**
 * CAN rx message structure.
 */
//typedef struct {
//    uint32_t ident;
//    uint8_t DLC;
//    uint8_t data[8];
//} CO_CANrxMsg_t;

/**
 * CANrx_callback() can read CAN identifier from received CAN message
 *
 * @param rxMsg Pointer to received message
 * @return 11-bit CAN standard identifier.
 */
static inline uint16_t
CO_CANrxMsg_readIdent(void* rxMsg) {
    return ((uint16_t)(((can_bus_rx_t*)(rxMsg)))->id);
}

/**
 * CANrx_callback() can read Data Length Code from received CAN message
 *
 * @param rxMsg Pointer to received message
 * @return data length in bytes (0 to 8)
 */
static inline uint8_t
CO_CANrxMsg_readDLC(void* rxMsg) {
    return ((uint8_t)(((can_bus_rx_t*)(rxMsg)))->dlc);
}

/**
 * CANrx_callback() can read pointer to data from received CAN message
 *
 * @param rxMsg Pointer to received message
 * @return pointer to data buffer
 */
static inline const uint8_t*
CO_CANrxMsg_readData(void* rxMsg) {
    return ((uint8_t*)(((can_bus_rx_t*)(rxMsg)))->data);
}

/**
 * Configuration object for CAN received message for specific \ref CO_obj "CANopenNode Object".
 */
typedef struct {
    //uint16_t ident; /**< Standard CAN Identifier (bits 0..10) + RTR (bit 11) */
    //uint16_t mask;  /**< Standard CAN Identifier mask with the same alignment as ident */
    void* object;   /**< \ref CO_obj "CANopenNode Object" initialized in from CO_CANrxBufferInit() */
    void (*pCANrx_callback)(void* object, void* message); /**< Pointer to CANrx_callback() initialized in CO_CANrxBufferInit() */
} CO_CANrx_t;

/**
 * Configuration object for CAN transmit message for specific \ref CO_obj "CANopenNode Object".
 */
typedef struct {
    uint32_t ident;             /**< CAN identifier as aligned in CAN module */
    uint8_t DLC;                /**< Length of CAN message */
    uint8_t data[8];            /**< 8 data bytes */
    volatile bool_t bufferFull; /**< True if previous message is still in the buffer */
    volatile bool_t syncFlag;   /**< Synchronous PDO messages has this flag set. It prevents them to be sent outside the
                                   synchronous window */
} CO_CANtx_t;

/**
 * Complete CAN module object.
 */
typedef struct {
    void* CANptr;                    /**< From CO_CANmodule_init() */
    CO_CANrx_t* rxArray;             /**< From CO_CANmodule_init() */
    uint16_t rxSize;                 /**< From CO_CANmodule_init() */
    CO_CANtx_t* txArray;             /**< From CO_CANmodule_init() */
    uint16_t txSize;                 /**< From CO_CANmodule_init() */
    uint16_t CANerrorStatus;         /**< CAN error status bitfield, see @ref CO_CAN_ERR_status_t */
    volatile bool_t CANnormal;       /**< CAN module is in normal mode */
    volatile bool_t useCANrxFilters; /**< Value different than zero indicates, that CAN module hardware filters are used
                                        for CAN reception. If there is not enough hardware filters, they won't be used.
                                        In this case will be *all* received CAN messages processed by software. */
    volatile bool_t bufferInhibitFlag; /**< If flag is true, then message in transmit buffer is synchronous PDO message,
                                          which will be aborted, if CO_clearPendingSyncPDOs() function will be called by
                                          application. This may be necessary if Synchronous window time was expired. */
    volatile bool_t firstCANtxMessage; /**< Equal to 1, when the first transmitted message (bootup message) is in CAN TX buffers */
    volatile uint16_t CANtxCount;  /**< Number of messages in transmit buffer, which are waiting to be copied to the CAN module */
    uint32_t errOld; /**< Previous state of CAN errors */

    /* STM32 specific features */
    uint32_t primask_send; /* Primask register for interrupts for send operation */
    uint32_t primask_emcy; /* Primask register for interrupts for emergency operation */
    uint32_t primask_od;   /* Primask register for interrupts for send operation */
} CO_CANmodule_t;


extern void CO_TX_IRQHandler(CO_CANmodule_t* CANmodule);
extern void CO_RX_IRQHandler(CO_CANmodule_t* CANmodule, int fifo);
extern void CO_SCE_IRQHandler(CO_CANmodule_t* CANmodule);


/* (un)lock critical section in CO_CANsend() */
// Why disabling the whole Interrupt
#define CO_LOCK_CAN_SEND(CAN_MODULE)                                                                                   \
    do {                                                                                                               \
        (CAN_MODULE)->primask_send = __get_PRIMASK();                                                                  \
        __disable_irq();                                                                                               \
    } while (0)
#define CO_UNLOCK_CAN_SEND(CAN_MODULE) __set_PRIMASK((CAN_MODULE)->primask_send)

/* (un)lock critical section in CO_errorReport() or CO_errorReset() */
#define CO_LOCK_EMCY(CAN_MODULE)                                                                                       \
    do {                                                                                                               \
        (CAN_MODULE)->primask_emcy = __get_PRIMASK();                                                                  \
        __disable_irq();                                                                                               \
    } while (0)
#define CO_UNLOCK_EMCY(CAN_MODULE) __set_PRIMASK((CAN_MODULE)->primask_emcy)

/* (un)lock critical section when accessing Object Dictionary */
#define CO_LOCK_OD(CAN_MODULE)                                                                                         \
    do {                                                                                                               \
        (CAN_MODULE)->primask_od = __get_PRIMASK();                                                                    \
        __disable_irq();                                                                                               \
    } while (0)
#define CO_UNLOCK_OD(CAN_MODULE) __set_PRIMASK((CAN_MODULE)->primask_od)

/* Synchronization between CAN receive and message processing threads. */
#define CO_MemoryBarrier()
#define CO_FLAG_READ(rxNew) ((rxNew) != NULL)
#define CO_FLAG_SET(rxNew)                                                                                             \
    do {                                                                                                               \
        CO_MemoryBarrier();                                                                                            \
        rxNew = (void*)1L;                                                                                             \
    } while (0)
#define CO_FLAG_CLEAR(rxNew)                                                                                           \
    do {                                                                                                               \
        CO_MemoryBarrier();                                                                                            \
        rxNew = NULL;                                                                                                  \
    } while (0)


#endif /* CAN_CANOPENNODE_301_CO_DRIVER_TARGET_H_ */
