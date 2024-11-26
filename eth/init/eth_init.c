/*
 * eth_init.c
 *
 *  Created on: 5 мар. 2024 г.
 *      Author: Ruslan
 */

#include "eth_init.h"
#include "sys/counter/sys_counter.h"

const gpio_pin_t GPO_ETH_Reset_App = GPIO_PIN(GPIOH, GPIO_PIN_2); /*43, PH2, GPIO_Output, ETH_Reset_App*/

#define GPIO_ETH_RST_COUNT 1
const gpio_pin_cfg_t gpio_eth_rst_cfg[GPIO_ETH_RST_COUNT] = {
		GPIO_PIN_CFG(GPIOH, GPIO_PIN_2, GPIO_MODE_OUT, GPIO_OTYPE_OD, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_0, GPIO_STATE_OFF),	/*43, PH2,		GPIO_Output,	ON,		ETH_Reset_App*/
};

#define ETH_GPIO_COUNT 9
const gpio_pin_cfg_t eth_gpio_cfg[ETH_GPIO_COUNT] = {
		GPIO_PIN_CFG(GPIOC, GPIO_PIN_1, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_11, GPIO_STATE_OFF),	/*33, PC1,		ETH_MDC,		OFF,		ETH_MDC_App*/
		GPIO_PIN_CFG(GPIOA, GPIO_PIN_1, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_11, GPIO_STATE_OFF),	/*41, PA1,		ETH_REF_CLK,	OFF,		ETH_OSCIN_App*/
		GPIO_PIN_CFG(GPIOA, GPIO_PIN_2, GPIO_MODE_AF, GPIO_OTYPE_OD, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_UP, GPIO_AF_11, GPIO_STATE_OFF),		/*42, PA2,		ETH_MDIO,		OFF,		ETH_MDIO_App*/
		GPIO_PIN_CFG(GPIOA, GPIO_PIN_7, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_11, GPIO_STATE_OFF),	/*53, PA7,		ETH_CRS_DV,		OFF,		ETH_CRS_DV_App*/
		GPIO_PIN_CFG(GPIOC, GPIO_PIN_4, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_11, GPIO_STATE_OFF),	/*54, PC4,		ETH_RXD0,		OFF,		ETH_RXD0_App*/
		GPIO_PIN_CFG(GPIOC, GPIO_PIN_5, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_11, GPIO_STATE_OFF),	/*55, PC5,		ETH_RXD1,		OFF,		ETH_RXD1_App*/
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_11, GPIO_STATE_OFF),	/*92, PB12,		ETH_TXD0,		OFF,		ETH_TXD0_App*/
		GPIO_PIN_CFG(GPIOB, GPIO_PIN_13, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_11, GPIO_STATE_OFF),	/*93, PB13,		ETH_TXD1,		OFF,		ETH_TXD1_App*/
		GPIO_PIN_CFG(GPIOG, GPIO_PIN_11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_OSPEED_VERY_HIGH, GPIO_PUPD_NONE, GPIO_AF_11, GPIO_STATE_OFF),	/*154, PG11,	ETH_TX_EN,		OFF,		ETH_TX_EN_App*/
};

void eth_sel_rmii(void) {
	SYSCFG->PMC |= SYSCFG_PMC_MII_RMII; //RMII PHY interface is selected
}

void eth_rcc_init(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACEN;
}

uint32_t eth_bmcr[32];

err_t eth_phy_init(void) {
	gpio_pins_cfg_setup(gpio_eth_rst_cfg, GPIO_ETH_RST_COUNT); //RST pin init LOW
	sys_counter_delay(0, 200000); //200ms
	gpio_output_bit_setup(&GPO_ETH_Reset_App, ENABLE); //RST pin HIGH
	sys_counter_delay(0, 200000); //200ms
	gpio_pins_cfg_setup(eth_gpio_cfg, ETH_GPIO_COUNT); //RMII pins init
	sys_counter_delay(0, 200000); //200ms

	//ETH->MACCR = (1 << 3) | (1 << 2);

	for(int i = 31; i > 0; i--) {
		ETH->MACMIIAR = ((i << 11) | (0 << 6) | (0b001 << 2));
		ETH->MACMIIAR |= (1 << 0);
		while(ETH->MACMIIAR & (1 << 0));
		eth_bmcr[i] = ETH->MACMIIDR;
	}

	//ETH->MACMIIAR = ((1 << 11) | (24 << 6) | (0b011 << 2));// | (1 << 1));
	//ETH->MACMIIDR = 0b111;
	//ETH->MACMIIAR |= (1 << 0);
	//while(ETH->MACMIIAR & (1 << 0));

	return E_NO_ERROR;
}

err_t eth_init(void) {
	eth_sel_rmii();
	eth_rcc_init();
	eth_phy_init();
	return E_NO_ERROR;
}
