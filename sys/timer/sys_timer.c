#include "sys_timer.h"
#include <assert.h>
#include <stddef.h>


METHOD_CALC_IMPL(M_sys_timer, sys_tmr) {
	if (sys_tmr->TIM->SR & TIM_SR_UIF) {
		sys_tmr->TIM->SR = ~TIM_SR_UIF;

		sys_tmr->out_counter++;

		CALLBACK_CALL(sys_tmr->on_timeout);
	}
}

static int timer_init_impl(M_sys_timer* sys_tmr)
{
    sys_tmr->TIM->CR1 = 0;
    sys_tmr->TIM->CR2 = 0;
    sys_tmr->TIM->DIER = TIM_DIER_UIE;
    sys_tmr->TIM->PSC = SYS_TIMER_PRESCALER - 1;
    sys_tmr->TIM->ARR = SYS_TIMER_TICKS_PERIOD_US - 1;
    sys_tmr->TIM->CNT = 0;
    sys_tmr->TIM->SR  = 0;

    return 0;
}

static int timer_start_impl(M_sys_timer* sys_tmr)
{
    sys_tmr->TIM->CR1 |= TIM_CR1_CEN;

    return 0;
}

static int timer_stop_impl(M_sys_timer* sys_tmr)
{
    sys_tmr->TIM->CR1 &= ~TIM_CR1_CEN;

    return 0;
}

static int timer_isrunning_impl(M_sys_timer* sys_tmr)
{
    return (sys_tmr->TIM->CR1 & TIM_CR1_CEN) != 0;
}

static void timer_deinit_impl(M_sys_timer* sys_tmr)
{
	sys_tmr->TIM->CR1 = 0;
	sys_tmr->TIM->CR2 = 0;
	sys_tmr->TIM->DIER = 0;
	sys_tmr->TIM->PSC = 0;
	sys_tmr->TIM->ARR = 0;
	sys_tmr->TIM->CNT = 0;
	sys_tmr->TIM->SR  = 0;
}


METHOD_INIT_IMPL(M_sys_timer, sys_tmr)
{
    sys_tmr->control = 0;
    sys_tmr->status = 0;
    sys_tmr->out_counter = 0;

    int res = timer_init_impl(sys_tmr);

    if(res == 0){
        sys_tmr->status = SYS_TIMER_STATUS_READY;
    }else{
        sys_tmr->status = SYS_TIMER_STATUS_ERROR;
    }
}

METHOD_DEINIT_IMPL(M_sys_timer, sys_tmr)
{
    timer_deinit_impl(sys_tmr);

    sys_tmr->status = SYS_TIMER_STATUS_NONE;
}

METHOD_CONTROL_IMPL(M_sys_timer, sys_tmr)
{
    if(!(sys_tmr->status & SYS_TIMER_STATUS_READY)){
        return;
    }

    if(sys_tmr->control & SYS_TIMER_CONTROL_ENABLE){
        //if(!(sys_tmr->status & SYS_TIMER_STATUS_RUN)){
        if(!timer_isrunning_impl(sys_tmr)){
            int res = timer_start_impl(sys_tmr);
            if(res == 0){
                sys_tmr->status = SYS_TIMER_STATUS_READY | SYS_TIMER_STATUS_RUN;
            }else{
                sys_tmr->status = SYS_TIMER_STATUS_ERROR;
            }
        }
    }else{ // !(sys_tmr->control & SYS_TIMER_CONTROL_ENABLE)
        //if(sys_tmr->status & SYS_TIMER_STATUS_RUN){
        if(timer_isrunning_impl(sys_tmr)){
            int res = timer_stop_impl(sys_tmr);
            if(res == 0){
                sys_tmr->status = SYS_TIMER_STATUS_READY;
            }else{
                sys_tmr->status = SYS_TIMER_STATUS_ERROR;
            }
        }
    }
}
