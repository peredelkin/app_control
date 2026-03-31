#include "modules/modules.h"
#include "sys_main.h"
#include <assert.h>

#include "sys/counter/sys_counter.h"
#include <sys/time.h>

#include <stdio.h>
#include "init/init.h"

static void sys_tim_handler(void* arg)
{
    M_sys_main* sys = (M_sys_main*)arg;
    assert(sys != NULL);

    CALC((*sys));
}

static void ms_tim_handler(void* arg)
{
    M_sys_main* sys = (M_sys_main*)arg;
    assert(sys != NULL);
}

void sys_main_idle_init();

METHOD_INIT_IMPL(M_sys_main, sys)
{
    // Инициализация внутренних переменных.
    sys->control = SYS_MAIN_CONTROL_NONE;
    sys->status = SYS_MAIN_STATUS_NONE;
    sys->errors = SYS_MAIN_ERROR_NONE;
    sys->warnings = SYS_MAIN_WARNING_NONE;
    sys->state = SYS_MAIN_STATE_NONE;


    // Инициализация модулей.
    status_t init_status = STATUS_NONE;

    //Индикатор состояния.
    INIT(rgb_led);

    // Вычислительные модули.
    INIT(digital_in);
    INIT(analog_in);
    INIT(analog_out);
    INIT(digital_out);
    INIT(modbus_to_can_panel);
    INIT(mso);

    //Модули, выполняемые в IDLE
    sys_main_idle_init();

    // Таймеры.
    // Системный таймер.
    INIT(sys_tim); //TIM3
    CALLBACK_PROC(sys_tim.on_timeout) = sys_tim_handler;
    CALLBACK_ARG(sys_tim.on_timeout) = (void*)sys;
    if(sys_tim.status & SYS_TIMER_STATUS_ERROR){
        init_status = STATUS_ERROR;
    }

    // Медленный таймер.
    INIT(ms_tim); //TIM4
    CALLBACK_PROC(ms_tim.on_timeout) = ms_tim_handler;
    CALLBACK_ARG(ms_tim.on_timeout) = (void*)sys;
    if(ms_tim.status & MS_TIMER_STATUS_ERROR){
        init_status = STATUS_ERROR;
    }

    // Включение в работу модулей.
    // Не будем включать модули,
    // если инициализация завершена с ошибкой.
    if(!(init_status & STATUS_ERROR)){

        // Запуск системного таймера.
    	sys_tim.control = SYS_TIMER_CONTROL_ENABLE;
		CONTROL(sys_tim);
        if(!(sys_tim.status & SYS_TIMER_STATUS_RUN)){
            init_status = STATUS_ERROR;
        }

        // Запуск медленного таймера.
        ms_tim.control = MS_TIMER_CONTROL_ENABLE;
        CONTROL(ms_tim);
        if(!(ms_tim.status & MS_TIMER_STATUS_RUN)){
            init_status = STATUS_ERROR;
        }

    }

    // Проверка ошибок инициализации.
    // Если нет ошибок - продолжим инициализацию.
    if(!(init_status & STATUS_ERROR)){
    	//команда чтения настроек
    	settings_cmd_read(&settings);
        sys->state = SYS_MAIN_STATE_INIT;
    }else{ // Иначе установим статус ошибки.
        // TODO: reaction on init error.
        sys->errors |= SYS_MAIN_ERROR_INTERNAL;
        sys->status = SYS_MAIN_STATUS_ERROR;
    }
}

void sys_main_idle_deinit();

METHOD_DEINIT_IMPL(M_sys_main, sys)
{
    // Деинициализация модулей.
    DEINIT(sys_tim);
    DEINIT(ms_tim);

    DEINIT(ntc_temp);
    DEINIT(temp_comp);
    DEINIT(digital_in);
    DEINIT(analog_in);
    DEINIT(analog_out);
    DEINIT(digital_out);
    DEINIT(modbus_to_can_panel);
    DEINIT(mso);

    //Индикатор состояния
    DEINIT(rgb_led);

    //Модули выполняемые в IDLE
    sys_main_idle_deinit();


    // Сброс внутренних переменных.
    sys->control = SYS_MAIN_CONTROL_NONE;
    sys->status = SYS_MAIN_STATUS_NONE;
    sys->errors = SYS_MAIN_ERROR_NONE;
    sys->warnings = SYS_MAIN_WARNING_NONE;
    sys->state = SYS_MAIN_STATE_NONE;
}

