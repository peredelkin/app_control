/*
 * sdcard_cmd.c
 *
 *  Created on: 10 апр. 2025 г.
 *      Author: Ruslan
 */

#include "sdcard_cmd.h"
#include "sdcard.h"

/*
 * Detailed Command Description
 */

// SD Version 9.10

//All stuff bits should be set to '0'.

/*
 * Resets all cards to idle state.
 * [31:0] stuff bits
 */
const sdcard_cmd_t sdcard_Class0_CMD0 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_GO_IDLE_STATE,
				SDCARD_RESPONSE_NO,
				SDCARD_STATE_IDLE,      //idle
				SDCARD_STATE_IDLE,      //ready
				SDCARD_STATE_IDLE,      //ident
				SDCARD_STATE_IDLE,      //stby
				SDCARD_STATE_IDLE,      //tran
				SDCARD_STATE_IDLE,      //data
				SDCARD_STATE_IDLE,      //rcv
				SDCARD_STATE_IDLE,      //prg
				SDCARD_STATE_IDLE,      //dis
				SDCARD_STATE_ILLEGAL);  //ina

/*
 * Asks any card to send the CID numbers on the CMD line.
 *
 * [31:0] stuff bits
 */
const sdcard_cmd_t sdcard_Class0_CMD2 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ALL_SEND_CID,
				SDCARD_RESPONSE_R2,
				SDCARD_STATE_ILLEGAL,   //idle
				SDCARD_STATE_IDENT,     //ready
				SDCARD_STATE_ILLEGAL,   //ident
				SDCARD_STATE_ILLEGAL,   //stby
				SDCARD_STATE_ILLEGAL,   //tran
				SDCARD_STATE_ILLEGAL,   //data
				SDCARD_STATE_ILLEGAL,   //rcv
				SDCARD_STATE_ILLEGAL,   //prg
				SDCARD_STATE_ILLEGAL,   //dis
				SDCARD_STATE_ILLEGAL);  //ina

/*
 * Ask the card to publish a new relative address (RCA).
 *
 * [31:0] stuff bits
 */
const sdcard_cmd_t sdcard_Class0_CMD3 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_RELATIVE_ADDR,
				SDCARD_RESPONSE_R6,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_STBY,		//ident
				SDCARD_STATE_STBY,		//stby
				SDCARD_STATE_ILLEGAL,	//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Programs the DSR of all cards.
 *
 * [31:16] DSR
 * [15:0] stuff bits
 */
const sdcard_cmd_t sdcard_Class0_CMD4 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SET_DSR,
				SDCARD_RESPONSE_NO,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_STBY,		//stby
				SDCARD_STATE_ILLEGAL,	//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

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
const sdcard_cmd_t sdcard_Class10_CMD6 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SWITCH_FUNC,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_DATA,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Command toggles a card between the stand-by
 * and transfer states or between the programming
 * and disconnect states.
 *
 * [31:16] RCA
 * [15:0] stuff bits
 */
const sdcard_cmd_t sdcard_Class0_CMD7_adressed =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SELECT_DESELECT_CARD,
				SDCARD_RESPONSE_R1b,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_TRAN,		//stby
				SDCARD_STATE_ILLEGAL,	//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_PRG,		//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

const sdcard_cmd_t sdcard_Class0_CMD7_not_adressed =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SELECT_DESELECT_CARD,
				SDCARD_RESPONSE_NO,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_STBY,		//stby
				SDCARD_STATE_STBY,		//tran
				SDCARD_STATE_STBY,		//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_DIS,		//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Sends SD Memory Card interface condition,
 * which includes host supply voltage information
 * and asks the card whether card supports voltage.
 *
 * [31:12]reserved bits
 * [11:8]supply voltage(VHS)
 * [7:0]check pattern
 */
const sdcard_cmd_t sdcard_Class0_CMD8 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_IF_COND,
				SDCARD_RESPONSE_R7,
				SDCARD_STATE_IDLE,		//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_ILLEGAL,	//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Addressed card sends its
 * card-specific data (CSD) on the CMD line.
 *
 * [31:16] RCA
 * [15:0] stuff bits
 */
