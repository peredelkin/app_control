/*
 * sdcard_cmd.c
 *
 *  Created on: 10 апр. 2025 г.
 *      Author: Ruslan
 */

#include "sdcard_cmd.h"

/*
 * Detailed Command Description
 */

// SD Version 9.10

/*
 * Resets all cards to idle state.
 */
const sdcard_cmd_t sdcard_CMD0 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_GO_IDLE_STATE,
				SDCARD_REPLY_NO,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Asks any card to send the CID numbers on the CMD line.
 */
const sdcard_cmd_t sdcard_CMD2 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ALL_SEND_CID,
				SDCARD_REPLY_R2,
				SDIO_RESP_TYPE_LONG,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Ask the card to publish a new relative address (RCA).
 */
const sdcard_cmd_t sdcard_CMD3 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_RELATIVE_ADDR,
				SDCARD_REPLY_R6,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Programs the DSR of all cards.
 */
const sdcard_cmd_t sdcard_CMD4 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SET_DSR,
				SDCARD_REPLY_NO,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Command toggles a card between the stand-by
 * and transfer states or between the programming
 * and disconnect states.
 */
const sdcard_cmd_t sdcard_CMD7 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SELECT_DESELECT_CARD,
				SDCARD_REPLY_R1b,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Sends SD Memory Card interface condition,
 * which includes host supply voltage information
 * and asks the card whether card supports voltage.
 */
const sdcard_cmd_t sdcard_CMD8 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_IF_COND,
				SDCARD_REPLY_R7,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Addressed card sends its
 * card-specific data (CSD) on the CMD line.
 */
const sdcard_cmd_t sdcard_CMD9 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_CSD,
				SDCARD_REPLY_R2,
				SDIO_RESP_TYPE_LONG,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Addressed card sends its card
 * identification (CID) on CMD the line.
 */
const sdcard_cmd_t sdcard_CMD10 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_CID,
				SDCARD_REPLY_R2,
				SDIO_RESP_TYPE_LONG,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Switch to 1.8V bus signaling level.
 */
const sdcard_cmd_t sdcard_CMD11 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_VOLTAGE_SWITCH,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Forces the card to stop transmission.
 */
const sdcard_cmd_t sdcard_CMD12 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_STOP_TRANSMISSION,
				SDCARD_REPLY_R1b,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * CQ not enabled:
 * [15] = ‘0’ or ‘1’, Addressed card sends its status register.
 * CQ enabled:
 * [15]=’0’, Addressed card sends its status register.
 * [15]=’1’, Addressed card sends task status register.
 */
const sdcard_cmd_t sdcard_CMD13 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_STATUS,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Sends an addressed card into the
 */
const sdcard_cmd_t sdcard_CMD15 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_GO_INACTIVE_STATE,
				SDCARD_REPLY_NO,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * In the case of a Standard Capacity SD
 * Memory Card, this command sets the
 * block length (in bytes) for all following
 * block commands (read, write, lock).
 */
const sdcard_cmd_t sdcard_CMD16 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SET_BLOCKLEN,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * In the case of a Standard Capacity SD
 * Memory Card, this command, this
 * command reads a block of the size
 * selected by the SET_BLOCKLEN command.
 */
const sdcard_cmd_t sdcard_CMD17 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_READ_SINGLE_BLOCK,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Continuously transfers data blocks from
 * card to host until interrupted by a
 * STOP_TRANSMISSION command.
 * Block length is specified the same as
 * READ_SINGLE_BLOCK command.
 */
const sdcard_cmd_t sdcard_CMD18 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_READ_MULTIPLE_BLOCK,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * 64 bytes tuning pattern is sent for SDR50 and SDR104.
 */
const sdcard_cmd_t sdcard_CMD19 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_TUNING_BLOCK,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Speed Class control command.
 */
const sdcard_cmd_t sdcard_CMD20 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SPEED_CLASS_CONTROL,
				SDCARD_REPLY_R1b,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Address extension for SDUC cards.
 */
const sdcard_cmd_t sdcard_CMD22 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ADDRESS_EXTENSION,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Specify block count for CMD18 and CMD25.
 */
const sdcard_cmd_t sdcard_CMD23 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SET_BLOCK_COUNT,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * In case of SDSC Card, block length is
 * set by the SET_BLOCKLEN command.
 */
const sdcard_cmd_t sdcard_CMD24 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_WRITE_BLOCK,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Continuously writes blocks of data until
 * a STOP_TRANSMISSION follows.
 * Block length is specified the same as
 * WRITE_BLOCK command.
 */
const sdcard_cmd_t sdcard_CMD25 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_WRITE_MULTIPLE_BLOCK,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Sets the address of the first write
 * block to be erased.
 */
const sdcard_cmd_t sdcard_CMD32 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ERASE_WR_BLK_START,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Sets the address of the last write
 * block of the continuous range to be
 * erased.
 */
const sdcard_cmd_t sdcard_CMD33 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ERASE_WR_BLK_END,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Erase Function
 * 00000001h = Discard
 * 00000002h = FULE
 * Others = Erase
 */
const sdcard_cmd_t sdcard_CMD38 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ERASE,
				SDCARD_REPLY_R1b,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Indicates to the card that the next
 * command is an application specific
 * command rather than a standard
 * command.
 */
const sdcard_cmd_t sdcard_CMD55 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_APP_CMD,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

