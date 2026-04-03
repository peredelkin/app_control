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

#define CAN1_CO_ENABLE
#define CAN2_CO_ENABLE

#define CAN_BRIDGE_ID_CLIENT_TO_SERVER	(0x641)
#define CAN_BRIDGE_ID_SERVER_TO_CLIENT	(0x621)

#define CAN1_SDO_CLI_ID_ClientToServer	(0x600)
#define CAN1_SDO_CLI_ID_ServerToClient	(0x580)

CO_t* can1_co = NULL;
CO_t* can2_co = NULL;

CO_SDO_CLI_Queue can1_cli_Queue[16];

CO_SDO_CLI_Driver_t can1_cli_driver;

#define CAN_BUS_QUEUE_SIZE 128

can_rx_frame_queue_t can_bus_1_rx_queue[CAN_BUS_QUEUE_SIZE];
can_tx_frame_queue_t can_bus_1_tx_queue[CAN_BUS_QUEUE_SIZE];

can_rx_frame_queue_t can_bus_2_rx_queue[CAN_BUS_QUEUE_SIZE];
can_tx_frame_queue_t can_bus_2_tx_queue[CAN_BUS_QUEUE_SIZE];

can_bus_t can_bus_1 = {
		.can_ptr[0] = CAN1,
		.can_ptr[1] = CAN2,
		.can_n = CAN_BUS_MASTER,
		.error = 0,
		.tx_error_counter  = 0,
		.rx_error_counter = 0,
		.last_error_code  = 0,
		.last_index = 0
};

can_bus_t can_bus_2 = {
		.can_ptr[0] = CAN1,
		.can_ptr[1] = CAN2,
		.can_n = CAN_BUS_SLAVE,
		.error = 0,
		.tx_error_counter  = 0,
		.rx_error_counter = 0,
		.last_error_code  = 0,
		.last_index = 0
};


uint32_t can_1_to_2_id = CAN_BUS_MAKE_ID(CAN_BRIDGE_ID_SERVER_TO_CLIENT);
uint32_t can_1_to_2_mask = CAN_BUS_MAKE_MASK(0x7FF);

uint32_t can_2_to_1_id = CAN_BUS_MAKE_ID(CAN_BRIDGE_ID_CLIENT_TO_SERVER);
uint32_t can_2_to_1_mask = CAN_BUS_MAKE_MASK(0x7FF);

err_t can_bus_rx_handler_callback(can_bus_t* bus, can_rx_frame_queue_t* head) {
	//если шина не задана
	if(bus == NULL) return E_NULL_POINTER;
	//если мост не задан
	if(bus->bridge_bus == NULL) return E_NULL_POINTER;
	//получим указатель на CANopen
	CO_t* co = (CO_t*)bus->co;
	//если индекс принадлежит мосту
	if(bus->bridge_index == head->index) {
		//передадим в мост
		if(can_bus_write(bus->bridge_bus, head->id, head->dlc, head->data) == false) {
			//return E_CANCELED;
		}
	} else {
		//иначе передадим в CANopen
		if((co == NULL) || (co->CANmodule == NULL) || (co->CANmodule->rxArray) == NULL) return E_NULL_POINTER;

		CO_CANrx_t *buffer = &co->CANmodule->rxArray[head->index];

		CO_CANrxMsg_t rcvMsg = { 0 };

		rcvMsg.ident = head->id;
		rcvMsg.DLC = head->dlc;
		memcpy(rcvMsg.data, head->data, head->dlc); //copy DATA

		if ((buffer != NULL) && (buffer->pCANrx_callback != NULL)) {
			buffer->pCANrx_callback(buffer->object, (void*) &rcvMsg);
		}
	}

	return E_NO_ERROR;
}

//CAN1
void CAN1_TX_IRQHandler() { CAN_TX_IRQHandler(&can_bus_1); }
void CAN1_RX0_IRQHandler() { CAN_RX_IRQHandler(&can_bus_1, CAN_RX_MAILBOX_0); }
void CAN1_RX1_IRQHandler() { CAN_RX_IRQHandler(&can_bus_1, CAN_RX_MAILBOX_1); }
void CAN1_SCE_IRQHandler() { CAN_SCE_IRQHandler(&can_bus_1); }

