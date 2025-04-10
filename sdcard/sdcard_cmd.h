/**
 * @file sdcard_cmd.h Декларация типа команды SD-карты.
 */

#ifndef SDCARD_CMD_H_
#define SDCARD_CMD_H_

#include <stdint.h>
#include <assert.h>
#include "sdio/sdio.h"
#include "sdcard_reply.h"


//! Размер команды SD-карты.
#define SDCARD_CMD_SIZE 5 //6 SPI
//! Размер команды SD-карты без поля контрольной суммы.
#define SDCARD_CMD_SIZE_CRC 5

// Поля команды.
//! Стартовый бит команды.
#define SDCARD_CMD_START_BIT 0x80
//! Бит передачи команды.
#define SDCARD_CMD_TRANSMISSION_BIT 0x40
//! Индекс.
#define SDCARD_CMD_INDEX_MASK 0x3f
#define SDCARD_CMD_INDEX_OFFSET 0
//! Контрольная сумма.
#define SDCARD_CMD_CRC7_MASK 0xfe
#define SDCARD_CMD_CRC7_OFFSET 1
//! Стоповый бит.
#define SDCARD_CMD_STOP_BIT 0x1

//! Структура команды SD-карты.
typedef struct _SD_Card_Cmd {
	sdcard_cmd_list_t index; //!< Индекс команды.
	sdcard_reply_type_t response_type; //!< Тип ответа
	sdio_resptype_t response_long; //!< Длинный или короткий ответ
	sdio_resp_crc_include_t response_include_crc; //!< Есть ли CRC в ответе
} sdcard_cmd_t;

//! Инициализация команды SD-карты по месту объявления. SDIO
#define SDCARD_CMD_MAKE(ind, resp, resp_long, resp_include_crc)\
		{\
		    .index = ind,\
			.response_type = resp,\
			.response_long = resp_long,\
			.response_include_crc = resp_include_crc,\
		}


/*
 * Команды карты памяти.
 */

typedef enum {
	SDCARD_CMD_GO_IDLE_STATE = 0,				//! Программный сброс карты.
	SDCARD_CMD_SEND_OP_COND,					//! Инициализация карты MMC.
	SDCARD_CMD_ALL_SEND_CID,					//! Запрашивает у любой карты послать номер CID
	SDCARD_CMD_SEND_RELATIVE_ADDR,				//! Запрашивает карту опубликовать новый относительный адрес
	SDCARD_CMD_SET_DSR,							//! Программирует DSP у всех карт.

	SDCARD_CMD_SELECT_DESELECT_CARD = 7,		//! Переключает между Standby и Transfer или Programming и Disconnect.
	SDCARD_CMD_SEND_IF_COND,					//! Посылается информация об интерфейсе и запрашивается - может ли она работать от этого напряжения.
	SDCARD_CMD_SEND_CSD,						//! Адресованная карта пошлет свои специфические данные (CSD)
	SDCARD_CMD_SEND_CID,						//! Адресованная карта пошлет свои идентификационные данные (CID)

	SDCARD_CMD_STOP_TRANSMISSION = 12,			//! Остановка передачи данных.
	SDCARD_CMD_SEND_STATUS,						//! Запрашивает выбранную карту послать её регистр статуса.

	SDCARD_CMD_GO_INACTIVE_STATE = 15,			//! Переводит адресованную карту в неактивное состояние
	SDCARD_CMD_SET_BLOCKLEN,					//! Устанавливает размер блока для карт SDSC. Размер блока карт SDHC равен 512 байт.
	SDCARD_CMD_READ_SINGLE_BLOCK,				//! Чтение блока. В аргументе передается адрес байта для SDSC или блока для SDHC.
	SDCARD_CMD_READ_MULTIPLE_BLOCK,				//! Чтение нескольких блоков. В аргументе передается адрес байта для SDSC или блока для SDHC.

	SDCARD_ACMD_SEND_NUM_WR_BLOCKS = 22,		//! Получения числа успешно записанных блоков.
	SDCARD_ACMD_SET_WR_BLK_ERASE_COUNT,			//! Установка числа записываемых блоков для стирания.
	SDCARD_CMD_WRITE_SINGLE_BLOCK,				//! Запись блока. В аргументе передается адрес байта для SDSC или блока для SDHC.
	SDCARD_CMD_WRITE_MULTIPLE_BLOCK,			//! Запись нескольких блоков. В аргументе передается адрес байта для SDSC или блока для SDHC.

	SDCARD_CMD_ERASE_WR_BLK_START_ADDR = 32,	//! Стирание блока. В аргументе передается адрес начального байта для SDSC или блока для SDHC.
	SDCARD_CMD_ERASE_WR_BLK_END_ADDR,			//! Стирание блока. В аргументе передается адрес конечного байта для SDSC или блока для SDHC.

	SDCARD_CMD_MMC_TAG_ERASE_GROUP_START = 35,	//! Стирание групп блоков MMS. В аргументе передается номер начальной групы MMS.
	SDCARD_CMD_MMC_TAG_ERASE_GROUP_END,			//! Стирание групп блоков MMS. В аргументе передается номер конечной группы MMS.

	SDCARD_CMD_ERASE = 38,						//! Стирает выбранные блоки.

	SDCARD_ACMD_SD_SEND_OP_COND = 41,			//! Инициализация карты.
	SDCARD_ACMD_SET_CLR_CARD_DETECT,			//! Управление 50 кОм подтяжкой на пину CardDetect карты.

	SDCARD_CMD_APP_CMD = 55,					//! Установка следующей команды приложения.

	SDCARD_CMD_READ_OCR = 58,					//! Чтение регистра OCR.
	SDCARD_CMD_CRC_ON_OFF,						//! Включение/выключение проверки CRC.
} sdcard_cmd_list_t;

#endif /* SDCARD_CMD_H_ */
