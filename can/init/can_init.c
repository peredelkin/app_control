/*
 * can_init.c
 *
 *  Created on: 6 нояб. 2024 г.
 *      Author: Ruslan
 */

#include <stdio.h>

#include "can_init.h"
#include "gpio/init/gpio_init.h"
#include "sys/counter/sys_counter.h"
#include "modules/modules.h"

#include "CO_CLI_driver.h"

#include "CANopenNode/CANopen.h"
#include "OD.h"


//CAN1_TX_IRQHandler                /* CAN1 TX                      */
//CAN1_RX0_IRQHandler               /* CAN1 RX0                     */
//CAN1_RX1_IRQHandler               /* CAN1 RX1                     */
//CAN1_SCE_IRQHandler               /* CAN1 SCE                     */


can_bus_t can1 = {
		.bus = CAN1,
		.error = 0,
		.tx_error_counter  = 0,
		.rx_error_counter = 0,
		.last_error_code  = 0
};

CO_t* co = NULL;

CO_SDO_CLI_Queue can1_cli_Queue[16];

CO_SDO_CLI_Driver_t can1_cli_driver;

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
	NVIC_EnableIRQ(CAN1_TX_IRQn);

	NVIC_SetPriority(CAN1_RX0_IRQn, priority);
	NVIC_EnableIRQ(CAN1_RX0_IRQn);

	NVIC_SetPriority(CAN1_RX1_IRQn, priority);
	NVIC_EnableIRQ(CAN1_RX1_IRQn);

	NVIC_SetPriority(CAN1_SCE_IRQn, priority);
	NVIC_EnableIRQ(CAN1_SCE_IRQn);
}

void can1_reset(void) {
	RCC->APB1RSTR |= RCC_APB1RSTR_CAN1RST; //Reset
	RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN1RST; //Not Reset
}

void can1_rcc_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
}

