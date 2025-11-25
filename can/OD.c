#define OD_DEFINITION
#include "301/CO_ODinterface.h"
#include "OD.h"

#if CO_VERSION_MAJOR < 4
#error This Object dictionary is compatible with CANopenNode V4.0 and above!
#endif


#include "CO_data.h"

// user code begin
#include "modules/modules.h"
// user code end

// All OD objects (constant definitions)
typedef struct {
    OD_obj_var_t o_1000_deviceType;
    OD_obj_var_t o_1001_errorRegister;
    OD_obj_array_t o_1003_pre_definedErrorField;
    OD_obj_var_t o_1005_COB_ID_SYNCMessage;
    OD_obj_var_t o_1006_communicationCyclePeriod;
    OD_obj_var_t o_1007_synchronousWindowLength;
    OD_obj_array_t o_1010_storeParameters;
    OD_obj_array_t o_1011_restoreDefaultParameters;
    OD_obj_var_t o_1012_COB_IDTimeStampObject;
    OD_obj_var_t o_1014_COB_ID_EMCY;
    OD_obj_var_t o_1015_inhibitTimeEMCY;
    OD_obj_array_t o_1016_consumerHeartbeatTime;
    OD_obj_var_t o_1017_producerHeartbeatTime;
    OD_obj_record_t o_1018_identity[5];
    OD_obj_var_t o_1019_synchronousCounterOverflowValue;
    OD_obj_record_t o_1200_SDOServerParameter[3];
    OD_obj_record_t o_1280_SDOClientParameter[4];
    OD_obj_record_t o_1400_RPDOCommunicationParameter[4];
    OD_obj_record_t o_1401_RPDOCommunicationParameter[4];
    OD_obj_record_t o_1402_RPDOCommunicationParameter[4];
    OD_obj_record_t o_1403_RPDOCommunicationParameter[4];
    OD_obj_record_t o_1600_RPDOMappingParameter[9];
    OD_obj_record_t o_1601_RPDOMappingParameter[9];
    OD_obj_record_t o_1602_RPDOMappingParameter[9];
    OD_obj_record_t o_1603_RPDOMappingParameter[9];
    OD_obj_record_t o_1800_TPDOCommunicationParameter[6];
    OD_obj_record_t o_1801_TPDOCommunicationParameter[6];
    OD_obj_record_t o_1802_TPDOCommunicationParameter[6];
    OD_obj_record_t o_1803_TPDOCommunicationParameter[6];
    OD_obj_record_t o_1A00_TPDOMappingParameter[9];
    OD_obj_record_t o_1A01_TPDOMappingParameter[9];
    OD_obj_record_t o_1A02_TPDOMappingParameter[9];
    OD_obj_record_t o_1A03_TPDOMappingParameter[9];
    OD_obj_record_t o_2000_conf[3];
    OD_obj_record_t o_2010_sys[6];
    OD_obj_record_t o_2020_sys_tim[4];
    OD_obj_record_t o_2030_ms_tim[4];
    OD_obj_record_t o_2040_cli[3];
    OD_obj_record_t o_2050_rgb_led[4];
    OD_obj_record_t o_2060_msdi[14];
    OD_obj_record_t o_2070_ntc_temp[15];
    OD_obj_record_t o_2080_do_relay[4];
    OD_obj_record_t o_2090_do_ncv7608[4];
    OD_obj_record_t o_20A0_digital_in[132];
    OD_obj_record_t o_20B0_digital_out[132];
    OD_obj_record_t o_20C0_mso[35];
    OD_obj_record_t o_20D0_ao_dac7562[5];
    OD_obj_record_t o_20E0_panel_led[4];
    OD_obj_record_t o_20F0_modbus_reg_can[6];
    OD_obj_record_t o_2100_settings[3];
    OD_obj_record_t o_2110_can_tim[4];
} ODObjs_t;

