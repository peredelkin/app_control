#include <stddef.h>
#include <stdio.h>
#include "sys_counter.h"


//! Тип структуры высокоточного таймера.
typedef struct _Sys_Counter {
    TIM_TypeDef* timer; //!< Периферия таймера.
    uint32_t counter; //!< Счётчик переполнений таймера.
} sys_counter_t;

//! Высокоточный таймер.
static sys_counter_t system_counter;

static void sys_counter_priph_init(TIM_TypeDef* TIM)
{
	//TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //TODO: добавить макрос параметризации шины и маски

    TIM->CR1 = 0;
    TIM->CR2 = 0;
    TIM->DIER = 0;
    TIM->PSC = SYS_COUNTER_PRESCALER - 1;
    TIM->ARR = SYS_COUNTER_PERIOD_TICKS - 1;
    TIM->CNT = 0;
    TIM->SR  = 0;
}


void sys_counter_init(TIM_TypeDef* timer)
{
    if(timer == NULL) while(1);
    
    system_counter.timer = timer;
    
    sys_counter_priph_init(timer);
}

void sys_counter_irq_handler(void)
{
    if(system_counter.timer->SR & TIM_SR_UIF){
        system_counter.timer->SR = ~TIM_SR_UIF;
        
        system_counter.counter ++;
    }
}

void sys_counter_irq_set_enabled(bool enabled)
{
    if(enabled){
        system_counter.timer->DIER |= TIM_DIER_UIE;
    }else{
        system_counter.timer->DIER &= ~TIM_DIER_UIE;
    }
}

bool sys_counter_irq_enabled(void)
{
    return system_counter.timer->DIER & TIM_DIER_UIE;
}

void sys_counter_irq_enable(void) {
	system_counter.timer->DIER |= TIM_DIER_UIE;
}

void sys_counter_irq_disable(void) {
	system_counter.timer->DIER &= ~TIM_DIER_UIE;
}

void sys_counter_set_running(bool running)
{
    if(running){
    	sys_counter_start();
    }else{
    	sys_counter_stop();
    }
}

bool sys_counter_running(void)
{
    return system_counter.timer->CR1 & TIM_CR1_CEN;
}

void sys_counter_start(void)
{
    system_counter.timer->CR1 |= TIM_CR1_CEN;
}

void sys_counter_stop(void)
{
    system_counter.timer->CR1 &= ~TIM_CR1_CEN;
}

void sys_counter_reset(void)
{
    system_counter.timer->CNT = 0;
}

__attribute__((noinline))
void sys_counter_value(struct timeval* tv)
{
    if(tv == NULL) return;
    
    // Сохраним флаг разрешения прерывания.
    bool irq_enabled = sys_counter_irq_enabled();
    // Запретим прерывание переполнения.
    sys_counter_irq_disable();
    
    // Считаем значение счётчиков.
    uint32_t cnt_us = system_counter.timer->CNT;
    uint32_t cnt_s = system_counter.counter;
    
    // Защита от переполнения.
    // Повторно считаем счётчик таймера.
    uint32_t cnt_us2 = system_counter.timer->CNT;
    
    // Если прерывание переполнения было разрешено.
    if(irq_enabled){
        // Вновь разрешим прерывание переполнения.
    	sys_counter_irq_enable();
    }
    
    // Если он меньше предыдущего значения - таймер переполнился.
    if(cnt_us2 < cnt_us){
        // Присвоим новое значение.
        cnt_us = cnt_us2;
        // И увеличим счётчик миллисекунд.
        cnt_s ++;
    }
    
    tv->tv_usec = cnt_us;
    tv->tv_sec = cnt_s;
}

void sys_counter_delay(uint32_t sec, uint32_t usec) {
	struct timeval tv_cur;
    struct timeval tv_dt;
    struct timeval tv_end;

    tv_dt.tv_sec = (time_t)sec;
    tv_dt.tv_usec = (suseconds_t)usec;

    sys_counter_value(&tv_cur);
    timeradd(&tv_cur, &tv_dt, &tv_end);

    for(;;){
		if(timercmp(&tv_cur, &tv_end, >=)){
			break;
		}
    	sys_counter_value(&tv_cur);
    }
}

void sys_counter_tv_print() {
	struct timeval tv;
	sys_counter_value(&tv);
	printf("[%u.%06u] ", (unsigned)tv.tv_sec, (unsigned)tv.tv_usec);
}