void can1_pre_init(void) {
	can_software_master_reset(can1.bus);	//Force a master reset of the bxCAN

	can_bus_initialization_request(can1.bus);

	can_MCR_TXFP_set(can1.bus, true);		//Priority driven by the request order (chronologically)
	can_MCR_RFLM_set(can1.bus, true);		//Receive FIFO locked against overrun.
	can_MCR_NART_set(can1.bus,false);		//The CAN hardware will automatically retransmit the message
	can_MCR_AWUM_set(can1.bus,false);		//The Sleep mode is left on software request
	can_MCR_ABOM_set(can1.bus, true);		//The Bus-Off state is left automatically by hardware
	can_MCR_TTCM_set(can1.bus,false);		//Time Triggered Communication mode disabled
	can_MCR_DBF_set(can1.bus, true);		//CAN reception/transmission frozen during debug

	can_filter_init_mode(can1.bus);			//Initialization mode for the filters
	can2_filter_start_bank_set(can1.bus, 28); //28d, all the filters to CAN1 can be used
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

CO_ReturnError_t init_CO(CO_t *co, can_bus_t *can_bus) {
	if (co == NULL)
		return CO_ERROR_ILLEGAL_ARGUMENT;
	if (can_bus == NULL)
		return CO_ERROR_ILLEGAL_ARGUMENT;

	CO_ReturnError_t coerr = CO_ERROR_NO;

	coerr = CO_CANinit(co, can_bus, 125);
	if (coerr != CO_ERROR_NO)
		return coerr;

	uint32_t errInfo = 0;

	coerr = CO_CANopenInit(co,
	NULL, NULL, OD, NULL, CO_CONFIG_NMT, FIRST_HB_TIME_MS, SDO_SERVER_TIMEOUT_MS, SDO_CLIENT_TIMEOUT_MS,
			SDO_CLIENT_BLOCK_TRANSFER, NODE_ID, &errInfo);

	if (coerr != CO_ERROR_NO) {
		printf("CANopen init fail! (err: %d err_info: %d)\n", (int) coerr, (int) errInfo);
		return coerr;
	}

	coerr = CO_CANopenInitPDO(co, co->em, OD, NODE_ID, &errInfo);

	if (coerr != CO_ERROR_NO) {
		printf("CANopen init PDO fail! (err: %d err_info: %d)\n", (int) coerr, (int) errInfo);
		return coerr;
	}

	/* Разрешение работы */
	CO_CANsetNormalMode(co->CANmodule);

	return CO_ERROR_NO;
}

void can1_CO_process(CO_t *co, uint32_t timeDifference_us, uint32_t* timerNext_us) {

	if(co == NULL) return;

	CO_NMT_reset_cmd_t reset_cmd = CO_RESET_NOT;

	reset_cmd = CO_process(co, false, timeDifference_us, timerNext_us);

	if (reset_cmd == CO_RESET_NOT) {
		//printf("CO_NMT_NO_COMMAND");
	} else if (reset_cmd == CO_RESET_COMM) {
		printf("CO_RESET_COMM");
	} else if (reset_cmd == CO_RESET_APP) {
		printf("CO_RESET_APP");
	} else if (reset_cmd == CO_RESET_QUIT) {
		printf("CO_RESET_QUIT");
	}
}

void can1_CO_sdo_cli_process(CO_SDO_CLI_Driver_t *drv, uint32_t dt) {
	if((drv == NULL) || (drv->sdo_cli == NULL) || (drv->queue == NULL)) return;

	CO_SDO_CLI_process(drv, dt);
}

void can_tim_handler(void* arg) {
	CO_t* co = (CO_t*)arg;
	can1_CO_process(co, 1000, NULL);
	can1_CO_sdo_cli_process(&can1_cli_driver, 1000);
}

void can1_sdo_cli_init(void) {
	if(co == NULL) return;
	can1_cli_driver.sdo_cli = co->SDOclient;
	can1_cli_driver.m_SDOclientBlockTransfer = SDO_CLIENT_BLOCK_TRANSFER;
	can1_cli_driver.m_cobidClientToServer = 0x600;
	can1_cli_driver.m_cobidServerToClient = 0x580;
	can1_cli_driver.m_defaultTimeout = 20;
	can1_cli_driver.queue = can1_cli_Queue;
	can1_cli_driver.queue_size = 16;
	can1_cli_driver.queue_head = 0;
	can1_cli_driver.queue_tail = 0;
}

void can1_init(void) {
	sys_counter_tv_print();

	can1_reset();

	gpio_can1_cfg_setup();

	can1_rcc_init();

	while(can1.bus == NULL);

	can1_pre_init();

	int res = create_CO(&co);

	if(res == -1 || co == NULL) {
		printf("Error create CO\n");
	} else {
		printf("CO created\n");
		CO_ReturnError_t coerr = init_CO(co, &can1);

		if(coerr != CO_ERROR_NO) {
			printf("Error init CO (%d)\n", (int)coerr);
		} else {
			//Настройка клиента
			can1_sdo_cli_init();
			//Настройка CO_process таймера.
			INIT(can_tim); //TIM5
			CALLBACK_PROC(can_tim.on_timeout) = can_tim_handler;
			CALLBACK_ARG(can_tim.on_timeout) = (void*)co;
			if (can_tim.status & MS_TIMER_STATUS_ERROR) {
				printf("CO timer init error(%lu)\n", can_tim.status);
			} else {
				printf("CO timer inited (%lu)\n", can_tim.status);
				// Запуск CO_process таймера.
				can_tim.control = MS_TIMER_CONTROL_ENABLE;
				CONTROL(can_tim);
				if (can_tim.status & MS_TIMER_STATUS_RUN) {
					printf("CO timer started (%lu)\n", can_tim.status);
				} else {
					printf("CO timer start error (%lu)\n", can_tim.status);
				}
			}
		}
	}
}



