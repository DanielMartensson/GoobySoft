#include "Uponor-X-148.h"

/* These must follow the same linear pattern as getFunctionValues() */
typedef enum {
	ROOM_TEMPERATURE_DATA_CHANNEL_1
}IO;

static int readModbusRegister(const char port[], int registerAddress) {
	uint16_t modbusRegister = 0;
	Tools_Hardware_USB_Protocols_ModbusRTU_readRegister(port, registerAddress, 1, &modbusRegister);
	return modbusRegister;
}

std::vector<TableColumnID> Tools_Communications_Devices_Uponor_X_148_getTableColumnIDs() {
	// Only one column definition is allowed.
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Modbus address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;
}

COLUMN_FUNCTION Tools_Communications_Devices_Uponor_X_148_getColumnFunction(int functionValueIndex) {
	/* These must follow the same linear pattern as getFunctionValues() */
	switch (functionValueIndex) {
	case ROOM_TEMPERATURE_DATA_CHANNEL_1:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	default:
		return COLUMN_FUNCTION_HOLD_DATA;
	}
}

std::string Tools_Communications_Devices_Uponor_X_148_getFunctionValues() {
	std::string functionNames;
	functionNames += "Read room temperature data channel 1";
	functionNames += '\0';
	return functionNames;
}

float Tools_Communications_Devices_Uponor_X_148_getInput(const char port[], int functionValueIndex, int address) {
	/* These must follow the same linear pattern as getFunctionValues() */
	Tools_Hardware_USB_Protocols_ModbusRTU_setSlaveAddress(port, address);
	switch (functionValueIndex) {
	case ROOM_TEMPERATURE_DATA_CHANNEL_1:
		return ((float)readModbusRegister(port, 30002));
	default:
		return -1.0f;
	}
}

bool Tools_Communications_Devices_Uponor_X_148_setOutput(const char port[], int functionValueIndex, int address, int value) {
	switch (functionValueIndex) {
	default:
		return false; // There are no outputs for ADL400
	}
}