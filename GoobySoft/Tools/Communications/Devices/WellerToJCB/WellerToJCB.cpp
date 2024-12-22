#include "WellerToJBC.h"

/* These must follow the same linear pattern as getFunctionValues() */
typedef enum {
	IO_READ_CURRENT_RAW,
	IO_READ_CURRENT_REAL,
	IO_READ_TEMPERATURE_RAW,
	IO_READ_TEMPERATURE_REAL,
	IO_READ_SETPOINT_RAW,
	IO_READ_SETPOINT_REAL,
	IO_READ_SETPOINT_XHAT0,
	IO_READ_SETPOINT_XHAT1,
}IO;

std::string Tools_Communications_Devices_WellerToJBC_getFunctionValues() {
	std::string functionNames;
	functionNames += "Read Current Raw";
	functionNames += '\0';
	functionNames += "Read Current Real";
	functionNames += '\0';
	functionNames += "Read Temperature Raw";
	functionNames += '\0';
	functionNames += "Read Temperature Real";
	functionNames += '\0';
	functionNames += "Read Setpoint Raw";
	functionNames += '\0';
	functionNames += "Read Setpoint Real";
	functionNames += '\0';
	functionNames += "Read estimated state x0";
	functionNames += '\0';
	functionNames += "Read estimated state x1";
	functionNames += '\0';
	return functionNames;
}
std::vector<TableColumnID> Tools_Communications_Devices_WellerToJBC_getTableColumnIDs() {
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Modbus address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value raw", COLUMN_DEFINITION_MIN_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value raw", COLUMN_DEFINITION_MAX_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value real", COLUMN_DEFINITION_MIN_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value real", COLUMN_DEFINITION_MAX_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;
}
float Tools_Communications_Devices_WellerToJBC_getInput(const char port[], int functionValueIndex, int address) {
	/* These must follow the same linear pattern as getFunctionValues() */
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[2];
	switch (functionValueIndex) {
	case IO_READ_CURRENT_RAW:
		modbus_client_get_parameters(registers, 2, 1);
		return (float)registers[0];
	case IO_READ_CURRENT_REAL:
		modbus_client_get_parameters(registers, 7, 2);
		return uint16_to_float(registers[1], registers[0]);
	case IO_READ_TEMPERATURE_RAW:
		modbus_client_get_parameters(registers, 0, 1);
		return (float) registers[0];
	case IO_READ_TEMPERATURE_REAL:
		modbus_client_get_parameters(registers, 3, 2);
		return uint16_to_float(registers[1], registers[0]);
	case IO_READ_SETPOINT_RAW:
		modbus_client_get_parameters(registers, 1, 1);
		return (float)registers[0];
	case IO_READ_SETPOINT_REAL:
		modbus_client_get_parameters(registers, 5, 2);
		return uint16_to_float(registers[1], registers[0]);
	case IO_READ_SETPOINT_XHAT0:
		modbus_client_get_parameters(registers, 9, 2);
		return uint16_to_float(registers[1], registers[0]);
	case IO_READ_SETPOINT_XHAT1:
		modbus_client_get_parameters(registers, 11, 2);
		return uint16_to_float(registers[1], registers[0]);
	default:
		return -1.0f;
	}
}

bool Tools_Communications_Devices_WellerToJBC_setOutput(const char port[], int functionValueIndex, int address, int value) {
	switch (functionValueIndex) {
	default:
		return false; // There are no outputs for WellerToJCB
	}
}

COLUMN_FUNCTION Tools_Communications_Devices_WellerToJBC_getColumnFunction(int functionValueIndex){
	/* These must follow the same linear pattern as getFunctionValues() */
	switch (functionValueIndex) {
	case IO_READ_CURRENT_RAW:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_READ_CURRENT_REAL:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_TEMPERATURE_RAW:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_READ_TEMPERATURE_REAL:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_SETPOINT_RAW:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_READ_SETPOINT_REAL:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_SETPOINT_XHAT0:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_SETPOINT_XHAT1:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	default:
		return COLUMN_FUNCTION_HOLD_DATA; // Just hold data
	}
}