#include "ADL400.h"

/* These must follow the same linear pattern as getFunctionValues() */
typedef enum {
	IO_VOLT_A,
	IO_VOLT_B,
	IO_VOLT_C,
	IO_CURRENT_A,
	IO_CURRENT_B,
	IO_CURRENT_C,
	IO_FREQUENCY,
	IO_VOLT_AB,
	IO_VOLT_AC,
	IO_VOLT_CB,
	IO_TOTAL_ACTIVE_POWER,
	IO_TOTAL_REACTIVE_POWER,
	IO_TOTAL_APPARENT_POWER,
	IO_TOTAL_POWER_FACTOR
}IO;

static int readModbusRegister(const char port[], int registerAddress) {
	uint16_t modbusRegister = 0;
	Tools_Hardware_USB_Protocols_ModbusRTU_readRegister(port, registerAddress, 1, &modbusRegister);
	return modbusRegister;
}

std::vector<TableColumnID> Tools_Communications_Devices_ADL400_getTableColumnIDs() {
	// Only one column definition is allowed.
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Modbus address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;
}

COLUMN_FUNCTION Tools_Communications_Devices_ADL400_getColumnFunction(int functionValueIndex) {
	/* These must follow the same linear pattern as getFunctionValues() */
	switch (functionValueIndex) {
	case IO_VOLT_A:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_VOLT_B:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_VOLT_C:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_CURRENT_A:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_CURRENT_B:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_CURRENT_C:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_FREQUENCY:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_VOLT_AB:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_VOLT_AC:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_VOLT_CB:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_TOTAL_ACTIVE_POWER:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_TOTAL_REACTIVE_POWER:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_TOTAL_APPARENT_POWER:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	case IO_TOTAL_POWER_FACTOR:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS;
	default:
		return COLUMN_FUNCTION_HOLD_DATA;
	}
}

std::string Tools_Communications_Devices_ADL400_getFunctionValues() {
	std::string functionNames;
	functionNames += "Volt A";
	functionNames += '\0';
	functionNames += "Volt B";
	functionNames += '\0';
	functionNames += "Volt C";
	functionNames += '\0';
	functionNames += "Current A";
	functionNames += '\0';
	functionNames += "Current B";
	functionNames += '\0';
	functionNames += "Current C";
	functionNames += '\0';
	functionNames += "Frequency";
	functionNames += '\0';
	functionNames += "Volt AB";
	functionNames += '\0';
	functionNames += "Volt AC";
	functionNames += '\0';
	functionNames += "Volt CB";
	functionNames += '\0';
	functionNames += "Total active power";
	functionNames += '\0';
	functionNames += "Total reactive power";
	functionNames += '\0';
	functionNames += "Total apparent power";
	functionNames += '\0';
	functionNames += "Total power factor";
	functionNames += '\0';
	return functionNames;
}

float Tools_Communications_Devices_ADL400_getInput(const char port[], int functionValueIndex, int address) {
	/* These must follow the same linear pattern as getFunctionValues() */
	Tools_Hardware_USB_Protocols_ModbusRTU_setSlaveAddress(port, address);
	switch (functionValueIndex) {
	case IO_VOLT_A:
		return ((float)readModbusRegister(port, 0x61)) / 10.0f;
	case IO_VOLT_B:
		return ((float)readModbusRegister(port, 0x62)) / 10.0f;
	case IO_VOLT_C:
		return ((float)readModbusRegister(port, 0x63)) / 10.0f;
	case IO_CURRENT_A:
		return ((float)readModbusRegister(port, 0x64)) / 100.0f;
	case IO_CURRENT_B:
		return ((float)readModbusRegister(port, 0x65)) / 100.0f;
	case IO_CURRENT_C:
		return ((float)readModbusRegister(port, 0x66)) / 100.0f;
	case IO_FREQUENCY:
		return ((float)readModbusRegister(port, 0x77)) / 100.0f;
	case IO_VOLT_AB:
		return ((float)readModbusRegister(port, 0x78)) / 10.0f;
	case IO_VOLT_AC:
		return ((float)readModbusRegister(port, 0x79)) / 10.0f;
	case IO_VOLT_CB:
		return ((float)readModbusRegister(port, 0x7A)) / 10.0f;
	case IO_TOTAL_ACTIVE_POWER:
		return ((float)readModbusRegister(port, 0x6A)) / 1000.0f;
	case IO_TOTAL_REACTIVE_POWER:
		return ((float)readModbusRegister(port, 0x6E)) / 1000.0f;
	case IO_TOTAL_APPARENT_POWER:
		return ((float)readModbusRegister(port, 0x72)) / 1000.0f;
	case IO_TOTAL_POWER_FACTOR:
		return ((float)readModbusRegister(port, 0x76)) / 1000.0f;
	default:
		return -1.0f;
	}
}

bool Tools_Communications_Devices_ADL400_setOutput(const char port[], int functionValueIndex, int address, int value) {
	switch (functionValueIndex) {
	default:
		return false; // There are no outputs for ADL400
	}
}