//обработчик статуса чтения настроек
static status_t settings_status_mask;
static status_t settings_status_masked;

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

//флаги готовности модулей
bool digital_in_ready_run = false;
bool analog_in_ready_run = false;
bool analog_out_ready_run = false;
bool digital_out_ready_run = false;

//функции обработки статусов модулей
static void digital_in_dependencies_check() {
	if(digital_in_ready_run == false) {
		if((digital_in.status & (DIGITAL_INPUT_STATUS_READY | DIGITAL_INPUT_STATUS_RUN)) ==
				(DIGITAL_INPUT_STATUS_READY | DIGITAL_INPUT_STATUS_RUN)) {
			digital_in_ready_run = true;
		}
	}
}

static void analog_in_dependencies_check() {
	analog_in_ready_run = true;
}

static void analog_out_dependencies_check() {
	analog_out_ready_run = true;
}

static void digital_out_dependencies_check() {
	if(digital_out_ready_run == false) {
		if((digital_out.status & (DIGITAL_OUTPUT_STATUS_READY | DIGITAL_OUTPUT_STATUS_RUN)) ==
				(DIGITAL_OUTPUT_STATUS_READY | DIGITAL_OUTPUT_STATUS_RUN)) {
			digital_out_ready_run = true;
		}
	}
}

static bool modules_dependencies_check() {
	return (digital_in_ready_run &&
			analog_in_ready_run &&
			analog_out_ready_run &&
			digital_out_ready_run);
}

static void modules_dependencies_start() {
	digital_in.control |= DIGITAL_INPUT_CONTROL_START;
	analog_in.control |= ANALOG_INPUT_CONTROL_START;
	analog_out.control |= ANALOG_OUTPUT_CONTROL_START;
	digital_out.control |= DIGITAL_OUTPUT_CONTROL_START;
}

static void FSM_state_none(M_sys_main* sys)
{
	rgb_led.in_data = RGB_LED_COLOR_BLACK;
}

static void FSM_state_init(M_sys_main* sys)
{
	rgb_led.in_data = RGB_LED_COLOR_VIOLET;
	settings_status_handler(sys, STATE_IDLE, STATE_ERROR);
}

static void FSM_state_idle(M_sys_main* sys)
{
	rgb_led.in_data = RGB_LED_COLOR_BLUE_DARK;
	modules_dependencies_start();
	sys->state = STATE_READY;
}

static void FSM_state_ready(M_sys_main* sys)
{
	rgb_led.in_data = RGB_LED_COLOR_BLUE;

	//проверим зависимости модулей
	digital_in_dependencies_check();
	analog_in_dependencies_check();
	analog_out_dependencies_check();
	digital_out_dependencies_check();

	if(modules_dependencies_check()) {
		sys->state = STATE_RUN;
	}
}

static void FSM_state_run(M_sys_main* sys)
{
	rgb_led.in_data = RGB_LED_COLOR_GREEN;
}

static void FSM_state_error(M_sys_main* sys)
{
	rgb_led.in_data = RGB_LED_COLOR_RED;
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
    case SYS_MAIN_STATE_IDLE:
        FSM_state_idle(sys);
        break;
    case SYS_MAIN_STATE_READY:
        FSM_state_ready(sys);
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

struct timeval sys_main_execution_time; //TODO: определить куда засунуть

METHOD_CALC_IMPL(M_sys_main, sys)
{
	struct timeval tv_start; //время начала
	sys_counter_value(&tv_start);

    FSM_state(sys);
    //Индикатор состояния
    CALC(rgb_led);

    // Вычислительные модули.
    CALC(digital_in);
    CALC(analog_in);
    CALC(analog_out);
    CALC(digital_out);
    CALC(modbus_to_can_panel);
    CALC(mso);

    struct timeval tv_stop; //время конца
    sys_counter_value(&tv_stop);

    timersub(&tv_stop, &tv_start, &sys_main_execution_time); //дельта времени
}

void sys_main_idle_init() {
	INIT(cli);
	INIT(settings);
    INIT(conf);
}

void sys_main_idle_deinit() {
	DEINIT(cli);
	DEINIT(settings);
    DEINIT(conf);
}

METHOD_IDLE_IMPL(M_sys_main, sys)
{
	IDLE(cli);
	IDLE(settings);
    IDLE(conf);
    IDLE(mso);
}

