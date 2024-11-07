/**
 * @file usart_int.h
 * Библиотека для работы с шиной USART.
 */

#ifndef USART_INT_H
#define	USART_INT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "stm32f4xx/stm32f4xx.h"
#include "errors/errors.h"
#include "defs/defs.h"
#include "buffer/buffer.h"

//! Состояние шины USART.
typedef enum _Usart_Int__Status {
    USART_INT_STATUS_IDLE = 0,//!< Бездействие.
    USART_INT_STATUS_TRANSFERING,//!< Обмен данными.
    USART_INT_STATUS_TRANSFERED,//!< Обмен данными завершён.
    USART_INT_STATUS_ERROR//!< Ошибка.
} usart_int_status_t;

//! Ошибки шины USART.
typedef enum _Usart_Int_Error {
    USART_INT_ERROR_NONE    = 0, //!< Нет ошибки.
    USART_INT_ERROR_PARITY  = 1, //!< Ошибка чётности.
    USART_INT_ERROR_NOISE   = 2, //!< Шум.
    USART_INT_ERROR_OVERRUN = 4, //!< Переполнение.
    USART_INT_ERROR_FRAMING = 8, //!< Ошибка кадра.
    USART_INT_ERROR_DMA     = 16//!< Ошибка DMA.
} usart_int_error_t;

//! Тип ошибок USART.
typedef uint8_t usart_errors_t;

//! Тип реакции на IDLE при приёме данных.
typedef enum _Usart_Idle_Mode {
    USART_INT_IDLE_MODE_NONE = 0, //!< Ничего не делать.
    USART_INT_IDLE_MODE_END_RX //!< Завершить приём данных.
} usart_int_idle_mode_t;

/**
 * Тип функции обратного вызова.
 * Вызывается при окончании приёма/передачи
 * даннх, или при возникновении ошибки.
 * @return true, если событие обработано, иначе false.
 */
typedef bool (*usart_int_callback_t)(void);

/**
 * Тип функции обратного вызова приёма байта.
 * Вызывается при приёме байта.
 * @return true, если событие обработано, иначе false.
 */
typedef bool (*usart_int_rx_byte_callback_t)(uint8_t byte);

/**
 * Тип идентификатора приёма/передачи.
 */
typedef uint8_t usart_transfer_id_t;

//! Идентификатор передачи по умолчанию.
#define USART_INT_DEFAULT_TRANSFER_ID 0

/**
 * Структура буферизации USART.
 */
typedef struct _UsartInt {
    USART_TypeDef* usart_device; //!< Устройство USART.
    usart_int_rx_byte_callback_t rx_byte_callback; //!< Каллбэк при приёме байта.
    usart_int_callback_t rx_callback; //!< Каллбэк событий приёма данных шины USART.
    usart_int_callback_t tx_callback; //!< Каллбэк событий передачи данных шины USART.
    usart_int_callback_t tc_callback; //!< Каллбэк события окончания передачи данных шины USART.
    usart_transfer_id_t rx_transfer_id;//!< Идентификатор приёма.
    usart_transfer_id_t tx_transfer_id;//!< Идентификатор передачи.
    usart_int_status_t rx_status; //!< Состояние канала приёма.
    usart_int_status_t tx_status; //!< Состояние канала передачи.
    usart_errors_t rx_errors; //!< Ошибки канала приёма.
    usart_errors_t tx_errors; //!< Ошибки канала передачи.
    buffer_t rx_buf; //!< Буфер для приёма.
    buffer_t tx_buf; //!< Буфер для передачи.
    usart_int_idle_mode_t idle_mode; //!< Режим реакции на IDLE при приёме.
} usart_int_t;

/**
 * Получает флаг разрешённости передатчика USART.
 * @param usart Устройство USART.
 * @return Флаг разрешённости передатчика USART.
 */
EXTERN bool usart_int_transmitter_state(USART_TypeDef* usart);

/**
 * Получает флаг разрешённости приёмника USART.
 * @param usart Устройство USART.
 * @return Флаг разрешённости приёмника USART.
 */
EXTERN bool usart_int_receiver_state(USART_TypeDef* usart);

/**
 * Получает флаг полудуплексного режима USART.
 * @param usart Устройство USART.
 * @return Флаг полудуплексного режима USART.
 */
