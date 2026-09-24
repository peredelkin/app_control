#include "modules/modules.h"
#include "sys_main.h"
#include <assert.h>

#include "modules/sys/counter/sys_counter.h"
#include <sys/time.h>

#include <stdio.h>

static void sys_main_handler(void* arg)
{
    M_sys_main* sys = (M_sys_main*)arg;
    assert(sys != NULL);

    CALC((*sys)); //sys_main
}


METHOD_INIT_IMPL(M_sys_main, sys)
{
    // Инициализация внутренних переменных.
    sys->control = SYS_MAIN_CONTROL_NONE;
    sys->status = SYS_MAIN_STATUS_NONE;
    sys->errors = SYS_MAIN_ERROR_NONE;
    sys->warnings = SYS_MAIN_WARNING_NONE;
    sys->state = SYS_MAIN_STATE_NONE;

    // Статус инициализации
    status_t init_status = STATUS_NONE;

    // Модули в CALC
    INIT(digital_in);
    INIT(digital_out);
    INIT(analog_in);
    INIT(analog_out);

    //Модули в IDLE
	INIT(settings);

    // Таймер основной системыы
    INIT(sys_main_tim); //TIM3
    CALLBACK_PROC(sys_main_tim.on_timeout) = sys_main_handler;
    CALLBACK_ARG(sys_main_tim.on_timeout) = (void*)sys;
    if(sys_main_tim.status & SYS_TIMER_STATUS_ERROR){
        init_status = STATUS_ERROR;
    }

    // Включение в работу модулей.
    // Не будем включать модули, если инициализация завершена с ошибкой.
    if(!(init_status & STATUS_ERROR)){
        // Запуск таймера основной системы.
    	sys_main_tim.control = SYS_TIMER_CONTROL_ENABLE;
		CONTROL(sys_main_tim);
        if(!(sys_main_tim.status & SYS_TIMER_STATUS_RUN)){
            init_status = STATUS_ERROR;
        }
    }

    // Проверка ошибок инициализации.
    // Если нет ошибок - продолжим инициализацию.
    if(!(init_status & STATUS_ERROR)){
        sys->state = SYS_MAIN_STATE_INIT;
    }else{ // Иначе установим статус ошибки.
        sys->errors |= SYS_MAIN_ERROR_INTERNAL;
        sys->status = SYS_MAIN_STATUS_ERROR;
    }
}

METHOD_DEINIT_IMPL(M_sys_main, sys)
{
    // Деинициализация модулей.
    DEINIT(sys_main_tim);

    //Модули в CALC
    DEINIT(digital_in);
    DEINIT(digital_out);
    DEINIT(analog_in);
    DEINIT(analog_out);

    //Модули в IDLE
    DEINIT(settings);

    // Сброс внутренних переменных.
    sys->control = SYS_MAIN_CONTROL_NONE;
    sys->status = SYS_MAIN_STATUS_NONE;
    sys->errors = SYS_MAIN_ERROR_NONE;
    sys->warnings = SYS_MAIN_WARNING_NONE;
    sys->state = SYS_MAIN_STATE_NONE;
}

static void modules_is_ready(M_sys_main* sys, state_t ok) {
	if(
			(digital_in.status & DIGITAL_INPUT_STATUS_READY) &&
			(digital_out.status & DIGITAL_OUTPUT_STATUS_READY) &&
			(analog_in.status & ANALOG_INPUT_STATUS_READY) &&
			(analog_out.status & ANALOG_OUTPUT_STATUS_READY)) {
		sys->state = ok;
	}
}

//обработчик статуса чтения настроек
static status_t settings_status_mask;
static status_t settings_status_masked;

/**
 * @brief Обработчик изменения статуса настроек.
 *
 * Функция отслеживает момент завершения чтения настроек (флаг READ_DONE).
 * Сам флаг READ_DONE не маскируется, что позволяет функции реагировать на каждое новое чтение.
 *
 * Переключение состояния системы (sys->state) происходит однократно только при изменении
 * результата: флага VALID или ERROR. Так как эти флаги взаимно сбрасывают друг друга
 * в маске, обработчик автоматически готов зафиксировать смену успешного результата
 * на ошибку (и наоборот) при последующих вызовах, исключая при этом циклическое
 * перезаписывание стейта на каждой итерации основного цикла.
 *
 * @param sys    Указатель на главный контекст системы для обновления стейта
 * @param ok     Целевое состояние при валидных настройках
 * @param not_ok Целевое состояние при ошибке чтения
 */
