/*
 * can_timer.c
 *
 *  Created on: 23 янв. 2025 г.
 *      Author: ruslan
 */

#include "can_timer.h"
#include <assert.h>
#include <stddef.h>


METHOD_CALC_IMPL(M_can_timer, can_tmr) {
	if (can_tmr->TIM->SR & TIM_SR_UIF) {
		can_tmr->TIM->SR = ~TIM_SR_UIF;

		can_tmr->out_counter++;

		CALLBACK_CALL(can_tmr->on_timeout);
	}
}

static int timer_init_impl(M_can_timer* can_tmr)
{
	//TIM5 CAN_TIM
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; //TODO: добавить макрос параметризации шины и маски

    can_tmr->TIM->CR1 = 0;
    can_tmr->TIM->CR2 = 0;
    can_tmr->TIM->DIER = TIM_DIER_UIE;
    can_tmr->TIM->PSC = CAN_TIMER_PRESCALER - 1;
    can_tmr->TIM->ARR = CAN_TIMER_TICKS_PERIOD_US - 1;
    can_tmr->TIM->CNT = 0;
    can_tmr->TIM->SR  = 0;

    //таймер проинициализирован?
    if((can_tmr->TIM->PSC == (CAN_TIMER_PRESCALER - 1)) &&
    		(can_tmr->TIM->ARR == (CAN_TIMER_TICKS_PERIOD_US - 1))) {
    	return 0;
    } else {
    	return -1;
    }
}

static int timer_start_impl(M_can_timer* can_tmr)
{
	can_tmr->TIM->CR1 |= TIM_CR1_CEN;

    return 0;
}

static int timer_stop_impl(M_can_timer* can_tmr)
{
	can_tmr->TIM->CR1 &= ~TIM_CR1_CEN;

    return 0;
}

static int timer_isrunning_impl(M_can_timer* can_tmr)
{
	return (can_tmr->TIM->CR1 & TIM_CR1_CEN) != 0;
}

static void timer_deinit_impl(M_can_timer* can_tmr)
{
	can_tmr->TIM->CR1 = 0;
	can_tmr->TIM->CR2 = 0;
	can_tmr->TIM->DIER = 0;
	can_tmr->TIM->PSC = 0;
	can_tmr->TIM->ARR = 0;
	can_tmr->TIM->CNT = 0;
	can_tmr->TIM->SR  = 0;
}


METHOD_INIT_IMPL(M_can_timer, can_tmr)
{
    can_tmr->control = 0;
    can_tmr->status = 0;
    can_tmr->out_counter = 0;

    int res = timer_init_impl(can_tmr);

    if(res == 0){
        can_tmr->status = CAN_TIMER_STATUS_READY;
        NVIC_SetPriority(CAN_TIM_IRQN, CAN_TIM_IRQ_PRIO);
        NVIC_EnableIRQ(CAN_TIM_IRQN);
    }else{
        can_tmr->status = CAN_TIMER_STATUS_ERROR;
    }
}

METHOD_DEINIT_IMPL(M_can_timer, can_tmr)
{
    timer_deinit_impl(can_tmr);

    can_tmr->status = CAN_TIMER_STATUS_NONE;
}

METHOD_CONTROL_IMPL(M_can_timer, can_tmr)
{
    if(!(can_tmr->status & CAN_TIMER_STATUS_READY)){
        return;
    }

    if(can_tmr->control & CAN_TIMER_CONTROL_ENABLE){
        //if(!(can_tmr->status & CAN_TIMER_STATUS_RUN)){
        if(!timer_isrunning_impl(can_tmr)){
            int res = timer_start_impl(can_tmr);
            if(res == 0){
                can_tmr->status = CAN_TIMER_STATUS_READY | CAN_TIMER_STATUS_RUN;
            }else{
                can_tmr->status = CAN_TIMER_STATUS_ERROR;
            }
        }
    }else{ // !(can_tmr->control & CAN_TIMER_CONTROL_ENABLE)
        //if(can_tmr->status & CAN_TIMER_STATUS_RUN){
        if(timer_isrunning_impl(can_tmr)){
            int res = timer_stop_impl(can_tmr);
            if(res == 0){
                can_tmr->status = CAN_TIMER_STATUS_READY;
            }else{
                can_tmr->status = CAN_TIMER_STATUS_ERROR;
            }
        }
    }
}

