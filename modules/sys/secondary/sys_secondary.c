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

	// Статус инициализации
	status_t init_status = STATUS_NONE;

	// Модули в CALC
	//INIT(rgb_led);
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
        // TODO: reaction on init error.
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

struct timeval sys_secondary_execution_time;

METHOD_CALC_IMPL(M_sys_secondary, sys_secondary)
{
	//время начала
	struct timeval tv_start;
	sys_counter_value(&tv_start);

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


