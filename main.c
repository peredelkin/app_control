#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "modules/modules.h"
#include "reg/regs.h"

#include "init/init.h"

#include "gpio/init/gpio_init.h"
#include "spi/init/spi_init.h"
#include "uart/init/uart_init.h"
#include "modbus/init/modbus_init.h"
#include "eth/init/eth_init.h"
#include "can/init/can_init.h"
#include "fmc/init/fmc_init.h"
#include "fmc/sram/fmc_sram.h"
#include "fmc/nand/fmc_nand.h"
#include "yaffs2/yaffsfs.h"
#include "yaffs2/yaffs_trace.h"

#include <malloc.h>
#include <unistd.h>

/*
void dumb_delay_05s(void) {
	for(int i = 90000000; i > 0; i--) {
		__NOP();
	}
}

void Default_Handler(void) {
	rgb_led.in = RGB_LED_COLOR_RED;
	CALC(rgb_led);
	dumb_delay_05s();
	rgb_led.in = RGB_LED_COLOR_BLACK;
	CALC(rgb_led);
	dumb_delay_05s();
}
*/

void SYS_TIM_IRQHANDLER(void)
{
	CALC(sys_tim);
}

void MS_TIM_IRQHANDLER(void)
{
	CALC(ms_tim);
}

void SYS_CNT_IRQHANDLER(void) {
	sys_counter_irq_handler();
}

//TODO: просмотреть все функции, в которых регистры модифицируются через битовые поля
/*
 * TODO: добавить функцию проверки внешних подтягивающих резисторов
 * через изменение внутренних подтяжек ног контроллера с
 * последующим чтением состояния входов:
 * если при изменении внутренней подтяжки изменяется состояние входа,
 * вывод не подтянут;
 * и т.д. .
 */

//__attribute__((section(".extsram"), used))
//volatile int test_extsram;

void system_counter_init(void) {
	sys_counter_init(SYS_CNT_TIM);
	sys_counter_irq_enable();
	NVIC_SetPriority(SYS_CNT_IRQN, SYS_CNT_IRQ_PRIO);
	NVIC_EnableIRQ(SYS_CNT_IRQN);
	sys_counter_start();
}


void dma_rcc_init() {
	//DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
}

typedef struct _RCC_CR_BIT{
	unsigned hsi_on :1;			/*0		HSION*/
	unsigned hsi_rdy :1;		/*1		HSIRDY*/
	unsigned reserved_2 :1;		/*2		Reserved*/
	unsigned hsi_trim :5;		/*7:3	HSITRIM[4:0]*/
	unsigned hsi_cal :8;		/*15:8	HSICAL[7:0]*/
	unsigned hse_on: 1;			/*16	HSEON*/
	unsigned hse_rdy :1;		/*17	HSERDY*/
	unsigned hse_byp :1;		/*18	HSEBYP*/
	unsigned css_on :1;			/*19	CSSON*/
	unsigned reserved_20_23 :4;	/*23:20 Reserved*/
	unsigned pll_on :1;			/*24	PLLON*/
	unsigned pll_rdy :1;		/*25	PLLRDY*/
	unsigned pll_i2s_on :1;		/*26	PLLI2SON*/
	unsigned pll_i2s_rdy :1;	/*27	PLLI2SRDY*/
	unsigned pll_sai_on :1;		/*28	PLLSAION:*/
	unsigned pll_sai_rdy :1;	/*29	PLLSAIRDY*/
	unsigned reserved_30_31 :2;	/*31:30	Reserved*/
} rcc_cr_bit_t;

typedef union {
	uint32_t all;
	struct _RCC_CR_BIT bit;
} rcc_cr_reg_t;

typedef struct _RCC_PLLCFGR_BIT {
	unsigned pll_m :6;
	unsigned pll_n :9;
	unsigned reserved_15: 1;
	unsigned pll_p :2;
	unsigned reserved_18_21 :4;
	unsigned pll_src :1;
	unsigned reserved_23 :1;
	unsigned pll_q :4;
	unsigned reserved_28_31 :4;
} rcc_pllcfgr_bit_t;

typedef union {
	uint32_t all;
	struct _RCC_PLLCFGR_BIT bit;
} rcc_pllcfgr_reg_t;

