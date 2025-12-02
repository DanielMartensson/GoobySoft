#include "ISO11783_ControlValve.h"

/* These must follow the same linear pattern as getFunctionValues() */
typedef enum {
	CAN_WRITE_AUXILIARY_VALVE_COMMAND_0,
	CAN_WRITE_AUXILIARY_VALVE_COMMAND_1,
	CAN_WRITE_AUXILIARY_VALVE_COMMAND_2,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_3,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_4,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_5,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_6,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_7,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_8,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_9,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_10,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_11,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_12,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_13,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_14,
    CAN_WRITE_AUXILIARY_VALVE_COMMAND_15,
	CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_0,
	CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_1,
	CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_2,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_3,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_4,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_5,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_6,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_7,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_8,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_9,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_10,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_11,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_12,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_13,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_14,
    CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_15,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_0,
	CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_1,
	CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_2,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_3,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_4,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_5,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_6,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_7,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_8,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_9,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_10,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_11,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_12,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_13,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_14,
    CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_15,
    CAN_WRITE_GENERAL_PURPOSE_VALVE_COMMAND,
    CAN_READ_GENERAL_PURPOSE_ESTIMATED_FLOW
}IO;

std::string Tools_Communications_Devices_ISO11783_ControlValve_getFunctionValues() {
    std::string functionNames;

    // Write Auxiliary Valve Command 0–15
    for (int i = 0; i < 16; i++) {
        functionNames += "Write Auxiliary Valve Command " + std::to_string(i);
        functionNames += '\0';
    }

    // Read Auxiliary Valve Estimated Flow 0–15
    for (int i = 0; i < 16; i++) {
        functionNames += "Read Auxiliary Valve Estimated Flow " + std::to_string(i);
        functionNames += '\0';
    }

    // Read Auxiliary Valve Measured Position 0–15
    for (int i = 0; i < 16; i++) {
        functionNames += "Read Auxiliary Valve Measured Position " + std::to_string(i);
        functionNames += '\0';
    }

    // Last two
    functionNames += "Write General Purpose Valve Command";
    functionNames += '\0';
    functionNames += "Read General Purpose Estimated Flow";
    functionNames += '\0';

    return functionNames;
}

std::vector<TableColumnID> Tools_Communications_Devices_ISO11783_ControlValve_getTableColumnIDs() {
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
    tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value", COLUMN_DEFINITION_MIN_VALUE_RAW));
    tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value", COLUMN_DEFINITION_MAX_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Destination address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;
}

float Tools_Communications_Devices_ISO11783_ControlValve_getInput(const char port[], int functionValueIndex, int address) {
	/* These must follow the same linear pattern as getFunctionValues() */

    // Get the SAE J1939 struct pointer
    J1939* j1939 = Tools_Hardware_ParameterStore_getJ1939();
    if(j1939 == nullptr){
        return -1.0f;
    }

    if(functionValueIndex >= CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_0 && functionValueIndex <= CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_15){
        int index = functionValueIndex - CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_0;
        ENUM_J1939_STATUS_CODES status = ISO_11783_Send_Request_Auxiliary_Valve_Estimated_Flow(j1939, address, index);
        if(status != STATUS_SEND_OK){
            return -1.0f;
        }
        uint8_t valveState = j1939->from_other_ecu_auxiliary_valve_estimated_flow[index].valve_state;
        switch(valveState){
            case VALVE_STATE_ERROR:
            return -1.0f;
            case VALVE_STATE_INITIALISATION:
            case VALVE_STATE_NEUTRAL:
            return 0.0f;
            case VALVE_STATE_EXTEND:
            return (float) j1939->from_other_ecu_auxiliary_valve_estimated_flow[index].extend_estimated_flow_standard;
            case VALVE_STATE_RETRACT:
            return -((float) j1939->from_other_ecu_auxiliary_valve_estimated_flow[index].retract_estimated_flow_standard);
            default:
            return -1.0f;
        }
    }else if(functionValueIndex >= CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_0 && functionValueIndex <= CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_15){
        int index = functionValueIndex - CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_0;
        ENUM_J1939_STATUS_CODES status = ISO_11783_Send_Request_Auxiliary_Valve_Measured_Position(j1939, address, index);
        if(status != STATUS_SEND_OK){
            return -1.0f;
        }
        uint8_t valveState = j1939->from_other_ecu_auxiliary_valve_measured_position[index].valve_state;
        switch(valveState){
            case VALVE_STATE_ERROR:
            return -1.0f;
            case VALVE_STATE_INITIALISATION:
            case VALVE_STATE_NEUTRAL:
            return 0.0f;
            case VALVE_STATE_EXTEND:
            return (float) j1939->from_other_ecu_auxiliary_valve_measured_position->measured_position_percent;
            case VALVE_STATE_RETRACT:
            return -((float) j1939->from_other_ecu_auxiliary_valve_measured_position[index].measured_position_percent);
            default:
            return -1.0f;
        }
    }else if(functionValueIndex == CAN_READ_GENERAL_PURPOSE_ESTIMATED_FLOW){
        int index = functionValueIndex - CAN_READ_GENERAL_PURPOSE_ESTIMATED_FLOW;
        ENUM_J1939_STATUS_CODES status = ISO_11783_Send_Request_General_Purpose_Valve_Estimated_Flow(j1939, address);
        if(status != STATUS_SEND_OK){
            return -1.0f;
        }
        uint8_t valveState = j1939->from_other_ecu_general_purpose_valve_estimated_flow.valve_state;
        switch(valveState){
            case VALVE_STATE_ERROR:
            return -1.0f;
            case VALVE_STATE_INITIALISATION:
            case VALVE_STATE_NEUTRAL:
            return 0.0f;
            case VALVE_STATE_EXTEND:
            return (float) j1939->from_other_ecu_general_purpose_valve_estimated_flow.extend_estimated_flow_standard;
            case VALVE_STATE_RETRACT:
            return -((float) j1939->from_other_ecu_general_purpose_valve_estimated_flow.retract_estimated_flow_standard);
            default:
            return -1.0f;
        }
    }else{
        return -1.0f;
    }
}

