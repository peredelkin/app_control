/*
 * fmc.h
 *
 *  Created on: 24 нояб. 2023 г.
 *      Author: Ruslan
 */

#ifndef FMC_FMC_H_
#define FMC_FMC_H_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include "defs/defs.h"
#include "errors/errors.h"
#include "stm32f4xx/stm32f4xx.h"

/* FMC memory banks begin */
//Bank1 BASE
#define FMC_BANK1_DATA_SECTION_OFFSET				0x4000000
#define FMC_BANK1_1_DATA_SECTION_BASE 				0x60000000
#define FMC_BANK1_2_DATA_SECTION_BASE 				(FMC_BANK1_1_DATA_SECTION_BASE + FMC_BANK1_DATA_SECTION_OFFSET)
#define FMC_BANK1_3_DATA_SECTION_BASE 				(FMC_BANK1_2_DATA_SECTION_BASE + FMC_BANK1_DATA_SECTION_OFFSET)
#define FMC_BANK1_4_DATA_SECTION_BASE 				(FMC_BANK1_3_DATA_SECTION_BASE + FMC_BANK1_DATA_SECTION_OFFSET)

//NAND CLE/ALE
#define FMC_NAND_COMMON_COMMAND_OFFSET_DEFAULT		(1<< 16)
#define FMC_NAND_COMMON_ADDRESS_OFFSET_DEFAULT		(1<< 17)

//Bank2 Common BASE
#define FMC_BANK2_COMMON_COMMAND_OFFSET				FMC_NAND_COMMON_COMMAND_OFFSET_DEFAULT
#define FMC_BANK2_COMMON_ADDRESS_OFFSET				FMC_NAND_COMMON_ADDRESS_OFFSET_DEFAULT
#define FMC_BANK2_COMMON_DATA_SECTION_BASE			0x70000000
#define FMC_BANK2_COMMON_COMMAND_SECTION_BASE		(FMC_BANK2_COMMON_DATA_SECTION_BASE | FMC_BANK2_COMMON_COMMAND_OFFSET)
#define FMC_BANK2_COMMON_ADDRESS_SECTION_BASE		(FMC_BANK2_COMMON_DATA_SECTION_BASE | FMC_BANK2_COMMON_ADDRESS_OFFSET)
//Bank2 Attribute BASE
#define FMC_BANK2_ATTRIBUTE_COMMAND_OFFSET			FMC_NAND_COMMON_COMMAND_OFFSET_DEFAULT
#define FMC_BANK2_ATTRIBUTE_ADDRESS_OFFSET			FMC_NAND_COMMON_ADDRESS_OFFSET_DEFAULT
#define FMC_BANK2_ATTRIBUTE_DATA_SECTION_BASE		0x78000000
#define FMC_BANK2_ATTRIBUTE_COMMAND_SECTION_BASE	(FMC_BANK2_ATTRIBUTE_DATA_SECTION_BASE | FMC_BANK2_ATTRIBUTE_COMMAND_OFFSET)
#define FMC_BANK2_ATTRIBUTE_ADDRESS_SECTION_BASE	(FMC_BANK2_ATTRIBUTE_DATA_SECTION_BASE | FMC_BANK2_ATTRIBUTE_ADDRESS_OFFSET)
//Bank3 Common BASE
#define FMC_BANK3_COMMON_COMMAND_OFFSET				FMC_NAND_COMMON_COMMAND_OFFSET_DEFAULT
#define FMC_BANK3_COMMON_ADDRESS_OFFSET				FMC_NAND_COMMON_ADDRESS_OFFSET_DEFAULT
#define FMC_BANK3_COMMON_DATA_SECTION_BASE			0x80000000
#define FMC_BANK3_COMMON_COMMAND_SECTION_BASE		(FMC_BANK3_COMMON_DATA_SECTION_BASE | FMC_BANK3_COMMON_COMMAND_OFFSET)
#define FMC_BANK3_COMMON_ADDRESS_SECTION_BASE		(FMC_BANK3_COMMON_DATA_SECTION_BASE | FMC_BANK3_COMMON_ADDRESS_OFFSET)
//Bank3 Attribute BASE
#define FMC_BANK3_ATTRIBUTE_COMMAND_OFFSET			FMC_NAND_COMMON_COMMAND_OFFSET_DEFAULT
#define FMC_BANK3_ATTRIBUTE_ADDRESS_OFFSET			FMC_NAND_COMMON_ADDRESS_OFFSET_DEFAULT
#define FMC_BANK3_ATTRIBUTE_DATA_SECTION_BASE		0x88000000
#define FMC_BANK3_ATTRIBUTE_COMMAND_SECTION_BASE	(FMC_BANK3_ATTRIBUTE_DATA_SECTION_BASE | FMC_BANK3_ATTRIBUTE_COMMAND_OFFSET)
#define FMC_BANK3_ATTRIBUTE_ADDRESS_SECTION_BASE	(FMC_BANK3_ATTRIBUTE_DATA_SECTION_BASE | FMC_BANK3_ATTRIBUTE_ADDRESS_OFFSET)