//CAN2
void CAN2_TX_IRQHandler() { CAN_TX_IRQHandler(&can_bus_2); }
void CAN2_RX0_IRQHandler() { CAN_RX_IRQHandler(&can_bus_2, CAN_RX_MAILBOX_0); }
void CAN2_RX1_IRQHandler() { CAN_RX_IRQHandler(&can_bus_2, CAN_RX_MAILBOX_1); }
void CAN2_SCE_IRQHandler() { CAN_SCE_IRQHandler(&can_bus_2); }


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

//! Настройки не менять!
void can_setup(can_bus_t* bus) {
	while(bus == NULL);

	CAN_TypeDef* can = bus->can_ptr[bus->can_n];

	while(can == NULL);

	can_software_master_reset(can);	//Force a master reset of the bxCAN

	can_bus_initialization_request(can);

	/*
	 * 0: Priority driven by the identifier of the message
	 * 1: Priority driven by the request order (chronologically)
	 */
	can_MCR_TXFP_set(can,	0); //Должно быть 0

	/*
	 * 0: Receive FIFO not locked on overrun. Once a receive FIFO is full the next incoming
	 * message will overwrite the previous one.
	 * 1: Receive FIFO locked against overrun. Once a receive FIFO is full the next incoming
	 * message will be discarded.
	 */
	can_MCR_RFLM_set(can,	0); //Должно быть 1 - мы должны забрать то, что получили

	/*
	 * 0: The CAN hardware will automatically retransmit the message until it has been
	 * successfully transmitted according to the CAN standard.
	 * 1: A message will be transmitted only once, independently of the transmission result
	 * (successful, error or arbitration lost).
	 */
	can_MCR_NART_set(can,	0); //Должно быть 0 - обработка ошибок драйвером отсуствует

	/*
	 * 0: The Sleep mode is left on software request by clearing the SLEEP bit of the CAN_MCR
	 * register.
	 * 1: The Sleep mode is left automatically by hardware on CAN message detection.
	 */
	can_MCR_AWUM_set(can,	1);

	/*
	 * 0: The Bus-Off state is left on software request, once 128 occurrences of 11 recessive bits
	 * have been monitored and the software has first set and cleared the INRQ bit of the
	 * CAN_MCR register.
	 * 1: The Bus-Off state is left automatically by hardware once 128 occurrences of 11 recessive
	 * bits have been monitored.
	 */
	can_MCR_ABOM_set(can,	1); //Должно быть 1 - выход из Bus-Off драйвером отсутствует

	/*
	 * 0: Time Triggered Communication mode disabled.
	 * 1: Time Triggered Communication mode enabled
	 */
	can_MCR_TTCM_set(can,	0); //Должно быть 0 - всё равно не работает

	/*
	 * 0: CAN working during debug
	 * 1: CAN reception/transmission frozen during debug. Reception FIFOs can still be
	 * accessed/controlled normally.
	 */
	can_MCR_DBF_set(can,	1);	 //Должно быть 1
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

#if (((CO_CONFIG_PDO)&CO_CONFIG_RPDO_ENABLE) != 0) || (((CO_CONFIG_PDO)&CO_CONFIG_TPDO_ENABLE) != 0)
	coerr = CO_CANopenInitPDO(co, co->em, OD, NODE_ID, &errInfo);

	if (coerr != CO_ERROR_NO) {
		printf("CANopen init PDO fail! (err: %d err_info: %d)\n", (int) coerr, (int) errInfo);
		return coerr;
	}
#endif

	can_bus->co = co; //установим указатель на CANopen в шине

	/* Разрешение работы */
	CO_CANsetNormalMode(co->CANmodule);

	return CO_ERROR_NO;
}

