/**
 * @file sdcard_cmd.h Декларация типа команды SD-карты.
 */

#ifndef SDCARD_CMD_H_
#define SDCARD_CMD_H_

#include <stdint.h>
#include <assert.h>
#include "sdio/sdio.h"
#include "sdcard_response.h"

typedef enum {
	SDCARD_CMD_GO_IDLE_STATE = 0,

	SDCARD_CMD_ALL_SEND_CID = 2,
	SDCARD_CMD_SEND_RELATIVE_ADDR,
	SDCARD_CMD_SET_DSR,

	SDCARD_CMD_SWITCH_FUNC = 6,
	SDCARD_CMD_SELECT_DESELECT_CARD,
	SDCARD_CMD_SEND_IF_COND,
	SDCARD_CMD_SEND_CSD,
	SDCARD_CMD_SEND_CID,
	SDCARD_CMD_VOLTAGE_SWITCH,
	SDCARD_CMD_STOP_TRANSMISSION,
	SDCARD_CMD_SEND_STATUS,

	SDCARD_CMD_GO_INACTIVE_STATE = 15,
	SDCARD_CMD_SET_BLOCKLEN,
	SDCARD_CMD_READ_SINGLE_BLOCK,
	SDCARD_CMD_READ_MULTIPLE_BLOCK,
	SDCARD_CMD_SEND_TUNING_BLOCK,
	SDCARD_CMD_SPEED_CLASS_CONTROL,

	SDCARD_CMD_ADDRESS_EXTENSION = 22,
	SDCARD_CMD_SET_BLOCK_COUNT,
	SDCARD_CMD_WRITE_BLOCK,
	SDCARD_CMD_WRITE_MULTIPLE_BLOCK,

	SDCARD_CMD_PROGRAM_CSD = 27,
	SDCARD_CMD_SET_WRITE_PROT,
	SDCARD_CMD_CLR_WRITE_PROT,
	SDCARD_CMD_SEND_WRITE_PROT,

	SDCARD_CMD_ERASE_WR_BLK_START = 32,
	SDCARD_CMD_ERASE_WR_BLK_END,

	SDCARD_CMD_ERASE = 38,
	SDCARD_CMD_SELECT_CARD_PARTITION,

	SDCARD_CMD_Q_MANAGEMENT = 43,
	SDCARD_CMD_Q_TASK_INFO_A,
	SDCARD_CMD_Q_TASK_INFO_B,
	SDCARD_CMD_Q_RD_TASK,
	SDCARD_CMD_Q_WR_TASK,
	SDCARD_CMD_READ_EXTR_SINGLE,
	SDCARD_CMD_WRITE_EXTR_SINGLE,

	SDCARD_CMD_APP_CMD = 55,
	SDCARD_CMD_GEN_CMD,

	SDCARD_CMD_READ_EXTR_MULTI = 58,
	SDCARD_CMD_WRITE_EXTR_MULTI

} sdcard_cmd_list_t;


typedef enum {
	SDCARD_ACMD_SET_BUS_WIDTH = 6,

	SDCARD_ACMD_SD_STATUS = 13,

	SDCARD_ACMD_SEND_NUM_WR_BLOCKS = 22,
	SDCARD_ACMD_SET_WR_BLK_ERASE_COUNT,

	SDCARD_ACMD_SD_SEND_OP_COND = 41,
	SDCARD_ACMD_SET_CLR_CARD_DETECT,

	SDCARD_ACMD_SEND_SCR = 51,

	SDCARD_ACMD_SECURE_RECEIVE = 53,
	SDCARD_ACMD_SECURE_SEND

} sdcard_acmd_list_t;

typedef enum {
	SDCARD_STATE_ILLEGAL = -1,
	SDCARD_STATE_IDLE,
	SDCARD_STATE_READY,
	SDCARD_STATE_IDENT,
	SDCARD_STATE_STBY,
	SDCARD_STATE_TRAN,
	SDCARD_STATE_DATA,
	SDCARD_STATE_RCV,
	SDCARD_STATE_PRG,
	SDCARD_STATE_DIS,
	SDCARD_STATE_INA,
	SDCARD_STATE_COUNT
} sdcard_state_t;

//! Структура команды SD-карты.
typedef struct _SD_Card_Cmd {
	sdcard_cmd_list_t index; //!< Индекс команды.
	sdcard_response_type_t response_type; //!< Тип ответа
	sdcard_state_t state[SDCARD_STATE_COUNT]; //!< Состояние SD карты в случае выполнения команды
} sdcard_cmd_t;

typedef struct _SD_Card_ACmd {
	sdcard_acmd_list_t index; //!< Индекс команды.
	sdcard_response_type_t response_type; //!< Тип ответа
	sdcard_state_t state[SDCARD_STATE_COUNT]; //!< Состояние SD карты в случае выполнения команды
} sdcard_acmd_t;

//! Инициализация команды SD-карты по месту объявления. SDIO
#define SDCARD_CMD_MAKE(ind, resp, idle, ready, ident, stby, train, data, rcv, prg, dis, ina)\
		{\
		    .index = ind,\
			.response_type = resp,\
			.state[0] = idle,\
			.state[1] = ready,\
			.state[2] = ident,\
			.state[3] = stby,\
			.state[4] = train,\
			.state[5] = data,\
			.state[6] = rcv,\
			.state[7] = prg,\
			.state[8] = dis,\
			.state[9] = ina,\
		}

#endif /* SDCARD_CMD_H_ */
