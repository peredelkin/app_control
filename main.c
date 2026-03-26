#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#include "modules/modules.h"
#include "reg/regs.h"

#include "init/init.h"

#include "gpio/init/gpio_init.h"
#include "spi/init/spi_init.h"
#include "spi/settings/spi_settings.h"
#include "uart/init/uart_init.h"
#include "modbus/init/modbus_init.h"
#include "eth/init/eth_init.h"
#include "can/init/can_init.h"
#include "can/timer/can_timer.h"
#include "fmc/init/fmc_init.h"
#include "fmc/sram/fmc_sram.h"
#include "fmc/nand/fmc_nand.h"
#include "fmc/yaffs2/yaffsfs.h"
#include "fmc/yaffs2/yaffs_trace.h"
#include "interrupts/interrupt_priorities.h"
#include "tft9341/tft9341.h"
#include "tft9341/tft9341_cache.h"
#include "tft9341/tft9341_cache_vbuf.h"
#include "graphics/graphics.h"
#include "graphics/painter.h"
#include "graphics/font_5x8_utf8.h"
#include "graphics/font_10x16_utf8.h"

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

//!FATFS nad SDCARD
sdcard_t sdcard;

#define FATFS_DISK_PATH_LEN 4
char* disk_path = "/SD"; //[FATFS_DISK_PATH_LEN];
FATFS sdcard_fatfs;

//TODO: заглушка для fatfs
DWORD get_fattime (void) {
	return 0;
}

TCHAR buff[256];

//void sdcard_ls_dir(const char* dirname)
//{
//	FRESULT res = FR_OK;
//	DIR dp;
//	FILINFO fno;
//	FIL fp;
//	FSIZE_t total_tize = 0;
//	const char* file_type = ".txt";
//	const char* SPLIT = "SPLIT";
//	char* strstr_res = NULL;
//	char* f_gets_res = NULL;
//
//	res = f_opendir(&dp, dirname);
//	if(res != FR_OK){
//		printf("Error %d open dir: %s\n", res, dirname);
//		return;
//	}
//
//	for(;;){
//		res = f_readdir(&dp, &fno);
//
//		if(res != FR_OK || fno.fname[0] == 0){
//			printf("Total Size: %0.1f kB\n", ((float)total_tize)/1024);
//			printf("No More Files\n");
//			break;
//		}
//
//		if(fno.fattrib & AM_DIR) {
//			printf("Dir: %s\n", fno.fname);
//		} else {
//			printf("File: %s, Size: %0.1f kB, ", fno.fname, ((float)fno.fsize)/1024);
//
//			total_tize += fno.fsize;
//
//			strstr_res = strstr(fno.fname, file_type);
//
//			printf("Type Is");
//
//			if(strstr_res != NULL) {
//				printf(": %s\n", file_type);
//
//				res = f_open(&fp, fno.fname, FA_READ);
//
//				if (res != FR_OK) {
//					printf("Error %d Open File: %s\n", res, fno.fname);
//					break;
//				}
//
//				while((f_gets_res = f_gets(buff, sizeof buff, &fp)) != NULL) {
//					strstr_res = strstr(buff, SPLIT);
//					if(strstr_res != NULL) {
//						strstr_res[0] = 0;
//						strstr_res[1] = 0;
//						strstr_res[2] = 0;
//						strstr_res[3] = 0;
//						strstr_res[4] = 0;
//						printf("%s", buff);
//						sys_counter_delay(0, 68000); // 100ms
//						printf("%s", "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
//						printf("%s", &strstr_res[5]);
//					} else {
//						printf("%s", buff);
//					}
//				}
//
//				f_close(&fp);
//
//			} else {
//				printf(" Not: %s\n", file_type);
//			}
//		}
//	}
//
//	f_closedir(&dp);
//}

void sdcard_ls_dir(const char* dirname)
{
	FRESULT res = FR_OK;
	DIR dp;
	FILINFO fno;
	FIL fp;
	FSIZE_t total_tize = 0;
	const char* file_type = ".txt";
	char* strstr_res = NULL;

	res = f_opendir(&dp, dirname);
	if(res != FR_OK){
		printf("Error %d open dir: %s\n", res, dirname);
		return;
	}

	for(;;){
		res = f_readdir(&dp, &fno);

		if(res != FR_OK || fno.fname[0] == 0){
			printf("Total Size: %0.1f kB\n", ((float)total_tize)/1024);
			printf("No More Files\n");
			break;
		}

		if(fno.fattrib & AM_DIR) {
			printf("Dir: %s\n", fno.fname);
		} else {
			printf("File: %s, Size: %0.1f kB, ", fno.fname, ((float)fno.fsize)/1024);

			total_tize += fno.fsize;

			strstr_res = strstr(fno.fname, file_type);

			printf("Type Is");

			if(strstr_res != NULL) {
				printf(": %s\n", file_type);

				res = f_open(&fp, fno.fname, FA_READ);

				if (res != FR_OK) {
					printf("Error %d Open File: %s\n", res, fno.fname);
					break;
				}

				//Begin

				//End

				f_close(&fp);

			} else {
				printf(" Not: %s\n", file_type);
			}
		}
	}

	f_closedir(&dp);
}

