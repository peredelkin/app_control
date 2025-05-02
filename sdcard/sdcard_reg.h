/*
 * sdcard_reg.h
 *
 *  Created on: 23 апр. 2025 г.
 *      Author: Ruslan
 */

#ifndef SDCARD_SDCARD_REG_H_
#define SDCARD_SDCARD_REG_H_

#include <stdint.h>
#include <assert.h>

// SD Version 9.10

#define SDCARD_CID_SIZE 16
#define SDCARD_CSD_SIZE 16

#pragma pack(push, 1)
typedef union {
	struct {
	    unsigned NOT_USED	:1;
	    unsigned CRC7		:7;
	    unsigned MDT_M		:4;
	    unsigned MDT_Y		:8;
	    unsigned RESERVED	:4;
	    uint8_t PSN[4];
	    uint8_t PRV;
	    uint8_t PNM[5];
	    uint8_t OID[2];
	    uint8_t MID;
	} bit;
	uint32_t all[4];
} sdcard_reg_CID_t;
#pragma pack(pop)
static_assert(sizeof(sdcard_reg_CID_t) == SDCARD_CID_SIZE, "Invalid size of sdcard CID!");


#pragma pack(push, 1)
typedef union {
	struct {
		unsigned NO_USED			:1;
		unsigned CRC7				:7;
		unsigned RESERVED_8			:1;
		unsigned WP_UPC				:1;
		unsigned FILE_FORMAT		:2;
		unsigned TMP_WRITE_PROTECT	:1;
		unsigned PERM_WRITE_PROTECT	:1;
		unsigned COPY				:1;
		unsigned FILE_FORMAT_GRP	:1;
		unsigned RESERVED_16_20		:5;
		unsigned WRITE_BL_PARTIAL	:1;
		unsigned WRITE_BL_LEN		:4;
		unsigned R2W_FACTOR			:3;
		unsigned RESERVED_29_30		:2;
		unsigned WP_GRP_ENABLE		:1;
		unsigned WP_GRP_SIZE		:7;
		unsigned SECTOR_SIZE		:7;
		unsigned ERASE_BLK_EN		:1;
		unsigned C_SIZE_MULT		:3;
		unsigned VDD_W_CURR_MAX		:3;
		unsigned VDD_W_CURR_MIN		:3;
		unsigned VDD_R_CURR_MAX		:3;
		unsigned VDD_R_CURR_MIN		:3;
		unsigned C_SIZE				:12;
		unsigned RESERVED_74_75		:2;
		unsigned DSR_IMP			:1;
		unsigned READ_BLK_MISALIGN	:1;
		unsigned WRITE_BLK_MISALIGN	:1;
		unsigned READ_BL_PARTIAL	:1;
		unsigned READ_BL_LEN		:4;
		unsigned CCC				:12;
		unsigned TRAN_SPEED_UNIT	:3;	//TRAN_SPEED 0:2
		unsigned TRAN_SPEED_VALUE	:4;	//TRAN_SPEED 3:6
		unsigned TRAN_SPEED_7		:1;	//TRAN_SPEED 7
		unsigned NSAC				:8;
		unsigned TAAC_UNIT			:3;	//TAAC 0:2
		unsigned TAAC_VALUE			:4;	//TAAC 3:6
		unsigned TAAC_7				:1;	//TAAC 7
		unsigned RESERVED_120_125	:6;
		unsigned CSD_STRUCTURE		:2;
	} bit;
	uint32_t all[4];
} sdcard_reg_CSD_v1_t;
#pragma pack(pop)
static_assert(sizeof(sdcard_reg_CSD_v1_t) == SDCARD_CSD_SIZE, "Invalid size of sdcard CSD V1!");