const sdcard_cmd_t sdcard_Class0_CMD9 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_CSD,
				SDCARD_RESPONSE_R2,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_STBY,		//stby
				SDCARD_STATE_ILLEGAL,	//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Addressed card sends its card
 * identification (CID) on CMD the line.
 *
 * [31:16] RCA
 * [15:0] stuff bits
 */
const sdcard_cmd_t sdcard_Class0_CMD10 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_CID,
				SDCARD_RESPONSE_R2,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_STBY,		//stby
				SDCARD_STATE_ILLEGAL,	//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Switch to 1.8V bus signaling level.
 *
 * [31:0] reserved bits (all 0)
 */
const sdcard_cmd_t sdcard_Class0_CMD11 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_VOLTAGE_SWITCH,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_READY,		//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_ILLEGAL,	//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Forces the card to stop transmission.
 *
 * [31:0] stuff bits
 */
const sdcard_cmd_t sdcard_Class0_CMD12 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_STOP_TRANSMISSION,
				SDCARD_RESPONSE_R1b,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran (for SDUC only)
				SDCARD_STATE_TRAN,		//data
				SDCARD_STATE_PRG,		//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

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
const sdcard_cmd_t sdcard_Class0_CMD13 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_STATUS,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_STBY,		//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_DATA,		//data
				SDCARD_STATE_RCV,		//rcv
				SDCARD_STATE_PRG,		//prg
				SDCARD_STATE_DIS,		//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Sends an addressed card into the
 *
 * [31:16] RCA
 * [15:0] reserved bits
 */
const sdcard_cmd_t sdcard_Class0_CMD15 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_GO_INACTIVE_STATE,
				SDCARD_RESPONSE_NO,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_INA,		//stby
				SDCARD_STATE_INA,		//tran
				SDCARD_STATE_INA,		//data
				SDCARD_STATE_INA,		//rcv
				SDCARD_STATE_INA,		//prg
				SDCARD_STATE_INA,		//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * In the case of a Standard Capacity SD
 * Memory Card, this command sets the
 * block length (in bytes) for all following
 * block commands (read, write, lock).
 *
 * [31:0] block length
 */
const sdcard_cmd_t sdcard_Class2_CMD16 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SET_BLOCKLEN,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * In the case of a Standard Capacity SD
 * Memory Card, this command, this
 * command reads a block of the size
 * selected by the SET_BLOCKLEN command.
 *
 * [31:0] data address
 */
const sdcard_cmd_t sdcard_Class2_CMD17 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_READ_SINGLE_BLOCK,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_DATA,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Continuously transfers data blocks from
 * card to host until interrupted by a
 * STOP_TRANSMISSION command.
 * Block length is specified the same as
 * READ_SINGLE_BLOCK command.
 *
 * [31:0] data address
 */
const sdcard_cmd_t sdcard_Class2_CMD18 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_READ_MULTIPLE_BLOCK,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_DATA,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * 64 bytes tuning pattern is sent for SDR50 and SDR104.
 *
 * [31:0] reserved bits (all 0)
 */
const sdcard_cmd_t sdcard_Class2_CMD19 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SEND_TUNING_BLOCK,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_DATA,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Speed Class control command.
 *
 * [31:28]Speed Class Control
 * [27:0]See command description
 */
const sdcard_cmd_t sdcard_Class2_CMD20 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SPEED_CLASS_CONTROL,
				SDCARD_RESPONSE_R1b,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_PRG,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Address extension for SDUC cards.
 *
 * [31:6] reserved bits (all 0)
 * [5:0] extended address
 */
const sdcard_cmd_t sdcard_Class2_CMD22 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ADDRESS_EXTENSION,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran (for SDUC only)
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Specify block count for CMD18 and CMD25.
 *
 * [31:0] Block Count
 */
const sdcard_cmd_t sdcard_Class2_CMD23 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SET_BLOCK_COUNT,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * In the case of a Standard Capacity SD
 * Memory Card, this command sets the
 * block length (in bytes) for all following
 * block commands (read, write, lock).
 *
 * [31:0] block length
 */
const sdcard_cmd_t sdcard_Class4_CMD16 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SET_BLOCKLEN,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Speed Class control command.
 *
 * [31:28]Speed Class Control
 * [27:0]See command description
 */