/* TFT BEGIN */
tft9341_t tft;
#define TFT_PIXEL_SIZE 2
#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define TFT_CACHE_BUFS_COUNT 2
#define TFT_CACHE_BUF_PIXELS 320
#define TFT_CACHE_BUF_SIZE (TFT_CACHE_BUF_PIXELS * TFT_PIXEL_SIZE)

static uint8_t tft_cache_buf_data0[TFT_CACHE_BUF_SIZE];
static uint8_t tft_cache_buf_data1[TFT_CACHE_BUF_SIZE];

static tft9341_cache_buffer_t tft_cache_bufs[TFT_CACHE_BUFS_COUNT] = {
    make_tft9341_cache_buffer(tft_cache_buf_data0, TFT_CACHE_BUF_SIZE),
    make_tft9341_cache_buffer(tft_cache_buf_data1, TFT_CACHE_BUF_SIZE)
};
static tft9341_cache_t tft_cache = make_tft9341_cache(&tft, TFT_PIXEL_SIZE, tft_cache_bufs, TFT_CACHE_BUFS_COUNT, TFT9341_ROW_COL_NORMAL_MODE);

//static bool tft_vbuf_set_pixel(graphics_t* graphics, graphics_pos_t x, graphics_pos_t y, graphics_color_t color);

