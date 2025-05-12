/**
 * @file sdcard.h Библиотека для работы с SD-картами по SPI.
 */

#ifndef SDCARD_H_
#define SDCARD_H_

//#include <stm32f10x.h>
#include "defs/defs.h"
//#include "spi/spi.h"
#include "sdio/sdio.h"
//#include "gpio/gpio.h"
#include "errors/errors.h"
#include "future/future.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "sdcard_response.h"
#include "sdcard_token.h"
#include "sdcard_reg.h"
#include "sdcard_cmd.h"
#include "dma/dma.h"

/*
 * Размер блока и ограничения SD-карты.
 */

//! Размер блока SD-карты.
#define SDCARD_BLOCK_SIZE 512

//! Максимальный номер читаемого/записываемого сектора для карт MMC/SDSC.
//! Ограничен из-за трансляции секторной адресации в байтовую.
#define SDCARD_SDSC_MMC_MAX_RW_SECTOR_NUMBER (UINT32_MAX >> 9) // UINT_MAX / 512.

//! Максимальное число читаемых/записываемых секторов.
//! Ограничено из-за трансляции числа секторов в размер читаемых данных.
//! Равно 4 Гб - 512 б данных, иначе 32 битная переменная
//! размера считываемых/записываемых данных переполнится.
#define SDCARD_MAX_RW_SECTORS_COUNT (UINT32_MAX >> 9) // UINT_MAX / 512.


/*
 * Ошибки SD-карты.
 */
//! Начальный код ошибки SD-карты.
#define E_SDCARD (E_USER + 10)
//! Неподдерживаемая карта.
#define E_SDCARD_INVALID (E_SDCARD + 0)
//! Принят некорректный токен данных.
#define E_SDCARD_INVALID_TOKEN (E_SDCARD + 1)
//! Ошибка записи данных картой.
#define E_SDCARD_WRITE_ERROR (E_SDCARD + 2)
//! Ошибки в статусе карты.
#define E_SDCARD_STATUS (E_SDCARD + 3)
//! Карта заблокирована.
#define E_SDCARD_CARD_IS_LOCKED (E_SDCARD_STATUS + 0)
//! Ошибка последовательности блокировки/разблокировки.
#define E_SDCARD_LOCK_UNLOCK_CMD_FAILED (E_SDCARD_STATUS + 1)
//! Стирание защищённого от записи блока.
#define E_SDCARD_WP_ERASE_SKIP (E_SDCARD_STATUS + 1)
//! Общая или неизвестная ошибка.
#define E_SDCARD_ERROR (E_SDCARD_STATUS + 2)
//! Ошибка контроллера карты.
#define E_SDCARD_CC_ERROR (E_SDCARD_STATUS + 3)
//! Ошибка ECC.
#define E_SDCARD_CARD_ECC_FAILED (E_SDCARD_STATUS + 4)
//! Запись в защищённый от записи блок.
#define E_SDCARD_WP_VIOLATION (E_SDCARD_STATUS + 5)
//! Параметр стирания.
#define E_SDCARD_ERASE_PARAM (E_SDCARD_STATUS + 6)
//! Передапись CSD.
#define E_SDCARD_CSD_OVERWRITE (E_SDCARD_STATUS + 7)
//! Выход за пределы.
#define E_SDCARD_OUT_OF_RANGE (E_SDCARD_STATUS + 7)
//! В состоянии IDLE.
#define E_SDCARD_IN_IDLE_STATE (E_SDCARD_STATUS + 8)
//! Сброс стирания.
#define E_SDCARD_ERASE_RESET (E_SDCARD_STATUS + 9)
//! Недопустимая команда.
#define E_SDCARD_ILLEGAL_COMMAND (E_SDCARD_STATUS + 10)
//! Ошибка CRC команды.
#define E_SDCARD_COM_CRC_ERROR (E_SDCARD_STATUS + 11)
//! Ошибка последовательности стирания.
#define E_SDCARD_ERASE_SEQ_ERROR (E_SDCARD_STATUS + 12)
//! Ошибка адреса.
#define E_SDCARD_ADDR_ERROR (E_SDCARD_STATUS + 13)
//! Ошибка параметра.
#define E_SDCARD_PARAM_ERROR (E_SDCARD_STATUS + 14)

