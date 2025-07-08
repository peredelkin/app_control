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


can_bus_t can_bus_1 = {
		.can_ptr[0] = CAN1,
		.can_ptr[1] = CAN2,
		.can_n = CAN_BUS_MASTER,
		.error = 0,
		.tx_error_counter  = 0,
		.rx_error_counter = 0,
		.last_error_code  = 0,
		.last_filter = 0
};

can_bus_t can_bus_2 = {
		.can_ptr[0] = CAN1,
		.can_ptr[1] = CAN2,
		.can_n = CAN_BUS_SLAVE,
		.error = 0,
		.tx_error_counter  = 0,
		.rx_error_counter = 0,
		.last_error_code  = 0,
		.last_filter = 0
};

void can_bus_1_bridge_callback(void* bus, void* message) {
	if((bus == NULL) || (message == NULL)) return;

	can_bus_t* can_bus = (can_bus_t*)bus;

	CAN_TypeDef* can = can_bus->can_ptr[can_bus->can_n];

	CO_CANrxMsg_t* buffer = (CO_CANrxMsg_t*)message;

	can_bus->bridge_error = can_tx_mailbox_write_and_request(can, buffer->ident, buffer->DLC, buffer->data);
}

void can_bus_2_bridge_callback(void* bus, void* message) {
	if((bus == NULL) || (message == NULL)) return;

	can_bus_t* can_bus = (can_bus_t*)bus;

	CAN_TypeDef *can = can_bus->can_ptr[can_bus->can_n];

	CO_CANrxMsg_t* buffer = (CO_CANrxMsg_t*)message;

	can_bus->bridge_error = can_tx_mailbox_write_and_request(can, buffer->ident, buffer->DLC, buffer->data);
}

CO_t* can1_co = NULL;
CO_t* can2_co = NULL;

CO_SDO_CLI_Queue can1_cli_Queue[16];

CO_SDO_CLI_Driver_t can1_cli_driver;

//CAN1
void CAN1_TX_IRQHandler() {
	CO_TX_IRQHandler(can1_co->CANmodule);
}

void CAN1_RX0_IRQHandler() {
	CO_RX_IRQHandler(can1_co->CANmodule, CAN_RX_MAILBOX_0);
}

void CAN1_RX1_IRQHandler() {
	CO_RX_IRQHandler(can1_co->CANmodule, CAN_RX_MAILBOX_1);
}

void CAN1_SCE_IRQHandler() {
	CO_SCE_IRQHandler(can1_co->CANmodule);
}

//CAN2
void CAN2_TX_IRQHandler() {
	CO_TX_IRQHandler(can2_co->CANmodule);
}

void CAN2_RX0_IRQHandler() {
	CO_RX_IRQHandler(can2_co->CANmodule, CAN_RX_MAILBOX_0);
}

void CAN2_RX1_IRQHandler() {
	CO_RX_IRQHandler(can2_co->CANmodule, CAN_RX_MAILBOX_1);
}

void CAN2_SCE_IRQHandler() {
	CO_SCE_IRQHandler(can2_co->CANmodule);
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

void can2_nvic_init(uint32_t priority) {
	NVIC_SetPriority(CAN2_TX_IRQn, priority);
	NVIC_EnableIRQ(CAN2_TX_IRQn);

	NVIC_SetPriority(CAN2_RX0_IRQn, priority);
	NVIC_EnableIRQ(CAN2_RX0_IRQn);

	NVIC_SetPriority(CAN2_RX1_IRQn, priority);
	NVIC_EnableIRQ(CAN2_RX1_IRQn);

	NVIC_SetPriority(CAN2_SCE_IRQn, priority);
	NVIC_EnableIRQ(CAN2_SCE_IRQn);
}

void can1_reset(void) {
	RCC->APB1RSTR |= RCC_APB1RSTR_CAN1RST; //Reset
	RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN1RST; //Not Reset
}

void can2_reset(void) {
	RCC->APB1RSTR |= RCC_APB1RSTR_CAN2RST; //Reset
	RCC->APB1RSTR &= ~RCC_APB1RSTR_CAN2RST; //Not Reset
}

void can1_rcc_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
}

void can2_rcc_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
}

void can_setup(can_bus_t* bus) {
	while(bus == NULL);

	CAN_TypeDef* can = bus->can_ptr[bus->can_n];

	while(can == NULL);

	can_software_master_reset(can);	//Force a master reset of the bxCAN

	can_bus_initialization_request(can);

	can_MCR_TXFP_set(can, true);		//Priority driven by the request order (chronologically)
	can_MCR_RFLM_set(can, true);		//Receive FIFO locked against overrun.
	can_MCR_NART_set(can,false);		//The CAN hardware will automatically retransmit the message
	can_MCR_AWUM_set(can,false);		//The Sleep mode is left on software request
	can_MCR_ABOM_set(can, true);		//The Bus-Off state is left automatically by hardware
	can_MCR_TTCM_set(can,false);		//Time Triggered Communication mode disabled
	can_MCR_DBF_set(can, true);			//CAN reception/transmission frozen during debug
}