void can_CO_process(CO_t *co, uint32_t timeDifference_us, uint32_t* timerNext_us) {

	if(co == NULL) return;

	CO_NMT_reset_cmd_t reset_cmd = CO_RESET_NOT;

	reset_cmd = CO_process(co, false, timeDifference_us, timerNext_us);

	switch(reset_cmd) {
	default:
		break;
	case CO_RESET_NOT:
		break;
	case CO_RESET_COMM:
		break;
	case CO_RESET_APP:
		break;
	case CO_RESET_QUIT:
		break;
	}

#if (((CO_CONFIG_PDO)&CO_CONFIG_RPDO_ENABLE) != 0) || (((CO_CONFIG_PDO)&CO_CONFIG_TPDO_ENABLE) != 0)
    bool_t syncWas = false;
#endif

#if (((CO_CONFIG_SYNC)&CO_CONFIG_SYNC_ENABLE) != 0)
#if (((CO_CONFIG_PDO)&CO_CONFIG_RPDO_ENABLE) != 0) || (((CO_CONFIG_PDO)&CO_CONFIG_TPDO_ENABLE) != 0)
    syncWas =
#endif
    CO_process_SYNC(co->m_co_ss, NET_TIMER_TICKS_PERIOD_US, NULL);
#endif

#if (((CO_CONFIG_PDO)&CO_CONFIG_RPDO_ENABLE) != 0)
    CO_process_RPDO(co, syncWas, timeDifference_us, NULL);
#endif

#if (((CO_CONFIG_PDO)&CO_CONFIG_TPDO_ENABLE) != 0)
    CO_process_TPDO(co, syncWas, timeDifference_us, NULL);
#endif
}

void can_CO_sdo_cli_process(CO_SDO_CLI_Driver_t *drv, uint32_t dt) {
	if((drv == NULL) || (drv->sdo_cli == NULL) || (drv->queue == NULL)) return;

	CO_SDO_CLI_process(drv, dt);
}

uint32_t can_bus_1_current_error;
uint32_t can_bus_2_current_error;

uint32_t can_bus_1_last_error;
uint32_t can_bus_2_last_error;

uint32_t can_bus_1_masked_error;
uint32_t can_bus_1_masked_error_n;
uint32_t can_bus_2_masked_error;
uint32_t can_bus_2_masked_error_n;

void CO_CANmodule_check_buffer_full(CO_t* co) {
	if(co == NULL) return;
	if(co->CANmodule == NULL) return;
	if(co->CANmodule->txArray == NULL) return;
	if(co->CANmodule->txSize == 0) return;

	CO_CANmodule_t* CANmodule = co->CANmodule;

	for (uint16_t i = 0U; i < CANmodule->txSize; i++) {
		if(CANmodule->txArray[i].bufferFull) {
			CO_ReturnError_t co_err = CO_CANsend(CANmodule, &(CANmodule->txArray[i]));
			if (co_err != CO_ERROR_NO) return;
		}
	}
}