void print_rcc_info() {
	rcc_cr_reg_t rcc_cr;
	rcc_cr.all = RCC->CR;

	printf("HSION %i, ", rcc_cr.bit.hsi_on);
	printf("HSIRDY %i, ", rcc_cr.bit.hsi_rdy);

	printf("HSITRIM %i, ", rcc_cr.bit.hsi_trim);
	printf("HSICAL %i, ", rcc_cr.bit.hsi_cal);

	printf("HSEON %i, ", rcc_cr.bit.hse_on);
	printf("HSERDY %i, ", rcc_cr.bit.hse_rdy);
	printf("HSEBYP %i, ", rcc_cr.bit.hse_byp);

	printf("CSSON %i, ", rcc_cr.bit.css_on);

	printf("PLLON %i, ", rcc_cr.bit.pll_on);
	printf("PLLRDY %i, ", rcc_cr.bit.pll_rdy);

	printf("PLLI2SON %i, ", rcc_cr.bit.pll_i2s_on);
	printf("PLLI2SRDY %i, ", rcc_cr.bit.pll_i2s_rdy);

	printf("PLLSAION %i, ", rcc_cr.bit.pll_sai_on);
	printf("PLLSAIRDY %i\n", rcc_cr.bit.pll_sai_rdy);

	rcc_pllcfgr_reg_t rcc_pllcfgr;
	rcc_pllcfgr.all = RCC->PLLCFGR;

	printf("PLLM %i, ", rcc_pllcfgr.bit.pll_m);
	printf("PLLN %i, ", rcc_pllcfgr.bit.pll_n);
	printf("PLLP %i, ", rcc_pllcfgr.bit.pll_p);
	printf("PLLSRC %i, ", rcc_pllcfgr.bit.pll_src);
	printf("PLLQ %i\n", rcc_pllcfgr.bit.pll_q);
}

int main(void)
{

	//yaffs_trace_mask |= YAFFS_TRACE_MOUNT;
	//yaffs_trace_mask |= YAFFS_TRACE_ALWAYS;
	//yaffs_trace_mask |= YAFFS_TRACE_NANDACCESS;
	//yaffs_trace_mask |= YAFFS_TRACE_VERIFY_ALL;
	//yaffs_trace_mask |= YAFFS_TRACE_ERROR;

	NVIC_SetPriorityGrouping(0b000);

	gpio_rcc_init();
	dma_rcc_init();

	system_counter_init(); //TIM2

	gpio_socket3_cfg_setup(); //OE_App
	gpio_output_bit_setup(&GPO_OE_App, GPIO_STATE_OFF); //Socket 3 Enable

	usart6_nvic_init(5);
	usart6_init(); //Socket3
	print_rcc_info(); //RCC Info!
	sys_counter_tv_print();
	printf("UART6 initialized\n");

	spi2_nvic_init(4);
	spi2_bus_init(); //dac7562
	sys_counter_tv_print();
	printf("SPI2 initialized\n");

	spi4_nvic_init(4);
	spi4_bus_init(); //tic12400,ncv7608
	sys_counter_tv_print();
	printf("SPI4 initialized\n");

	usart3_nvic_init(5);
	usart3_init(); //RS485_1
	sys_counter_tv_print();
	printf("UART3 initialized\n");

	modbus1_init(); //RS485_1
	sys_counter_tv_print();
	printf("MODBUS1 initialized\n");

	uart7_nvic_init(5);
	uart7_init(); //RS485_Panel
	sys_counter_tv_print();
	printf("UART7 initialized\n");

	modbus_panel_init(); //RS485_Panel
	sys_counter_tv_print();
	printf("MODBUS Panel initialized\n");

	can1_nvic_init(5);
	can1_init();
	sys_counter_tv_print();
	printf("CAN1 initialized\n");

	//FMC, SRAM, NAND, YAFFS2
	int res;
	if(fmc_init() == E_NO_ERROR) {
		res = yaffs_start_up();
		sys_counter_tv_print();
		printf("YAFFS Start Up: %d\n", res);
		res = yaffs_mount("/nand");
		sys_counter_tv_print();
		printf("YAFFS Mount: %d\n", res);
	}

	//eth_init(); //отпаяно

	//loadsettings();

    // Blink.
	//dlog.p_ch[31].reg_id = REG_ID_BLINK_Q;
	//dlog.p_ch[31].enabled = 1;

	//dlog.control = CONTROL_ENABLE;

    INIT(sys);

//    if(sys.status & SYS_MAIN_STATUS_ERROR){
//        //printf("Error init main system!\n");
//        DEINIT(sys);
//
//        return 0;
//    }

    for(;;){
    	IDLE(sys);
    }

    //dlog.control = CONTROL_NONE;

    //DEINIT(sys);
    return 0;
}
