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

    // Вычислительные модули.
    INIT(cli);
    INIT(msdi);
    INIT(ntc_temp);
    INIT(digital_in);
    INIT(digital_out);
    INIT(ao_dac7562);
    INIT(mso);
    INIT(rgb_led);
    INIT(panel_led);

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

    DEINIT(mso);
    DEINIT(cli);
    DEINIT(msdi);
    DEINIT(ntc_temp);
    DEINIT(digital_in);
    DEINIT(digital_out);
    DEINIT(ao_dac7562);
    DEINIT(rgb_led);
    DEINIT(panel_led);

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

#include "can/init/can_init.h" //CANopen
#include "CO_CLI_driver.h"
extern CO_SDO_CLI_Driver_t can1_cli_driver; //TODO: тест CANopen SDO CLI

CO_SDO_CLI_Queue* mc_pid_i_ptr = NULL; //TODO: тест CANopen SDO CLI
reg_iq24_t pid_i_out_value; //TODO: тест CANopen SDO CLI
reg_iq24_t pid_i_out_value_buffered; //TODO: тест CANopen SDO CLI

CO_SDO_CLI_Queue* mc_cmd_ptr = NULL; //TODO: тест CANopen SDO CLI
volatile uint32_t mc_cmd; //TODO: тест CANopen SDO CLI

struct timeval sys_main_execution_time; //TODO: определить куда засунуть

METHOD_CALC_IMPL(M_sys_main, sys)
{
	struct timeval tv_start; //время начала
	sys_counter_value(&tv_start);

    FSM_state(sys);

    // Вычислительные модули.
    CALC(msdi);
    CALC(ntc_temp);
    CALC(digital_in);

    //TODO: тест CANopen SDO CLI
    if(mc_pid_i_ptr == NULL) {
    	mc_pid_i_ptr = CO_SDO_CLI_read(&can1_cli_driver, 1, 0x2740, 5, &pid_i_out_value, 4, 20);
    } else {
    	if(mc_pid_i_ptr->m_state == CO_SDO_CLI_State_DONE) {
    		pid_i_out_value_buffered = pid_i_out_value/256;
    		mc_pid_i_ptr = NULL;
    	}
    }

    if(mc_cmd_ptr == NULL) {
    	mc_cmd_ptr = CO_SDO_CLI_write(&can1_cli_driver, 1, 0x2010, 3, &mc_cmd, 4, 20);
    } else {
		if (mc_cmd_ptr->m_state == CO_SDO_CLI_State_DONE) {
			if((digital_in.out_data & 0b1) == 0b1) {
				mc_cmd = 1;
			} else {
				mc_cmd = 2;
			}
			mc_cmd_ptr = NULL;
		}
    }

    //digital_out.in_data = digital_in.out_data; //TODO: тут должна быть логика MC

    CALC(ao_dac7562);
    CALC(digital_out);
    CALC(rgb_led);
    CALC(panel_led);

    CALC(mso);

    struct timeval tv_stop; //время конца
    sys_counter_value(&tv_stop);

    timersub(&tv_stop, &tv_start, &sys_main_execution_time); //дельта времени
}

METHOD_IDLE_IMPL(M_sys_main, sys)
{
    IDLE(conf);
    IDLE(mso);
    CALC(cli);
}

