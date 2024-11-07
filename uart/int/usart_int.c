#include "usart_int.h"
#include "uart/usart.h"
#include "utils/utils.h"

//UART RE
bool usart_int_receiver_enabled(usart_int_t* usart) {
	return usart_receiver_state(usart->usart_device);
}

void usart_int_receiver_enable(usart_int_t* usart) {
	usart_cr1_set(usart->usart_device, USART_CR1_RE, ENABLE);
	usart_cr1_set(usart->usart_device, USART_CR1_UE, ENABLE);
}

void usart_int_receiver_disable(usart_int_t* usart) {
	usart_cr1_set(usart->usart_device, USART_CR1_RE, DISABLE);
}

//UART TE
bool usart_int_transmitter_enabled(usart_int_t* usart) {
	return usart_transmitter_state(usart->usart_device);
}

void usart_int_transmitter_enable(usart_int_t* usart) {
	usart_cr1_set(usart->usart_device, USART_CR1_TE, ENABLE);
	usart_cr1_set(usart->usart_device, USART_CR1_UE, ENABLE);
}

void usart_int_transmitter_disable(usart_int_t* usart) {
	usart_cr1_set(usart->usart_device, USART_CR1_TE, DISABLE);
}

__attribute__((noinline))
void usart_int_read_dr(USART_TypeDef* usart) {
	__attribute__((used)) static volatile uint8_t byte = 0;

	byte = usart->DR;
}

ALWAYS_INLINE static bool usart_int_has_rx_errors(uint16_t SR) {
    return (SR & (USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE)) != 0;
}

static usart_errors_t usart_int_get_rx_errors(uint16_t SR) {
    usart_errors_t errs = USART_INT_ERROR_NONE;

    if (SR & USART_SR_ORE) errs |= USART_INT_ERROR_OVERRUN;
    if (SR & USART_SR_NE) errs |= USART_INT_ERROR_NOISE;
    if (SR & USART_SR_FE) errs |= USART_INT_ERROR_FRAMING;
    if (SR & USART_SR_PE) errs |= USART_INT_ERROR_PARITY;

    return errs;
}

static void usart_int_rx_done(usart_int_t* usart) {
    if (usart->rx_errors == USART_INT_ERROR_NONE) {
        usart->rx_status = USART_INT_STATUS_TRANSFERED;
    } else {
        usart->rx_status = USART_INT_STATUS_ERROR;
    }

    if (usart->rx_callback) usart->rx_callback();
}

static void usart_int_tx_done(usart_int_t* usart) {
    if (usart->tx_errors == USART_INT_ERROR_NONE) {
        usart->tx_status = USART_INT_STATUS_TRANSFERED;
    } else {
        usart->tx_status = USART_INT_STATUS_ERROR;
    }

    if (usart->tx_callback) usart->tx_callback();
}

/*
 * Основной функционал.
 */

err_t usart_int_init(usart_int_t* usart) {
    // Адрес периферии не может быть нулём.
    if (usart->usart_device == NULL) return E_NULL_POINTER;

    usart->rx_byte_callback = NULL;
    usart->rx_callback = NULL;
    usart->tx_callback = NULL;
    usart->tc_callback = NULL;
    usart->rx_status = USART_INT_STATUS_IDLE;
    usart->tx_status = USART_INT_STATUS_IDLE;
    usart->rx_errors = USART_INT_ERROR_NONE;
    usart->tx_errors = USART_INT_ERROR_NONE;

    buffer_init(&usart->rx_buf, NULL, 0);
    buffer_init(&usart->tx_buf, NULL, 0);

    usart->rx_transfer_id = USART_INT_DEFAULT_TRANSFER_ID;
    usart->tx_transfer_id = USART_INT_DEFAULT_TRANSFER_ID;

    usart->idle_mode = USART_INT_IDLE_MODE_NONE;

    // Режим выхода из сна - свободная шина.
    //usart->usart_device->CR1 &= ~USART_CR1_WAKE;
    usart_cr1_set(usart->usart_device, USART_CR1_WAKE, DISABLE);
    //USART_WakeUpConfig(usart->usart_device, USART_WakeUp_IdleLine);

    // Если включен приёмник - разрешим прерывания приёма и ошибок.
    if (usart_receiver_state(usart->usart_device)) {

        //usart->usart_device->CR1 |= USART_CR1_RXNEIE;
        usart_cr1_set(usart->usart_device, USART_CR1_RXNEIE, ENABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_RXNE, ENABLE);

        //usart->usart_device->CR1 |= USART_CR1_PEIE;
        usart_cr1_set(usart->usart_device, USART_CR1_PEIE, ENABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_PE, ENABLE);

        //usart->usart_device->CR3 |= USART_CR3_EIE;
        usart_cr3_set(usart->usart_device, USART_CR3_EIE, ENABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_NE, ENABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_ORE, ENABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_FE, ENABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_ERR, ENABLE);
    }

    return E_NO_ERROR;
}