#pragma pack(push, 1)
typedef union {
	struct {
		unsigned NO_USED			:1;
		unsigned CRC7				:7;
		unsigned RESERVED_8			:1;
		unsigned WP_UPC				:1;
		unsigned FILE_FORMAT		:2;
		unsigned TMP_WRITE_PROTECT	:1;
		unsigned PERM_WRITE_PROTECT	:1;
		unsigned COPY				:1;
		unsigned FILE_FORMAT_GRP	:1;
		unsigned RESERVED_16_20		:5;
		unsigned WRITE_BL_PARTIAL	:1;
		unsigned WRITE_BL_LEN		:4;
		unsigned R2W_FACTOR			:3;
		unsigned RESERVED_29_30		:2;
		unsigned WP_GRP_ENABLE		:1;
		unsigned WP_GRP_SIZE		:7;
		unsigned SECTOR_SIZE		:7;
		unsigned ERASE_BLK_EN		:1;
		unsigned RESERVED_47		:1;
		unsigned C_SIZE				:22;
		unsigned RESERVED_70_75		:6;
		unsigned DSR_IMP			:1;
		unsigned READ_BLK_MISALIGN	:1;
		unsigned WRITE_BLK_MISALIGN	:1;
		unsigned READ_BL_PARTIAL	:1;
		unsigned READ_BL_LEN		:4;
		unsigned CCC				:12;
		unsigned TRAN_SPEED_UNIT	:3;	//TRAN_SPEED 0:2
		unsigned TRAN_SPEED_VALUE	:4;	//TRAN_SPEED 3:6
		unsigned TRAN_SPEED_7		:1;	//TRAN_SPEED 7
		unsigned NSAC				:8;
		unsigned TAAC_UNIT			:3;	//TAAC 0:2
		unsigned TAAC_VALUE			:4;	//TAAC 3:6
		unsigned TAAC_7				:1;	//TAAC 7
		unsigned RESERVED_120_125	:6;
		unsigned CSD_STRUCTURE		:2;
	} bit;
	uint32_t all[4];
} sdcard_reg_CSD_v2_t;
#pragma pack(pop)
static_assert(sizeof(sdcard_reg_CSD_v2_t) == SDCARD_CSD_SIZE, "Invalid size of sdcard CSD V2!");


#pragma pack(push, 1)
typedef union {
	struct {
		unsigned NO_USED			:1;
		unsigned CRC7				:7;
		unsigned RESERVED_8			:1;
		unsigned WP_UPC				:1;
		unsigned FILE_FORMAT		:2;
		unsigned TMP_WRITE_PROTECT	:1;
		unsigned PERM_WRITE_PROTECT	:1;
		unsigned COPY				:1;
		unsigned FILE_FORMAT_GRP	:1;
		unsigned RESERVED_16_20		:5;
		unsigned WRITE_BL_PARTIAL	:1;
		unsigned WRITE_BL_LEN		:4;
		unsigned R2W_FACTOR			:3;
		unsigned RESERVED_29_30		:2;
		unsigned WP_GRP_ENABLE		:1;
		unsigned WP_GRP_SIZE		:7;
		unsigned SECTOR_SIZE		:7;
		unsigned ERASE_BLK_EN		:1;
		unsigned RESERVED_47		:1;
		unsigned C_SIZE				:28;
		unsigned DSR_IMP			:1;
		unsigned READ_BLK_MISALIGN	:1;
		unsigned WRITE_BLK_MISALIGN	:1;
		unsigned READ_BL_PARTIAL	:1;
		unsigned READ_BL_LEN		:4;
		unsigned CCC				:12;
		unsigned TRAN_SPEED_UNIT	:3;	//TRAN_SPEED 0:2
		unsigned TRAN_SPEED_VALUE	:4;	//TRAN_SPEED 3:6
		unsigned TRAN_SPEED_7		:1;	//TRAN_SPEED 7
		unsigned NSAC				:8;
		unsigned TAAC_UNIT			:3;	//TAAC 0:2
		unsigned TAAC_VALUE			:4;	//TAAC 3:6
		unsigned TAAC_7				:1;	//TAAC 7
		unsigned RESERVED_120_125	:6;
		unsigned CSD_STRUCTURE		:2;
	} bit;
	uint32_t all[4];
} sdcard_reg_CSD_v3_t;
#pragma pack(pop)
static_assert(sizeof(sdcard_reg_CSD_v3_t) == SDCARD_CSD_SIZE, "Invalid size of sdcard CSD V3!");

#endif /* SDCARD_SDCARD_REG_H_ */
