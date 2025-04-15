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

//All stuff bits should be set to '0'.

/*
 * Resets all cards to idle state.
 * [31:0] stuff bits
 */
const sdcard_cmd_t sdcard_CMD0 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_GO_IDLE_STATE,
				SDCARD_REPLY_NO,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Asks any card to send the CID numbers on the CMD line.
 *
 * [31:0] stuff bits
 */
const sdcard_cmd_t sdcard_CMD2 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ALL_SEND_CID,
				SDCARD_REPLY_R2,
				SDIO_RESP_TYPE_LONG,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Ask the card to publish a new relative address (RCA).
 *
 * [31:0] stuff bits
 */
const sdcard_cmd_t sdcard_CMD3 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_RELATIVE_ADDR,
				SDCARD_REPLY_R6,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Programs the DSR of all cards.
 *
 * [31:16] DSR
 * [15:0] stuff bits
 */
const sdcard_cmd_t sdcard_CMD4 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SET_DSR,
				SDCARD_REPLY_NO,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Checks switchable function (mode 0)
 * and switch card function (mode 1)
 *
 * [31] Mode:
 * 		0:Check function
 * 		1:Switch function
 * [30:24] reserved (All '0')
 * [23:20] reserved for function group 6 (0h or Fh)
 * [19:16] reserved for function group 5 (0h or Fh)
 * [15:12] function group 4 for Power Limit
 * [11:8] function group 3 for Drive Strength
 * [7:4] function group 2 for Command System
 * [3:0] function group 1 for Access Mode
 */
const sdcard_cmd_t sdcard_CMD6 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SWITCH_FUNC,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Command toggles a card between the stand-by
 * and transfer states or between the programming
 * and disconnect states.
 *
 * [31:16] RCA
 * [15:0] stuff bits
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
 *
 * [31:12]reserved bits
 * [11:8]supply voltage(VHS)
 * [7:0]check pattern
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
 *
 * [31:16] RCA
 * [15:0] stuff bits
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
 *
 * [31:16] RCA
 * [15:0] stuff bits
 */
const sdcard_cmd_t sdcard_CMD10 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_CID,
				SDCARD_REPLY_R2,
				SDIO_RESP_TYPE_LONG,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Switch to 1.8V bus signaling level.
 *
 * [31:0] reserved bits (all 0)
 */
const sdcard_cmd_t sdcard_CMD11 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_VOLTAGE_SWITCH,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Forces the card to stop transmission.
 *
 * [31:0] stuff bits
 */
const sdcard_cmd_t sdcard_CMD12 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_STOP_TRANSMISSION,
				SDCARD_REPLY_R1b,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * if CQ not enabled:
 * [15] = ‘0’ or ‘1’, Addressed card sends its status register.
 * if CQ enabled:
 * [15]=’0’, Addressed card sends its status register.
 * [15]=’1’, Addressed card sends task status register.
 *
 * [31:16] RCA
 * [15] Send Task Status Register
 * [14:0] stuff bits
 */
const sdcard_cmd_t sdcard_CMD13 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_STATUS,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Sends an addressed card into the
 *
 * [31:16] RCA
 * [15:0] reserved bits
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
 *
 * [31:0] block length
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
 *
 * [31:0] data address
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
 *
 * [31:0] data address
 */
const sdcard_cmd_t sdcard_CMD18 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_READ_MULTIPLE_BLOCK,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * 64 bytes tuning pattern is sent for SDR50 and SDR104.
 *
 * [31:0] reserved bits (all 0)
 */
const sdcard_cmd_t sdcard_CMD19 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_TUNING_BLOCK,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Speed Class control command.
 *
 * [31:28]Speed Class Control
 * [27:0]See command description
 */
const sdcard_cmd_t sdcard_CMD20 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SPEED_CLASS_CONTROL,
				SDCARD_REPLY_R1b,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Address extension for SDUC cards.
 *
 * [31:6] reserved bits (all 0)
 * [5:0] extended address
 */
const sdcard_cmd_t sdcard_CMD22 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ADDRESS_EXTENSION,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Specify block count for CMD18 and CMD25.
 *
 * [31:0] Block Count
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
 *
 * [31:0] data address
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
 *
 * [31:0] data address
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
 *
 * [31:0] data address
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
 *
 * [31:0] data address
 */