void usart_int_irq_handler(usart_int_t* usart) {
    uint16_t SR = usart->usart_device->SR;
    uint8_t byte = usart->usart_device->DR;

    // Если есть ошибки.
    if (usart_int_has_rx_errors(SR)) {
        usart->rx_errors = usart_int_get_rx_errors(SR);

        if (usart->rx_status == USART_INT_STATUS_TRANSFERING) {

            usart_int_rx_done(usart);
        }
    } else {
        usart->rx_errors = USART_INT_ERROR_NONE;
    }

    // Получен IDLE при приёме данных.
    if (SR & USART_SR_IDLE) {
        if (usart->idle_mode == USART_INT_IDLE_MODE_END_RX &&
                usart->rx_status == USART_INT_STATUS_TRANSFERING) {

            usart_int_rx_done(usart);
        }
    }

    // Получен байт.
    if (SR & USART_SR_RXNE) {
    	if(usart->rx_status == USART_INT_STATUS_TRANSFERING && buffer_valid(&usart->rx_buf)){
			buffer_set_next(&usart->rx_buf, byte);
			if(buffer_at_end(&usart->rx_buf)){
				usart_int_rx_done(usart);
			}
    	}else{
    		if (usart->rx_byte_callback) usart->rx_byte_callback(byte);
    	}
    }

    // Можно отправить байт.
    if (SR & USART_SR_TXE) {
    	if(usart->tx_status == USART_INT_STATUS_TRANSFERING && buffer_valid(&usart->tx_buf)){
			if(buffer_at_end(&usart->tx_buf)){
				usart_cr1_set(usart->usart_device, USART_CR1_TXEIE, DISABLE);

				usart_int_tx_done(usart);
			}else{
				usart->usart_device->DR = buffer_get_next(&usart->tx_buf);
			}
    	}else{
    		usart_cr1_set(usart->usart_device, USART_CR1_TXEIE, DISABLE);
    	}
    }

    // Завершена передача.
    if (SR & USART_SR_TC) {
        //usart->usart_device->SR &= ~USART_SR_TC;
        usart_sr_clr(usart->usart_device, USART_SR_TC);
        if (usart->tc_callback) usart->tc_callback();
    }
}



bool usart_int_rx_busy(usart_int_t* usart) {
    return usart->rx_status == USART_INT_STATUS_TRANSFERING;
}

bool usart_int_tx_busy(usart_int_t* usart) {
    return usart->tx_status == USART_INT_STATUS_TRANSFERING;
}

void usart_int_rx_wait(usart_int_t* usart) {
    WAIT_WHILE_TRUE(usart_int_rx_busy(usart));
}

void usart_int_tx_wait(usart_int_t* usart) {
    WAIT_WHILE_TRUE(usart_int_tx_busy(usart));
}

usart_transfer_id_t usart_int_rx_transfer_id(usart_int_t* usart) {
    return usart->rx_transfer_id;
}

bool usart_int_set_rx_transfer_id(usart_int_t* usart, usart_transfer_id_t id) {
    if (usart_int_rx_busy(usart)) return false;
    usart->rx_transfer_id = id;
    return true;
}

usart_transfer_id_t usart_int_tx_transfer_id(usart_int_t* usart) {
    return usart->tx_transfer_id;
}

bool usart_int_set_tx_transfer_id(usart_int_t* usart, usart_transfer_id_t id) {
    if (usart_int_tx_busy(usart)) return false;
    usart->tx_transfer_id = id;
    return true;
}

usart_int_rx_byte_callback_t usart_int_rx_byte_callback(usart_int_t* usart) {
    return usart->rx_byte_callback;
}

void usart_int_set_rx_byte_callback(usart_int_t* usart, usart_int_rx_byte_callback_t callback) {
    usart->rx_byte_callback = callback;
}

usart_int_callback_t usart_int_rx_callback(usart_int_t* usart) {
    return usart->rx_callback;
}

void usart_int_set_rx_callback(usart_int_t* usart, usart_int_callback_t callback) {
    usart->rx_callback = callback;
}

usart_int_callback_t usart_int_tx_callback(usart_int_t* usart) {
    return usart->tx_callback;
}