const sdcard_cmd_t sdcard_Class4_CMD20 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SPEED_CLASS_CONTROL,
				SDCARD_RESPONSE_R1b,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_PRG,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Address extension for SDUC cards.
 *
 * [31:6] reserved bits (all 0)
 * [5:0] extended address
 */
const sdcard_cmd_t sdcard_Class4_CMD22 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ADDRESS_EXTENSION,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran (for SDUC only)
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Specify block count for CMD18 and CMD25.
 *
 * [31:0] Block Count
 */
const sdcard_cmd_t sdcard_Class4_CMD23 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_SET_BLOCK_COUNT,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * In case of SDSC Card, block length is
 * set by the SET_BLOCKLEN command.
 *
 * [31:0] data address
 */
const sdcard_cmd_t sdcard_Class4_CMD24 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_WRITE_BLOCK,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_RCV,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Continuously writes blocks of data until
 * a STOP_TRANSMISSION follows.
 * Block length is specified the same as
 * WRITE_BLOCK command.
 *
 * [31:0] data address
 */
const sdcard_cmd_t sdcard_Class4_CMD25 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_WRITE_MULTIPLE_BLOCK,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_RCV,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Programming of the programmable
 * bits of the CSD.
 */
const sdcard_cmd_t sdcard_Class4_CMD27 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_PROGRAM_CSD,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_RCV,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Sets the address of the first write
 * block to be erased.
 *
 * [31:0] data address
 */
const sdcard_cmd_t sdcard_Class5_CMD32 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ERASE_WR_BLK_START,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Sets the address of the last write
 * block of the continuous range to be
 * erased.
 *
 * [31:0] data address
 */
const sdcard_cmd_t sdcard_Class5_CMD33 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ERASE_WR_BLK_END,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Erase Function:
 * 00000001h = Discard
 * 00000002h = FULE
 * Others = Erase
 *
 * [31:0] Erase Function
 */
const sdcard_cmd_t sdcard_Class5_CMD38 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_ERASE,
				SDCARD_RESPONSE_R1b,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_PRG,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Select an active partition.
 *
 * [31:24] Partition ID
 * [23:0] Reserved
 */
//const sdcard_cmd_t sdcard_CMD39 =
//		SDCARD_CMD_MAKE(
//				SDCARD_CMD_SELECT_CARD_PARTITION,
//				SDCARD_REPLY_R1b,
//				SDIO_RESP_TYPE_SHORT,
//				SDIO_RESP_CRC_INCLUDED);

/*
 * Indicates to the card that the next
 * command is an application specific
 * command rather than a standard
 * command.
 *
 * [31:16] RCA
 * [15:0] stuff bits
 */
const sdcard_cmd_t sdcard_Class8_CMD55 =
		SDCARD_CMD_MAKE(
				SDCARD_CMD_APP_CMD,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_IDLE,		//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_STBY,		//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_DATA,		//data
				SDCARD_STATE_RCV,		//rcv
				SDCARD_STATE_PRG,		//prg
				SDCARD_STATE_DIS,		//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Defines the data bus width ('00'=1bit
 * or '10'=4 bits bus) to be used for data
 * transfer. The allowed data bus widths
 * are given in SCR register.
 *
 * [31:2] stuff bits
 * [1:0]bus width
 */
const sdcard_acmd_t sdcard_Class8_ACMD6 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SET_BUS_WIDTH,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Send the SD Status.
 *
 * [31:0] stuff bits
 */
const sdcard_acmd_t sdcard_Class8_ACMD13 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SD_STATUS,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_DATA,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

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
const sdcard_acmd_t sdcard_Class8_ACMD22 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SEND_NUM_WR_BLOCKS,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_DATA,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

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
const sdcard_acmd_t sdcard_Class8_ACMD23 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SET_WR_BLK_ERASE_COUNT,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

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
const sdcard_acmd_t sdcard_Class8_ACMD41 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SD_SEND_OP_COND,
				SDCARD_RESPONSE_R3,
				SDCARD_STATE_READY,		//idle (ready if OCR check is OK and card is not busy)
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_ILLEGAL,	//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Connect[1]/Disconnect[0] the 50
 * KOhm pull-up resistor on CD/DAT3
 * (pin 1) of the card.
 *
 * [31:1] stuff bits
 * [0] set_cd
 */
