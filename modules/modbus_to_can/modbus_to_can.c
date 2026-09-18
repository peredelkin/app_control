#include "modbus_to_can.h"

//CANopen
#include "CO_CLI_driver.h"
#include "reg_mc/reg_mc_ids.h"
extern CO_SDO_CLI_Driver_t can1_cli_driver;

static CO_SDO_CLI_Queue* modbus_to_can_read_queue = NULL;
static CO_SDO_CLI_Queue* modbus_to_can_write_queue = NULL;

void modbus_to_can_reset(M_modbus_to_can* modbus_to_can) {
	modbus_to_can->status = MODBUS_TO_CAN_STATUS_NONE; //Reset All Status
	modbus_to_can->control = MODBUS_TO_CAN_CONTROL_NONE; //Reset All Control

	modbus_to_can->m_id = 0;
	modbus_to_can->m_index = 0;
	modbus_to_can->m_subindex = 0;
	modbus_to_can->m_size = 0;
	modbus_to_can->m_data = 0;
	modbus_to_can->m_timeout_cnt = 0;

	modbus_to_can->status = MODBUS_TO_CAN_STATUS_READY; //Set Ready Status
}


//TODO: разобраться с зависанием SDO клиента на этапе CO_SDO_CLI_State_QUEUED. временно исправлено установкой состояния в CO_SDO_CLI_State_DONE по внутреннему таймауту.

static void modbus_to_can_read(M_modbus_to_can* modbus_to_can) {
	//если очередь не задана
    if(modbus_to_can_read_queue == NULL) {
    	//сбросим статусы VALID, ERROR, WARNING, READ_DONE
    	modbus_to_can->status &= ~(MODBUS_TO_CAN_STATUS_VALID |
    			MODBUS_TO_CAN_STATUS_ERROR |
				MODBUS_TO_CAN_STATUS_WARNING |
				MODBUS_TO_CAN_STATUS_READ_DONE);
    	//установим таймаут модуля
    	modbus_to_can->m_timeout_cnt = 40; //40 циклов или 400mc
    	//добавим в очередь задание
    	modbus_to_can_read_queue = CO_SDO_CLI_read(
    			&can1_cli_driver,
				modbus_to_can->m_id,
				modbus_to_can->m_index,
				modbus_to_can->m_subindex,
				&modbus_to_can->m_data,
				modbus_to_can->m_size, 200); //200ms timeout
    	//если задание добавлено в очередь
    	if(modbus_to_can_read_queue != NULL) {
    		//установим статус RUN
    		modbus_to_can->status |= MODBUS_TO_CAN_STATUS_RUN;
    	} else {
			//сбросим управляющие биты
			modbus_to_can->control &= ~(MODBUS_TO_CAN_CONTROL_START | MODBUS_TO_CAN_CONTROL_READ);
			//установим статусы ERROR, WRITE_DONE
			modbus_to_can->status |= (MODBUS_TO_CAN_STATUS_ERROR | MODBUS_TO_CAN_STATUS_READ_DONE);
    	}
    } else {
    	//если задание выполнено
		if (modbus_to_can_read_queue->m_state == CO_SDO_CLI_State_DONE) {
			//сбросим управляющие биты
			modbus_to_can->control &= ~(MODBUS_TO_CAN_CONTROL_START | MODBUS_TO_CAN_CONTROL_READ);
			//проверим статус задания
			if (modbus_to_can_read_queue->m_error == CO_SDO_CLI_Error_NONE) {
				//установим статусы VALID, WRITE_DONE
				modbus_to_can->status |= (MODBUS_TO_CAN_STATUS_VALID | MODBUS_TO_CAN_STATUS_READ_DONE);
			} else {
				//установим статусы ERROR, WRITE_DONE
				modbus_to_can->status |= (MODBUS_TO_CAN_STATUS_ERROR | MODBUS_TO_CAN_STATUS_READ_DONE);
			}
			//сбросим статус RUN
			modbus_to_can->status &= ~MODBUS_TO_CAN_STATUS_RUN;
			//сбросим указатель на очередь
			modbus_to_can_read_queue = NULL;
		} else {
			//если таймаут не наступил
			if(modbus_to_can->m_timeout_cnt) {
				modbus_to_can->m_timeout_cnt--;
			} else {
				//принудительно завершим
				modbus_to_can_read_queue->m_state = CO_SDO_CLI_State_DONE;
			}
		}
    }
}

