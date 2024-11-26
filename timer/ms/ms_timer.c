#include "ms_timer.h"
#include <assert.h>
#include <stddef.h>


METHOD_CALC_IMPL(M_ms_timer, ms_tmr) {
	if (ms_tmr->TIM->SR & TIM_SR_UIF) {
		ms_tmr->TIM->SR = ~TIM_SR_UIF;

		ms_tmr->out_counter++;

		CALLBACK_CALL(ms_tmr->on_timeout);
	}
}

static int timer_init_impl(M_ms_timer* ms_tmr)
{
	//TIM4 MS_TIM
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //TODO: добавить макрос параметризации шины и маски

    ms_tmr->TIM->CR1 = 0;
    ms_tmr->TIM->CR2 = 0;
    ms_tmr->TIM->DIER = TIM_DIER_UIE;
    ms_tmr->TIM->PSC = MS_TIMER_PRESCALER - 1;
    ms_tmr->TIM->ARR = MS_TIMER_TICKS_PERIOD_US - 1;
    ms_tmr->TIM->CNT = 0;
    ms_tmr->TIM->SR  = 0;

    //таймер проинициализирован?
    if((ms_tmr->TIM->PSC == (MS_TIMER_PRESCALER - 1)) &&
    		(ms_tmr->TIM->ARR == (MS_TIMER_TICKS_PERIOD_US - 1))) {
    	return 0;
    } else {
    	return -1;
    }
}

static int timer_start_impl(M_ms_timer* ms_tmr)
{
	ms_tmr->TIM->CR1 |= TIM_CR1_CEN;

    return 0;
}

static int timer_stop_impl(M_ms_timer* ms_tmr)
{
	ms_tmr->TIM->CR1 &= ~TIM_CR1_CEN;

    return 0;
}

static int timer_isrunning_impl(M_ms_timer* ms_tmr)
{
	return (ms_tmr->TIM->CR1 & TIM_CR1_CEN) != 0;
}

static void timer_deinit_impl(M_ms_timer* ms_tmr)
{
	ms_tmr->TIM->CR1 = 0;
	ms_tmr->TIM->CR2 = 0;
	ms_tmr->TIM->DIER = 0;
	ms_tmr->TIM->PSC = 0;
	ms_tmr->TIM->ARR = 0;
	ms_tmr->TIM->CNT = 0;
	ms_tmr->TIM->SR  = 0;
}


METHOD_INIT_IMPL(M_ms_timer, ms_tmr)
{
    ms_tmr->control = 0;
    ms_tmr->status = 0;
    ms_tmr->out_counter = 0;

    int res = timer_init_impl(ms_tmr);

    if(res == 0){
        ms_tmr->status = MS_TIMER_STATUS_READY;
        NVIC_SetPriority(MS_TIM_IRQN, MS_TIM_IRQ_PRIO);
        NVIC_EnableIRQ(MS_TIM_IRQN);
    }else{
        ms_tmr->status = MS_TIMER_STATUS_ERROR;
    }
}

METHOD_DEINIT_IMPL(M_ms_timer, ms_tmr)
{
    timer_deinit_impl(ms_tmr);

    ms_tmr->status = MS_TIMER_STATUS_NONE;
}

METHOD_CONTROL_IMPL(M_ms_timer, ms_tmr)
{
    if(!(ms_tmr->status & MS_TIMER_STATUS_READY)){
        return;
    }

    if(ms_tmr->control & MS_TIMER_CONTROL_ENABLE){
        //if(!(ms_tmr->status & MS_TIMER_STATUS_RUN)){
        if(!timer_isrunning_impl(ms_tmr)){
            int res = timer_start_impl(ms_tmr);
            if(res == 0){
                ms_tmr->status = MS_TIMER_STATUS_READY | MS_TIMER_STATUS_RUN;
            }else{
                ms_tmr->status = MS_TIMER_STATUS_ERROR;
            }
        }
    }else{ // !(ms_tmr->control & MS_TIMER_CONTROL_ENABLE)
        //if(ms_tmr->status & MS_TIMER_STATUS_RUN){
        if(timer_isrunning_impl(ms_tmr)){
            int res = timer_stop_impl(ms_tmr);
            if(res == 0){
                ms_tmr->status = MS_TIMER_STATUS_READY;
            }else{
                ms_tmr->status = MS_TIMER_STATUS_ERROR;
            }
        }
    }
}
