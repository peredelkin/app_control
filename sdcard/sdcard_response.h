/**
 * @file sdcard_reply.h Декларация ответов SD-карты.
 */

#ifndef SDCARD_REPLY_H_
#define SDCARD_REPLY_H_

#include <stdint.h>
#include <assert.h>
#include "sdcard_reg.h"

/*
 * Short response: Card Status [39:8]
 * Long response: Card Status [127:96]
 */

// [39:8] Argument field
typedef union {
	uint32_t all;
} sdcard_response_R1_t;

// [127:96] Argument field
typedef union {
	uint32_t all;
} sdcard_response_R2_t;

// [39:8] Argument field
typedef union {
	uint32_t all;
} sdcard_response_R3_t;

// [39:8] Argument field
typedef union {
	uint32_t all;
} sdcard_response_R4_t;

// [39:8] Argument field
typedef union {
	uint32_t all;
} sdcard_response_R4b_t;

// [39:8] Argument field
typedef union {
	uint32_t all;
} sdcard_response_R5_t;

// [39:8] Argument field
typedef union {
	uint32_t all;
} sdcard_response_R6_t;

// [39:8] Argument field
typedef union {
	struct {
		unsigned echo :8;
		unsigned voltage :4;
		unsigned pci_e :1;
		unsigned pci_e_1v2 :1;
		unsigned res_22_39 :18;
	} bit;
	uint32_t all;
} sdcard_response_R7_t;

//! Перечисления типа ответа SD-карты.
typedef enum _SD_Card_Reply_Type {
	SDCARD_RESPONSE_NO, //!<Нет ответа
    SDCARD_RESPONSE_R1, //!< Ответ R1.
    SDCARD_RESPONSE_R1b, //!< Ответ R1 с ожиданием busy.
    SDCARD_RESPONSE_R2, //!< Ответ R2.
    SDCARD_RESPONSE_R3, //!< Ответ R3.
	SDCARD_RESPONSE_R6, //!< Ответ R6
    SDCARD_RESPONSE_R7, //!< Ответ R7.
} sdcard_reply_type_t;

//! Общий ответ SD-карты.
typedef union _SD_Card_Reply {
	sdcard_response_R1_t r1; //!< Ответ R1.
	sdcard_response_R2_t r2; //!< Ответ R2.
	sdcard_response_R3_t r3; //!< Ответ R3.
	sdcard_response_R4_t r4; //!< Ответ R4.
	sdcard_response_R4b_t r4b; //!< Ответ R4b.
	sdcard_response_R5_t r5; //!< Ответ R5.
	sdcard_response_R6_t r6; //!< Ответ R7.
	sdcard_response_R7_t r7; //!< Ответ R7.
} sdcard_reply_t;


//! Проверка отсутствия данных кроме r1 в ответе SD-карты.
#define SDCARD_REPLY_ONLY_R1(R1) (((R1) & SDCARD_REPLY_R1_ILLEGAL_COMMAND) ||\
                                  ((R1) & SDCARD_REPLY_R1_COM_CRC_ERROR))


#endif /* SDCARD_REPLY_H_ */