EXTERN bool usart_int_halfduplex_state(USART_TypeDef* usart);

/**
 * Инициализирует шину USART.
 * Разрешает необходимые прерывания USART.
 * @param usart Шина USART.
 * @param usart_int_init Структура инициализации USART.
 * @return Код ошибки.
 */
EXTERN err_t usart_int_init(usart_int_t* usart);

/**
 * Функция для обработки прерывания USART.
 * Должна вызываться из обработчика прерывания USART.
 * @param usart Шина USART.
 */
EXTERN void usart_int_irq_handler(usart_int_t* usart);

/**
 * Получает флаг включения приёмника.
 * @param usart Шина USART.
 * @return true, если приёмник включен, иначе false.
 */
EXTERN bool usart_int_receiver_enabled(usart_int_t* usart);

/**
 * Включает приёмник.
 * @param usart Шина USART.
 */
EXTERN void usart_int_receiver_enable(usart_int_t* usart);

/**
 * Выключает приёмник.
 * @param usart Шина USART.
 */
EXTERN void usart_int_receiver_disable(usart_int_t* usart);

/**
 * Получает флаг включения передатчика.
 * @param usart Шина USART.
 * @return true, если передатчик включен, иначе false.
 */
EXTERN bool usart_int_transmitter_enabled(usart_int_t* usart);

/**
 * Включает передатчик.
 * @param usart Шина USART.
 */
EXTERN void usart_int_transmitter_enable(usart_int_t* usart);

/**
 * Выключает передатчик.
 * @param usart Шина USART.
 */
EXTERN void usart_int_transmitter_disable(usart_int_t* usart);

/**
 * Получает флаг занятости шины usart на приём.
 * @param usart Шина usart.
 * @return Флаг занятости шины usart.
 */
EXTERN bool usart_int_rx_busy(usart_int_t* usart);

/**
 * Получает флаг занятости шины usart на передачу.
 * @param usart Шина usart.
 * @return Флаг занятости шины usart.
 */
EXTERN bool usart_int_tx_busy(usart_int_t* usart);

/**
 * Ждёт завершения текущей операции usart на приём.
 * @param usart Шина USART.
 */
EXTERN void usart_int_rx_wait(usart_int_t* usart);

/**
 * Ждёт завершения текущей операции usart на передачу.
 * @param usart Шина USART.
 */
EXTERN void usart_int_tx_wait(usart_int_t* usart);

/**
 * Получает идентификатор приёма.
 * @param usart Шина USART.
 * @return Идентификатор передачи.
 */
EXTERN usart_transfer_id_t usart_int_rx_transfer_id(usart_int_t* usart);

/**
 * Устанавливает идентификатор приёма.
 * @param usart Шина USART.
 * @param id Идентификатор передачи.
 * @return true, если идентификатор передачи установлен, иначе false (шина занята).
 */
EXTERN bool usart_int_set_rx_transfer_id(usart_int_t* usart, usart_transfer_id_t id);

/**
 * Получает идентификатор передачи.
 * @param usart Шина USART.
 * @return Идентификатор передачи.
 */
EXTERN usart_transfer_id_t usart_int_tx_transfer_id(usart_int_t* usart);

/**
 * Устанавливает идентификатор передачи.
 * @param usart Шина USART.
 * @param id Идентификатор передачи.
 * @return true, если идентификатор передачи установлен, иначе false (шина занята).
 */
EXTERN bool usart_int_set_tx_transfer_id(usart_int_t* usart, usart_transfer_id_t id);

/**
 * Получает каллбэк приёма байта шины USART.
 * @param usart Шина USART.
 * @return Каллбэк приёма байта.
 */
EXTERN usart_int_rx_byte_callback_t usart_int_rx_byte_callback(usart_int_t* usart);

/**
 * Устанавливает каллбэк приёма байта шины USART.
 * @param usart Шина USART.
 * @param callback Каллбэк приёма байта.
 */
EXTERN void usart_int_set_rx_byte_callback(usart_int_t* usart, usart_int_rx_byte_callback_t callback);

/**
 * Получает каллбэк событий приёма данных шины USART.
 * @param usart Шина USART.
 * @return Каллбэк приёма байта.
 */
