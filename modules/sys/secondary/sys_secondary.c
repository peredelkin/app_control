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


METHOD_INIT_IMPL(M_sys_secondary, sys_secondary)
{
	// Инициализация внутренних переменных.
	sys_secondary->control = SYS_SECONDARY_CONTROL_NONE;
	sys_secondary->status = SYS_SECONDARY_STATUS_NONE;
	sys_secondary->errors = SYS_SECONDARY_ERROR_NONE;
	sys_secondary->warnings = SYS_SECONDARY_WARNING_NONE;
	sys_secondary->state = SYS_SECONDARY_STATE_NONE;

	// Статус инициализации
	status_t init_status = STATUS_NONE;

	// Модули в CALC
	INIT(rgb_led);
	INIT(panel_led);
	INIT(modbus_to_can_panel);
	INIT(ntc_temp);
	INIT(temp_comp);

	//Модули в IDLE
	INIT(cli);

    // Таймер второстепенной системы.
    INIT(sys_secondary_tim); //TIM4
    CALLBACK_PROC(sys_secondary_tim.on_timeout) = sys_secondary_handler;
    CALLBACK_ARG(sys_secondary_tim.on_timeout) = (void*)sys_secondary;
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
    	sys_secondary->state = SYS_MAIN_STATE_INIT;
    }else{ // Иначе установим статус ошибки.
    	sys_secondary->errors |= SYS_MAIN_ERROR_INTERNAL;
    	sys_secondary->status = SYS_MAIN_STATUS_ERROR;
    }
}

METHOD_DEINIT_IMPL(M_sys_secondary, sys_secondary)
{
	// Деинициализация модулей
	DEINIT(sys_secondary_tim);

	//Модули в CALC
    DEINIT(rgb_led);
    DEINIT(panel_led);
	DEINIT(modbus_to_can_panel);
    DEINIT(ntc_temp);
    DEINIT(temp_comp);

	//Модули в IDLE
	DEINIT(cli);
}
//обработчик статуса чтения настроек
static status_t settings_status_mask;
static status_t settings_status_masked;

static void settings_status_handler(M_sys_secondary* sys_secondary, state_t ok, state_t not_ok) {
	settings_status_masked = settings.status & ~settings_status_mask;

	//настройки прочитаны
	if(settings_status_masked & SETTINGS_STATUS_READ_DONE) {
		if(settings_status_masked & SETTINGS_STATUS_VALID) {
			settings_status_mask |= SETTINGS_STATUS_VALID;
			settings_status_mask &= ~SETTINGS_STATUS_ERROR;
			sys_secondary->state = ok;
		}

		if(settings_status_masked & SETTINGS_STATUS_ERROR) {
			settings_status_mask |= SETTINGS_STATUS_ERROR;
			settings_status_mask &= ~SETTINGS_STATUS_VALID;
			sys_secondary->state = not_ok;
		}
	}
}

//флаги готовности модулей
bool rgb_led_ready_run = false;
bool panel_led_ready_run = false;
bool modbus_to_can_panel_ready_run = false;
bool ntc_temp_ready_run = false;
bool temp_comp_ready_run = false;

static void rgb_led_dependencies_check() {
	if(rgb_led_ready_run == false) {
		if((rgb_led.status & (RGB_LED_STATUS_READY | RGB_LED_STATUS_RUN)) ==
				(RGB_LED_STATUS_READY | RGB_LED_STATUS_RUN)) {
			rgb_led_ready_run = true;
		}
	}
}

static void panel_led_dependencies_check() {
	if(panel_led_ready_run == false) {
		if((panel_led.status & (PANEL_LED_STATUS_READY | PANEL_LED_STATUS_RUN)) ==
				(PANEL_LED_STATUS_READY | PANEL_LED_STATUS_RUN)) {
			panel_led_ready_run = true;
		}
	}
}

