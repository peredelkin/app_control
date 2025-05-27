/**
 * @file sdcard.h Библиотека для работы с SD-картами по SPI.
 */

#ifndef SDCARD_H_
#define SDCARD_H_

//#include <stm32f10x.h>
#include "defs/defs.h"
//#include "spi/spi.h"
#include "sdio/sdio.h"
#include "gpio/gpio.h"
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
 * Ошибки
 */
//! Нельзя выполнить команду в этом состоянии
#define E_SDCARD_CMD_STATE_MISMATCH (E_SDCARD_BASE + 1)

//! Несоответствие индекса в ответе
#define E_SDCARD_RESPONSE_INDEX_MISMATCH (E_SDCARD_BASE + 2)
//! Несоответствие состояний карты и драйвера
#define E_SDCARD_RESPONSE_STATE_MISMATCH (E_SDCARD_BASE + 3)
//! Error in the sequence of the authentication process
#define E_SDCARD_RESPONSE_AKE_SEQ_ERROR (E_SDCARD_BASE + 4)
//! Set when only partial address
//! space was erased due to existing
//! write protected blocks or the
//! temporary write protected or write
//! protected until power cycle or
//! permanent write protected card
//! was erased.
#define E_SDCARD_RESPONSE_WP_ERASE_SKIP (E_SDCARD_BASE + 5)
//! Can be either one of the following errors:
//! - The read only section of the CSD does not match the card content.
//! - An attempt to reverse the copy (set as original) or permanent WP (unprotected) bits was made.
#define E_SDCARD_RESPONSE_CSD_OVERWRITE (E_SDCARD_BASE + 6)
//! A general or an unknown error occurred during the operation.
#define E_SDCARD_RESPONSE_ERROR (E_SDCARD_BASE + 7)
//! Internal card controller error
#define E_SDCARD_RESPONSE_CC_ERROR (E_SDCARD_BASE + 8)
//! Card internal ECC was applied butfailed to correct the data.
#define E_SDCARD_RESPONSE_CARD_ECC_FAILED (E_SDCARD_BASE + 9)
//! Command not legal for the cardstate
#define E_SDCARD_RESPONSE_ILLEGAL_COMMAND (E_SDCARD_BASE + 10)
//! The CRC check of the previous command failed.
#define E_SDCARD_RESPONSE_COM_CRC_ERROR (E_SDCARD_BASE + 11)
//! Set when a sequence or passworderror has been detected in lock/unlock card command.
#define E_SDCARD_RESPONSE_LOCK_UNLOCK_FAILED (E_SDCARD_BASE + 12)
//! Set when the host attempts to write
//! to a protected block or to the
//! temporary write protected card or
//! write protected until power cycle
//! card or permanent write protected
//! card.
#define E_SDCARD_RESPONSE_WP_VIOLATION (E_SDCARD_BASE + 13)
//! An invalid selection of write-blocksfor erase occurred.
#define E_SDCARD_RESPONSE_ERASE_PARAM (E_SDCARD_BASE + 14)
//! An error in the sequence of erasecommands occurred.
#define E_SDCARD_RESPONSE_ERASE_SEQ_ERROR (E_SDCARD_BASE + 15)
//! The transferred block length is not
//! allowed for this card, or the number
//! of transferred bytes does not match
//! the block length.
#define E_SDCARD_RESPONSE_BLOCK_LEN_ERROR (E_SDCARD_BASE + 16)
//! A misaligned address which did not
//! match the block length was used in
//! the command.
#define E_SDCARD_RESPONSE_ADDRESS_ERROR (E_SDCARD_BASE + 17)
//! The command's argument was out of the allowed range for this card.
#define E_SDCARD_RESPONSE_OUT_OF_RANGE (E_SDCARD_BASE + 18)

//! Перечисление типа SD-карты.
typedef enum _SD_Card_Type {
    SDCARD_TYPE_UNKNOWN = 0, //!< Неизвестный тип карты.
    SDCARD_TYPE_SC,//!< SDCARD_TYPE_SC
	SDCARD_TYPE_HC_XC,//!< SDCARD_TYPE_HC_XC
	SDCARD_TYPE_UC    //!< SDCARD_TYPE_UC
} sdcard_type_t;


typedef struct {
	const gpio_pin_t* dat0;
	const gpio_pin_t* pwr;
	const gpio_pin_t* cd;
} sdcard_gpio;


//! Структура SD-карты.
typedef struct _SD_Card {
	dma_t dma;
	dma_n_stream_n_t dma_stream;
	sdcard_state_t current_state; //!< Текущее состояние карты.
	sdcard_cmd_t* cmd; //!< Указатель на выполняемую команду
	sdio_respwait_t resp_wait; //!< Ожидается ли ответ
	sdio_resptype_t resp_long; //!< Длинный ли ответ
	sdio_resp_crc_include_t resp_crc; //!< Содержит ли ответ контрольную сумму
	sdcard_response_t response; //!< Ответы
	uint32_t RCA; //!< Адрес карты
	sdcard_reg_CID_t CID;
	sdcard_reg_CSD_t CSD;
	sdcard_ccc_t CCC; //!< Классы поддерживаемых картой команд
	sdcard_type_t type; //!< Тип карты
	err_t cmd_err; //!< Ошибки при отправке команды
	err_t data_err; //!< Ошибки при обмене с картой
	err_t resp_err; //!< Ошибки в ответе карты
	err_t dma_err; //!< Ошибки DMA
	bool inserted;
	bool initialized;
	sdcard_gpio gpio;
} sdcard_t;

//вспопогательные функции
extern err_t sdcard_card_reset(sdcard_t* sdcard);
extern err_t sdcard_card_initialization(sdcard_t* sdcard);
extern err_t sdcard_card_CID_read_any(sdcard_t* sdcard);
extern err_t sdcard_card_RCA_read(sdcard_t* sdcard);
extern err_t sdcard_card_CSD_read(sdcard_t* sdcard);
extern err_t sdcard_card_CID_read(sdcard_t* sdcard);
extern err_t sdcard_card_select(sdcard_t* sdcard);
extern err_t sdcard_card_deselect(sdcard_t* sdcard);

extern bool sdcard_identified(sdcard_t* sdcard);
extern bool sdcard_initialized(sdcard_t* sdcard);

extern bool sdcard_card_detect(sdcard_t* sdcard);
extern void sdcard_card_pwr_on(sdcard_t* sdcard);
extern void sdcard_card_pwr_off(sdcard_t* sdcard);

extern void sdcard_sdio_power_off();

extern err_t sdcard_card_init(sdcard_t *sdcard);

//CSD
extern err_t sdcard_CSD_TRAN_SPEED_calc(sdcard_t *sdcard, uint8_t csd_version, float *tran_speed);
extern err_t sdcard_CSD_BLOCK_LEN_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* len, uint32_t* len_power);
extern err_t sdcard_CSD_BLOCKNR_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* count);
extern err_t sdcard_CSD_memory_capacity_calc(sdcard_t* sdcard, uint8_t csd_version, uint64_t* capacity);

//read write
extern err_t sdcard_read(sdcard_t* sdcard, uint32_t* memory_addr, uint64_t block_addr, uint32_t block_count, uint32_t timeout);
extern err_t sdcard_write(sdcard_t* sdcard, uint32_t* memory_addr, uint64_t block_addr, uint32_t block_count, uint32_t timeout);

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
