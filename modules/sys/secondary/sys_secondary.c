#include "modules/modules.h"
#include "sys_secondary.h"
#include <assert.h>

#include "modules/sys/counter/sys_counter.h"
#include <sys/time.h>

#include <stdio.h>

static void sys_secondary_handler(void* arg)
{
	M_sys_secondary* sys = (M_sys_secondary*)arg;
    assert(sys != NULL);

    CALC((*sys)); //sys_secondary
}


METHOD_INIT_IMPL(M_sys_secondary, sys)
{
	// Инициализация внутренних переменных.
	sys->control = SYS_SECONDARY_CONTROL_NONE;
	sys->status = SYS_SECONDARY_STATUS_NONE;
	sys->errors = SYS_SECONDARY_ERROR_NONE;
	sys->warnings = SYS_SECONDARY_WARNING_NONE;
	sys->state = SYS_SECONDARY_STATE_NONE;

	// Статус инициализации
	status_t init_status = STATUS_NONE;

	// Модули в CALC
	INIT(panel_led);
	INIT(modbus_to_can_panel);
	INIT(ntc_temp);
	INIT(temp_comp);

	//Модули в IDLE
	INIT(cli);

    // Таймер второстепенной системы.
    INIT(sys_secondary_tim); //TIM4
    CALLBACK_PROC(sys_secondary_tim.on_timeout) = sys_secondary_handler;
    CALLBACK_ARG(sys_secondary_tim.on_timeout) = (void*)sys;
    if(sys_secondary_tim.status & MS_TIMER_STATUS_ERROR){
        init_status = STATUS_ERROR;
    }

    // Включение в работу модулей.
    // Не будем включать модули, если инициализация завершена с ошибкой.
    if(!(init_status & STATUS_ERROR)){
        // Запуск таймера второстепенной системы
        sys_secondary_tim.control = MS_TIMER_CONTROL_ENABLE;
        CONTROL(sys_secondary_tim);
        if(!(sys_secondary_tim.status & MS_TIMER_STATUS_RUN)){
            init_status = STATUS_ERROR;
        }
    }

    // Проверка ошибок инициализации.
    // Если нет ошибок - продолжим инициализацию.
    if(!(init_status & STATUS_ERROR)){
    	sys->state = SYS_SECONDARY_STATE_INIT;
    }else{ // Иначе установим статус ошибки.
    	sys->errors |= SYS_SECONDARY_ERROR_INTERNAL;
    	sys->status = SYS_SECONDARY_STATUS_ERROR;
    }
}

METHOD_DEINIT_IMPL(M_sys_secondary, sys)
{
	// Деинициализация модулей
	DEINIT(sys_secondary_tim);

	//Модули в CALC
    DEINIT(panel_led);
	DEINIT(modbus_to_can_panel);
    DEINIT(ntc_temp);
    DEINIT(temp_comp);

	//Модули в IDLE
	DEINIT(cli);

    // Сброс внутренних переменных.
    sys->control = SYS_SECONDARY_CONTROL_NONE;
    sys->status = SYS_SECONDARY_STATUS_NONE;
    sys->errors = SYS_SECONDARY_ERROR_NONE;
    sys->warnings = SYS_SECONDARY_WARNING_NONE;
    sys->state = SYS_SECONDARY_STATE_NONE;
}

static void modules_is_ready(M_sys_secondary* sys, state_t ok) {
	if(
			(panel_led.status & PANEL_LED_STATUS_READY) &&
			(modbus_to_can_panel.status & MODBUS_TO_CAN_STATUS_READY) &&
			(ntc_temp.status & NTC_TEMP_STATUS_READY) &&
			(temp_comp.status & TEMP_WIND_COMP_STATUS_READY)) {
		sys->state = ok;
	}
}

//обработчик статуса чтения настроек
static status_t settings_status_mask;
static status_t settings_status_masked;

static void settings_status_handler(M_sys_secondary* sys, state_t ok, state_t not_ok) {
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

static void modules_start(M_sys_secondary* sys, state_t ok) {
	panel_led.control |= DIGITAL_INPUT_CONTROL_START;
	//modbus_to_can_panel.control |= DIGITAL_OUTPUT_CONTROL_START;
	ntc_temp.control |= ANALOG_INPUT_CONTROL_START;
	temp_comp.control |= ANALOG_OUTPUT_CONTROL_START;
	sys->state = ok;
}

static void FSM_state_none(M_sys_secondary* sys)
{
	rgb_led.in_data_2 = RGB_LED_COLOR_BLACK;
}

//INIT->READY
static void FSM_state_init(M_sys_secondary* sys)
{
	rgb_led.in_data_2 = RGB_LED_COLOR_VIOLET;
	modules_is_ready(sys, SYS_SECONDARY_STATE_READY);
}

//READY->IDLE
static void FSM_state_ready(M_sys_secondary* sys)
{
	rgb_led.in_data_2 = RGB_LED_COLOR_BLUE_DARK;
	settings_status_handler(sys, SYS_SECONDARY_STATE_IDLE, SYS_SECONDARY_STATE_ERROR);
}

//IDLE->RUN
static void FSM_state_idle(M_sys_secondary* sys)
{
	rgb_led.in_data_2 = RGB_LED_COLOR_BLUE;
	modules_start(sys, SYS_SECONDARY_STATE_RUN);
}

static void FSM_state_run(M_sys_secondary* sys)
{
	rgb_led.in_data_2 = RGB_LED_COLOR_GREEN;
}

static void FSM_state_error(M_sys_secondary* sys)
{
	rgb_led.in_data_2 = RGB_LED_COLOR_RED;
}

static void FSM_state(M_sys_secondary* sys) {

    switch(sys->state){
    case SYS_SECONDARY_STATE_NONE:
        FSM_state_none(sys);
        break;
    case SYS_SECONDARY_STATE_INIT:
        FSM_state_init(sys);
        break;
    case SYS_SECONDARY_STATE_READY:
        FSM_state_ready(sys);
        break;
    case SYS_SECONDARY_STATE_IDLE:
        FSM_state_idle(sys);
        break;
    case SYS_SECONDARY_STATE_RUN:
        FSM_state_run(sys);
        break;
    case SYS_SECONDARY_STATE_ERROR:
        FSM_state_error(sys);
        break;
    default:
        // TODO: reaction on invalid state error.
    	sys->errors |= SYS_SECONDARY_ERROR_SOFTWARE;
    	sys->state = SYS_SECONDARY_STATE_ERROR;
        break;
    }
}

struct timeval sys_secondary_execution_time;

METHOD_CALC_IMPL(M_sys_secondary, sys)
{
	//время начала
	struct timeval tv_start;
	sys_counter_value(&tv_start);

	FSM_state(sys);

	//Модули
	CALC(panel_led);
	CALC(modbus_to_can_panel);
    CALC(ntc_temp);
    CALC(temp_comp);

    //стоп
    struct timeval tv_stop;
    sys_counter_value(&tv_stop);

    //дельта времени
    timersub(&tv_stop, &tv_start, &sys_secondary_execution_time);
}

METHOD_IDLE_IMPL(M_sys_secondary, sys)
{
	IDLE(cli);
}


