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
#include "can/timer/can_timer.h"
#include "fmc/init/fmc_init.h"
#include "fmc/sram/fmc_sram.h"
#include "fmc/nand/fmc_nand.h"
#include "yaffs2/yaffsfs.h"
#include "yaffs2/yaffs_trace.h"
#include "interrupts/interrupt_priorities.h"

#include "sdio/sdio.h"

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

void SYS_CNT_IRQHANDLER(void) {
	sys_counter_irq_handler();
}

void MS_TIM_IRQHANDLER(void)
{
	CALC(ms_tim);
}

void SYS_TIM_IRQHANDLER(void)
{
	CALC(sys_tim);
}

void CAN_TIM_IRQHANDLER(void) {
	CALC(can_tim);
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




void dma_rcc_init() {
	//DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
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

	usart6_nvic_init(UART6_IRQ_PRIO);
	usart6_init(); //Socket3
	printf("SystemCoreClock: %luMHz\n", SystemCoreClock/1000000);
	sys_counter_tv_print();
	printf("UART6 initialized\n");

	spi2_nvic_init(SPI2_IRQ_PRIO);
	spi2_bus_init(); //dac7562
	sys_counter_tv_print();
	printf("SPI2 initialized\n");

	spi4_nvic_init(SPI4_IRQ_PRIO);
	spi4_bus_init(); //tic12400,ncv7608
	sys_counter_tv_print();
	printf("SPI4 initialized\n");

	usart3_nvic_init(UART3_IRQ_PRIO);
	usart3_init(); //RS485_1
	sys_counter_tv_print();
	printf("UART3 initialized\n");

	modbus1_init(); //RS485_1
	sys_counter_tv_print();
	printf("MODBUS1 initialized\n");

	uart7_nvic_init(UART7_IRQ_PRIO);
	uart7_init(); //RS485_Panel
	sys_counter_tv_print();
	printf("UART7 initialized\n");

	modbus_panel_init(); //RS485_Panel
	sys_counter_tv_print();
	printf("MODBUS Panel initialized\n");

	can1_nvic_init(CAN1_IRQ_PRIO);
	can1_init();
	sys_counter_tv_print();
	printf("CAN1 initialized\n");

	//SDIO!
	gpio_sdio_cfg_setup();
	sys_counter_tv_print();
	if(gpio_input_bit_read(&GPI_SDIO_CD_App) == false) {
		printf("SD Card Inserted\n");
		gpio_output_bit_setup(&GPO_SDIO_PWR_App, DISABLE);
		sys_counter_delay(0, 300000); // 300ms
		RCC->APB2ENR |= RCC_APB2ENR_SDIOEN;
	} else {
		printf("SD Card Not Inserted\n");
	}

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
