#include "usart_bus.h"
#include "uart/usart.h"
#include "utils/utils.h"

//UART RE
bool usart_bus_receiver_enabled(usart_bus_t* usart) {
	return usart_receiver_state(usart->usart_device);
}

void usart_bus_receiver_enable(usart_bus_t* usart) {
	usart_cr1_set(usart->usart_device, USART_CR1_RE, ENABLE);
	usart_cr1_set(usart->usart_device, USART_CR1_UE, ENABLE);
}

void usart_bus_receiver_disable(usart_bus_t* usart) {
	usart_cr1_set(usart->usart_device, USART_CR1_RE, DISABLE);
}

//UART TE
bool usart_bus_transmitter_enabled(usart_bus_t* usart) {
	return usart_transmitter_state(usart->usart_device);
}

void usart_bus_transmitter_enable(usart_bus_t* usart) {
	usart_cr1_set(usart->usart_device, USART_CR1_TE, ENABLE);
	usart_cr1_set(usart->usart_device, USART_CR1_UE, ENABLE);
}

void usart_bus_transmitter_disable(usart_bus_t* usart) {
	usart_cr1_set(usart->usart_device, USART_CR1_TE, DISABLE);
}

/*
 * Работа с DMA.
 */

//static bool usart_bus_dma_lock_tx_channel(usart_bus_t* usart) {
//    //    usart->dma_tx_locked = dma_channel_trylock(usart->dma_tx_channel);
//    return usart->dma_tx_locked;
//}
//
//static bool usart_bus_dma_lock_rx_channel(usart_bus_t* usart) {
//    //    usart->dma_rx_locked = dma_channel_trylock(usart->dma_rx_channel);
//    return usart->dma_rx_locked;
//}

static void usart_bus_dma_unlock_tx_channel(usart_bus_t* usart) {
    dma_stream_deinit(&usart->dma_tx_channel);
    //    dma_channel_unlock(usart->dma_tx_channel);
//    usart->dma_tx_locked = false;
}

static void usart_bus_dma_unlock_rx_channel(usart_bus_t* usart) {
    dma_stream_deinit(&usart->dma_rx_channel);
    //    dma_channel_unlock(usart->dma_rx_channel);
//    usart->dma_rx_locked = false;
}

static void usart_bus_dma_tx_config(usart_bus_t* usart, void* address, size_t size) {
    dma_stream_enable(&usart->dma_tx_channel, false);
    dma_stream_number_of_data(&usart->dma_tx_channel, size);
    dma_stream_peripheral_address(&usart->dma_tx_channel, (uint32_t) & usart->usart_device->DR);
    dma_stream_memory_address(&usart->dma_tx_channel, 0, (uint32_t) address); //M0AR

    dma_stream_channel_selection(&usart->dma_tx_channel, usart->dma_tx_channel_n);
    dma_stream_transfer_error_interrupt_enable(&usart->dma_tx_channel, true);
    dma_stream_transfer_complete_interrupt_enable(&usart->dma_tx_channel, true);
    dma_stream_data_transfer_direction(&usart->dma_tx_channel, (uint8_t) 0b01); //Memory-to-peripheral
    dma_stream_memory_increment_mode(&usart->dma_tx_channel, true);
}

static void usart_bus_dma_rx_config(usart_bus_t* usart, void* address, size_t size) {
    dma_stream_enable(&usart->dma_rx_channel, false);
    dma_stream_number_of_data(&usart->dma_rx_channel, size);
    dma_stream_peripheral_address(&usart->dma_rx_channel, (uint32_t) & usart->usart_device->DR);
    dma_stream_memory_address(&usart->dma_rx_channel, 0, (uint32_t) address); //M0AR

    dma_stream_channel_selection(&usart->dma_rx_channel, usart->dma_rx_channel_n);
    dma_stream_transfer_error_interrupt_enable(&usart->dma_rx_channel, true);
    dma_stream_transfer_complete_interrupt_enable(&usart->dma_rx_channel, true);
    dma_stream_data_transfer_direction(&usart->dma_rx_channel, (uint8_t) 0b00); //Peripheral-to-memory
    dma_stream_memory_increment_mode(&usart->dma_rx_channel, true);
}

static void usart_bus_dma_start_tx(usart_bus_t* usart) {
    usart_cr3_set(usart->usart_device, USART_CR3_DMAT, ENABLE);
    dma_stream_enable(&usart->dma_tx_channel, true);
}

static void usart_bus_dma_start_rx(usart_bus_t* usart) {
    usart_cr3_set(usart->usart_device, USART_CR3_DMAR, ENABLE);
    dma_stream_enable(&usart->dma_rx_channel, true);
}

//static void usart_bus_dma_stop_tx(usart_bus_t* usart) {
//    dma_stream_enable(&usart->dma_tx_channel, false);
//    usart->usart_device->CR3 &= ~USART_CR3_DMAT;
//}