//Bank1
#define FMC_BANK1_1_DATA_SECTION					((__IO uint32_t *)FMC_BANK1_1_DATA_SECTION_BASE)
#define FMC_BANK1_2_DATA_SECTION					((__IO uint32_t *)FMC_BANK1_2_DATA_SECTION_BASE)
#define FMC_BANK1_3_DATA_SECTION					((__IO uint32_t *)FMC_BANK1_3_DATA_SECTION_BASE)
#define FMC_BANK1_4_DATA_SECTION					((__IO uint32_t *)FMC_BANK1_4_DATA_SECTION_BASE)
//Bank2 Common
#define FMC_BANK2_COMMON_DATA_SECTION				((__IO uint8_t *)FMC_BANK2_COMMON_DATA_SECTION_BASE)
#define FMC_BANK2_COMMON_COMMAND_SECTION			((__IO uint8_t *)FMC_BANK2_COMMON_COMMAND_SECTION_BASE)
#define FMC_BANK2_COMMON_ADDRESS_SECTION			((__IO uint8_t *)FMC_BANK2_COMMON_ADDRESS_SECTION_BASE)
//Bank2 Attribute
#define FMC_BANK2_ATTRIBUTE_DATA_SECTION			((__IO uint8_t *)FMC_BANK2_ATTRIBUTE_DATA_SECTION_BASE)
#define FMC_BANK2_ATTRIBUTE_COMMAND_SECTION			((__IO uint8_t *)FMC_BANK2_ATTRIBUTE_COMMAND_SECTION_BASE)
#define FMC_BANK2_ATTRIBUTE_ADDRESS_SECTION			((__IO uint8_t *)FMC_BANK2_ATTRIBUTE_ADDRESS_SECTION_BASE)
//Bank3 Common
#define FMC_BANK3_COMMON_DATA8_SECTION				((__IO uint8_t *)FMC_BANK3_COMMON_DATA_SECTION_BASE)
#define FMC_BANK3_COMMON_COMMAND8_SECTION			((__IO uint8_t *)FMC_BANK3_COMMON_COMMAND_SECTION_BASE)
#define FMC_BANK3_COMMON_ADDRESS8_SECTION			((__IO uint8_t *)FMC_BANK3_COMMON_ADDRESS_SECTION_BASE)

#define FMC_BANK3_COMMON_DATA16_SECTION				((__IO uint16_t *)FMC_BANK3_COMMON_DATA_SECTION_BASE)
#define FMC_BANK3_COMMON_COMMAND16_SECTION			((__IO uint16_t *)FMC_BANK3_COMMON_COMMAND_SECTION_BASE)
#define FMC_BANK3_COMMON_ADDRESS16_SECTION			((__IO uint16_t *)FMC_BANK3_COMMON_ADDRESS_SECTION_BASE)

#define FMC_BANK3_COMMON_DATA32_SECTION				((__IO uint32_t *)FMC_BANK3_COMMON_DATA_SECTION_BASE)
#define FMC_BANK3_COMMON_COMMAND32_SECTION			((__IO uint32_t *)FMC_BANK3_COMMON_COMMAND_SECTION_BASE)
#define FMC_BANK3_COMMON_ADDRESS32_SECTION			((__IO uint32_t *)FMC_BANK3_COMMON_ADDRESS_SECTION_BASE)
//Bank3 Attribute
#define FMC_BANK3_ATTRIBUTE_DATA_SECTION			((__IO uint8_t *)FMC_BANK3_ATTRIBUTE_DATA_SECTION_BASE)
#define FMC_BANK3_ATTRIBUTE_COMMAND_SECTION			((__IO uint8_t *)FMC_BANK3_ATTRIBUTE_COMMAND_SECTION_BASE)
#define FMC_BANK3_ATTRIBUTE_ADDRESS_SECTION			((__IO uint8_t *)FMC_BANK3_ATTRIBUTE_ADDRESS_SECTION_BASE)
/* FMC memory banks end */

/* NOR/PSRAM controller registers begin */