static CO_PROGMEM ODObjs_t ODObjs = {
.o_1000_deviceType = {
    .dataOrig = &CO_data.deviceType.value,
    .attribute = ODA_MB,
    .dataLength = 4
},
.o_1001_errorRegister = {
    .dataOrig = &CO_data.errorRegister.value,
    .attribute = ODA_TPDO,
    .dataLength = 1
},
.o_1003_pre_definedErrorField = {
    .dataOrig0 = &CO_data.pre_definedErrorField.numberOfErrors,
    .dataOrig = &CO_data.pre_definedErrorField.pre_definedErrorField[0],
    .attribute0 = ODA_SDO_RW,
    .attribute = ODA_SDO_R,
    .dataElementLength = 4,
    .dataElementSizeof = sizeof(uint32_t)
},
.o_1005_COB_ID_SYNCMessage = {
    .dataOrig = &CO_data.COB_ID_SYNCMessage.value,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
},
.o_1006_communicationCyclePeriod = {
    .dataOrig = &CO_data.communicationCyclePeriod.value,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
},
.o_1007_synchronousWindowLength = {
    .dataOrig = &CO_data.synchronousWindowLength.value,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
},
.o_1010_storeParameters = {
    .dataOrig0 = &CO_data.storeParameters.highestSub_indexSupported,
    .dataOrig = &CO_data.storeParameters.storeParameters[0],
    .attribute0 = ODA_SDO_R,
    .attribute = ODA_SDO_RW,
    .dataElementLength = 4,
    .dataElementSizeof = sizeof(uint32_t)
},
.o_1011_restoreDefaultParameters = {
    .dataOrig0 = &CO_data.restoreDefaultParameters.highestSub_indexSupported,
    .dataOrig = &CO_data.restoreDefaultParameters.restoreDefaultParameters[0],
    .attribute0 = ODA_SDO_R,
    .attribute = ODA_SDO_RW,
    .dataElementLength = 4,
    .dataElementSizeof = sizeof(uint32_t)
},
.o_1012_COB_IDTimeStampObject = {
    .dataOrig = &CO_data.COB_IDTimeStampObject.value,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
},
.o_1014_COB_ID_EMCY = {
    .dataOrig = &CO_data.COB_ID_EMCY.value,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
},
.o_1015_inhibitTimeEMCY = {
    .dataOrig = &CO_data.inhibitTimeEMCY.value,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
},
.o_1016_consumerHeartbeatTime = {
    .dataOrig0 = &CO_data.consumerHeartbeatTime.highestSub_indexSupported,
    .dataOrig = &CO_data.consumerHeartbeatTime.consumerHeartbeatTime[0],
    .attribute0 = ODA_SDO_R,
    .attribute = ODA_SDO_RW,
    .dataElementLength = 4,
    .dataElementSizeof = sizeof(uint32_t)
},
.o_1017_producerHeartbeatTime = {
    .dataOrig = &CO_data.producerHeartbeatTime.value,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
},
.o_1018_identity = {
    {
    .dataOrig = &CO_data.identity.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.identity.vendor_id,
    .subIndex = 1,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.identity.productCode,
    .subIndex = 2,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.identity.revisionNumber,
    .subIndex = 3,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.identity.serialNumber,
    .subIndex = 4,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    }
},
.o_1019_synchronousCounterOverflowValue = {
    .dataOrig = &CO_data.synchronousCounterOverflowValue.value,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
},
.o_1200_SDOServerParameter = {
    {
    .dataOrig = &CO_data.SDOServerParameter.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.SDOServerParameter.COB_IDClientToServer_rx_,
    .subIndex = 1,
    .attribute = ODA_SDO_R | ODA_TPDO | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.SDOServerParameter.COB_IDServerToClient_tx_,
    .subIndex = 2,
    .attribute = ODA_SDO_R | ODA_TPDO | ODA_MB,
    .dataLength = 4
    }
},
.o_1280_SDOClientParameter = {
    {
    .dataOrig = &CO_data.SDOClientParameter.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.SDOClientParameter.COB_IDClientToServer_tx_,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_TRPDO | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.SDOClientParameter.COB_IDServerToClient_rx_,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_TRPDO | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.SDOClientParameter.node_idOfTheSDOServer,
    .subIndex = 3,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    }
},
.o_1400_RPDOCommunicationParameter = {
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter.COB_IDUsedByRPDO,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter.transmissionType,
    .subIndex = 2,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter.eventTimer,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    }
},
.o_1401_RPDOCommunicationParameter = {
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1401.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1401.COB_IDUsedByRPDO,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1401.transmissionType,
    .subIndex = 2,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1401.eventTimer,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    }
},
.o_1402_RPDOCommunicationParameter = {
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1402.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1402.COB_IDUsedByRPDO,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1402.transmissionType,
    .subIndex = 2,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1402.eventTimer,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    }
},
.o_1403_RPDOCommunicationParameter = {
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1403.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1403.COB_IDUsedByRPDO,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1403.transmissionType,
    .subIndex = 2,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOCommunicationParameter_1403.eventTimer,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    }
},
.o_1600_RPDOMappingParameter = {
    {
    .dataOrig = &CO_data.RPDOMappingParameter.numberOfMappedApplicationObjectsInPDO,
    .subIndex = 0,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter.applicationObject1,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter.applicationObject2,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter.applicationObject3,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter.applicationObject4,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter.applicationObject5,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter.applicationObject6,
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter.applicationObject7,
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter.applicationObject8,
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_1601_RPDOMappingParameter = {
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1601.numberOfMappedApplicationObjectsInPDO,
    .subIndex = 0,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1601.applicationObject1,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1601.applicationObject2,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1601.applicationObject3,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1601.applicationObject4,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1601.applicationObject5,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1601.applicationObject6,
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1601.applicationObject7,
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1601.applicationObject8,
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_1602_RPDOMappingParameter = {
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1602.numberOfMappedApplicationObjectsInPDO,
    .subIndex = 0,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1602.applicationObject1,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1602.applicationObject2,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1602.applicationObject3,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1602.applicationObject4,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1602.applicationObject5,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1602.applicationObject6,
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1602.applicationObject7,
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1602.applicationObject8,
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_1603_RPDOMappingParameter = {
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1603.numberOfMappedApplicationObjectsInPDO,
    .subIndex = 0,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1603.applicationObject1,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1603.applicationObject2,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1603.applicationObject3,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1603.applicationObject4,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1603.applicationObject5,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1603.applicationObject6,
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1603.applicationObject7,
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.RPDOMappingParameter_1603.applicationObject8,
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_1800_TPDOCommunicationParameter = {
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter.COB_IDUsedByTPDO,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter.transmissionType,
    .subIndex = 2,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter.inhibitTime,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter.eventTimer,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter.SYNCStartValue,
    .subIndex = 6,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    }
},
.o_1801_TPDOCommunicationParameter = {
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1801.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1801.COB_IDUsedByTPDO,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1801.transmissionType,
    .subIndex = 2,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1801.inhibitTime,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1801.eventTimer,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1801.SYNCStartValue,
    .subIndex = 6,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    }
},
.o_1802_TPDOCommunicationParameter = {
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1802.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1802.COB_IDUsedByTPDO,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1802.transmissionType,
    .subIndex = 2,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1802.inhibitTime,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1802.eventTimer,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1802.SYNCStartValue,
    .subIndex = 6,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    }
},
.o_1803_TPDOCommunicationParameter = {
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1803.highestSub_indexSupported,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1803.COB_IDUsedByTPDO,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1803.transmissionType,
    .subIndex = 2,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1803.inhibitTime,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1803.eventTimer,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &CO_data.TPDOCommunicationParameter_1803.SYNCStartValue,
    .subIndex = 6,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    }
},
.o_1A00_TPDOMappingParameter = {
    {
    .dataOrig = &CO_data.TPDOMappingParameter.numberOfMappedApplicationObjectsInPDO,
    .subIndex = 0,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter.applicationObject1,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter.applicationObject2,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter.applicationObject3,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter.applicationObject4,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter.applicationObject5,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter.applicationObject6,
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter.applicationObject7,
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter.applicationObject8,
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_1A01_TPDOMappingParameter = {
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a01.numberOfMappedApplicationObjectsInPDO,
    .subIndex = 0,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a01.applicationObject1,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a01.applicationObject2,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a01.applicationObject3,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a01.applicationObject4,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a01.applicationObject5,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a01.applicationObject6,
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a01.applicationObject7,
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a01.applicationObject8,
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_1A02_TPDOMappingParameter = {
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a02.numberOfMappedApplicationObjectsInPDO,
    .subIndex = 0,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a02.applicationObject1,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a02.applicationObject2,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a02.applicationObject3,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a02.applicationObject4,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a02.applicationObject5,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a02.applicationObject6,
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a02.applicationObject7,
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a02.applicationObject8,
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_1A03_TPDOMappingParameter = {
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a03.numberOfMappedApplicationObjectsInPDO,
    .subIndex = 0,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a03.applicationObject1,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a03.applicationObject2,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a03.applicationObject3,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a03.applicationObject4,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a03.applicationObject5,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a03.applicationObject6,
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a03.applicationObject7,
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &CO_data.TPDOMappingParameter_1a03.applicationObject8,
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2000_conf = {
    {
    .dataOrig = &CO_data.conf.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &conf.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &conf.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2010_sys = {
    {
    .dataOrig = &CO_data.sys.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &sys.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &sys.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &sys.errors,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &sys.warnings,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &sys.state,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2020_sys_tim = {
    {
    .dataOrig = &CO_data.sys_tim.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &sys_tim.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &sys_tim.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &sys_tim.out_counter,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2030_ms_tim = {
    {
    .dataOrig = &CO_data.ms_tim.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &ms_tim.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ms_tim.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ms_tim.out_counter,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2040_cli = {
    {
    .dataOrig = &CO_data.cli.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &cli.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &cli.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2050_rgb_led = {
    {
    .dataOrig = &CO_data.rgb_led.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &rgb_led.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &rgb_led.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &rgb_led.in_data,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    }
},
.o_2060_msdi = {
    {
    .dataOrig = &CO_data.msdi.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &msdi.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &msdi.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &msdi.out_digital,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &msdi.out_analog[0],
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &msdi.out_analog[1],
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &msdi.out_analog[2],
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &msdi.out_analog[3],
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &msdi.out_analog[4],
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &msdi.out_analog[5],
    .subIndex = 9,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &msdi.out_analog[6],
    .subIndex = 10,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &msdi.out_analog[7],
    .subIndex = 11,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &msdi.out_ref,
    .subIndex = 12,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &msdi.out_vcc,
    .subIndex = 13,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2070_ntc_temp = {
    {
    .dataOrig = &CO_data.ntc_temp.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &ntc_temp.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[0],
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[1],
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[2],
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[3],
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[4],
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[5],
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_ohm[0],
    .subIndex = 9,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_ohm[1],
    .subIndex = 10,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_ohm[2],
    .subIndex = 11,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_ohm[3],
    .subIndex = 12,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_ohm[4],
    .subIndex = 13,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_ohm[5],
    .subIndex = 14,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2080_do_relay = {
    {
    .dataOrig = &CO_data.do_relay.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &do_relay.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &do_relay.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &do_relay.in_data,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2090_do_ncv7608 = {
    {
    .dataOrig = &CO_data.do_ncv7608.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &do_ncv7608.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &do_ncv7608.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &do_ncv7608.in_data,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_20A0_digital_in = {
    {
    .dataOrig = &CO_data.digital_in.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &digital_in.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &digital_in.out_data,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_TPDO | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &digital_in.p_invert[0],
    .subIndex = 4,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[1],
    .subIndex = 5,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[2],
    .subIndex = 6,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[3],
    .subIndex = 7,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[4],
    .subIndex = 8,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[5],
    .subIndex = 9,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[6],
    .subIndex = 10,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[7],
    .subIndex = 11,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[8],
    .subIndex = 12,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[9],
    .subIndex = 13,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[10],
    .subIndex = 14,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[11],
    .subIndex = 15,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[12],
    .subIndex = 16,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[13],
    .subIndex = 17,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[14],
    .subIndex = 18,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[15],
    .subIndex = 19,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[16],
    .subIndex = 20,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[17],
    .subIndex = 21,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[18],
    .subIndex = 22,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[19],
    .subIndex = 23,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[20],
    .subIndex = 24,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[21],
    .subIndex = 25,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[22],
    .subIndex = 26,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[23],
    .subIndex = 27,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[24],
    .subIndex = 28,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[25],
    .subIndex = 29,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[26],
    .subIndex = 30,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[27],
    .subIndex = 31,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[28],
    .subIndex = 32,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[29],
    .subIndex = 33,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[30],
    .subIndex = 34,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_invert[31],
    .subIndex = 35,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[0],
    .subIndex = 36,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[1],
    .subIndex = 37,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[2],
    .subIndex = 38,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[3],
    .subIndex = 39,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[4],
    .subIndex = 40,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[5],
    .subIndex = 41,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[6],
    .subIndex = 42,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[7],
    .subIndex = 43,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[8],
    .subIndex = 44,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[9],
    .subIndex = 45,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[10],
    .subIndex = 46,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[11],
    .subIndex = 47,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[12],
    .subIndex = 48,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[13],
    .subIndex = 49,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[14],
    .subIndex = 50,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[15],
    .subIndex = 51,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[16],
    .subIndex = 52,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[17],
    .subIndex = 53,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[18],
    .subIndex = 54,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[19],
    .subIndex = 55,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[20],
    .subIndex = 56,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[21],
    .subIndex = 57,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[22],
    .subIndex = 58,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[23],
    .subIndex = 59,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[24],
    .subIndex = 60,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[25],
    .subIndex = 61,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[26],
    .subIndex = 62,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[27],
    .subIndex = 63,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[28],
    .subIndex = 64,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[29],
    .subIndex = 65,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[30],
    .subIndex = 66,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_select[31],
    .subIndex = 67,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[0],
    .subIndex = 68,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[1],
    .subIndex = 69,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[2],
    .subIndex = 70,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[3],
    .subIndex = 71,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[4],
    .subIndex = 72,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[5],
    .subIndex = 73,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[6],
    .subIndex = 74,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[7],
    .subIndex = 75,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[8],
    .subIndex = 76,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[9],
    .subIndex = 77,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[10],
    .subIndex = 78,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[11],
    .subIndex = 79,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[12],
    .subIndex = 80,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[13],
    .subIndex = 81,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[14],
    .subIndex = 82,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[15],
    .subIndex = 83,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[16],
    .subIndex = 84,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[17],
    .subIndex = 85,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[18],
    .subIndex = 86,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[19],
    .subIndex = 87,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[20],
    .subIndex = 88,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[21],
    .subIndex = 89,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[22],
    .subIndex = 90,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[23],
    .subIndex = 91,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[24],
    .subIndex = 92,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[25],
    .subIndex = 93,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[26],
    .subIndex = 94,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[27],
    .subIndex = 95,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[28],
    .subIndex = 96,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[29],
    .subIndex = 97,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[30],
    .subIndex = 98,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_set[31],
    .subIndex = 99,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[0],
    .subIndex = 100,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[1],
    .subIndex = 101,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[2],
    .subIndex = 102,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[3],
    .subIndex = 103,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[4],
    .subIndex = 104,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[5],
    .subIndex = 105,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[6],
    .subIndex = 106,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[7],
    .subIndex = 107,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[8],
    .subIndex = 108,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[9],
    .subIndex = 109,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[10],
    .subIndex = 110,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[11],
    .subIndex = 111,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[12],
    .subIndex = 112,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[13],
    .subIndex = 113,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[14],
    .subIndex = 114,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[15],
    .subIndex = 115,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[16],
    .subIndex = 116,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[17],
    .subIndex = 117,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[18],
    .subIndex = 118,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[19],
    .subIndex = 119,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[20],
    .subIndex = 120,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[21],
    .subIndex = 121,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[22],
    .subIndex = 122,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[23],
    .subIndex = 123,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[24],
    .subIndex = 124,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[25],
    .subIndex = 125,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[26],
    .subIndex = 126,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[27],
    .subIndex = 127,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[28],
    .subIndex = 128,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[29],
    .subIndex = 129,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[30],
    .subIndex = 130,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_in.p_t_reset[31],
    .subIndex = 131,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    }
},
.o_20B0_digital_out = {
    {
    .dataOrig = &CO_data.digital_out.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &digital_out.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &digital_out.in_data,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_RPDO | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &digital_out.p_invert[0],
    .subIndex = 4,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[1],
    .subIndex = 5,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[2],
    .subIndex = 6,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[3],
    .subIndex = 7,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[4],
    .subIndex = 8,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[5],
    .subIndex = 9,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[6],
    .subIndex = 10,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[7],
    .subIndex = 11,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[8],
    .subIndex = 12,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[9],
    .subIndex = 13,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[10],
    .subIndex = 14,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[11],
    .subIndex = 15,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[12],
    .subIndex = 16,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[13],
    .subIndex = 17,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[14],
    .subIndex = 18,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[15],
    .subIndex = 19,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[16],
    .subIndex = 20,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[17],
    .subIndex = 21,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[18],
    .subIndex = 22,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[19],
    .subIndex = 23,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[20],
    .subIndex = 24,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[21],
    .subIndex = 25,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[22],
    .subIndex = 26,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[23],
    .subIndex = 27,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[24],
    .subIndex = 28,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[25],
    .subIndex = 29,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[26],
    .subIndex = 30,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[27],
    .subIndex = 31,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[28],
    .subIndex = 32,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[29],
    .subIndex = 33,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[30],
    .subIndex = 34,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_invert[31],
    .subIndex = 35,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[0],
    .subIndex = 36,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[1],
    .subIndex = 37,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[2],
    .subIndex = 38,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[3],
    .subIndex = 39,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[4],
    .subIndex = 40,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[5],
    .subIndex = 41,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[6],
    .subIndex = 42,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[7],
    .subIndex = 43,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[8],
    .subIndex = 44,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[9],
    .subIndex = 45,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[10],
    .subIndex = 46,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[11],
    .subIndex = 47,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[12],
    .subIndex = 48,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[13],
    .subIndex = 49,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[14],
    .subIndex = 50,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[15],
    .subIndex = 51,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[16],
    .subIndex = 52,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[17],
    .subIndex = 53,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[18],
    .subIndex = 54,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[19],
    .subIndex = 55,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[20],
    .subIndex = 56,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[21],
    .subIndex = 57,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[22],
    .subIndex = 58,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[23],
    .subIndex = 59,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[24],
    .subIndex = 60,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[25],
    .subIndex = 61,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[26],
    .subIndex = 62,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[27],
    .subIndex = 63,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[28],
    .subIndex = 64,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[29],
    .subIndex = 65,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[30],
    .subIndex = 66,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_select[31],
    .subIndex = 67,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[0],
    .subIndex = 68,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[1],
    .subIndex = 69,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[2],
    .subIndex = 70,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[3],
    .subIndex = 71,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[4],
    .subIndex = 72,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[5],
    .subIndex = 73,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[6],
    .subIndex = 74,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[7],
    .subIndex = 75,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[8],
    .subIndex = 76,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[9],
    .subIndex = 77,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[10],
    .subIndex = 78,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[11],
    .subIndex = 79,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[12],
    .subIndex = 80,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[13],
    .subIndex = 81,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[14],
    .subIndex = 82,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[15],
    .subIndex = 83,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[16],
    .subIndex = 84,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[17],
    .subIndex = 85,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[18],
    .subIndex = 86,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[19],
    .subIndex = 87,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[20],
    .subIndex = 88,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[21],
    .subIndex = 89,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[22],
    .subIndex = 90,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[23],
    .subIndex = 91,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[24],
    .subIndex = 92,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[25],
    .subIndex = 93,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[26],
    .subIndex = 94,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[27],
    .subIndex = 95,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[28],
    .subIndex = 96,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[29],
    .subIndex = 97,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[30],
    .subIndex = 98,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_set[31],
    .subIndex = 99,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[0],
    .subIndex = 100,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[1],
    .subIndex = 101,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[2],
    .subIndex = 102,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[3],
    .subIndex = 103,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[4],
    .subIndex = 104,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[5],
    .subIndex = 105,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[6],
    .subIndex = 106,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[7],
    .subIndex = 107,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[8],
    .subIndex = 108,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[9],
    .subIndex = 109,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[10],
    .subIndex = 110,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[11],
    .subIndex = 111,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[12],
    .subIndex = 112,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[13],
    .subIndex = 113,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[14],
    .subIndex = 114,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[15],
    .subIndex = 115,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[16],
    .subIndex = 116,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[17],
    .subIndex = 117,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[18],
    .subIndex = 118,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[19],
    .subIndex = 119,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[20],
    .subIndex = 120,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[21],
    .subIndex = 121,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[22],
    .subIndex = 122,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[23],
    .subIndex = 123,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[24],
    .subIndex = 124,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[25],
    .subIndex = 125,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[26],
    .subIndex = 126,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[27],
    .subIndex = 127,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[28],
    .subIndex = 128,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[29],
    .subIndex = 129,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[30],
    .subIndex = 130,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &digital_out.p_t_reset[31],
    .subIndex = 131,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    }
},
.o_20C0_mso = {
    {
    .dataOrig = &CO_data.mso.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &mso.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[0],
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[1],
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[2],
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[3],
    .subIndex = 6,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[4],
    .subIndex = 7,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[5],
    .subIndex = 8,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[6],
    .subIndex = 9,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[7],
    .subIndex = 10,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[8],
    .subIndex = 11,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[9],
    .subIndex = 12,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[10],
    .subIndex = 13,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[11],
    .subIndex = 14,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[12],
    .subIndex = 15,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[13],
    .subIndex = 16,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[14],
    .subIndex = 17,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[15],
    .subIndex = 18,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[16],
    .subIndex = 19,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[17],
    .subIndex = 20,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[18],
    .subIndex = 21,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[19],
    .subIndex = 22,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[20],
    .subIndex = 23,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[21],
    .subIndex = 24,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[22],
    .subIndex = 25,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[23],
    .subIndex = 26,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[24],
    .subIndex = 27,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[25],
    .subIndex = 28,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[26],
    .subIndex = 29,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[27],
    .subIndex = 30,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[28],
    .subIndex = 31,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[29],
    .subIndex = 32,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[30],
    .subIndex = 33,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &mso.r_channel[31],
    .subIndex = 34,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_20D0_ao_dac7562 = {
    {
    .dataOrig = &CO_data.ao_dac7562.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &ao_dac7562.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ao_dac7562.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ao_dac7562.in_ch_a,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &ao_dac7562.in_ch_b,
    .subIndex = 4,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    }
},
.o_20E0_panel_led = {
    {
    .dataOrig = &CO_data.panel_led.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &panel_led.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &panel_led.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &panel_led.out_data,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    }
},
.o_20F0_modbus_reg_can = {
    {
    .dataOrig = &CO_data.modbus_reg_can.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &modbus_reg_can.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &modbus_reg_can.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &modbus_reg_can.reg_id,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &modbus_reg_can.reg_size,
    .subIndex = 4,
    .attribute = ODA_SDO_RW,
    .dataLength = 1
    },
    {
    .dataOrig = &modbus_reg_can.reg_data,
    .subIndex = 5,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2100_settings = {
    {
    .dataOrig = &CO_data.settings.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &settings.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &settings.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
},
.o_2110_can_tim = {
    {
    .dataOrig = &CO_data.can_tim.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &can_tim.control,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &can_tim.status,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &can_tim.out_counter,
    .subIndex = 3,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 4
    }
}
};
// Object dictionary
static OD_ATTR_OD OD_entry_t ODList[] = {
    {0x1000, 0x01, ODT_VAR, &ODObjs.o_1000_deviceType, NULL},
    {0x1001, 0x01, ODT_VAR, &ODObjs.o_1001_errorRegister, NULL},
    {0x1003, 0x11, ODT_ARR, &ODObjs.o_1003_pre_definedErrorField, NULL},
    {0x1005, 0x01, ODT_VAR, &ODObjs.o_1005_COB_ID_SYNCMessage, NULL},
    {0x1006, 0x01, ODT_VAR, &ODObjs.o_1006_communicationCyclePeriod, NULL},
    {0x1007, 0x01, ODT_VAR, &ODObjs.o_1007_synchronousWindowLength, NULL},
    {0x1010, 0x05, ODT_ARR, &ODObjs.o_1010_storeParameters, NULL},
    {0x1011, 0x05, ODT_ARR, &ODObjs.o_1011_restoreDefaultParameters, NULL},
    {0x1012, 0x01, ODT_VAR, &ODObjs.o_1012_COB_IDTimeStampObject, NULL},
    {0x1014, 0x01, ODT_VAR, &ODObjs.o_1014_COB_ID_EMCY, NULL},
    {0x1015, 0x01, ODT_VAR, &ODObjs.o_1015_inhibitTimeEMCY, NULL},
    {0x1016, 0x09, ODT_ARR, &ODObjs.o_1016_consumerHeartbeatTime, NULL},
    {0x1017, 0x01, ODT_VAR, &ODObjs.o_1017_producerHeartbeatTime, NULL},
    {0x1018, 0x05, ODT_REC, &ODObjs.o_1018_identity, NULL},
    {0x1019, 0x01, ODT_VAR, &ODObjs.o_1019_synchronousCounterOverflowValue, NULL},
    {0x1200, 0x03, ODT_REC, &ODObjs.o_1200_SDOServerParameter, NULL},
    {0x1280, 0x04, ODT_REC, &ODObjs.o_1280_SDOClientParameter, NULL},
    {0x1400, 0x04, ODT_REC, &ODObjs.o_1400_RPDOCommunicationParameter, NULL},
    {0x1401, 0x04, ODT_REC, &ODObjs.o_1401_RPDOCommunicationParameter, NULL},
    {0x1402, 0x04, ODT_REC, &ODObjs.o_1402_RPDOCommunicationParameter, NULL},
    {0x1403, 0x04, ODT_REC, &ODObjs.o_1403_RPDOCommunicationParameter, NULL},
    {0x1600, 0x09, ODT_REC, &ODObjs.o_1600_RPDOMappingParameter, NULL},
    {0x1601, 0x09, ODT_REC, &ODObjs.o_1601_RPDOMappingParameter, NULL},
    {0x1602, 0x09, ODT_REC, &ODObjs.o_1602_RPDOMappingParameter, NULL},
    {0x1603, 0x09, ODT_REC, &ODObjs.o_1603_RPDOMappingParameter, NULL},
    {0x1800, 0x06, ODT_REC, &ODObjs.o_1800_TPDOCommunicationParameter, NULL},
    {0x1801, 0x06, ODT_REC, &ODObjs.o_1801_TPDOCommunicationParameter, NULL},
    {0x1802, 0x06, ODT_REC, &ODObjs.o_1802_TPDOCommunicationParameter, NULL},
    {0x1803, 0x06, ODT_REC, &ODObjs.o_1803_TPDOCommunicationParameter, NULL},
    {0x1a00, 0x09, ODT_REC, &ODObjs.o_1A00_TPDOMappingParameter, NULL},
    {0x1a01, 0x09, ODT_REC, &ODObjs.o_1A01_TPDOMappingParameter, NULL},
    {0x1a02, 0x09, ODT_REC, &ODObjs.o_1A02_TPDOMappingParameter, NULL},
    {0x1a03, 0x09, ODT_REC, &ODObjs.o_1A03_TPDOMappingParameter, NULL},
    {0x2000, 0x03, ODT_REC, &ODObjs.o_2000_conf, NULL},
    {0x2010, 0x06, ODT_REC, &ODObjs.o_2010_sys, NULL},
    {0x2020, 0x04, ODT_REC, &ODObjs.o_2020_sys_tim, NULL},
    {0x2030, 0x04, ODT_REC, &ODObjs.o_2030_ms_tim, NULL},
    {0x2040, 0x03, ODT_REC, &ODObjs.o_2040_cli, NULL},
    {0x2050, 0x04, ODT_REC, &ODObjs.o_2050_rgb_led, NULL},
    {0x2060, 0x0e, ODT_REC, &ODObjs.o_2060_msdi, NULL},
    {0x2070, 0x0f, ODT_REC, &ODObjs.o_2070_ntc_temp, NULL},
    {0x2080, 0x04, ODT_REC, &ODObjs.o_2080_do_relay, NULL},
    {0x2090, 0x04, ODT_REC, &ODObjs.o_2090_do_ncv7608, NULL},
    {0x20a0, 0x84, ODT_REC, &ODObjs.o_20A0_digital_in, NULL},
    {0x20b0, 0x84, ODT_REC, &ODObjs.o_20B0_digital_out, NULL},
    {0x20c0, 0x23, ODT_REC, &ODObjs.o_20C0_mso, NULL},
    {0x20d0, 0x05, ODT_REC, &ODObjs.o_20D0_ao_dac7562, NULL},
    {0x20e0, 0x04, ODT_REC, &ODObjs.o_20E0_panel_led, NULL},
    {0x20f0, 0x06, ODT_REC, &ODObjs.o_20F0_modbus_reg_can, NULL},
    {0x2100, 0x03, ODT_REC, &ODObjs.o_2100_settings, NULL},
    {0x2110, 0x04, ODT_REC, &ODObjs.o_2110_can_tim, NULL},
    {0x0000, 0x00, 0, NULL, NULL}
};

static OD_t _OD = {
    (sizeof(ODList) / sizeof(ODList[0])) - 1,
    &ODList[0]
};

OD_t *OD = &_OD;


