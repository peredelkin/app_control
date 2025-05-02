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

sdcard_t sdcard; //TODO: не забыть убрать

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

	//SDIO!
	gpio_sdio_cfg_setup();
	sys_counter_tv_print();
	if (gpio_input_bit_read(&GPI_SDIO_CD_App) == false) {
		printf("SD Card Inserted\n");
		gpio_output_bit_setup(&GPO_SDIO_PWR_App, DISABLE);
		sys_counter_delay(0, 300000); // 300ms

		RCC->APB2ENR |= RCC_APB2ENR_SDIOEN;
		sdio_clock_control(126, SDIO_CLKCR_CLK_EN, SDIO_CLKCR_PWRSAV_ENA, SDIO_CLKCR_BYP_DIS);
		sdio_power_control(SDIO_POWER_PWRCTRL_ON);

		err_t sdio_err = E_NO_ERROR;
		//инициализация структуры sdcard
		sdcard.cmd = NULL;
		sdcard.current_state = SDCARD_STATE_IDLE;

		//CMD0
		sdio_err = sdcard_cmd(&sdcard, &sdcard_Class0_CMD0, 0);
		if(sdio_err != E_NO_ERROR) {
			printf("CMD 0 Err: %d\n", sdio_err);
			goto exit_sdcard_init;
		}

		printf("CMD 0 STATE: %d\n", sdcard.current_state);

		//CMD8
		sdio_err = sdcard_cmd(&sdcard, &sdcard_Class0_CMD8, (0b1 << 8));
		if(sdio_err != E_NO_ERROR) {
			printf("CMD 8 Err: %d\n", sdio_err);
			goto exit_sdcard_init;
		}

		printf("CMD 8 STATE: %d\n", sdcard.current_state);

		//ACMD41 without argument
		sdio_err = sdcard_acmd(&sdcard, &sdcard_Class8_ACMD41, 0);
		if (sdio_err != E_NO_ERROR) {
			printf("ACMD 41 Err: %d\n", sdio_err);
			goto exit_sdcard_init;
		}

		//ACMD41 with argument
		uint8_t acmd41_timeout = 11; //11 * 100ms
		do {

			sdio_err = sdcard_acmd(&sdcard, &sdcard_Class8_ACMD41, ((0b1 << 30) | (0b11 << 20)));
			if (sdio_err != E_NO_ERROR) {
				printf("ACMD 41 Err: %d\n", sdio_err);
				goto exit_sdcard_init;
			}

			sys_counter_delay(0, 100000); // 100ms

			acmd41_timeout--;

		} while (sdcard.response.r3.bit.CARD_POWER_UP_STATUS == 0 && acmd41_timeout);

		if(acmd41_timeout == 0) {
			printf("ACMD 41 TIMEOUT\n");
			goto exit_sdcard_init;
		}

		sdio_err = sdcard_change_current_state(&sdcard);
		if (sdio_err != E_NO_ERROR) {
			printf("STATE 41 Err: %d\n", sdio_err);
			goto exit_sdcard_init;
		}

		//TODO: определить тип карты по ответу ACMD41
		printf("ACMD 41 STATE: %d\n", sdcard.current_state);

		//CMD2
		sdio_err = sdcard_cmd(&sdcard, &sdcard_Class0_CMD2, 0);
		if (sdio_err != E_NO_ERROR) {
			printf("CMD 2 Err: %d\n", sdio_err);
			goto exit_sdcard_init;
		}

		printf("CMD 2 STATE: %d\n", sdcard.current_state);

		//CMD3
		sdio_err = sdcard_cmd(&sdcard, &sdcard_Class0_CMD3, 0);
		if(sdio_err != E_NO_ERROR) {
			printf("CMD 3 Err: %d\n", sdio_err);
			goto exit_sdcard_init;
		}

		printf("CMD 3 STATE: %d\n", sdcard.current_state);

		//NEW_RCA
		sdcard.RCA = sdcard.response.r6.all & 0xFFFF0000;

		//CMD9
		sdio_err = sdcard_cmd(&sdcard, &sdcard_Class0_CMD9, sdcard.RCA);
		if (sdio_err != E_NO_ERROR) {
			printf("CMD 9 Err: %d\n", sdio_err);
			goto exit_sdcard_init;
		}

		sdcard_CSD_fill(&sdcard);

		printf("CSD Version: %d\n", sdcard.CSD.v1.bit.CSD_STRUCTURE);

		printf("CMD 9 STATE: %d\n", sdcard.current_state);

		//CMD10
		sdio_err = sdcard_cmd(&sdcard, &sdcard_Class0_CMD10, sdcard.RCA);
		if (sdio_err != E_NO_ERROR) {
			printf("CMD 10 Err: %d\n", sdio_err);
			goto exit_sdcard_init;
		}

		sdcard.CID.all[0] = sdcard.response.r2.all[0];
		sdcard.CID.all[1] = sdcard.response.r2.all[1];
		sdcard.CID.all[2] = sdcard.response.r2.all[2];
		sdcard.CID.all[3] = sdcard.response.r2.all[3];

		printf("MID: %d\n", sdcard.CID.bit.MID);
		printf("MDT_M: %d, MDT_Y: %d\n", sdcard.CID.bit.MDT_M, sdcard.CID.bit.MDT_Y);

		printf("CMD 10 STATE: %d\n", sdcard.current_state);

		//CMD7
		sdio_err = sdcard_cmd(&sdcard, &sdcard_Class0_CMD7_adressed, sdcard.RCA);
		if (sdio_err != E_NO_ERROR) {
			printf("CMD 7 Err: %d\n", sdio_err);
			goto exit_sdcard_init;
		}

		printf("CMD 7 STATE: %d\n", sdcard.current_state);

	} else {
		printf("SD Card Not Inserted\n");
	}

	exit_sdcard_init: ;

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