bool Tools_Communications_Devices_ISO11783_ControlValve_setOutput(const char port[], int functionValueIndex, int address, int value) {
	/* These must follow the same linear pattern as getFunctionValues() */

    // Get the SAE J1939 struct pointer
    J1939* j1939 = Tools_Hardware_ParameterStore_getJ1939();
    if(j1939 == nullptr){
        return false;
    }

    if(functionValueIndex >= CAN_WRITE_AUXILIARY_VALVE_COMMAND_0 && functionValueIndex <= CAN_WRITE_AUXILIARY_VALVE_COMMAND_15){
        int index = functionValueIndex - CAN_WRITE_AUXILIARY_VALVE_COMMAND_0;
        uint8_t valve_state = VALVE_STATE_INITIALISATION;
        if(value > 0){
            valve_state = VALVE_STATE_EXTEND;
        }else if(value < 0){
            valve_state = VALVE_STATE_RETRACT;
        }
        ENUM_J1939_STATUS_CODES status = ISO_11783_Send_Auxiliary_Valve_Command(j1939, index, value, FAIL_SAFE_MODE_BLOCKED, valve_state);
        return status == STATUS_SEND_OK;
    }else if(functionValueIndex == CAN_WRITE_GENERAL_PURPOSE_VALVE_COMMAND){
        uint8_t valve_state = VALVE_STATE_INITIALISATION;
        if(value > 0){
            valve_state = VALVE_STATE_EXTEND;
        }else if(value < 0){
            valve_state = VALVE_STATE_RETRACT;
        }
        ENUM_J1939_STATUS_CODES status = ISO_11783_Send_General_Purpose_Valve_Command(j1939, address, value, FAIL_SAFE_MODE_BLOCKED, valve_state, 0);   
        return status == STATUS_SEND_OK;
    }else{
        return false;
    }
}

COLUMN_FUNCTION Tools_Communications_Devices_ISO11783_ControlValve_getColumnFunction(int functionValueIndex){
	/* These must follow the same linear pattern as getFunctionValues() */
    if(functionValueIndex >= CAN_WRITE_AUXILIARY_VALVE_COMMAND_0 && functionValueIndex <= CAN_WRITE_AUXILIARY_VALVE_COMMAND_15){
        return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
    }else if(functionValueIndex >= CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_0 && functionValueIndex <= CAN_READ_AUXILIARY_VALVE_ESTIMATED_FLOW_15){
        return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
    }else if(functionValueIndex >= CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_0 && functionValueIndex <= CAN_READ_AUXILIARY_VALVE_MEASURED_POSITION_15){
        return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
    }else if(functionValueIndex == CAN_WRITE_GENERAL_PURPOSE_VALVE_COMMAND){
        return COLUMN_FUNCTION_OUTPUT_ACTUATOR_ADDRESS;
    }else if(functionValueIndex == CAN_READ_GENERAL_PURPOSE_ESTIMATED_FLOW){
        return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
    }else{
        return COLUMN_FUNCTION_HOLD_DATA;
    }
}
