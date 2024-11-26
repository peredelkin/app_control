/*
 * eth_init.h
 *
 *  Created on: 5 мар. 2024 г.
 *      Author: Ruslan
 */

#ifndef ETH_INIT_ETH_INIT_H_
#define ETH_INIT_ETH_INIT_H_

#include "eth/eth.h"
#include "gpio/gpio.h"
#include "lib/errors/errors.h"

extern err_t eth_init(void);
extern err_t eth_phy_init(void);

#endif /* ETH_INIT_ETH_INIT_H_ */