typedef struct PACKED _FMC_BCR_BIT {
	unsigned MBKEN :1;
	unsigned MUXEN :1;
	unsigned MTYP :2;
	unsigned MWID :2;
	unsigned FACCEN :1;
	unsigned RESERVED_7 :1;
	unsigned BURSTEN :1;
	unsigned WAITPOL :1;
	unsigned WRAPMOD :1;
	unsigned WAITCFG :1;
	unsigned WREN :1;
	unsigned WAITEN :1;
	unsigned EXTMOD :1;
	unsigned ASYNCWAIT :1;
	unsigned CPSIZE :3;
	unsigned CBURSTRW :1;
	unsigned CCLKEN :1;
	unsigned RESERVED_21_31 :11;
} FMC_BCR_BIT_TypeDef;
static_assert(sizeof(FMC_BCR_BIT_TypeDef) == 4, "Invalid size of FMC_BCR_BIT_TypeDef!");

typedef struct PACKED _FMC_BTR_BIT {
	unsigned ADDSET :4;
	unsigned ADDHLD :4;
	unsigned DATAST :8;
	unsigned BUSTURN :4;
	unsigned CLKDIV :4;
	unsigned DATLAT :4;
	unsigned ACCMOD :2;
	unsigned RESERVED_30_31 :2;
} FMC_BTR_BIT_TypeDef;
static_assert(sizeof(FMC_BTR_BIT_TypeDef) == 4, "Invalid size of FMC_BTR_BIT_TypeDef!");

typedef struct PACKED _FMC_BWTR_BIT {
	unsigned ADDSET :4;
	unsigned ADDHLD :4;
	unsigned DATAST :8;
	unsigned BUSTURN :4;
	unsigned RESERVED_20_27 :8;
	unsigned ACCMOD :2;
	unsigned RESERVED_30_31 :2;
} FMC_BWTR_BIT_TypeDef;
static_assert(sizeof(FMC_BWTR_BIT_TypeDef) == 4, "Invalid size of FMC_BWTR_BIT_TypeDef!");

typedef union _FMC_BCR_REG {
	uint32_t all;
	struct _FMC_BCR_BIT bit;
} FMC_BCR_REG_TypeDef;

typedef union _FMC_BTR_REG {
	uint32_t all;
	struct _FMC_BTR_BIT bit;
} FMC_BTR_REG_TypeDef;

typedef union _FMC_BWTR_REG {
	uint32_t all;
	struct _FMC_BWTR_BIT bit;
} FMC_BWTR_REG_TypeDef;


typedef struct {
	FMC_BCR_REG_TypeDef CR;
	FMC_BTR_REG_TypeDef TR;
} FMC_BCТRn_TypeDef;


typedef struct {
	FMC_BWTR_REG_TypeDef WTR;
	uint32_t reserved1;
} FMC_BWTRn_TypeDef;


typedef struct {
	FMC_BCТRn_TypeDef BCTR[4];
} FMC_BANK1_TypeDef;

typedef struct {
	FMC_BWTRn_TypeDef BWTR[4];
} FMC_BANK1E_TypeDef;

#define FMC_BANK1_BASE	(FMC_R_BASE + 0x0000)
#define FMC_BANK1E_BASE	(FMC_R_BASE + 0x0104)

#define FMC_BANK1	((FMC_BANK1_TypeDef *) FMC_BANK1_BASE)
#define FMC_BANK1E	((FMC_BANK1E_TypeDef *) FMC_BANK1E_BASE)

/* NOR/PSRAM controller registers end */

/* NAND Flash/PC Card controller registers begin */

typedef struct PACKED _FMC_PCR_BIT {
	unsigned RESERVED_0 :1;
	unsigned PWAITEN :1;
	unsigned PBKEN :1;
	unsigned PTYP :1;
	unsigned PWID :2;
	unsigned ECCEN :1;
	unsigned RESERVED_7_8 :2;
	unsigned TCLR :4;
	unsigned TAR :4;
	unsigned ECCPS :3;
	unsigned RESERVED_20_31 :12;
} FMC_PCR_BIT_TypeDef;
static_assert(sizeof(FMC_PCR_BIT_TypeDef) == 4, "Invalid size of FMC_PCR_BIT_TypeDef!");

typedef struct PACKED _FMC_SR_BIT {
	unsigned IRS :1;
	unsigned ILS :1;
	unsigned IFS :1;
	unsigned IREN :1;
	unsigned ILEN :1;
	unsigned IFEN :1;
	unsigned FEMPT :1;
	unsigned RESERVED_7_31 :25;
} FMC_SR_BIT_TypeDef;
static_assert(sizeof(FMC_SR_BIT_TypeDef) == 4, "Invalid size of FMC_SR_BIT_TypeDef!");