static void usart_bus_dma_stop_tx(usart_bus_t* usart) {
    dma_stream_transfer_complete_interrupt_enable(&usart->dma_tx_channel, false);

    dma_stream_enable(&usart->dma_tx_channel, false);
    usart_cr3_set(usart->usart_device, USART_CR3_DMAT, DISABLE);

    dma_stream_transfer_complete_interrupt_clear(&usart->dma_tx_channel);
}

//static void usart_bus_dma_stop_rx(usart_bus_t* usart) {
//    dma_stream_enable(&usart->dma_rx_channel, false);
//    usart->usart_device->CR3 &= ~USART_CR3_DMAR;
//}

static void usart_bus_dma_stop_rx(usart_bus_t* usart) {
    dma_stream_transfer_complete_interrupt_enable(&usart->dma_rx_channel, false);

    dma_stream_enable(&usart->dma_rx_channel, false);
    usart_cr3_set(usart->usart_device, USART_CR3_DMAR, DISABLE);

    dma_stream_transfer_complete_interrupt_clear(&usart->dma_rx_channel);
}

ALWAYS_INLINE static bool usart_bus_has_rx_errors(uint16_t SR) {
    return (SR & (USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE)) != 0;
}

static usart_errors_t usart_bus_get_rx_errors(uint16_t SR) {
    usart_errors_t errs = USART_ERROR_NONE;

    if (SR & USART_SR_ORE) errs |= USART_ERROR_OVERRUN;
    if (SR & USART_SR_NE) errs |= USART_ERROR_NOISE;
    if (SR & USART_SR_FE) errs |= USART_ERROR_FRAMING;
    if (SR & USART_SR_PE) errs |= USART_ERROR_PARITY;

    return errs;
}

static void usart_bus_dma_rx_done(usart_bus_t* usart) {
    usart_bus_dma_stop_rx(usart);


    usart->rx_size -= dma_stream_number_of_data_read(&usart->dma_rx_channel);

    usart_bus_dma_unlock_rx_channel(usart);

    usart_cr1_set(usart->usart_device, USART_CR1_RXNEIE, ENABLE);
}

static void usart_bus_dma_rx_error(usart_bus_t* usart) {
    usart_bus_dma_stop_rx(usart);

    usart->rx_errors |= USART_ERROR_DMA;
    usart->rx_size -= dma_stream_number_of_data_read(&usart->dma_rx_channel);

    usart_bus_dma_unlock_rx_channel(usart);

    usart_cr1_set(usart->usart_device, USART_CR1_RXNEIE, ENABLE);
}

static void usart_bus_dma_tx_done(usart_bus_t* usart) {
    usart_bus_dma_stop_tx(usart);

    usart->tx_size -= dma_stream_number_of_data_read(&usart->dma_tx_channel);

    usart_bus_dma_unlock_tx_channel(usart);
}

static void usart_bus_dma_tx_error(usart_bus_t* usart) {
    usart_bus_dma_stop_tx(usart);

    usart->tx_errors |= USART_ERROR_DMA;
    usart->tx_size -= dma_stream_number_of_data_read(&usart->dma_tx_channel);

    usart_bus_dma_unlock_tx_channel(usart);
}

static void usart_bus_rx_done(usart_bus_t* usart) {
    if (usart->rx_errors == USART_ERROR_NONE) {
        usart->rx_status = USART_STATUS_TRANSFERED;
    } else {
        usart->rx_status = USART_STATUS_ERROR;
    }

    if (usart->rx_callback) usart->rx_callback();
}

static void usart_bus_tx_done(usart_bus_t* usart) {
    if (usart->tx_errors == USART_ERROR_NONE) {
        usart->tx_status = USART_STATUS_TRANSFERED;
    } else {
        usart->tx_status = USART_STATUS_ERROR;
    }

    if (usart->tx_callback) usart->tx_callback();
}

/*
 * Основной функционал.
 */

err_t usart_bus_init(usart_bus_t* usart) {
    // Адрес периферии не может быть нулём.
    if (usart->usart_device == NULL) return E_NULL_POINTER;

    usart->rx_byte_callback = NULL;
    usart->rx_callback = NULL;
    usart->tx_callback = NULL;
    usart->tc_callback = NULL;
    usart->dma_rx_locked = false;
    usart->dma_tx_locked = false;
    usart->rx_status = USART_STATUS_IDLE;
    usart->tx_status = USART_STATUS_IDLE;
    usart->rx_errors = USART_ERROR_NONE;
    usart->tx_errors = USART_ERROR_NONE;

    usart->rx_transfer_id = USART_BUS_DEFAULT_TRANSFER_ID;
    usart->tx_transfer_id = USART_BUS_DEFAULT_TRANSFER_ID;

    usart->idle_mode = USART_IDLE_MODE_NONE;

    // Режим выхода из сна - свободная шина.
    usart_cr1_set(usart->usart_device, USART_CR1_WAKE, DISABLE);

    // Если включен приёмник - разрешим прерывания приёма и ошибок.
    if (usart_bus_receiver_enabled(usart)) {

        usart_cr1_set(usart->usart_device, USART_CR1_RXNEIE, ENABLE);

        usart_cr1_set(usart->usart_device, USART_CR1_PEIE, ENABLE);

        usart_cr3_set(usart->usart_device, USART_CR3_EIE, ENABLE);
    }

    return E_NO_ERROR;
}

