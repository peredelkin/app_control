/*
 * CO_CLI_driver.c
 *
 *  Created on: 7 февр. 2025 г.
 *      Author: ruslan
 */

#include "CO_CLI_driver.h"

CO_SDO_CLI_Type CO_SDO_CLI_type(CO_SDO_CLI_Queue* prt) {
	return prt->m_type;
}

CO_SDO_CLI_State CO_SDO_CLI_state(CO_SDO_CLI_Queue* prt) {
	return prt->m_state;
}

bool CO_SDO_CLI_process(CO_t *co, uint32_t timeDifference_us, uint32_t* timerNext_us)
{
    if(m_sdoComms.isEmpty()) return false;

    CO_SDO_CLI_Queue* head = NULL; //m_sdoComms.head(); TODO: не забыть убрать заглушку

    size_t size_ret = 0;
    size_t size_to_ret = 0;
    CO_SDOclient_t* sdo_cli = co->SDOclient;
    CO_SDO_return_t sdo_ret = CO_SDO_RT_ok_communicationEnd;
    CO_SDO_abortCode_t sdo_abort_ret = CO_SDO_AB_NONE;

    if(CO_SDO_CLI_type(head) == CO_SDO_CLI_Type_DOWNLOAD){

        switch(CO_SDO_CLI_state(head)){
        case CO_SDO_CLI_State_QUEUED:{
            uint32_t cobidCliToSrv = m_cobidClientToServer + head->nodeId();
            uint32_t cobidSrvToCli = m_cobidServerToClient + head->nodeId();
            sdo_ret = CO_SDOclient_setup(sdo_cli, cobidCliToSrv, cobidSrvToCli, head->nodeId());
            if(sdo_ret != CO_SDO_RT_ok_communicationEnd){
                head->setState(SDOComm::DONE);
                head->setError(SDOComm::ERROR_IO);
                return true;
            }
            head->setState(SDOComm::INIT);
            }

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_INIT:
            sdo_ret = CO_SDOclientDownloadInitiate(sdo_cli,
                            head->index(), head->subIndex(), head->transferSize(),
                            std::min(static_cast<uint>(head->timeout() == 0 ? m_defaultTimeout : head->timeout()),
                                     static_cast<uint>(UINT16_MAX)),
                            m_SDOclientBlockTransfer);
            if(sdo_ret < CO_SDO_RT_ok_communicationEnd){
                head->setState(SDOComm::DONE);
                head->setError(SDOComm::ERROR_IO);
                return true;
            }else if(sdo_ret > CO_SDO_RT_ok_communicationEnd){
                break;
            }
            head->setState(SDOComm::DATA);
            dt = 0;

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_DATA:
            size_ret = CO_SDOclientDownloadBufWrite(sdo_cli,
                            static_cast<uint8_t*>(head->dataToBuffering()), head->dataSizeToBuffering());
            head->dataBuffered(size_ret);
            if(head->dataBufferingDone()){
                head->setState(SDOComm::RUN);
            }

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_RUN:
            sdo_ret = CO_SDOclientDownload(sdo_cli,
                            dt, head->cancelled(), !head->dataBufferingDone(), &sdo_abort_ret,
                            &size_ret, nullptr);
            head->setDataTransfered(size_ret);
            if(sdo_ret == 0){
                if(head->cancelled()){
                    head->setError(SDOComm::ERROR_CANCEL);
                }else{
                    head->setError(SDOComm::ERROR_NONE);
                }
                head->setState(SDOComm::DONE);
            }else if(sdo_ret < 0){
                //qDebug() << size_ret << Qt::hex << sdo_abort_ret;
                SDOComm::Error finish_err = sdoCommError(sdo_abort_ret);
                head->setState(SDOComm::DONE);
                head->setError(finish_err);
            }else{
                break;
            }

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_DONE:
            CO_SDOclientClose(sdo_cli);

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_IDLE:
            m_sdoComms.dequeue();
            head->finish();
            return true;
        }
    }else{ // sdoc->type() == SDOCommunication::UPLOAD

        switch(head->state()){
        case CO_SDO_CLI_State_QUEUED:{
            uint32_t cobidCliToSrv = m_cobidClientToServer + head->nodeId();
            uint32_t cobidSrvToCli = m_cobidServerToClient + head->nodeId();
            sdo_ret = CO_SDOclient_setup(sdo_cli, cobidCliToSrv, cobidSrvToCli, head->nodeId());
            if(sdo_ret != CO_SDO_RT_ok_communicationEnd){
                head->setState(SDOComm::DONE);
                head->setError(SDOComm::ERROR_IO);
                return true;
            }
            head->setState(SDOComm::INIT);
            }

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_INIT:
            sdo_ret = CO_SDOclientUploadInitiate(sdo_cli,
                            head->index(), head->subIndex(),
                            std::min(static_cast<uint>(head->timeout() == 0 ? m_defaultTimeout : head->timeout()),
                                     static_cast<uint>(UINT16_MAX)),
                            m_SDOclientBlockTransfer);
            if(sdo_ret < CO_SDO_RT_ok_communicationEnd){
                head->setState(SDOComm::DONE);
                head->setError(SDOComm::ERROR_IO);
                return true;
            }else if(sdo_ret > CO_SDO_RT_ok_communicationEnd){
                break;
            }
            head->setState(SDOComm::RUN);
            dt = 0;

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_RUN:
            sdo_ret = CO_SDOclientUpload(sdo_cli,
                            dt, head->cancelled(), &sdo_abort_ret,
                            &size_to_ret, &size_ret,
                            nullptr);
            head->setDataBuffered(size_ret);
            if(sdo_ret == 0){
                if(head->cancelled()){
                    head->setState(SDOComm::DONE);
                    head->setError(SDOComm::ERROR_CANCEL);
                    return true;
                }
                head->setState(SDOComm::DATA);
            }else if(sdo_ret < 0){
                //qDebug() << size_ret << Qt::hex << sdo_abort_ret;
                SDOComm::Error finish_err = sdoCommError(sdo_abort_ret);
                head->setState(SDOComm::DONE);
                head->setError(finish_err);
                return true;
            }else{
                if(sdo_ret == CO_SDO_RT_blockUploadInProgress){
                    break;
                }
#if defined(SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH) && SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH == 1
                if(size_to_ret > 0){
                    if(size_to_ret != head->transferSize()){
                        head->setState(SDOCommunication::DONE);
                        head->setError(SDOCommunication::ERROR_INVALID_SIZE);
                        return true;
                    }
                }
#endif
                if(size_ret == 0){
                    break;
                }
            }

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_DATA:
            size_ret = CO_SDOclientUploadBufRead(sdo_cli,
                            static_cast<uint8_t*>(head->dataToTransfer()), head->dataSizeToTransfer());
            head->dataTransfered(size_ret);

            if(head->dataTransferDone()){
                head->setState(SDOComm::DONE);
                head->setError(SDOComm::ERROR_NONE);
            }else{
                if(head->state() == SDOComm::DATA && size_ret == 0){
#if defined(SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH) && SDO_COMM_READ_ERROR_ON_SIZE_MISMATCH == 1
                    head->setState(SDOCommunication::DONE);
                    head->setError(SDOCommunication::ERROR_INVALID_SIZE);
#else
                    head->setState(SDOComm::DONE);
                    head->setError(SDOComm::ERROR_NONE);
#endif
                }else{
                    break;
                }
            }

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_DONE:
            CO_SDOclientClose(sdo_cli);

        __attribute__ ((fallthrough));
        case CO_SDO_CLI_State_IDLE:
            m_sdoComms.dequeue();
            head->finish();
            return true;
        }
    }

    return false;
}
