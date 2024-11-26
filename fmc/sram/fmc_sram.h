/*
 * sram.h
 *
 *  Created on: 12 дек. 2023 г.
 *      Author: Ruslan
 */

#ifndef FMC_SRAM_FMC_SRAM_H_
#define FMC_SRAM_FMC_SRAM_H_

#include "../fmc.h"
#include "lib/errors/errors.h"

#define SRAM_WORDS_32			262144
#define SRAM_WORDS_16			(SRAM_WORDS_32 * 2)
#define SRAM_WORDS_8			(SRAM_WORDS_16 * 2)

#define SRAM_DATA_32_SECTION	((__IO uint32_t *)FMC_BANK1_1_DATA_SECTION_BASE)
#define SRAM_DATA_16_SECTION	((__IO uint16_t *)FMC_BANK1_1_DATA_SECTION_BASE)
#define SRAM_DATA_8_SECTION		((__IO uint8_t *)FMC_BANK1_1_DATA_SECTION_BASE)

#endif /* FMC_SRAM_FMC_SRAM_H_ */
