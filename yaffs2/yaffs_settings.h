/*
 * yaffs_settings.h
 *
 *  Created on: 15 дек. 2023 г.
 *      Author: Ruslan
 */

#ifndef YAFFS2_YAFFS_SETTINGS_H_
#define YAFFS2_YAFFS_SETTINGS_H_

//#include "stm32f4xx/stm32f4xx.h"
#include <sys/types.h>

//from Yaffs Tuning
#define CONFIG_YAFFS_ALWAYS_CHECK_CHUNK_ERASED 1 //This is slower, but is worth using when doing early debugging.
#define CONFIG_YAFFS_DIRECT 1 //Used when compiling Yaffs Direct.
#define CONFIG_YAFFS_YAFFS2 1 //Set when using Yaffs2 mode.
#define CONFIG_YAFFSFS_PROVIDE_VALUES 1 //Used when compiling Yaffs Direct only.

/* Definition of types */
//typedef uint8_t u8;
//typedef uint16_t u16;
//typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t s64;
//typedef int32_t s32;

//#define LOFF_T_32_BIT 1
#define Y_LOFF_T s64

#define CONFIG_YAFFS_PROVIDE_DEFS 1
#define CONFIG_YAFFS_DEFINES_TYPES 1

#endif /* YAFFS2_YAFFS_SETTINGS_H_ */
