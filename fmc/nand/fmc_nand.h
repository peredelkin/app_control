/*
 * fmc_nand.h
 *
 *  Created on: 12 дек. 2023 г.
 *      Author: Ruslan
 */

#ifndef FMC_NAND_FMC_NAND_H_
#define FMC_NAND_FMC_NAND_H_

#include "../fmc.h"
#include "lib/errors/errors.h"
#include "dma/dma.h"

//BLOCK
#define K9F1G08U0E_BLOCK_SIZE			64
#define K9F1G08U0E_BLOCK_COUNT			1024
#define K9F1G08U0E_PAGE_COUNT			(K9F1G08U0E_BLOCK_SIZE * K9F1G08U0E_BLOCK_COUNT)

//BLOCK check/mark
#define K9F1G08U0E_MARK_SPARE_SIZE		1
#define K9F1G08U0E_MARK_SPARE_COUNT		2
#define K9F1G08U0E_BLOCK_GOOD_MASK		0xFF
#define K9F1G08U0E_BLOCK_BAD_MASK		0x00
#define K9F1G08U0E_BLOCK_BAD_MAX		20

//PAGE
#define K9F1G08U0E_PAGE_DATA_SIZE		2048
#define K9F1G08U0E_PAGE_SPARE_SIZE		64
#define K9F1G08U0E_PAGE_TOTAL_SIZE		(K9F1G08U0E_PAGE_DATA_SIZE + K9F1G08U0E_PAGE_SPARE_SIZE)

//ECC
#define K9F1G08U0E_ECC_PAGE_SIZE		256
#define K9F1G08U0E_ECC_RESULT_SIZE		3
#define K9F1G08U0E_ECC_RESULT_COUNT		(K9F1G08U0E_PAGE_DATA_SIZE / K9F1G08U0E_ECC_PAGE_SIZE)
#define K9F1G08U0E_ECC_SPARE_SIZE		(K9F1G08U0E_ECC_RESULT_COUNT * K9F1G08U0E_ECC_RESULT_SIZE)

//ADDRESSES
#define K9F1G08U0E_PAGE_START_ADDRESS	0
#define K9F1G08U0E_PAGE_STATE_ADDRESS	2048
#define K9F1G08U0E_PAGE_ECC_ADDRESS		(K9F1G08U0E_PAGE_STATE_ADDRESS + K9F1G08U0E_MARK_SPARE_SIZE)
#define K9F1G08U0E_PAGE_OOB_ADDRESS		(K9F1G08U0E_PAGE_ECC_ADDRESS + K9F1G08U0E_ECC_SPARE_SIZE)

//OOB
#define K9F1G08U0E_PAGE_OOB_SIZE		(K9F1G08U0E_PAGE_SPARE_SIZE - K9F1G08U0E_MARK_SPARE_SIZE - K9F1G08U0E_ECC_SPARE_SIZE)

//K9F1G08U0E YAFFS Param
#define YAFFS_PARAM_INBAND_TAGS				0							//Yaffs2 only:Flag indicating whether tags should be stored in the data area.
#define YAFFS_PARAM_TOTAL_BYTES_PER_CHUNK	K9F1G08U0E_PAGE_DATA_SIZE	//Page 2K Bytes
#define YAFFS_PARAM_CHUNKS_PER_BLOCK		64							//Block 64 Pages
#define YAFFS_PARAM_SPARE_BYTES_PER_CHUNK	K9F1G08U0E_PAGE_SPARE_SIZE	//Spare 64 Bytes
#define YAFFS_PARAM_START_BLOCK				1							//00h block reserved for settings backup
#define YAFFS_PARAM_END_BLOCK				(K9F1G08U0E_BLOCK_COUNT - 1)//Total blocks is 1024
#define YAFFS_PARAM_N_RESERVED_BLOCKS		5							//Needs to be at least 2
#define YAFFS_PARAM_N_CACHES				10							//Typical value is 10 to 20 or so
#define YAFFS_PARAM_CACHE_BYPASS_ALIGNED	0							//TODO: ?
#define YAFFS_PARAM_USE_NAND_ECC			0							//Yaffs1 only: Flag indicating whether the driver performs ECC.
#define YAFFS_PARAM_TAGS_9BYTES				0							//TODO: ?
#define YAFFS_PARAM_NO_TAGS_ECC				0							//Yaffs2 only: Flag indicating whether the tags have ECC attached to them.
#define YAFFS_PARAM_IS_YAFFS2				1							//Use Yaffs2 mode on this device
#define YAFFS_PARAM_EMPTY_LOST_N_FOUND		1							//Flag to delete all files in lost and found on mount.
#define YAFFS_PARAM_REFRESH_PERIOD			1000						//Typical values would be 1000
#define YAFFS_PARAM_SKIP_CHECKPT_RD			1							//Yaffs2 only: Flag to skip reading checkpoint on mount. If set then a re-scan is forced.
#define YAFFS_PARAM_SKIP_CHECKPT_WR			1							//Yaffs2 only: Flag to skip writing checkpoint on sync or unmount.
#define YAFFS_PARAM_ENABLE_XATTR			0							//TODO: ?
#define YAFFS_PARAM_MAX_OBJECTS				0							//No limit
#define YAFFS_PARAM_HIDE_LOST_N_FOUND		1							//Hide the lost-n-found dir
#define YAFFS_PARAM_STORED_ENDIAN			0							//CPU endian