static void modbus_to_can_panel_dependencies_check() {
	if(modbus_to_can_panel_ready_run == false) {
		if(modbus_to_can_panel.status & MODBUS_TO_CAN_STATUS_READY) {
			modbus_to_can_panel_ready_run = true;
		}
	}
}

static void ntc_temp_dependencies_check() {
	if(ntc_temp_ready_run == false) {
		if((ntc_temp.status & (NTC_TEMP_STATUS_READY | NTC_TEMP_STATUS_RUN)) ==
				(NTC_TEMP_STATUS_READY | NTC_TEMP_STATUS_RUN)) {
			ntc_temp_ready_run = true;
		}
	}
}

static void temp_comp_dependencies_check() {
	if(temp_comp_ready_run == false) {
		if((temp_comp.status & (TEMP_WIND_COMP_STATUS_READY | TEMP_WIND_COMP_STATUS_RUN)) ==
					(TEMP_WIND_COMP_STATUS_READY | TEMP_WIND_COMP_STATUS_RUN)) {
			temp_comp_ready_run = true;
		}
	}
}

static bool modules_dependencies_check() {
	//проверим зависимости модулей
	rgb_led_dependencies_check();
	panel_led_dependencies_check();
	modbus_to_can_panel_dependencies_check();
	ntc_temp_dependencies_check();
	temp_comp_dependencies_check();

	return (rgb_led_ready_run &&
			panel_led_ready_run &&
			modbus_to_can_panel_ready_run &&
			ntc_temp_ready_run &&
			temp_comp_ready_run);
}

static void modules_dependencies_start() {
	rgb_led.control |= RGB_LED_CONTROL_START;
	panel_led.control |= PANEL_LED_CONTROL_START;
	//modbus_to_can_panel |= MODBUS_TO_CAN_CONTROL_START; Не нуждается в запуске
	ntc_temp.control |= NTC_TEMP_CONTROL_START;
	temp_comp.control |= TEMP_WIND_COMP_CONTROL_START;
}

static void FSM_state_none(M_sys_secondary* sys_secondary)
{
}

static void FSM_state_init(M_sys_secondary* sys_secondary)
{
	settings_status_handler(sys_secondary, STATE_IDLE, STATE_ERROR);
}

static void FSM_state_idle(M_sys_secondary* sys_secondary)
{
	modules_dependencies_start();
	sys_secondary->state = STATE_READY;
}

static void FSM_state_ready(M_sys_secondary* sys_secondary)
{
	if(modules_dependencies_check()) {
		sys_secondary->state = STATE_RUN;
	}
}

static void FSM_state_run(M_sys_secondary* sys_secondary)
{
}

static void FSM_state_error(M_sys_secondary* sys_secondary)
{
}

static void FSM_state(M_sys_secondary* sys_secondary) {

    switch(sys_secondary->state){
    case SYS_MAIN_STATE_NONE:
        FSM_state_none(sys_secondary);
        break;
    case SYS_MAIN_STATE_INIT:
        FSM_state_init(sys_secondary);
        break;
    case SYS_MAIN_STATE_IDLE:
        FSM_state_idle(sys_secondary);
        break;
    case SYS_MAIN_STATE_READY:
        FSM_state_ready(sys_secondary);
        break;
    case SYS_MAIN_STATE_RUN:
        FSM_state_run(sys_secondary);
        break;
    case SYS_MAIN_STATE_ERROR:
        FSM_state_error(sys_secondary);
        break;
    default:
        // TODO: reaction on invalid state error.
    	sys_secondary->errors |= SYS_MAIN_ERROR_SOFTWARE;
    	sys_secondary->state = STATE_ERROR;
        break;
    }
}

struct timeval sys_secondary_execution_time;

METHOD_CALC_IMPL(M_sys_secondary, sys_secondary)
{
	//время начала
	struct timeval tv_start;
	sys_counter_value(&tv_start);

	FSM_state(sys_secondary);

	//Модули
	CALC(rgb_led);
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

METHOD_IDLE_IMPL(M_sys_secondary, sys_secondary)
{
	IDLE(cli);
}


