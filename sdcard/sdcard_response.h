/**
 * @file sdcard_reply.h Декларация ответов SD-карты.
 */

#ifndef SDCARD_REPLY_H_
#define SDCARD_REPLY_H_

#include <stdint.h>
#include <assert.h>

/*
 * Short response: Card Status [39:8]
 * Long response: Card Status [127:96]
 */

// [39:8] Argument field
typedef union {
	struct {
		unsigned RESERVED_0				:1;
		unsigned RESERVED_1				:1;
		unsigned RESERVED_2				:1;
		unsigned AKE_SEQ_ERROR			:1;
		unsigned RESERVED_4				:1;
		unsigned APP_CMD				:1;
		unsigned RESERVED_6				:1;
		unsigned SWITCH_ERROR			:1;
		unsigned READY_FOR_DATA			:1;
		unsigned CURRENT_STATE			:4;
		unsigned ERASE_RESET			:1;
		unsigned CARD_ECC_DISABLED		:1;
		unsigned WP_ERASE_SKIP			:1;
		unsigned CID_CSD_OVERWRITE		:1;
		unsigned RESERVED_17			:1;
		unsigned RESERVED_18			:1;
		unsigned ERROR					:1;
		unsigned CC_ERROR				:1;
		unsigned CARD_ECC_FAILED		:1;
		unsigned ILLEGAL_COMMAND		:1;
		unsigned COM_CRC_ERROR			:1;
		unsigned LOCK_UNLOCK_FAILED		:1;
		unsigned CARD_IS_LOCKED			:1;
		unsigned WP_VIOLATION			:1;
		unsigned ERASE_PARAM			:1;
		unsigned ERASE_SEQ_ERROR		:1;
		unsigned BLOCK_LEN_ERROR		:1;
		unsigned ADDRESS_MISALIGN		:1;
		unsigned ADDRESS_OUT_OF_RANGE	:1;
	} bit;
	uint32_t all;
} sdcard_response_R1_t;

// [127:96] Argument field
typedef union {
	struct {
		unsigned CID_CSD :32;
	} bit;
	uint32_t all;
} sdcard_response_R2_t;

// [39:8] Argument field
typedef union {
	struct {
		unsigned RESERVED_0_14			:15;
		unsigned VDD_2v7_2v8			:1;	//15
		unsigned VDD_2v8_2v9			:1;	//16
		unsigned VDD_2v9_3v0			:1;	//17
		unsigned VDD_3v0_3v1			:1;	//18
		unsigned VDD_3v1_3v2			:1;	//19
		unsigned VDD_3v2_3v3			:1;	//20
		unsigned VDD_3v3_3v4			:1;	//21
		unsigned VDD_3v4_3v5			:1;	//22
		unsigned VDD_3v5_3v6			:1; //23
		unsigned SWITCH_1v8_ACCEPTED	:1; //24
		unsigned RESERVED_25_26			:2;
		unsigned OVER_2TB_SUPPORT		:1; //27
		unsigned RESERVED_28			:1;
		unsigned UHS_2_CARD_STATUS		:1; //29
		unsigned CARD_CAPACITY_STATUS	:1; //30
		unsigned CARD_POWER_UP_STATUS	:1; //31
	} bit;
	uint32_t all;
} sdcard_response_R3_t;

// [39:8] Argument field
typedef union {
	struct {
		unsigned REG_DATA :8;
		unsigned REG_ADDR :8;
		unsigned RCA :16;
	} bit;
	uint32_t all;
} sdcard_response_R4_t;

// [39:8] Argument field
typedef union {
	struct {
		unsigned OCR :24;
		unsigned STUFF :3; //?
		unsigned PRESENT_MEMORY :1;
		unsigned NUMBER_OF_IO_FUNC :3;
		unsigned READY :1;
	} bit;
	uint32_t all;
} sdcard_response_R4b_t;

// [39:8] Argument field
typedef union {
	struct {
		unsigned RES_0_15 :16;
		unsigned RCA :16;
	} bit;
	uint32_t all;
} sdcard_response_R5_t;

// [39:8] Argument field
typedef union {
	struct {
		unsigned RESERVED_0				:1;
		unsigned RESERVED_1				:1;
		unsigned RESERVED_2				:1;
		unsigned AKE_SEQ_ERROR			:1;
		unsigned RESERVED_4				:1;
		unsigned APP_CMD				:1;
		unsigned RESERVED_6				:1;
		unsigned SWITCH_ERROR			:1;
		unsigned READY_FOR_DATA			:1;
		unsigned CURRENT_STATE			:4;
		unsigned ERROR					:1;
		unsigned ILLEGAL_COMMAND		:1;
		unsigned COM_CRC_ERROR			:1;
		unsigned NEW_RCA :16;
	} bit;
	uint32_t all;
} sdcard_response_R6_t;

// [39:8] Argument field
typedef union {
	struct {
		unsigned ECHO :8;
		unsigned VOLTAGE :4;
		unsigned PCI_E :1;
		unsigned PCI_E_1v2 :1;
		unsigned RES_22_39 :18;
	} bit;
	uint32_t all;
} sdcard_response_R7_t;

//! Перечисления типа ответа SD-карты.
typedef enum _SD_Card_Reply_Type {
	SDCARD_RESPONSE_NO,		//!<Нет ответа
    SDCARD_RESPONSE_R1,		//!< Ответ R1.
    SDCARD_RESPONSE_R1b,	//!< Ответ R1 с ожиданием busy.
    SDCARD_RESPONSE_R2,		//!< Ответ R2.
    SDCARD_RESPONSE_R3,		//!< Ответ R3.
	SDCARD_RESPONSE_R4,		//!< Ответ R4.
	SDCARD_RESPONSE_R4b,	//!< Ответ R4 с ожиданием busy.
	SDCARD_RESPONSE_R5,		//!< Ответ R5.
	SDCARD_RESPONSE_R6,		//!< Ответ R6.
    SDCARD_RESPONSE_R7,		//!< Ответ R7.
} sdcard_reply_type_t;

//! Общий ответ SD-карты.
typedef union _SD_Card_Reply {
	sdcard_response_R1_t r1;	//!< Ответ R1.
	sdcard_response_R1_t r1b;	//!< Ответ R1b.
	sdcard_response_R2_t r2;	//!< Ответ R2.
	sdcard_response_R3_t r3;	//!< Ответ R3.
	sdcard_response_R4_t r4;	//!< Ответ R4.
	sdcard_response_R4b_t r4b;	//!< Ответ R4b.
	sdcard_response_R5_t r5;	//!< Ответ R5.
	sdcard_response_R6_t r6;	//!< Ответ R6.
	sdcard_response_R7_t r7;	//!< Ответ R7.
} sdcard_reply_t;


//! Проверка отсутствия данных кроме r1 в ответе SD-карты.
#define SDCARD_REPLY_ONLY_R1(R1) (((R1) & SDCARD_REPLY_R1_ILLEGAL_COMMAND) ||\
                                  ((R1) & SDCARD_REPLY_R1_COM_CRC_ERROR))


#endif /* SDCARD_REPLY_H_ */