/*K9F1G08U0E CMD BEGIN*/
//Page Read Cmd 00h 30h
#define NANDFLASH_CMD_PAGE_READ_CYCLE1 ((uint8_t) 0x00)
#define NANDFLASH_CMD_PAGE_READ_CYCLE2 ((uint8_t) 0x30)

//Read for Copy Back	00h	35h
#define NANDFLASH_CMD_PAGE_READ_COPY_BACK_CYCLE1 ((uint8_t )0x00)
#define NANDFLASH_CMD_PAGE_READ_COPY_BACK_CYCLE2 ((uint8_t) 0x35)

//Read ID 90h
#define NANDFLASH_CMD_ID_READ_CYCLE1	((uint8_t) 0x90)
//Read ID Cycles
#define K9F1G08U0E_ID_DATA_COUNT	5
//1st Cycle ID
#define NANDFLASH_ID_READ_CYCLE1		((uint8_t) 0xEC)
//2nd Cycle ID
#define NANDFLASH_ID_READ_CYCLE2		((uint8_t) 0xF1)
//3rd Cycle ID
#define NANDFLASH_ID_READ_CYCLE3		((uint8_t) 0x00)
//4th Cycle ID
#define NANDFLASH_ID_READ_CYCLE4		((uint8_t) 0x95)
//5th Cycle ID
#define NANDFLASH_ID_READ_CYCLE5		((uint8_t) 0x41)

//Reset FFh
#define NANDFLASH_CMD_RESET_CYCLE1 ((uint8_t) 0xFF)

//Page Program 80h 10h
#define NANDFLASH_CMD_PAGE_PROGRAM_CYCLE1 ((uint8_t) 0x80)
#define NANDFLASH_CMD_PAGE_PROGRAM_CYCLE2 ((uint8_t) 0x10)

//Copy-Back Program	85h 10h
#define NANDFLASH_CMD_PAGE_PROGRAM_COPY_BACK_CYCLE1 ((uint8_t) 0x85)
#define NANDFLASH_CMD_PAGE_PROGRAM_COPY_BACK_CYCLE2 ((uint8_t) 0x10)

//Block Erase 60h D0h
#define NANDFLASH_CMD_BLOCK_ERASE_CYCLE1 ((uint8_t) 0x60)
#define NANDFLASH_CMD_BLOCK_ERASE_CYCLE2 ((uint8_t) 0xD0)

//Random Data Input 85h
#define NANDFLASH_CMD_PAGE_RANDOM_INPUT_CYCLE1 ((uint8_t) 0x85)

//Random Data Output 05h E0h
#define NANDFLASH_CMD_PAGE_RANDOM_OUTPUT_CYCLE1 ((uint8_t) 0x05)
#define NANDFLASH_CMD_PAGE_RANDOM_OUTPUT_CYCLE2 ((uint8_t) 0xE0)

//Read Status 70h
#define NANDFLASH_CMD_STATUS_READ_CYCLE1 ((uint8_t) 0x70)
//Fail
#define NANDFLASH_STATUS_READ_FAIL	((uint8_t) 1 << 0)
//Ready
#define NANDFLASH_STATUS_READ_READY	((uint8_t) 1 << 6)
//Not Protected
#define NANDFLASH_STATUS_READ_NOT_PROTECTED ((uint8_t) 1 << 7)
/*K9F1G08U0E CMD END*/

//K9F1G08U0E SECTIONS
#define K9F1G08U0E_CONTROL			FMC_BANK3

#define K9F1G08U0E_DATA8_SECTION		FMC_BANK3_COMMON_DATA8_SECTION
#define K9F1G08U0E_COMMAND8_SECTION		FMC_BANK3_COMMON_COMMAND8_SECTION
#define K9F1G08U0E_ADDRESS8_SECTION		FMC_BANK3_COMMON_ADDRESS8_SECTION

#define K9F1G08U0E_DATA16_SECTION		FMC_BANK3_COMMON_DATA16_SECTION
#define K9F1G08U0E_COMMAND16_SECTION	FMC_BANK3_COMMON_COMMAND16_SECTION
#define K9F1G08U0E_ADDRESS16_SECTION	FMC_BANK3_COMMON_ADDRESS16_SECTION

#define K9F1G08U0E_DATA32_SECTION		FMC_BANK3_COMMON_DATA32_SECTION
#define K9F1G08U0E_COMMAND32_SECTION	FMC_BANK3_COMMON_COMMAND32_SECTION
#define K9F1G08U0E_ADDRESS32_SECTION	FMC_BANK3_COMMON_ADDRESS32_SECTION

typedef struct {
	bool initialized;
	dma_t dma;
	uint8_t id[K9F1G08U0E_ID_DATA_COUNT];
	uint8_t data[K9F1G08U0E_PAGE_TOTAL_SIZE];
	uint8_t ecc_tmp[K9F1G08U0E_ECC_RESULT_SIZE];
} nand_flash_driver_t;

extern nand_flash_driver_t nand_K9F1G08U0E_drv;

extern err_t nand_K9F1G08U0E_init(nand_flash_driver_t *drv);
extern err_t nand_K9F1G08U0E_page_read(nand_flash_driver_t *drv, uint16_t row);
extern err_t nand_K9F1G08U0E_block_check_bad(uint16_t block);
extern err_t nand_K9F1G08U0E_id_check(nand_flash_driver_t *drv);
extern err_t nand_K9F1G08U0E_page_program(nand_flash_driver_t *drv, uint16_t row);
extern err_t nand_K9F1G08U0E_block_mark_bad(uint16_t block);
extern err_t nand_K9F1G08U0E_block_erase(uint16_t block);

#endif /* FMC_NAND_FMC_NAND_H_ */