const sdcard_cmd_t sdcard_CMD33 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ERASE_WR_BLK_END,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Erase Function:
 * 00000001h = Discard
 * 00000002h = FULE
 * Others = Erase
 *
 * [31:0] Erase Function
 */
const sdcard_cmd_t sdcard_CMD38 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ERASE,
				SDCARD_REPLY_R1b,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Select an active partition.
 *
 * [31:24] Partition ID
 * [23:0] Reserved
 */
const sdcard_cmd_t sdcard_CMD39 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SELECT_CARD_PARTITION,
				SDCARD_REPLY_R1b,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Indicates to the card that the next
 * command is an application specific
 * command rather than a standard
 * command.
 *
 * [31:16] RCA
 * [15:0] stuff bits
 */
const sdcard_cmd_t sdcard_CMD55 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_APP_CMD,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Defines the data bus width ('00'=1bit
 * or '10'=4 bits bus) to be used for data
 * transfer. The allowed data bus widths
 * are given in SCR register.
 *
 * [31:2] stuff bits
 * [1:0]bus width
 */
const sdcard_acmd_t sdcard_ACMD6 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SET_BUS_WIDTH,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Send the SD Status.
 *
 * [31:0] stuff bits
 */
const sdcard_acmd_t sdcard_ACMD13 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SD_STATUS,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Send the number of the written (with-
 * out errors) write blocks. Responds with
 * 32bit+CRC data block.
 * If WRITE_BL_PARTIAL='0', the unit of
 * ACMD22 is always 512 byte.
 * If WRITE_BL_PARTIAL='1', the unit of
 * ACMD22 is a block length which was
 * used when the write command was
 * executed.
 *
 * [31:0] stuff bits
 */
const sdcard_acmd_t sdcard_ACMD22 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SEND_NUM_WR_BLOCKS,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Set the number of write blocks to be
 * pre-erased before writing (to be used
 * for faster Multiple Block WR com-
 * mand). "1"=default (one wr block) .
 * Not supported by SDUC card.
 *
 * [31:23] stuff bits
 * [22:0] Number of blocks
 */
const sdcard_acmd_t sdcard_ACMD23 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SET_WR_BLK_ERASE_COUNT,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Sends host capacity support
 * information (HCS) and asks the
 * accessed card to send its operating
 * condition register (OCR) content in the
 * response on the CMD line.
 *
 * [31] reserved bit
 * [30] HCS(OCR[30])
 * [29] reserved for eSD
 * [28] XPC
 * [27:25] reserved bits
 * [24] S18R
 * [23:0] VDD Voltage Window(OCR[23:0])
 */
const sdcard_acmd_t sdcard_ACMD41 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SD_SEND_OP_COND,
				SDCARD_REPLY_R3,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_NOT_INCLUDED);

/*
 * Connect[1]/Disconnect[0] the 50
 * KOhm pull-up resistor on CD/DAT3
 * (pin 1) of the card.
 *
 * [31:1] stuff bits
 * [0] set_cd
 */
const sdcard_acmd_t sdcard_ACMD42 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SET_CLR_CARD_DETECT,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Reads the SD Configuration Register (SCR).
 *
 * [31:0] stuff bits
 */
const sdcard_acmd_t sdcard_ACMD51 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SEND_SCR,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);

/*
 * Continuously transfers data blocks
 * from device to host.
 * Bus timing of this command is
 * equivalent to a multi-block read
 * command, but the maximum data
 * access time from the end bit of
 * ACMD53 is 1 second.
 *
 * [31:24] Security Protocol
 * [23:16] Security Protocol Specific 1 (SPSP1)
 * [15:8] Security Protocol Specific 0 (SPSP0)
 * [7:0] SD Security Specific Field (SSSF)
 */
const sdcard_acmd_t sdcard_ACMD53 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SECURE_RECEIVE,
				SDCARD_REPLY_R1,
				SDIO_RESP_TYPE_SHORT,
				SDIO_RESP_CRC_INCLUDED);















