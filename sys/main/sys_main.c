#include "modules/modules.h"
#include "sys_main.h"
#include <assert.h>

#include "sys/counter/sys_counter.h"
#include <sys/time.h>

#include <stdio.h>
#include "init/init.h"

#include "gpio/init/gpio_init.h" //TODO: убрать (для rs485_panel_detect)

#include "can/init/can_init.h" //CANopen

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

    // Базовый конфиг.
    INIT(conf);

    // Осциллограф.

    // Вычислительные модули.
    INIT(cli);
    INIT(rgb_led);
    INIT(msdi);
    ntc_temp.ai = msdi.out_ai; //настройка указателя данных NTC
    INIT(ntc_temp);
    INIT(do_relay);
    INIT(do_ncv7608);

    // Таймеры.
    // Системный таймер.
    INIT(sys_tim);
    CALLBACK_PROC(sys_tim.on_timeout) = sys_tim_handler;
    CALLBACK_ARG(sys_tim.on_timeout) = (void*)sys;
    if(sys_tim.status & SYS_TIMER_STATUS_ERROR){
        init_status = STATUS_ERROR;
    }

    // Медленный таймер.
    INIT(ms_tim);
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
        sys->state = SYS_MAIN_STATE_INIT;
    }else{ // Иначе установим статус ошибки.
        // TODO: reaction on init error.
        sys->errors |= SYS_MAIN_ERROR_INTERNAL;
        sys->status = SYS_MAIN_STATUS_ERROR;
    }
}

METHOD_DEINIT_IMPL(M_sys_main, sys)
{
    // Деинициализация модулей.
    DEINIT(sys_tim);
    DEINIT(ms_tim);
    DEINIT(conf);
    DEINIT(cli);
    DEINIT(rgb_led);
    DEINIT(msdi);
    DEINIT(ntc_temp);
    DEINIT(do_relay);
    DEINIT(do_ncv7608);

    // Вычислительные модули.


    // Сброс внутренних переменных.
    sys->control = SYS_MAIN_CONTROL_NONE;
    sys->status = SYS_MAIN_STATUS_NONE;
    sys->errors = SYS_MAIN_ERROR_NONE;
    sys->warnings = SYS_MAIN_WARNING_NONE;
    sys->state = SYS_MAIN_STATE_NONE;
}


static void FSM_state_none(M_sys_main* sys)
{
}

static void FSM_state_init(M_sys_main* sys)
{
}

static void FSM_state_idle(M_sys_main* sys)
{
}

static void FSM_state_ready(M_sys_main* sys)
{
}

static void FSM_state_run(M_sys_main* sys)
{
}

static void FSM_state_error(M_sys_main* sys)
{
}

static void FSM_state(M_sys_main* sys)
{
    switch(sys->state){
    case STATE_NONE:
        FSM_state_none(sys);
        break;
    case STATE_INIT:
        FSM_state_init(sys);
        break;
    case STATE_IDLE:
        FSM_state_idle(sys);
        break;
    case STATE_READY:
        FSM_state_ready(sys);
        break;
    case STATE_RUN:
        FSM_state_run(sys);
        break;
    case STATE_ERROR:
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

extern CO_t* co; //CANopen

METHOD_CALC_IMPL(M_sys_main, sys)
{
	struct timeval tv_start; //время начала
	sys_counter_value(&tv_start);

    FSM_state(sys);

    //CALC(conf); // conf не требует вычисления.

    // Вычислительные модули.
    can1_CO_process(co, 10000, NULL);

    CALC(cli);
    CALC(rgb_led);
    CALC(msdi);
    CALC(ntc_temp);
    CALC(do_relay);

    //TODO: убрать
    if(rs485_panel_detect()) {
    	 do_ncv7608.in |= 1;
    } else {
    	 do_ncv7608.in &= ~1;
    }

    CALC(do_ncv7608);

    // Последний модуль - запись лога.

    struct timeval tv_stop; //время конца
    sys_counter_value(&tv_stop);

    timersub(&tv_stop, &tv_start, &sys_main_execution_time); //дельта времени
}

METHOD_IDLE_IMPL(M_sys_main, sys)
{
    IDLE(conf);
    //IDLE(dlog);
}

