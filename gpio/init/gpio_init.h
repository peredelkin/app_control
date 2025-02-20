/*
 * gpio_init.h
 *
 *  Created on: 31 авг. 2022 г.
 *      Author: Ruslan
 */

#ifndef INC_GPIO_INIT_H_
#define INC_GPIO_INIT_H_

#include "../gpio.h"

extern void gpio_rcc_init();

extern const gpio_pin_t GPO_OE_App;
extern void gpio_socket3_cfg_setup();

extern void gpio_spi2_cfg_setup();

extern void gpio_spi4_cfg_setup();

extern void gpio_spi5_cfg_setup();

extern void gpio_uart3_cfg_setup();

extern void gpio_uart6_cfg_setup();

extern void gpio_uart7_cfg_setup();

extern void gpio_can1_cfg_setup();

extern const gpio_pin_t GPIO_SRAM_nCE;
extern const gpio_pin_t GPIO_NAND_nCE;
extern const gpio_pin_t GPI_NAND_RnB;
extern const gpio_pin_t GPO_NAND_nWP;
extern void gpio_fmc_cfg_setup();

extern const gpio_pin_t gpio_rs485_panel_detect;
extern void rs485_panel_set_out();
extern void rs485_panel_set_in();
extern void gpio_rs485_panel_cfg_setup();

extern void rs485_1_set_out();
extern void rs485_1_set_in();
extern void gpio_rs485_1_cfg_setup();

extern const gpio_pin_t GPO_CS_DI_App;
extern const gpio_pin_t GPO_Reset_DI_App;
extern const gpio_pin_t GPI_Int_DI_App;
extern void gpio_tic12400_cfg_setup();

extern const gpio_pin_t GPO_CS_DO_App;
extern const gpio_pin_t GPO_EN_DO_App;
extern void gpio_ncv7608_cfg_setup();

extern const gpio_pin_t GPI_EmStop_App;
extern void gpio_em_stop_cfg_setup();

extern const gpio_pin_t GPI_AC_lost_App;
extern const gpio_pin_t GPI_Lost_5V_App;
extern void gpio_ac_dc_lost_cfg_setup();

extern const gpio_pin_t GPO_Relay_DO_App[];
extern void gpio_relay_output_cfg_setup();

extern const gpio_pin_t GPO_RGB_LED_App[];
extern void gpio_rgb_led_cfg_setup();

extern const gpio_pin_t GPO_nCLR_App;
extern const gpio_pin_t GPO_OutDis_I_AO_App;
extern const gpio_pin_t GPI_ErrorAO1_App;
extern const gpio_pin_t GPI_ErrorAO2_App;
extern const gpio_pin_t GPO_SYNCAO_App;
extern const gpio_pin_t GPO_nLDAC_App;
extern void gpio_dac7562_cfg_setup();

extern const gpio_pin_t GPO_SDIO_PWR_App;
extern const gpio_pin_t GPI_SDIO_CD_App;
extern void gpio_sdio_cfg_setup();

#endif /* INC_GPIO_INIT_H_ */