static void settings_status_handler(M_sys_main* sys, state_t ok, state_t not_ok) {
	settings_status_masked = settings.status & ~settings_status_mask;

	//настройки прочитаны
	if(settings_status_masked & SETTINGS_STATUS_READ_DONE) {
		if(settings_status_masked & SETTINGS_STATUS_VALID) {
			settings_status_mask |= SETTINGS_STATUS_VALID;
			settings_status_mask &= ~SETTINGS_STATUS_ERROR;
			sys->state = ok;
		}

		if(settings_status_masked & SETTINGS_STATUS_ERROR) {
			settings_status_mask |= SETTINGS_STATUS_ERROR;
			settings_status_mask &= ~SETTINGS_STATUS_VALID;
			sys->state = not_ok;
		}
	}
}

static void mosules_start(M_sys_main* sys, state_t ok) {
	digital_in.control |= DIGITAL_INPUT_CONTROL_START;
	digital_out.control |= DIGITAL_OUTPUT_CONTROL_START;
	analog_in.control |= ANALOG_INPUT_CONTROL_START;
	analog_out.control |= ANALOG_OUTPUT_CONTROL_START;
	sys->state = ok;
}

static void FSM_state_none(M_sys_main* sys)
{
	rgb_led.in_data_1 = RGB_LED_COLOR_BLACK;
}

//INIT->READY
static void FSM_state_init(M_sys_main* sys)
{
	rgb_led.in_data_1 = RGB_LED_COLOR_VIOLET;
	modules_is_ready(sys, SYS_MAIN_STATE_READY);
}

//IDLE->RUN
static void FSM_state_idle(M_sys_main* sys)
{
	rgb_led.in_data_1 = RGB_LED_COLOR_BLUE_DARK;
	mosules_start(sys, SYS_MAIN_STATE_RUN);
}

//READY->IDLE
static void FSM_state_ready(M_sys_main* sys)
{
	rgb_led.in_data_1 = RGB_LED_COLOR_BLUE;
	settings_status_handler(sys, SYS_MAIN_STATE_IDLE, SYS_MAIN_STATE_ERROR);
}

static void FSM_state_run(M_sys_main* sys)
{
	rgb_led.in_data_1 = RGB_LED_COLOR_GREEN;

}

static void FSM_state_error(M_sys_main* sys)
{
	rgb_led.in_data_1 = RGB_LED_COLOR_RED;
}

static void FSM_state(M_sys_main* sys)
{

    switch(sys->state){
    case SYS_MAIN_STATE_NONE:
        FSM_state_none(sys);
        break;
    case SYS_MAIN_STATE_INIT:
        FSM_state_init(sys);
        break;
    case SYS_MAIN_STATE_READY:
        FSM_state_ready(sys);
        break;
    case SYS_MAIN_STATE_IDLE:
        FSM_state_idle(sys);
        break;
    case SYS_MAIN_STATE_RUN:
        FSM_state_run(sys);
        break;
    case SYS_MAIN_STATE_ERROR:
        FSM_state_error(sys);
        break;
    default:
        // TODO: reaction on invalid state error.
        sys->errors |= SYS_MAIN_ERROR_SOFTWARE;
        sys->state = STATE_ERROR;
        break;
    }
}

struct timeval sys_main_execution_time;

METHOD_CALC_IMPL(M_sys_main, sys)
{
	//старт
	struct timeval tv_start;
	sys_counter_value(&tv_start);

    FSM_state(sys);

    // Вычислительные модули.
    CALC(digital_in);
    CALC(analog_in);
    CALC(analog_out);
    CALC(digital_out);

    //стоп
    struct timeval tv_stop;
    sys_counter_value(&tv_stop);

    //дельта времени
    timersub(&tv_stop, &tv_start, &sys_main_execution_time);
}

METHOD_IDLE_IMPL(M_sys_main, sys)
{
	IDLE(settings);
}

