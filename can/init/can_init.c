/*
 * can_init.c
 *
 *  Created on: 6 нояб. 2024 г.
 *      Author: Ruslan
 */

#include <stdio.h>

#include "can_init.h"
#include "gpio/init/gpio_init.h"

#include "CANopenNode/CANopen.h"
#include "OD.h"


//CAN1_TX_IRQHandler                /* CAN1 TX                      */
//CAN1_RX0_IRQHandler               /* CAN1 RX0                     */
//CAN1_RX1_IRQHandler               /* CAN1 RX1                     */
//CAN1_SCE_IRQHandler               /* CAN1 SCE                     */


//CAN interrupts
//TRANSMIT INTERRUPT
#define CAN1_TMEIE		false

//FIFO 0 INTERRUPT
#define CAN1_FMPIE0		false
#define CAN1_FFIE0		false
#define CAN1_FOVIE0		false

//FIFO 1 INTERRUPT
#define CAN1_FMPIE1		false
#define CAN1_FFIE1		false
#define CAN1_FOVIE1		false

//STATUS CHANGE ERROR INTERRUPT
#define CAN1_ERRIE		false
#define CAN1_EWGIE		false
#define CAN1_EPVIE		false
#define CAN1_BOFIE		false
#define CAN1_LECIE		false

#define CAN1_WKUIE		false
#define CAN1_SLKIE		false


can_bus_t can1;

CO_t* co = NULL;


void CAN1_TX_IRQHandler() {
	CO_TX_IRQHandler(co->CANmodule);
}

void CAN1_RX0_IRQHandler() {
	CO_RX_IRQHandler(co->CANmodule, 0);
}

void CAN1_RX1_IRQHandler() {
	CO_RX_IRQHandler(co->CANmodule, 1);
}

void CAN1_SCE_IRQHandler() {
	CO_SCE_IRQHandler(co->CANmodule);
}

void can1_nvic_init(uint32_t priority) {
	NVIC_SetPriority(CAN1_TX_IRQn, priority);
	//NVIC_EnableIRQ(CAN1_TX_IRQn);

	NVIC_SetPriority(CAN1_RX0_IRQn, priority);
	//NVIC_EnableIRQ(CAN1_RX0_IRQn);

	NVIC_SetPriority(CAN1_RX1_IRQn, priority);
	//NVIC_EnableIRQ(CAN1_RX1_IRQn);

	NVIC_SetPriority(CAN1_SCE_IRQn, priority);
	//NVIC_EnableIRQ(CAN1_SCE_IRQn);
}

void can1_reset(void) {
	RCC->APB1RSTR |= RCC_APB1RSTR_CAN1RST; //Reset
	RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN1RST; //Not Reset
}

void can1_rcc_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
}

void can1_struct_init(void) {
	can1.bus = CAN1;
}

void can1_interrupts_init(void) {
	//TRANSMIT INTERRUPT
	can_IER_TMEIE_set(can1.bus, CAN1_TMEIE);

	//FIFO 0 INTERRUPT
	can_IER_FMPIE_set(can1.bus, 0, CAN1_FMPIE0);
	can_IER_FFIE_set(can1.bus, 0, CAN1_FFIE0);
	can_IER_FOVIE_set(can1.bus, 0, CAN1_FOVIE0);

	//FIFO 1 INTERRUPT
	can_IER_FMPIE_set(can1.bus, 1, CAN1_FMPIE1);
	can_IER_FFIE_set(can1.bus, 1, CAN1_FFIE1);
	can_IER_FOVIE_set(can1.bus, 1, CAN1_FOVIE1);

	//STATUS CHANGE ERROR INTERRUPT
	can_IER_ERRIE_set(can1.bus, CAN1_ERRIE);
	can_IER_EWGIE_set(can1.bus, CAN1_EWGIE);
	can_IER_EPVIE_set(can1.bus, CAN1_EPVIE);
	can_IER_BOFIE_set(can1.bus, CAN1_BOFIE);
	can_IER_LECIE_set(can1.bus, CAN1_LECIE);
	can_IER_WKUIE_set(can1.bus, CAN1_WKUIE);
	can_IER_SLKIE_set(can1.bus, CAN1_SLKIE);
}

int create_CO(CO_t** co)
{
    if(co == NULL) return -1;

    CO_t* co_res = CO_new(NULL, NULL);

    //printf("co: 0x%x\n", (int)(long)co);

    if(co_res == NULL) return -1;

    *co = co_res;

    return 0;
}

CO_ReturnError_t init_CO(CO_t* co, can_bus_t* can_bus)
{
    if(co == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;
    if(can_bus == NULL) return CO_ERROR_ILLEGAL_ARGUMENT;

    CO_ReturnError_t coerr = CO_ERROR_NO;

    coerr = CO_CANinit(co, can_bus, 125);
    if(coerr != CO_ERROR_NO) return coerr;

    uint32_t errInfo = 0;

    coerr = CO_CANopenInit(co,
            NULL, NULL, OD, NULL, CO_CONFIG_NMT, FIRST_HB_TIME_MS, SDO_SERVER_TIMEOUT_MS, SDO_CLIENT_TIMEOUT_MS, SDO_CLIENT_BLOCK_TRANSFER, NODE_ID, &errInfo);

    if(coerr != CO_ERROR_NO){
        printf("CANopen init fail! (err: %d err_info: %d)\n", (int)coerr, (int)errInfo);
        return coerr;
    }

    coerr = CO_CANopenInitPDO(co, co->em, OD, NODE_ID, &errInfo);

    if(coerr != CO_ERROR_NO){
        printf("CANopen init PDO fail! (err: %d err_info: %d)\n", (int)coerr, (int)errInfo);
        return coerr;
    }

    /* Разрешение работы */
    CO_CANsetNormalMode(co->CANmodule);

    return CO_ERROR_NO;
}

void can1_init(void) {
	can1_reset();

	gpio_can1_cfg_setup();

	can1_rcc_init();

	can1_struct_init();



	can1_interrupts_init();
}
