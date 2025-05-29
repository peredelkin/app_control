#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

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

#define USE_SDCARD_FATFS_DISKIO
#include "sdcard/sdcard.h"

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

//!FATFS
FATFS sdcard_fatfs;
sdcard_t sdcard;
uint8_t sdcard_data_array[1024] = "Hello world!";
char hex_to_str[17];

void dma_rcc_init() {
	//DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
}

void dma_enable_irq() {
	//UART3 RX
	NVIC_SetPriority(DMA1_Stream1_IRQn, DMA1_Stream1_IRQ_PRIO);
	NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	//UART3 TX
	NVIC_SetPriority(DMA1_Stream3_IRQn, DMA1_Stream3_IRQ_PRIO);
	NVIC_EnableIRQ(DMA1_Stream3_IRQn);
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
	dma_controller_init();
	dma_enable_irq();

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

	//Init SDIO!
	gpio_sdio_cfg_setup();

	sdcard.dma_stream = DMA2_Stream_6;

	sdcard.gpio.dat0 = &SDIO_DAT0_App;
	sdcard.gpio.pwr = &GPO_SDIO_PWR_App;
	sdcard.gpio.cd = &GPI_SDIO_CD_App;

	sdcard.inserted = false;

	err_t sdcard_init_err = E_NO_ERROR;

	for (;;) {
		IDLE(sys);

		if (sdcard_card_detect(&sdcard)) {
			if (sdcard.inserted == false) {
				sdcard.inserted = true;

				sys_counter_tv_print();
				printf("SD Card Inserted\n");

				sdio_enable();

				sdcard_card_pwr_on(&sdcard);

				sdcard_init_err = sdcard_card_init(&sdcard);

				if (sdcard_init_err == E_NO_ERROR) {

					for (uint64_t block_addr = 0; block_addr < 8; block_addr += 2) {
						sdcard_init_err = sdcard_read(&sdcard, ((uint32_t*) sdcard_data_array),
								block_addr, 2, 0xFFFFFF);
						if (sdcard_init_err != E_NO_ERROR) {
							printf("READ Err: %lu\n", sdcard_init_err);
							break;
						}

						for (int i = 0; i < 1024; i += 16) {
							memcpy(hex_to_str, &sdcard_data_array[i], 16);
							hex_to_str[16] = 0;

							for (int ch = 0; ch < 16; ch++) {
								if (isprint((int)hex_to_str[ch]) == 0) hex_to_str[ch] = '.';
							}

							printf(
									"%08llx %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %s\n",
									((block_addr * 512) + i), sdcard_data_array[i],
									sdcard_data_array[i + 1], sdcard_data_array[i + 2],
									sdcard_data_array[i + 3], sdcard_data_array[i + 4],
									sdcard_data_array[i + 5], sdcard_data_array[i + 6],
									sdcard_data_array[i + 7], sdcard_data_array[i + 8],
									sdcard_data_array[i + 9], sdcard_data_array[i + 10],
									sdcard_data_array[i + 11], sdcard_data_array[i + 12],
									sdcard_data_array[i + 13], sdcard_data_array[i + 14],
									sdcard_data_array[i + 15], hex_to_str);
							sys_counter_delay(0, 1000); // 1ms
						}
					}

				} else {
					printf("SD Card Init Error: %lu\n", sdcard_init_err);
				}
			}
		} else {
			if (sdcard.inserted == true) {
				sdcard.inserted = false;
				sdcard_sdio_power_off();

				sdcard_card_pwr_off(&sdcard);

				sdio_disable();

				sys_counter_tv_print();
				printf("SD Card removed\n");
			}
		}
	}

    //dlog.control = CONTROL_NONE;

    //DEINIT(sys);
    return 0;
}