void can_filter_setup(int CAN2SB) {
	CAN_TypeDef* can_master = CAN1;

	can_master_filter_init_mode(can_master);			//Initialization mode for the filters
	can_master_filter_reset_all(can_master);	//
	can_master_can2_filter_start_bank_set(can_master, CAN2SB); //28d, all the filters to CAN1 can be used
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

void can_CO_process(CO_t *co, uint32_t timeDifference_us, uint32_t* timerNext_us) {

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

void can_CO_sdo_cli_process(CO_SDO_CLI_Driver_t *drv, uint32_t dt) {
	if((drv == NULL) || (drv->sdo_cli == NULL) || (drv->queue == NULL)) return;

	CO_SDO_CLI_process(drv, dt);
}

void can_tim_handler(void* arg) {
	can_CO_process(can1_co, 1000, NULL);
	can_CO_sdo_cli_process(&can1_cli_driver, 1000);
	can_CO_process(can2_co, 1000, NULL);
}

void can1_sdo_cli_init(void) {
	if(can1_co == NULL) return;
	can1_cli_driver.sdo_cli = can1_co->SDOclient;
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
	can1_reset();
	gpio_can1_cfg_setup();
	can_setup(&can_bus_1);
}

void can2_init(void) {
	can2_reset();
	gpio_can2_cfg_setup();
	can_setup(&can_bus_2);
}

void can_filter_init(void) {
	can_filter_setup(14);
}

#define CAN1_CO_ENABLE
#define CAN2_CO_ENABLE

void can_canopen_init(void) {
	int can1_co_res = 0;
	CO_ReturnError_t co1_err = CO_ERROR_NO;

	int can2_co_res = 0;
	CO_ReturnError_t co2_err = CO_ERROR_NO;

#if defined(CAN1_CO_ENABLE) || defined(CAN2_CO_ENABLE)
	can1_rcc_init();
	can2_rcc_init();
#endif

#ifdef CAN1_CO_ENABLE
	can1_init();
#endif

#ifdef CAN2_CO_ENABLE
	can2_init();
#endif

#if defined(CAN1_CO_ENABLE) || defined(CAN2_CO_ENABLE)
	can_filter_init();
#endif

#ifdef CAN1_CO_ENABLE
	can1_co_res = create_CO(&can1_co);

	if(can1_co_res == -1 || can1_co == NULL) {
			printf("Error create CO1\n");
	} else {
		printf("CO1 created\n");
		co1_err = init_CO(can1_co, &can_bus_1);

		if (co1_err != CO_ERROR_NO) {
			printf("Error init CO (%d)\n", (int) co1_err);
		} else {
			//Настройка клиента
			can1_sdo_cli_init();
		}
	}
#endif

#ifdef CAN2_CO_ENABLE
	can2_co_res = create_CO(&can2_co);

	if(can2_co_res == -1 || can2_co == NULL) {
			printf("Error create CO2\n");
	} else {
		printf("CO2 created\n");
		co2_err = init_CO(can2_co, &can_bus_2);

		if (co2_err != CO_ERROR_NO) {
			printf("Error init CO (%d)\n", (int) co2_err);
		} else {

		}
	}
#endif

#define CAN_COB_ID_2_TO_1 (0x600 + 0x10)
#define CAN_COB_ID_1_TO_2 (0x580 + 0x10)

//Инициализация моста имени Артёма Тянутова
#if defined(CAN1_CO_ENABLE) && defined(CAN2_CO_ENABLE)
	uint32_t can_1_to_2_id = (uint32_t) (CAN_FIR_STID & (CAN_COB_ID_1_TO_2 << CAN_FIR_STID_SHIFT));
	uint32_t can_1_to_2_mask = (uint32_t) (CAN_FIR_STID & (0x7FF << CAN_FIR_STID_SHIFT));

	uint32_t can_2_to_1_id = (uint32_t) (CAN_FIR_STID & (CAN_COB_ID_2_TO_1 << CAN_FIR_STID_SHIFT));
	uint32_t can_2_to_1_mask = (uint32_t) (CAN_FIR_STID & (0x7FF << CAN_FIR_STID_SHIFT));

	if(can_bus_filter_16b_bank_set(&can_bus_1, can_bus_1.last_filter + 1, can_1_to_2_id, can_1_to_2_mask) == E_NO_ERROR) {
		can_bus_1.bridge_callback = can_bus_2_bridge_callback;
		can_bus_1.bridge_callback_argument = &can_bus_2;
		printf("CAN1 to CAN2 bridge initialized\n");
	}

	if(can_bus_filter_16b_bank_set(&can_bus_2, can_bus_1.last_filter + 1, can_2_to_1_id, can_2_to_1_mask) == E_NO_ERROR) {
		can_bus_2.bridge_callback = can_bus_1_bridge_callback;
		can_bus_2.bridge_callback_argument = &can_bus_1;
		printf("CAN2 to CAN1 bridge initialized\n");
	}
#endif

#if defined(CAN1_CO_ENABLE) || defined(CAN2_CO_ENABLE)
	if(can1_co_res == 0 && can2_co_res == 0 && co1_err == CO_ERROR_NO && co2_err == CO_ERROR_NO) {
		//Настройка CO_process таймера.
		INIT(can_tim); //TIM5
		CALLBACK_PROC(can_tim.on_timeout) = can_tim_handler;
		CALLBACK_ARG(can_tim.on_timeout) = NULL; //(void*) can1_co;
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
	} else {
		printf("CO timer NOT started!\n");
	}
#endif
}