EXTERN usart_int_callback_t usart_int_rx_callback(usart_int_t* usart);

/**
 * Устанавливает каллбэк событий приёма данных шины USART.
 * @param usart Шина USART.
 * @param callback Каллбэк приёма байта.
 */
EXTERN void usart_int_set_rx_callback(usart_int_t* usart, usart_int_callback_t callback);

/**
 * Получает каллбэк событий передачи данных шины USART.
 * @param usart Шина USART.
 * @return Каллбэк.
 */
EXTERN usart_int_callback_t usart_int_tx_callback(usart_int_t* usart);

/**
 * Устанавливает каллбэк событий передачи данных шины USART.
 * @param usart Шина USART.
 * @param callback Каллбэк.
 */
EXTERN void usart_int_set_tx_callback(usart_int_t* usart, usart_int_callback_t callback);

/**
 * Получает каллбэк события окончания передачи данных шины USART.
 * @param usart Шина USART.
 * @return Каллбэк.
 */
EXTERN usart_int_callback_t usart_int_tc_callback(usart_int_t* usart);

/**
 * Устанавливает каллбэк события окончания передачи данных шины USART.
 * @param usart Шина USART.
 * @param callback Каллбэк.
 */
EXTERN void usart_int_set_tc_callback(usart_int_t* usart, usart_int_callback_t callback);

/**
 * Получает состояние канала приёма шины USART.
 * @param usart Шина USART.
 * @return Состояние канала приёма.
 */
EXTERN usart_int_status_t usart_int_rx_status(usart_int_t* usart);

/**
 * Получает состояние канала передачи шины USART.
 * @param usart Шина USART.
 * @return Состояние канала передачи.
 */
EXTERN usart_int_status_t usart_int_tx_status(usart_int_t* usart);

/**
 * Получает ошибки канала приёма шины USART.
 * @param usart Шина USART.
 * @return Ошибки канала приёма.
 */
EXTERN usart_errors_t usart_int_rx_errors(usart_int_t* usart);

/**
 * Получает ошибки канала передачи шины USART.
 * @param usart Шина USART.
 * @return Ошибки канала передачи.
 */
EXTERN usart_errors_t usart_int_tx_errors(usart_int_t* usart);

/**
 * Получает число полученных
 * после завершения приёма байт данных.
 * @param usart Шина USART.
 * @return Число полученных байт данных.
 */
EXTERN size_t usart_int_bytes_received(usart_int_t* usart);

/**
 * Получает число переданных
 * после завершения передачи байт данных.
 * @param usart Шина USART.
 * @return Число переданных байт данных.
 */
EXTERN size_t usart_int_bytes_transmitted(usart_int_t* usart);

/**
 * Получает вид реакции на IDLE при приёме.
 * @param usart Шина USART.
 * @return Вид реакции на IDLE.
 */
EXTERN usart_int_idle_mode_t usart_int_idle_mode(usart_int_t* usart);

/**
 * Устанавливает вид реакции на IDLE при приёме.
 * @param usart Шина USART.
 * @param mode Вид реакции на IDLE.
 */
EXTERN void usart_int_set_idle_mode(usart_int_t* usart, usart_int_idle_mode_t mode);

/**
 * Пропускает текущий поток данных
 * до метки IDLE.
 * @param usart Шина USART.
 */
EXTERN void usart_int_sleep(usart_int_t* usart);

/**
 * Начинает принимать данные текущего потока
 * без ожидания метки IDLE.
 * @param usart Шина USART.
 */
EXTERN void usart_int_wake(usart_int_t* usart);

/**
 * Передаёт данные по шине USART.
 * Асинхронная операция.
 * @param usart Шина USART.
 * @param data Данные.
 * @param size Размер данных.
 * @return Код ошибки.
 */
EXTERN err_t usart_int_send(usart_int_t* usart, const void* data, size_t size);

/**
 * Направляет все последующие
 * полученные данные вплоть до
 * заданного размера в заданный буфер.
 * @param usart Шина USART.
 * @param data Буфер для данных.
 * @param size Размер буфера.
 * @return Код ошибки.
 */
EXTERN err_t usart_int_recv(usart_int_t* usart, void* data, size_t size);

#endif	/* USART_INT_H */