void usart_bus_irq_handler(usart_bus_t* usart) {
    uint16_t SR = usart->usart_device->SR;
    uint8_t byte = usart->usart_device->DR;

    // Если есть ошибки.
    if (usart_bus_has_rx_errors(SR)) {
        usart->rx_errors = usart_bus_get_rx_errors(SR);

        if (usart->rx_status == USART_STATUS_TRANSFERING) {
            
            usart_bus_dma_rx_done(usart);

            usart_bus_rx_done(usart);
        }
    } else {
        usart->rx_errors = USART_ERROR_NONE;
    }

    // Получен IDLE при приёме данных.
    if (SR & USART_SR_IDLE) {
        if (usart->idle_mode == USART_IDLE_MODE_END_RX &&
                usart->rx_status == USART_STATUS_TRANSFERING) {

            usart_bus_dma_rx_done(usart);

            usart_bus_rx_done(usart);
        }
    }

    // Получен байт.
    if (SR & USART_SR_RXNE) {
        if (usart->rx_byte_callback) usart->rx_byte_callback(byte);
    }

    // Завершена передача.
    if (SR & USART_SR_TC) {
        usart_sr_clr(usart->usart_device, USART_SR_TC);
        if (usart->tc_callback) usart->tc_callback();
    }
}

bool usart_bus_dma_rx_channel_irq_handler(usart_bus_t* usart) {
    if (usart->rx_status != USART_STATUS_TRANSFERING) return false;

    //bool can_rx = usart_bus_transmitter_state(usart->usart_device);

    //if (!can_rx || !usart->dma_rx_locked) return false;

    if (dma_stream_transfer_complete_interrupt_read(&usart->dma_rx_channel)) {

        dma_stream_transfer_complete_interrupt_clear(&usart->dma_rx_channel);

        usart_bus_dma_rx_done(usart);

        usart_bus_rx_done(usart);

    } else if (dma_stream_transfer_error_interrupt_read(&usart->dma_rx_channel)) {

        dma_stream_transfer_error_interrupt_clear(&usart->dma_rx_channel);

        usart_bus_dma_rx_error(usart);

        usart_bus_rx_done(usart);
    }

    return true;
}

bool usart_bus_dma_tx_channel_irq_handler(usart_bus_t* usart) {
    if (usart->tx_status != USART_STATUS_TRANSFERING) return false;

    //bool can_tx = usart_bus_transmitter_state(usart->usart_device);

    //if (!can_tx || !usart->dma_tx_locked) return false;

    if (dma_stream_transfer_complete_interrupt_read(&usart->dma_tx_channel)) {

        dma_stream_transfer_complete_interrupt_clear(&usart->dma_tx_channel);

        usart_bus_dma_tx_done(usart);

        usart_bus_tx_done(usart);

    } else if (dma_stream_transfer_error_interrupt_read(&usart->dma_tx_channel)) {

        dma_stream_transfer_error_interrupt_clear(&usart->dma_tx_channel);

        usart_bus_dma_tx_error(usart);

        usart_bus_tx_done(usart);
    }

    return true;
}

bool usart_bus_rx_busy(usart_bus_t* usart) {
    return usart->rx_status == USART_STATUS_TRANSFERING;
}

bool usart_bus_tx_busy(usart_bus_t* usart) {
    return usart->tx_status == USART_STATUS_TRANSFERING;
}

void usart_bus_rx_wait(usart_bus_t* usart) {
    WAIT_WHILE_TRUE(usart_bus_rx_busy(usart));
}

void usart_bus_tx_wait(usart_bus_t* usart) {
    WAIT_WHILE_TRUE(usart_bus_tx_busy(usart));
}

usart_transfer_id_t usart_bus_rx_transfer_id(usart_bus_t* usart) {
    return usart->rx_transfer_id;
}

bool usart_bus_set_rx_transfer_id(usart_bus_t* usart, usart_transfer_id_t id) {
    if (usart_bus_rx_busy(usart)) return false;
    usart->rx_transfer_id = id;
    return true;
}

usart_transfer_id_t usart_bus_tx_transfer_id(usart_bus_t* usart) {
    return usart->tx_transfer_id;
}