static graphics_vbuf_t graph_vbuf = make_tft9341_cache_vbuf();//make_graphics_vbuf(NULL, tft_vbuf_set_pixel, NULL, NULL, NULL, NULL, NULL)));
static graphics_t graphics = make_graphics_virtual(&tft_cache, TFT_WIDTH, TFT_HEIGHT, GRAPHICS_FORMAT_RGB_565, &graph_vbuf);
static painter_t painter = make_painter(&graphics);
static const font_bitmap_t font_5x8_utf8_bitmaps[] = {
    make_font_bitmap(32, 127, font_5x8_utf8_part0_data, FONT_5X8_UTF8_PART0_WIDTH, FONT_5X8_UTF8_PART0_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_font_bitmap(0xb0, 0xb0, font_5x8_utf8_part1_data, FONT_5X8_UTF8_PART1_WIDTH, FONT_5X8_UTF8_PART1_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_font_bitmap(0x400, 0x451, font_5x8_utf8_part2_data, FONT_5X8_UTF8_PART2_WIDTH, FONT_5X8_UTF8_PART2_HEIGHT, GRAPHICS_FORMAT_BW_1_V)
};
static font_t font5x8 = make_font(font_5x8_utf8_bitmaps, 3, 5, 8, 1, 0);

const font_bitmap_t font_10x16_utf8_bitmaps[] = {
    make_font_bitmap(32, 127, font_10x16_utf8_part0_data, FONT_10X16_UTF8_PART0_WIDTH, FONT_10X16_UTF8_PART0_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_font_bitmap(0xb0, 0xb0, font_10x16_utf8_part1_data, FONT_10X16_UTF8_PART1_WIDTH, FONT_10X16_UTF8_PART1_HEIGHT, GRAPHICS_FORMAT_BW_1_V),
    make_font_bitmap(0x400, 0x451, font_10x16_utf8_part2_data, FONT_10X16_UTF8_PART2_WIDTH, FONT_10X16_UTF8_PART2_HEIGHT, GRAPHICS_FORMAT_BW_1_V)
};
static font_t font10x16 = make_font(font_10x16_utf8_bitmaps, 3, 10, 16, 1, 0);
/* TFT END */

void init_tft(void) {
	gpio_ili9341_cfg_setup();

	tft9341_init_t is;

	is.spi = &SPI5_Bus;
	is.transfer_id = TFT9341_DEFAULT_TRANSFER_ID;
	is.dc_pin = &GPO_Board_Out3_App;
	is.reset_pin = &GPO_Board_Out4_App;

	tft9341_init(&tft, &is);

	tft9341_reset(&tft);

	tft9341_madctl_t madctl;
	madctl.row_address_order = TFT9341_ROW_TOP_TO_BOTTOM;
	madctl.col_address_order = TFT9341_COL_RIGHT_TO_LEFT;
	madctl.row_col_exchange = TFT9341_ROW_COL_NORMAL_MODE;
	madctl.vertical_refresh = TFT9341_REFRESH_TOP_TO_BOTTOM;
	madctl.horizontal_refresh = TFT9341_REFRESH_LEFT_TO_RIGHT;
	madctl.color_order = TFT9341_COLOR_ORDER_BGR;

	spi_bus_open(tft.spi, &spi_ili9341_cfg);

	tft9341_set_madctl(&tft, &madctl);
	tft9341_set_pixel_format(&tft, TFT9341_PIXEL_16BIT, TFT9341_PIXEL_16BIT);
	tft9341_sleep_out(&tft);
	tft9341_display_on(&tft);

	painter_set_brush(&painter, PAINTER_BRUSH_SOLID);
	painter_set_brush_color(&painter, TFT9341_MAKE_RGB565(255, 0, 255));
	painter_set_pen_color(&painter, TFT9341_MAKE_RGB565(0, 255, 0));
	painter_draw_fillrect(&painter, 0, 0, 239, 319);
	painter_draw_circle(&painter, 150, 150, 50);
	painter_set_font(&painter, &font5x8);
	painter_set_source_image_mode(&painter, PAINTER_SOURCE_IMAGE_MODE_BITMAP);
	painter_draw_string(&painter, 50, 50, "Bla bla bla!");
	painter_flush(&painter);

	spi_bus_close(tft.spi);
}

int main(void)
{

	//yaffs_trace_mask |= YAFFS_TRACE_MOUNT;
	//yaffs_trace_mask |= YAFFS_TRACE_ALWAYS;
	//yaffs_trace_mask |= YAFFS_TRACE_NANDACCESS;
	//yaffs_trace_mask |= YAFFS_TRACE_VERIFY_ALL;
	//yaffs_trace_mask |= YAFFS_TRACE_ERROR;

	NVIC_SetPriorityGrouping(0b000);

	gpio_rcc_init(); //RCC of all GPIO

	dma_controller_init(); //DMA1 and DMA2 struct init

	system_counter_init(); //TIM2

	gpio_socket3_cfg_setup(); //OE_App

	usart6_nvic_init(UART6_IRQ_PRIO);
	usart6_init(); //Socket3
	printf("SysCoreClock: %uMHz\n", (unsigned)(SystemCoreClock/1000000));
	sys_counter_tv_print();
	printf("UART6\n");

	//FMC, SRAM, NAND, YAFFS2
	int res;
	if(fmc_init() == E_NO_ERROR) {
		res = yaffs_start_up();
		sys_counter_tv_print();
		printf("Mount /nand ");
		res = yaffs_mount("/nand");
		if(res == 0) {
			printf("[OK]\n");
		} else {
			printf("[ERROR]\n");
		}
	}

	spi2_nvic_init(SPI2_IRQ_PRIO);
	spi2_bus_init(); //dac7562
	sys_counter_tv_print();
	printf("SPI2\n");

	spi4_nvic_init(SPI4_IRQ_PRIO);
	spi4_bus_init(); //tic12400,ncv7608
	sys_counter_tv_print();
	printf("SPI4\n");

	spi5_nvic_init(SPI5_IRQ_PRIO);
	spi5_bus_init(); //Socket3
	sys_counter_tv_print();
	printf("SPI5\n");

	usart3_dma_nvic_init(UART3_DMA_Stream_IRQ_PRIO);
	usart3_nvic_init(UART3_IRQ_PRIO);
	usart3_init(); //RS485_1
	sys_counter_tv_print();
	printf("UART3\n");

	modbus1_init(); //RS485_1
	sys_counter_tv_print();
	printf("MODBUS 1\n");

	uart7_nvic_init(UART7_IRQ_PRIO);
	uart7_init(); //RS485_Panel
	sys_counter_tv_print();
	printf("UART7\n");

	modbus_panel_init(); //RS485_Panel
	sys_counter_tv_print();
	printf("MODBUS Panel\n");

	can1_nvic_init(CAN1_IRQ_PRIO);
	can2_nvic_init(CAN2_IRQ_PRIO);
	can_canopen_init();
	sys_counter_tv_print();
	printf("CAN 1/2\n");

	//init_tft();
	//sys_counter_tv_print();
	//printf("TFT\n");

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
	FRESULT fatfs_result = FR_OK;

	for (;;) {
		IDLE(sys);

		if (sdcard_card_detect(&sdcard)) {
			if (sdcard.inserted == false) {
				sdcard.inserted = true;

				sdcard_setup_diskio(&sdcard, 1);

				sys_counter_tv_print();
				printf("SD Card Inserted\n");

				sdio_enable();

				sdcard_card_pwr_on(&sdcard);

				sdcard_init_err = sdcard_card_init(&sdcard);

				if (sdcard_init_err == E_NO_ERROR) {

					printf("CARD TYPE: ");
					switch(sdcard.type) {
					case SDCARD_TYPE_UNKNOWN:
						printf("UNKNOWN\n");
						break;
					case SDCARD_TYPE_SC:
						printf("SDSC\n");
						break;
					case SDCARD_TYPE_HC_XC:
						printf("SDHC or SDXC\n");
						break;
					case SDCARD_TYPE_UC:
						printf("SDUC\n");
						break;
					}

					printf("CAPACITY: %llu MB\n", (sdcard.CSD.capacity/(1024*1024)));

					fatfs_result = f_mount(&sdcard_fatfs, disk_path, 0);
					printf("FATFS Mout Result: %d\n", fatfs_result);

					if(fatfs_result == FR_OK) {
						sdcard_ls_dir(disk_path);
					}else{
						f_unmount(disk_path);
					}

				} else {
					printf("SD Card Init Error: %u\n", (unsigned)sdcard_init_err);
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