void can_process_callback(void* arg) {
	//CAN1 CAN2 RX
	can_bus_rx_process(&can_bus_1);
	can_bus_rx_process(&can_bus_2);

	//CANopen
	can_CO_process(can1_co, CAN_TIMER_TICKS_FREQ, NULL);
	can_CO_process(can2_co, CAN_TIMER_TICKS_FREQ, NULL);

	//CANopen CLIent
	can_CO_sdo_cli_process(&can1_cli_driver, CAN_TIMER_TICKS_FREQ);

	//CAN1 CAN2 TX
	can_bus_tx_process(&can_bus_1);
	can_bus_tx_process(&can_bus_2);

	//CANopen bufferFUll check
	CO_CANmodule_check_buffer_full(can1_co);
	CO_CANmodule_check_buffer_full(can2_co);

	can_bus_1_current_error = can_bus_1.error;
	can_bus_2_current_error = can_bus_2.error;

	can_bus_1_masked_error = can_bus_1_current_error & ~can_bus_1_last_error;
	can_bus_1_masked_error_n = ~can_bus_1_current_error & can_bus_1_last_error;
	can_bus_1_last_error = can_bus_1_current_error;

	can_bus_2_masked_error = can_bus_2_current_error & ~can_bus_2_last_error;
	can_bus_2_masked_error_n = ~can_bus_2_current_error & can_bus_2_last_error;
	can_bus_2_last_error = can_bus_2_current_error;

	//CAN1
	if (can_bus_1_masked_error & CAN_ERROR_TX_QUEUE_FULL) {
		printf("CAN1 TX QUEUE FULL\n");
	}
	if (can_bus_1_masked_error_n & CAN_ERROR_TX_QUEUE_FULL) {
		printf("CAN1 TX QUEUE NOT FULL\n");
	}

	if (can_bus_1_masked_error & CAN_ERROR_RX0_FULL) {
		printf("CAN1 RX0 FULL\n");
	}
	if (can_bus_1_masked_error & CAN_ERROR_RX1_FULL) {
		printf("CAN1 RX1 FULL\n");
	}

	if (can_bus_1_masked_error & CAN_ERROR_RX0_OVERRUN) {
		printf("CAN1 RX0 OVERRUN\n");
	}
	if (can_bus_1_masked_error & CAN_ERROR_RX1_OVERRUN) {
		printf("CAN1 RX1 OVERRUN\n");
	}

	if (can_bus_1_masked_error & CAN_ERROR_TX_BUSSOFF) {
		printf("CAN1 TX BUSSOFF\n");
	}

	if (can_bus_1_masked_error & CAN_ERROR_TX_PASSIVE) {
		printf("CAN1 TX PASSIVE\n");
	}
	if (can_bus_1_masked_error & CAN_ERROR_RX_PASSIVE) {
		printf("CAN1 RX PASSIVE\n");
	}

	if (can_bus_1_masked_error & CAN_ERROR_TX_WARNING) {
		printf("CAN1 TX WARNING\n");
	}
	if (can_bus_1_masked_error & CAN_ERROR_RX_WARNING) {
		printf("CAN1 RX WARNING\n");
	}


	//CAN2
	if (can_bus_2_masked_error & CAN_ERROR_TX_QUEUE_FULL) {
		printf("CAN2 TX QUEUE FULL\n");
	}
	if(can_bus_2_masked_error_n & CAN_ERROR_TX_QUEUE_FULL) {
		printf("CAN2 TX QUEUE NOT FULL\n");
	}

	if (can_bus_2_masked_error & CAN_ERROR_RX0_FULL) {
		printf("CAN2 RX0 FULL\n");
	}
	if (can_bus_2_masked_error & CAN_ERROR_RX1_FULL) {
		printf("CAN2 RX1 FULL\n");
	}

	if (can_bus_2_masked_error & CAN_ERROR_RX0_OVERRUN) {
		printf("CAN2 RX0 OVERRUN\n");
	}
	if (can_bus_2_masked_error & CAN_ERROR_RX1_OVERRUN) {
		printf("CAN2 RX1 OVERRUN\n");
	}

	if (can_bus_2_masked_error & CAN_ERROR_TX_BUSSOFF) {
		printf("CAN2 TX BUSSOFF\n");
	}

	if (can_bus_2_masked_error & CAN_ERROR_TX_PASSIVE) {
		printf("CAN2 TX PASSIVE\n");
	}
	if (can_bus_2_masked_error & CAN_ERROR_RX_PASSIVE) {
		printf("CAN2 RX PASSIVE\n");
	}

	if (can_bus_2_masked_error & CAN_ERROR_TX_WARNING) {
		printf("CAN2 TX WARNING\n");
	}
	if (can_bus_2_masked_error & CAN_ERROR_RX_WARNING) {
		printf("CAN2 RX WARNING\n");
	}
}

void can1_sdo_cli_init(void) {
	if(can1_co == NULL) return;
	can1_cli_driver.sdo_cli = can1_co->SDOclient;
	can1_cli_driver.m_SDOclientBlockTransfer = SDO_CLIENT_BLOCK_TRANSFER;
	can1_cli_driver.m_cobidClientToServer = CAN1_SDO_CLI_ID_ClientToServer;
	can1_cli_driver.m_cobidServerToClient = CAN1_SDO_CLI_ID_ServerToClient;
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
	can_bus_rx_queue_init(&can_bus_1, can_bus_1_rx_queue, CAN_BUS_QUEUE_SIZE);
	can_bus_tx_queue_init(&can_bus_1, can_bus_1_tx_queue, CAN_BUS_QUEUE_SIZE);
	can_bus_1.bridge_bus = &can_bus_2; //установим мост
	can_bus_1.rx_callback = can_bus_rx_handler_callback;
}