bool usart_bus_set_tx_transfer_id(usart_bus_t* usart, usart_transfer_id_t id) {
    if (usart_bus_tx_busy(usart)) return false;
    usart->tx_transfer_id = id;
    return true;
}

usart_bus_rx_byte_callback_t usart_bus_rx_byte_callback(usart_bus_t* usart) {
    return usart->rx_byte_callback;
}

void usart_bus_set_rx_byte_callback(usart_bus_t* usart, usart_bus_rx_byte_callback_t callback) {
    usart->rx_byte_callback = callback;
}

usart_bus_callback_t usart_bus_rx_callback(usart_bus_t* usart) {
    return usart->rx_callback;
}

void usart_bus_set_rx_callback(usart_bus_t* usart, usart_bus_callback_t callback) {
    usart->rx_callback = callback;
}

usart_bus_callback_t usart_bus_tx_callback(usart_bus_t* usart) {
    return usart->tx_callback;
}

void usart_bus_set_tx_callback(usart_bus_t* usart, usart_bus_callback_t callback) {
    usart->tx_callback = callback;
}

usart_bus_callback_t usart_bus_tc_callback(usart_bus_t* usart) {
    return usart->tc_callback;
}

void usart_bus_set_tc_callback(usart_bus_t* usart, usart_bus_callback_t callback) {
    if (callback) {
        usart->tc_callback = callback;
        usart_cr1_set(usart->usart_device, USART_CR1_TCIE, ENABLE);
    } else {
    	usart_cr1_set(usart->usart_device, USART_CR1_TCIE, DISABLE);
        usart->tc_callback = callback;
    }
}

usart_status_t usart_bus_rx_status(usart_bus_t* usart) {
    return usart->rx_status;
}

usart_status_t usart_bus_tx_status(usart_bus_t* usart) {
    return usart->tx_status;
}

usart_errors_t usart_bus_rx_errors(usart_bus_t* usart) {
    return usart->rx_errors;
}

usart_errors_t usart_bus_tx_errors(usart_bus_t* usart) {
    return usart->tx_errors;
}

size_t usart_bus_bytes_received(usart_bus_t* usart) {
    return usart->rx_size;
}

size_t usart_bus_bytes_transmitted(usart_bus_t* usart) {
    return usart->tx_size;
}

usart_idle_mode_t usart_bus_idle_mode(usart_bus_t* usart) {
    return usart->idle_mode;
}

void usart_bus_set_idle_mode(usart_bus_t* usart, usart_idle_mode_t mode) {
    usart->idle_mode = mode;
    if (mode == USART_IDLE_MODE_NONE) {
        usart_cr1_set(usart->usart_device, USART_CR1_IDLEIE, DISABLE);
    } else {
    	usart_cr1_set(usart->usart_device, USART_CR1_IDLEIE, ENABLE);
    }
}

void usart_bus_sleep(usart_bus_t* usart) {
    usart_cr1_set(usart->usart_device, USART_CR1_RWU, ENABLE);
}

void usart_bus_wake(usart_bus_t* usart) {
    usart_cr1_set(usart->usart_device, USART_CR1_RWU, DISABLE);
}

err_t usart_bus_send(usart_bus_t* usart, const void* data, size_t size) {
    if (usart_bus_tx_busy(usart)) return E_BUSY;
    if (data == NULL) return E_NULL_POINTER;
    if (size == 0) return E_INVALID_VALUE;
    if (size > UINT16_MAX) return E_OUT_OF_RANGE;

    //if (!usart_bus_dma_lock_tx_channel(usart)) return E_BUSY;

    usart->tx_errors = USART_ERROR_NONE;
    usart->tx_status = USART_STATUS_TRANSFERING;
    usart->tx_size = size;

    usart_bus_dma_tx_config(usart, (void*) data, size);

    usart_bus_dma_start_tx(usart);

    return E_NO_ERROR;
}

err_t usart_bus_recv(usart_bus_t* usart, void* data, size_t size) {
    if (usart_bus_rx_busy(usart)) return E_BUSY;
    if (data == NULL) return E_NULL_POINTER;
    if (size == 0) return E_INVALID_VALUE;
    if (size > UINT16_MAX) return E_OUT_OF_RANGE;

    //if (!usart_bus_dma_lock_rx_channel(usart)) return E_BUSY;
    if (!usart_bus_receiver_enabled(usart)) return E_STATE;

    usart_cr1_set(usart->usart_device, USART_CR1_RXNEIE, DISABLE);

    usart->rx_errors = USART_ERROR_NONE;
    usart->rx_status = USART_STATUS_TRANSFERING;
    usart->rx_size = size;
    
    usart_bus_dma_rx_config(usart, data, size);

    usart_bus_dma_start_rx(usart);

    return E_NO_ERROR;
}

