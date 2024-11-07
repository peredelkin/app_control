/*
 * spi_init.h
 *
 *  Created on: 24 окт. 2023 г.
 *      Author: Ruslan
 */

#ifndef SPI_INIT_SPI_INIT_H_
#define SPI_INIT_SPI_INIT_H_

#include "spi/spi.h"

extern SPI_BUS_TypeDef SPI2_Bus;
extern SPI_BUS_TypeDef SPI4_Bus;
extern SPI_BUS_TypeDef SPI5_Bus;

//SPI2
extern void spi2_nvic_init(uint32_t priority);
extern void spi2_bus_init(void);

//SPI4
extern void spi4_nvic_init(uint32_t priority);
extern void spi4_bus_init(void);

//SPI5
extern void spi5_bus_init(void);

#endif /* SPI_INIT_SPI_INIT_H_ */