const sdcard_acmd_t sdcard_Class8_ACMD42 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SET_CLR_CARD_DETECT,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_TRAN,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Reads the SD Configuration Register (SCR).
 *
 * [31:0] stuff bits
 */
const sdcard_acmd_t sdcard_Class8_ACMD51 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SEND_SCR,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_DATA,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

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
const sdcard_acmd_t sdcard_Class8_ACMD53 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SECURE_RECEIVE,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_DATA,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina

/*
 * Continuously transfers data blocks
 * from host to device. 3
 * Bus timing of this command is
 * equivalent to a multi-block write
 * command, but the maximum busy
 * length to write by ACMD54 is 1
 * second.
 *
 * [31:24] Security Protocol
 * [23:16] Security Protocol Specific 1 (SPSP1)
 * [15:8] Security Protocol Specific 0 (SPSP0)
 * [7:0] SD Security Specific Field (SSSF)
 */
const sdcard_acmd_t sdcard_Class8_ACMD54 =
		SDCARD_CMD_MAKE(
				SDCARD_ACMD_SECURE_SEND,
				SDCARD_RESPONSE_R1,
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_RCV,		//tran
				SDCARD_STATE_ILLEGAL,	//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_ILLEGAL,	//prg
				SDCARD_STATE_ILLEGAL,	//dis
				SDCARD_STATE_ILLEGAL);	//ina


static void sdcard_resp_setup(sdcard_t* sdcard) {
	sdcard->resp_wait = SDIO_RESP_WAIT_ENA;
	sdcard->resp_long = SDIO_RESP_TYPE_SHORT;
	sdcard->resp_crc = SDIO_RESP_CRC_INCLUDED;

	switch (sdcard->cmd->response_type) {
	case SDCARD_RESPONSE_NO:
		sdcard->resp_wait = SDIO_RESP_WAIT_DIS;
		break;

	case SDCARD_RESPONSE_R2:
		sdcard->resp_long = SDIO_RESP_TYPE_LONG;
		//no break

	case SDCARD_RESPONSE_R3:
		//no break

	case SDCARD_RESPONSE_R4b:
		sdcard->resp_crc = SDIO_RESP_CRC_NOT_INCLUDED;
		break;

	default:
		break;
	}
}


err_t sdcard_cmd_send(sdcard_t* sdcard, const sdcard_cmd_t* cmd, uint32_t argument) {
	if(sdcard == NULL || cmd == NULL) return E_NULL_POINTER;

	if(cmd->state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_SDCARD_ILLEGAL_COMMAND;

	sdcard->cmd = (sdcard_cmd_t*) cmd;

	sdcard_resp_setup(sdcard);

	sdio_command(
			argument,
			sdcard->cmd->index,
			sdcard->resp_wait,
			sdcard->resp_long,
			SDIO_INT_WAIT_DIS,
			SDIO_PEND_WAIT_DIS,
			SDIO_CPSM_EN,
			SDIO_SUSPEND_DIS,
			SDIO_CMD_COMPLETION_DIS,
			SDIO_nIEN_DIS,
			SDIO_ATACMD_DIS);

	return E_NO_ERROR;
}

err_t sdcard_acmd_send(sdcard_t* sdcard, const sdcard_acmd_t* cmd, uint32_t argument) {
	if(sdcard == NULL || cmd == NULL) return E_NULL_POINTER;

	if(cmd->state[sdcard->current_state] == SDCARD_STATE_ILLEGAL) return E_SDCARD_ILLEGAL_COMMAND;

	sdcard->cmd = (sdcard_cmd_t*) cmd;

	sdcard_resp_setup(sdcard);

	sdio_command(
			argument,
			sdcard->cmd->index,
			sdcard->resp_wait,
			sdcard->resp_long,
			SDIO_INT_WAIT_DIS,
			SDIO_PEND_WAIT_DIS,
			SDIO_CPSM_EN,
			SDIO_SUSPEND_DIS,
			SDIO_CMD_COMPLETION_DIS,
			SDIO_nIEN_DIS,
			SDIO_ATACMD_DIS);

	return E_NO_ERROR;
}