//! Перечисление типа SD-карты.
typedef enum _SD_Card_Type {
    SDCARD_TYPE_UNKNOWN = 0, //!< Неизвестный тип карты.
    SDCARD_TYPE_SC,
	SDCARD_TYPE_HC_XC,
	SDCARD_TYPE_UC
} sdcard_type_t;


//! Структура SD-карты.
typedef struct _SD_Card {
	dma_t dma;
	sdcard_state_t current_state; //!< Текущее состояние карты.
	sdcard_cmd_t* cmd; //!< Указатель на выполняемую команду
	sdio_respwait_t resp_wait;
	sdio_resptype_t resp_long;
	sdio_resp_crc_include_t resp_crc;
	sdcard_response_t response;
	uint32_t RCA;
	sdcard_reg_CID_t CID;
	sdcard_reg_CSD_t CSD;
	sdcard_ccc_t CCC;
	sdcard_type_t type;
} sdcard_t;

//вспопогательные функции
extern err_t sdcard_operation_complete_state(sdcard_t* sdcard);
extern err_t sdcard_change_current_state(sdcard_t* sdcard);
extern void sdcard_CSD_fill(sdcard_t* sdcard);
extern void sdcard_CID_fill(sdcard_t* sdcard);
extern void sdcard_type_set(sdcard_t* sdcard);
//CSD
extern err_t sdcard_CSD_TRAN_SPEED_calc(sdcard_t *sdcard, uint8_t csd_version, float *tran_speed);
extern err_t sdcard_CSD_BLOCK_LEN_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* len, uint32_t* len_power);
extern err_t sdcard_CSD_BLOCKNR_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* count);
extern err_t sdcard_CSD_memory_capacity_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* capacity);

//read write
extern err_t sdcard_read(sdcard_t* sdcard, uint32_t* memory_addr, uint32_t* block_addr, uint32_t block_count, uint32_t timeout);

//функции отправки команд
extern err_t sdcard_cmd(sdcard_t* sdcard, const sdcard_cmd_t* cmd, uint32_t argument);
extern err_t sdcard_acmd(sdcard_t* sdcard, const sdcard_acmd_t* cmd, uint32_t argument);


/*
 * FatFS diskio.
 */
#ifdef USE_SDCARD_FATFS_DISKIO

#include "sdcard_diskio.h"

static size_t sdcard_disks_count = 0;
static sdcard_t* sdcard_disks = NULL;

/**
 * Устанавливает для diskio массив SD-карт.
 * @param sdcards Массив SD-карт.
 * @param count Число SD-карт.
 */
static void sdcard_setup_diskio(sdcard_t* sdcards, size_t count)
{
    sdcard_disks = sdcards;
    sdcard_disks_count = count;
}

/**
 * Получает SD-карту по номеру диска.
 * @param pdrv Номер диска.
 * @return SD-карта.
 */
static sdcard_t* sdcard_diskio_get_sdcard(BYTE pdrv)
{
    if(pdrv >= sdcard_disks_count) return NULL;

    return &sdcard_disks[pdrv];
}

DSTATUS disk_initialize(BYTE pdrv)
{
    sdcard_t* sdcard = sdcard_diskio_get_sdcard(pdrv);
    if(sdcard == NULL) return STA_NOINIT;

    return sdcard_disk_initialize(sdcard);
}


DSTATUS disk_status(BYTE pdrv)
{
    sdcard_t* sdcard = sdcard_diskio_get_sdcard(pdrv);
    if(sdcard == NULL) return STA_NOINIT;

    return sdcard_disk_status(sdcard);
}


DRESULT disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    sdcard_t* sdcard = sdcard_diskio_get_sdcard(pdrv);
    if(sdcard == NULL) return RES_PARERR;

    return sdcard_disk_read(sdcard, buff, sector, count);
}

#if _USE_WRITE
DRESULT disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
    sdcard_t* sdcard = sdcard_diskio_get_sdcard(pdrv);
    if(sdcard == NULL) return RES_PARERR;

    return sdcard_disk_write(sdcard, buff, sector, count);
}

#endif // _USE_WRITE

#if _USE_IOCTL
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff)
{
    sdcard_t* sdcard = sdcard_diskio_get_sdcard(pdrv);
    if(sdcard == NULL) return RES_PARERR;

    return sdcard_disk_ioctl(sdcard, cmd, buff);
}
#endif // _USE_IOCTL

#endif // USE_SDCARD_FATFS_DISKIO

#endif /* SDCARD_H_ */
