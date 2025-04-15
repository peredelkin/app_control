/*
 * sdcard_state.c
 *
 *  Created on: 15 апр. 2025 г.
 *      Author: Ruslan
 */

#include "sdcard_state.h"

const sdcard_state_cmd_t sdcard_state_operation_complete =
		SDCARD_STATE_CMD_MAKE(
				SDCARD_STATE_ILLEGAL,	//idle
				SDCARD_STATE_ILLEGAL,	//ready
				SDCARD_STATE_ILLEGAL,	//ident
				SDCARD_STATE_ILLEGAL,	//stby
				SDCARD_STATE_ILLEGAL,	//tran
				SDCARD_STATE_TRAN,		//data
				SDCARD_STATE_ILLEGAL,	//rcv
				SDCARD_STATE_TRAN,		//prg
				SDCARD_STATE_STBY,		//dis
				SDCARD_STATE_ILLEGAL);	//ina

const sdcard_state_cmd_t sdcard_state_class0_CMD0 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD2 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD3 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD4 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD7_adressed =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD7_not_adressed =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD8 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD9 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD10 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD11 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD12 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD13 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class0_CMD15 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class2_CMD16 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class2_CMD17 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class2_CMD18 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class2_CMD19 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class2_CMD20 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class2_CMD22 =
		SDCARD_STATE_CMD_MAKE(
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

const sdcard_state_cmd_t sdcard_state_class2_CMD23 =
		SDCARD_STATE_CMD_MAKE(
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

err_t sdcard_card_state_transition_cmd(sdcard_t* sdcard) {

}

err_t sdcard_card_state_transition_acmd(sdcard_t* sdcard) {

}





















