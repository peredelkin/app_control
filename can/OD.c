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
    OD_obj_record_t o_2000_temp[7];
    OD_obj_record_t o_2001_voltage[2];
    OD_obj_record_t o_2002_current[3];
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
.o_2000_temp = {
    {
    .dataOrig = &CO_data.temp.count,
    .subIndex = 0,
    .attribute = ODA_SDO_R,
    .dataLength = 1
    },
    {
    .dataOrig = &ntc_temp.out_temp[0],
    .subIndex = 1,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[1],
    .subIndex = 2,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[2],
    .subIndex = 3,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[3],
    .subIndex = 4,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[4],
    .subIndex = 5,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    },
    {
    .dataOrig = &ntc_temp.out_temp[5],
    .subIndex = 6,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    }
},
.o_2001_voltage = {
    {
    .dataOrig = &CO_data.voltage.count,
    .subIndex = 0,
    .attribute = 0,
    .dataLength = 1
    },
    {
    .dataOrig = &msdi.vcc,
    .subIndex = 1,
    .attribute = ODA_SDO_R | ODA_MB,
    .dataLength = 4
    }
},
.o_2002_current = {
    {
    .dataOrig = &CO_data.current.count,
    .subIndex = 0,
    .attribute = 0,
    .dataLength = 1
    },
    {
    .dataOrig = &ao_dac7562.in_a,
    .subIndex = 1,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
    },
    {
    .dataOrig = &ao_dac7562.in_b,
    .subIndex = 2,
    .attribute = ODA_SDO_RW | ODA_MB,
    .dataLength = 2
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
    {0x2000, 0x07, ODT_REC, &ODObjs.o_2000_temp, NULL},
    {0x2001, 0x02, ODT_REC, &ODObjs.o_2001_voltage, NULL},
    {0x2002, 0x03, ODT_REC, &ODObjs.o_2002_current, NULL},
    {0x0000, 0x00, 0, NULL, NULL}
};

static OD_t _OD = {
    (sizeof(ODList) / sizeof(ODList[0])) - 1,
    &ODList[0]
};

OD_t *OD = &_OD;