typedef struct PACKED _FMC_PMEM_BIT {
	unsigned MEMSET :8;
	unsigned MEMWAIT :8;
	unsigned MEMHOLD :8;
	unsigned MEMHIZ :8;
} FMC_PMEM_BIT_TypeDef;
static_assert(sizeof(FMC_PMEM_BIT_TypeDef) == 4, "Invalid size of FMC_PMEM_BIT_TypeDef!");

typedef struct PACKED _FMC_PATT_BIT {
	unsigned ATTSET :8;
	unsigned ATTWAIT :8;
	unsigned ATTHOLD :8;
	unsigned ATTHIZ :8;
} FMC_PATT_BIT_TypeDef;
static_assert(sizeof(FMC_PATT_BIT_TypeDef) == 4, "Invalid size of FMC_PATT_BIT_TypeDef!");

typedef struct PACKED _FMC_PIO_BIT {
	unsigned IOSET :8;
	unsigned IOWAIT :8;
	unsigned IOHOLD :8;
	unsigned IOHIZ :8;
} FMC_PIO_BIT_TypeDef;
static_assert(sizeof(FMC_PIO_BIT_TypeDef) == 4, "Invalid size of FMC_PIO_BIT_TypeDef!");

typedef union _FMC_PCR_REG {
	uint32_t all;
	struct _FMC_PCR_BIT bit;
} FMC_PCR_REG_TypeDef;

typedef union _FMC_SR_REG {
	uint32_t all;
	struct _FMC_SR_BIT bit;
} FMC_SR_REG_TypeDef;

typedef union _FMC_PMEM_REG {
	uint32_t all;
	struct _FMC_PMEM_BIT bit;
} FMC_PMEM_REG_TypeDef;

typedef union _FMC_PATT_REG {
	uint32_t all;
	struct _FMC_PATT_BIT bit;
} FMC_PATT_REG_TypeDef;

typedef union _FMC_PIO_REG {
	uint32_t all;
	struct _FMC_PIO_BIT bit;
} FMC_PIO_REG_TypeDef;

typedef struct {
	FMC_PCR_REG_TypeDef PCR;
	FMC_SR_REG_TypeDef SR;
	FMC_PMEM_REG_TypeDef PMEM;
	FMC_PATT_REG_TypeDef PATT;
	uint32_t RESERVED;
	__IO uint32_t ECCR;
} FMC_BANK23_TypeDef;

typedef struct {
	FMC_PCR_REG_TypeDef PCR;
	FMC_SR_REG_TypeDef SR;
	FMC_PMEM_REG_TypeDef PMEM;
	FMC_PATT_REG_TypeDef PATT;
	FMC_PIO_REG_TypeDef PIO;
} FMC_BANK4_TypeDef;

#define FMC_BANK2_BASE	(FMC_R_BASE + 0x0060)
#define FMC_BANK3_BASE	(FMC_R_BASE + 0x0080)
#define FMC_BANK4_BASE	(FMC_R_BASE + 0x00A0)

#define FMC_BANK2 ((FMC_BANK23_TypeDef *) FMC_BANK2_BASE)
#define FMC_BANK3 ((FMC_BANK23_TypeDef *) FMC_BANK3_BASE)
#define FMC_BANK4 ((FMC_BANK4_TypeDef *) FMC_BANK4_BASE)

#define FMC_BANK23_FIFO32_COUNT	16
#define FMC_BANK23_FIFO16_COUNT	16	//32
#define FMC_BANK23_FIFO8_COUNT	16	//64
/* NAND Flash/PC Card controller registers end */

extern void fmc_eccen_set(FMC_BANK23_TypeDef *bank, bool state);
extern int fmc_ecc_page_size(FMC_BANK23_TypeDef* bank);
extern void fmc_fifo_empty_wait(FMC_BANK23_TypeDef *control);
extern err_t fmc_data_8_read(__IO uint8_t *fmc_data, uint8_t *data, int data_len);
extern err_t fmc_data_32_read(__IO uint32_t *fmc_data, uint32_t *data, int data_len);
extern err_t fmc_bank23_data_8_write(__IO uint8_t *fmc_data, uint8_t *data, int data_len);
extern err_t fmc_bank23_data_32_write(__IO uint32_t *fmc_data, uint32_t *data, int data_len);

#endif /* FMC_FMC_H_ */