void usart_int_set_tx_callback(usart_int_t* usart, usart_int_callback_t callback) {
    usart->tx_callback = callback;
}

usart_int_callback_t usart_int_tc_callback(usart_int_t* usart) {
    return usart->tc_callback;
}

void usart_int_set_tc_callback(usart_int_t* usart, usart_int_callback_t callback) {
    if (callback) {
        usart->tc_callback = callback;
        //usart->usart_device->CR1 |= USART_CR1_TCIE;
        usart_cr1_set(usart->usart_device, USART_CR1_TCIE, ENABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_TC, ENABLE);
    } else {
        //usart->usart_device->CR1 &= ~USART_CR1_TCIE;
    	usart_cr1_set(usart->usart_device, USART_CR1_TCIE, DISABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_TC, DISABLE);
        usart->tc_callback = callback;
    }
}

usart_int_status_t usart_int_rx_status(usart_int_t* usart) {
    return usart->rx_status;
}

usart_int_status_t usart_int_tx_status(usart_int_t* usart) {
    return usart->tx_status;
}

usart_errors_t usart_int_rx_errors(usart_int_t* usart) {
    return usart->rx_errors;
}

usart_errors_t usart_int_tx_errors(usart_int_t* usart) {
    return usart->tx_errors;
}

size_t usart_int_bytes_received(usart_int_t* usart) {
    return usart->rx_buf.pos;
}

size_t usart_int_bytes_transmitted(usart_int_t* usart) {
    return usart->tx_buf.pos;
}

usart_int_idle_mode_t usart_int_idle_mode(usart_int_t* usart) {
    return usart->idle_mode;
}

void usart_int_set_idle_mode(usart_int_t* usart, usart_int_idle_mode_t mode) {
    usart->idle_mode = mode;
    if (mode == USART_INT_IDLE_MODE_NONE) {
        //usart->usart_device->CR1 &= ~USART_CR1_IDLEIE;
        usart_cr1_set(usart->usart_device, USART_CR1_IDLEIE, DISABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_IDLE, DISABLE);
    } else {
        //usart->usart_device->CR1 |= USART_CR1_IDLEIE;
    	usart_cr1_set(usart->usart_device, USART_CR1_IDLEIE, ENABLE);
        //USART_ITConfig(usart->usart_device, USART_IT_IDLE, ENABLE);
    }
}

void usart_int_sleep(usart_int_t* usart) {
    //usart->usart_device->CR1 |= USART_CR1_RWU;
    usart_cr1_set(usart->usart_device, USART_CR1_RWU, ENABLE);
    //USART_ReceiverWakeUpCmd(usart->usart_device, ENABLE);
}

void usart_int_wake(usart_int_t* usart) {
    //usart->usart_device->CR1 &= ~USART_CR1_RWU;
    usart_cr1_set(usart->usart_device, USART_CR1_RWU, DISABLE);
    //USART_ReceiverWakeUpCmd(usart->usart_device, DISABLE);
}

err_t usart_int_send(usart_int_t* usart, const void* data, size_t size) {
    if (usart_int_tx_busy(usart)) return E_BUSY;
    if (data == NULL) return E_NULL_POINTER;
    if (size == 0) return E_INVALID_VALUE;
    if (size > UINT16_MAX) return E_OUT_OF_RANGE;

    if (!usart_transmitter_state(usart->usart_device)) return E_STATE;

    usart->tx_errors = USART_INT_ERROR_NONE;
    usart->tx_status = USART_INT_STATUS_TRANSFERING;

    buffer_init(&usart->tx_buf, (uint8_t*)data, size);

    usart_cr1_set(usart->usart_device, USART_CR1_TXEIE, ENABLE);

    return E_NO_ERROR;
}

err_t usart_int_recv(usart_int_t* usart, void* data, size_t size) {
    if (usart_int_rx_busy(usart)) return E_BUSY;
    if (data == NULL) return E_NULL_POINTER;
    if (size == 0) return E_INVALID_VALUE;
    if (size > UINT16_MAX) return E_OUT_OF_RANGE;

    if (!usart_receiver_state(usart->usart_device)) return E_STATE;

    usart->rx_errors = USART_INT_ERROR_NONE;
    usart->rx_status = USART_INT_STATUS_TRANSFERING;
    
    buffer_init(&usart->rx_buf, (uint8_t*)data, size);

    usart_int_read_dr(usart->usart_device);

    usart_cr1_set(usart->usart_device, USART_CR1_RXNEIE, ENABLE);

    return E_NO_ERROR;
}