void can2_init(void) {
	can2_reset();
	gpio_can2_cfg_setup();
	can_setup(&can_bus_2);
	can_bus_rx_queue_init(&can_bus_2, can_bus_2_rx_queue, CAN_BUS_QUEUE_SIZE);
	can_bus_tx_queue_init(&can_bus_2, can_bus_2_tx_queue, CAN_BUS_QUEUE_SIZE);
	can_bus_2.bridge_bus = &can_bus_1; //установим мост
	can_bus_2.rx_callback = can_bus_rx_handler_callback;
}

void can_filter_init(void) {
	can_filter_setup(14);
}

void can_bridge_init(can_bus_t* bus, uint32_t id, uint32_t mask) {
	//получим индекс моста
	bus->bridge_index = bus->last_index + 1;
	//выделим и настроим филттр
	err_t can1_bridge_err = can_bus_filter_16b_bank_set(bus, bus->bridge_index, id, mask);
	if(can1_bridge_err != E_NO_ERROR) {
		printf("CAN bridge filter allocation error: %d\n", (int) can1_bridge_err);
	} else {
		printf("CAN%d bridge allocated\n", (bus->can_n + 1));
	}
}

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
	sys_counter_tv_print();
	can1_co_res = create_CO(&can1_co);

	if(can1_co_res == -1 || can1_co == NULL) {
			printf("Error create CO1\n");
	} else {
		printf("CO1 created\n");
		co1_err = init_CO(can1_co, &can_bus_1);

		if (co1_err != CO_ERROR_NO) {
			printf("Error init CO (%d)\n", (int) co1_err);
		} else {
#if defined(CAN1_CO_ENABLE) && defined(CAN2_CO_ENABLE)
			sys_counter_tv_print();
			//Настройка моста
			can_bridge_init(&can_bus_1, can_1_to_2_id, can_1_to_2_mask);
#endif
			//Настройка клиента
			can1_sdo_cli_init();
		}
	}
#endif

#ifdef CAN2_CO_ENABLE
	sys_counter_tv_print();
	can2_co_res = create_CO(&can2_co);

	if(can2_co_res == -1 || can2_co == NULL) {
			printf("Error create CO2\n");
	} else {
		printf("CO2 created\n");
		co2_err = init_CO(can2_co, &can_bus_2);

		if (co2_err != CO_ERROR_NO) {
			printf("Error init CO (%d)\n", (int) co2_err);
		} else {
#if defined(CAN1_CO_ENABLE) && defined(CAN2_CO_ENABLE)
			sys_counter_tv_print();
			//Настройка моста
			can_bridge_init(&can_bus_2, can_2_to_1_id, can_2_to_1_mask);
#endif
		}
	}
#endif

#if defined(CAN1_CO_ENABLE) || defined(CAN2_CO_ENABLE)
	sys_counter_tv_print();
	printf("CO timer ");
	if(can1_co_res == 0 && can2_co_res == 0 && co1_err == CO_ERROR_NO && co2_err == CO_ERROR_NO) {
		//Настройка CO_process таймера.
		INIT(can_tim); //TIM5
		CALLBACK_PROC(can_tim.on_timeout) = can_process_callback;
		CALLBACK_ARG(can_tim.on_timeout) = NULL; //(void*) can1_co;
		if (can_tim.status & MS_TIMER_STATUS_ERROR) {
			printf("init error(%u)\n", (unsigned)can_tim.status);
		} else {
			// Запуск CO_process таймера.
			can_tim.control = MS_TIMER_CONTROL_ENABLE;
			CONTROL(can_tim);
			if (can_tim.status & MS_TIMER_STATUS_RUN) {
				printf("started (%u)\n", (unsigned)can_tim.status);
			} else {
				printf("start error (%u)\n", (unsigned)can_tim.status);
			}
		}
	} else {
		printf("NOT started!\n");
	}
#endif
}