static void modbus_to_can_write(M_modbus_to_can* modbus_to_can) {
	//если очередь не задана
    if(modbus_to_can_write_queue == NULL) {
    	//сбросим статусы VALID, ERROR, WARNING, WRITE_DONE
    	modbus_to_can->status &= ~(MODBUS_TO_CAN_STATUS_VALID |
    			MODBUS_TO_CAN_STATUS_ERROR |
				MODBUS_TO_CAN_STATUS_WARNING |
				MODBUS_TO_CAN_STATUS_WRITE_DONE);
    	//установим таймаут модуля
    	modbus_to_can->m_timeout_cnt = 40; //40 циклов или 400mc
    	//добавим в очередь задание
    	modbus_to_can_write_queue = CO_SDO_CLI_write(
    			&can1_cli_driver,
				modbus_to_can->m_id,
				modbus_to_can->m_index,
				modbus_to_can->m_subindex,
				&modbus_to_can->m_data,
				modbus_to_can->m_size, 200); //200ms timeout
    	//если задание добавлено в очередь
    	if(modbus_to_can_write_queue != NULL) {
    		//установим статус RUN
    		modbus_to_can->status |= MODBUS_TO_CAN_STATUS_RUN;
    	} else {
			//сбросим управляющие биты
			modbus_to_can->control &= ~(MODBUS_TO_CAN_CONTROL_START | MODBUS_TO_CAN_CONTROL_WRITE);
			//установим статусы ERROR, WRITE_DONE
			modbus_to_can->status |= (MODBUS_TO_CAN_STATUS_ERROR | MODBUS_TO_CAN_STATUS_WRITE_DONE);
    	}
    } else {
    	//если задание выполнено
		if (modbus_to_can_write_queue->m_state == CO_SDO_CLI_State_DONE) {
			//сбросим управляющие биты
			modbus_to_can->control &= ~(MODBUS_TO_CAN_CONTROL_START | MODBUS_TO_CAN_CONTROL_WRITE);
			//проверим статус задания
			if (modbus_to_can_write_queue->m_error == CO_SDO_CLI_Error_NONE) {
				//установим статусы VALID, WRITE_DONE
				modbus_to_can->status |= (MODBUS_TO_CAN_STATUS_VALID | MODBUS_TO_CAN_STATUS_WRITE_DONE);
			} else {
				//установим статусы ERROR, WRITE_DONE
				modbus_to_can->status |= (MODBUS_TO_CAN_STATUS_ERROR | MODBUS_TO_CAN_STATUS_WRITE_DONE);
			}
			//сбросим статус RUN
			modbus_to_can->status &= ~MODBUS_TO_CAN_STATUS_RUN;
			//сбросим указатель на очередь
			modbus_to_can_write_queue = NULL;
		} else {
			//если таймаут не наступил
			if(modbus_to_can->m_timeout_cnt) {
				modbus_to_can->m_timeout_cnt--;
			} else {
				//принудительно завершим
				modbus_to_can_write_queue->m_state = CO_SDO_CLI_State_DONE;
			}
		}
    }
}

METHOD_INIT_IMPL(M_modbus_to_can, modbus_to_can)
{
	modbus_to_can->status = MODBUS_TO_CAN_STATUS_NONE;
	modbus_to_can->control = MODBUS_TO_CAN_CONTROL_RESET;
}

METHOD_DEINIT_IMPL(M_modbus_to_can, modbus_to_can)
{

}

METHOD_CALC_IMPL(M_modbus_to_can, modbus_to_can)
{
	if (modbus_to_can->control & MODBUS_TO_CAN_CONTROL_RESET) {
		modbus_to_can_reset(modbus_to_can);
	} else if ((modbus_to_can->control
			& (MODBUS_TO_CAN_CONTROL_ENABLE | MODBUS_TO_CAN_CONTROL_START))
			== (MODBUS_TO_CAN_CONTROL_ENABLE | MODBUS_TO_CAN_CONTROL_START)) {

		if((modbus_to_can->control & MODBUS_TO_CAN_CONTROL_READ) &&
				!(modbus_to_can->control & MODBUS_TO_CAN_CONTROL_WRITE)) {
			modbus_to_can_read(modbus_to_can);
		}

		if ((modbus_to_can->control & MODBUS_TO_CAN_CONTROL_WRITE) &&
				!(modbus_to_can->control & MODBUS_TO_CAN_CONTROL_READ)) {
			modbus_to_can_write(modbus_to_can);
		}
	}
}
