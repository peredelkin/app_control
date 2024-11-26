/**
 * @file sys_counter.h Библиотека таймера высокого разрешения.
 */

#ifndef SYSTEM_COUNTER_H
#define SYSTEM_COUNTER_H

#include <stm32f4xx.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>



// Параметры таймера.
//! Период таймера в тиках.
#define SYS_COUNTER_PERIOD_TICKS 1000000
//! Предделитель таймера.
#define SYS_COUNTER_PRESCALER 90



/**
 * Инициализирует высокоточный таймер привода.
 * @param TIM Периферия таймера.
 * @return Код ошибки.
 */
extern void sys_counter_init(TIM_TypeDef* timer);

/**
 * Обработчик прерывания переполнения таймера высокого разрешения.
 */
extern void sys_counter_irq_handler(void);

/**
 * Устанавливает разрешение прерывания переполнения таймера.
 * @param enabled Флаг разрешения прерывания.
 */
extern void sys_counter_irq_set_enabled(bool enabled);

/**
 * Получает разрешение прерывания переполнения таймера.
 * @return Флаг разрешения прерывания.
 */
extern bool sys_counter_irq_enabled(void);

/**
 * Включает прерывание таймера.
 */
extern void sys_counter_irq_enable(void);

/**
 * Выключает прерывание таймера.
 */
extern void sys_counter_irq_disable(void);

/**
 * Получает флаг режима однократного срабатывания.
 * @return Флаг режима однократного срабатывания.
 */
extern bool sys_counter_one_pulse_mode(void);

/**
 * Устанавливает флаг режима однократного срабатывания.
 * @return Флаг режима однократного срабатывания.
 */
extern bool sys_counter_one_pulse_mode(void);

/**
 * Устанавливает флаг работы таймера.
 * @param running Флаг работы таймера.
 */
extern void sys_counter_set_running(bool running);

/**
 * Получает флаг работы таймера.
 * @return Флаг работы таймера.
 */
extern bool sys_counter_running(void);

/**
 * Запускает таймер.
 */
extern void sys_counter_start(void);

/**
 * Останавливает таймер.
 */
extern void sys_counter_stop(void);

/**
 * Сбрасывает счётчик таймера.
 */
extern void sys_counter_reset(void);

/**
 * Получает время работы таймера.
 * Переполняется каждые 4294967296 секунд.
 * @param tv Время.
 */
extern void sys_counter_value(struct timeval* tv);

/**
 * Функция задержки
 * @param sec Время задержки в секундах.
 * @param usec Время задержки в микросекундах.
 */
extern void sys_counter_delay(uint32_t sec, uint32_t usec);

/**
 * Функция вывода времени
 */
extern void sys_counter_tv_print();

#endif /* SYSTEM_COUNTER_H */

